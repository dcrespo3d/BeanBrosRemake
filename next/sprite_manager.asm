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

SECTION code_user   ; must be as_is, do NOT capitalize code_user

;---------------------------------------------------------------------

PUBLIC _sprite_load_game_defs

EXTERN sprite_game_defs
EXTERN sprite_game_defs_len

SPDEFS_GAME: EQU 3
SPDEFS_MENU: EQU 4

spdefs_current:
	DB 0

_sprite_load_game_defs:
	; avoid loading sprite defs if already loaded
	LD		A, (spdefs_current)
	CP      A, SPDEFS_GAME
	JR      Z, slgd_exit
	; annotate current loaded defs
	LD      A, SPDEFS_GAME
	LD      (spdefs_current), A

	; load bank/page for game sprite data
 	NEXTREG $50, 32             ; page must match sprite_game_defs.asm
	NEXTREG $51, 33

	; Load sprite data using DMA
	LD HL, sprite_game_defs			; sprite data source
	                        	    ; SEE sprite_game_defs.asm!
	LD BC, sprite_game_defs_len  	; Copy sprites, each 16x16 pixels
	LD A, 0						    ; Start with first sprite
	CALL sprite_load_def_to_fpga	; Load sprites to FPGA
slgd_exit:
    RET

;---------------------------------------------------------------------

PUBLIC _sprite_load_menu_defs

EXTERN sprite_menu_defs
EXTERN sprite_menu_defs_len

_sprite_load_menu_defs:
	; avoid loading sprite defs if already loaded
	LD		A, (spdefs_current)
	CP      A, SPDEFS_MENU
	JR      Z, slmd_exit
	; annotate current loaded defs
	LD      A, SPDEFS_MENU
	LD      (spdefs_current), A

	; load bank/page for menu sprite data
 	NEXTREG $50, 42             ; page must match sprite_menu_defs.asm
	NEXTREG $51, 43

	; Load sprite data using DMA
	LD HL, sprite_menu_defs			; sprite data source
	                        	    ; SEE sprite_menu_defs.asm!
	LD BC, sprite_menu_defs_len  	; Copy sprites, each 16x16 pixels
	LD A, 0						    ; Start with first sprite
	CALL sprite_load_def_to_fpga	; Load sprites to FPGA
slmd_exit:
    RET

;---------------------------------------------------------------------

PUBLIC _sprite_setup

_sprite_setup:
  	; NEXTREG $15, %01000011		; sprite 0 on top, SLU, over border, sprites visible
  	; NEXTREG $15, %01001011		; sprite 0 on top, SUL, over border, sprites visible
	NEXTREG $15, %00000011	 ; sprite 0 on bottom, SLU, over border, sprites visible

	; Setup sprite transparency index
	NEXTREG $4B, %00001111
    RET

;---------------------------------------------------------------------
; HL = address of sprite sheet in memory
; BC = number of bytes to load
; A  = index of first sprite to load
sprite_load_def_to_fpga:
	LD (dma_source), HL	        ; Copy sprite sheet address from HL
	LD (dma_length), BC	        ; Copy length in bytes from BC
	LD BC, $303B		        ; Prepare port for sprite index
	OUT (C), A		            ; Load index of first sprite
	LD HL, dma_program	        ; Setup source for OTIR
	LD B, dma_program_length	; Setup length for OTIR
	LD C, $6B		            ; Setup DMA port
	OTIR			            ; Invoke DMA code
	RET
dma_program:
	DB %10000011		        ; WR6 - Disable DMA
	DB %01111101		        ; WR0 - append length + port A address, A->B
dma_source:
	DW 0			            ; WR0 par 1&2 - port A start address
dma_length:
	DW 0			            ; WR0 par 3&4 - transfer length
	DB %00010100		        ; WR1 - A incr, A=memory
	DB %00101000		        ; WR2 - B fixed, B=I/O
	DB %10101101		        ; WR4 - continuous, append port B address
	DW $005B		            ; WR4 par 1&2 - port B address
	DB %10000010		        ; WR5 - stop on end of block, CE only
	DB %11001111		        ; WR6 - load addresses into DMA counters
	DB %10000111		        ; WR6 - enable DMA
dma_program_length = $-dma_program

;---------------------------------------------------------------------

PUBLIC _sprite_clear_all_slots
_sprite_clear_all_slots:
	LD		L, 0
_sprite_clear_all_slots_loop:
	LD      A, L
	NEXTREG $34, A
	NEXTREG $38, 0
	INC     L
	LD		A, L
	CP      A, 128
	JR		NZ, _sprite_clear_all_slots_loop
	RET

;---------------------------------------------------------------------

PUBLIC _sprite_clear_all_from
_sprite_clear_all_from:
    ; retrieve parameter restoring stack
    POP     BC      ; return address
    POP     HL      ; initial sprite slot
    PUSH    HL      ; restore stack
    PUSH    BC      ; restore stack

_sprite_clear_all_from_loop:
	LD      A, L
	NEXTREG $34, A
	NEXTREG $38, 0
	INC     L
	LD		A, L
	CP      A, 128
	JR		NZ, _sprite_clear_all_from_loop
	RET

