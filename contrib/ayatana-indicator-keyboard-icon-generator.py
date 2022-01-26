#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import gi

gi.require_version('PangoFT2', '1.0')

from gi.repository import PangoFT2, Pango
from pathlib import Path
import sys
import getopt
#import subprocess

m_lLanguages = [
'Ap', #APL
'Fa', #Afghani
'Ak', #Akan
'Sq', #Albanian
'Am', #Amharic
'Ar', #Arabic
'Hy', #Armenian
'As', #Asturian
'At', #Atsina
'Av', #Avatime
'Ae', #Avestan
'Az', #Azerbaijani
'Bm', #Bambara
'Bn', #Bangla
'Ba', #Bashkirian
'Be', #Belarusian
'Bl', #Belgian
'Bb', #Berber
'Bs', #Bosnian
'Bi', #Braille
'Bg', #Bulgarian
'By', #Burmese
'Zg', #Zawgyi
'Cm', #Cameroon
'Cn', #Canadian
'Ca', #Catalan
'Ck', #Cherokee
'Zh', #Chinese
'Cu', #Church Slavonic
'Cv', #Chuvash
'Cl', #CloGaelach
'Sx', #Coeur d'Alene Salish
'Cp', #Coptic
'Tt', #Tatar
'Hr', #Croatian
'Cs', #Czech
'Da', #Danish
'Dv', #Dhivehi
'Dl', #Dutch
'Dz', #Dzongkha
'Ed', #Elfdalian
'En', #English
'Eo', #Esperanto
'Et', #Estonian
'Ek', #EurKEY
'Ee', #Ewe
'Fo', #Faroese
'Ph', #Filipino
'Fi', #Finnish
'Fr', #French
'Fu', #Friulian
'Ff', #Fula
'Gh', #Ga
'Ka', #Georgian
'De', #German
'Gr', #Greek
'Gu', #Gujarati
'Hp', #Hanyu Pinyin
'Ha', #Hausa
'Hw', #Hawaiian
'He', #Hebrew
'Hi', #Hindi
'Hu', #Hungarian
'Is', #Icelandic
'Ig', #Igbo
'Il', #Indian
'Ip', #Indic
'Id', #Indonesian
'Ix', #International Phonetic Alphabet
'Iu', #Inuktitut
'Iq', #Iraqi
'Ga', #Irish
'It', #Italian
'Ja', #Japanese
'Kb', #Kabyle
'Kx', #Kalmyk
'Kn', #Kannada
'Kp', #Kashubian
'Kk', #Kazakh
'Km', #Khmer
'Ki', #Kikuyu
'Kv', #Komi
'Ko', #Korean
'Ku', #Kurdish
'Kt', #Kutenai
'Ky', #Kyrgyz
'Lo', #Lao
'Lv', #Latvian
'Lt', #Lithuanian
'Sb', #Lower Sorbian
'Mk', #Macedonian
'Ms', #Malay
'Ml', #Malayalam
'Mt', #Maltese
'Ma', #Manipuri
'Mi', #Maori
'Mr', #Marathi
'Mx', #Mari
'Mu', #Mmuock
'Md', #Modi
'Ro', #Moldavian
'Mn', #Mongolian
'Me', #Montenegrin
'Mb', #Multilingual
'Nk', #N'Ko
'Ne', #Nepali
'Ns', #Saami
'No', #Norwegian
'Oc', #Occitan
'Og', #Ogham
'Ol', #Ol Chiki
'Or', #Oriya
'Os', #Ossetian
'Rs', #Rusyn
'Ps', #Pashto
'Fa', #Persian
'Pl', #Polish
'Pt', #Portuguese
'Pa', #Punjabi
'Ro', #Romanian
'Ru', #Russian
'Sy', #Saisiyat
'Sf', #Samogitian
'Sa', #Sanskrit
'Sp', #Secwepemctsin
'Sr', #Serbian
'Hs', #Serbo-Croatian
'Sj', #Sicilian
'Sj', #Silesian
'Sd', #Sindhi
'Si', #Sinhala
'Sk', #Slovak
'Sl', #Slovenian
'Es', #Spanish
'Sw', #Swahili
'Sv', #Swedish
'Ls', #Syriac
'Tw', #Taiwanese
'Tg', #Tajik
'Ta', #Tamil
'Tt', #Tatar
'Te', #Telugu
'Th', #Thai
'Bo', #Tibetan
'Tn', #Tswana
'Tr', #Turkish
'Tk', #Turkmen
'Ud', #Udmurt
'Ua', #Ugaritic
'Uk', #Ukrainian
'Ur', #Urdu
'Ug', #Uyghur
'Uz', #Uzbek
'Vi', #Vietnamese
'Wo', #Wolof
'Ya', #Yakut
'Yo', #Yoruba
'emoji', #Emoji
'custom', #Custom
'00' #Unknown
]

