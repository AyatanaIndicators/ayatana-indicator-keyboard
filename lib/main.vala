[DBus (name = "com.canonical.indicator.keyboard")]
public class Indicator.Keyboard.Service : GLib.Object {

	private GLib.MainLoop loop;
	private GLib.ActionGroup action_group;
	private GLib.MenuModel menu_model;

	public Service (bool force) {
		GLib.Bus.own_name (GLib.BusType.SESSION,
		                   "com.canonical.indicator.keyboard",
		                   GLib.BusNameOwnerFlags.ALLOW_REPLACEMENT | (force ? GLib.BusNameOwnerFlags.REPLACE : 0),
		                   this.handle_bus_acquired,
		                   null,
		                   this.handle_name_lost);

		this.loop = new GLib.MainLoop ();
		this.loop.run ();
	}

	[DBus (visible = false)]
	protected virtual GLib.ActionGroup create_action_group () {
		const GLib.ActionEntry[] entries = {
			{ "indicator", null, null, "('X', '', '', true)", null }
		};

		var group = new GLib.SimpleActionGroup ();

		group.add_entries (entries, null);

		return group;
	}

	[DBus (visible = false)]
	protected virtual GLib.MenuModel create_menu_model () {
		var menu = new GLib.Menu ();

		var submenu = new GLib.Menu ();

		var section = new GLib.Menu ();
		section.append ("Chinese - Chewing", null);
		section.append ("Chinese - Pinyin", null);
		section.append ("Chinese - tonepy", null);
		section.append ("USA - Macintosh", null);
		section.append ("USA - Dvorak", null);
		submenu.append_section (null, section);

		section = new GLib.Menu ();
		section.append ("Character Map", null);
		section.append ("Keyboard Layout Chart", null);
		section.append ("Text Entry Settings...", null);
		submenu.append_section (null, section);

		var indicator = new GLib.MenuItem.submenu ("x", submenu);
		indicator.set_attribute ("x-canonical-type", "s", "com.canonical.indicator.root");
		indicator.set_detailed_action ("indicator");
		menu.append_item (indicator);

		return menu;
	}

	[DBus (visible = false)]
	public GLib.ActionGroup get_action_group () {
		if (this.action_group == null)
			this.action_group = create_action_group ();

		return this.action_group;
	}

	[DBus (visible = false)]
	public GLib.MenuModel get_menu_model () {
		if (this.menu_model == null)
			this.menu_model = create_menu_model ();

		return this.menu_model;
	}

	[DBus (visible = false)]
	private void handle_bus_acquired (GLib.DBusConnection connection, string name) {
		try {
			connection.export_action_group ("/com/canonical/indicator/keyboard", get_action_group ());
			connection.export_menu_model ("/com/canonical/indicator/keyboard/desktop", get_menu_model ());
		} catch {
			GLib.warn_if_reached ();
		}
	}

	[DBus (visible = false)]
	private void handle_name_lost (GLib.DBusConnection connection, string name) {
		this.loop.quit ();
		this.loop = null;
	}

	public static int main (string[] args) {
		new Service ("--force" in args);
		return 0;
	}
}
