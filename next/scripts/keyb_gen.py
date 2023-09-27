###############################################################################
#
#  Bean Brothers (remake) - for ZX Spectrum Next / N-Go
#
#  (c) 2023 David Crespo - https://github.com/dcrespo3d
#                          https://davidprograma.itch.io
#                          https://www.youtube.com/@Davidprograma
#
#  Based on Bean Brothers for ZX Spectrum 48/128K - (c) 2018 Dave Hughes
#
###############################################################################
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.  
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY# without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
# 
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <https://www.gnu.org/licenses/>. 
#
###############################################################################

def DEF_KEY_DOWN_FUNC(NAME, VAR, MASK):
    template = '''
PUBLIC NAME
NAME:
    LD      A, (VAR)
    AND     A, MASK
    LD      HL, 0
    JR      Z, ASMPC+3
    INC     HL
    RET
'''
    return template.replace('NAME',NAME).replace('VAR',VAR).replace('MASK',MASK)

s = ''
s += DEF_KEY_DOWN_FUNC("_zxkey1", "_zxkey54321", "0x01")
s += DEF_KEY_DOWN_FUNC("_zxkey2", "_zxkey54321", "0x02")
s += DEF_KEY_DOWN_FUNC("_zxkey3", "_zxkey54321", "0x04")
s += DEF_KEY_DOWN_FUNC("_zxkey4", "_zxkey54321", "0x08")
s += DEF_KEY_DOWN_FUNC("_zxkey5", "_zxkey54321", "0x10")
s += DEF_KEY_DOWN_FUNC("_zxkey6", "_zxkey67890", "0x10")
s += DEF_KEY_DOWN_FUNC("_zxkey7", "_zxkey67890", "0x08")
s += DEF_KEY_DOWN_FUNC("_zxkey8", "_zxkey67890", "0x04")
s += DEF_KEY_DOWN_FUNC("_zxkey9", "_zxkey67890", "0x02")
s += DEF_KEY_DOWN_FUNC("_zxkey0", "_zxkey67890", "0x01")

s += DEF_KEY_DOWN_FUNC("_zxkeyQ", "_zxkeyTREWQ", "0x01")
s += DEF_KEY_DOWN_FUNC("_zxkeyW", "_zxkeyTREWQ", "0x02")
s += DEF_KEY_DOWN_FUNC("_zxkeyE", "_zxkeyTREWQ", "0x04")
s += DEF_KEY_DOWN_FUNC("_zxkeyR", "_zxkeyTREWQ", "0x08")
s += DEF_KEY_DOWN_FUNC("_zxkeyT", "_zxkeyTREWQ", "0x10")
s += DEF_KEY_DOWN_FUNC("_zxkeyY", "_zxkeyYUIOP", "0x10")
s += DEF_KEY_DOWN_FUNC("_zxkeyU", "_zxkeyYUIOP", "0x08")
s += DEF_KEY_DOWN_FUNC("_zxkeyI", "_zxkeyYUIOP", "0x04")
s += DEF_KEY_DOWN_FUNC("_zxkeyO", "_zxkeyYUIOP", "0x02")
s += DEF_KEY_DOWN_FUNC("_zxkeyP", "_zxkeyYUIOP", "0x01")

s += DEF_KEY_DOWN_FUNC("_zxkeyA", "_zxkeyGFDSA", "0x01")
s += DEF_KEY_DOWN_FUNC("_zxkeyS", "_zxkeyGFDSA", "0x02")
s += DEF_KEY_DOWN_FUNC("_zxkeyD", "_zxkeyGFDSA", "0x04")
s += DEF_KEY_DOWN_FUNC("_zxkeyF", "_zxkeyGFDSA", "0x08")
s += DEF_KEY_DOWN_FUNC("_zxkeyG", "_zxkeyGFDSA", "0x10")
s += DEF_KEY_DOWN_FUNC("_zxkeyH", "_zxkeyHJKLe", "0x10")
s += DEF_KEY_DOWN_FUNC("_zxkeyJ", "_zxkeyHJKLe", "0x08")
s += DEF_KEY_DOWN_FUNC("_zxkeyK", "_zxkeyHJKLe", "0x04")
s += DEF_KEY_DOWN_FUNC("_zxkeyL", "_zxkeyHJKLe", "0x02")
s += DEF_KEY_DOWN_FUNC("_zxkeyENT", "_zxkeyHJKLe", "0x01")

s += DEF_KEY_DOWN_FUNC("_zxkeyCAP", "_zxkeyVCXZc", "0x01")
s += DEF_KEY_DOWN_FUNC("_zxkeyZ", "_zxkeyVCXZc", "0x02")
s += DEF_KEY_DOWN_FUNC("_zxkeyX", "_zxkeyVCXZc", "0x04")
s += DEF_KEY_DOWN_FUNC("_zxkeyC", "_zxkeyVCXZc", "0x08")
s += DEF_KEY_DOWN_FUNC("_zxkeyV", "_zxkeyVCXZc", "0x10")
s += DEF_KEY_DOWN_FUNC("_zxkeyB", "_zxkeyBNMys", "0x10")
s += DEF_KEY_DOWN_FUNC("_zxkeyN", "_zxkeyBNMys", "0x08")
s += DEF_KEY_DOWN_FUNC("_zxkeyM", "_zxkeyBNMys", "0x04")
s += DEF_KEY_DOWN_FUNC("_zxkeySYM", "_zxkeyBNMys", "0x02")
s += DEF_KEY_DOWN_FUNC("_zxkeySPC", "_zxkeyBNMys", "0x01")

s += DEF_KEY_DOWN_FUNC("_nxkeyUp", "_nxkey0", "0x08")
s += DEF_KEY_DOWN_FUNC("_nxkeyDown", "_nxkey0", "0x04")
s += DEF_KEY_DOWN_FUNC("_nxkeyLeft", "_nxkey0", "0x02")
s += DEF_KEY_DOWN_FUNC("_nxkeyRight", "_nxkey0", "0x01")
s += DEF_KEY_DOWN_FUNC("_nxkeyDel", "_nxkey1", "0x80")

print(s)