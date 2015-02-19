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

	private static const uint PROPERTIES_DELAY = 250;

	private static Service service;

	private bool force;
	private bool use_gtk;

	private MainLoop? loop;
	private Settings indicator_settings;
	private Settings source_settings;
	private Settings per_window_settings;
	private SList<Act.User> users;

	private WindowStack? window_stack;
	private Gee.HashMap<uint, Source>? window_sources;
	private uint focused_window_id;

	private IBus.Bus? ibus;
	private IBusPanel? ibus_panel;
	private ulong ibus_connected_id;
	private uint panel_timeout;

	private Fcitx.InputMethod? fcitx;
	private bool fcitx_initialized;

	private Source[]? sources;

	private SimpleActionGroup? action_group;
	private SimpleAction? indicator_action;
	private SimpleAction? active_action;
	private IndicatorMenu? desktop_menu;
	private IndicatorMenu? desktop_greeter_menu;
	private IndicatorMenu? desktop_lockscreen_menu;

	private KeyboardPlugin? keyboard_plugin;
	private UnitySession? unity_session;
	private UnityGreeter? unity_greeter;
	private string? greeter_user;
	private uint lightdm_current;

	[DBus (visible = false)]
	public Service (ref unowned string[] args) {
		force = "--force" in args;
		use_gtk = "--use-gtk" in args;

		if (use_gtk) {
			use_gtk = Gtk.init_check (ref args);

			Gtk.IconTheme? icon_theme = Gtk.IconTheme.get_default ();

			if (icon_theme != null) {
				((!) icon_theme).changed.connect (() => {
					if (sources != null) {
						foreach (var source in (!) sources) {
							source.icon = null;
						}
					}

					if (desktop_menu != null) {
						get_desktop_menu ().set_sources (get_sources ());
					}

					if (desktop_greeter_menu != null) {
						get_desktop_greeter_menu ().set_sources (get_sources ());
					}

					if (desktop_lockscreen_menu != null) {
						get_desktop_lockscreen_menu ().set_sources (get_sources ());
					}

					if (indicator_action != null) {
						update_indicator_action ();
					}
				});
			}
		} else {
			Gdk.init (ref args);
		}

		if (is_login_user ()) {
			var name = Environment.get_variable ("UNITY_GREETER_DBUS_NAME");

			if (name != null) {
				Bus.watch_name (BusType.SESSION,
				                (!) name,
				                BusNameWatcherFlags.NONE,
				                handle_unity_greeter_name_appeared,
				                handle_unity_greeter_name_vanished);
			}
		} else {
			Bus.watch_name (BusType.SESSION,
			                "org.gnome.SettingsDaemon.Keyboard",
			                BusNameWatcherFlags.NONE,
			                handle_keyboard_name_appeared,
			                handle_keyboard_name_vanished);

			Bus.watch_name (BusType.SESSION,
			                "com.canonical.Unity",
			                BusNameWatcherFlags.NONE,
			                handle_unity_name_appeared,
			                handle_unity_name_vanished);

			if (!is_fcitx_active ()) {
				Bus.watch_name (BusType.SESSION,
				                "com.canonical.Unity.WindowStack",
				                BusNameWatcherFlags.NONE,
				                handle_window_stack_name_appeared,
				                handle_window_stack_name_vanished);
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
	private static bool is_ibus_active () {
		if (is_login_user ()) {
			return false;
		}

		var module = Environment.get_variable ("GTK_IM_MODULE");
		return module != null && (!) module == "ibus";
	}

	[DBus (visible = false)]
	private static bool is_fcitx_active () {
		if (is_login_user ()) {
			return false;
		}

		var module = Environment.get_variable ("GTK_IM_MODULE");
		return module != null && (!) module == "fcitx";
	}

	[DBus (visible = false)]
	private IBus.Bus get_ibus () {
		if (ibus == null) {
			IBus.init ();

			var proxy = new IBus.Bus ();

			proxy.connected.connect (() => {
				if (desktop_menu != null) {
					get_desktop_menu ().set_sources (get_sources ());
				}

				if (desktop_greeter_menu != null) {
					get_desktop_greeter_menu ().set_sources (get_sources ());
				}

				if (desktop_lockscreen_menu != null) {
					get_desktop_lockscreen_menu ().set_sources (get_sources ());
				}

				if (indicator_action != null) {
					update_indicator_action ();
				}
			});

			ibus = proxy;
		}

		return (!) ibus;
	}

	[DBus (visible = false)]
	private IBusPanel? get_ibus_panel () {
		if (ibus_panel == null && get_ibus ().is_connected ()) {
			var connection = get_ibus ().get_connection ();
			var name = "org.freedesktop.IBus.Panel";
			var path = "/org/freedesktop/IBus/Panel";

			try {
				var proxy = connection.get_proxy_sync<IBusPanel> (name, path);

				proxy.properties_registered.connect ((variant) => {
					var properties = new IBus.PropList ();
					properties.deserialize (variant);

					if (properties is IBus.PropList) {
						handle_properties_registered ((!) (properties as IBus.PropList));
					}
				});
				proxy.property_updated.connect ((variant) => {
					var type = IBus.PropType.NORMAL;
					var state = IBus.PropState.INCONSISTENT;
					var text = new IBus.Text.from_static_string ("");
					var property = new IBus.Property ("", type, text, null, text, false, false, state, null);
					property.deserialize (variant);

					if (property is IBus.Property) {
						handle_property_updated ((!) (property as IBus.Property));
					}
				});

				ibus_panel = proxy;
			} catch (IOError error) {
				warning ("error: %s", error.message);
			}
		}

		return ibus_panel;
	}

	[DBus (visible = false)]
	private Fcitx.InputMethod? get_fcitx () {
		if (!fcitx_initialized) {
			fcitx_initialized = true;

			if (is_fcitx_active ()) {
				try {
					var proxy = new Fcitx.InputMethod (BusType.SESSION, DBusProxyFlags.NONE, 0);
					proxy.notify["current-im"].connect ((pspec) => { handle_changed_current ("current"); });
					fcitx = proxy;
				} catch (Error error) {
					warning ("error: %s", error.message);
				}
			}
		}

		return fcitx;
	}

	[DBus (visible = false)]
	public void up () {
		if (loop == null) {
			var main_loop = new MainLoop ();
			loop = main_loop;
			main_loop.run ();
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
		if (greeter_user == null && unity_greeter != null) {
			try {
				greeter_user = ((!) unity_greeter).get_active_entry ();
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
					foreach (var outer in ((!) user).input_sources) {
						foreach (var inner in (!) outer) {
							unowned string key;
							unowned string value;

							((!) inner).get ("{&s&s}", out key, out value);

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
							source = layouts[0].replace (" ", "+").replace ("\t", "+");
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

			for (var i = 0; i < array.n_children (); i++) {
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
			lightdm_current = get_current ();

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

				foreach (var outer in user.input_sources) {
					foreach (var inner in (!) outer) {
						unowned string key;
						unowned string source;

						((!) inner).get ("{&s&s}", out key, out source);

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
		if (is_login_user ()) {
			unowned List<LightDM.Layout> layouts = LightDM.get_layouts ();
			var current = get_current ();

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
	}

	[DBus (visible = false)]
	private void update_window_sources () {
		if (window_stack != null) {
			var group_per_window = per_window_settings.get_boolean ("group-per-window");

			if (group_per_window != (window_sources != null)) {
				if (group_per_window) {
					focused_window_id = 0;

					try {
						var windows = ((!) window_stack).get_window_stack ();

						foreach (var window in windows) {
							if (window.focused) {
								focused_window_id = window.window_id;
								break;
							}
						}
					} catch (IOError error) {
						warning ("error: %s", error.message);
					}

					window_sources = new Gee.HashMap<uint, Source> ();
					((!) window_stack).window_destroyed.connect (handle_window_destroyed);
					((!) window_stack).focused_window_changed.connect (handle_focused_window_changed);
				} else {
					((!) window_stack).focused_window_changed.disconnect (handle_focused_window_changed);
					((!) window_stack).window_destroyed.disconnect (handle_window_destroyed);
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
	private void handle_window_destroyed (uint window_id, string app_id) {
		((!) window_sources).unset (window_id);
	}

	[DBus (visible = false)]
	private void handle_focused_window_changed (uint window_id, string app_id, uint stage) {
		var sources = get_sources ();
		var old_current = get_current ();

		if (old_current < sources.length) {
			((!) window_sources)[focused_window_id] = sources[old_current];
		}

		if (!(((!) window_sources).has_key (window_id))) {
			var default_group = per_window_settings.get_int ("default-group");

			if (default_group >= 0) {
				for (var offset = 0; offset < sources.length; offset++) {
					var current = (default_group + offset) % sources.length;
					var source = sources[current];

					if (source.is_xkb ||
					    (source.is_ibus && is_ibus_active ()) ||
					    (source.is_fcitx && is_fcitx_active ())) {
						if (current != old_current) {
							source_settings.set_uint ("current", current);
						}

						break;
					}
				}
			}
		} else {
			var source = ((!) window_sources)[window_id];

			for (var current = 0; current < sources.length; current++) {
				if (sources[current] == source) {
					if (current != old_current) {
						source_settings.set_uint ("current", current);
					}

					break;
				}
			}
		}

		focused_window_id = window_id;
	}

	[DBus (visible = false)]
	private uint get_current () {
		if (is_fcitx_active () && get_fcitx () != null) {
			string? engine = ((!) get_fcitx ()).current_im;

			if (engine != null) {
				var is_xkb = ((!) engine).has_prefix ("fcitx-keyboard-");
				var type = is_xkb ? "xkb" : "fcitx";
				var name = (!) engine;

				if (is_xkb) {
					name = name.substring ("fcitx-keyboard-".length);
					var index = name.index_of ("-");
					if (index >= 0) {
						name.data[index] = '+';
					}
				}

				var i = 0;

				foreach (var pair in source_settings.get_value ("sources")) {
					unowned string source_type;
					unowned string source_name;

					((!) pair).get ("(&s&s)", out source_type, out source_name);

					if (source_name == name && source_type == type) {
						return i;
					}

					i++;
				}
			}
		}

		return source_settings.get_uint ("current");
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

				if (ibus_connected_id == 0 && sources[i].is_ibus) {
					ibus_connected_id = get_ibus ().connected.connect (() => { get_ibus_panel (); });
					get_ibus ().disconnected.connect (() => { ibus_panel = null; });

					if (get_ibus ().is_connected ()) {
						get_ibus_panel ();
					}
				}
			}
		}

		return (!) sources;
	}

	[DBus (visible = false)]
	private void handle_properties_registered (IBus.PropList list) {
		if (panel_timeout > 0) {
			GLib.Source.remove (panel_timeout);
			panel_timeout = 0;
		}

		panel_timeout = Timeout.add (PROPERTIES_DELAY, () => {
			get_desktop_menu ().set_properties (list);
			panel_timeout = 0;
			return false;
		});
	}

	[DBus (visible = false)]
	private void handle_property_updated (IBus.Property property) {
		get_desktop_menu ().update_property (property);
	}

	[DBus (visible = false)]
	private void update_indicator_action () {
		Icon? icon = null;
		string? name = null;

		var sources = get_sources ();
		var active = get_active_action ().get_state ().get_uint32 ();

		if (active < sources.length) {
			icon = sources[active].icon;
			name = sources[active].name;
		}

		var builder = new VariantBuilder (new VariantType ("a{sv}"));
		builder.add ("{sv}", "visible", indicator_settings.get_value ("visible"));
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
	private void handle_changed_active (Variant? value) {
		if (value != null) {
			((!) active_action).set_state ((!) value);
			update_indicator_action ();

			if (keyboard_plugin != null) {
				try {
					((!) keyboard_plugin).activate_input_source (((!) value).get_uint32 ());
				} catch (IOError error) {
					warning ("error: %s", error.message);
				}
			}
		}
	}

	[DBus (visible = false)]
	private void update_active_action () {
		if (active_action != null) {
			((!) active_action).set_state (new Variant.uint32 (get_current ()));
			update_indicator_action ();
		}
	}

	[DBus (visible = false)]
	private Action get_active_action () {
		if (active_action == null) {
			var action = new SimpleAction.stateful ("active", VariantType.UINT32, new Variant.uint32 (get_current ()));
			action.change_state.connect (handle_changed_active);
			active_action = action;
		}

		return (!) active_action;
	}

	[DBus (visible = false)]
	private void handle_middle_click (Variant? parameter) {
		handle_scroll_wheel (new Variant.int32 (-1));
	}

	[DBus (visible = false)]
	private void handle_scroll_wheel (Variant? parameter) {
		if (parameter != null) {
			var old_current = get_current ();
			var sources = get_sources ();
			var length = 0;

			foreach (var source in sources) {
				if (source.is_xkb ||
				    (source.is_ibus && is_ibus_active ()) ||
				    (source.is_fcitx && is_fcitx_active ())) {
					length++;
				}
			}

			if (length > 1) {
				var current = old_current;
				var offset = -((!) parameter).get_int32 () % length;

				/* Go backward. */
				for (; offset < 0; offset++) {
					do {
						current = (current + sources.length - 1) % sources.length;
					} while ((sources[current].is_ibus && !is_ibus_active ()) ||
					         (sources[current].is_fcitx && !is_fcitx_active ()));
				}

				/* Go forward. */
				for (; offset > 0; offset--) {
					do {
						current = (current + 1) % sources.length;
					} while ((sources[current].is_ibus && !is_ibus_active ()) ||
					         (sources[current].is_fcitx && !is_fcitx_active ()));
				}

				if (current != old_current) {
					source_settings.set_uint ("current", current);
				}
			}
		}
	}

	[DBus (visible = false)]
	private void handle_middle_click_when_locked (Variant? parameter) {
		handle_scroll_wheel_when_locked (new Variant.int32 (-1));
	}

	[DBus (visible = false)]
	private void handle_scroll_wheel_when_locked (Variant? parameter) {
		if (parameter != null) {
			var sources = get_sources ();
			var xkb_length = 0;

			/* Figure out how many Xkb sources we have. */
			foreach (var source in sources) {
				if (source.is_xkb) {
					xkb_length++;
				}
			}

			if (xkb_length > 1) {
				var active_action = get_active_action ();
				var active = active_action.get_state ().get_uint32 ();
				var offset = -((!) parameter).get_int32 () % xkb_length;

				/* Make offset positive modulo xkb_length. */
				if (offset < 0) {
					offset += xkb_length;
				}

				/* We need to cycle through Xkb sources only. */
				while (offset > 0) {
					do {
						active = (active + 1) % sources.length;
					} while (!sources[active].is_xkb);

					offset--;
				}

				active_action.change_state (new Variant.uint32 (active));
			}
		}
	}

	[DBus (visible = false)]
	protected virtual SimpleActionGroup create_action_group (Action root_action) {
		var group = new SimpleActionGroup ();

		/*
		 * The 'current' action reflects the current setting in
		 * GSettings and the 'active' action only exists to set the
		 * active input source without persisting it.
		 *
		 * The lock screen menu uses the 'active' action while the
		 * other menus instead persist the current input source.
		 */

		group.add_action (root_action);
		group.add_action (get_active_action ());
		group.add_action (source_settings.create_action ("current"));

		var action = new SimpleAction ("next", null);
		action.activate.connect (handle_middle_click);
		group.add_action (action);

		action = new SimpleAction ("scroll", VariantType.INT32);
		action.activate.connect (handle_scroll_wheel);
		group.add_action (action);

		action = new SimpleAction ("locked_next", null);
		action.activate.connect (handle_middle_click_when_locked);
		group.add_action (action);

		action = new SimpleAction ("locked_scroll", VariantType.INT32);
		action.activate.connect (handle_scroll_wheel_when_locked);
		group.add_action (action);

		action = new SimpleAction ("map", null);
		action.activate.connect (handle_activate_map);
		group.add_action (action);

		action = new SimpleAction ("chart", null);
		action.activate.connect (handle_activate_chart);
		group.add_action (action);

		action = new SimpleAction ("settings", null);
		action.activate.connect (handle_activate_settings);
		group.add_action (action);

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
	public IndicatorMenu get_desktop_menu () {
		if (desktop_menu == null) {
			var options = IndicatorMenu.Options.DCONF;

			if (!is_fcitx_active ()) {
				options |= IndicatorMenu.Options.XKB | IndicatorMenu.Options.SETTINGS;

				if (is_ibus_active ()) {
					options |= IndicatorMenu.Options.IBUS;
				}
			}

			var menu = new IndicatorMenu (get_action_group (), options);

			menu.set_sources (get_sources ());
			menu.activate.connect ((property, state) => {
				var panel = get_ibus_panel ();

				if (panel != null) {
					try {
						((!) panel).activate_property (property.key, state);
					} catch (IOError error) {
						warning ("error: %s", error.message);
					}
				}
			});

			desktop_menu = menu;
		}

		return (!) desktop_menu;
	}

	[DBus (visible = false)]
	public IndicatorMenu get_desktop_greeter_menu () {
		if (desktop_greeter_menu == null) {
			var options = IndicatorMenu.Options.DCONF |
			              IndicatorMenu.Options.XKB;

			var menu = new IndicatorMenu (get_action_group (), options);
			menu.set_sources (get_sources ());
			desktop_greeter_menu = menu;
		}

		return (!) desktop_greeter_menu;
	}

	[DBus (visible = false)]
	public IndicatorMenu get_desktop_lockscreen_menu () {
		if (desktop_lockscreen_menu == null) {
			var options = IndicatorMenu.Options.XKB;

			var menu = new IndicatorMenu (get_action_group (), options);
			menu.set_sources (get_sources ());
			desktop_lockscreen_menu = menu;
		}

		return (!) desktop_lockscreen_menu;
	}

	[DBus (visible = false)]
	private void handle_changed_visible (string key) {
		update_indicator_action ();
	}

	[DBus (visible = false)]
	private void handle_changed_current (string key) {
		update_indicator_action ();
		update_active_action ();
		update_login_layout ();
	}

	[DBus (visible = false)]
	private void handle_changed_sources (string key) {
		sources = null;

		get_desktop_menu ().set_sources (get_sources ());
		get_desktop_greeter_menu ().set_sources (get_sources ());
		get_desktop_lockscreen_menu ().set_sources (get_sources ());
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
		var current = get_current ();

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
			Process.spawn_command_line_async ("unity-control-center region layouts");
		} catch (SpawnError error) {
			warning ("error: %s", error.message);
		}
	}

	[DBus (visible = false)]
	private void handle_unity_greeter_name_appeared (DBusConnection connection, string name, string name_owner) {
		try {
			var greeter = Bus.get_proxy_sync<UnityGreeter> (BusType.SESSION, name, "/list");
			greeter.entry_selected.connect (handle_entry_selected);
			unity_greeter = greeter;
		} catch (IOError error) {
			warning ("error: %s", error.message);
		}
	}

	[DBus (visible = false)]
	private void handle_unity_greeter_name_vanished (DBusConnection connection, string name) {
		unity_greeter = null;
	}

	[DBus (visible = false)]
	private void handle_keyboard_name_appeared (DBusConnection connection, string name, string name_owner) {
		try {
			keyboard_plugin = Bus.get_proxy_sync (BusType.SESSION, name, "/org/gnome/SettingsDaemon/Keyboard");
		} catch (IOError error) {
			warning ("error: %s", error.message);
		}
	}

	[DBus (visible = false)]
	private void handle_keyboard_name_vanished (DBusConnection connection, string name) {
		keyboard_plugin = null;
	}

	[DBus (visible = false)]
	private void handle_unity_name_appeared (DBusConnection connection, string name, string name_owner) {
		try {
			var session = Bus.get_proxy_sync<UnitySession> (BusType.SESSION, name, "/com/canonical/Unity/Session");

			session.locked.connect (() => {
				var sources = get_sources ();

				if (sources.length > 0) {
					var current = get_current ();

					if (current < sources.length && !sources[current].is_xkb) {
						for (var i = 0; i < sources.length; i++) {
							if (sources[i].is_xkb) {
								get_active_action ().change_state (new Variant.uint32 (i));
								break;
							}
						}
					}
				}
			});
			session.unlocked.connect (() => {
				get_active_action ().change_state (new Variant.uint32 (get_current ()));
			});

			unity_session = session;
		} catch (IOError error) {
			warning ("error: %s", error.message);
		}
	}

	[DBus (visible = false)]
	private void handle_unity_name_vanished (DBusConnection connection, string name) {
		unity_session = null;
	}

	[DBus (visible = false)]
	private void handle_window_stack_name_appeared (DBusConnection connection, string name, string name_owner) {
		try {
			window_stack = Bus.get_proxy_sync (BusType.SESSION, name, "/com/canonical/Unity/WindowStack");
			update_window_sources ();
		} catch (IOError error) {
			warning ("error: %s", error.message);
		}
	}

	[DBus (visible = false)]
	private void handle_window_stack_name_vanished (DBusConnection connection, string name) {
		window_stack = null;
	}

	[DBus (visible = false)]
	private void handle_bus_acquired (DBusConnection connection, string name) {
		try {
			connection.export_action_group ("/com/canonical/indicator/keyboard", get_action_group ());
			connection.export_menu_model ("/com/canonical/indicator/keyboard/desktop", get_desktop_menu ());
			connection.export_menu_model ("/com/canonical/indicator/keyboard/desktop_greeter", get_desktop_greeter_menu ());
			connection.export_menu_model ("/com/canonical/indicator/keyboard/desktop_lockscreen", get_desktop_lockscreen_menu ());
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
