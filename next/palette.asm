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

EXTERN _tileset_palette
EXTERN _sprite_game_palette

;---------------------------------------------------------------------

; PUBLIC _palette_tilemap_load_default

; _palette_tilemap_load_default:
; 	; Setup palette pages
;  	NEXTREG $50, 38
; 	NEXTREG $51, 39

; 	; Setup tilemap palette
; 	NEXTREG $43, %00110000		    ; Auto increment, select first tilemap palette
; 	NEXTREG $40, 0			        ; Start with first entry

; 	; Copy palette
; 	LD      HL, _tileset_palette     ; Address of palette data in memory
; 	LD      B,  0	                ; == 256	; Copy 16*16 colours
; 	CALL    palette_load		    ; Call routine for copying

;     RET

; ;---------------------------------------------------------------------

; PUBLIC _palette_sprite_load_default

; _palette_sprite_load_default:
; 	; Setup sprites palette
; 	NEXTREG $43, %00100000		    ; Auto increment, select first sprites palette
; 	NEXTREG $40, 0			        ; Start with first entry

; 	; Setup palette pages
;  	NEXTREG $50, 38
; 	NEXTREG $51, 39

; 	; Copy palette
; 	LD      HL, _sprite_game_palette      ; Address of palette data in memory
; 	LD      B,  0	                ; == 256	; Copy 16*16 colours
; 	CALL    palette_load		    ; Call routine for copying

;     RET

;---------------------------------------------------------------------

PUBLIC _palette_tilemap_load

_palette_tilemap_load:
    ; retrieve parameter restoring stack
    POP     BC      ; return address
    POP     HL      ; palette address
    PUSH    HL      ; restore stack
    PUSH    BC      ; restore stack

	; don't setup palette pages, now it is done at update_palette
 	; NEXTREG $50, 38
	; NEXTREG $51, 39

	; Setup tilemap palette
	NEXTREG $43, %00110000		    ; Auto increment, select first tilemap palette
	NEXTREG $40, 0

	; Copy palette
	LD      B,  0
	CALL    palette_load		    ; Call routine for copying

    RET

;---------------------------------------------------------------------

PUBLIC _palette_sprite_load

_palette_sprite_load:
    ; retrieve parameter restoring stack
    POP     BC      ; return address
    POP     HL      ; palette address
    PUSH    HL      ; restore stack
    PUSH    BC      ; restore stack

	; don't setup palette pages, now it is done at update_palette
 	; NEXTREG $50, 38
	; NEXTREG $51, 39

	; Setup sprites palette
	NEXTREG $43, %00100000		    ; Auto increment, select first sprites palette
	NEXTREG $40, 0			        ; Start with first entry

	; Copy palette
	LD      B,  0	                ; == 256	; Copy 16*16 colours
	CALL    palette_load		    ; Call routine for copying

    RET

;---------------------------------------------------------------------

PUBLIC _palette_layer2_load

_palette_layer2_load:
    ; retrieve parameter restoring stack
    POP     BC      ; return address
    POP     HL      ; palette address
    PUSH    HL      ; restore stack
    PUSH    BC      ; restore stack

	; don't setup palette pages, now it is done at update_palette
 	; NEXTREG $50, 38
	; NEXTREG $51, 39

	; Setup layer2 palette
	NEXTREG $43, %00010000		    ; Auto increment, select first layer2 palette
	NEXTREG $40, 0			        ; Start with first entry

	; Copy palette
	LD      B,  0	                ; == 256	; Copy 16*16 colours
	CALL    palette_load		    ; Call routine for copying

    RET

;---------------------------------------------------------------------
; HL = memory location of the palette
palette_load_256:
	LD B, 0			; This variant always starts with 0
;---------------------------------------------------------------------
; HL = memory location of the palette
; B = number of colours to copy
palette_load:
	LD A, (HL)			; Load RRRGGGBB into A
	INC HL				; Increment to next entry
	NEXTREG $44, A		; Send entry to Next HW
	LD A, (HL)			; Load 0000000B into A
	INC HL				; Increment to next entry
	NEXTREG $44, A		; Send entry to Next HW
	DJNZ palette_load	; Repeat until B=0
	RET

;---------------------------------------------------------------------

PUBLIC _palette_tilemap_all_black

_palette_tilemap_all_black:
	; Setup tilemap palette
	NEXTREG $43, %00110000		    ; Auto increment, select first tilemap palette
	NEXTREG $40, 0			        ; Start with first entry
	; Copy palette
	LD      B,  0	                ; == 256	; Copy 16*16 colours
    LD      A,  0
ptab_loop:
    NEXTREG $44, A
    NEXTREG $44, A
    DJNZ    ptab_loop

    RET

;---------------------------------------------------------------------

PUBLIC _palette_sprite_all_black

_palette_sprite_all_black:
	; Setup sprites palette
	NEXTREG $43, %00100000		    ; Auto increment, select first sprites palette
	NEXTREG $40, 0			        ; Start with first entry
	; Copy palette
	LD      B,  0	                ; == 256	; Copy 16*16 colours
    LD      A,  0
psab_loop:
    NEXTREG $44, A
    NEXTREG $44, A
    DJNZ    psab_loop

    RET

;---------------------------------------------------------------------

PUBLIC _palette_layer2_all_black

_palette_layer2_all_black:
	; Setup layer2 palette
	NEXTREG $43, %00010000		    ; Auto increment, select first layer2 palette
	NEXTREG $40, 0			        ; Start with first entry
	; Copy palette
	LD      B,  0	                ; == 256	; Copy 16*16 colours
    LD      A,  0
plab_loop:
    NEXTREG $44, A
    NEXTREG $44, A
    DJNZ    plab_loop

    RET
