;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;  Bean Brothers (remake) - for ZX Spectrum Next / N-Go
;
;  (c) 2023 David Crespo - https://github.com/dcrespo3d
;                          https://davidprograma.itch.io
;                          https://www.youtube.com/@Davidprograma
;
;  Based on Bean Brothers for ZX Spectrum 48/128K - (c) 2018 Dave Hughes
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;  This program is free software: you can redistribute it and/or modify
;  it under the terms of the GNU General Public License as published by
;  the Free Software Foundation, either version 3 of the License, or
;  (at your option) any later version.  
;
;  This program is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;  GNU General Public License for more details.
; 
;  You should have received a copy of the GNU General Public License
;  along with this program.  If not, see <https://www.gnu.org/licenses/>. 
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SECTION code_user

; PUBLIC _player_smarto

; _player_smarto:
;     defb 0    ; cx
;     defb 0    ; cy
;     defb 0    ; dx
;     defb 0    ; dy
;     defb 0    ; active
;     defw 0    ; asdef
;     defs 16, 0; collision info

; PUBLIC _player_dumber

; _player_dumber:
;     defb 0    ; cx
;     defb 0    ; cy
;     defb 0    ; dx
;     defb 0    ; dy
;     defb 0    ; active
;     defw 0    ; asdef
;     defs 16, 0; collision info
