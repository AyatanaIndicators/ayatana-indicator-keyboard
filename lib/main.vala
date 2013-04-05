[DBus (name = "com.canonical.indicator.keyboard")]
public class Indicator.Keyboard.Service : Object {

	private MainLoop loop;
	private Settings settings;
	private ActionGroup action_group;
	private MenuModel menu_model;
	private IBus.Bus ibus;

	[DBus (visible = false)]
	public Service (bool force) {
		Bus.own_name (BusType.SESSION,
		              "com.canonical.indicator.keyboard",
		              BusNameOwnerFlags.ALLOW_REPLACEMENT | (force ? BusNameOwnerFlags.REPLACE : 0),
		              this.handle_bus_acquired,
		              null,
		              this.handle_name_lost);

		this.settings = new Settings ("org.gnome.desktop.input-sources");

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
		context.set_source_rgba (1.0, 0.0, 0.0, 1.0);
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

		return Gdk.pixbuf_get_from_surface (surface, 0, 0, W, H);
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
		Variant array;
		string type;
		string name;

		var current = this.settings.get_uint ("current");
		this.settings.get ("sources", "@a(ss)", out array);
		array.get_child (current, "(ss)", out type, out name);

		if (type == "xkb") {
			layout = name;
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
			Process.spawn_command_line_async ("gnome-control-center region");
		} catch {
			warn_if_reached ();
		}
	}

	[DBus (visible = false)]
	private void handle_activate_foo (Variant? parameter) {
		var window = new Gtk.Window ();

		window.add (new Gtk.Image.from_gicon (create_icon ("US"), Gtk.IconSize.INVALID));

		window.show_all ();
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
	protected virtual ActionGroup create_action_group () {
		var group = new SimpleActionGroup ();

		var state = new Variant.parsed ("('x', '', '', true)");
		var action = new SimpleAction.stateful ("indicator", null, state);
		group.insert (action);

		group.insert (this.settings.create_action ("current"));

		action = new SimpleAction ("map", null);
		action.activate.connect (this.handle_activate_map);
		group.insert (action);

		action = new SimpleAction ("chart", null);
		action.activate.connect (this.handle_activate_chart);
		group.insert (action);

		action = new SimpleAction ("settings", null);
		action.activate.connect (this.handle_activate_settings);
		group.insert (action);

		action = new SimpleAction ("foo", null);
		action.activate.connect (this.handle_activate_foo);
		group.insert (action);

		return group;
	}

	[DBus (visible = false)]
	protected virtual MenuModel create_menu_model () {
		var menu = new Menu ();

		var submenu = new Menu ();

		var section = new Menu ();

		VariantIter iter;
		string type;
		string name;

		this.settings.get ("sources", "a(ss)", out iter);

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
			section.append_item (menu_item);
		}

		submenu.append_section (null, section);

		section = new Menu ();
		section.append ("Character Map", "indicator.map");
		section.append ("Keyboard Layout Chart", "indicator.chart");
		section.append ("Text Entry Settings...", "indicator.settings");
		section.append ("foo", "indicator.foo");
		submenu.append_section (null, section);

		var indicator = new MenuItem.submenu ("x", submenu);
		indicator.set_attribute ("x-canonical-type", "s", "com.canonical.indicator.root");
		indicator.set_detailed_action ("indicator.indicator");
		menu.append_item (indicator);

		return menu;
	}

	[DBus (visible = false)]
	public ActionGroup get_action_group () {
		if (this.action_group == null) {
			this.action_group = create_action_group ();
		}

		return this.action_group;
	}

	[DBus (visible = false)]
	public MenuModel get_menu_model () {
		if (this.menu_model == null) {
			this.menu_model = create_menu_model ();
		}

		return this.menu_model;
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
