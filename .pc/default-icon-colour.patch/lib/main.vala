[DBus (name = "com.canonical.indicator.keyboard")]
public class Indicator.Keyboard.Service : Object {

	private MainLoop? loop;
	private Settings indicator_settings;
	private Settings source_settings;
	private Settings per_window_settings;
	private Gnome.XkbInfo xkb_info;
	private IBus.Bus? ibus;
	private Bamf.Matcher? matcher;
	private Gee.HashMap<string, uint>? window_sources;

	private SimpleActionGroup? action_group;
	private SimpleAction? indicator_action;
	private MenuModel? menu_model;
	private Menu? sources_menu;

	private Icon?[]? icons;
	private string[]? icon_strings;
	private int[]? icon_string_uniques;
	private uint[]? icon_string_subscripts;

	[DBus (visible = false)]
	public Service (bool force) {
		Bus.own_name (BusType.SESSION,
		              "com.canonical.indicator.keyboard",
		              BusNameOwnerFlags.ALLOW_REPLACEMENT | (force ? BusNameOwnerFlags.REPLACE : 0),
		              this.handle_bus_acquired,
		              null,
		              this.handle_name_lost);

		this.indicator_settings = new Settings ("com.canonical.indicator.keyboard");
		this.indicator_settings.changed["visible"].connect (this.handle_changed_visible);

		this.source_settings = new Settings ("org.gnome.desktop.input-sources");
		this.source_settings.changed["current"].connect (this.handle_changed_current);
		this.source_settings.changed["sources"].connect (this.handle_changed_sources);

		this.per_window_settings = new Settings ("org.gnome.libgnomekbd.desktop");
		this.per_window_settings.changed["group-per-window"].connect (this.handle_changed_group_per_window);

		this.xkb_info = new Gnome.XkbInfo ();

		migrate_keyboard_layouts ();

		update_window_sources ();

		this.loop = new MainLoop ();
		((!) this.loop).run ();
	}

	[DBus (visible = false)]
	private IBus.Bus get_ibus () {
		if (this.ibus == null) {
			IBus.init ();
			this.ibus = new IBus.Bus ();
		}

		return (!) this.ibus;
	}

	[DBus (visible = false)]
	private void migrate_keyboard_layouts () {
		if (!this.indicator_settings.get_boolean ("migrated")) {
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

			this.source_settings.set_value ("sources", builder.end ());

			this.indicator_settings.set_boolean ("migrated", true);
		}
	}

