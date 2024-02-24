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
'00',
'Ae',
'Ak',
'Am',
'An',
'Ap',
'Ar',
'As',
'At',
'Av',
'Az',
'Ba',
'Bb',
'Be',
'Bg',
'Bi',
'Bl',
'Bm',
'Bn',
'Bo',
'Bs',
'By',
'Ca',
'Ck',
'Cl',
'Cm',
'Cn',
'Cp',
'Cs',
'Cu',
'custom',
'Cv',
'Da',
'De',
'Dl',
'Dv',
'Dz',
'Ed',
'Ee',
'Ek',
'emoji',
'En',
'Eo',
'Es',
'Et',
'Fa',
'Ff',
'Fi',
'Fo',
'Fr',
'Fu',
'Ga',
'Gh',
'Go',
'Gr',
'Gu',
'Ha',
'He',
'Hi',
'Hp',
'Hr',
'Hs',
'Hu',
'Hw',
'Hy',
'Id',
'Ig',
'Il',
'Ip',
'Iq',
'Is',
'It',
'Iu',
'Ix',
'Ja',
'Ka',
'Kb',
'Ki',
'Kk',
'Km',
'Kn',
'Ko',
'Kp',
'Kt',
'Ku',
'Kv',
'Kx',
'Ky',
'Lo',
'Ls',
'Lt',
'Lv',
'Ma',
'Mb',
'Md',
'Me',
'Mi',
'Mk',
'Ml',
'Mn',
'Mr',
'Ms',
'Mt',
'Mu',
'Mx',
'Ne',
'Nk',
'No',
'Ns',
'Oc',
'Og',
'Ol',
'Or',
'Os',
'Pa',
'Ph',
'Pl',
'Ps',
'Pt',
'Ro',
'Rs',
'Ru',
'Sa',
'Sb',
'Sd',
'Sf',
'Si',
'Sj',
'Sk',
'Sl',
'Sp',
'Sq',
'Sr',
'Sv',
'Sw',
'Sx',
'Sy',
'Sz',
'Ta',
'Te',
'Tg',
'Th',
'Tk',
'Tn',
'Tr',
'Tt',
'Tw',
'Ua',
'Ud',
'Ug',
'Uk',
'Ur',
'Uz',
'Vi',
'Wo',
'Ya',
'Yo',
'Zg',
'Zh'
]

def printHelp():

    print('')
    print('Usage:')
    print('')
    print('ayatana-indicator-keyboard-icon-generator -p <padding> -c <colour> -f <font> -n -x -o <outdir>')
    print('')
    print('<padding> The padding between the fill and the edges of the image')
    print('          Defaults to 0.8')
    print('<colour>  The colour of the fill')
    print('          Defaults to "#ffffff"')
    print('<font>    The font family')
    print('          Defaults to "Sans Regular"')
    print('-n        Do not fill the icon, draw the border only')
    print('          Not used by default')
    print('-x        Generate high colour icons')
    print('          Overrides all other options')
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
    bHighColour = False
    lOpts = []

    try:

        lOpts = getopt.getopt(sys.argv[1:], 'hp:c:f:nxo:')[0]

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

        elif sOpt == '-x':

            bHighColour = True
            fBorder = 0.0

        elif sOpt == '-o':

            pOutDir = Path(sArg)

    pOutDir.mkdir(parents=True, exist_ok=True)

    fRx = 2.4
    fSize = 24.0 - (fBorder * 2)
    fIconX = (24.0 - fSize) / 2
    fIconY = (24.0 - fSize) / 2

    if bNoFill:

        nWeight = 300
        nFont = 13
        fSize -= 0.8
        fIconX = fBorder + 0.4
        fIconY = fBorder + 0.4

    if bHighColour:

        nWeight = 500
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

            if bHighColour:

                pFile.write('<?xml version="1.0" encoding="UTF-8"?><svg version="1.1" width="24" height="24"><rect x="' + str(fIconX) + '" y="' + str(fIconY) + '" width="' + str(fSize) + '" height="' + str(fSize) + '" rx="' + str(fRx) + '" style="stroke:black;stroke-width:0.8;fill:white;"/><text x="' + str(fLayoutX) + '" y="' + str(fLayoutY) + '" style="font-family:' + sFont + ';font-weight:' + str(nWeight) + ';font-size:' + str(nFont) + ';fill:black">' + sLanguage + '</text></svg>')

            elif bNoFill:

                pFile.write('<?xml version="1.0" encoding="UTF-8"?><svg version="1.1" width="24" height="24"><rect x="' + str(fIconX) + '" y="' + str(fIconY) + '" width="' + str(fSize) + '" height="' + str(fSize) + '" rx="' + str(fRx) + '" style="stroke:' + sColour + ';stroke-width:0.8;fill:none;"/><text x="' + str(fLayoutX) + '" y="' + str(fLayoutY) + '" style="font-family:' + sFont + ';font-weight:' + str(nWeight) + ';font-size:' + str(nFont) + ';fill:' + sColour + '">' + sLanguage + '</text></svg>')

            else:

                pFile.write('<?xml version="1.0" encoding="UTF-8"?><svg version="1.1" width="24" height="24"><defs><mask id="m"><rect x="0" y="0" width="24" height="24" style="fill:white"/><text x="' + str(fLayoutX) + '" y="' + str(fLayoutY) + '" style="font-family:' + sFont + ';font-weight:' + str(nWeight) + ';font-size:' + str(nFont) + ';fill:black">' + sLanguage + '</text></mask></defs><rect x="' + str(fIconX) + '" y="' + str(fIconY) + '" width="' + str(fSize) + '" height="' + str(fSize) + '" rx="' + str(fRx) + '" mask="url(#m)" style="fill:' + sColour + '"/></svg>')

            #subprocess.run('inkscape "' + str(sOutPath) + '" --export-text-to-path --export-plain-svg --export-filename="' + str(sOutPath) + '"', shell=True, check=True);

    sys.exit(0)
