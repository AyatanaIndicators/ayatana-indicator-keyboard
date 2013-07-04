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
	var loop = new MainLoop (null, false);
	var signal_name = action_group.action_state_changed["current"].connect ((action, state) => {
		loop.quit ();
	});

	action_group.list_actions ();
	action_group.activate_action ("current", new Variant.uint32 (2));

	var source = Timeout.add_seconds (TIMEOUT_S, () => { loop.quit (); return false; });
	loop.run ();
	Source.remove (source);
	action_group.disconnect (signal_name);

	var state = action_group.get_action_state ("current");
	var current = ((!) state).get_uint32 ();
	assert (current == 2);

	try {
		string output;
		Process.spawn_command_line_sync ("gsettings get org.gnome.desktop.input-sources current", out output);
		assert (strcmp (output, "uint32 2\n") == 0);
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

	var action_group = DBusActionGroup.get ((!) fixture.connection,
	                                        "com.canonical.indicator.keyboard",
	                                        "/com/canonical/indicator/keyboard");
	var loop = new MainLoop (null, false);
	var signal_name = ((!) fixture.service).notify["command"].connect ((pspec) => {
		loop.quit ();
	});

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

	var action_group = DBusActionGroup.get ((!) fixture.connection,
	                                        "com.canonical.indicator.keyboard",
	                                        "/com/canonical/indicator/keyboard");
	var loop = new MainLoop (null, false);
	var signal_name = ((!) fixture.service).notify["command"].connect ((pspec) => {
		loop.quit ();
	});

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

	var action_group = DBusActionGroup.get ((!) fixture.connection,
	                                        "com.canonical.indicator.keyboard",
	                                        "/com/canonical/indicator/keyboard");
	var loop = new MainLoop (null, false);
	var signal_name = ((!) fixture.service).notify["command"].connect ((pspec) => {
		loop.quit ();
	});

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

	var action_group = DBusActionGroup.get ((!) fixture.connection,
	                                        "com.canonical.indicator.keyboard",
	                                        "/com/canonical/indicator/keyboard");
	var loop = new MainLoop (null, false);
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
	var loop = new MainLoop (null, false);
	var signal_name = action_group.action_state_changed["current"].connect ((action, state) => {
		loop.quit ();
	});

	action_group.list_actions ();

	try {
		var current = 1;
		Process.spawn_command_line_sync (@"gsettings set org.gnome.desktop.input-sources current $current");
	} catch (SpawnError error) {
		Test.message ("error: %s", error.message);
		Test.fail ();
		return;
	}

	var source = Timeout.add_seconds (TIMEOUT_S, () => { loop.quit (); return false; });
	loop.run ();
	Source.remove (source);
	action_group.disconnect (signal_name);

	var state = action_group.get_action_state ("current");
	var current = ((!) state).get_uint32 ();
	assert (current == 1);

	try {
		string output;
		Process.spawn_command_line_sync ("gsettings get org.gnome.desktop.input-sources current", out output);
		assert (strcmp (output, "uint32 1\n") == 0);
	} catch (SpawnError error) {
		Test.message ("error: %s", error.message);
		Test.fail ();
		return;
	}

	loop = new MainLoop (null, false);
	signal_name = action_group.action_state_changed["current"].connect ((action, state) => {
		loop.quit ();
	});

	try {
		current = 0;
		Process.spawn_command_line_sync (@"gsettings set org.gnome.desktop.input-sources current $current");
	} catch (SpawnError error) {
		Test.message ("error: %s", error.message);
		Test.fail ();
		return;
	}

	source = Timeout.add_seconds (TIMEOUT_S, () => { loop.quit (); return false; });
	loop.run ();
	Source.remove (source);
	action_group.disconnect (signal_name);

	state = action_group.get_action_state ("current");
	current = ((!) state).get_uint32 ();
	assert (current == 0);

	try {
		string output;
		Process.spawn_command_line_sync ("gsettings get org.gnome.desktop.input-sources current", out output);
		assert (strcmp (output, "uint32 0\n") == 0);
	} catch (SpawnError error) {
		Test.message ("error: %s", error.message);
		Test.fail ();
		return;
	}
}

