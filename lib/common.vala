string get_abbreviation (string name) {
	var index = 0;
	unichar first;
	unichar second;

	if (name.get_next_char (ref index, out first)) {
		if (name.get_next_char (ref index, out second)) {
			return @"$((!) first.toupper ().to_string ())$((!) second.to_string ())";
		} else {
			return (!) first.toupper ().to_string ();
		}
	} else {
		return "";
	}
}
