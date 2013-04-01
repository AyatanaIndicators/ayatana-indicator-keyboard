[DBus (name = "com.canonical.indicator.keyboard")]
public class Indicator.Keyboard.Service : Object {

	private MainLoop loop;
	private Settings settings;
	private ActionGroup action_group;
	private MenuModel menu_model;

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
	private void handle_activate_map (Variant? parameter) {
		try {
			Process.spawn_command_line_async ("gucharmap");
		} catch {
			warn_if_reached ();
		}
	}

	[DBus (visible = false)]
	private void handle_activate_chart (Variant? parameter) {
		try {
			Process.spawn_command_line_async ("gkbd-keyboard-display -l fr");
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
	protected virtual ActionGroup create_action_group () {
		var group = new SimpleActionGroup ();

		var state = new Variant.parsed ("('x', '', '', true)");
		var action = new SimpleAction.stateful ("indicator", null, state);
		group.insert (action);

		if (this.settings != null) {
			group.insert (this.settings.create_action ("current"));
		}

		action = new SimpleAction ("map", null);
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
	protected virtual MenuModel create_menu_model () {
		/* IBus.init (); */
		/* var bus = new IBus.Bus (); */
		/* var engines = bus.list_engines (); */
		/* var context = new IBus.InputContext (bus.current_input_context (), bus.get_connection ()); */

		/* foreach (var engine in engines) { */
			/* stdout.printf (@"$(engine.name) $(engine.longname)\n"); */
		/* } */

		/* stdout.printf (@"global engine = $(bus.get_global_engine ().name)\n"); */
		/* stdout.printf (@"current engine = $(context.get_engine ().name)\n"); */

		/* bus.set_global_engine ("pinyin"); */
		/* context.set_engine ("pinyin"); */

		var menu = new Menu ();

		var submenu = new Menu ();

		var section = new Menu ();

		if (this.settings != null) {
			VariantIter iter;
			string type;
			string name;
			uint i;

			this.settings.get ("sources", "a(ss)", out iter);

			for (i = 0; iter.next ("(ss)", out type, out name); i++) {
				MenuItem menu_item = new MenuItem (name, "indicator.current");
				menu_item.set_attribute (Menu.ATTRIBUTE_TARGET, "u", i);
				section.append_item (menu_item);
			}
		}

		submenu.append_section (null, section);

		section = new Menu ();
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
