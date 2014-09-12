/*
 * Copyright 2013 Canonical Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors: William Hua <william.hua@canonical.com>
 */

public class Indicator.Keyboard.Source : Object {

	private static Gnome.XkbInfo? xkb_info;
	private static IBus.Bus? ibus_bus;
	private static Fcitx.InputMethod? fcitx_proxy;

	private string? xkb;
	private string? ibus;
	private string? fcitx;

	private string? _name;
	private string? _short_name;
	private string? _layout;
	private string? _variant;
	private Icon? _icon;
	private uint _subscript;
	private bool _show_subscript;
	private bool _use_gtk;

	public string? name {
		get { if (_name == null) { _name = _get_name (); } return _name; }
	}

	public string? short_name {
		get { if (_short_name == null) { _short_name = _get_short_name (); } return _short_name; }
	}

	public string? layout {
		get { if (_layout == null) { _layout = _get_layout (); } return _layout; }
	}

	public string? variant {
		get { if (_variant == null) { _variant = _get_variant (); } return _variant; }
	}

	public Icon? icon {
		get { if (_icon == null) { _icon = _get_icon (); } return _icon; }
		set { _icon = value; }
	}

	public uint subscript {
		get { return _subscript; }
		set { _subscript = value; icon = null; }
	}

	public bool show_subscript {
		get { return _show_subscript; }
		set { _show_subscript = value; icon = null; }
	}

	public bool use_gtk {
		get { return _use_gtk; }
		construct set { _use_gtk = value; icon = null; }
	}

	public bool is_xkb {
		get { return xkb != null; }
	}

	public bool is_ibus {
		get { return ibus != null; }
	}

	public bool is_fcitx {
		get { return fcitx != null; }
	}

	public Source (Variant variant, bool use_gtk = false) {
		Object (use_gtk: use_gtk);

		if (variant.is_of_type (new VariantType ("(ss)"))) {
			unowned string type;
			unowned string name;

			variant.get ("(&s&s)", out type, out name);

			if (type == "xkb") {
				xkb = name;
			} else if (type == "ibus") {
				ibus = name;
			} else if (type == "fcitx") {
				fcitx = name;
			}
		} else if (variant.is_of_type (new VariantType ("a{ss}"))) {
			var iter = variant.iterator ();

			unowned string key;
			unowned string value;

			while (iter.next ("{&s&s}", out key, out value)) {
				if (key == "xkb") {
					xkb = value;
				} else if (key == "ibus") {
					ibus = value;
				} else if (key == "fcitx") {
					fcitx = value;
				}
			}
		}
	}

	private static Gnome.XkbInfo get_xkb_info () {
		if (xkb_info == null) {
			xkb_info = new Gnome.XkbInfo ();
		}

		return (!) xkb_info;
	}

	private static IBus.Bus get_ibus_bus () {
		if (ibus_bus == null) {
			IBus.init ();
			ibus_bus = new IBus.Bus ();
		}

		return (!) ibus_bus;
	}

	private static Fcitx.InputMethod get_fcitx_proxy () throws Error {
		if (fcitx_proxy == null) {
			fcitx_proxy = new Fcitx.InputMethod (BusType.SESSION, DBusProxyFlags.NONE, 0);
		}

		return (!) fcitx_proxy;
	}

	private IBus.EngineDesc? get_engine () {
		IBus.EngineDesc? engine = null;

		if (ibus != null) {
			var names = new string[2];
			names[0] = (!) ibus;

			var engines = get_ibus_bus ().get_engines_by_names (names);

			if (engines.length > 0) {
				engine = engines[0];
			}
		}

		return engine;
	}

	protected virtual string? _get_name () {
		string? name = null;

		if (xkb != null) {
			string? display_name = null;
			string? layout = null;

			get_xkb_info ().get_layout_info ((!) xkb, out display_name, null, out layout, null);

			var has_display_name = display_name != null && ((!) display_name).get_char () != '\0';
			var has_layout = layout != null && ((!) layout).get_char () != '\0';

			if (has_display_name) {
				name = display_name;
			} else if (has_layout) {
				string? language = Xkl.get_language_name ((!) layout);
				string? country = Xkl.get_country_name ((!) layout);
				var has_language = language != null && ((!) language).get_char () != '\0';
				var has_country = country != null && ((!) country).get_char () != '\0';

				if (has_language && has_country) {
					name = @"$((!) language) ($((!) country))";
				} else if (has_language) {
					name = language;
				} else if (has_country) {
					name = country;
				}
			}

			if (name == null || ((!) name).get_char () == '\0') {
				name = xkb;
			}
		} else if (ibus != null) {
			var engine = get_engine ();

			if (engine != null) {
				string? language = ((!) engine).get_language ();
				string? display_name = ((!) engine).get_longname ();
				var has_language = language != null && ((!) language).get_char () != '\0';
				var has_display_name = display_name != null && ((!) display_name).get_char () != '\0';

				if (has_language) {
					language = Xkl.get_language_name ((!) language);
					has_language = language != null && ((!) language).get_char () != '\0';
				}

				if (has_language && has_display_name) {
					name = @"$((!) language) ($((!) display_name))";
				} else if (has_language) {
					name = language;
				} else if (has_display_name) {
					name = display_name;
				}
			}

			if (name == null || ((!) name).get_char () == '\0') {
				name = ibus;
			}
		} else if (fcitx != null) {
			try {
				var input_methods = get_fcitx_proxy ().get_imlist_nofree ();

				for (var i = 0; i < input_methods.length; i++) {
					if (input_methods.get (i).unique_name == (!) fcitx) {
						name = input_methods.get (i).name;
						break;
					}
				}
			} catch (Error error) {
				warning ("error: %s", error.message);
			}

			if (name == null || ((!) name).get_char () == '\0') {
				name = fcitx;
			}
		}

		return name;
	}

