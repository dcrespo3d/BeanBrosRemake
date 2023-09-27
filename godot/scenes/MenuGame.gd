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

var menuExited
var option = 0
var curY0
const curDY = 48

func _ready():
	G.onEnterScreen()
	$FadeRect.connect("fadeOutFinished", self, "onFadeOutFinished")
	menuExited = false
	curY0 = $frame/cursor_label.rect_position.y
	updateCursor()

func updateCursor():
	$frame/cursor_label.rect_position.y = curY0 + curDY * option
	$frame/cursor_bean.position.y = curY0 + curDY * option
	
func _physics_process(delta):
	if menuExited:
		return
	
	if Input.is_action_just_pressed("ui_up") or Input.is_action_just_pressed("ui_up2"):
		option -= 1
		if option < 0: option = 4
	if Input.is_action_just_pressed("ui_down") or Input.is_action_just_pressed("ui_down2"):
		option += 1
		if option > 4: option = 0
	
	updateCursor()
	
	if Input.is_action_just_pressed("ui_accept"):
		menuExited = true
		$FadeRect.startFadeOut()

func onFadeOutFinished():
	if option == 0:
		G.isExtLevMode = false
		G.isLevSelMode = false
		G.nextLevnum = 1
		G.livesLost = 0
		get_tree().change_scene('res://scenes/Inter.tscn')
	elif option == 1:
		G.isExtLevMode = false
		G.isLevSelMode = true
		G.nextLevnum = 1
		G.livesLost = 0
		get_tree().change_scene('res://scenes/LevelSelect.tscn') # TODO
	elif option == 2:
		G.isExtLevMode = true
		G.isLevSelMode = false
		G.nextLevnum = 21
		G.livesLost = 0
		get_tree().change_scene('res://scenes/Inter.tscn')
	elif option == 3:
		G.isExtLevMode = true
		G.isLevSelMode = true
		G.nextLevnum = 21
		G.livesLost = 0
		get_tree().change_scene('res://scenes/LevelSelect.tscn') # TODO
	elif option == 4:
		get_tree().change_scene('res://scenes/MainMenu.tscn')

################################################################################
################################################################################

