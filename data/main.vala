int main (string[] args) {
	var force = false;
	var width = 220.0;
	var height = 220.0;
	var radius = 40.0;
	var colour = "grey";
	var font = "Ubuntu";
	var weight = 500;
	var layout_size = 120;
	var subscript_size = 80;
	string output_path = null;
	string no_subscript_path = null;
	string with_subscript_path = null;

	OptionEntry[] options = new OptionEntry[13];
	options[0] = { "force", 'f', 0, OptionArg.NONE, ref force, "Overwrite existing files" };
	options[1] = { "width", 'w', 0, OptionArg.DOUBLE, ref width, "Icon width", "DOUBLE" };
	options[2] = { "height", 'h', 0, OptionArg.DOUBLE, ref height, "Icon height", "DOUBLE" };
	options[3] = { "radius", 'r', 0, OptionArg.DOUBLE, ref radius, "Icon radius", "DOUBLE" };
	options[4] = { "colour", 'c', 0, OptionArg.STRING, ref colour, "Icon colour", "COLOUR" };
	options[5] = { "font", 'f', 0, OptionArg.STRING, ref font, "Font family", "NAME" };
	options[6] = { "weight", 'W', 0, OptionArg.INT, ref weight, "Font weight (100 to 1000)", "INT" };
	options[7] = { "layout-size", 's', 0, OptionArg.INT, ref layout_size, "Layout font size", "INT" };
	options[8] = { "subscript-size", 'S', 0, OptionArg.INT, ref subscript_size, "Subscript font size", "INT" };
	options[9] = { "output", 'o', 0, OptionArg.FILENAME, ref output_path, "Output directory", "PATH" };
	options[10] = { "no-subscript", 'i', 0, OptionArg.FILENAME, ref no_subscript_path, "Icon template", "PATH" };
	options[11] = { "with-subscript", 'I', 0, OptionArg.FILENAME, ref with_subscript_path, "Subscript icon template", "PATH" };
	options[12] = { null };

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

		var layout_font = @"font-family:$font;font-weight:$weight;font-size:$layout_size";
		var subscript_font = @"font-family:$font;font-weight:$weight;font-size:$subscript_size";

		File.new_for_path (no_subscript_path).load_contents (null, out contents, null);
		no_subscript_data = (string) contents;
		no_subscript_data = no_subscript_data.replace ("@WIDTH@", @"$width");
		no_subscript_data = no_subscript_data.replace ("@HEIGHT@", @"$height");
		no_subscript_data = no_subscript_data.replace ("@RADIUS@", @"$radius");
		no_subscript_data = no_subscript_data.replace ("@COLOUR@", colour);
		no_subscript_data = no_subscript_data.replace ("@LAYOUT_FONT@", layout_font);
		no_subscript_data = no_subscript_data.replace ("@SUBSCRIPT_FONT@", subscript_font);

		File.new_for_path (with_subscript_path).load_contents (null, out contents, null);
		with_subscript_data = (string) contents;
		with_subscript_data = with_subscript_data.replace ("@WIDTH@", @"$width");
		with_subscript_data = with_subscript_data.replace ("@HEIGHT@", @"$height");
		with_subscript_data = with_subscript_data.replace ("@RADIUS@", @"$radius");
		with_subscript_data = with_subscript_data.replace ("@COLOUR@", colour);
		with_subscript_data = with_subscript_data.replace ("@LAYOUT_FONT@", layout_font);
		with_subscript_data = with_subscript_data.replace ("@SUBSCRIPT_FONT@", subscript_font);
	} catch (Error error) {
		stderr.printf ("error: %s\n", error.message);
		return 4;
	}

	var font_map = new PangoFT2.FontMap ();
	var layout_layout = new Pango.Layout (font_map.create_context ());
	var subscript_layout = new Pango.Layout (font_map.create_context ());

	var font_description = new Pango.FontDescription ();
	font_description.set_family (font);
	font_description.set_weight ((Pango.Weight) weight);
	font_description.set_size (layout_size * Pango.SCALE);
	layout_layout.set_font_description (font_description);

	font_description = new Pango.FontDescription ();
	font_description.set_family (font);
	font_description.set_weight ((Pango.Weight) weight);
	font_description.set_size (subscript_size * Pango.SCALE);
	subscript_layout.set_font_description (font_description);

	foreach (var entry in occurrences.entries) {
		var layout = entry.key;
		var count = entry.value;
		var file = File.new_for_path (@"$output_path/$layout.svg");

		if (force || !file.query_exists (null)) {
			int layout_width;
			int layout_height;

			layout_layout.set_text (layout, -1);
			layout_layout.get_size (out layout_width, out layout_height);
			var layout_baseline = layout_layout.get_baseline ();

			var layout_x = 0.5 * (width - 1.0 * layout_width / Pango.SCALE);
			var layout_y = 0.5 * (height - 1.0 * layout_height / Pango.SCALE) + 1.0 * layout_baseline / Pango.SCALE;

			var output_data = no_subscript_data;
			output_data = output_data.replace ("@LAYOUT@", layout);
			output_data = output_data.replace ("@LAYOUT_X@", @"$layout_x");
			output_data = output_data.replace ("@LAYOUT_Y@", @"$layout_y");
			output_data = output_data.replace ("@SUBSCRIPT@", "");
			output_data = output_data.replace ("@SUBSCRIPT_X@", "0");
			output_data = output_data.replace ("@SUBSCRIPT_Y@", "0");

			try {
				file.replace_contents (output_data.data, null, false, FileCreateFlags.REPLACE_DESTINATION, null, null);
			} catch (Error error) {
				stderr.printf ("error: %s\n", error.message);
			}
		}

		if (count > 1) {
			int layout_width;
			int layout_height;

			layout_layout.set_text (layout, -1);
			layout_layout.get_size (out layout_width, out layout_height);
			var layout_baseline = layout_layout.get_baseline ();

			var layout_y = 0.5 * (height - 1.0 * layout_height / Pango.SCALE) + 1.0 * layout_baseline / Pango.SCALE;

			var partial_data = with_subscript_data;
			partial_data = partial_data.replace ("@LAYOUT@", layout);
			partial_data = partial_data.replace ("@LAYOUT_Y@", @"$layout_y");

			for (var i = 1; i <= count; i++) {
				file = File.new_for_path (@"$output_path/$layout-$i.svg");

				if (force || !file.query_exists (null)) {
					var subscript = @"$i";
					int subscript_width;
					int subscript_height;

					subscript_layout.set_text (subscript, -1);
					subscript_layout.get_size (out subscript_width, out subscript_height);
					var subscript_baseline = subscript_layout.get_baseline ();

					var layout_x = 0.5 * (width - 1.0 * (layout_width + subscript_width) / Pango.SCALE);
					var subscript_x = layout_x + 1.0 * layout_width / Pango.SCALE;
					var subscript_y = layout_y - 0.5 * subscript_height / Pango.SCALE + 1.0 * subscript_baseline / Pango.SCALE;

					var output_data = partial_data;
					output_data = output_data.replace ("@LAYOUT_X@", @"$layout_x");
					output_data = output_data.replace ("@LAYOUT_Y@", @"$layout_y");
					output_data = output_data.replace ("@SUBSCRIPT@", subscript);
					output_data = output_data.replace ("@SUBSCRIPT_X@", @"$subscript_x");
					output_data = output_data.replace ("@SUBSCRIPT_Y@", @"$subscript_y");

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
