/*
 * Copyright 2021-2025 Robert Tari <robert@tari.in>
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

#include <glib.h>

typedef struct
{
   gchar *sLanguage;
   gchar *sId;
} Language;

Language LANGUAGES[] =
{
    {"Sq", "al"}, //Albanian
    {"Sq", "al+plisi"}, //Albanian (Plisi)
    {"Sq", "al+veqilharxhi"}, //Albanian (Veqilharxhi)
    {"Am", "et"}, //Amharic
    {"Hy", "am"}, //Armenian
    {"Hy", "am+phonetic"}, //Armenian (phonetic)
    {"Hy", "am+phonetic-alt"}, //Armenian (alt. phonetic)
    {"Hy", "am+eastern"}, //Armenian (eastern)
    {"Hy", "am+eastern-alt"}, //Armenian (alt. eastern)
    {"Hy", "am+western"}, //Armenian (western)
    {"Ar", "ara"}, //Arabic
    {"Ar", "ara+digits"}, //Arabic (Eastern Arabic numerals)
    {"Ar", "ara+azerty"}, //Arabic (AZERTY)
    {"Ar", "ara+azerty_digits"}, //Arabic (AZERTY, Eastern Arabic numerals)
    {"Ar", "ara+buckwalter"}, //Arabic (Buckwalter)
    {"Ar", "ara+mac"}, //Arabic (Macintosh)
    {"Ar", "ara+mac-phonetic"}, //Arabic (Macintosh, phonetic)
    {"Ar", "ara+olpc"}, //Arabic (OLPC)
    {"Ar", "eg"}, //Arabic (Egypt)
    {"Iq", "iq"}, //Arabic (Iraq)
    {"Ku", "iq+ku"}, //Kurdish (Iraq, Latin Q)
    {"Ku", "iq+ku_alt"}, //Kurdish (Iraq, Latin Alt-Q)
    {"Ku", "iq+ku_f"}, //Kurdish (Iraq, F)
    {"Ku", "iq+ku_ara"}, //Kurdish (Iraq, Arabic-Latin)
    {"Ar", "ma"}, //Arabic (Morocco)
    {"Bb", "ma+tifinagh"}, //Berber (Morocco, Tifinagh)
    {"Bb", "ma+tifinagh-alt"}, //Berber (Morocco, Tifinagh alt.)
    {"Bb", "ma+tifinagh-alt-phonetic"}, //Berber (Morocco, Tifinagh phonetic, alt.)
    {"Bb", "ma+tifinagh-extended"}, //Berber (Morocco, Tifinagh extended)
    {"Bb", "ma+tifinagh-phonetic"}, //Berber (Morocco, Tifinagh phonetic)
    {"Bb", "ma+tifinagh-extended-phonetic"}, //Berber (Morocco, Tifinagh extended phonetic)
    {"Fr", "ma+french"}, //French (Morocco)
    {"Bb", "ma+rif"}, //Tarifit
    {"Ar", "sy"}, //Arabic (Syria)
    {"Ls", "sy+syc"}, //Syriac
    {"Ls", "sy+syc_phonetic"}, //Syriac (phonetic)
    {"Ku", "sy+ku"}, //Kurdish (Syria, Latin Q)
    {"Ku", "sy+ku_alt"}, //Kurdish (Syria, Latin Alt-Q)
    {"Ku", "sy+ku_f"}, //Kurdish (Syria, F)
    {"Az", "az"}, //Azerbaijani
    {"Az", "az+cyrillic"}, //Azerbaijani (Cyrillic)
    {"Bm", "ml"}, //Bambara
    {"Fr", "ml+fr-oss"}, //French (Mali, alt.)
    {"En", "ml+us-mac"}, //English (Mali, US, Macintosh)
    {"En", "ml+us-intl"}, //English (Mali, US, intl.)
    {"Bn", "bd"}, //Bangla
    {"Bn", "bd+probhat"}, //Bangla (Probhat)
    {"Be", "by"}, //Belarusian
    {"Be", "by+legacy"}, //Belarusian (legacy)
    {"Be", "by+latin"}, //Belarusian (Latin)
    {"Be", "by+intl"}, //Belarusian (intl.)
    {"Be", "by+phonetic"}, //Belarusian (phonetic)
    {"Ru", "by+ru"}, //Russian (Belarus)
    {"Bl", "be"}, //Belgian
    {"Bl", "be+oss"}, //Belgian (alt.)
    {"Bl", "be+oss_latin9"}, //Belgian (Latin-9 only, alt.)
    {"Bl", "be+iso-alternate"}, //Belgian (ISO, alt.)
    {"Bl", "be+nodeadkeys"}, //Belgian (no dead keys)
    {"Bl", "be+wang"}, //Belgian (Wang 724 AZERTY)
    {"Bb", "dz"}, //Berber (Algeria, Latin)
    {"Bb", "dz+ber"}, //Berber (Algeria, Tifinagh)
    {"Kb", "dz+azerty-deadkeys"}, //Kabyle (AZERTY, with dead keys)
    {"Kb", "dz+qwerty-gb-deadkeys"}, //Kabyle (QWERTY, UK, with dead keys)
    {"Kb", "dz+qwerty-us-deadkeys"}, //Kabyle (QWERTY, US, with dead keys)
    {"Ar", "dz+ar"}, //Arabic (Algeria)
    {"Bs", "ba"}, //Bosnian
    {"Bs", "ba+alternatequotes"}, //Bosnian (with guillemets)
    {"Bs", "ba+unicode"}, //Bosnian (with Bosnian digraphs)
    {"Bs", "ba+unicodeus"}, //Bosnian (US, with Bosnian digraphs)
    {"Bs", "ba+us"}, //Bosnian (US)
    {"Bi", "brai"}, //Braille
    {"Bi", "brai+left_hand"}, //Braille (left-handed)
    {"Bi", "brai+left_hand_invert"}, //Braille (left-handed inverted thumb)
    {"Bi", "brai+right_hand"}, //Braille (right-handed)
    {"Bi", "brai+right_hand_invert"}, //Braille (right-handed inverted thumb)
    {"Bg", "bg"}, //Bulgarian
    {"Bg", "bg+phonetic"}, //Bulgarian (traditional phonetic)
    {"Bg", "bg+bas_phonetic"}, //Bulgarian (new phonetic)
    {"Bg", "bg+bekl"}, //Bulgarian (enhanced)
    {"My", "mm"}, //Burmese
    {"Zg", "mm+zawgyi"}, //Burmese (Zawgyi)
    {"My", "mm+mnw"}, //Mon
    {"My", "mm+mnw-a1"}, //Mon (A1)
    {"My", "mm+shn"}, //Shan
    {"My", "mm+zgt"}, //Shan (Zawgyi)
    {"Zh", "cn"}, //Chinese
    {"Hp", "cn+altgr-pinyin"}, //Hanyu Pinyin Letters (with AltGr dead keys)
    {"Mn", "cn+mon_trad"}, //Mongolian (Bichig)
    {"Mn", "cn+mon_trad_todo"}, //Mongolian (Todo)
    {"Mn", "cn+mon_trad_xibe"}, //Mongolian (Xibe)
    {"Mn", "cn+mon_trad_manchu"}, //Mongolian (Manchu)
    {"Mn", "cn+mon_trad_galik"}, //Mongolian (Galik)
    {"Mn", "cn+mon_todo_galik"}, //Mongolian (Todo Galik)
    {"Mn", "cn+mon_manchu_galik"}, //Mongolian (Manchu Galik)
    {"Bo", "cn+tib"}, //Tibetan
    {"Bo", "cn+tib_asciinum"}, //Tibetan (with ASCII numerals)
    {"Ug", "cn+ug"}, //Uyghur
    {"Hr", "hr"}, //Croatian
    {"Hr", "hr+alternatequotes"}, //Croatian (with guillemets)
    {"Hr", "hr+unicode"}, //Croatian (with Croatian digraphs)
    {"Hr", "hr+unicodeus"}, //Croatian (US, with Croatian digraphs)
    {"Hr", "hr+us"}, //Croatian (US)
    {"Cs", "cz"}, //Czech
    {"Cs", "cz+bksl"}, //Czech (extra backslash)
    {"Cs", "cz+qwerty"}, //Czech (QWERTY)
    {"Cs", "cz+qwerty_bksl"}, //Czech (QWERTY, extra backslash)
    {"Cs", "cz+winkeys"}, //Czech (QWERTZ, Windows)
    {"Cs", "cz+winkeys-qwerty"}, //Czech (QWERTY, Windows)
    {"Cs", "cz+qwerty-mac"}, //Czech (QWERTY, Macintosh)
    {"Cs", "cz+ucw"}, //Czech (UCW, only accented letters)
    {"Cs", "cz+dvorak-ucw"}, //Czech (US, Dvorak, UCW support)
    {"Ru", "cz+rus"}, //Russian (Czechia, phonetic)
    {"Da", "dk"}, //Danish
    {"Da", "dk+nodeadkeys"}, //Danish (no dead keys)
    {"Da", "dk+winkeys"}, //Danish (Windows)
    {"Da", "dk+mac"}, //Danish (Macintosh)
    {"Da", "dk+mac_nodeadkeys"}, //Danish (Macintosh, no dead keys)
    {"Da", "dk+dvorak"}, //Danish (Dvorak)
    {"Fa", "af"}, //Dari
    {"Ps", "af+ps"}, //Pashto
    {"Uz", "af+uz"}, //Uzbek (Afghanistan)
    {"Fa", "af+fa-olpc"}, //Dari (Afghanistan, OLPC)
    {"Ps", "af+ps-olpc"}, //Pashto (Afghanistan, OLPC)
    {"Uz", "af+uz-olpc"}, //Uzbek (Afghanistan, OLPC)
    {"Dv", "mv"}, //Dhivehi
    {"Nl", "nl"}, //Dutch
    {"Nl", "nl+us"}, //Dutch (US)
    {"Nl", "nl+mac"}, //Dutch (Macintosh)
    {"Nl", "nl+std"}, //Dutch (standard)
    {"Dz", "bt"}, //Dzongkha
    {"En", "au"}, //English (Australia)
    {"En", "cm"}, //English (Cameroon)
    {"Fr", "cm+french"}, //French (Cameroon)
    {"Cm", "cm+qwerty"}, //Cameroon Multilingual (QWERTY, intl.)
    {"Cm", "cm+azerty"}, //Cameroon (AZERTY, intl.)
    {"Cm", "cm+dvorak"}, //Cameroon (Dvorak, intl.)
    {"Mu", "cm+mmuock"}, //Mmuock
    {"En", "gh"}, //English (Ghana)
    {"En", "gh+generic"}, //English (Ghana, multilingual)
    {"En", "gh+gillbt"}, //English (Ghana, GILLBT)
    {"Ak", "gh+akan"}, //Akan
    {"Av", "gh+avn"}, //Avatime
    {"Ee", "gh+ewe"}, //Ewe
    {"Ff", "gh+fula"}, //Fula
    {"Gh", "gh+ga"}, //Ga
    {"Ha", "gh+hausa"}, //Hausa (Ghana)
    {"En", "nz"}, //English (New Zealand)
    {"Mi", "nz+mao"}, //Maori
    {"En", "ng"}, //English (Nigeria)
    {"Ha", "ng+hausa"}, //Hausa (Nigeria)
    {"Ig", "ng+igbo"}, //Igbo
    {"Yo", "ng+yoruba"}, //Yoruba
    {"En", "za"}, //English (South Africa)
    {"En", "gb"}, //English (UK)
    {"En", "gb+extd"}, //English (UK, extended, Windows)
    {"En", "gb+intl"}, //English (UK, intl., with dead keys)
    {"En", "gb+dvorak"}, //English (UK, Dvorak)
    {"En", "gb+dvorakukp"}, //English (UK, Dvorak, with UK punctuation)
    {"En", "gb+mac"}, //English (UK, Macintosh)
    {"En", "gb+mac_intl"}, //English (UK, Macintosh, intl.)
    {"En", "gb+colemak"}, //English (UK, Colemak)
    {"En", "gb+colemak_dh"}, //English (UK, Colemak-DH)
    {"Gd", "gb+gla"}, //Scottish Gaelic
    {"Pl", "gb+pl"}, //Polish (British keyboard)
    {"En", "us"}, //English (US)
    {"En", "us+euro"}, //English (US, euro on 5)
    {"En", "us+intl"}, //English (US, intl., with dead keys)
    {"En", "us+alt-intl"}, //English (US, alt. intl.)
    {"En", "us+altgr-intl"}, //English (intl., with AltGr dead keys)
    {"En", "us+mac"}, //English (Macintosh)
    {"En", "us+colemak"}, //English (Colemak)
    {"En", "us+colemak_dh"}, //English (Colemak-DH)
    {"En", "us+colemak_dh_wide"}, //English (Colemak-DH Wide)
    {"En", "us+colemak_dh_ortho"}, //English (Colemak-DH Ortholinear)
    {"En", "us+colemak_dh_iso"}, //English (Colemak-DH ISO)
    {"En", "us+colemak_dh_wide_iso"}, //English (Colemak-DH Wide ISO)
    {"En", "us+dvorak"}, //English (Dvorak)
    {"En", "us+dvorak-intl"}, //English (Dvorak, intl., with dead keys)
    {"En", "us+dvorak-alt-intl"}, //English (Dvorak, alt. intl.)
    {"En", "us+dvorak-l"}, //English (Dvorak, left-handed)
    {"En", "us+dvorak-r"}, //English (Dvorak, right-handed)
    {"En", "us+dvorak-classic"}, //English (classic Dvorak)
    {"En", "us+dvp"}, //English (programmer Dvorak)
    {"En", "us+dvorak-mac"}, //English (Dvorak, Macintosh)
    {"En", "us+norman"}, //English (Norman)
    {"En", "us+symbolic"}, //English (US, Symbolic)
    {"En", "us+workman"}, //English (Workman)
    {"En", "us+workman-intl"}, //English (Workman, intl., with dead keys)
    {"En", "us+olpc2"}, //English (the divide/multiply toggle the layout)
    {"Ck", "us+chr"}, //Cherokee
    {"Hw", "us+haw"}, //Hawaiian
    {"Ru", "us+rus"}, //Russian (US, phonetic)
    {"Hs", "us+hbs"}, //Serbo-Croatian (US)
    {"Eo", "epo"}, //Esperanto
    {"Eo", "epo+legacy"}, //Esperanto (legacy)
    {"Et", "ee"}, //Estonian
    {"Et", "ee+nodeadkeys"}, //Estonian (no dead keys)
    {"Et", "ee+dvorak"}, //Estonian (Dvorak)
    {"Et", "ee+us"}, //Estonian (US)
    {"Fo", "fo"}, //Faroese
    {"Fo", "fo+nodeadkeys"}, //Faroese (no dead keys)
    {"Ph", "ph"}, //Filipino
    {"Ph", "ph+qwerty-bay"}, //Filipino (QWERTY, Baybayin)
    {"Ph", "ph+capewell-dvorak"}, //Filipino (Capewell-Dvorak, Latin)
    {"Ph", "ph+capewell-dvorak-bay"}, //Filipino (Capewell-Dvorak, Baybayin)
    {"Ph", "ph+capewell-qwerf2k6"}, //Filipino (Capewell-QWERF 2006, Latin)
    {"Ph", "ph+capewell-qwerf2k6-bay"}, //Filipino (Capewell-QWERF 2006, Baybayin)
    {"Ph", "ph+colemak"}, //Filipino (Colemak, Latin)
    {"Ph", "ph+colemak-bay"}, //Filipino (Colemak, Baybayin)
    {"Ph", "ph+dvorak"}, //Filipino (Dvorak, Latin)
    {"Ph", "ph+dvorak-bay"}, //Filipino (Dvorak, Baybayin)
    {"Fi", "fi"}, //Finnish
    {"Fi", "fi+winkeys"}, //Finnish (Windows)
    {"Fi", "fi+classic"}, //Finnish (classic)
    {"Fi", "fi+nodeadkeys"}, //Finnish (classic, no dead keys)
    {"Fi", "fi+mac"}, //Finnish (Macintosh)
    {"Ns", "fi+smi"}, //Northern Saami (Finland)
    {"Fr", "fr"}, //French
    {"Fr", "fr+nodeadkeys"}, //French (no dead keys)
    {"Fr", "fr+oss"}, //French (alt.)
    {"Fr", "fr+oss_nodeadkeys"}, //French (alt., no dead keys)
    {"Fr", "fr+oss_latin9"}, //French (alt., Latin-9 only)
    {"Fr", "fr+latin9"}, //French (legacy, alt.)
    {"Fr", "fr+latin9_nodeadkeys"}, //French (legacy, alt., no dead keys)
    {"Fr", "fr+azerty"}, //French (AZERTY)
    {"Fr", "fr+afnor"}, //French (AZERTY, AFNOR)
    {"Fr", "fr+bepo"}, //French (BEPO)
    {"Fr", "fr+bepo_latin9"}, //French (BEPO, Latin-9 only)
    {"Fr", "fr+bepo_afnor"}, //French (BEPO, AFNOR)
    {"Fr", "fr+dvorak"}, //French (Dvorak)
    {"Fr", "fr+mac"}, //French (Macintosh)
    {"Fr", "fr+us"}, //French (US)
    {"Fr", "fr+bre"}, //Breton (France)
    {"Oc", "fr+oci"}, //Occitan
    {"Ka", "fr+geo"}, //Georgian (France, AZERTY Tskapo)
    {"Fr", "ca"}, //French (Canada)
    {"Fr", "ca+fr-dvorak"}, //French (Canada, Dvorak)
    {"Fr", "ca+fr-legacy"}, //French (Canada, legacy)
    {"Cn", "ca+multix"}, //Canadian (CSA)
    {"En", "ca+eng"}, //English (Canada)
    {"Iu", "ca+ike"}, //Inuktitut
    {"Fr", "cd"}, //French (Democratic Republic of the Congo)
    {"Fr", "tg"}, //French (Togo)
    {"Ka", "ge"}, //Georgian
    {"Ka", "ge+ergonomic"}, //Georgian (ergonomic)
    {"Ka", "ge+mess"}, //Georgian (MESS)
    {"Os", "ge+os"}, //Ossetian (Georgia)
    {"Ru", "ge+ru"}, //Russian (Georgia)
    {"De", "de"}, //German
    {"De", "de+deadacute"}, //German (dead acute)
    {"De", "de+deadgraveacute"}, //German (dead grave acute)
    {"De", "de+deadtilde"}, //German (dead tilde)
    {"De", "de+nodeadkeys"}, //German (no dead keys)
    {"De", "de+e1"}, //German (E1)
    {"De", "de+e2"}, //German (E2)
    {"De", "de+T3"}, //German (T3)
    {"De", "de+us"}, //German (US)
    {"De", "de+dvorak"}, //German (Dvorak)
    {"De", "de+mac"}, //German (Macintosh)
    {"De", "de+mac_nodeadkeys"}, //German (Macintosh, no dead keys)
    {"De", "de+neo"}, //German (Neo 2)
    {"De", "de+qwerty"}, //German (QWERTY)
    {"Sb", "de+dsb"}, //Lower Sorbian
    {"Sb", "de+dsb_qwertz"}, //Lower Sorbian (QWERTZ)
    {"Ro", "de+ro"}, //Romanian (Germany)
    {"Ro", "de+ro_nodeadkeys"}, //Romanian (Germany, no dead keys)
    {"Ru", "de+ru"}, //Russian (Germany, phonetic)
    {"Tr", "de+tr"}, //Turkish (Germany)
    {"De", "at"}, //German (Austria)
    {"De", "at+nodeadkeys"}, //German (Austria, no dead keys)
    {"De", "at+mac"}, //German (Austria, Macintosh)
    {"De", "ch"}, //German (Switzerland)
    {"De", "ch+de_nodeadkeys"}, //German (Switzerland, no dead keys)
    {"De", "ch+de_mac"}, //German (Switzerland, Macintosh)
    {"De", "ch+legacy"}, //German (Switzerland, legacy)
    {"Fr", "ch+fr"}, //French (Switzerland)
    {"Fr", "ch+fr_nodeadkeys"}, //French (Switzerland, no dead keys)
    {"Fr", "ch+fr_mac"}, //French (Switzerland, Macintosh)
    {"Gr", "gr"}, //Greek
    {"Gr", "gr+simple"}, //Greek (simple)
    {"Gr", "gr+extended"}, //Greek (extended)
    {"Gr", "gr+nodeadkeys"}, //Greek (no dead keys)
    {"Gr", "gr+polytonic"}, //Greek (polytonic)
    {"He", "il"}, //Hebrew
    {"He", "il+si2"}, //Hebrew (SI-1452-2)
    {"He", "il+lyx"}, //Hebrew (lyx)
    {"He", "il+phonetic"}, //Hebrew (phonetic)
    {"He", "il+biblical"}, //Hebrew (Biblical, Tiro)
    {"Hu", "hu"}, //Hungarian
    {"Hu", "hu+standard"}, //Hungarian (standard)
    {"Hu", "hu+nodeadkeys"}, //Hungarian (no dead keys)
    {"Hu", "hu+qwerty"}, //Hungarian (QWERTY)
    {"Hu", "hu+101_qwertz_comma_dead"}, //Hungarian (QWERTZ, 101-key, comma, dead keys)
    {"Hu", "hu+101_qwertz_comma_nodead"}, //Hungarian (QWERTZ, 101-key, comma, no dead keys)
    {"Hu", "hu+101_qwertz_dot_dead"}, //Hungarian (QWERTZ, 101-key, dot, dead keys)
    {"Hu", "hu+101_qwertz_dot_nodead"}, //Hungarian (QWERTZ, 101-key, dot, no dead keys)
    {"Hu", "hu+101_qwerty_comma_dead"}, //Hungarian (QWERTY, 101-key, comma, dead keys)
    {"Hu", "hu+101_qwerty_comma_nodead"}, //Hungarian (QWERTY, 101-key, comma, no dead keys)
    {"Hu", "hu+101_qwerty_dot_dead"}, //Hungarian (QWERTY, 101-key, dot, dead keys)
    {"Hu", "hu+101_qwerty_dot_nodead"}, //Hungarian (QWERTY, 101-key, dot, no dead keys)
    {"Hu", "hu+102_qwertz_comma_dead"}, //Hungarian (QWERTZ, 102-key, comma, dead keys)
    {"Hu", "hu+102_qwertz_comma_nodead"}, //Hungarian (QWERTZ, 102-key, comma, no dead keys)
    {"Hu", "hu+102_qwertz_dot_dead"}, //Hungarian (QWERTZ, 102-key, dot, dead keys)
    {"Hu", "hu+102_qwertz_dot_nodead"}, //Hungarian (QWERTZ, 102-key, dot, no dead keys)
    {"Hu", "hu+102_qwerty_comma_dead"}, //Hungarian (QWERTY, 102-key, comma, dead keys)
    {"Hu", "hu+102_qwerty_comma_nodead"}, //Hungarian (QWERTY, 102-key, comma, no dead keys)
    {"Hu", "hu+102_qwerty_dot_dead"}, //Hungarian (QWERTY, 102-key, dot, dead keys)
    {"Hu", "hu+102_qwerty_dot_nodead"}, //Hungarian (QWERTY, 102-key, dot, no dead keys)
    {"Is", "is"}, //Icelandic
    {"Is", "is+mac_legacy"}, //Icelandic (Macintosh, legacy)
    {"Is", "is+mac"}, //Icelandic (Macintosh)
    {"Is", "is+dvorak"}, //Icelandic (Dvorak)
    {"Il", "in"}, //Indian
    {"As", "in+asm-kagapa"}, //Assamese (KaGaPa, phonetic)
    {"Bn", "in+ben"}, //Bangla (India)
    {"Bn", "in+ben_probhat"}, //Bangla (India, Probhat)
    {"Bn", "in+ben_baishakhi"}, //Bangla (India, Baishakhi)
    {"Bn", "in+ben_bornona"}, //Bangla (India, Bornona)
    {"Bn", "in+ben-kagapa"}, //Bangla (India, KaGaPa, phonetic)
    {"Bn", "in+ben_gitanjali"}, //Bangla (India, Gitanjali)
    {"Bn", "in+ben_inscript"}, //Bangla (India, Baishakhi InScript)
    {"En", "in+eng"}, //English (India, with rupee)
    {"Gu", "in+guj"}, //Gujarati
    {"Gu", "in+guj-kagapa"}, //Gujarati (KaGaPa, phonetic)
    {"Hi", "in+bolnagri"}, //Hindi (Bolnagri)
    {"Hi", "in+hin-wx"}, //Hindi (Wx)
    {"Hi", "in+hin-kagapa"}, //Hindi (KaGaPa, phonetic)
    {"Kn", "in+kan"}, //Kannada
    {"Kn", "in+kan-kagapa"}, //Kannada (KaGaPa, phonetic)
    {"Ml", "in+mal"}, //Malayalam
    {"Ml", "in+mal_lalitha"}, //Malayalam (Lalitha)
    {"Ml", "in+mal_enhanced"}, //Malayalam (enhanced InScript, with rupee)
    {"Ml", "in+mal_poorna"}, //Malayalam (Poorna, extended InScript)
    {"Ma", "in+mni"}, //Manipuri (Meitei)
    {"Mr", "in+mar-kagapa"}, //Marathi (KaGaPa, phonetic)
    {"Mr", "in+marathi"}, //Marathi (enhanced InScript)
    {"Or", "in+ori"}, //Oriya
    {"Or", "in+ori-bolnagri"}, //Oriya (Bolnagri)
    {"Or", "in+ori-wx"}, //Oriya (Wx)
    {"Pa", "in+guru"}, //Punjabi (Gurmukhi)
    {"Pa", "in+jhelum"}, //Punjabi (Gurmukhi Jhelum)
    {"Sa", "in+san-kagapa"}, //Sanskrit (KaGaPa, phonetic)
    {"Sz", "in+sat"}, //Santali (Ol Chiki)
    {"Ta", "in+tamilnet"}, //Tamil (TamilNet '99)
    {"Ta", "in+tamilnet_tamilnumbers"}, //Tamil (TamilNet '99 with Tamil numerals)
    {"Ta", "in+tamilnet_TAB"}, //Tamil (TamilNet '99, TAB encoding)
    {"Ta", "in+tamilnet_TSCII"}, //Tamil (TamilNet '99, TSCII encoding)
    {"Ta", "in+tam"}, //Tamil (InScript, with Arabic numerals)
    {"Ta", "in+tam_tamilnumbers"}, //Tamil (InScript, with Tamil numerals)
    {"Te", "in+tel"}, //Telugu
    {"Te", "in+tel-kagapa"}, //Telugu (KaGaPa, phonetic)
    {"Te", "in+tel-sarala"}, //Telugu (Sarala)
    {"Ur", "in+urd-phonetic"}, //Urdu (phonetic)
    {"Ur", "in+urd-phonetic3"}, //Urdu (alt. phonetic)
    {"Ur", "in+urd-winkeys"}, //Urdu (Windows)
    {"Ip", "in+iipa"}, //Indic IPA
    {"Id", "id"}, //Indonesian (Latin)
    {"Id", "id+melayu-phonetic"}, //Indonesian (Arab Melayu, phonetic)
    {"Id", "id+melayu-phoneticx"}, //Indonesian (Arab Melayu, extended phonetic)
    {"Id", "id+pegon-phonetic"}, //Indonesian (Arab Pegon, phonetic)
    {"Jv", "id+javanese"}, //Javanese
    {"Ga", "ie"}, //Irish
    {"Ga", "ie+UnicodeExpert"}, //Irish (UnicodeExpert)
    {"Cl", "ie+CloGaelach"}, //CloGaelach
    {"Og", "ie+ogam"}, //Ogham
    {"Og", "ie+ogam_is434"}, //Ogham (IS434)
    {"It", "it"}, //Italian
    {"It", "it+nodeadkeys"}, //Italian (no dead keys)
    {"It", "it+winkeys"}, //Italian (Windows)
    {"It", "it+mac"}, //Italian (Macintosh)
    {"It", "it+us"}, //Italian (US)
    {"It", "it+ibm"}, //Italian (IBM 142)
    {"It", "it+intl"}, //Italian (intl., with dead keys)
    {"Fu", "it+fur"}, //Friulian (Italy)
    {"Sj", "it+scn"}, //Sicilian
    {"Ka", "it+geo"}, //Georgian (Italy)
    {"Ja", "jp"}, //Japanese
    {"Ja", "jp+kana"}, //Japanese (Kana)
    {"Ja", "jp+kana86"}, //Japanese (Kana 86)
    {"Ja", "jp+OADG109A"}, //Japanese (OADG 109A)
    {"Ja", "jp+mac"}, //Japanese (Macintosh)
    {"Ja", "jp+dvorak"}, //Japanese (Dvorak)
    {"Kk", "kz"}, //Kazakh
    {"Kk", "kz+kazrus"}, //Kazakh (with Russian)
    {"Kk", "kz+ext"}, //Kazakh (extended)
    {"Kk", "kz+latin"}, //Kazakh (Latin)
    {"Ru", "kz+ruskaz"}, //Russian (Kazakhstan, with Kazakh)
    {"Km", "kh"}, //Khmer (Cambodia)
    {"Ko", "kr"}, //Korean
    {"Ko", "kr+kr104"}, //Korean (101/104-key compatible)
    {"Ky", "kg"}, //Kyrgyz
    {"Ky", "kg+phonetic"}, //Kyrgyz (phonetic)
    {"Lo", "la"}, //Lao
    {"Lo", "la+stea"}, //Lao (STEA)
    {"Lv", "lv"}, //Latvian
    {"Lv", "lv+apostrophe"}, //Latvian (apostrophe)
    {"Lv", "lv+tilde"}, //Latvian (tilde)
    {"Lv", "lv+fkey"}, //Latvian (F)
    {"Lv", "lv+modern"}, //Latvian (Modern Latin)
    {"Lv", "lv+modern-cyr"}, //Latvian (Modern Cyrillic)
    {"Lv", "lv+ergonomic"}, //Latvian (ergonomic, ŪGJRMV)
    {"Lv", "lv+adapted"}, //Latvian (adapted)
    {"Lt", "lt"}, //Lithuanian
    {"Lt", "lt+std"}, //Lithuanian (standard)
    {"Lt", "lt+us"}, //Lithuanian (US)
    {"Lt", "lt+ibm"}, //Lithuanian (IBM)
    {"Lt", "lt+lekp"}, //Lithuanian (LEKP)
    {"Lt", "lt+lekpa"}, //Lithuanian (LEKPa)
    {"Lt", "lt+ratise"}, //Lithuanian (Ratise)
    {"Sf", "lt+sgs"}, //Samogitian
    {"Mk", "mk"}, //Macedonian
    {"Mk", "mk+nodeadkeys"}, //Macedonian (no dead keys)
    {"Ms", "my"}, //Malay (Jawi, Arabic Keyboard)
    {"Ms", "my+phonetic"}, //Malay (Jawi, phonetic)
    {"Mt", "mt"}, //Maltese
    {"Mt", "mt+us"}, //Maltese (US)
    {"Mt", "mt+alt-us"}, //Maltese (US, with AltGr overrides)
    {"Mt", "mt+alt-gb"}, //Maltese (UK, with AltGr overrides)
    {"Ro", "md"}, //Moldavian
    {"Ro", "md+gag"}, //Gagauz (Moldova)
    {"Mn", "mn"}, //Mongolian
    {"Me", "me"}, //Montenegrin
    {"Me", "me+cyrillic"}, //Montenegrin (Cyrillic)
    {"Me", "me+cyrillicyz"}, //Montenegrin (Cyrillic, ZE and ZHE swapped)
    {"Me", "me+cyrillicalternatequotes"}, //Montenegrin (Cyrillic, with guillemets)
    {"Me", "me+latinunicode"}, //Montenegrin (Latin, Unicode)
    {"Me", "me+latinyz"}, //Montenegrin (Latin, QWERTY)
    {"Me", "me+latinunicodeyz"}, //Montenegrin (Latin, Unicode, QWERTY)
    {"Me", "me+latinalternatequotes"}, //Montenegrin (Latin, with guillemets)
    {"Ne", "np"}, //Nepali
    {"Nk", "gn"}, //N'Ko (AZERTY)
    {"No", "no"}, //Norwegian
    {"No", "no+nodeadkeys"}, //Norwegian (no dead keys)
    {"No", "no+winkeys"}, //Norwegian (Windows)
    {"No", "no+mac"}, //Norwegian (Macintosh)
    {"No", "no+mac_nodeadkeys"}, //Norwegian (Macintosh, no dead keys)
    {"No", "no+colemak"}, //Norwegian (Colemak)
    {"No", "no+colemak_dh"}, //Norwegian (Colemak-DH)
    {"No", "no+colemak_dh_wide"}, //Norwegian (Colemak-DH Wide)
    {"No", "no+dvorak"}, //Norwegian (Dvorak)
    {"Ns", "no+smi"}, //Northern Saami (Norway)
    {"Ns", "no+smi_nodeadkeys"}, //Northern Saami (Norway, no dead keys)
    {"Fa", "ir"}, //Persian
    {"Fa", "ir+pes_keypad"}, //Persian (with Persian keypad)
    {"Fa", "ir+winkeys"}, //Persian (Windows)
    {"Az", "ir+azb"}, //Azerbaijani (Iran)
    {"Ku", "ir+ku"}, //Kurdish (Iran, Latin Q)
    {"Ku", "ir+ku_alt"}, //Kurdish (Iran, Latin Alt-Q)
    {"Ku", "ir+ku_f"}, //Kurdish (Iran, F)
    {"Ku", "ir+ku_ara"}, //Kurdish (Iran, Arabic-Latin)
    {"Pl", "pl"}, //Polish
    {"Pl", "pl+legacy"}, //Polish (legacy)
    {"Pl", "pl+qwertz"}, //Polish (QWERTZ)
    {"Pl", "pl+dvorak"}, //Polish (Dvorak)
    {"Pl", "pl+dvorak_quotes"}, //Polish (Dvorak, with Polish quotes on quotemark key)
    {"Pl", "pl+dvorak_altquotes"}, //Polish (Dvorak, with Polish quotes on key 1)
    {"Pl", "pl+dvp"}, //Polish (programmer Dvorak)
    {"Kp", "pl+csb"}, //Kashubian
    {"Sj", "pl+szl"}, //Silesian
    {"Ru", "pl+ru_phonetic_dvorak"}, //Russian (Poland, phonetic Dvorak)
    {"Pt", "pt"}, //Portuguese
    {"Pt", "pt+nodeadkeys"}, //Portuguese (no dead keys)
    {"Pt", "pt+mac"}, //Portuguese (Macintosh)
    {"Pt", "pt+mac_nodeadkeys"}, //Portuguese (Macintosh, no dead keys)
    {"Pt", "pt+nativo"}, //Portuguese (Nativo)
    {"Pt", "pt+nativo-us"}, //Portuguese (Nativo for US keyboards)
    {"Eo", "pt+nativo-epo"}, //Esperanto (Portugal, Nativo)
    {"Pt", "br"}, //Portuguese (Brazil)
    {"Pt", "br+nodeadkeys"}, //Portuguese (Brazil, no dead keys)
    {"Pt", "br+dvorak"}, //Portuguese (Brazil, Dvorak)
    {"Pt", "br+nativo"}, //Portuguese (Brazil, Nativo)
    {"Pt", "br+nativo-us"}, //Portuguese (Brazil, Nativo for US keyboards)
    {"Pt", "br+thinkpad"}, //Portuguese (Brazil, IBM/Lenovo ThinkPad)
    {"Eo", "br+nativo-epo"}, //Esperanto (Brazil, Nativo)
    {"Ru", "br+rus"}, //Russian (Brazil, phonetic)
    {"Ro", "ro"}, //Romanian
    {"Ro", "ro+std"}, //Romanian (standard)
    {"Ro", "ro+winkeys"}, //Romanian (Windows)
    {"Ru", "ru"}, //Russian
    {"Ru", "ru+phonetic"}, //Russian (phonetic)
    {"Ru", "ru+phonetic_winkeys"}, //Russian (phonetic, Windows)
    {"Ru", "ru+phonetic_YAZHERTY"}, //Russian (phonetic, YAZHERTY)
    {"Ru", "ru+phonetic_azerty"}, //Russian (phonetic, AZERTY)
    {"Ru", "ru+phonetic_dvorak"}, //Russian (phonetic, Dvorak)
    {"Ru", "ru+typewriter"}, //Russian (typewriter)
    {"Ru", "ru+ruchey_ru"}, //Russian (engineering, RU)
    {"Ru", "ru+ruchey_en"}, //Russian (engineering, EN)
    {"Ru", "ru+legacy"}, //Russian (legacy)
    {"Ru", "ru+typewriter-legacy"}, //Russian (typewriter, legacy)
    {"Ru", "ru+dos"}, //Russian (DOS)
    {"Ru", "ru+mac"}, //Russian (Macintosh)
    {"Ab", "ru+ab"}, //Abkhazian (Russia)
    {"Ba", "ru+bak"}, //Bashkirian
    {"Cv", "ru+cv"}, //Chuvash
    {"Cv", "ru+cv_latin"}, //Chuvash (Latin)
    {"Kx", "ru+xal"}, //Kalmyk
    {"Kv", "ru+kom"}, //Komi
    {"Mx", "ru+chm"}, //Mari
    {"Os", "ru+os_legacy"}, //Ossetian (legacy)
    {"Os", "ru+os_winkeys"}, //Ossetian (Windows)
    {"Sr", "ru+srp"}, //Serbian (Russia)
    {"Tt", "ru+tt"}, //Tatar
    {"Ud", "ru+udm"}, //Udmurt
    {"Ya", "ru+sah"}, //Yakut
    {"Sr", "rs"}, //Serbian
    {"Sr", "rs+alternatequotes"}, //Serbian (Cyrillic, with guillemets)
    {"Sr", "rs+yz"}, //Serbian (Cyrillic, ZE and ZHE swapped)
    {"Sr", "rs+latin"}, //Serbian (Latin)
    {"Sr", "rs+latinalternatequotes"}, //Serbian (Latin, with guillemets)
    {"Sr", "rs+latinunicode"}, //Serbian (Latin, Unicode)
    {"Sr", "rs+latinyz"}, //Serbian (Latin, QWERTY)
    {"Sr", "rs+latinunicodeyz"}, //Serbian (Latin, Unicode, QWERTY)
    {"Rs", "rs+rue"}, //Pannonian Rusyn
    {"Si", "lk"}, //Sinhala (phonetic)
    {"Si", "lk+us"}, //Sinhala (US)
    {"Ta", "lk+tam_unicode"}, //Tamil (Sri Lanka, TamilNet '99)
    {"Ta", "lk+tam_TAB"}, //Tamil (Sri Lanka, TamilNet '99, TAB encoding)
    {"Sk", "sk"}, //Slovak
    {"Sk", "sk+bksl"}, //Slovak (extra backslash)
    {"Sk", "sk+qwerty"}, //Slovak (QWERTY)
    {"Sk", "sk+qwerty_bksl"}, //Slovak (QWERTY, extra backslash)
    {"Sl", "si"}, //Slovenian
    {"Sl", "si+alternatequotes"}, //Slovenian (with guillemets)
    {"Sl", "si+us"}, //Slovenian (US)
    {"Es", "es"}, //Spanish
    {"Es", "es+nodeadkeys"}, //Spanish (no dead keys)
    {"Es", "es+deadtilde"}, //Spanish (dead tilde)
    {"Es", "es+winkeys"}, //Spanish (Windows)
    {"Es", "es+dvorak"}, //Spanish (Dvorak)
    {"As", "es+ast"}, //Asturian (Spain, with bottom-dot H and L)
    {"Ca", "es+cat"}, //Catalan (Spain, with middle-dot L)
    {"Es", "latam"}, //Spanish (Latin American)
    {"Es", "latam+nodeadkeys"}, //Spanish (Latin American, no dead keys)
    {"Es", "latam+deadtilde"}, //Spanish (Latin American, dead tilde)
    {"Es", "latam+dvorak"}, //Spanish (Latin American, Dvorak)
    {"Es", "latam+colemak"}, //Spanish (Latin American, Colemak)
    {"Sw", "ke"}, //Swahili (Kenya)
    {"Ki", "ke+kik"}, //Kikuyu
    {"Sw", "tz"}, //Swahili (Tanzania)
    {"Sv", "se"}, //Swedish
    {"Sv", "se+nodeadkeys"}, //Swedish (no dead keys)
    {"Sv", "se+dvorak"}, //Swedish (Dvorak)
    {"Sv", "se+us_dvorak"}, //Swedish (Dvorak, intl.)
    {"Sv", "se+svdvorak"}, //Swedish (Svdvorak)
    {"Sv", "se+mac"}, //Swedish (Macintosh)
    {"Sv", "se+us"}, //Swedish (US)
    {"Sv", "se+swl"}, //Swedish Sign Language
    {"Ns", "se+smi"}, //Northern Saami (Sweden)
    {"Ru", "se+rus"}, //Russian (Sweden, phonetic)
    {"Tw", "tw"}, //Taiwanese
    {"Tw", "tw+indigenous"}, //Taiwanese (indigenous)
    {"Sy", "tw+saisiyat"}, //Saisiyat (Taiwan)
    {"Tg", "tj"}, //Tajik
    {"Tg", "tj+legacy"}, //Tajik (legacy)
    {"Th", "th"}, //Thai
    {"Th", "th+tis"}, //Thai (TIS-820.2538)
    {"Th", "th+pat"}, //Thai (Pattachote)
    {"Tn", "bw"}, //Tswana
    {"Tk", "tm"}, //Turkmen
    {"Tk", "tm+alt"}, //Turkmen (Alt-Q)
    {"Tr", "tr"}, //Turkish
    {"Tr", "tr+f"}, //Turkish (F)
    {"Tr", "tr+e"}, //Turkish (E)
    {"Tr", "tr+alt"}, //Turkish (Alt-Q)
    {"Tr", "tr+intl"}, //Turkish (intl., with dead keys)
    {"Ku", "tr+ku"}, //Kurdish (Turkey, Latin Q)
    {"Ku", "tr+ku_f"}, //Kurdish (Turkey, F)
    {"Ku", "tr+ku_alt"}, //Kurdish (Turkey, Latin Alt-Q)
    {"Uk", "ua"}, //Ukrainian
    {"Uk", "ua+phonetic"}, //Ukrainian (phonetic)
    {"Uk", "ua+typewriter"}, //Ukrainian (typewriter)
    {"Uk", "ua+winkeys"}, //Ukrainian (Windows)
    {"Uk", "ua+macOS"}, //Ukrainian (macOS)
    {"Uk", "ua+legacy"}, //Ukrainian (legacy)
    {"Uk", "ua+homophonic"}, //Ukrainian (homophonic)
    {"Tt", "ua+crh"}, //Crimean Tatar (Turkish Q)
    {"Tt", "ua+crh_f"}, //Crimean Tatar (Turkish F)
    {"Tt", "ua+crh_alt"}, //Crimean Tatar (Turkish Alt-Q)
    {"Ur", "pk"}, //Urdu (Pakistan)
    {"Ur", "pk+urd-crulp"}, //Urdu (Pakistan, CRULP)
    {"Ur", "pk+urd-nla"}, //Urdu (Pakistan, NLA)
    {"Ar", "pk+ara"}, //Arabic (Pakistan)
    {"Sd", "pk+snd"}, //Sindhi
    {"Uz", "uz"}, //Uzbek
    {"Uz", "uz+latin"}, //Uzbek (Latin)
    {"Vi", "vn"}, //Vietnamese
    {"Vi", "vn+us"}, //Vietnamese (US)
    {"Vi", "vn+fr"}, //Vietnamese (France)
    {"Wo", "sn"}, //Wolof
    {"custom", "custom"}, //A user-defined custom Layout
    {"An", "ancient"}, //Ancient
    {"Go", "ancient+got"}, //Gothic
    {"Ua", "ancient+uga"}, //Ugaritic
    {"Ae", "ancient+ave"}, //Avestan
    {"Ap", "apl"}, //APL
    {"Ap", "apl+dyalog"}, //APL symbols (Dyalog APL)
    {"Ap", "apl+sax"}, //APL symbols (SAX, Sharp APL for Unix)
    {"Ap", "apl+unified"}, //APL symbols (unified)
    {"Ap", "apl+apl2"}, //APL symbols (IBM APL2)
    {"Ap", "apl+aplplusII"}, //APL symbols (Manugistics APL*PLUS II)
    {"Ap", "apl+aplx"}, //APL symbols (APLX unified)
    {"Bg", "bqn"}, //BQN
    {"Kt", "ca+kut"}, //Kutenai
    {"Sp", "ca+shs"}, //Secwepemctsin
    {"Mb", "ca+sun_type6"}, //Multilingual (Canada, Sun Type 6/7)
    {"De", "de+hu"}, //German (with Hungarian letters, no dead keys)
    {"Pl", "de+pl"}, //Polish (Germany, no dead keys)
    {"De", "de+sun_type6"}, //German (Sun Type 6/7)
    {"De", "de+adnw"}, //German (Aus der Neo-Welt)
    {"De", "de+koy"}, //German (KOY)
    {"De", "de+bone"}, //German (Bone)
    {"De", "de+bone_eszett_home"}, //German (Bone, eszett in the home row)
    {"De", "de+neo_qwertz"}, //German (Neo, QWERTZ)
    {"De", "de+neo_qwerty"}, //German (Neo, QWERTY)
    {"Ru", "de+ru-recom"}, //Russian (Germany, recommended)
    {"Ru", "de+ru-translit"}, //Russian (Germany, transliteration)
    {"Cp", "eg+cop"}, //Coptic
    {"Hu", "hu+oldhun"}, //Old Hungarian
    {"Hu", "hu+oldhunlig"}, //Old Hungarian (for ligatures)
    {"Hu", "hu+us"}, //Hungarian (US)
    {"Lt", "lt+us_dvorak"}, //Lithuanian (Dvorak)
    {"Lt", "lt+sun_type6"}, //Lithuanian (Sun Type 6/7)
    {"Lv", "lv+dvorak"}, //Latvian (Dvorak)
    {"Lv", "lv+ykeydvorak"}, //Latvian (Dvorak, with Y)
    {"Lv", "lv+minuskeydvorak"}, //Latvian (Dvorak, with minus)
    {"Lv", "lv+dvorakprogr"}, //Latvian (programmer Dvorak)
    {"Lv", "lv+ykeydvorakprogr"}, //Latvian (programmer Dvorak, with Y)
    {"Lv", "lv+minuskeydvorakprogr"}, //Latvian (programmer Dvorak, with minus)
    {"Lv", "lv+colemak"}, //Latvian (Colemak)
    {"Lv", "lv+apostrophecolemak"}, //Latvian (Colemak, with apostrophe)
    {"Lv", "lv+sun_type6"}, //Latvian (Sun Type 6/7)
    {"Lv", "lv+apostrophe-deadquotes"}, //Latvian (apostrophe, dead quotes)
    {"En", "us+intl-unicode"}, //English (US, intl., AltGr Unicode combining)
    {"En", "us+alt-intl-unicode"}, //English (US, intl., AltGr Unicode combining, alt.)
    {"At", "us+ats"}, //Atsina
    {"Sx", "us+crd"}, //Coeur d'Alene Salish
    {"Cs", "us+cz_sk_de"}, //Czech, Slovak and German (US)
    {"Cs", "us+cz_sk_pl_de_es_fi_sv"}, //Czech, Slovak, Polish, Spanish, Finnish, Swedish and German (US)
    {"En", "us+drix"}, //English (Drix)
    {"De", "us+de_se_fi"}, //German, Swedish and Finnish (US)
    {"En", "us+ibm238l"}, //English (US, IBM Arabic 238_L)
    {"En", "us+sun_type6"}, //English (US, Sun Type 6/7)
    {"En", "us+carpalx"}, //English (Carpalx)
    {"En", "us+carpalx-intl"}, //English (Carpalx, intl., with dead keys)
    {"En", "us+carpalx-altgr-intl"}, //English (Carpalx, intl., with AltGr dead keys)
    {"En", "us+carpalx-full"}, //English (Carpalx, full optimization)
    {"En", "us+carpalx-full-intl"}, //English (Carpalx, full optimization, intl., with dead keys)
    {"En", "us+carpalx-full-altgr-intl"}, //English (Carpalx, full optimization, intl., with AltGr dead keys)
    {"En", "us+3l"}, //English (3l)
    {"En", "us+3l-cros"}, //English (3l, Chromebook)
    {"En", "us+3l-emacs"}, //English (3l, emacs)
    {"En", "us+workman-p"}, //English (Workman-P)
    {"Sj", "us+scn"}, //Sicilian (US keyboard)
    {"En", "us+altgr-weur"}, //English (Western European AltGr dead keys)
    {"Pl", "pl+intl"}, //Polish (intl., with dead keys)
    {"Pl", "pl+colemak"}, //Polish (Colemak)
    {"Pl", "pl+colemak_dh_ansi"}, //Polish (Colemak-DH)
    {"Pl", "pl+colemak_dh"}, //Polish (Colemak-DH ISO)
    {"Pl", "pl+sun_type6"}, //Polish (Sun Type 6/7)
    {"Pl", "pl+glagolica"}, //Polish (Glagolica)
    {"Pl", "pl+lefty"}, //Polish (lefty)
    {"Tt", "ro+crh_dobruja"}, //Crimean Tatar (Dobruja Q)
    {"Ro", "ro+ergonomic"}, //Romanian (ergonomic Touchtype)
    {"Ro", "ro+sun_type6"}, //Romanian (Sun Type 6/7)
    {"Sr", "rs+combiningkeys"}, //Serbian (combining accents instead of dead keys)
    {"Cu", "ru+chu"}, //Church Slavonic
    {"Ru", "ru+ruu"}, //Russian (plus Ukrainian and Belarusian letters)
    {"Ru", "ru+rulemak"}, //Russian (Rulemak, phonetic Colemak)
    {"Ru", "ru+phonetic_mac"}, //Russian (phonetic, Macintosh)
    {"Ru", "ru+sun_type6"}, //Russian (Sun Type 6/7)
    {"Ru", "ru+unipunct"}, //Russian (with US punctuation)
    {"Ru", "ru+gost-6431-75-48"}, //Russian (GOST 6431-75)
    {"Ru", "ru+gost-14289-88"}, //Russian (GOST 14289-88)
    {"Ru", "ru+prxn"}, //Russian (Polyglot and Reactionary)
    {"Ru", "ru+winkeys-p"}, //Russian (Programmer)
    {"Ru", "ru+typo"}, //Russian (plus typographic symbols)
    {"Ru", "ru+rtu"}, //Russian (plus Tatar letters)
    {"Hy", "am+olpc-phonetic"}, //Armenian (OLPC, phonetic)
    {"He", "il+biblicalSIL"}, //Hebrew (Biblical, SIL phonetic)
    {"Ar", "ara+sun_type6"}, //Arabic (Sun Type 6/7)
    {"Ar", "ara+basic_ext"}, //Arabic (Arabic numerals, extensions in the 4th level)
    {"Ar", "ara+basic_ext_digits"}, //Arabic (Eastern Arabic numerals, extensions in the 4th level)
    {"Ar", "ara+ergoarabic"}, //Arabic (ErgoArabic)
    {"Bl", "be+sun_type6"}, //Belgian (Sun Type 6/7)
    {"Pt", "br+sun_type6"}, //Portuguese (Brazil, Sun Type 6/7)
    {"Cs", "cz+sun_type6"}, //Czech (Sun Type 6/7)
    {"Cs", "cz+prog"}, //Czech (programming)
    {"Cs", "cz+prog_typo"}, //Czech (programming, typographic)
    {"Cs", "cz+coder"}, //Czech (coder)
    {"Cs", "cz+colemak-ucw"}, //Czech (US, Colemak, UCW support)
    {"Da", "dk+sun_type6"}, //Danish (Sun Type 6/7)
    {"Nl", "nl+sun_type6"}, //Dutch (Sun Type 6/7)
    {"Et", "ee+sun_type6"}, //Estonian (Sun Type 6/7)
    {"Fi", "fi+sun_type6"}, //Finnish (Sun Type 6/7)
    {"Fi", "fi+das"}, //Finnish (DAS)
    {"Fi", "fi+fidvorak"}, //Finnish (Dvorak)
    {"Fr", "fr+sun_type6"}, //French (Sun Type 6/7)
    {"Fr", "fr+us-alt"}, //French (US with dead keys, alt.)
    {"Fr", "fr+us-azerty"}, //French (US, AZERTY)
    {"Gr", "gr+sun_type6"}, //Greek (Sun Type 6/7)
    {"Gr", "gr+colemak"}, //Greek (Colemak)
    {"It", "it+sun_type6"}, //Italian (Sun Type 6/7)
    {"It", "it+lld"}, //Ladin (Italian keyboard)
    {"It", "it+lldde"}, //Ladin (German keyboard)
    {"It", "it+dvorak"}, //Italian (Dvorak)
    {"Ja", "jp+sun_type6"}, //Japanese (Sun Type 6)
    {"Ja", "jp+sun_type7"}, //Japanese (Sun Type 7, PC-compatible)
    {"Ja", "jp+sun_type7_suncompat"}, //Japanese (Sun Type 7, Sun-compatible)
    {"No", "no+sun_type6"}, //Norwegian (Sun Type 6/7)
    {"Ur", "pk+urd-navees"}, //Urdu (Pakistan, Navees)
    {"Pt", "pt+sun_type6"}, //Portuguese (Sun Type 6/7)
    {"Pt", "pt+colemak"}, //Portuguese (Colemak)
    {"Sk", "sk+acc"}, //Slovak (ACC layout, only accented letters)
    {"Sk", "sk+sun_type6"}, //Slovak (Sun Type 6/7)
    {"Es", "es+sun_type6"}, //Spanish (Sun Type 6/7)
    {"Sv", "se+dvorak_a5"}, //Swedish (Dvorak A5)
    {"Sv", "se+sun_type6"}, //Swedish (Sun Type 6/7)
    {"Ed", "se+ovd"}, //Elfdalian (Sweden, with combining ogonek)
    {"De", "ch+sun_type6_de"}, //German (Switzerland, Sun Type 6/7)
    {"Fr", "ch+sun_type6_fr"}, //French (Switzerland, Sun Type 6/7)
    {"Tr", "tr+sun_type6"}, //Turkish (Sun Type 6/7)
    {"Tr", "tr+us"}, //Turkish (i and ı swapped)
    {"Tr", "tr+otk"}, //Old Turkic
    {"Tr", "tr+otkf"}, //Old Turkic (F)
    {"Tr", "tr+ot"}, //Ottoman (Q)
    {"Tr", "tr+otf"}, //Ottoman (F)
    {"Uk", "ua+sun_type6"}, //Ukrainian (Sun Type 6/7)
    {"En", "gb+sun_type6"}, //English (UK, Sun Type 6/7)
    {"Ko", "kr+sun_type6"}, //Korean (Sun Type 6/7)
    {"Vi", "vn+aderty"}, //Vietnamese (AÐERTY)
    {"Vi", "vn+qderty"}, //Vietnamese (QĐERTY)
    {"Ek", "eu"}, //EurKEY (US)
    {"Ix", "trans"}, //International Phonetic Alphabet
    {"Ix", "trans+qwerty"}, //International Phonetic Alphabet (QWERTY)
    {"Md", "in+modi-kagapa"}, //Modi (KaGaPa, phonetic)
    {"Sa", "in+san-misc"}, //Sanskrit symbols
    {"Ur", "in+urd-navees"}, //Urdu (Navees)
    {"emoji", "emoji"}, //Emoji
    {NULL, NULL}
};

gchar* lookupLanguage (const gchar* sId)
{
    guint nId = 0;

    while (LANGUAGES[nId].sLanguage != NULL)
    {
        gboolean bEqual = g_str_equal (LANGUAGES[nId].sId, sId);

        if (bEqual)
        {
            return LANGUAGES[nId].sLanguage;
        }

        nId++;
    }

    g_debug ("No language found for Id %s", sId);

    return "00";
}
