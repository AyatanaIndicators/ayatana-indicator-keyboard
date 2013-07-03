const int TIMEOUT_S = 1;
const int TIMEOUT_MS = 1000;

[DBus (name = "com.canonical.indicator.keyboard.test")]
public class Service : Object {

	[DBus (visible = false)]
	private string? _command;

	[DBus (visible = false)]
	public string? command {
		get { return _command; }
	}

	public void execute (string command) {
		this._command = command;

		var pspec = this.get_class ().find_property ("command");

		if (pspec != null) {
			this.notify["command"] ((!) pspec);
		}
	}
}

struct Fixture {
	TestDBus? bus;
	uint service_name;
	DBusConnection? connection;
	Service? service;
	uint object_name;
}

static void start_service (Fixture *fixture) {
	if (fixture.connection != null) {
		try {
			fixture.service = new Service ();
			fixture.object_name = ((!) fixture.connection).register_object ("/com/canonical/indicator/keyboard/test", fixture.service);
		} catch (IOError error) {
			fixture.connection = null;
			fixture.service = null;
			fixture.object_name = 0;

			Test.message ("error: %s", error.message);
			Test.fail ();
		}
	}
}

static void begin_test (void *data) {
	var fixture = (Fixture *) data;

	fixture.bus = new TestDBus (TestDBusFlags.NONE);
	((!) fixture.bus).add_service_dir (SERVICE_DIR);
	((!) fixture.bus).up ();

	var loop = new MainLoop (null, false);

	fixture.service_name = Bus.own_name (BusType.SESSION,
	                                     "com.canonical.indicator.keyboard.test",
	                                     BusNameOwnerFlags.ALLOW_REPLACEMENT | BusNameOwnerFlags.REPLACE,
	                                     (connection, name) => {
	                                         if (loop.is_running ()) {
	                                             fixture.connection = connection;

	                                             start_service (fixture);

	                                             loop.quit ();
	                                         }
	                                     },
	                                     null,
	                                     (connection, name) => {
	                                         if (loop.is_running ()) {
	                                             fixture.connection = null;
	                                             fixture.service = null;
	                                             fixture.object_name = 0;

	                                             loop.quit ();
	                                         }
	                                     });

	loop.run ();

	if (fixture.connection == null) {
		Test.message ("error: Unable to connect to com.canonical.indicator.keyboard.test.");
		Test.fail ();
	}
}

static void end_test (void *data) {
	var fixture = (Fixture *) data;

	if (fixture.object_name != 0) {
		((!) fixture.connection).unregister_object (fixture.object_name);
		fixture.object_name = 0;
	}

	if (fixture.service_name != 0) {
		Bus.unown_name (fixture.service_name);
		fixture.service_name = 0;
	}

	fixture.service = null;
	fixture.connection = null;

	if (fixture.bus != null) {
		((!) fixture.bus).down ();
		fixture.bus = null;
	}
}

static void test_activate_input_source (void *data) {
	var fixture = (Fixture *) data;

	if (fixture.object_name == 0) {
		Test.message ("error: Test fixture not initialized.");
		Test.fail ();
		return;
	}

	try {
		var current = 0;
		var sources = "[('xkb', 'us'), ('xkb', 'ca+eng'), ('xkb', 'epo'), ('ibus', 'pinyin')]";
		Process.spawn_command_line_sync (@"gsettings set org.gnome.desktop.input-sources current $current");
		Process.spawn_command_line_sync (@"gsettings set org.gnome.desktop.input-sources sources \"$sources\"");
	} catch (SpawnError error) {
		Test.message ("error: %s", error.message);
		Test.fail ();
		return;
	}

	var action_group = DBusActionGroup.get ((!) fixture.connection,
	                                        "com.canonical.indicator.keyboard",
	                                        "/com/canonical/indicator/keyboard");
	action_group.activate_action ("current", new Variant.uint32 (2));
	Posix.sleep (TIMEOUT_S);

	try {
		string current;
		Process.spawn_command_line_sync ("gsettings get org.gnome.desktop.input-sources current", out current);
		assert (strcmp (current, "uint32 2\n") == 0);
	} catch (SpawnError error) {
		Test.message ("error: %s", error.message);
		Test.fail ();
		return;
	}
}

