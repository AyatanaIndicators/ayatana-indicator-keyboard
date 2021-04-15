/*
 * Copyright 2021 Robert Tari <robert@tari.in>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3, as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranties of
 * MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

gchar *LANGUAGES[] =
{
    "Ap", //APL
    "Ap", //APL symbols (APLX unified)
    "Ap", //APL symbols (Dyalog APL)
    "Ap", //APL symbols (IBM APL2)
    "Ap", //APL symbols (Manugistics APL*PLUS II)
    "Ap", //APL symbols (SAX, Sharp APL for Unix)
    "Ap", //APL symbols (unified)
    "Fa", //Afghani
    "Ak", //Akan
    "Sq", //Albanian
    "Sq", //Albanian (Plisi)
    "Sq", //Albanian (Veqilharxhi)
    "Am", //Amharic
    "Ar", //Arabic
    "Ar", //Arabic (AZERTY)
    "Ar", //Arabic (AZERTY, Eastern Arabic numerals)
    "Ar", //Arabic (Algeria)
    "Ar", //Arabic (Arabic numerals, extensions in the 4th level)
    "Ar", //Arabic (Buckwalter)
    "Ar", //Arabic (Eastern Arabic numerals)
    "Ar", //Arabic (Eastern Arabic numerals, extensions in the 4th level)
    "Ar", //Arabic (Macintosh)
    "Ar", //Arabic (Morocco)
    "Ar", //Arabic (OLPC)
    "Ar", //Arabic (Pakistan)
    "Ar", //Arabic (QWERTY)
    "Ar", //Arabic (QWERTY, Eastern Arabic numerals)
    "Ar", //Arabic (Sun Type 6/7)
    "Ar", //Arabic (Syria)
    "Hy", //Armenian
    "Hy", //Armenian (OLPC, phonetic)
    "Hy", //Armenian (alt. eastern)
    "Hy", //Armenian (alt. phonetic)
    "Hy", //Armenian (eastern)
    "Hy", //Armenian (phonetic)
    "Hy", //Armenian (western)
    "As", //Asturian (Spain, with bottom-dot H and L)
    "At", //Atsina
    "Av", //Avatime
    "Ae", //Avestan
    "Az", //Azerbaijani
    "Az", //Azerbaijani (Cyrillic)
    "Bm", //Bambara
    "Bn", //Bangla
    "Bn", //Bangla (India)
    "Bn", //Bangla (India, Baishakhi Inscript)
    "Bn", //Bangla (India, Baishakhi)
    "Bn", //Bangla (India, Bornona)
    "Bn", //Bangla (India, Gitanjali)
    "Bn", //Bangla (India, Probhat)
    "Bn", //Bangla (Probhat)
    "Ba", //Bashkirian
    "Be", //Belarusian
    "Be", //Belarusian (Latin)
    "Be", //Belarusian (intl.)
    "Be", //Belarusian (legacy)
    "Bl", //Belgian
    "Bl", //Belgian (ISO, alt.)
    "Bl", //Belgian (Latin-9 only, alt.)
    "Bl", //Belgian (Sun Type 6/7)
    "Bl", //Belgian (Wang 724 AZERTY)
    "Bl", //Belgian (alt.)
    "Bl", //Belgian (no dead keys)
    "Bb", //Berber (Algeria, Latin)
    "Bb", //Berber (Algeria, Tifinagh)
    "Bb", //Berber (Morocco, Tifinagh alt.)
    "Bb", //Berber (Morocco, Tifinagh extended phonetic)
    "Bb", //Berber (Morocco, Tifinagh extended)
    "Bb", //Berber (Morocco, Tifinagh phonetic)
    "Bb", //Berber (Morocco, Tifinagh phonetic, alt.)
    "Bb", //Berber (Morocco, Tifinagh)
    "Bs", //Bosnian
    "Bs", //Bosnian (US)
    "Bs", //Bosnian (US, with Bosnian digraphs)
    "Bs", //Bosnian (with Bosnian digraphs)
    "Bs", //Bosnian (with guillemets)
    "Bi", //Braille
    "Bi", //Braille (left-handed inverted thumb)
    "Bi", //Braille (left-handed)
    "Bi", //Braille (right-handed inverted thumb)
    "Bi", //Braille (right-handed)
    "Bg", //Bulgarian
    "Bg", //Bulgarian (enhanced)
    "Bg", //Bulgarian (new phonetic)
    "Bg", //Bulgarian (traditional phonetic)
    "My", //Burmese
    "Zg", //Burmese Zawgyi
    "Cm", //Cameroon (AZERTY, intl.)
    "Cm", //Cameroon (Dvorak, intl.)
    "Cm", //Cameroon Multilingual (QWERTY, intl.)
    "Cn", //Canadian (intl.)
    "Cn", //Canadian (intl., 1st part)
    "Cn", //Canadian (intl., 2nd part)
    "Ca", //Catalan (Spain, with middle-dot L)
    "Ck", //Cherokee
    "Zh", //Chinese
    "Cu", //Church Slavonic
    "Cv", //Chuvash
    "Cv", //Chuvash (Latin)
    "Cl", //CloGaelach
    "Sx", //Coeur d'Alene Salish
    "Cp", //Coptic
    "Tt", //Crimean Tatar (Dobruja Q)
    "Tt", //Crimean Tatar (Turkish Alt-Q)
    "Tt", //Crimean Tatar (Turkish F)
    "Tt", //Crimean Tatar (Turkish Q)
    "Hr", //Croatian
    "Hr", //Croatian (US)
    "Hr", //Croatian (US, with Croatian digraphs)
    "Hr", //Croatian (with Croatian digraphs)
    "Hr", //Croatian (with guillemets)
    "Cs", //Czech
    "Cs", //Czech (QWERTY)
    "Cs", //Czech (QWERTY, Macintosh)
    "Cs", //Czech (QWERTY, extended backslash)
    "Cs", //Czech (Sun Type 6/7)
    "Cs", //Czech (UCW, only accented letters)
    "Cs", //Czech (US, Dvorak, UCW support)
    "Cs", //Czech (coder)
    "Cs", //Czech (programming)
    "Cs", //Czech (programming, typographic)
    "Cs", //Czech (typographic)
    "Cs", //Czech (with <\\|> key)
    "Cs", //Czech Slovak and German (US)
    "Da", //Danish
    "Da", //Danish (Dvorak)
    "Da", //Danish (Macintosh)
    "Da", //Danish (Macintosh, no dead keys)
    "Da", //Danish (Sun Type 6/7)
    "Da", //Danish (Windows)
    "Da", //Danish (no dead keys)
    "Dv", //Dhivehi
    "Nl", //Dutch
    "Nl", //Dutch (Macintosh)
    "Nl", //Dutch (Sun Type 6/7)
    "Nl", //Dutch (standard)
    "Dz", //Dzongkha
    "Ed", //Elfdalian (Swedish, with combining ogonek)
    "En", //English (3l)
    "En", //English (3l, Chromebook)
    "En", //English (3l, emacs)
    "En", //English (Australian)
    "En", //English (Cameroon)
    "En", //English (Canada)
    "En", //English (Carpalx)
    "En", //English (Carpalx, full optimization)
    "En", //English (Carpalx, full optimization, Hyena Layer5)
    "En", //English (Carpalx, full optimization, intl., with AltGr dead keys)
    "En", //English (Carpalx, full optimization, intl., with AltGr dead keys, Hyena Layer5)
    "En", //English (Carpalx, full optimization, intl., with dead keys)
    "En", //English (Carpalx, full optimization, intl., with dead keys, Hyena Layer5)
    "En", //English (Carpalx, intl., with AltGr dead keys)
    "En", //English (Carpalx, intl., with dead keys)
    "En", //English (Colemak)
    "En", //English (Colemak-DH)
    "En", //English (Drix)
    "En", //English (Dvorak)
    "En", //English (Dvorak, alt. intl.)
    "En", //English (Dvorak, intl., with dead keys)
    "En", //English (Dvorak, left-handed)
    "En", //English (Dvorak, right-handed)
    "En", //English (Ghana)
    "En", //English (Ghana, GILLBT)
    "En", //English (Ghana, multilingual)
    "En", //English (India, with rupee)
    "En", //English (Macintosh)
    "En", //English (Mali, US, Macintosh)
    "En", //English (Mali, US, intl.)
    "En", //English (Nigeria)
    "En", //English (Norman)
    "En", //English (South Africa)
    "En", //English (UK)
    "En", //English (UK, Colemak)
    "En", //English (UK, Colemak-DH)
    "En", //English (UK, Dvorak)
    "En", //English (UK, Dvorak, with UK punctuation)
    "En", //English (UK, Hyena Layer5)
    "En", //English (UK, Macintosh)
    "En", //English (UK, Macintosh, intl.)
    "En", //English (UK, MiniGuru Layer5)
    "En", //English (UK, Sun Type 6/7)
    "En", //English (UK, TEX Yoda Layer5)
    "En", //English (UK, extended, Windows)
    "En", //English (UK, intl., with dead keys)
    "En", //English (UK, intl., with dead keys, Hyena Layer5)
    "En", //English (UK, intl., with dead keys, MiniGuru Layer5)
    "En", //English (UK, intl., with dead keys, TEX Yoda Layer5)
    "En", //English (US)
    "En", //English (US, Hyena Layer5)
    "En", //English (US, IBM Arabic 238_L)
    "En", //English (US, MiniGuru Layer5)
    "En", //English (US, Sun Type 6/7)
    "En", //English (US, Symbolic)
    "En", //English (US, TEX Yoda Layer5)
    "En", //English (US, alt. intl.)
    "En", //English (US, alt. intl., with dead keys, Hyena Layer5)
    "En", //English (US, alt. intl., with dead keys, MiniGuru Layer5)
    "En", //English (US, alt. intl., with dead keys, TEX Yoda Layer5)
    "En", //English (US, euro on 5)
    "En", //English (US, intl., AltGr Unicode combining)
    "En", //English (US, intl., AltGr Unicode combining, Hyena Layer5)
    "En", //English (US, intl., AltGr Unicode combining, MiniGuru Layer5)
    "En", //English (US, intl., AltGr Unicode combining, TEX Yoda Layer5)
    "En", //English (US, intl., AltGr Unicode combining, alt.)
    "En", //English (US, intl., with dead keys)
    "En", //English (Workman)
    "En", //English (Workman, intl., with dead keys)
    "En", //English (classic Dvorak)
    "En", //English (intl., with AltGr dead keys)
    "En", //English (programmer Dvorak)
    "En", //English (the divide/multiply toggle the layout)
    "Eo", //Esperanto
    "Eo", //Esperanto (Brazil, Nativo)
    "Eo", //Esperanto (Portugal, Nativo)
    "Eo", //Esperanto (legacy)
    "Et", //Estonian
    "Et", //Estonian (Dvorak)
    "Et", //Estonian (Sun Type 6/7)
    "Et", //Estonian (US)
    "Et", //Estonian (no dead keys)
    "Ek", //EurKEY (US)
    "Ee", //Ewe
    "Fo", //Faroese
    "Fo", //Faroese (no dead keys)
    "Ph", //Filipino
    "Ph", //Filipino (Capewell-Dvorak, Baybayin)
    "Ph", //Filipino (Capewell-Dvorak, Latin)
    "Ph", //Filipino (Capewell-QWERF 2006, Baybayin)
    "Ph", //Filipino (Capewell-QWERF 2006, Latin)
    "Ph", //Filipino (Colemak, Baybayin)
    "Ph", //Filipino (Colemak, Latin)
    "Ph", //Filipino (Dvorak, Baybayin)
    "Ph", //Filipino (Dvorak, Latin)
    "Ph", //Filipino (QWERTY, Baybayin)
    "Fi", //Finnish
    "Fi", //Finnish (DAS)
    "Fi", //Finnish (Dvorak)
    "Fi", //Finnish (Macintosh)
    "Fi", //Finnish (Sun Type 6/7)
    "Fi", //Finnish (Windows)
    "Fi", //Finnish (classic)
    "Fi", //Finnish (classic, no dead keys)
    "Fr", //French
    "Fr", //French (AZERTY)
    "Fr", //French (AZERTY, AFNOR)
    "Fr", //French (BEPO)
    "Fr", //French (BEPO, AFNOR)
    "Fr", //French (BEPO, Latin-9 only)
    "Fr", //French (Breton)
    "Fr", //French (Cameroon)
    "Fr", //French (Canada)
    "Fr", //French (Canada, Dvorak)
    "Fr", //French (Canada, legacy)
    "Fr", //French (Democratic Republic of the Congo)
    "Fr", //French (Dvorak)
    "Fr", //French (Macintosh)
    "Fr", //French (Mali, alt.)
    "Fr", //French (Morocco)
    "Fr", //French (Sun Type 6/7)
    "Fr", //French (Switzerland)
    "Fr", //French (Switzerland, Macintosh)
    "Fr", //French (Switzerland, Sun Type 6/7)
    "Fr", //French (Switzerland, no dead keys)
    "Fr", //French (Togo)
    "Fr", //French (US with dead keys, alt.)
    "Fr", //French (US)
    "Fr", //French (US, AZERTY)
    "Fr", //French (alt.)
    "Fr", //French (alt., Latin-9 only)
    "Fr", //French (alt., no dead keys)
    "Fr", //French (legacy, alt.)
    "Fr", //French (legacy, alt., no dead keys)
    "Fr", //French (no dead keys)
    "Fu", //Friulian (Italy)
    "Ff", //Fula
    "Gh", //Ga
    "Ka", //Georgian
    "Ka", //Georgian (France, AZERTY Tskapo)
    "Ka", //Georgian (Italy)
    "Ka", //Georgian (MESS)
    "Ka", //Georgian (ergonomic)
    "De", //German
    "De", //German (Aus der Neo-Welt)
    "De", //German (Austria)
    "De", //German (Austria, Macintosh)
    "De", //German (Austria, no dead keys)
    "De", //German (Bone)
    "De", //German (Bone, eszett in the home row)
    "De", //German (Dvorak)
    "De", //German (E1)
    "De", //German (E2)
    "De", //German (KOY)
    "De", //German (Ladin)
    "De", //German (Macintosh)
    "De", //German (Macintosh, no dead keys)
    "De", //German (Neo 2)
    "De", //German (Neo, QWERTY)
    "De", //German (Neo, QWERTZ)
    "De", //German (QWERTY)
    "De", //German (Sun Type 6/7)
    "De", //German (Switzerland)
    "De", //German (Switzerland, Macintosh)
    "De", //German (Switzerland, Sun Type 6/7)
    "De", //German (Switzerland, legacy)
    "De", //German (Switzerland, no dead keys)
    "De", //German (T3)
    "De", //German (US)
    "De", //German (dead acute)
    "De", //German (dead grave acute)
    "De", //German (dead tilde)
    "De", //German (no dead keys)
    "De", //German (with Hungarian letters, no dead keys)
    "De", //German, Swedish and Finnish (US)
    "Gr", //Greek
    "Gr", //Greek (Colemak)
    "Gr", //Greek (Sun Type 6/7)
    "Gr", //Greek (extended)
    "Gr", //Greek (no dead keys)
    "Gr", //Greek (polytonic)
    "Gr", //Greek (simple)
    "Gu", //Gujarati
    "Hp", //Hanyu Pinyin (with AltGr dead keys)
    "Ha", //Hausa (Ghana)
    "Ha", //Hausa (Nigeria)
    "Hw", //Hawaiian
    "He", //Hebrew
    "He", //Hebrew (Biblical, SIL phonetic)
    "He", //Hebrew (Biblical, Tiro)
    "He", //Hebrew (lyx)
    "He", //Hebrew (phonetic)
    "Hi", //Hindi (Bolnagri)
    "Hi", //Hindi (KaGaPa, phonetic)
    "Hi", //Hindi (Wx)
    "Hu", //Hungarian
    "Hu", //Hungarian (QWERTY)
    "Hu", //Hungarian (QWERTY, 101-key, comma, dead keys)
    "Hu", //Hungarian (QWERTY, 101-key, comma, no dead keys)
    "Hu", //Hungarian (QWERTY, 101-key, dot, dead keys)
    "Hu", //Hungarian (QWERTY, 101-key, dot, no dead keys)
    "Hu", //Hungarian (QWERTY, 102-key, comma, dead keys)
    "Hu", //Hungarian (QWERTY, 102-key, comma, no dead keys)
    "Hu", //Hungarian (QWERTY, 102-key, dot, dead keys)
    "Hu", //Hungarian (QWERTY, 102-key, dot, no dead keys)
    "Hu", //Hungarian (QWERTZ, 101-key, comma, dead keys)
    "Hu", //Hungarian (QWERTZ, 101-key, comma, no dead keys)
    "Hu", //Hungarian (QWERTZ, 101-key, dot, dead keys)
    "Hu", //Hungarian (QWERTZ, 101-key, dot, no dead keys)
    "Hu", //Hungarian (QWERTZ, 102-key, comma, dead keys)
    "Hu", //Hungarian (QWERTZ, 102-key, comma, no dead keys)
    "Hu", //Hungarian (QWERTZ, 102-key, dot, dead keys)
    "Hu", //Hungarian (QWERTZ, 102-key, dot, no dead keys)
    "Hu", //Hungarian (no dead keys)
    "Hu", //Hungarian (standard)
    "Is", //Icelandic
    "Is", //Icelandic (Dvorak)
    "Is", //Icelandic (Macintosh)
    "Is", //Icelandic (Macintosh, legacy)
    "Ig", //Igbo
    "Il", //Indian
    "Ip", //Indic IPA
    "Id", //Indonesian (Arab Melayu, extended phonetic)
    "Id", //Indonesian (Arab Melayu, phonetic)
    "Id", //Indonesian (Javanese)
    "Ix", //International Phonetic Alphabet
    "Iu", //Inuktitut
    "Iq", //Iraqi
    "Ga", //Irish
    "Ga", //Irish (UnicodeExpert)
    "It", //Italian
    "It", //Italian (Dvorak)
    "It", //Italian (IBM 142)
    "It", //Italian (Ladin)
    "It", //Italian (Macintosh)
    "It", //Italian (Sun Type 6/7)
    "It", //Italian (US)
    "It", //Italian (Windows)
    "It", //Italian (intl., with dead keys)
    "It", //Italian (no dead keys)
    "Ja", //Japanese
    "Ja", //Japanese (Dvorak)
    "Ja", //Japanese (Kana 86)
    "Ja", //Japanese (Kana)
    "Ja", //Japanese (Macintosh)
    "Ja", //Japanese (OADG 109A)
    "Ja", //Japanese (PC-98)
    "Ja", //Japanese (Sun Type 6)
    "Ja", //Japanese (Sun Type 7, PC-compatible)
    "Ja", //Japanese (Sun Type 7, Sun-compatible)
    "Kb", //Kabyle (azerty layout, dead keys)
    "Kb", //Kabyle (qwerty-gb layout, dead keys)
    "Kb", //Kabyle (qwerty-us layout, dead keys)
    "Kx", //Kalmyk
    "Kn", //Kannada
    "Kn", //Kannada (KaGaPa, phonetic)
    "Kp", //Kashubian
    "Kk", //Kazakh
    "Kk", //Kazakh (Latin)
    "Kk", //Kazakh (extended)
    "Kk", //Kazakh (with Russian)
    "Km", //Khmer (Cambodia)
    "Ki", //Kikuyu
    "Kv", //Komi
    "Ko", //Korean
    "Ko", //Korean (101/104-key compatible)
    "Ko", //Korean (Sun Type 6/7)
    "Ku", //Kurdish (Iran, Arabic-Latin)
    "Ku", //Kurdish (Iran, F)
    "Ku", //Kurdish (Iran, Latin Alt-Q)
    "Ku", //Kurdish (Iran, Latin Q)
    "Ku", //Kurdish (Iraq, Arabic-Latin)
    "Ku", //Kurdish (Iraq, F)
    "Ku", //Kurdish (Iraq, Latin Alt-Q)
    "Ku", //Kurdish (Iraq, Latin Q)
    "Ku", //Kurdish (Syria, F)
    "Ku", //Kurdish (Syria, Latin Alt-Q)
    "Ku", //Kurdish (Syria, Latin Q)
    "Ku", //Kurdish (Turkey, F)
    "Ku", //Kurdish (Turkey, Latin Alt-Q)
    "Ku", //Kurdish (Turkey, Latin Q)
    "Kt", //Kutenai
    "Ky", //Kyrgyz
    "Ky", //Kyrgyz (phonetic)
    "Lo", //Lao
    "Lo", //Lao (STEA)
    "Lv", //Latvian
    "Lv", //Latvian (Colemak)
    "Lv", //Latvian (Colemak, with apostrophe)
    "Lv", //Latvian (Dvorak)
    "Lv", //Latvian (Dvorak, with Y)
    "Lv", //Latvian (Dvorak, with minus)
    "Lv", //Latvian (F)
    "Lv", //Latvian (Sun Type 6/7)
    "Lv", //Latvian (adapted)
    "Lv", //Latvian (apostrophe)
    "Lv", //Latvian (ergonomic, ŪGJRMV)
    "Lv", //Latvian (modern)
    "Lv", //Latvian (programmer Dvorak)
    "Lv", //Latvian (programmer Dvorak, with Y)
    "Lv", //Latvian (programmer Dvorak, with minus)
    "Lv", //Latvian (tilde)
    "Lt", //Lithuanian
    "Lt", //Lithuanian (Dvorak)
    "Lt", //Lithuanian (IBM LST 1205-92)
    "Lt", //Lithuanian (LEKP)
    "Lt", //Lithuanian (LEKPa)
    "Lt", //Lithuanian (Ratise)
    "Lt", //Lithuanian (Sun Type 6/7)
    "Lt", //Lithuanian (US)
    "Lt", //Lithuanian (standard)
    "Sb", //Lower Sorbian
    "Sb", //Lower Sorbian (QWERTZ)
    "Mk", //Macedonian
    "Mk", //Macedonian (no dead keys)
    "Ms", //Malay (Jawi, Arabic Keyboard)
    "Ms", //Malay (Jawi, phonetic)
    "Ml", //Malayalam
    "Ml", //Malayalam (Lalitha)
    "Ml", //Malayalam (enhanced Inscript, with rupee)
    "Mt", //Maltese
    "Mt", //Maltese (UK, with AltGr overrides)
    "Mt", //Maltese (US layout with AltGr overrides)
    "Mt", //Maltese (US)
    "Ma", //Manipuri (Eeyek)
    "Mi", //Maori
    "Mr", //Marathi (KaGaPa, phonetic)
    "Mr", //Marathi (enhanced Inscript)
    "Mx", //Mari
    "Mu", //Mmuock
    "Md", //Modi (KaGaPa phonetic)
    "Ro", //Moldavian
    "Ro", //Moldavian (Gagauz)
    "Mn", //Mongolian
    "Mn", //Mongolian (Bichig)
    "Mn", //Mongolian (Galik)
    "Mn", //Mongolian (Manchu Galik)
    "Mn", //Mongolian (Manchu)
    "Mn", //Mongolian (Todo Galik)
    "Mn", //Mongolian (Todo)
    "Mn", //Mongolian (Xibe)
    "Me", //Montenegrin
    "Me", //Montenegrin (Cyrillic)
    "Me", //Montenegrin (Cyrillic, ZE and ZHE swapped)
    "Me", //Montenegrin (Cyrillic, with guillemets)
    "Me", //Montenegrin (Latin, QWERTY)
    "Me", //Montenegrin (Latin, Unicode)
    "Me", //Montenegrin (Latin, Unicode, QWERTY)
    "Me", //Montenegrin (Latin, with guillemets)
    "Mb", //Multilingual (Canada, Sun Type 6/7)
    "Nk", //N'Ko (azerty)
    "Ne", //Nepali
    "Ns", //Northern Saami (Finland)
    "Ns", //Northern Saami (Norway)
    "Ns", //Northern Saami (Norway, no dead keys)
    "Ns", //Northern Saami (Sweden)
    "No", //Norwegian
    "No", //Norwegian (Colemak)
    "No", //Norwegian (Dvorak)
    "No", //Norwegian (Macintosh)
    "No", //Norwegian (Macintosh, no dead keys)
    "No", //Norwegian (Sun Type 6/7)
    "No", //Norwegian (Windows)
    "No", //Norwegian (no dead keys)
    "Oc", //Occitan
    "Og", //Ogham
    "Og", //Ogham (IS434)
    "Ol", //Ol Chiki
    "Hu", //Old Hungarian
    "Hu", //Old Hungarian (for ligatures)
    "Tr", //Old Turkic
    "Or", //Oriya
    "Os", //Ossetian (Georgia)
    "Os", //Ossetian (Windows)
    "Os", //Ossetian (legacy)
    "Rs", //Pannonian Rusyn
    "Ps", //Pashto
    "Ps", //Pashto (Afghanistan, OLPC)
    "Fa", //Persian
    "Fa", //Persian (Afghanistan, Dari OLPC)
    "Fa", //Persian (with Persian keypad)
    "Pl", //Polish
    "Pl", //Polish (British keyboard)
    "Pl", //Polish (Colemak)
    "Pl", //Polish (Colemak-DH)
    "Pl", //Polish (Dvorak)
    "Pl", //Polish (Dvorak, with Polish quotes on key 1)
    "Pl", //Polish (Dvorak, with Polish quotes on quotemark key)
    "Pl", //Polish (Germany, no dead keys)
    "Pl", //Polish (Glagolica)
    "Pl", //Polish (QWERTZ)
    "Pl", //Polish (Sun Type 6/7)
    "Pl", //Polish (intl., with dead keys)
    "Pl", //Polish (legacy)
    "Pl", //Polish (programmer Dvorak)
    "Pt", //Portuguese
    "Pt", //Portuguese (Brazil)
    "Pt", //Portuguese (Brazil, Dvorak)
    "Pt", //Portuguese (Brazil, IBM/Lenovo ThinkPad)
    "Pt", //Portuguese (Brazil, Nativo for US keyboards)
    "Pt", //Portuguese (Brazil, Nativo)
    "Pt", //Portuguese (Brazil, Sun Type 6/7)
    "Pt", //Portuguese (Brazil, no dead keys)
    "Pt", //Portuguese (Colemak)
    "Pt", //Portuguese (Macintosh)
    "Pt", //Portuguese (Macintosh, no dead keys)
    "Pt", //Portuguese (Nativo for US keyboards)
    "Pt", //Portuguese (Nativo)
    "Pt", //Portuguese (Sun Type 6/7)
    "Pt", //Portuguese (no dead keys)
    "Pa", //Punjabi (Gurmukhi Jhelum)
    "Pa", //Punjabi (Gurmukhi)
    "Ro", //Romanian
    "Ro", //Romanian (Germany)
    "Ro", //Romanian (Germany, no dead keys)
    "Ro", //Romanian (Sun Type 6/7)
    "Ro", //Romanian (Windows)
    "Ro", //Romanian (ergonomic Touchtype)
    "Ro", //Romanian (standard)
    "Ru", //Russian
    "Ru", //Russian (Belarus)
    "Ru", //Russian (Czech, phonetic)
    "Ru", //Russian (DOS)
    "Ru", //Russian (Georgia)
    "Ru", //Russian (Germany, phonetic)
    "Ru", //Russian (Germany, recommended)
    "Ru", //Russian (Germany, transliteration)
    "Ru", //Russian (Kazakhstan, with Kazakh)
    "Ru", //Russian (Macintosh)
    "Ru", //Russian (Poland, phonetic Dvorak)
    "Ru", //Russian (Polyglot and Reactionary)
    "Ru", //Russian (Rulemak, phonetic Colemak)
    "Ru", //Russian (Sun Type 6/7)
    "Ru", //Russian (Sweden, phonetic)
    "Ru", //Russian (Sweden, phonetic, no dead keys)
    "Ru", //Russian (US, phonetic)
    "Ru", //Russian (Ukraine, standard RSTU)
    "Ru", //Russian (legacy)
    "Ru", //Russian (phonetic Macintosh)
    "Ru", //Russian (phonetic)
    "Ru", //Russian (phonetic, AZERTY)
    "Ru", //Russian (phonetic, Dvorak)
    "Ru", //Russian (phonetic, French)
    "Ru", //Russian (phonetic, Windows)
    "Ru", //Russian (phonetic, YAZHERTY)
    "Ru", //Russian (typewriter)
    "Ru", //Russian (typewriter, legacy)
    "Ru", //Russian (with US punctuation)
    "Ru", //Russian (with Ukrainian-Belorussian layout)
    "Sy", //Saisiyat (Taiwan)
    "Sf", //Samogitian
    "Sa", //Sanskrit (KaGaPa, phonetic)
    "Sa", //Sanskrit symbols
    "Sp", //Secwepemctsin
    "Sr", //Serbian
    "Sr", //Serbian (Cyrillic, ZE and ZHE swapped)
    "Sr", //Serbian (Cyrillic, with guillemets)
    "Sr", //Serbian (Latin)
    "Sr", //Serbian (Latin, QWERTY)
    "Sr", //Serbian (Latin, Unicode)
    "Sr", //Serbian (Latin, Unicode, QWERTY)
    "Sr", //Serbian (Latin, with guillemets)
    "Sr", //Serbian (Russia)
    "Sr", //Serbian (combining accents instead of dead keys)
    "Hs", //Serbo-Croatian (US)
    "Sj", //Sicilian
    "Sj", //Sicilian (US keyboard)
    "Sj", //Silesian
    "Sd", //Sindhi
    "Si", //Sinhala (US)
    "Si", //Sinhala (phonetic)
    "Sk", //Slovak
    "Sk", //Slovak (ACC layout, only accented letters)
    "Sk", //Slovak (QWERTY)
    "Sk", //Slovak (QWERTY, extended backslash)
    "Sk", //Slovak (Sun Type 6/7)
    "Sk", //Slovak (extended backslash)
    "Sl", //Slovenian
    "Sl", //Slovenian (US)
    "Sl", //Slovenian (with guillemets)
    "Es", //Spanish
    "Es", //Spanish (Dvorak)
    "Es", //Spanish (Latin American)
    "Es", //Spanish (Latin American, Colemak for gaming)
    "Es", //Spanish (Latin American, Colemak)
    "Es", //Spanish (Latin American, Dvorak)
    "Es", //Spanish (Latin American, dead tilde)
    "Es", //Spanish (Latin American, no dead keys)
    "Es", //Spanish (Macintosh)
    "Es", //Spanish (Sun Type 6/7)
    "Es", //Spanish (Windows)
    "Es", //Spanish (dead tilde)
    "Es", //Spanish (no dead keys)
    "Sw", //Swahili (Kenya)
    "Sw", //Swahili (Tanzania)
    "Sv", //Swedish
    "Sv", //Swedish (Dvorak A5)
    "Sv", //Swedish (Dvorak)
    "Sv", //Swedish (Dvorak, intl.)
    "Sv", //Swedish (Macintosh)
    "Sv", //Swedish (Sun Type 6/7)
    "Sv", //Swedish (Svdvorak)
    "Sv", //Swedish (US)
    "Sv", //Swedish (no dead keys)
    "Sv", //Swedish Sign Language
    "Ls", //Syriac
    "Ls", //Syriac (phonetic)
    "Tw", //Taiwanese
    "Tw", //Taiwanese (indigenous)
    "Tg", //Tajik
    "Tg", //Tajik (legacy)
    "Ta", //Tamil (Inscript)
    "Ta", //Tamil (Sri Lanka, TamilNet '99)
    "Ta", //Tamil (Sri Lanka, TamilNet '99, TAB encoding)
    "Ta", //Tamil (TamilNet '99 with Tamil numerals)
    "Ta", //Tamil (TamilNet '99)
    "Ta", //Tamil (TamilNet '99, TAB encoding)
    "Ta", //Tamil (TamilNet '99, TSCII encoding)
    "Tt", //Tatar
    "Te", //Telugu
    "Te", //Telugu (KaGaPa, phonetic)
    "Te", //Telugu (Sarala)
    "Th", //Thai
    "Th", //Thai (Pattachote)
    "Th", //Thai (TIS-820.2538)
    "Bo", //Tibetan
    "Bo", //Tibetan (with ASCII numerals)
    "Tn", //Tswana
    "Tr", //Turkish
    "Tr", //Turkish (Alt-Q)
    "Tr", //Turkish (F)
    "Tr", //Turkish (Germany)
    "Tr", //Turkish (Sun Type 6/7)
    "Tr", //Turkish (intl., with dead keys)
    "Tk", //Turkmen
    "Tk", //Turkmen (Alt-Q)
    "Ud", //Udmurt
    "Ua", //Ugaritic instead of Arabic
    "Uk", //Ukrainian
    "Uk", //Ukrainian (Sun Type 6/7)
    "Uk", //Ukrainian (Windows)
    "Uk", //Ukrainian (homophonic)
    "Uk", //Ukrainian (legacy)
    "Uk", //Ukrainian (phonetic)
    "Uk", //Ukrainian (standard RSTU)
    "Uk", //Ukrainian (typewriter)
    "Ur", //Urdu (Pakistan)
    "Ur", //Urdu (Pakistan, CRULP)
    "Ur", //Urdu (Pakistan, NLA)
    "Ur", //Urdu (Windows)
    "Ur", //Urdu (alt. phonetic)
    "Ur", //Urdu (phonetic)
    "Ug", //Uyghur
    "Uz", //Uzbek
    "Uz", //Uzbek (Afghanistan)
    "Uz", //Uzbek (Afghanistan, OLPC)
    "Uz", //Uzbek (Latin)
    "Vi", //Vietnamese
    "Vi", //Vietnamese (AÐERTY)
    "Vi", //Vietnamese (French)
    "Vi", //Vietnamese (QĐERTY)
    "Vi", //Vietnamese (US)
    "Wo", //Wolof
    "Ya", //Yakut
    "Yo", //Yoruba
    NULL
};

gchar *IDS[] =
{
    "apl", //APL
    "apl+aplx", //APL symbols (APLX unified)
    "apl+dyalog", //APL symbols (Dyalog APL)
    "apl+apl2", //APL symbols (IBM APL2)
    "apl+aplplusII", //APL symbols (Manugistics APL*PLUS II)
    "apl+sax", //APL symbols (SAX, Sharp APL for Unix)
    "apl+unified", //APL symbols (unified)
    "af", //Afghani
    "gh+akan", //Akan
    "al", //Albanian
    "al+plisi", //Albanian (Plisi)
    "al+veqilharxhi", //Albanian (Veqilharxhi)
    "et", //Amharic
    "ara", //Arabic
    "ara+azerty", //Arabic (AZERTY)
    "ara+azerty_digits", //Arabic (AZERTY, Eastern Arabic numerals)
    "dz+ar", //Arabic (Algeria)
    "ara+basic_ext", //Arabic (Arabic numerals, extensions in the 4th level)
    "ara+buckwalter", //Arabic (Buckwalter)
    "ara+digits", //Arabic (Eastern Arabic numerals)
    "ara+basic_ext_digits", //Arabic (Eastern Arabic numerals, extensions in the 4th level)
    "ara+mac", //Arabic (Macintosh)
    "ma", //Arabic (Morocco)
    "ara+olpc", //Arabic (OLPC)
    "pk+ara", //Arabic (Pakistan)
    "ara+qwerty", //Arabic (QWERTY)
    "ara+qwerty_digits", //Arabic (QWERTY, Eastern Arabic numerals)
    "ara+sun_type6", //Arabic (Sun Type 6/7)
    "sy", //Arabic (Syria)
    "am", //Armenian
    "am+olpc-phonetic", //Armenian (OLPC, phonetic)
    "am+eastern-alt", //Armenian (alt. eastern)
    "am+phonetic-alt", //Armenian (alt. phonetic)
    "am+eastern", //Armenian (eastern)
    "am+phonetic", //Armenian (phonetic)
    "am+western", //Armenian (western)
    "es+ast", //Asturian (Spain, with bottom-dot H and L)
    "us+ats", //Atsina
    "gh+avn", //Avatime
    "ir+ave", //Avestan
    "az", //Azerbaijani
    "az+cyrillic", //Azerbaijani (Cyrillic)
    "ml", //Bambara
    "bd", //Bangla
    "in+ben", //Bangla (India)
    "in+ben_inscript", //Bangla (India, Baishakhi Inscript)
    "in+ben_baishakhi", //Bangla (India, Baishakhi)
    "in+ben_bornona", //Bangla (India, Bornona)
    "in+ben_gitanjali", //Bangla (India, Gitanjali)
    "in+ben_probhat", //Bangla (India, Probhat)
    "bd+probhat", //Bangla (Probhat)
    "ru+bak", //Bashkirian
    "by", //Belarusian
    "by+latin", //Belarusian (Latin)
    "by+intl", //Belarusian (intl.)
    "by+legacy", //Belarusian (legacy)
    "be", //Belgian
    "be+iso-alternate", //Belgian (ISO, alt.)
    "be+oss_latin9", //Belgian (Latin-9 only, alt.)
    "be+sun_type6", //Belgian (Sun Type 6/7)
    "be+wang", //Belgian (Wang 724 AZERTY)
    "be+oss", //Belgian (alt.)
    "be+nodeadkeys", //Belgian (no dead keys)
    "dz", //Berber (Algeria, Latin)
    "dz+ber", //Berber (Algeria, Tifinagh)
    "ma+tifinagh-alt", //Berber (Morocco, Tifinagh alt.)
    "ma+tifinagh-extended-phonetic", //Berber (Morocco, Tifinagh extended phonetic)
    "ma+tifinagh-extended", //Berber (Morocco, Tifinagh extended)
    "ma+tifinagh-phonetic", //Berber (Morocco, Tifinagh phonetic)
    "ma+tifinagh-alt-phonetic", //Berber (Morocco, Tifinagh phonetic, alt.)
    "ma+tifinagh", //Berber (Morocco, Tifinagh)
    "ba", //Bosnian
    "ba+us", //Bosnian (US)
    "ba+unicodeus", //Bosnian (US, with Bosnian digraphs)
    "ba+unicode", //Bosnian (with Bosnian digraphs)
    "ba+alternatequotes", //Bosnian (with guillemets)
    "brai", //Braille
    "brai+left_hand_invert", //Braille (left-handed inverted thumb)
    "brai+left_hand", //Braille (left-handed)
    "brai+right_hand_invert", //Braille (right-handed inverted thumb)
    "brai+right_hand", //Braille (right-handed)
    "bg", //Bulgarian
    "bg+bekl", //Bulgarian (enhanced)
    "bg+bas_phonetic", //Bulgarian (new phonetic)
    "bg+phonetic", //Bulgarian (traditional phonetic)
    "mm", //Burmese
    "mm+zawgyi", //Burmese Zawgyi
    "cm+azerty", //Cameroon (AZERTY, intl.)
    "cm+dvorak", //Cameroon (Dvorak, intl.)
    "cm+qwerty", //Cameroon Multilingual (QWERTY, intl.)
    "ca+multix", //Canadian (intl.)
    "ca+multi", //Canadian (intl., 1st part)
    "ca+multi-2gr", //Canadian (intl., 2nd part)
    "es+cat", //Catalan (Spain, with middle-dot L)
    "us+chr", //Cherokee
    "cn", //Chinese
    "ru+chu", //Church Slavonic
    "ru+cv", //Chuvash
    "ru+cv_latin", //Chuvash (Latin)
    "ie+CloGaelach", //CloGaelach
    "us+crd", //Coeur d'Alene Salish
    "eg", //Coptic
    "ro+crh_dobruja", //Crimean Tatar (Dobruja Q)
    "tr+crh_alt", //Crimean Tatar (Turkish Alt-Q)
    "tr+crh_f", //Crimean Tatar (Turkish F)
    "tr+crh", //Crimean Tatar (Turkish Q)
    "hr", //Croatian
    "hr+us", //Croatian (US)
    "hr+unicodeus", //Croatian (US, with Croatian digraphs)
    "hr+unicode", //Croatian (with Croatian digraphs)
    "hr+alternatequotes", //Croatian (with guillemets)
    "cz", //Czech
    "cz+qwerty", //Czech (QWERTY)
    "cz+qwerty-mac", //Czech (QWERTY, Macintosh)
    "cz+qwerty_bksl", //Czech (QWERTY, extended backslash)
    "cz+sun_type6", //Czech (Sun Type 6/7)
    "cz+ucw", //Czech (UCW, only accented letters)
    "cz+dvorak-ucw", //Czech (US, Dvorak, UCW support)
    "cz+coder", //Czech (coder)
    "cz+prog", //Czech (programming)
    "cz+prog_typo", //Czech (programming, typographic)
    "cz+typo", //Czech (typographic)
    "cz+bksl", //Czech (with <\\|> key)
    "us+cz_sk_de", //Czech Slovak and German (US)
    "dk", //Danish
    "dk+dvorak", //Danish (Dvorak)
    "dk+mac", //Danish (Macintosh)
    "dk+mac_nodeadkeys", //Danish (Macintosh, no dead keys)
    "dk+sun_type6", //Danish (Sun Type 6/7)
    "dk+winkeys", //Danish (Windows)
    "dk+nodeadkeys", //Danish (no dead keys)
    "mv", //Dhivehi
    "nl", //Dutch
    "nl+mac", //Dutch (Macintosh)
    "nl+sun_type6", //Dutch (Sun Type 6/7)
    "nl+std", //Dutch (standard)
    "bt", //Dzongkha
    "se+ovd", //Elfdalian (Swedish, with combining ogonek)
    "us+3l", //English (3l)
    "us+3l-cros", //English (3l, Chromebook)
    "us+3l-emacs", //English (3l, emacs)
    "au", //English (Australian)
    "cm", //English (Cameroon)
    "ca+eng", //English (Canada)
    "us+carpalx", //English (Carpalx)
    "us+carpalx-full", //English (Carpalx, full optimization)
    "us+hyena-carpalx", //English (Carpalx, full optimization, Hyena Layer5)
    "us+carpalx-full-altgr-intl", //English (Carpalx, full optimization, intl., with AltGr dead keys)
    "us+hyena-carpalx-altgr-intl", //English (Carpalx, full optimization, intl., with AltGr dead keys, Hyena Layer5)
    "us+carpalx-full-intl", //English (Carpalx, full optimization, intl., with dead keys)
    "us+hyena-carpalx-intl", //English (Carpalx, full optimization, intl., with dead keys, Hyena Layer5)
    "us+carpalx-altgr-intl", //English (Carpalx, intl., with AltGr dead keys)
    "us+carpalx-intl", //English (Carpalx, intl., with dead keys)
    "us+colemak", //English (Colemak)
    "us+colemak_dh", //English (Colemak-DH)
    "us+drix", //English (Drix)
    "us+dvorak", //English (Dvorak)
    "us+dvorak-alt-intl", //English (Dvorak, alt. intl.)
    "us+dvorak-intl", //English (Dvorak, intl., with dead keys)
    "us+dvorak-l", //English (Dvorak, left-handed)
    "us+dvorak-r", //English (Dvorak, right-handed)
    "gh", //English (Ghana)
    "gh+gillbt", //English (Ghana, GILLBT)
    "gh+generic", //English (Ghana, multilingual)
    "in+eng", //English (India, with rupee)
    "us+mac", //English (Macintosh)
    "ml+us-mac", //English (Mali, US, Macintosh)
    "ml+us-intl", //English (Mali, US, intl.)
    "ng", //English (Nigeria)
    "us+norman", //English (Norman)
    "za", //English (South Africa)
    "gb", //English (UK)
    "gb+colemak", //English (UK, Colemak)
    "gb+colemak_dh", //English (UK, Colemak-DH)
    "gb+dvorak", //English (UK, Dvorak)
    "gb+dvorakukp", //English (UK, Dvorak, with UK punctuation)
    "gb+hyena", //English (UK, Hyena Layer5)
    "gb+mac", //English (UK, Macintosh)
    "gb+mac_intl", //English (UK, Macintosh, intl.)
    "gb+miniguru", //English (UK, MiniGuru Layer5)
    "gb+sun_type6", //English (UK, Sun Type 6/7)
    "gb+yoda", //English (UK, TEX Yoda Layer5)
    "gb+extd", //English (UK, extended, Windows)
    "gb+intl", //English (UK, intl., with dead keys)
    "gb+hyena-intl", //English (UK, intl., with dead keys, Hyena Layer5)
    "gb+miniguru-intl", //English (UK, intl., with dead keys, MiniGuru Layer5)
    "gb+yoda-intl", //English (UK, intl., with dead keys, TEX Yoda Layer5)
    "us", //English (US)
    "us+hyena", //English (US, Hyena Layer5)
    "us+ibm238l", //English (US, IBM Arabic 238_L)
    "us+miniguru", //English (US, MiniGuru Layer5)
    "us+sun_type6", //English (US, Sun Type 6/7)
    "us+symbolic", //English (US, Symbolic)
    "us+yoda", //English (US, TEX Yoda Layer5)
    "us+alt-intl", //English (US, alt. intl.)
    "us+hyena-alt-intl", //English (US, alt. intl., with dead keys, Hyena Layer5)
    "us+miniguru-alt-intl", //English (US, alt. intl., with dead keys, MiniGuru Layer5)
    "us+yoda-alt-intl", //English (US, alt. intl., with dead keys, TEX Yoda Layer5)
    "us+euro", //English (US, euro on 5)
    "us+intl-unicode", //English (US, intl., AltGr Unicode combining)
    "us+hyena-intl-unicode", //English (US, intl., AltGr Unicode combining, Hyena Layer5)
    "us+miniguru-intl-unicode", //English (US, intl., AltGr Unicode combining, MiniGuru Layer5)
    "us+yoda-intl-unicode", //English (US, intl., AltGr Unicode combining, TEX Yoda Layer5)
    "us+alt-intl-unicode", //English (US, intl., AltGr Unicode combining, alt.)
    "us+intl", //English (US, intl., with dead keys)
    "us+workman", //English (Workman)
    "us+workman-intl", //English (Workman, intl., with dead keys)
    "us+dvorak-classic", //English (classic Dvorak)
    "us+altgr-intl", //English (intl., with AltGr dead keys)
    "us+dvp", //English (programmer Dvorak)
    "us+olpc2", //English (the divide/multiply toggle the layout)
    "epo", //Esperanto
    "br+nativo-epo", //Esperanto (Brazil, Nativo)
    "pt+nativo-epo", //Esperanto (Portugal, Nativo)
    "epo+legacy", //Esperanto (legacy)
    "ee", //Estonian
    "ee+dvorak", //Estonian (Dvorak)
    "ee+sun_type6", //Estonian (Sun Type 6/7)
    "ee+us", //Estonian (US)
    "ee+nodeadkeys", //Estonian (no dead keys)
    "eu", //EurKEY (US)
    "gh+ewe", //Ewe
    "fo", //Faroese
    "fo+nodeadkeys", //Faroese (no dead keys)
    "ph", //Filipino
    "ph+capewell-dvorak-bay", //Filipino (Capewell-Dvorak, Baybayin)
    "ph+capewell-dvorak", //Filipino (Capewell-Dvorak, Latin)
    "ph+capewell-qwerf2k6-bay", //Filipino (Capewell-QWERF 2006, Baybayin)
    "ph+capewell-qwerf2k6", //Filipino (Capewell-QWERF 2006, Latin)
    "ph+colemak-bay", //Filipino (Colemak, Baybayin)
    "ph+colemak", //Filipino (Colemak, Latin)
    "ph+dvorak-bay", //Filipino (Dvorak, Baybayin)
    "ph+dvorak", //Filipino (Dvorak, Latin)
    "ph+qwerty-bay", //Filipino (QWERTY, Baybayin)
    "fi", //Finnish
    "fi+das", //Finnish (DAS)
    "fi+fidvorak", //Finnish (Dvorak)
    "fi+mac", //Finnish (Macintosh)
    "fi+sun_type6", //Finnish (Sun Type 6/7)
    "fi+winkeys", //Finnish (Windows)
    "fi+classic", //Finnish (classic)
    "fi+nodeadkeys", //Finnish (classic, no dead keys)
    "fr", //French
    "fr+azerty", //French (AZERTY)
    "fr+afnor", //French (AZERTY, AFNOR)
    "fr+bepo", //French (BEPO)
    "fr+bepo_afnor", //French (BEPO, AFNOR)
    "fr+bepo_latin9", //French (BEPO, Latin-9 only)
    "fr+bre", //French (Breton)
    "cm+french", //French (Cameroon)
    "ca", //French (Canada)
    "ca+fr-dvorak", //French (Canada, Dvorak)
    "ca+fr-legacy", //French (Canada, legacy)
    "cd", //French (Democratic Republic of the Congo)
    "fr+dvorak", //French (Dvorak)
    "fr+mac", //French (Macintosh)
    "ml+fr-oss", //French (Mali, alt.)
    "ma+french", //French (Morocco)
    "fr+sun_type6", //French (Sun Type 6/7)
    "ch+fr", //French (Switzerland)
    "ch+fr_mac", //French (Switzerland, Macintosh)
    "ch+sun_type6_fr", //French (Switzerland, Sun Type 6/7)
    "ch+fr_nodeadkeys", //French (Switzerland, no dead keys)
    "tg", //French (Togo)
    "fr+us-alt", //French (US with dead keys, alt.)
    "fr+us", //French (US)
    "fr+us-azerty", //French (US, AZERTY)
    "fr+oss", //French (alt.)
    "fr+oss_latin9", //French (alt., Latin-9 only)
    "fr+oss_nodeadkeys", //French (alt., no dead keys)
    "fr+latin9", //French (legacy, alt.)
    "fr+latin9_nodeadkeys", //French (legacy, alt., no dead keys)
    "fr+nodeadkeys", //French (no dead keys)
    "it+fur", //Friulian (Italy)
    "gh+fula", //Fula
    "gh+ga", //Ga
    "ge", //Georgian
    "fr+geo", //Georgian (France, AZERTY Tskapo)
    "it+geo", //Georgian (Italy)
    "ge+mess", //Georgian (MESS)
    "ge+ergonomic", //Georgian (ergonomic)
    "de", //German
    "de+adnw", //German (Aus der Neo-Welt)
    "at", //German (Austria)
    "at+mac", //German (Austria, Macintosh)
    "at+nodeadkeys", //German (Austria, no dead keys)
    "de+bone", //German (Bone)
    "de+bone_eszett_home", //German (Bone, eszett in the home row)
    "de+dvorak", //German (Dvorak)
    "de+e1", //German (E1)
    "de+e2", //German (E2)
    "de+koy", //German (KOY)
    "de+lld", //German (Ladin)
    "de+mac", //German (Macintosh)
    "de+mac_nodeadkeys", //German (Macintosh, no dead keys)
    "de+neo", //German (Neo 2)
    "de+neo_qwerty", //German (Neo, QWERTY)
    "de+neo_qwertz", //German (Neo, QWERTZ)
    "de+qwerty", //German (QWERTY)
    "de+sun_type6", //German (Sun Type 6/7)
    "ch", //German (Switzerland)
    "ch+de_mac", //German (Switzerland, Macintosh)
    "ch+sun_type6_de", //German (Switzerland, Sun Type 6/7)
    "ch+legacy", //German (Switzerland, legacy)
    "ch+de_nodeadkeys", //German (Switzerland, no dead keys)
    "de+T3", //German (T3)
    "de+us", //German (US)
    "de+deadacute", //German (dead acute)
    "de+deadgraveacute", //German (dead grave acute)
    "de+deadtilde", //German (dead tilde)
    "de+nodeadkeys", //German (no dead keys)
    "de+hu", //German (with Hungarian letters, no dead keys)
    "us+de_se_fi", //German, Swedish and Finnish (US)
    "gr", //Greek
    "gr+colemak", //Greek (Colemak)
    "gr+sun_type6", //Greek (Sun Type 6/7)
    "gr+extended", //Greek (extended)
    "gr+nodeadkeys", //Greek (no dead keys)
    "gr+polytonic", //Greek (polytonic)
    "gr+simple", //Greek (simple)
    "in+guj", //Gujarati
    "cn+altgr-pinyin", //Hanyu Pinyin (with AltGr dead keys)
    "gh+hausa", //Hausa (Ghana)
    "ng+hausa", //Hausa (Nigeria)
    "us+haw", //Hawaiian
    "il", //Hebrew
    "il+biblicalSIL", //Hebrew (Biblical, SIL phonetic)
    "il+biblical", //Hebrew (Biblical, Tiro)
    "il+lyx", //Hebrew (lyx)
    "il+phonetic", //Hebrew (phonetic)
    "in+bolnagri", //Hindi (Bolnagri)
    "in+hin-kagapa", //Hindi (KaGaPa, phonetic)
    "in+hin-wx", //Hindi (Wx)
    "hu", //Hungarian
    "hu+qwerty", //Hungarian (QWERTY)
    "hu+101_qwerty_comma_dead", //Hungarian (QWERTY, 101-key, comma, dead keys)
    "hu+101_qwerty_comma_nodead", //Hungarian (QWERTY, 101-key, comma, no dead keys)
    "hu+101_qwerty_dot_dead", //Hungarian (QWERTY, 101-key, dot, dead keys)
    "hu+101_qwerty_dot_nodead", //Hungarian (QWERTY, 101-key, dot, no dead keys)
    "hu+102_qwerty_comma_dead", //Hungarian (QWERTY, 102-key, comma, dead keys)
    "hu+102_qwerty_comma_nodead", //Hungarian (QWERTY, 102-key, comma, no dead keys)
    "hu+102_qwerty_dot_dead", //Hungarian (QWERTY, 102-key, dot, dead keys)
    "hu+102_qwerty_dot_nodead", //Hungarian (QWERTY, 102-key, dot, no dead keys)
    "hu+101_qwertz_comma_dead", //Hungarian (QWERTZ, 101-key, comma, dead keys)
    "hu+101_qwertz_comma_nodead", //Hungarian (QWERTZ, 101-key, comma, no dead keys)
    "hu+101_qwertz_dot_dead", //Hungarian (QWERTZ, 101-key, dot, dead keys)
    "hu+101_qwertz_dot_nodead", //Hungarian (QWERTZ, 101-key, dot, no dead keys)
    "hu+102_qwertz_comma_dead", //Hungarian (QWERTZ, 102-key, comma, dead keys)
    "hu+102_qwertz_comma_nodead", //Hungarian (QWERTZ, 102-key, comma, no dead keys)
    "hu+102_qwertz_dot_dead", //Hungarian (QWERTZ, 102-key, dot, dead keys)
    "hu+102_qwertz_dot_nodead", //Hungarian (QWERTZ, 102-key, dot, no dead keys)
    "hu+nodeadkeys", //Hungarian (no dead keys)
    "hu+standard", //Hungarian (standard)
    "is", //Icelandic
    "is+dvorak", //Icelandic (Dvorak)
    "is+mac", //Icelandic (Macintosh)
    "is+mac_legacy", //Icelandic (Macintosh, legacy)
    "ng+igbo", //Igbo
    "in", //Indian
    "in+iipa", //Indic IPA
    "id+phoneticx", //Indonesian (Arab Melayu, extended phonetic)
    "id", //Indonesian (Arab Melayu, phonetic)
    "jv", //Indonesian (Javanese)
    "trans", //International Phonetic Alphabet
    "ca+ike", //Inuktitut
    "iq", //Iraqi
    "ie", //Irish
    "ie+UnicodeExpert", //Irish (UnicodeExpert)
    "it", //Italian
    "it+dvorak", //Italian (Dvorak)
    "it+ibm", //Italian (IBM 142)
    "it+lld", //Italian (Ladin)
    "it+mac", //Italian (Macintosh)
    "it+sun_type6", //Italian (Sun Type 6/7)
    "it+us", //Italian (US)
    "it+winkeys", //Italian (Windows)
    "it+intl", //Italian (intl., with dead keys)
    "it+nodeadkeys", //Italian (no dead keys)
    "jp", //Japanese
    "jp+dvorak", //Japanese (Dvorak)
    "jp+kana86", //Japanese (Kana 86)
    "jp+kana", //Japanese (Kana)
    "jp+mac", //Japanese (Macintosh)
    "jp+OADG109A", //Japanese (OADG 109A)
    "nec_vndr/jp", //Japanese (PC-98)
    "jp+sun_type6", //Japanese (Sun Type 6)
    "jp+sun_type7", //Japanese (Sun Type 7, PC-compatible)
    "jp+sun_type7_suncompat", //Japanese (Sun Type 7, Sun-compatible)
    "dz+azerty-deadkeys", //Kabyle (azerty layout, dead keys)
    "dz+qwerty-gb-deadkeys", //Kabyle (qwerty-gb layout, dead keys)
    "dz+qwerty-us-deadkeys", //Kabyle (qwerty-us layout, dead keys)
    "ru+xal", //Kalmyk
    "in+kan", //Kannada
    "in+kan-kagapa", //Kannada (KaGaPa, phonetic)
    "pl+csb", //Kashubian
    "kz", //Kazakh
    "kz+latin", //Kazakh (Latin)
    "kz+ext", //Kazakh (extended)
    "kz+kazrus", //Kazakh (with Russian)
    "kh", //Khmer (Cambodia)
    "ke+kik", //Kikuyu
    "ru+kom", //Komi
    "kr", //Korean
    "kr+kr104", //Korean (101/104-key compatible)
    "kr+sun_type6", //Korean (Sun Type 6/7)
    "ir+ku_ara", //Kurdish (Iran, Arabic-Latin)
    "ir+ku_f", //Kurdish (Iran, F)
    "ir+ku_alt", //Kurdish (Iran, Latin Alt-Q)
    "ir+ku", //Kurdish (Iran, Latin Q)
    "iq+ku_ara", //Kurdish (Iraq, Arabic-Latin)
    "iq+ku_f", //Kurdish (Iraq, F)
    "iq+ku_alt", //Kurdish (Iraq, Latin Alt-Q)
    "iq+ku", //Kurdish (Iraq, Latin Q)
    "sy+ku_f", //Kurdish (Syria, F)
    "sy+ku_alt", //Kurdish (Syria, Latin Alt-Q)
    "sy+ku", //Kurdish (Syria, Latin Q)
    "tr+ku_f", //Kurdish (Turkey, F)
    "tr+ku_alt", //Kurdish (Turkey, Latin Alt-Q)
    "tr+ku", //Kurdish (Turkey, Latin Q)
    "ca+kut", //Kutenai
    "kg", //Kyrgyz
    "kg+phonetic", //Kyrgyz (phonetic)
    "la", //Lao
    "la+stea", //Lao (STEA)
    "lv", //Latvian
    "lv+colemak", //Latvian (Colemak)
    "lv+apostrophecolemak", //Latvian (Colemak, with apostrophe)
    "lv+dvorak", //Latvian (Dvorak)
    "lv+ykeydvorak", //Latvian (Dvorak, with Y)
    "lv+minuskeydvorak", //Latvian (Dvorak, with minus)
    "lv+fkey", //Latvian (F)
    "lv+sun_type6", //Latvian (Sun Type 6/7)
    "lv+adapted", //Latvian (adapted)
    "lv+apostrophe", //Latvian (apostrophe)
    "lv+ergonomic", //Latvian (ergonomic, ŪGJRMV)
    "lv+modern", //Latvian (modern)
    "lv+dvorakprogr", //Latvian (programmer Dvorak)
    "lv+ykeydvorakprogr", //Latvian (programmer Dvorak, with Y)
    "lv+minuskeydvorakprogr", //Latvian (programmer Dvorak, with minus)
    "lv+tilde", //Latvian (tilde)
    "lt", //Lithuanian
    "lt+us_dvorak", //Lithuanian (Dvorak)
    "lt+ibm", //Lithuanian (IBM LST 1205-92)
    "lt+lekp", //Lithuanian (LEKP)
    "lt+lekpa", //Lithuanian (LEKPa)
    "lt+ratise", //Lithuanian (Ratise)
    "lt+sun_type6", //Lithuanian (Sun Type 6/7)
    "lt+us", //Lithuanian (US)
    "lt+std", //Lithuanian (standard)
    "de+dsb", //Lower Sorbian
    "de+dsb_qwertz", //Lower Sorbian (QWERTZ)
    "mk", //Macedonian
    "mk+nodeadkeys", //Macedonian (no dead keys)
    "my", //Malay (Jawi, Arabic Keyboard)
    "my+phonetic", //Malay (Jawi, phonetic)
    "in+mal", //Malayalam
    "in+mal_lalitha", //Malayalam (Lalitha)
    "in+mal_enhanced", //Malayalam (enhanced Inscript, with rupee)
    "mt", //Maltese
    "mt+alt-gb", //Maltese (UK, with AltGr overrides)
    "mt+alt-us", //Maltese (US layout with AltGr overrides)
    "mt+us", //Maltese (US)
    "in+eeyek", //Manipuri (Eeyek)
    "mao", //Maori
    "in+mar-kagapa", //Marathi (KaGaPa, phonetic)
    "in+marathi", //Marathi (enhanced Inscript)
    "ru+chm", //Mari
    "cm+mmuock", //Mmuock
    "in+modi-kagapa", //Modi (KaGaPa phonetic)
    "md", //Moldavian
    "md+gag", //Moldavian (Gagauz)
    "mn", //Mongolian
    "cn+mon_trad", //Mongolian (Bichig)
    "cn+mon_trad_galik", //Mongolian (Galik)
    "cn+mon_manchu_galik", //Mongolian (Manchu Galik)
    "cn+mon_trad_manchu", //Mongolian (Manchu)
    "cn+mon_todo_galik", //Mongolian (Todo Galik)
    "cn+mon_trad_todo", //Mongolian (Todo)
    "cn+mon_trad_xibe", //Mongolian (Xibe)
    "me", //Montenegrin
    "me+cyrillic", //Montenegrin (Cyrillic)
    "me+cyrillicyz", //Montenegrin (Cyrillic, ZE and ZHE swapped)
    "me+cyrillicalternatequotes", //Montenegrin (Cyrillic, with guillemets)
    "me+latinyz", //Montenegrin (Latin, QWERTY)
    "me+latinunicode", //Montenegrin (Latin, Unicode)
    "me+latinunicodeyz", //Montenegrin (Latin, Unicode, QWERTY)
    "me+latinalternatequotes", //Montenegrin (Latin, with guillemets)
    "ca+sun_type6", //Multilingual (Canada, Sun Type 6/7)
    "gn", //N'Ko (azerty)
    "np", //Nepali
    "fi+smi", //Northern Saami (Finland)
    "no+smi", //Northern Saami (Norway)
    "no+smi_nodeadkeys", //Northern Saami (Norway, no dead keys)
    "se+smi", //Northern Saami (Sweden)
    "no", //Norwegian
    "no+colemak", //Norwegian (Colemak)
    "no+dvorak", //Norwegian (Dvorak)
    "no+mac", //Norwegian (Macintosh)
    "no+mac_nodeadkeys", //Norwegian (Macintosh, no dead keys)
    "no+sun_type6", //Norwegian (Sun Type 6/7)
    "no+winkeys", //Norwegian (Windows)
    "no+nodeadkeys", //Norwegian (no dead keys)
    "fr+oci", //Occitan
    "ie+ogam", //Ogham
    "ie+ogam_is434", //Ogham (IS434)
    "in+olck", //Ol Chiki
    "hu+oldhun", //Old Hungarian
    "hu+oldhunlig", //Old Hungarian (for ligatures)
    "tr+otk", //Old Turkic
    "in+ori", //Oriya
    "ge+os", //Ossetian (Georgia)
    "ru+os_winkeys", //Ossetian (Windows)
    "ru+os_legacy", //Ossetian (legacy)
    "rs+rue", //Pannonian Rusyn
    "af+ps", //Pashto
    "af+olpc-ps", //Pashto (Afghanistan, OLPC)
    "ir", //Persian
    "af+fa-olpc", //Persian (Afghanistan, Dari OLPC)
    "ir+pes_keypad", //Persian (with Persian keypad)
    "pl", //Polish
    "gb+pl", //Polish (British keyboard)
    "pl+colemak", //Polish (Colemak)
    "pl+colemak_dh", //Polish (Colemak-DH)
    "pl+dvorak", //Polish (Dvorak)
    "pl+dvorak_altquotes", //Polish (Dvorak, with Polish quotes on key 1)
    "pl+dvorak_quotes", //Polish (Dvorak, with Polish quotes on quotemark key)
    "de+pl", //Polish (Germany, no dead keys)
    "pl+glagolica", //Polish (Glagolica)
    "pl+qwertz", //Polish (QWERTZ)
    "pl+sun_type6", //Polish (Sun Type 6/7)
    "pl+intl", //Polish (intl., with dead keys)
    "pl+legacy", //Polish (legacy)
    "pl+dvp", //Polish (programmer Dvorak)
    "pt", //Portuguese
    "br", //Portuguese (Brazil)
    "br+dvorak", //Portuguese (Brazil, Dvorak)
    "br+thinkpad", //Portuguese (Brazil, IBM/Lenovo ThinkPad)
    "br+nativo-us", //Portuguese (Brazil, Nativo for US keyboards)
    "br+nativo", //Portuguese (Brazil, Nativo)
    "br+sun_type6", //Portuguese (Brazil, Sun Type 6/7)
    "br+nodeadkeys", //Portuguese (Brazil, no dead keys)
    "pt+colemak", //Portuguese (Colemak)
    "pt+mac", //Portuguese (Macintosh)
    "pt+mac_nodeadkeys", //Portuguese (Macintosh, no dead keys)
    "pt+nativo-us", //Portuguese (Nativo for US keyboards)
    "pt+nativo", //Portuguese (Nativo)
    "pt+sun_type6", //Portuguese (Sun Type 6/7)
    "pt+nodeadkeys", //Portuguese (no dead keys)
    "in+jhelum", //Punjabi (Gurmukhi Jhelum)
    "in+guru", //Punjabi (Gurmukhi)
    "ro", //Romanian
    "de+ro", //Romanian (Germany)
    "de+ro_nodeadkeys", //Romanian (Germany, no dead keys)
    "ro+sun_type6", //Romanian (Sun Type 6/7)
    "ro+winkeys", //Romanian (Windows)
    "ro+ergonomic", //Romanian (ergonomic Touchtype)
    "ro+std", //Romanian (standard)
    "ru", //Russian
    "by+ru", //Russian (Belarus)
    "cz+rus", //Russian (Czech, phonetic)
    "ru+dos", //Russian (DOS)
    "ge+ru", //Russian (Georgia)
    "de+ru", //Russian (Germany, phonetic)
    "de+ru-recom", //Russian (Germany, recommended)
    "de+ru-translit", //Russian (Germany, transliteration)
    "kz+ruskaz", //Russian (Kazakhstan, with Kazakh)
    "ru+mac", //Russian (Macintosh)
    "pl+ru_phonetic_dvorak", //Russian (Poland, phonetic Dvorak)
    "ru+prxn", //Russian (Polyglot and Reactionary)
    "ru+rulemak", //Russian (Rulemak, phonetic Colemak)
    "ru+sun_type6", //Russian (Sun Type 6/7)
    "se+rus", //Russian (Sweden, phonetic)
    "se+rus_nodeadkeys", //Russian (Sweden, phonetic, no dead keys)
    "us+rus", //Russian (US, phonetic)
    "ua+rstu_ru", //Russian (Ukraine, standard RSTU)
    "ru+legacy", //Russian (legacy)
    "ru+phonetic_mac", //Russian (phonetic Macintosh)
    "ru+phonetic", //Russian (phonetic)
    "ru+phonetic_azerty", //Russian (phonetic, AZERTY)
    "ru+phonetic_dvorak", //Russian (phonetic, Dvorak)
    "ru+phonetic_fr", //Russian (phonetic, French)
    "ru+phonetic_winkeys", //Russian (phonetic, Windows)
    "ru+phonetic_YAZHERTY", //Russian (phonetic, YAZHERTY)
    "ru+typewriter", //Russian (typewriter)
    "ru+typewriter-legacy", //Russian (typewriter, legacy)
    "ru+unipunct", //Russian (with US punctuation)
    "ru+ruu", //Russian (with Ukrainian-Belorussian layout)
    "tw+saisiyat", //Saisiyat (Taiwan)
    "lt+sgs", //Samogitian
    "in+san-kagapa", //Sanskrit (KaGaPa, phonetic)
    "in+san-misc", //Sanskrit symbols
    "ca+shs", //Secwepemctsin
    "rs", //Serbian
    "rs+yz", //Serbian (Cyrillic, ZE and ZHE swapped)
    "rs+alternatequotes", //Serbian (Cyrillic, with guillemets)
    "rs+latin", //Serbian (Latin)
    "rs+latinyz", //Serbian (Latin, QWERTY)
    "rs+latinunicode", //Serbian (Latin, Unicode)
    "rs+latinunicodeyz", //Serbian (Latin, Unicode, QWERTY)
    "rs+latinalternatequotes", //Serbian (Latin, with guillemets)
    "ru+srp", //Serbian (Russia)
    "rs+combiningkeys", //Serbian (combining accents instead of dead keys)
    "us+hbs", //Serbo-Croatian (US)
    "it+scn", //Sicilian
    "us+scn", //Sicilian (US keyboard)
    "pl+szl", //Silesian
    "pk+snd", //Sindhi
    "lk+us", //Sinhala (US)
    "lk", //Sinhala (phonetic)
    "sk", //Slovak
    "sk+acc", //Slovak (ACC layout, only accented letters)
    "sk+qwerty", //Slovak (QWERTY)
    "sk+qwerty_bksl", //Slovak (QWERTY, extended backslash)
    "sk+sun_type6", //Slovak (Sun Type 6/7)
    "sk+bksl", //Slovak (extended backslash)
    "si", //Slovenian
    "si+us", //Slovenian (US)
    "si+alternatequotes", //Slovenian (with guillemets)
    "es", //Spanish
    "es+dvorak", //Spanish (Dvorak)
    "latam", //Spanish (Latin American)
    "latam+colemak-gaming", //Spanish (Latin American, Colemak for gaming)
    "latam+colemak", //Spanish (Latin American, Colemak)
    "latam+dvorak", //Spanish (Latin American, Dvorak)
    "latam+deadtilde", //Spanish (Latin American, dead tilde)
    "latam+nodeadkeys", //Spanish (Latin American, no dead keys)
    "es+mac", //Spanish (Macintosh)
    "es+sun_type6", //Spanish (Sun Type 6/7)
    "es+winkeys", //Spanish (Windows)
    "es+deadtilde", //Spanish (dead tilde)
    "es+nodeadkeys", //Spanish (no dead keys)
    "ke", //Swahili (Kenya)
    "tz", //Swahili (Tanzania)
    "se", //Swedish
    "se+dvorak_a5", //Swedish (Dvorak A5)
    "se+dvorak", //Swedish (Dvorak)
    "se+us_dvorak", //Swedish (Dvorak, intl.)
    "se+mac", //Swedish (Macintosh)
    "se+sun_type6", //Swedish (Sun Type 6/7)
    "se+svdvorak", //Swedish (Svdvorak)
    "se+us", //Swedish (US)
    "se+nodeadkeys", //Swedish (no dead keys)
    "se+swl", //Swedish Sign Language
    "sy+syc", //Syriac
    "sy+syc_phonetic", //Syriac (phonetic)
    "tw", //Taiwanese
    "tw+indigenous", //Taiwanese (indigenous)
    "tj", //Tajik
    "tj+legacy", //Tajik (legacy)
    "in+tam", //Tamil (Inscript)
    "lk+tam_unicode", //Tamil (Sri Lanka, TamilNet '99)
    "lk+tam_TAB", //Tamil (Sri Lanka, TamilNet '99, TAB encoding)
    "in+tam_tamilnet_with_tam_nums", //Tamil (TamilNet '99 with Tamil numerals)
    "in+tam_tamilnet", //Tamil (TamilNet '99)
    "in+tam_tamilnet_TAB", //Tamil (TamilNet '99, TAB encoding)
    "in+tam_tamilnet_TSCII", //Tamil (TamilNet '99, TSCII encoding)
    "ru+tt", //Tatar
    "in+tel", //Telugu
    "in+tel-kagapa", //Telugu (KaGaPa, phonetic)
    "in+tel-sarala", //Telugu (Sarala)
    "th", //Thai
    "th+pat", //Thai (Pattachote)
    "th+tis", //Thai (TIS-820.2538)
    "cn+tib", //Tibetan
    "cn+tib_asciinum", //Tibetan (with ASCII numerals)
    "bw", //Tswana
    "tr", //Turkish
    "tr+alt", //Turkish (Alt-Q)
    "tr+f", //Turkish (F)
    "de+tr", //Turkish (Germany)
    "tr+sun_type6", //Turkish (Sun Type 6/7)
    "tr+intl", //Turkish (intl., with dead keys)
    "tm", //Turkmen
    "tm+alt", //Turkmen (Alt-Q)
    "ru+udm", //Udmurt
    "ara+uga", //Ugaritic instead of Arabic
    "ua", //Ukrainian
    "ua+sun_type6", //Ukrainian (Sun Type 6/7)
    "ua+winkeys", //Ukrainian (Windows)
    "ua+homophonic", //Ukrainian (homophonic)
    "ua+legacy", //Ukrainian (legacy)
    "ua+phonetic", //Ukrainian (phonetic)
    "ua+rstu", //Ukrainian (standard RSTU)
    "ua+typewriter", //Ukrainian (typewriter)
    "pk", //Urdu (Pakistan)
    "pk+urd-crulp", //Urdu (Pakistan, CRULP)
    "pk+urd-nla", //Urdu (Pakistan, NLA)
    "in+urd-winkeys", //Urdu (Windows)
    "in+urd-phonetic3", //Urdu (alt. phonetic)
    "in+urd-phonetic", //Urdu (phonetic)
    "cn+ug", //Uyghur
    "uz", //Uzbek
    "af+uz", //Uzbek (Afghanistan)
    "af+uz-olpc", //Uzbek (Afghanistan, OLPC)
    "uz+latin", //Uzbek (Latin)
    "vn", //Vietnamese
    "vn+aderty", //Vietnamese (AÐERTY)
    "vn+fr", //Vietnamese (French)
    "vn+qderty", //Vietnamese (QĐERTY)
    "vn+us", //Vietnamese (US)
    "sn", //Wolof
    "ru+sah", //Yakut
    "ng+yoruba", //Yoruba
    NULL
};

gchar* lookupLanguage(const gchar* sId)
{
    guint nId = 0;

    while (IDS[nId] != NULL)
    {
        if (g_str_equal(IDS[nId], sId))
        {
            return LANGUAGES[nId];
        }

        nId++;
    }

    g_warning("No language found for Id %s", sId);

    return "00";
}
