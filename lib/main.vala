/*
 * Copyright 2013 Canonical Ltd.
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

[DBus (name = "com.canonical.indicator.keyboard")]
public class Indicator.Keyboard.Service : Object {

	private static Service service;
	private static IBus.Bus? ibus;

	private bool force;
	private bool use_gtk;
	private bool use_bamf;

	private MainLoop? loop;
	private Settings indicator_settings;
	private Settings source_settings;
	private Settings per_window_settings;
	private SList<Act.User> users;
	private Bamf.Matcher? matcher;
	private Gee.HashMap<string, uint>? window_sources;

	private Source[]? sources;

	private SimpleActionGroup? action_group;
	private SimpleAction? indicator_action;
	private MenuModel? menu_model;
	private Menu? sources_menu;

	private Greeter? greeter;
	private string? greeter_user;
	private uint lightdm_current;

	[DBus (visible = false)]
	public Service (ref unowned string[] args) {
		force = "--force" in args;
		use_gtk = "--use-gtk" in args;
		use_bamf = "--use-bamf" in args;

		if (use_gtk) {
			use_gtk = Gtk.init_check (ref args);
		} else {
			Gdk.init (ref args);
		}

		if (is_login_user ()) {
			var name = Environment.get_variable ("UNITY_GREETER_DBUS_NAME");

			if (name != null) {
				Bus.watch_name (BusType.SESSION,
				                (!) name,
				                BusNameWatcherFlags.NONE,
				                handle_name_appeared,
				                handle_name_vanished);
			}
		}

		indicator_settings = new Settings ("com.canonical.indicator.keyboard");
		indicator_settings.changed["visible"].connect (handle_changed_visible);

		source_settings = new Settings ("org.gnome.desktop.input-sources");
		source_settings.changed["current"].connect (handle_changed_current);
		source_settings.changed["sources"].connect (handle_changed_sources);

		per_window_settings = new Settings ("org.gnome.libgnomekbd.desktop");
		per_window_settings.changed["group-per-window"].connect (handle_changed_group_per_window);

		migrate_keyboard_layouts ();
		update_window_sources ();
		acquire_bus_name ();
	}

	[DBus (visible = false)]
	private static bool is_login_user () {
		return Environment.get_user_name () == "lightdm";
	}

	[DBus (visible = false)]
	private static IBus.Bus get_ibus () {
		if (ibus == null) {
			IBus.init ();
			ibus = new IBus.Bus ();
		}

		return (!) ibus;
	}

	[DBus (visible = false)]
	public void up () {
		if (loop == null) {
			loop = new MainLoop ();
			((!) loop).run ();
		}
	}

	[DBus (visible = false)]
	public void down () {
		if (loop != null) {
			((!) loop).quit ();
			loop = null;
		}
	}

	[DBus (visible = false)]
	private void acquire_bus_name () {
		Bus.own_name (BusType.SESSION,
		              "com.canonical.indicator.keyboard",
		              BusNameOwnerFlags.ALLOW_REPLACEMENT | (force ? BusNameOwnerFlags.REPLACE : 0),
		              handle_bus_acquired,
		              null,
		              handle_name_lost);
	}

	[DBus (visible = false)]
	private void update_greeter_user () {
		if (greeter_user == null && greeter != null) {
			try {
				greeter_user = ((!) greeter).get_active_entry ();
			} catch (IOError error) {
				warning ("error: %s", error.message);
			}
		}

		string? source = null;

		if (greeter_user != null) {
			var manager = Act.UserManager.get_default ();

			if (manager.is_loaded) {
				Act.User? user = manager.get_user ((!) greeter_user);

				if (user != null && ((!) user).is_loaded) {
					VariantIter outer;
					VariantIter inner;

					var sources = ((!) user).input_sources;
					sources.get ("aa{ss}", out outer);

					while (outer.next ("a{ss}", out inner)) {
						unowned string key;
						unowned string value;

						while (inner.next ("{&s&s}", out key, out value)) {
							if (key == "xkb") {
								source = value;
								break;
							}
						}

						if (source != null) {
							break;
						}
					}

					if (source == null) {
						var layouts = ((!) user).xkeyboard_layouts;

						if (layouts.length <= 0) {
							var user_list = LightDM.UserList.get_instance ();
							LightDM.User? light_user = user_list.get_user_by_name ((!) greeter_user);

							if (light_user != null) {
								layouts = ((!) light_user).get_layouts ();
							}
						}

						if (layouts.length > 0) {
							source = layouts[0];
							source = ((!) source).replace (" ", "+");
							source = ((!) source).replace ("\t", "+");
						}
					}
				}
			}
		}

		if (source == null) {
			LightDM.Layout? layout = LightDM.get_layout ();

			if (layout != null) {
				source = ((!) layout).name;

				if (source != null) {
					source = ((!) source).replace (" ", "+");
					source = ((!) source).replace ("\t", "+");
				}
			}
		}

		if (source != null) {
			var array = source_settings.get_value ("sources");

			for (int i = 0; i < array.n_children (); i++) {
				unowned string type;
				unowned string name;

				array.get_child (i, "(&s&s)", out type, out name);

				if (type == "xkb" && name == (!) source) {
					source_settings.set_uint ("current", i);
					break;
				}
			}
		}
	}

	[DBus (visible = false)]
	private void handle_entry_selected (string entry_name) {
		if (greeter_user == null || entry_name != (!) greeter_user) {
			greeter_user = entry_name;

			update_greeter_user ();
		}
	}

	[DBus (visible = false)]
	private void migrate_keyboard_layouts () {
		if (is_login_user ()) {
			lightdm_current = source_settings.get_uint ("current");

			var manager = Act.UserManager.get_default ();

			if (manager.is_loaded) {
				users = manager.list_users ();

				foreach (var user in users) {
					if (user.is_loaded) {
						migrate_input_sources ();
					} else {
						user.notify["is-loaded"].connect ((pspec) => {
							if (user.is_loaded) {
								migrate_input_sources ();
							}
						});
					}
				}
			} else {
				manager.notify["is-loaded"].connect ((pspec) => {
					if (manager.is_loaded) {
						users = manager.list_users ();

						foreach (var user in users) {
							if (user.is_loaded) {
								migrate_input_sources ();
							} else {
								user.notify["is-loaded"].connect ((pspec) => {
									if (user.is_loaded) {
										migrate_input_sources ();
									}
								});
							}
						}
					}
				});
			}

			var user_list = LightDM.UserList.get_instance ();

			user_list.user_added.connect ((user) => { migrate_input_sources (); });
			user_list.user_changed.connect ((user) => { migrate_input_sources (); });
			user_list.user_removed.connect ((user) => { migrate_input_sources (); });

			/* Force the loading of the user list. */
			user_list.get_user_by_name ("");
		} else {
			if (!indicator_settings.get_boolean ("migrated")) {
				var builder = new VariantBuilder (new VariantType ("a(ss)"));
				var length = 0;

				var layout_settings = new Settings ("org.gnome.libgnomekbd.keyboard");
				var layouts = layout_settings.get_strv ("layouts");

				foreach (var layout in layouts) {
					var source = layout;
					source = source.replace (" ", "+");
					source = source.replace ("\t", "+");

					builder.add ("(ss)", "xkb", source);
					length++;
				}

				var engines = get_ibus ().list_active_engines ();

				foreach (var engine in engines) {
					if (length == 0 || engine.name.has_prefix ("xkb")) {
						var source = "us";
						string? layout = engine.get_layout ();
						string? variant = engine.get_layout_variant ();

						if (layout != null && ((!) layout).length == 0) {
							layout = null;
						}

						if (variant != null && ((!) variant).length == 0) {
							variant = null;
						}

						if (layout != null && variant != null) {
							source = @"$((!) layout)+$((!) variant)";
						} else if (layout != null) {
							source = (!) layout;
						}

						builder.add ("(ss)", "xkb", source);
						length++;
					}

					if (!engine.name.has_prefix ("xkb")) {
						builder.add ("(ss)", "ibus", engine.name);
						length++;
					}
				}

				source_settings.set_value ("sources", builder.end ());
				indicator_settings.set_boolean ("migrated", true);
			}
		}
	}

	[DBus (visible = false)]
	private void migrate_input_sources () {
		var list = new Gee.LinkedList<string> ();
		var added = new Gee.HashSet<string> ();

		foreach (var user in users) {
			if (user.is_loaded) {
				var done = false;

				VariantIter outer;
				VariantIter inner;

				var sources = user.input_sources;
				sources.get ("aa{ss}", out outer);

				while (outer.next ("a{ss}", out inner)) {
					unowned string key;
					unowned string source;

					while (inner.next ("{&s&s}", out key, out source)) {
						if (key == "xkb") {
							done = true;

							if (!added.contains (source)) {
								list.add (source);
								added.add (source);
							}
						}
					}
				}

				if (!done) {
					var layouts = user.xkeyboard_layouts;
					foreach (var layout in layouts) {
						done = true;

						var source = layout;
						source = source.replace (" ", "+");
						source = source.replace ("\t", "+");

						if (!added.contains (source)) {
							list.add (source);
							added.add (source);
						}
					}
				}

				if (!done) {
					var user_list = LightDM.UserList.get_instance ();
					LightDM.User? light_user = user_list.get_user_by_name (user.user_name);

					if (light_user != null) {
						var layouts = ((!) light_user).get_layouts ();
						foreach (var layout in layouts) {
							done = true;

							var source = layout;
							source = source.replace (" ", "+");
							source = source.replace ("\t", "+");

							if (!added.contains (source)) {
								list.add (source);
								added.add (source);
							}
						}
					}
				}
			}
		}

		LightDM.Layout? layout = LightDM.get_layout ();

		if (layout != null) {
			string? source = ((!) layout).name;

			if (source != null) {
				source = ((!) source).replace (" ", "+");
				source = ((!) source).replace ("\t", "+");

				if (!added.contains ((!) source)) {
					list.add ((!) source);
					added.add ((!) source);
				}
			}
		}

		var builder = new VariantBuilder (new VariantType ("a(ss)"));

		foreach (var name in list) {
			builder.add ("(ss)", "xkb", name);
		}

		if (lightdm_current < list.size) {
			source_settings.set_uint ("current", lightdm_current);
		} else {
			source_settings.set_uint ("current", list.size - 1);
		}

		source_settings.set_value ("sources", builder.end ());

		update_greeter_user ();
	}

	[DBus (visible = false)]
	private void update_login_layout () {
		unowned List<LightDM.Layout> layouts = LightDM.get_layouts ();
		var current = source_settings.get_uint ("current");

		if (current < get_sources ().length) {
			var source = get_sources ()[current];
			string? name = null;

			if (source.layout != null && source.variant != null) {
				name = @"$((!) source.layout)\t$((!) source.variant)";
			} else if (source.layout != null) {
				name = source.layout;
			}

			if (name != null) {
				foreach (var layout in layouts) {
					if (layout.name == (!) name) {
						LightDM.set_layout (layout);
						break;
					}
				}
			}
		}
	}

	[DBus (visible = false)]
	private void update_window_sources () {
		if (use_bamf && !is_login_user ()) {
			var group_per_window = per_window_settings.get_boolean ("group-per-window");

			if (group_per_window != (window_sources != null)) {
				if (group_per_window) {
					window_sources = new Gee.HashMap<string, uint> ();
					matcher = Bamf.Matcher.get_default ();
					((!) matcher).active_window_changed.connect (handle_active_window_changed);
				} else {
					((!) matcher).active_window_changed.disconnect (handle_active_window_changed);
					matcher = null;
					window_sources = null;
				}
			}
		}
	}

	[DBus (visible = false)]
	private void handle_changed_group_per_window (string key) {
		update_window_sources ();
	}

	[DBus (visible = false)]
	private void handle_active_window_changed (Bamf.View? old_view, Bamf.View? new_view) {
		if (old_view != null) {
			((!) window_sources)[((!) old_view).path] = source_settings.get_uint ("current");
		}

		if (new_view != null) {
			if (!((!) window_sources).has_key (((!) new_view).path)) {
				var default_group = per_window_settings.get_int ("default-group");

				if (default_group >= 0) {
					source_settings.set_uint ("current", (uint) default_group);
				}
			} else {
				source_settings.set_uint ("current", ((!) window_sources)[((!) new_view).path]);
			}
		}
	}

	[DBus (visible = false)]
	private Source[] get_sources () {
		if (sources == null) {
			var array = source_settings.get_value ("sources");

			sources = new Source[array.n_children ()];

			for (var i = 0; i < ((!) sources).length; i++) {
				sources[i] = new Source(array.get_child_value (i), use_gtk);
				sources[i].show_subscript = false;
				sources[i].subscript = 1;

				for (var j = (int) i - 1; j >= 0; j--) {
					if ((!) sources[j].short_name == (!) sources[i].short_name) {
						sources[i].subscript = sources[j].subscript + 1;
						sources[i].show_subscript = true;
						sources[j].show_subscript = true;

						break;
					}
				}
			}
		}

		return (!) sources;
	}

	[DBus (visible = false)]
	private void update_indicator_action () {
		var visible = indicator_settings.get_boolean ("visible");
		var current = source_settings.get_uint ("current");
		var sources = get_sources ();

		Icon? icon = null;
		string? name = null;

		if (current < sources.length) {
			icon = sources[current].icon;
			name = sources[current].name;
		}

		var builder = new VariantBuilder (new VariantType ("a{sv}"));
		builder.add ("{sv}", "visible", new Variant.boolean (visible));
		if (name != null) {
			var description = _ ("%s input source").printf ((!) name);
			builder.add ("{sv}", "accessible-desc", new Variant.string (description));
		}
		if (icon != null) {
			builder.add ("{sv}", "icon", ((!) icon).serialize ());
		}

		get_indicator_action ().set_state (builder.end ());
	}

	[DBus (visible = false)]
	private SimpleAction get_indicator_action () {
		if (indicator_action == null) {
			var state = new Variant.parsed ("{ 'visible' : <false> }");
			indicator_action = new SimpleAction.stateful ("indicator", null, state);
			update_indicator_action ();
		}

		return (!) indicator_action;
	}

	[DBus (visible = false)]
	private void handle_middle_click (Variant? parameter) {
		handle_scroll_wheel (new Variant.int32 (-1));
	}

	[DBus (visible = false)]
	private void handle_scroll_wheel (Variant? parameter) {
		if (parameter != null) {
			var sources = source_settings.get_value ("sources");
			var current = source_settings.get_uint ("current");
			var length = (int) sources.n_children ();

			if (length > 0) {
				var offset = parameter.get_int32 () % length;
				source_settings.set_uint ("current", (current + (length - offset)) % length);
			}
		}
	}

	[DBus (visible = false)]
	protected virtual SimpleActionGroup create_action_group (Action root_action) {
		var group = new SimpleActionGroup ();

		group.insert (root_action);
		group.insert (source_settings.create_action ("current"));

		var action = new SimpleAction ("next", null);
		action.activate.connect (handle_middle_click);
		group.insert (action);

		action = new SimpleAction ("scroll", VariantType.INT32);
		action.activate.connect (handle_scroll_wheel);
		group.insert (action);

		action = new SimpleAction ("map", null);
		action.activate.connect (handle_activate_map);
		group.insert (action);

		action = new SimpleAction ("chart", null);
		action.activate.connect (handle_activate_chart);
		group.insert (action);

		action = new SimpleAction ("settings", null);
		action.activate.connect (handle_activate_settings);
		group.insert (action);

		return group;
	}

	[DBus (visible = false)]
	public SimpleActionGroup get_action_group () {
		if (action_group == null) {
			action_group = create_action_group (get_indicator_action ());
		}

		return (!) action_group;
	}

	[DBus (visible = false)]
	private void update_sources_menu () {
		if (sources_menu != null) {
			var menu = get_sources_menu ();

			while (menu.get_n_items () > 0)
				menu.remove (0);

			var sources = get_sources ();

			for (var i = 0; i < sources.length; i++) {
				var item = new MenuItem (sources[i].name, "indicator.current");
				item.set_attribute (Menu.ATTRIBUTE_TARGET, "u", i);

				var icon = sources[i].icon;
				if (icon != null) {
					item.set_icon ((!) icon);
				}

				menu.append_item (item);
			}
		} else {
			get_sources_menu ();
		}
	}

	[DBus (visible = false)]
	private Menu get_sources_menu () {
		if (sources_menu == null) {
			sources_menu = new Menu ();
			update_sources_menu ();
		}

		return (!) sources_menu;
	}

	[DBus (visible = false)]
	protected virtual MenuModel create_menu_model (MenuModel section_menu) {
		var menu = new Menu ();

		var submenu = new Menu ();

		submenu.append_section (null, section_menu);

		if (!is_login_user ()) {
			var section = new Menu ();
			section.append (_ ("Character Map"), "indicator.map");
			section.append (_ ("Keyboard Layout Chart"), "indicator.chart");
			section.append (_ ("Text Entry Settings..."), "indicator.settings");
			submenu.append_section (null, section);
		}

		var indicator = new MenuItem.submenu ("x", submenu);
		indicator.set_attribute ("x-canonical-type", "s", "com.canonical.indicator.root");
		indicator.set_attribute ("x-canonical-secondary-action", "s", "indicator.next");
		indicator.set_attribute ("x-canonical-scroll-action", "s", "indicator.scroll");
		indicator.set_detailed_action ("indicator.indicator");
		menu.append_item (indicator);

		return menu;
	}

	[DBus (visible = false)]
	public MenuModel get_menu_model () {
		if (menu_model == null) {
			menu_model = create_menu_model (get_sources_menu ());
		}

		return (!) menu_model;
	}

	[DBus (visible = false)]
	private void handle_changed_visible (string key) {
		update_indicator_action ();
	}

	[DBus (visible = false)]
	private void handle_changed_current (string key) {
		update_indicator_action ();
		update_login_layout ();
	}

	[DBus (visible = false)]
	private void handle_changed_sources (string key) {
		sources = null;

		update_sources_menu ();
		update_indicator_action ();
		update_login_layout ();
	}

	[DBus (visible = false)]
	private void handle_activate_map (Variant? parameter) {
		try {
			Process.spawn_command_line_async ("gucharmap");
		} catch (SpawnError error) {
			warning ("error: %s", error.message);
		}
	}

	[DBus (visible = false)]
	private void handle_activate_chart (Variant? parameter) {
		string? layout = "us";
		string? variant = null;

		var sources = get_sources ();
		var current = source_settings.get_uint ("current");

		if (current < sources.length) {
			layout = sources[current].layout;
			variant = sources[current].variant;
		}

		var has_layout = layout != null && ((!) layout).get_char () != '\0';
		var has_variant = variant != null && ((!) variant).get_char () != '\0';

		try {
			string command;

			if (has_layout && has_variant) {
				command = @"gkbd-keyboard-display -l \"$((!) layout)\t$((!) variant)\"";
			} else if (has_layout) {
				command = @"gkbd-keyboard-display -l $((!) layout)";
			} else {
				command = @"gkbd-keyboard-display -l us";
			}

			Process.spawn_command_line_async (command);
		} catch (SpawnError error) {
			warning ("error: %s", error.message);
		}
	}

	[DBus (visible = false)]
	private void handle_activate_settings (Variant? parameter) {
		try {
			Process.spawn_command_line_async ("gnome-control-center region layouts");
		} catch (SpawnError error) {
			warning ("error: %s", error.message);
		}
	}

	[DBus (visible = false)]
	private void handle_name_appeared (DBusConnection connection, string name, string name_owner) {
		try {
			greeter = Bus.get_proxy_sync (BusType.SESSION, name, "/list");
			((!) greeter).entry_selected.connect (handle_entry_selected);
		} catch (IOError error) {
			warning ("error: %s", error.message);
		}
	}

	[DBus (visible = false)]
	private void handle_name_vanished (DBusConnection connection, string name) {
		greeter = null;
	}

	[DBus (visible = false)]
	private void handle_bus_acquired (DBusConnection connection, string name) {
		try {
			connection.export_action_group ("/com/canonical/indicator/keyboard", get_action_group ());
			connection.export_menu_model ("/com/canonical/indicator/keyboard/desktop", get_menu_model ());
		} catch (Error error) {
			warning ("error: %s", error.message);
		}
	}

	[DBus (visible = false)]
	private void handle_name_lost (DBusConnection? connection, string name) {
		down ();
	}

	[DBus (visible = false)]
	public static int main (string[] args) {
		Service.service = new Service (ref args);

		Posix.signal (Posix.SIGTERM, (code) => {
			Service.service.down ();
		});

		Service.service.up ();

		return 0;
	}
}