static void test_update_input_sources (void *data) {
	var fixture = (Fixture *) data;

	if (fixture.object_name == 0) {
		Test.message ("error: Test fixture not initialized.");
		Test.fail ();
		return;
	}

	try {
		var current = 0;
		var sources = "[('xkb', 'us')]";
		Process.spawn_command_line_sync (@"gsettings set org.gnome.desktop.input-sources current $current");
		Process.spawn_command_line_sync (@"gsettings set org.gnome.desktop.input-sources sources \"$sources\"");
	} catch (SpawnError error) {
		Test.message ("error: %s", error.message);
		Test.fail ();
		return;
	}

	var menu_model = DBusMenuModel.get ((!) fixture.connection,
	                                    "com.canonical.indicator.keyboard",
	                                    "/com/canonical/indicator/keyboard/desktop");
	var loop = new MainLoop (null, false);
	var signal_name = menu_model.items_changed.connect ((position, removed, added) => {
		loop.quit ();
	});

	menu_model.get_n_items ();

	var source = Timeout.add_seconds (TIMEOUT_S, () => { loop.quit (); return false; });
	loop.run ();
	Source.remove (source);
	menu_model.disconnect (signal_name);

	var menu = menu_model.get_item_link (0, Menu.LINK_SUBMENU);
	loop = new MainLoop (null, false);
	signal_name = menu.items_changed.connect ((position, removed, added) => {
		loop.quit ();
	});

	menu.get_n_items ();

	source = Timeout.add_seconds (TIMEOUT_S, () => { loop.quit (); return false; });
	loop.run ();
	Source.remove (source);
	menu.disconnect (signal_name);

	var section = menu.get_item_link (0, Menu.LINK_SECTION);
	loop = new MainLoop (null, false);
	signal_name = section.items_changed.connect ((position, removed, added) => {
		loop.quit ();
	});

	section.get_n_items ();

	source = Timeout.add_seconds (TIMEOUT_S, () => { loop.quit (); return false; });
	loop.run ();
	Source.remove (source);
	section.disconnect (signal_name);

	string label;

	assert (section.get_n_items () == 1);
	section.get_item_attribute (0, Menu.ATTRIBUTE_LABEL, "s", out label);
	assert (strcmp (label, "English (US)") == 0);

	loop = new MainLoop (null, false);
	signal_name = section.items_changed.connect ((position, removed, added) => {
		if (section.get_n_items () == 4) {
			loop.quit ();
		}
	});

	try {
		var sources = "[('xkb', 'us'), ('xkb', 'ca+eng'), ('xkb', 'epo'), ('ibus', 'pinyin')]";
		Process.spawn_command_line_sync (@"gsettings set org.gnome.desktop.input-sources sources \"$sources\"");
	} catch (SpawnError error) {
		Test.message ("error: %s", error.message);
		Test.fail ();
		return;
	}

	source = Timeout.add_seconds (TIMEOUT_S, () => { loop.quit (); return false; });
	loop.run ();
	Source.remove (source);
	section.disconnect (signal_name);

	assert (section.get_n_items () == 4);
	section.get_item_attribute (0, Menu.ATTRIBUTE_LABEL, "s", out label);
	assert (strcmp (label, "English (US)") == 0);
	section.get_item_attribute (1, Menu.ATTRIBUTE_LABEL, "s", out label);
	assert (strcmp (label, "English (Canada)") == 0);
	section.get_item_attribute (2, Menu.ATTRIBUTE_LABEL, "s", out label);
	assert (strcmp (label, "Esperanto") == 0);
	section.get_item_attribute (3, Menu.ATTRIBUTE_LABEL, "s", out label);
	assert (strcmp (label, "Pinyin") == 0);
}

public int main (string[] args) {
	Environment.set_variable ("DCONF_PROFILE", DCONF_PROFILE, true);
	Environment.set_variable ("LC_ALL", "C", true);

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
