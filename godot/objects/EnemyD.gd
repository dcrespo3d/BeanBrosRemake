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

tool

extends Node2D

export var advx = 4
export var advy = 4
export var anim = 0 setget set_anim

# collision tiles
var tcol = null

var anims = ['enemy1', 'enemy2', 'enemy3']

var G

func _ready():
	if Engine.editor_hint: return
	G = get_node('/root/Global')
	
	if advx > 0:
		fperx = advx
		deltax = 1
	else:
		fperx = -advx
		deltax = -1
	fctrx = fperx
	
	if advy > 0:
		fpery = advy
		deltay = 1
	else:
		fpery = -advy
		deltay = -1
	fctry = fpery
	
	$anisprite.play()

var deltax
var deltay
var fperx
var fctrx
var fpery
var fctry

func set_anim(a):
	if a < 0: a += 3
	anim = a % 3
	$anisprite.animation = anims[a]

func getType():
	return 'enemyd'

func getAnimIndex():
	for i in range(anims.size()):
		if anims[i] == $anisprite.animation:
			return i
	return 0

func _physics_process(delta):
	if Engine.editor_hint: return

	# pause
	$anisprite.playing = not G.paused
	if G.paused: return
	
	# slow motion
	if G.mustSkipDueToSlowMotion():
		$ColorRect.visible = true
		return
	else:
		$ColorRect.visible = false
		
	var processX = true
	var processY = true

	fctrx -= 1
	if fctrx > 0: processX = false
	else: fctrx = fperx
	
	fctry -= 1
	if fctry > 0: processY = false
	else: fctry = fperx

	if not processX and not processY:
		return
	
	var cx = int(position.x) >> 3
	var dx = int(position.x) & 7
	var cy = int(position.y) >> 3
	var dy = int(position.y) & 7
	
	if processX:
		if dx == 0:
			var mask = G.TMA | G.TMG
			if deltax > 0:
				var A = tcol.get_cell(cx+2, cy)
				var B = tcol.get_cell(cx+2, cy+1)
				if A & mask or B & mask:
					deltax = -1
					position.x -= deltax
			else:
				var A = tcol.get_cell(cx-1, cy)
				var B = tcol.get_cell(cx-1, cy+1)
				if A & mask or B & mask:
					deltax = 1
					position.x -= deltax
		position.x += deltax

	if processY:
		if dy == 0:
			if deltay > 0:
				var mask = G.TMA | G.TMD | G.TMG
				var A = tcol.get_cell(cx,   cy+2)
				var B = tcol.get_cell(cx+1, cy+2)
				if A & mask or B & mask:
					deltay = -1
					position.y -= deltay
			else:
				var mask = G.TMA | G.TMG
				var A = tcol.get_cell(cx,   cy-1)
				var B = tcol.get_cell(cx+1, cy-1)
				if A & mask or B & mask:
					deltay = 1
					position.y -= deltay
		position.y += deltay

#
##################################################################################################
##################################################################################################

# collision rectangle
func getCollRect():
	var r = Rect2()
	r.position = position + Vector2( 1,  1)
	r.end      = position + Vector2(14, 14)
	return r


