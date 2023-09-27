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

export var closed = true

var cx
var cy
var tcol

# Called when the node enters the scene tree for the first time.
func _ready():
	cx = int(position.x / 8)
	cy = int(position.y / 8)
		
func _physics_process(delta):
	# pause
	# $anisprite.playing = not G.paused   # has side effects
	if G.paused: return
	
func getType():
	return 'door'
	
func toggle():
	closed = not closed
	updateClosedState()
	
func updateClosedState():
	if closed: $anisprite.animation = 'clos'
	else:       $anisprite.animation = 'open'
	$anisprite.play()
	for dy in range(2):
		for dx in range(2):
			var cell = tcol.get_cell(cx+dx, cy+dy)
			if closed: cell |= G.TMG
			else     : cell &= ~G.TMG
			tcol.set_cell(cx+dx, cy+dy, cell)		

##################################################################################################
##################################################################################################

# collision rectangle
func getCollRect():
	if not closed: return null
	var r = Rect2()
	r.position = position + Vector2( 1,  1)
	r.end      = position + Vector2(14, 14)
	return r
