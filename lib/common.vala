string get_abbreviation (string name) {
	string abbreviation = null;

	if (name != null) {
		char letters[2];
		var index = 0;

		for (var i = 0; i < name.length && index < 2; i++) {
			if (name[i].isupper ()) {
				letters[index++] = name[i];
			}
		}

		if (index < 2) {
			index = 0;

			for (var i = 0; i < name.length && index < 2; i++) {
				if (name[i].isalpha () && (i == 0 || !name[i - 1].isalpha ())) {
					letters[index++] = name[i++].toupper ();
				}
			}

			if (index < 2) {
				index = 0;

				for (var i = 0; i < name.length && index < 2; i++) {
					if (name[i].isalpha ()) {
						letters[index++] = name[i];
					}
				}
			}
		}

		if (index == 2) {
			abbreviation = @"$(letters[0])$(letters[1])";
		} else if (index == 1) {
			abbreviation = @"$(letters[0])";
		}
	}

	return abbreviation;
}

string? get_display_name (string? layout) {
	if (layout == null) {
		return null;
	}

	var language = Xkl.get_language_name (layout);
	var country = Xkl.get_country_name (layout);

	if (language != null && country != null) {
		return @"$language ($country)";
	} else if (language != null) {
		return language;
	} else if (country != null) {
		return country;
	} else {
		return null;
	}
}