	[DBus (visible = false)]
	private void update_window_sources () {
		var group_per_window = this.per_window_settings.get_boolean ("group-per-window");

		if (group_per_window != (this.window_sources != null)) {
			if (group_per_window) {
				this.window_sources = new Gee.HashMap<string, uint> ();
				this.matcher = Bamf.Matcher.get_default ();
				((!) this.matcher).active_window_changed.connect (this.handle_active_window_changed);
			} else {
				((!) this.matcher).active_window_changed.disconnect (this.handle_active_window_changed);
				this.matcher = null;
				this.window_sources = null;
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
			((!) this.window_sources)[((!) old_view).path] = this.source_settings.get_uint ("current");
		}

		if (new_view != null) {
			if (!((!) this.window_sources).has_key (((!) new_view).path)) {
				var default_group = this.per_window_settings.get_int ("default-group");

				if (default_group >= 0) {
					this.source_settings.set_uint ("current", (uint) default_group);
				}
			} else {
				this.source_settings.set_uint ("current", ((!) this.window_sources)[((!) new_view).path]);
			}
		}
	}

	[DBus (visible = false)]
	private Gtk.StyleContext get_style_context () {
		var context = new Gtk.StyleContext ();

		context.set_screen (Gdk.Screen.get_default ());

		var path = new Gtk.WidgetPath ();
		path.append_type (typeof (Gtk.MenuItem));
		context.set_path (path);

		return context;
	}

	[DBus (visible = false)]
	protected virtual Icon create_icon (string? text, uint subscript) {
		const int W = 22;
		const int H = 22;
		const int w = 20;
		const int h = 20;
		const double R = 2.0;
		const double TEXT_SIZE = 12.0;
		const double SUBSCRIPT_SIZE = 8.0;

		Pango.FontDescription description;
		var style = get_style_context ();
		var colour = style.get_color (Gtk.StateFlags.NORMAL);
		style.get (Gtk.StateFlags.NORMAL, Gtk.STYLE_PROPERTY_FONT, out description);

		var surface = new Cairo.ImageSurface (Cairo.Format.ARGB32, W, H);
		var context = new Cairo.Context (surface);

		context.translate (0.5 * (W - w), 0.5 * (H - h));

		context.new_sub_path ();
		context.arc (R, R, R, Math.PI, -0.5 * Math.PI);
		context.arc (w - R, R, R, -0.5 * Math.PI, 0);
		context.arc (w - R, h - R, R, 0, 0.5 * Math.PI);
		context.arc (R, h - R, R, 0.5 * Math.PI, Math.PI);
		context.close_path ();

		context.set_source_rgba (colour.red, colour.green, colour.blue, colour.alpha);
		context.fill ();
		context.set_operator (Cairo.Operator.CLEAR);

		if (text != null) {
			var text_layout = Pango.cairo_create_layout (context);
			text_layout.set_alignment (Pango.Alignment.CENTER);
			description.set_absolute_size (Pango.units_from_double (TEXT_SIZE));
			text_layout.set_font_description (description);
			text_layout.set_text ((!) text, -1);
			Pango.cairo_update_layout (context, text_layout);
			int text_width;
			int text_height;
			text_layout.get_pixel_size (out text_width, out text_height);

			if (subscript > 0) {
				var subscript_layout = Pango.cairo_create_layout (context);
				subscript_layout.set_alignment (Pango.Alignment.CENTER);
				description.set_absolute_size (Pango.units_from_double (SUBSCRIPT_SIZE));
				subscript_layout.set_font_description (description);
				subscript_layout.set_text (@"$subscript", -1);
				Pango.cairo_update_layout (context, subscript_layout);
				int subscript_width;
				int subscript_height;
				subscript_layout.get_pixel_size (out subscript_width, out subscript_height);

				context.save ();
				context.translate ((w - (text_width + subscript_width)) / 2, (h - text_height) / 2);
				Pango.cairo_layout_path (context, text_layout);
				context.fill ();
				context.restore ();

				context.save ();
				context.translate ((w + (text_width - subscript_width)) / 2, (h + text_height) / 2 - subscript_height);
				Pango.cairo_layout_path (context, subscript_layout);
				context.fill ();
				context.restore ();
			} else {
				context.save ();
				context.translate ((w - text_width) / 2, (h - text_height) / 2);
				Pango.cairo_layout_path (context, text_layout);
				context.fill ();
				context.restore ();
			}
		}

		var buffer = new ByteArray ();

		surface.write_to_png_stream ((data) => {
			buffer.append (data);
			return Cairo.Status.SUCCESS;
		});

		return new BytesIcon (ByteArray.free_to_bytes ((owned) buffer));
	}

	[DBus (visible = false)]
	private string get_icon_string (uint index) {
		string? icon_string = null;

		if (this.icon_strings == null) {
			var array = this.source_settings.get_value ("sources");
			this.icon_strings = new string[array.n_children ()];
		}

		if (index < ((!) this.icon_strings).length) {
			icon_string = this.icon_strings[index];

			if (icon_string == null) {
				var array = this.source_settings.get_value ("sources");

				string type;
				string name;

				array.get_child (index, "(ss)", out type, out name);

				if (type == "xkb") {
					string? short_name;

					this.xkb_info.get_layout_info (name, null, out short_name, null, null);

					if (short_name != null) {
						this.icon_strings[index] = get_abbreviation ((!) short_name);
						icon_string = this.icon_strings[index];
					}
				}
			}
		}

		if (icon_string == null) {
			icon_string = "";
		}

		return (!) icon_string;
	}

	[DBus (visible = false)]
	private bool is_icon_string_unique (uint index) {
		bool icon_string_unique = true;

		if (this.icon_string_uniques == null) {
			var array = this.source_settings.get_value ("sources");
			this.icon_string_uniques = new int[array.n_children ()];

			for (var i = 0; i < ((!) this.icon_string_uniques).length; i++) {
				this.icon_string_uniques[i] = -1;
			}
		}

		if (index < ((!) this.icon_string_uniques).length) {
			if (this.icon_string_uniques[index] == -1) {
				this.icon_string_uniques[index] = 1;

				var icon_string = get_icon_string (index);

				for (var i = 0; i < ((!) this.icon_string_uniques).length && this.icon_string_uniques[index] == 1; i++) {
					if (i != index && get_icon_string (i) == icon_string) {
						this.icon_string_uniques[index] = 0;
					}
				}
			}

			icon_string_unique = this.icon_string_uniques[index] != 0;
		}

		return icon_string_unique;
	}

	[DBus (visible = false)]
	private uint get_icon_string_subscript (uint index) {
		uint icon_string_subscript = 0;

		if (this.icon_string_subscripts == null) {
			var array = this.source_settings.get_value ("sources");
			this.icon_string_subscripts = new uint[array.n_children ()];
		}

		if (index < ((!) this.icon_string_subscripts).length) {
			icon_string_subscript = this.icon_string_subscripts[index];

			if (icon_string_subscript == 0) {
				this.icon_string_subscripts[index] = 1;

				for (var i = (int) index - 1; i >= 0 && this.icon_string_subscripts[index] == 1; i--) {
					if (get_icon_string (i) == get_icon_string (index)) {
						this.icon_string_subscripts[index] = get_icon_string_subscript (i) + 1;
					}
				}

				icon_string_subscript = this.icon_string_subscripts[index];
			}
		}

		return icon_string_subscript;
	}

	[DBus (visible = false)]
	private Icon? get_icon (uint index) {
		Icon? icon = null;

		if (this.icons == null) {
			var array = this.source_settings.get_value ("sources");
			this.icons = new Icon?[array.n_children ()];
		}

		if (index < ((!) this.icons).length) {
			icon = this.icons[index];

			if (icon == null) {
				var array = this.source_settings.get_value ("sources");

				string type;
				string name;

				array.get_child (index, "(ss)", out type, out name);

				if (type == "xkb") {
					var icon_string = get_icon_string (index);
					var icon_unique = is_icon_string_unique (index);
					var icon_subscript = get_icon_string_subscript (index);

					if (icon_string.get_char () != '\0') {
						string icon_name;

						if (icon_unique) {
							icon_name = @"indicator-keyboard-$((!) icon_string)";
						} else {
							icon_name = @"indicator-keyboard-$((!) icon_string)-$icon_subscript";
						}

						var icon_theme = Gtk.IconTheme.get_default ();
						Gtk.IconInfo? icon_info = icon_theme.lookup_icon (icon_name, 22, 0);

						if (icon_info != null) {
							try {
								this.icons[index] = Icon.new_for_string (((!) icon_info).get_filename ());
							} catch (Error error) {
								this.icons[index] = null;
							}
						}
					}

					if (this.icons[index] == null) {
						if (icon_unique) {
							this.icons[index] = create_icon (icon_string, 0);
						} else {
							this.icons[index] = create_icon (icon_string, icon_subscript);
						}
					}
				} else if (type == "ibus") {
					var names = new string[2];
					names[0] = name;

					var engines = get_ibus ().get_engines_by_names (names);

					if (engines.length > 0) {
						var engine = engines[0];

						try {
							this.icons[index] = Icon.new_for_string (engine.get_icon ());
						} catch (Error error) {
							warning ("error: %s", error.message);
						}
					}
				}

				icon = this.icons[index];
			}
		}

		return icon;
	}

	[DBus (visible = false)]
	protected virtual SimpleActionGroup create_action_group (Action root_action) {
		var group = new SimpleActionGroup ();

		group.insert (root_action);
		group.insert (this.source_settings.create_action ("current"));

		var action = new SimpleAction ("map", null);
		action.activate.connect (this.handle_activate_map);
		group.insert (action);

		action = new SimpleAction ("chart", null);
		action.activate.connect (this.handle_activate_chart);
		group.insert (action);

		action = new SimpleAction ("settings", null);
		action.activate.connect (this.handle_activate_settings);
		group.insert (action);

		return group;
	}

	[DBus (visible = false)]
	private void update_indicator_action () {
		var visible = this.indicator_settings.get_boolean ("visible");
		var current = this.source_settings.get_uint ("current");
		var icon = get_icon (current);
		Variant state;

		if (icon != null) {
			state = new Variant.parsed ("{ 'visible' : <%b>, 'icon' : %v }", visible, ((!) icon).serialize ());
		} else {
			state = new Variant.parsed ("{ 'visible' : <%b> }", visible);
		}

		get_indicator_action ().set_state (state);
	}

	[DBus (visible = false)]
	private SimpleAction get_indicator_action () {
		if (this.indicator_action == null) {
			var state = new Variant.parsed ("{ 'visible' : <false> }");
			this.indicator_action = new SimpleAction.stateful ("indicator", null, state);
			update_indicator_action ();
		}

		return (!) this.indicator_action;
	}

	[DBus (visible = false)]
	public SimpleActionGroup get_action_group () {
		if (this.action_group == null) {
			this.action_group = create_action_group (get_indicator_action ());
		}

		return (!) this.action_group;
	}

	[DBus (visible = false)]
	protected virtual MenuModel create_menu_model (MenuModel section_menu) {
		var menu = new Menu ();

		var submenu = new Menu ();

		submenu.append_section (null, section_menu);

		var section = new Menu ();
		section.append (_ ("Character Map"), "indicator.map");
		section.append (_ ("Keyboard Layout Chart"), "indicator.chart");
		section.append (_ ("Text Entry Settings..."), "indicator.settings");
		submenu.append_section (null, section);

		var indicator = new MenuItem.submenu ("x", submenu);
		indicator.set_attribute ("x-canonical-type", "s", "com.canonical.indicator.root");
		indicator.set_detailed_action ("indicator.indicator");
		menu.append_item (indicator);

		return menu;
	}

	[DBus (visible = false)]
	private string get_display_name (string layout) {
		string? language = Xkl.get_language_name (layout);
		string? country = Xkl.get_country_name (layout);
		var has_language = language != null && ((!) language).get_char () != '\0';
		var has_country = country != null && ((!) country).get_char () != '\0';

		if (has_language && has_country) {
			return @"$((!) language) ($((!) country))";
		} else if (has_language) {
			return (!) language;
		} else if (has_country) {
			return (!) country;
		} else {
			return "";
		}
	}

	[DBus (visible = false)]
	private void update_sources_menu () {
		if (this.sources_menu != null) {
			var menu = get_sources_menu ();

			while (menu.get_n_items () > 0)
				menu.remove (0);

			VariantIter iter;
			string type;
			string name;

			this.source_settings.get ("sources", "a(ss)", out iter);

			for (var i = 0; iter.next ("(ss)", out type, out name); i++) {
				if (type == "xkb") {
					string? display_name;
					string? layout_name;

					this.xkb_info.get_layout_info (name, out display_name, null, out layout_name, null);

					if (display_name != null) {
						name = (!) display_name;
					} else if (layout_name != null) {
						name = get_display_name ((!) layout_name);
					}
				}
				else if (type == "ibus") {
					var names = new string[2];
					names[0] = name;

					var engines = get_ibus ().get_engines_by_names (names);

					if (engines.length > 0) {
						var engine = engines[0];
						string? language = engine.get_language ();
						string? display_name = engine.get_longname ();

						if (language != null) {
							language = Xkl.get_language_name ((!) language);
						}

						if (language != null && display_name != null) {
							name = @"$((!) language) ($((!) display_name))";
						} else if (language != null) {
							name = (!) language;
						} else if (display_name != null) {
							name = (!) display_name;
						}
					}
				}

				var menu_item = new MenuItem (name, "indicator.current");
				menu_item.set_attribute (Menu.ATTRIBUTE_TARGET, "u", i);

				var icon = get_icon (i);
				if (icon != null) {
					menu_item.set_icon ((!) icon);
				}

				menu.append_item (menu_item);
			}
		} else {
			get_sources_menu ();
		}
	}

	[DBus (visible = false)]
	private Menu get_sources_menu () {
		if (this.sources_menu == null) {
			this.sources_menu = new Menu ();
			update_sources_menu ();
		}

		return (!) this.sources_menu;
	}

	[DBus (visible = false)]
	public MenuModel get_menu_model () {
		if (this.menu_model == null) {
			this.menu_model = create_menu_model (get_sources_menu ());
		}

		return (!) this.menu_model;
	}

	[DBus (visible = false)]
	private void handle_changed_visible (string key) {
		update_indicator_action ();
	}

	[DBus (visible = false)]
	private void handle_changed_current (string key) {
		update_indicator_action ();
	}

	[DBus (visible = false)]
	private void handle_changed_sources (string key) {
		this.icon_string_subscripts = null;
		this.icon_string_uniques = null;
		this.icon_strings = null;
		this.icons = null;

		update_sources_menu ();
		update_indicator_action ();
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
		var layout = "us";
		string? variant = null;

		var current = this.source_settings.get_uint ("current");
		var array = this.source_settings.get_value ("sources");

		if (current < array.n_children ()) {
			string type;
			string name;

			array.get_child (current, "(ss)", out type, out name);

			if (type == "xkb") {
				this.xkb_info.get_layout_info (name, null, null, out layout, out variant);
			} else if (type == "ibus") {
				var names = new string[2];
				names[0] = name;

				var engines = get_ibus ().get_engines_by_names (names);

				if (engines.length > 0) {
					var engine = engines[0];

					layout = engine.get_layout ();
					variant = engine.get_layout_variant ();
				}
			}
		}

		try {
			string command;

			if (variant != null && ((!) variant).get_char () != '\0') {
				command = @"gkbd-keyboard-display -l \"$layout\t$((!) variant)\"";
			} else {
				command = @"gkbd-keyboard-display -l $layout";
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
		((!) this.loop).quit ();
		this.loop = null;
	}

	[DBus (visible = false)]
	public static int main (string[] args) {
		Gtk.init (ref args);
		new Service ("--force" in args);
		return 0;
	}
}