	protected virtual string? _get_short_name () {
		string? short_name = null;

		if (xkb != null) {
			get_xkb_info ().get_layout_info ((!) xkb, null, out short_name, null, null);

			if (short_name == null || ((!) short_name).get_char () == '\0') {
				short_name = xkb;
			}
		} else if (ibus != null) {
			var engine = get_engine ();

			if (engine != null) {
				short_name = ((!) engine).get_name ();
			}

			if (short_name == null || ((!) short_name).get_char () == '\0') {
				short_name = ibus;
			}
		} else if (fcitx != null) {
			try {
				var input_methods = get_fcitx_proxy ().get_imlist_nofree ();

				for (var i = 0; i < input_methods.length; i++) {
					if (input_methods.get (i).unique_name == (!) fcitx) {
						short_name = input_methods.get (i).langcode;
						break;
					}
				}
			} catch (Error error) {
				warning ("error: %s", error.message);
			}

			if (short_name == null || ((!) short_name).get_char () == '\0') {
				short_name = fcitx;
			}
		}

		return abbreviate (short_name);
	}

	protected virtual string? _get_layout () {
		string? layout = null;

		if (xkb != null) {
			get_xkb_info ().get_layout_info ((!) xkb, null, null, out layout, null);
		}

		var has_layout = layout != null && ((!) layout).get_char () != '\0';

		if (!has_layout) {
			var engine = get_engine ();

			if (engine != null) {
				layout = ((!) engine).get_layout ();
			}
		}

		if (layout == null || ((!) layout).get_char () == '\0') {
			layout = xkb;
		}

		return layout;
	}

	protected virtual string? _get_variant () {
		string? variant = null;

		if (xkb != null) {
			get_xkb_info ().get_layout_info ((!) xkb, null, null, null, out variant);
		}

		var has_variant = variant != null && ((!) variant).get_char () != '\0';

		if (!has_variant) {
			var engine = get_engine ();

			if (engine != null) {
				variant = ((!) engine).get_layout_variant ();
			}
		}

		if (variant == null || ((!) variant).get_char () == '\0') {
			variant = null;
		}

		return variant;
	}

	private Gtk.StyleContext? get_style_context () {
		Gtk.StyleContext? context = null;

		if (_use_gtk) {
			Gdk.Screen? screen = Gdk.Screen.get_default ();

			if (screen != null) {
				context = new Gtk.StyleContext ();
				((!) context).set_screen ((!) screen);

				var path = new Gtk.WidgetPath ();
				path.append_type (typeof (Gtk.MenuItem));
				((!) context).set_path (path);
			}
		}

		return context;
	}

	protected virtual Icon? create_icon () {
		Icon? icon = null;

		var style = get_style_context ();

		if (style != null) {
			const int W = 22;
			const int H = 22;
			const int w = 20;
			const int h = 20;
			const double R = 2.0;
			const double TEXT_SIZE = 12.0;
			const double SUBSCRIPT_SIZE = 8.0;

			Pango.FontDescription description;
			var colour = ((!) style).get_color (Gtk.StateFlags.NORMAL);
			colour = { 0.5, 0.5, 0.5, 1.0 };
			((!) style).get (Gtk.StateFlags.NORMAL, Gtk.STYLE_PROPERTY_FONT, out description);

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

			if (short_name != null) {
				var text_layout = Pango.cairo_create_layout (context);
				text_layout.set_alignment (Pango.Alignment.CENTER);
				description.set_absolute_size (Pango.units_from_double (TEXT_SIZE));
				text_layout.set_font_description (description);
				text_layout.set_text ((!) short_name, -1);
				Pango.cairo_update_layout (context, text_layout);
				int text_width;
				int text_height;
				text_layout.get_pixel_size (out text_width, out text_height);

				if (_show_subscript) {
					var subscript_layout = Pango.cairo_create_layout (context);
					subscript_layout.set_alignment (Pango.Alignment.CENTER);
					description.set_absolute_size (Pango.units_from_double (SUBSCRIPT_SIZE));
					subscript_layout.set_font_description (description);
					subscript_layout.set_text (@"$_subscript", -1);
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

			icon = new BytesIcon (ByteArray.free_to_bytes ((owned) buffer));
		}

		return icon;
	}

	private Icon? _get_icon () {
		Icon? icon = null;

		var engine = get_engine ();

		if (engine != null) {
			string? icon_name = ((!) engine).get_icon ();
			var has_icon_name = icon_name != null && ((!) icon_name).get_char () != '\0';

			if (has_icon_name) {
				try {
					icon = Icon.new_for_string ((!) icon_name);
				} catch (Error error) {
					warning ("error: %s", error.message);
				}
			}
		}

		if (icon == null && short_name != null) {
			string icon_name;

			if (_show_subscript) {
				icon_name = @"indicator-keyboard-$((!) short_name)-$_subscript";
			} else {
				icon_name = @"indicator-keyboard-$((!) short_name)";
			}

			if (_use_gtk) {
				var icon_theme = Gtk.IconTheme.get_default ();
				Gtk.IconInfo? icon_info = icon_theme.lookup_icon (icon_name, 22, 0);

				if (icon_info != null) {
					icon = new ThemedIcon (icon_name);
				}
			} else {
				icon = new ThemedIcon (icon_name);
			}
		}

		if (icon == null) {
			icon = create_icon ();
		}

		return icon;
	}
}