static void test_activate_character_map (void *data) {
	var fixture = (Fixture *) data;

	if (fixture.object_name == 0) {
		Test.message ("error: Test fixture not initialized.");
		Test.fail ();
		return;
	}

	var loop = new MainLoop (null, false);

	var signal_name = ((!) fixture.service).notify["command"].connect ((pspec) => {
		loop.quit ();
	});

	var action_group = DBusActionGroup.get ((!) fixture.connection,
	                                        "com.canonical.indicator.keyboard",
	                                        "/com/canonical/indicator/keyboard");
	action_group.activate_action ("map", null);

	var source = Timeout.add_seconds (TIMEOUT_S, () => { loop.quit (); return false; });
	loop.run ();
	Source.remove (source);
	((!) fixture.service).disconnect (signal_name);

	assert (strcmp ((!) ((!) fixture.service).command, "'gucharmap '") == 0);
}

static void test_activate_keyboard_layout_chart (void *data) {
	var fixture = (Fixture *) data;

	if (fixture.object_name == 0) {
		Test.message ("error: Test fixture not initialized.");
		Test.fail ();
		return;
	}

	try {
		var current = 1;
		var sources = "[('xkb', 'us'), ('xkb', 'ca+eng'), ('xkb', 'epo'), ('ibus', 'pinyin')]";
		Process.spawn_command_line_sync (@"gsettings set org.gnome.desktop.input-sources current $current");
		Process.spawn_command_line_sync (@"gsettings set org.gnome.desktop.input-sources sources \"$sources\"");
	} catch (SpawnError error) {
		Test.message ("error: %s", error.message);
		Test.fail ();
		return;
	}

	var loop = new MainLoop (null, false);

	var signal_name = ((!) fixture.service).notify["command"].connect ((pspec) => {
		loop.quit ();
	});

	var action_group = DBusActionGroup.get ((!) fixture.connection,
	                                        "com.canonical.indicator.keyboard",
	                                        "/com/canonical/indicator/keyboard");

	action_group.activate_action ("chart", null);

	var source = Timeout.add_seconds (TIMEOUT_S, () => { loop.quit (); return false; });
	loop.run ();
	Source.remove (source);
	((!) fixture.service).disconnect (signal_name);

	assert (strcmp ((!) ((!) fixture.service).command, "'gkbd-keyboard-display -l ca\teng'") == 0);
}

static void test_activate_text_entry_settings (void *data) {
	var fixture = (Fixture *) data;

	if (fixture.object_name == 0) {
		Test.message ("error: Test fixture not initialized.");
		Test.fail ();
		return;
	}

	var loop = new MainLoop (null, false);

	var signal_name = ((!) fixture.service).notify["command"].connect ((pspec) => {
		loop.quit ();
	});

	var action_group = DBusActionGroup.get ((!) fixture.connection,
	                                        "com.canonical.indicator.keyboard",
	                                        "/com/canonical/indicator/keyboard");

	action_group.activate_action ("settings", null);

	var source = Timeout.add_seconds (TIMEOUT_S, () => { loop.quit (); return false; });
	loop.run ();
	Source.remove (source);
	((!) fixture.service).disconnect (signal_name);

	assert (strcmp ((!) ((!) fixture.service).command, "'gnome-control-center region layouts'") == 0);
}

