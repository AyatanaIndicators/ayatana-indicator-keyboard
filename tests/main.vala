const int LONG_TIMEOUT = 10;
const int SHORT_TIMEOUT = 1000;

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
		Test.message ("Unable to connect to 'com.canonical.indicator.keyboard.test'.");
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
}

static void test_activate_character_map (void *data) {
	var fixture = (Fixture *) data;

	if (fixture.object_name == 0) {
		Test.message ("Invalid test fixture.");
		Test.fail ();
		return;
	}

	/* XXX: bootstrap the settings with what we want to test with. */
	var settings = new Settings ("org.gnome.desktop.input-sources");
	settings.set_uint ("current", 0);
	settings.set_value ("sources", new Variant.parsed ("[('xkb', 'us'), ('ibus', 'pinyin')]"));

	var cancellable = new Cancellable ();
	DBusProxy action_proxy;
	DBusProxy menu_proxy;

	var source = Timeout.add_seconds (LONG_TIMEOUT, () => { cancellable.cancel (); return false; });

	try {
		action_proxy = new DBusProxy.for_bus_sync (BusType.SESSION,
		                                           DBusProxyFlags.NONE,
		                                           null,
		                                           "com.canonical.indicator.keyboard",
		                                           "/com/canonical/indicator/keyboard",
		                                           "org.gtk.Actions",
		                                           cancellable);
	} catch (Error error) {
		Test.message ("error: %s", error.message);
		Test.fail ();
		return;
	}

	Source.remove (source);

	if (cancellable.is_cancelled ()) {
		Test.message ("Unable to connect to 'com.canonical.indicator.keyboard'.\n");
		Test.fail ();
		return;
	}

	source = Timeout.add_seconds (LONG_TIMEOUT, () => { cancellable.cancel (); return false; });

	try {
		menu_proxy = new DBusProxy.for_bus_sync (BusType.SESSION,
		                                         DBusProxyFlags.NONE,
		                                         null,
		                                         "com.canonical.indicator.keyboard",
		                                         "/com/canonical/indicator/keyboard/desktop",
		                                         "org.gtk.Menus",
		                                         cancellable);
	} catch (Error error) {
		Test.message ("error: %s", error.message);
		Test.fail ();
		return;
	}

	Source.remove (source);

	if (cancellable.is_cancelled ()) {
		Test.message ("Unable to connect to 'com.canonical.indicator.keyboard'.\n");
		Test.fail ();
		return;
	}

	/* XXX: This is just to make sure the GSettings are shared between processes. */
	{
		var builder = new VariantBuilder (new VariantType ("(au)"));
		builder.open (new VariantType ("au"));
		builder.add ("u", 0);
		builder.add ("u", 1);
		builder.close ();
		var variant = builder.end ();
		stdout.printf ("%s\n", menu_proxy.call_sync ("Start", variant, DBusCallFlags.NONE, SHORT_TIMEOUT).print (true));
		menu_proxy.call_sync ("End", variant, DBusCallFlags.NONE, SHORT_TIMEOUT);
	}

	var loop = new MainLoop (null, false);

	var signal_name = ((!) fixture.service).notify["command"].connect ((pspec) => {
		loop.quit ();
	});

	try {
		var builder = new VariantBuilder (new VariantType ("(sava{sv})"));
		builder.add ("s", "chart");
		builder.add_value (new Variant.parsed ("@av []"));
		builder.add_value (new Variant.parsed ("@a{sv} {}"));
		action_proxy.call_sync ("Activate", builder.end (), DBusCallFlags.NONE, SHORT_TIMEOUT);
	} catch (Error error) {
		Test.message ("error: %s", error.message);
		Test.fail ();
		return;
	}

	source = Timeout.add_seconds (LONG_TIMEOUT, () => { loop.quit (); return false; });
	loop.run ();
	Source.remove (source);
	((!) fixture.service).disconnect (signal_name);
}

static void test_activate_keyboard_layout_chart (void *data) {
}

static void test_activate_text_entry_settings (void *data) {
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
	suite.add (new TestCase ("update-input-source", begin_test, test_update_input_source, end_test, sizeof (Fixture)));
	suite.add (new TestCase ("update-input-sources", begin_test, test_update_input_sources, end_test, sizeof (Fixture)));

	TestSuite.get_root ().add_suite (suite);

	return Test.run ();
}
