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

export var levnum = 0

var menuExited

var odefs
var scanning
var is_bean
var beancnt
var is3
var objidx

func _ready():
	G.onEnterScreen()
	$FadeRect.connect("fadeOutFinished", self, "onFadeOutFinished")
	$frame/smarto.inhibited = true
	$frame/dumber.inhibited = true
	menuExited = false
	odefs = []
	odefs.append($frame/fo0)
	odefs.append($frame/fo1)
	odefs.append($frame/fo2)
	odefs.append($frame/fo3)
	odefs.append($frame/fo4)
	scanning = false
	is_bean = false
	is3 = false
	beancnt = 4
	objidx = 0
	
func _process(delta):
	if !menuExited and G.isPressedAnyKeyOrButton():
		menuExited = true
		$FadeRect.startFadeOut()

func onFadeOutFinished():
	get_tree().change_scene('res://scenes/MainMenu.tscn')
		
var altframe = false
func _physics_process(delta):
	altframe = not altframe
	if altframe: return

	update_conveyor()
	update_objects()
	update_scanner()
	
const x0 = 0
const x1 = 27
const y0 = 15

func update_conveyor():
	var tmg = $frame/tmap_gra
	for x in range(x1):
		var cell = tmg.get_cell(x, y0)
		cell += 4
		if cell > 22: cell -= 16
		tmg.set_cell(x, y0, cell)
		
func choose_next_object():
	if beancnt > 0:
		beancnt -= 1
		return 'b'
	
	if is3:
		is3 = false
		beancnt = 3
	else:
		is3 = true
		beancnt = 4

	var next_obj = 'o' + String(objidx)
	objidx += 1
	if objidx >= 6:
		objidx = 0
	return next_obj

func update_objects():
	for odef in odefs:
		if odef.position.x == 160:
			scanning = true
			if odef.animation == 'b':
				is_bean = true
			else:
				is_bean = false
		if odef.position.x == 216:
			odef.position.x = -24
		if odef.position.x == -16:
			odef.animation = choose_next_object()
			print(odef.animation)
		odef.position.x += 1

func update_scanner():
	$scanning.hide()
	$bean.hide()
	$notbean.hide()
	if not scanning:
		$scanning.show()
	elif is_bean:
		$bean.show()
	else:
		$notbean.show()
		


################################################################################
################################################################################