def printHelp():

    print('')
    print('Usage:')
    print('')
    print('ayatana-indicator-keyboard-icon-generator -p <padding> -c <colour> -f <font> -n -o <outdir>')
    print('')
    print('<padding> The padding between the fill and the edges of the image')
    print('          Defaults to 0.8')
    print('<colour>  The colour of the fill')
    print('          Defaults to "#ffffff"')
    print('<font>    The font family')
    print('          Defaults to "Sans Regular"')
    print('-n        Do not fill the icon, draw the border only')
    print('          Not used by default')
    print('<outdir>  The output directory')
    print('          Defaults to the current directory')
    print('')

if __name__ == '__main__':

    fBorder = 0.8
    sColour = '#ffffff'
    sFont = "Sans Regular"
    pOutDir = Path.cwd()
    nFont = 15
    nWeight = 500
    bNoFill = False
    lOpts = []

    try:

        lOpts = getopt.getopt(sys.argv[1:], 'hp:c:f:no:')[0]

    except getopt.GetoptError:

        printHelp()
        sys.exit(1)

    for sOpt, sArg in lOpts:

        if sOpt == '-h':

            printHelp()
            sys.exit(0)

        elif sOpt == '-p':

            fBorder = float(sArg)

        elif sOpt == '-c':

            sColour = sArg

        elif sOpt == '-f':

            sFont = sArg

        elif sOpt == '-n':

            bNoFill = True

        elif sOpt == '-o':

            pOutDir = Path(sArg)

    pOutDir.mkdir(parents=True, exist_ok=True)

    fRx = 2.4

    if fRx == 0:

       fRx = 3.0

    fSize = 24.0 - (fBorder * 2)
    fIconX = (24.0 - fSize) / 2
    fIconY = (24.0 - fSize) / 2

    if bNoFill == True:

        nWeight = 300
        nFont = 13
        fSize -= 0.8
        fIconX = fBorder + 0.4
        fIconY = fBorder + 0.4

    pFontMap = PangoFT2.FontMap.new()
    pContext = Pango.FontMap.create_context(pFontMap)
    pFontDescription = Pango.FontDescription.new()
    pFontDescription.set_family(sFont)
    pFontDescription.set_weight(nWeight)
    pFontDescription.set_size(nFont * Pango.SCALE)
    pLayout = Pango.Layout.new(pContext)
    pLayout.set_font_description(pFontDescription)

    for sLanguage in m_lLanguages:

        sOutPath = pOutDir.joinpath('ayatana-indicator-keyboard-' + sLanguage + '.svg')

        with open(sOutPath, 'w') as pFile:

            if sLanguage == '00':

                sLanguage = '?'

            elif sLanguage == 'emoji':

                sLanguage = '😐'

            elif sLanguage == 'custom':

                sLanguage = '#'

            pLayout.set_text(sLanguage, -1)
            nLayoutWidth, nLayoutHeight = pLayout.get_size()
            nBaseline = pLayout.get_baseline()
            fLayoutX = (24.0 - (nLayoutWidth / Pango.SCALE)) / 2 + (fBorder * 0.1)
            fLayoutY = (24.0 - (nLayoutHeight / Pango.SCALE)) / 2 + (nBaseline / Pango.SCALE) + (fBorder * 0.1)

            if bNoFill == False:

                pFile.write('<?xml version="1.0" encoding="UTF-8"?><svg version="1.1" width="24" height="24"><defs><mask id="m"><rect x="0" y="0" width="24" height="24" style="fill:white"/><text x="' + str(fLayoutX) + '" y="' + str(fLayoutY) + '" style="font-family:' + sFont + ';font-weight:' + str(nWeight) + ';font-size:' + str(nFont) + ';fill:black">' + sLanguage + '</text></mask></defs><rect x="' + str(fIconX) + '" y="' + str(fIconY) + '" width="' + str(fSize) + '" height="' + str(fSize) + '" rx="' + str(fRx) + '" mask="url(#m)" style="fill:' + sColour + '"/></svg>')

            else:

                pFile.write('<?xml version="1.0" encoding="UTF-8"?><svg version="1.1" width="24" height="24"><rect x="' + str(fIconX) + '" y="' + str(fIconY) + '" width="' + str(fSize) + '" height="' + str(fSize) + '" rx="' + str(fRx) + '" style="stroke:' + sColour + ';stroke-width:0.8;fill:none;"/><text x="' + str(fLayoutX) + '" y="' + str(fLayoutY) + '" style="font-family:' + sFont + ';font-weight:' + str(nWeight) + ';font-size:' + str(nFont) + ';fill:' + sColour + '">' + sLanguage + '</text></svg>')
                #subprocess.run('inkscape "' + str(sOutPath) + '" --export-text-to-path --export-plain-svg --export-filename="' + str(sOutPath) + '"', shell=True, check=True);

    sys.exit(0)
