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

PUBLIC _layer2_fill_page
_layer2_fill_page:
    ; retrieve parameter restoring stack
    POP     BC      ; return address
    POP     HL      ; palette address
    PUSH    HL      ; restore stack
    PUSH    BC      ; restore stack

    LD      A, L
    LD      HL, 0
    LD      B, 64
l2fb2:
    PUSH    BC
    LD      B, 0
l2fb1:
    LD      (HL), A
    INC     L
    DJNZ    l2fb1
    POP     BC
    INC     H
    DJNZ    l2fb2

    RET

PUBLIC _layer2_uncompress_image
_layer2_uncompress_image:
    ; retrieve parameter restoring stack
    POP     BC      ; return address
    POP     DE      ; destination address
    POP     HL      ; source address
    PUSH    HL      ; restore stack
    PUSH    DE      ; restore stack
    PUSH    BC      ; restore stack

    ; source begins at $4000
    LD      A, H
    ADD     A, $40
    LD      H, A

l2ui_decode:
    LD      A, (HL)
    CP      0
    JR      Z, l2ui_exit
    LD      B, A
    INC     HL
    LD      A, (HL)
l2ui_rep_pix:
    LD      (DE), A
    INC     DE
    DJNZ    l2ui_rep_pix
    INC     HL        
    JR      l2ui_decode

l2ui_exit:
    RET