static void test_migration (void *data) {
	var fixture = (Fixture *) data;

	if (fixture.object_name == 0) {
		Test.message ("error: Test fixture not initialized.");
		Test.fail ();
		return;
	}

	try {
		var migrated = false;
		var sources = "[('xkb', 'us')]";
		var layouts = "['us', 'ca\teng', 'epo']";
		Process.spawn_command_line_sync (@"gsettings set com.canonical.indicator.keyboard migrated $migrated");
		Process.spawn_command_line_sync (@"gsettings set org.gnome.desktop.input-sources sources \"$sources\"");
		Process.spawn_command_line_sync (@"gsettings set org.gnome.libgnomekbd.keyboard layouts \"$layouts\"");
	} catch (SpawnError error) {
		Test.message ("error: %s", error.message);
		Test.fail ();
		return;
	}

	try {
		var cancellable = new Cancellable ();

		var source = Timeout.add_seconds (TIMEOUT_S, () => { cancellable.cancel (); return false; });

		var dbus_proxy = new DBusProxy.sync ((!) fixture.connection,
		                                     DBusProxyFlags.NONE,
		                                     null,
		                                     "org.freedesktop.DBus",
		                                     "/",
		                                     "org.freedesktop.DBus",
		                                     cancellable);

		Source.remove (source);

		if (cancellable.is_cancelled ()) {
			Test.message ("error: Unable to connect to org.freedesktop.DBus.");
			Test.fail ();
			return;
		}

		dbus_proxy.call_sync ("StartServiceByName", new Variant ("(su)", "com.canonical.indicator.keyboard", 0), DBusCallFlags.NONE, TIMEOUT_MS);
	} catch (Error error) {
		Test.message ("error: %s", error.message);
		Test.fail ();
		return;
	}

	try {
		string sources;
		Process.spawn_command_line_sync ("gsettings get org.gnome.desktop.input-sources sources", out sources);
		assert (strcmp (sources, "[('xkb', 'us'), ('xkb', 'ca+eng'), ('xkb', 'epo')]\n") == 0);
	} catch (SpawnError error) {
		Test.message ("error: %s", error.message);
		Test.fail ();
		return;
	}
}

static void test_no_migration (void *data) {
	var fixture = (Fixture *) data;

	if (fixture.object_name == 0) {
		Test.message ("error: Test fixture not initialized.");
		Test.fail ();
		return;
	}

	try {
		var migrated = true;
		var sources = "[('xkb', 'us')]";
		var layouts = "['us', 'ca\teng', 'epo']";
		Process.spawn_command_line_sync (@"gsettings set com.canonical.indicator.keyboard migrated $migrated");
		Process.spawn_command_line_sync (@"gsettings set org.gnome.desktop.input-sources sources \"$sources\"");
		Process.spawn_command_line_sync (@"gsettings set org.gnome.libgnomekbd.keyboard layouts \"$layouts\"");
	} catch (SpawnError error) {
		Test.message ("error: %s", error.message);
		Test.fail ();
		return;
	}

	try {
		var cancellable = new Cancellable ();

		var source = Timeout.add_seconds (TIMEOUT_S, () => { cancellable.cancel (); return false; });

		var dbus_proxy = new DBusProxy.sync ((!) fixture.connection,
		                                     DBusProxyFlags.NONE,
		                                     null,
		                                     "org.freedesktop.DBus",
		                                     "/",
		                                     "org.freedesktop.DBus",
		                                     cancellable);

		Source.remove (source);

		if (cancellable.is_cancelled ()) {
			Test.message ("error: Unable to connect to org.freedesktop.DBus.");
			Test.fail ();
			return;
		}

		dbus_proxy.call_sync ("StartServiceByName", new Variant ("(su)", "com.canonical.indicator.keyboard", 0), DBusCallFlags.NONE, TIMEOUT_MS);
	} catch (Error error) {
		Test.message ("error: %s", error.message);
		Test.fail ();
		return;
	}

	try {
		string sources;
		Process.spawn_command_line_sync ("gsettings get org.gnome.desktop.input-sources sources", out sources);
		assert (strcmp (sources, "[('xkb', 'us')]\n") == 0);
	} catch (SpawnError error) {
		Test.message ("error: %s", error.message);
		Test.fail ();
		return;
	}
}

