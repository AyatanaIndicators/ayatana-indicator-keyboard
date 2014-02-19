/*
 * Copyright 2014 Canonical Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors: William Hua <william.hua@canonical.com>
 */

public class Indicator.Keyboard.IBusMenu : MenuModel {

	private static uint radio_counter = 0;

	private IBus.PropList? properties;

	private Menu menu;
	private SimpleActionGroup? action_group;

	private string? radio_name;
	private SimpleAction? radio_action;
	private Gee.HashMap<string, IBus.Property> radio_properties;

	private Gee.LinkedList<string> names;

	public IBusMenu (SimpleActionGroup? action_group = null, IBus.PropList? properties = null) {
		menu = new Menu ();

		menu.items_changed.connect ((position, removed, added) => {
			items_changed (position, removed, added);
		});

		names = new Gee.LinkedList<string> ();
		set_action_group (action_group);
		set_properties (properties);
	}

	~IBusMenu () {
		remove_actions ();
	}

	public signal void activate (IBus.Property property, IBus.PropState state);

	private string get_action_name (string key) {
		if (!action_name_is_valid (key)) {
			var builder = new StringBuilder.sized (key.length + 1);

			unichar letter = 0;
			int index = 0;

			while (key.get_next_char (ref index, out letter)) {
				if (letter == '-' || letter == '.' || letter.isalnum ()) {
					builder.append_unichar (letter);
				} else {
					builder.append_c ('-');
				}
			}

			return builder.str;
		} else {
			return key;
		}
	}

	private string? get_label (IBus.Property property) {
		string? label = null;

		if ((IBus.Text?) property.label != null) {
			label = property.label.text;
		}

		if (label == null && (IBus.Text?) property.symbol != null) {
			label = property.symbol.text;
		}

		return label;
	}

	private void append_normal_property (IBus.Property property) {
		if (property.prop_type == IBus.PropType.NORMAL) {
			if ((string?) property.key != null) {
				var name = get_action_name (property.key);

				if (action_group != null) {
					var action = new SimpleAction (name, null);
					action.activate.connect ((parameter) => { activate (property, property.state); });
					((!) action_group).add_action (action);
					names.add (name);
				}

				menu.append (get_label (property), @"indicator.$name");
			}
		}
	}

	private void append_toggle_property (IBus.Property property) {
		if (property.prop_type == IBus.PropType.TOGGLE) {
			if ((string?) property.key != null) {
				var name = get_action_name (property.key);

				if (action_group != null) {
					var state = new Variant.boolean (property.state == IBus.PropState.CHECKED);
					var action = new SimpleAction.stateful (name, null, state);

					action.activate.connect ((parameter) => {
						action.change_state (new Variant.boolean (!action.get_state ().get_boolean ()));
					});

					action.change_state.connect ((value) => {
						if (value != null) {
							activate (property, ((!) value).get_boolean () ? IBus.PropState.CHECKED : IBus.PropState.UNCHECKED);
						}
					});

					((!) action_group).add_action (action);
					names.add (name);
				}

				menu.append (get_label (property), @"indicator.$name");
			}
		}
	}

	private void append_radio_property (IBus.Property property) {
		if (property.prop_type == IBus.PropType.RADIO) {
			if ((string?) property.key != null) {
				if (action_group != null && radio_name == null) {
					radio_counter++;
					radio_name = @"-private-$radio_counter";
					radio_action = new SimpleAction.stateful ((!) radio_name, VariantType.STRING, new Variant.string (""));

					((!) radio_action).activate.connect ((parameter) => {
						((!) radio_action).change_state (parameter);
					});

					((!) radio_action).change_state.connect ((value) => {
						if (value != null) {
							var key = ((!) value).get_string ();

							if (radio_properties.has_key (key)) {
								activate (radio_properties[key], IBus.PropState.CHECKED);
							}
						}
					});

					((!) action_group).add_action ((!) radio_action);
					names.add ((!) radio_name);
				}

				radio_properties[property.key] = property;

				if (property.state == IBus.PropState.CHECKED) {
					((!) radio_action).change_state (new Variant.string (property.key));
				}

				var item = new MenuItem (get_label (property), null);
				item.set_action_and_target_value (@"indicator.$((!) radio_name)", new Variant.string (property.key));
				menu.append_item (item);
			}
		}
	}

	private void append_menu_property (IBus.Property property) {
		if (property.prop_type == IBus.PropType.MENU) {
			var submenu = new IBusMenu (action_group, ((!) property).sub_props);
			submenu.activate.connect ((property, state) => { activate (property, state); });
			menu.append_submenu (get_label (property), submenu);
		}
	}

	private void append_property (IBus.Property? property) {
		if (property != null && ((!) property).visible) {
			switch (((!) property).prop_type) {
			case IBus.PropType.NORMAL:
				append_normal_property ((!) property);
				break;

			case IBus.PropType.TOGGLE:
				append_toggle_property ((!) property);
				break;

			case IBus.PropType.RADIO:
				append_radio_property ((!) property);
				break;

			case IBus.PropType.MENU:
				append_menu_property ((!) property);
				break;

			case IBus.PropType.SEPARATOR:
				break;
			}
		}
	}

	private void update_menu () {
		menu.remove_all ();

		if (properties != null) {
			for (var i = 0; i < ((!) properties).properties.length; i++) {
				append_property (((!) properties).get (i));
			}
		}
	}

	private void remove_actions () {
		radio_action = null;
		radio_name = null;

		if (action_group != null) {
			foreach (var name in names) {
				((!) action_group).remove_action (name);
			}
		}

		names.clear ();
	}

	public void set_action_group (SimpleActionGroup? action_group) {
		if (action_group != this.action_group) {
			remove_actions ();
			this.action_group = action_group;
			update_menu ();
		}
	}

	public void set_properties (IBus.PropList? properties) {
		if (properties != this.properties) {
			remove_actions ();
			radio_properties = new Gee.HashMap<string, IBus.Property> ();
			this.properties = properties;
			update_menu ();
		}
	}

	public override Variant get_item_attribute_value (int item_index, string attribute, VariantType? expected_type) {
		return menu.get_item_attribute_value (item_index, attribute, expected_type);
	}

	public override void get_item_attributes (int item_index, out HashTable<string, Variant>? attributes) {
		menu.get_item_attributes (item_index, out attributes);
	}

	public override MenuModel get_item_link (int item_index, string link) {
		return menu.get_item_link (item_index, link);
	}

	public override void get_item_links (int item_index, out HashTable<string, MenuModel>? links) {
		menu.get_item_links (item_index, out links);
	}

	public override int get_n_items () {
		return menu.get_n_items ();
	}

	public override bool is_mutable () {
		return menu.is_mutable ();
	}

	public override MenuAttributeIter iterate_item_attributes (int item_index) {
		return menu.iterate_item_attributes (item_index);
	}

	public override MenuLinkIter iterate_item_links (int item_index) {
		return menu.iterate_item_links (item_index);
	}
}
