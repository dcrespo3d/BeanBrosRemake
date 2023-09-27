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
onready var BGM = get_node('/root/BGMusic')

var menuExited
var option = 0
var curY0
const curDY = 20

func _ready():
	G.onEnterScreen()
	$FadeRect.connect("fadeOutFinished", self, "onFadeOutFinished")
	menuExited = false
	BGM.playLevelSong(1)
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
	if Input.is_action_just_pressed("ui_down") or Input.is_action_just_pressed("ui_down2"):
		option += 1
	option &= 3
	
	updateCursor()
	
	if Input.is_action_just_pressed("ui_accept"):
		menuExited = true
		$FadeRect.startFadeOut()

func onFadeOutFinished():
	if option == 0:
		G.nextLevnum = 1
		get_tree().change_scene('res://scenes/MenuGame.tscn')
	elif option == 1:
		get_tree().change_scene('res://scenes/Info1.tscn')
	elif option == 2:
		get_tree().change_scene('res://scenes/Control.tscn')
	elif option == 3:
		get_tree().change_scene('res://scenes/About.tscn')

################################################################################
################################################################################