static void test_update_visible (void *data) {
	var fixture = (Fixture *) data;

	if (fixture.object_name == 0) {
		Test.message ("error: Test fixture not initialized.");
		Test.fail ();
		return;
	}

	bool visible;

	try {
		visible = true;
		Process.spawn_command_line_sync (@"gsettings set com.canonical.indicator.keyboard visible $visible");
	} catch (SpawnError error) {
		Test.message ("error: %s", error.message);
		Test.fail ();
		return;
	}

	var loop = new MainLoop (null, false);

	var action_group = DBusActionGroup.get ((!) fixture.connection,
	                                        "com.canonical.indicator.keyboard",
	                                        "/com/canonical/indicator/keyboard");
	var signal_name = action_group.action_added["indicator"].connect ((action) => {
		loop.quit ();
	});
	action_group.list_actions ();

	var source = Timeout.add_seconds (TIMEOUT_S, () => { loop.quit (); return false; });
	loop.run ();
	Source.remove (source);
	action_group.disconnect (signal_name);

	var state = action_group.get_action_state ("indicator");
	assert (((!) state).lookup ("visible", "b", out visible));
	assert (visible);

	loop = new MainLoop (null, false);
	signal_name = action_group.action_state_changed["indicator"].connect ((action, state) => {
		loop.quit ();
	});

	try {
		visible = false;
		Process.spawn_command_line_sync (@"gsettings set com.canonical.indicator.keyboard visible $visible");
	} catch (SpawnError error) {
		Test.message ("error: %s", error.message);
		Test.fail ();
		return;
	}

	source = Timeout.add_seconds (TIMEOUT_S, () => { loop.quit (); return false; });
	loop.run ();
	Source.remove (source);
	action_group.disconnect (signal_name);

	state = action_group.get_action_state ("indicator");
	assert (((!) state).lookup ("visible", "b", out visible));
	assert (!visible);

	loop = new MainLoop (null, false);
	signal_name = action_group.action_state_changed["indicator"].connect ((action, state) => {
		loop.quit ();
	});

	try {
		visible = true;
		Process.spawn_command_line_sync (@"gsettings set com.canonical.indicator.keyboard visible $visible");
	} catch (SpawnError error) {
		Test.message ("error: %s", error.message);
		Test.fail ();
		return;
	}

	source = Timeout.add_seconds (TIMEOUT_S, () => { loop.quit (); return false; });
	loop.run ();
	Source.remove (source);
	action_group.disconnect (signal_name);

	state = action_group.get_action_state ("indicator");
	assert (((!) state).lookup ("visible", "b", out visible));
	assert (visible);
}

static void test_update_input_source (void *data) {
}

static void test_update_input_sources (void *data) {
}

public int main (string[] args) {
	Environment.set_variable ("DCONF_PROFILE", DCONF_PROFILE, true);

	Test.init (ref args, null);

	var suite = new TestSuite ("indicator-keyboard");

	suite.add (new TestCase ("activate-input-source", begin_test, test_activate_input_source, end_test, sizeof (Fixture)));
	suite.add (new TestCase ("activate-character-map", begin_test, test_activate_character_map, end_test, sizeof (Fixture)));
	suite.add (new TestCase ("activate-keyboard-layout-chart", begin_test, test_activate_keyboard_layout_chart, end_test, sizeof (Fixture)));
	suite.add (new TestCase ("activate-text-entry-settings", begin_test, test_activate_text_entry_settings, end_test, sizeof (Fixture)));
	suite.add (new TestCase ("migration", begin_test, test_migration, end_test, sizeof (Fixture)));
	suite.add (new TestCase ("no-migration", begin_test, test_no_migration, end_test, sizeof (Fixture)));
	suite.add (new TestCase ("update-visible", begin_test, test_update_visible, end_test, sizeof (Fixture)));
	suite.add (new TestCase ("update-input-source", begin_test, test_update_input_source, end_test, sizeof (Fixture)));
	suite.add (new TestCase ("update-input-sources", begin_test, test_update_input_sources, end_test, sizeof (Fixture)));

	TestSuite.get_root ().add_suite (suite);

	return Test.run ();
}
