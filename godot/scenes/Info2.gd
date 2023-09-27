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

# reduced version of GameScreen

onready var G = get_node('/root/Global')
onready var BGM = get_node('/root/BGMusic')

export var levnum = 0
export var wall_offset = 0
export var plat_offset = 0

var menuExited

# Called when the node enters the scene tree for the first time.
func _ready():
	G.onEnterScreen()
	$bg360x270.color = Color.black
	analyze_tilemap()
	analyze_others()
	setup_conveyors()
	var smarto = $frame/smarto
	var dumber = $frame/dumber
	smarto.inhibited = true
	dumber.inhibited = true
	smarto.tcol = $frame/tmap_col
	dumber.tcol = $frame/tmap_col
	smarto.tgra = $frame/tmap_gra
	dumber.tgra = $frame/tmap_gra
	smarto.level = self
	dumber.level = self
	smarto.other = dumber
	dumber.other = smarto
	menuExited = false
	
	exiting_to_menu = false

	$FadeRect.connect("fadeOutFinished", self, "onFadeOutFinished")
#	BGM.playLevelSong(levnum)
	
func _physics_process(delta):
	update_conveyors()
	
################################################################################
################################################################################

func analyze_tilemap():
	var A = G.TMD	                         # 1 
	var B = G.TMD | G.TMA                    # 3
	var C = G.TMD | G.TMA | G.TMC            # 7
	var D = G.TMD | G.TMA | G.TMC | G.TML    # F
	var E = G.TME                            #20
	var F = G.TMN                            #40
	var H = G.TMD | G.TMR                    #81


	var c4g = {
		 0:B,  1:B,  2:B,  3:B,
		 4:A,  5:A,  6:A,
		 7:C,  8:C,  9:C, 10:C,
		15:D, 16:D, 17:D, 18:D,
		23:E, 24:E, 25:E, 26:0,
		27:F, 28:F, 29:F, 30:F,
		47:H, 86:F,
	}
	var tmg = $frame/tmap_gra
	var tsg = tmg.tile_set
	var tmc = $frame/tmap_col
	for y in range(G.NY):
		for x in range(G.NX):
			tmc.set_cell(x, y, 0)
	for y in range(G.NY):
		for x in range(G.NX):
			var gcell = tmg.get_cell(x, y)
			var ccell = 0
			var dx = 1
			var dy = 1
			if gcell in c4g:
				ccell = c4g[gcell]
				var sz = tsg.tile_get_region(gcell)
				dx = sz.size.x / G.TH
				dy = sz.size.y / G.TW
				for iy in range(dy):
					for ix in range(dx):
						tmc.set_cell(x+ix, y+iy, ccell)
			if x == 30 && y == 25:
				tmc.set_cell(x, y, B)
						
	if wall_offset > 0:
		for y in range(G.NY):
			for x in range(G.NX):
				var gcell = tmg.get_cell(x, y)
				if gcell < 0 or gcell > 3: continue
				gcell += 44 + 4 * wall_offset
				tmg.set_cell(x, y, gcell)		

	if plat_offset > 0:
		for y in range(G.NY):
			for x in range(G.NX):
				var gcell = tmg.get_cell(x, y)
				if gcell < 4 or gcell > 6: continue
				gcell += 61 + 3 * plat_offset
				tmg.set_cell(x, y, gcell)		

################################################################################
################################################################################

class Conveyor:
	var left
	var y
	var x1
	var x2
	var idx
	
	func _init(_left, _x, _y):
		left = _left
		y = _y
		x1 = _x
		x2 = _x
		idx = 0
		
	func print():
		var dirstr = ('left' if left else 'right')
		print('conveyor: ', dirstr, ', y = ', y, ', x = ', x1, ' to ', x2)

var convArr = []

func setup_conveyors():
	var C = G.TMD | G.TMA | G.TMC            # 7
	var D = G.TMD | G.TMA | G.TMC | G.TML    # F

	var tmc = $frame/tmap_col
	for y in range(G.NY):
		var conv = null
		for x in range(G.NX):
			var ccell = tmc.get_cell(x, y)
			if ccell == C:
				if not conv:
					conv = Conveyor.new(false, x, y)
				elif conv.left == false:
					conv.x2 = x
				else:
					convArr.append(conv)
					conv = Conveyor.new(false, x, y)
			elif ccell == D:
				if not conv:
					conv = Conveyor.new(true, x, y)
				elif conv.left == true:
					conv.x2 = x
				else:
					convArr.append(conv)
					conv = Conveyor.new(true, x, y)
			elif conv:
				convArr.append(conv)
				conv = null
		if conv:
			convArr.append(conv)
			conv = null
	for conv in convArr:
		conv.print()

#var conv_frame = 1
#var conv_count = 1

func update_conveyors():
#	if conv_frame > 0:
#		conv_frame -= 1
#		return
#	conv_frame = conv_count
	
	var tmg = $frame/tmap_gra
	for conv in convArr:
		for i in range(1 + conv.x2 - conv.x1):
			var val = tmg.get_cell(conv.x1 + i, conv.y)
			val = (val - 7) & 3
			val += 4 * conv.idx
			val += 7
			tmg.set_cell(conv.x1 + i, conv.y, val)
		if not conv.left:
			conv.idx += 1
			if conv.idx > 3: conv.idx = 0
		else:
			conv.idx -= 1
			if conv.idx < 0: conv.idx = 3
			
	

################################################################################
################################################################################

################################################################################
################################################################################
################################################################################
################################################################################

var enemyArrH = []
var enemyArrV = []
var enemyArrD = []
var enemyArrS = []
var leverArr = []
var doorArr = []

func analyze_others():
	var frame = $frame
	for child in frame.get_children():
		if !child.has_method('getType'):
			continue
		elif child.getType() == 'enemyh':
			child.tcol = $frame/tmap_col
			enemyArrH.append(child)
		elif child.getType() == 'enemyv':
			enemyArrV.append(child)
			child.tcol = $frame/tmap_col
		elif child.getType() == 'enemyd':
			enemyArrD.append(child)
			child.tcol = $frame/tmap_col
		elif child.getType() == 'enemys':
			enemyArrS.append(child)
		elif child.getType() == 'lever':
			leverArr.append(child)
		elif child.getType() == 'door':
			doorArr.append(child)
			child.tcol = $frame/tmap_col
			child.updateClosedState()
		
			
################################################################################
################################################################################

func _process(delta):
	if !menuExited and G.isPressedAnyKeyOrButton():
		exit_level()

func onDeadPlayerExited():
	$FadeRect.startFadeOut()

func onFadeOutFinished():
		if G.nextLevnum > G.LEVEL_COUNT: G.nextLevnum = 1
		get_tree().change_scene('res://scenes/Info3.tscn')
		

################################################################################
################################################################################

func exit_level():
	menuExited = true
	$FadeRect.startFadeOut()

################################################################################
################################################################################

var exiting_to_menu = false
func exitToMenu():
	if not exiting_to_menu:
		exiting_to_menu = true
		$FadeRect.startFadeOut()
		
func restartLevelKillingPlayer():
	if $frame/smarto.active:
		$frame/smarto.killPlayer()
	else:
		$frame/dumber.killPlayer()
