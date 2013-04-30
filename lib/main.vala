[DBus (name = "com.canonical.indicator.keyboard")]
public class Indicator.Keyboard.Service : Object {

	private MainLoop loop;
	private Settings indicator_settings;
	private Settings source_settings;
	private IBus.Bus ibus;

	private SimpleActionGroup action_group;
	private SimpleAction indicator_action;
	private MenuModel menu_model;
	private Menu sources_menu;

	private Icon[] icons;

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

		this.loop = new MainLoop ();
		this.loop.run ();
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
	protected virtual Icon create_icon (string text) {
		const int W = 20;
		const int H = 20;
		const double R = 2.0;

		Pango.FontDescription description;
		var style = get_style_context ();
		var colour = style.get_color (Gtk.StateFlags.NORMAL);
		style.get (Gtk.StateFlags.NORMAL, Gtk.STYLE_PROPERTY_FONT, out description);

		var surface = new Cairo.ImageSurface (Cairo.Format.ARGB32, W, H);
		var context = new Cairo.Context (surface);

		context.new_sub_path ();
		context.arc (R, R, R, Math.PI, -0.5 * Math.PI);
		context.arc (W - R, R, R, -0.5 * Math.PI, 0);
		context.arc (W - R, H - R, R, 0, 0.5 * Math.PI);
		context.arc (R, H - R, R, 0.5 * Math.PI, Math.PI);
		context.close_path ();

		context.set_source_rgba (colour.red, colour.green, colour.blue, colour.alpha);
		context.fill ();

		context.set_operator (Cairo.Operator.CLEAR);
		var layout = Pango.cairo_create_layout (context);
		layout.set_alignment (Pango.Alignment.CENTER);
		layout.set_font_description (description);
		layout.set_text (text, -1);
		Pango.cairo_update_layout (context, layout);
		int width;
		int height;
		layout.get_pixel_size (out width, out height);
		context.translate ((W - width) / 2, (H - height) / 2);
		Pango.cairo_layout_path (context, layout);
		context.fill ();

		var buffer = new ByteArray ();

		surface.write_to_png_stream ((data) => {
			buffer.append (data);
			return Cairo.Status.SUCCESS;
		});

		return new BytesIcon (ByteArray.free_to_bytes ((owned) buffer));
	}

	[DBus (visible = false)]
	private Icon get_icon (uint index) {
		Icon icon = null;
		Variant array = null;

		if (this.icons == null) {
			this.source_settings.get ("sources", "@a(ss)", out array);
			this.icons = new Icon[array.n_children ()];
		}

		if (index < this.icons.length) {
			icon = this.icons[index];

			if (icon == null) {
				if (array == null) {
					this.source_settings.get ("sources", "@a(ss)", out array);
				}

				string type;
				string name;

				array.get_child (index, "(ss)", out type, out name);
				this.icons[index] = create_icon (name);
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
			state = new Variant.parsed ("{ 'visible' : <%b>, 'icon' : %v }", visible, icon.serialize ());
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

		return this.indicator_action;
	}

	[DBus (visible = false)]
	public SimpleActionGroup get_action_group () {
		if (this.action_group == null) {
			this.action_group = create_action_group (get_indicator_action ());
		}

		return this.action_group;
	}

	[DBus (visible = false)]
	protected virtual MenuModel create_menu_model (MenuModel section_menu) {
		var menu = new Menu ();

		var submenu = new Menu ();

		submenu.append_section (null, section_menu);

		var section = new Menu ();
		section.append ("Character Map", "indicator.map");
		section.append ("Keyboard Layout Chart", "indicator.chart");
		section.append ("Text Entry Settings...", "indicator.settings");
		submenu.append_section (null, section);

		var indicator = new MenuItem.submenu ("x", submenu);
		indicator.set_attribute ("x-canonical-type", "s", "com.canonical.indicator.root");
		indicator.set_detailed_action ("indicator.indicator");
		menu.append_item (indicator);

		return menu;
	}

	[DBus (visible = false)]
	private IBus.Bus get_ibus () {
		if (this.ibus == null) {
			IBus.init ();

			this.ibus = new IBus.Bus ();
		}

		return this.ibus;
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
					var language = Xkl.get_language_name (name);
					var country = Xkl.get_country_name (name);

					if (language != null && country != null) {
						name = @"$language ($country)";
					} else if (language != null) {
						name = language;
					} else if (country != null) {
						name = country;
					}
				}
				else if (type == "ibus") {
					var ibus = get_ibus ();
					string[] names = { name, null };
					var engines = ibus.get_engines_by_names (names);
					var engine = engines[0];
					var longname = engine.longname;
					var language = Xkl.get_language_name (engine.language);
					var country = Xkl.get_country_name (engine.language);

					if (language != null) {
						name = @"$language ($longname)";
					} else if (country != null) {
						name = @"$country ($longname)";
					} else {
						name = longname;
					}
				}

				var menu_item = new MenuItem (name, "indicator.current");
				menu_item.set_attribute (Menu.ATTRIBUTE_TARGET, "u", i);
				menu_item.set_icon (get_icon (i));
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

		return this.sources_menu;
	}

	[DBus (visible = false)]
	public MenuModel get_menu_model () {
		if (this.menu_model == null) {
			this.menu_model = create_menu_model (get_sources_menu ());
		}

		return this.menu_model;
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
		this.icons = null;
		update_sources_menu ();
		update_indicator_action ();
	}

	[DBus (visible = false)]
	private void handle_activate_map (Variant? parameter) {
		try {
			Process.spawn_command_line_async ("gucharmap");
		} catch {
			warn_if_reached ();
		}
	}

	[DBus (visible = false)]
	private void handle_activate_chart (Variant? parameter) {
		var layout = "us";

		var current = this.source_settings.get_uint ("current");
		Variant array;
		this.source_settings.get ("sources", "@a(ss)", out array);

		if (current < array.n_children ()) {
			string type;
			string name;

			array.get_child (current, "(ss)", out type, out name);

			if (type == "xkb") {
				layout = name;
			}
		}

		try {
			Process.spawn_command_line_async (@"gkbd-keyboard-display -l $layout");
		} catch {
			warn_if_reached ();
		}
	}

	[DBus (visible = false)]
	private void handle_activate_settings (Variant? parameter) {
		try {
			Process.spawn_command_line_async ("gnome-control-center region layouts");
		} catch {
			warn_if_reached ();
		}
	}

	[DBus (visible = false)]
	private void handle_bus_acquired (DBusConnection connection, string name) {
		try {
			connection.export_action_group ("/com/canonical/indicator/keyboard", get_action_group ());
			connection.export_menu_model ("/com/canonical/indicator/keyboard/desktop", get_menu_model ());
		} catch {
			warn_if_reached ();
		}
	}

	[DBus (visible = false)]
	private void handle_name_lost (DBusConnection connection, string name) {
		this.loop.quit ();
		this.loop = null;
	}

	[DBus (visible = false)]
	public static int main (string[] args) {
		Gtk.init (ref args);
		new Service ("--force" in args);
		return 0;
	}
}
