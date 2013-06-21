string get_abbreviation (string name) {
	var index = 0;
	unichar first;
	unichar second;

	if (name.get_next_char (ref index, out first)) {
		if (name.get_next_char (ref index, out second)) {
			return @"$(first.toupper ())$second";
		} else {
			return @"$(first.toupper ())";
		}
	} else {
		return "";
	}
}
