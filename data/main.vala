int main (string[] args) {
	var force = false;
	var colour = "grey";
	string output_path = null;
	string no_subscript_path = null;
	string with_subscript_path = null;

	OptionEntry[] options = new OptionEntry[6];
	options[0] = { "force", 'f', 0, OptionArg.NONE, ref force, "Overwrite existing files" };
	options[1] = { "colour", 'c', 0, OptionArg.STRING, ref colour, "Icon colour", "COLOUR" };
	options[2] = { "output", 'o', 0, OptionArg.FILENAME, ref output_path, "Output directory", "PATH" };
	options[3] = { "no-subscript", 's', 0, OptionArg.FILENAME, ref no_subscript_path, "Icon template", "PATH" };
	options[4] = { "with-subscript", 'S', 0, OptionArg.FILENAME, ref with_subscript_path, "Subscript icon template", "PATH" };
	options[5] = { null };

	try {
		var context = new OptionContext ("- generate keyboard layout icons");
		context.add_main_entries (options, null);
		context.set_help_enabled (true);
		context.parse (ref args);
	} catch (OptionError error) {
		stderr.printf ("error: %s\n", error.message);
		return 1;
	}

	if (no_subscript_path == null && with_subscript_path == null) {
		stderr.printf ("error: No icon template\n");
		return 2;
	} else if (no_subscript_path == null) {
		no_subscript_path = with_subscript_path;
	} else if (with_subscript_path == null) {
		with_subscript_path = no_subscript_path;
	}

	if (output_path != null) {
		var file = File.new_for_path (output_path);

		if (!file.query_exists (null)) {
			try {
				file.make_directory_with_parents (null);
			} catch (Error error) {
				stderr.printf ("error: %s\n", error.message);
				return 3;
			}
		}
	} else {
		output_path = ".";
	}

	Gtk.init (ref args);

	var info = new Gnome.XkbInfo ();
	var layouts = info.get_all_layouts ();
	var occurrences = new Gee.HashMap <string, int> ();

	layouts.foreach ((name) => {
		string display;
		string layout;

		info.get_layout_info (name, out display, null, out layout, null);

		if (display == null) {
			display = get_display_name (layout);
		}

		var abbreviation = get_abbreviation (display);

		if (!occurrences.has_key (abbreviation)) {
			occurrences[abbreviation] = 1;
		} else {
			occurrences[abbreviation] = occurrences[abbreviation] + 1;
		}
	});

	string no_subscript_data;
	string with_subscript_data;

	try {
		uint8[] contents;

		File.new_for_path (no_subscript_path).load_contents (null, out contents, null);
		no_subscript_data = ((string) contents).replace ("@COLOUR@", colour);

		File.new_for_path (with_subscript_path).load_contents (null, out contents, null);
		with_subscript_data = ((string) contents).replace ("@COLOUR@", colour);
	} catch (Error error) {
		stderr.printf ("error: %s\n", error.message);
		return 4;
	}

	foreach (var entry in occurrences.entries) {
		var layout = entry.key;
		var count = entry.value;
		var file = File.new_for_path (@"$output_path/$layout.svg");

		if (force || !file.query_exists (null)) {
			var output_data = no_subscript_data;

			output_data = output_data.replace ("@LAYOUT@", layout);
			output_data = output_data.replace ("@LAYOUT_X@", "3.5");
			output_data = output_data.replace ("@LAYOUT_Y@", "15.5");

			try {
				file.replace_contents (output_data.data, null, false, FileCreateFlags.REPLACE_DESTINATION, null, null);
			} catch (Error error) {
				stderr.printf ("error: %s\n", error.message);
			}
		}

		if (count > 1) {
			var partial_data = with_subscript_data;

			partial_data = partial_data.replace ("@LAYOUT@", layout);

			for (var i = 1; i <= count; i++) {
				file = File.new_for_path (@"$output_path/$layout-$i.svg");

				if (force || !file.query_exists (null)) {
					var output_data = partial_data;

					output_data = output_data.replace ("@LAYOUT_X@", "3.5");
					output_data = output_data.replace ("@LAYOUT_Y@", "15.5");
					output_data = output_data.replace ("@SUBSCRIPT@", @"$i");
					output_data = output_data.replace ("@SUBSCRIPT_X@", "15");
					output_data = output_data.replace ("@SUBSCRIPT_Y@", "10");

					try {
						file.replace_contents (output_data.data, null, false, FileCreateFlags.REPLACE_DESTINATION, null, null);
					} catch (Error error) {
						stderr.printf ("error: %s\n", error.message);
					}
				}
			}
		}
	}

	return 0;
}
