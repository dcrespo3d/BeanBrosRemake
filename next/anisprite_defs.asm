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

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

PUBLIC _animdefs_smarto

_animdefs_smarto:
    ; anim 0: walk
    defb 0    ; start
    defb 4    ; end
    defb 4    ; dur
    defb 1    ; loop

    ; anim 1: idle
    defb 5    ; start
    defb 8    ; end
    defb 8    ; dur
    defb 1    ; loop

    ; anim 2: asleep
    defb 9    ; start
    defb 9    ; end
    defb 0    ; dur
    defb 0    ; loop

    ; anim 3: dead
    defb 10   ; start
    defb 10   ; end
    defb 0    ; dur
    defb 0    ; loop

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

PUBLIC _animdefs_dumber

_animdefs_dumber:
    ; anim 0: walk
    defb 19   ; start
    defb 23   ; end
    defb 4    ; dur
    defb 1    ; loop

    ; anim 1: idle
    defb 24   ; start
    defb 27   ; end
    defb 8    ; dur
    defb 1    ; loop

    ; anim 2: asleep
    defb 28   ; start
    defb 20   ; end
    defb 0    ; dur
    defb 0    ; loop

    ; anim 4: dead
    defb 29   ; start
    defb 21   ; end
    defb 0    ; dur
    defb 0    ; loop

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

PUBLIC _animdefs_enemy1

_animdefs_enemy1:
    ; anim 0
    defb 30   ; start
    defb 33   ; end
    defb 4    ; dur
    defb 1    ; loop

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

PUBLIC _animdefs_spikes

_animdefs_spikes:
    ; anim 0 - leave
    defb 38   ; start
    defb 34   ; end
    defb 2    ; dur
    defb 0    ; loop

    ; anim 1 - enter
    defb 34   ; start
    defb 38   ; end
    defb 2    ; dur
    defb 0    ; loop

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

PUBLIC _animdefs_enemy2

_animdefs_enemy2:
    ; anim 0
    defb 39   ; start
    defb 42   ; end
    defb 4    ; dur
    defb 1    ; loop

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

PUBLIC _animdefs_enemy3
    
_animdefs_enemy3:
    ; anim 0
    defb 43   ; start
    defb 46   ; end
    defb 4    ; dur
    defb 1    ; loop

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

PUBLIC _animdefs_lever

_animdefs_lever:
    ; anim 0 - open
    defb 47   ; start
    defb 51   ; end
    defb 2    ; dur
    defb 0    ; loop

    ; anim 1 - close
    defb 51   ; start
    defb 47   ; end
    defb 2    ; dur
    defb 0    ; loop   

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

PUBLIC _animdefs_door
    
_animdefs_door:
    ; anim 0 - open
    defb 52   ; start
    defb 56   ; end
    defb 2    ; dur
    defb 0    ; loop

    ; anim 1 - close
    defb 56   ; start
    defb 52   ; end
    defb 2    ; dur
    defb 0    ; loop   

