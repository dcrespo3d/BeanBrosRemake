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

extends Node2D

onready var G = get_node('/root/Global')

export var advmask = 1
export var xpre = -8
export var xpost = 8

# collision tiles
var tcol = null

var anims = ['enemy1', 'enemy2', 'enemy3']

# Called when the node enters the scene tree for the first time.
func _ready():
	if advmask > 0:
		fper = advmask
		deltax = 1
	else:
		fper = -advmask
		deltax = -1
	fctr = advmask
	xmin = position.x + 8 * xpre
	xmax = position.x + 8 * xpost
	$anisprite.play()

var xmin
var xmax
var deltax
var fper
var fctr

func getType():
	return 'enemyh'

func getAnimIndex():
	for i in range(anims.size()):
		if anims[i] == $anisprite.animation:
			return i
	return 0

func _physics_process(delta):
	# pause
	$anisprite.playing = not G.paused
	if G.paused: return
	
	# slow motion
	if G.mustSkipDueToSlowMotion():
		$ColorRect.visible = true
		return
	else:
		$ColorRect.visible = false

	fctr -= 1
	if fctr > 0: return
	fctr = fper
		
	position.x += deltax
	
	if position.x >= xmax:
		deltax = -1
	if position.x <= xmin:
		deltax = 1
		
	var cx = int(position.x) >> 3
	var dx = int(position.x) & 7
	var cy = int(position.y) >> 3
	var dy = int(position.y) & 7

	if dx == 0:
		var mask = G.TMA | G.TMG
		if deltax > 0:
			var A = tcol.get_cell(cx+2, cy)
			var B = tcol.get_cell(cx+2, cy+1)
			if A & mask or B & mask:
				deltax = -1
		else:
			var A = tcol.get_cell(cx-1, cy)
			var B = tcol.get_cell(cx-1, cy+1)
			if A & mask or B & mask:
				deltax = 1

##################################################################################################
##################################################################################################

# collision rectangle
func getCollRect():
	var r = Rect2()
	r.position = position + Vector2( 1,  1)
	r.end      = position + Vector2(14, 14)
	return r


