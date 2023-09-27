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
var curX0
var optX = 0
var curDX = 56
var curY0
var optY = 0
const curDY = 40

const txtOri = 'SELECT ORIGINAL LEVEL'
const txtExt = 'SELECT EXTENDED LEVEL'

func _ready():
	G.onEnterScreen()
	$FadeRect.connect("fadeOutFinished", self, "onFadeOutFinished")
	menuExited = false	
	
	curX0 = $frame/cursor_label.rect_position.x
	curY0 = $frame/cursor_label.rect_position.y
	updateCursor()
	
	$frame/title.text = txtOri if not G.isExtLevMode else txtExt
	
	var opt = 0;
	if not G.isExtLevMode:
		opt = G.nextLevnum - 1;
	else:
		opt = G.nextLevnum - 21;
	optY = opt / 5;
	optX = opt % 5;

	setupMenu()
		
func setupMenu():
	var threshold = G.unlockedLevel.ori if not G.isExtLevMode else G.unlockedLevel.ext - 20
	for i in range(20):
		var n = 1 + i
		if n <= threshold:
			$frame/locks.find_node("Lock" + str(n)).visible = false
			$frame/numbers.find_node("lev" + str(n)).visible = true
		else:
			$frame/locks.find_node("Lock" + str(n)).visible = true
			$frame/numbers.find_node("lev" + str(n)).visible = false
	
func setupExt():
	pass

func updateCursor():
	$frame/cursor_label.rect_position.x = curX0 + curDX * optX
	$frame/cursor_bean.position.x = curX0 + curDX * optX
	$frame/cursor_label.rect_position.y = curY0 + curDY * optY
	$frame/cursor_bean.position.y = curY0 + curDY * optY
	
func _physics_process(delta):
	if menuExited:
		return
	
	if Input.is_action_just_pressed("ui_left") or Input.is_action_just_pressed("ui_left2"):
		optX -= 1
		if optX < 0: optX = 4
	if Input.is_action_just_pressed("ui_right") or Input.is_action_just_pressed("ui_right2"):
		optX += 1
		if optX > 4: optX = 0
	
	if Input.is_action_just_pressed("ui_up") or Input.is_action_just_pressed("ui_up2"):
		optY -= 1
		if optY < 0: optY = 4
	if Input.is_action_just_pressed("ui_down") or Input.is_action_just_pressed("ui_down2"):
		optY += 1
		if optY > 4: optY = 0
	
	if optY == 4: optX = 0
	
	updateCursor()
	
	if Input.is_action_just_pressed("ui_accept"):
		if canGo():
			menuExited = true
			$FadeRect.startFadeOut()
		
func canGo():
	if optY == 4:
		return true
	var levnum = 1 + optX + optY * 5
	if not G.isExtLevMode:
		return levnum <= G.unlockedLevel.ori
	else:
		return levnum <= G.unlockedLevel.ext - 20

func onFadeOutFinished():
	if optY == 4:
		get_tree().change_scene('res://scenes/MenuGame.tscn')
	else:
		var levnum = 1 + optX + optY * 5
		if not G.isExtLevMode:
			G.nextLevnum = levnum
		else:
			G.nextLevnum = 20 + levnum
		G.toNextLevel()

################################################################################
################################################################################

