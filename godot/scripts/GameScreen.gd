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

export var levnum = 0
export var wall_offset = 0
export var plat_offset = 0

#Load the resourse using preload
const pauseResource = preload("res://objects/Pause.tscn")
var pauseScene

var levelExited
var deadPlayerExited

# Called when the node enters the scene tree for the first time.
func _ready():
	G.onEnterScreen()
	$bg360x270.color = Color.black
	analyze_tilemap()
	setup_conveyors()
	if G.RUNNING_IN_EDITOR: export_tilemap()
	analyze_others()
	if G.RUNNING_IN_EDITOR: export_level()
	var smarto = $frame/smarto
	var dumber = $frame/dumber
	smarto.inhibited = false
	dumber.inhibited = false
	smarto.tcol = $frame/tmap_col
	dumber.tcol = $frame/tmap_col
	smarto.tgra = $frame/tmap_gra
	dumber.tgra = $frame/tmap_gra
	smarto.level = self
	dumber.level = self
	smarto.other = dumber
	dumber.other = smarto
	levelExited = false
	
	pauseScene = pauseResource.instance()
	pauseScene.name = "pause"
	pauseScene.level = self
	$frame.add_child(pauseScene)
	exiting_to_menu = false
	deadPlayerExited = false
	
	$FadeRect.connect("fadeOutFinished", self, "onFadeOutFinished")
	BGM.playLevelSong(levnum)
	
	if not G.isLevSelMode:
		if not G.isExtLevMode:
			if G.unlockedLevel.ori < levnum:
				G.unlockedLevel.ori = levnum
				G.saveUnlockedLevel()
		else:
			if G.unlockedLevel.ext < levnum:
				G.unlockedLevel.ext = levnum
				G.saveUnlockedLevel()
	
	
func _physics_process(delta):
	# pause
	if G.paused: return
	
	update_others()
	update_conveyors()
	
	if G.DUSE_DEBUG_KEYS:
		if Input.is_action_just_pressed("debug_exit_level"):
			exit_level()

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

const exppfx = 'res://art/'

func levsfx():
	var s = str(levnum)
	while len(s) < 2:
		s = '0' + s
	return s

func get_tilemap_fn():
	return exppfx + 'levmap' + '_' + levsfx() + '.txt'
	
func get_collmap_fn():
	return exppfx + 'levcoll' + '_' + levsfx() + '.txt'
	
func get_levdata_fn():
	return exppfx + 'levdef' + '_' + levsfx() + '.txt'

################################################################################
################################################################################

func export_tilemap():
	var empty = 10
	var border = 9
	
	var rows = []
	for irow in range(G.NY):
		var row = []
		for icol in range(G.NX):
			row.append(border)
		rows.append(row)
		
	for bg in $frame.get_children():
		if not bg.name.begins_with('bg'):
			continue
		var X0 = int(bg.rect_position.x) >> 3;
		var Y0 = int(bg.rect_position.y) >> 3;
		var X1 = X0 + (int(bg.rect_size.x) >> 3);
		var Y1 = Y0 + (int(bg.rect_size.y) >> 3);
		var s = ','
		print(X0,s,Y0,s,X1,s,Y1)	
		for y in range(G.NY):
			for x in range(G.NX):
				if x <  X0: continue
				if x >= X1: continue 
				if y <  Y0: continue
				if y >= Y1: continue
				rows[y][x] = empty

	var TMCW = 16
		
	var tmg = $frame/tmap_gra
	var tsg = tmg.tile_set
	for y in range(G.NY):
		for x in range(G.NX):
			var gcell = tmg.get_cell(x, y)
			if gcell < 0: continue
			var r = tsg.tile_get_region(gcell)
			r.position /= 8
			r.size /= 8
#			print(r)
			for cy in range(r.size.y):
				for cx in range(r.size.x):
					var val = (r.position.x + cx) + (r.position.y + cy) * TMCW
					rows[y+cy][x+cx] = val
	
	var f = File.new()
	f.open(get_tilemap_fn(), File.WRITE)
	for row in rows:
		for col in row:
			f.store_string('%2d'%col + ',')
		f.store_string('\n')
	f.close()
	
#func export_collmap():
#	var empty = 0
#
#	var rows = []
#	for irow in range(G.NY):
#		var row = []
#		for icol in range(G.NX):
#			row.append(empty)
#		rows.append(row)
#
#	var tmc = $frame/tmap_col
#	for y in range(G.NY):
#		for x in range(G.NX):
#			var c = tmc.get_cell(x, y)
#			rows[y][x] = c
#
#	var f = File.new()
#	f.open(get_collmap_fn(), File.WRITE)
#	for row in rows:
#		for col in row:
#			f.store_string('%2d'%col + ',')
#		f.store_string('\n')
#	f.close()

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
		
			
func update_others():
	var smarto = $frame/smarto
	var dumber = $frame/dumber
	for enemyH in enemyArrH:
		var erect = enemyH.getCollRect()
		if checkCollRects(erect, smarto.getCollRect()): smarto.killPlayer()
		if checkCollRects(erect, dumber.getCollRect()): dumber.killPlayer()
	for enemyV in enemyArrV:
		var erect = enemyV.getCollRect()
		if checkCollRects(erect, smarto.getCollRect()): smarto.killPlayer()
		if checkCollRects(erect, dumber.getCollRect()): dumber.killPlayer()
	for enemyD in enemyArrD:
		var erect = enemyD.getCollRect()
		if checkCollRects(erect, smarto.getCollRect()): smarto.killPlayer()
		if checkCollRects(erect, dumber.getCollRect()): dumber.killPlayer()
	for enemyS in enemyArrS:
		var erect = enemyS.getCollRect()
		if checkCollRects(erect, smarto.getCollRect()): smarto.killPlayer()
		if checkCollRects(erect, dumber.getCollRect()): dumber.killPlayer()
	for door in doorArr:
		var erect = door.getCollRect()
		if checkCollRects(erect, smarto.getCollRect()): smarto.killPlayer()
		if checkCollRects(erect, dumber.getCollRect()): dumber.killPlayer()
	for lever in leverArr:
		var erect = lever.getCollRect()
		var stouch = checkCollRects(erect, smarto.getCollRect())
		var dtouch = checkCollRects(erect, dumber.getCollRect())
		if stouch or dtouch:
			if not lever.touch:
				moveLever()
				lever.touch = true
		else:
			lever.touch = false
		
			
func checkCollRects(r1, r2):
	if r1 == null or r2 == null: return false
	if r1.end.x < r2.position.x: return false
	if r2.end.x < r1.position.x: return false
	if r1.end.y < r2.position.y: return false
	if r2.end.y < r1.position.y: return false
	return true

func moveLever():
	for lever in leverArr:
		lever.toggle()
	for door in doorArr:
		door.toggle()
		
const OTYPE_ENEMYH = 1
const OTYPE_ENEMYV = 2
const OTYPE_ENEMYD = 3
const OTYPE_SPIKES = 4
const OTYPE_DOOR   = 5
const OTYPE_LEVER  = 6
const OTYPE_CONV   = 7


func export_level():
	var smarto = $frame/smarto
	var dumber = $frame/dumber
	
	var objcount = 0
	objcount += enemyArrH.size()
	objcount += enemyArrV.size()
	objcount += enemyArrD.size()
	objcount += enemyArrS.size()
	objcount += leverArr.size()
	objcount += doorArr.size()
	objcount += convArr.size()
	
	var s = ''
	s += '    %d, // smarto_ini_cx\n' % int(smarto.position.x / 8)
	s += '    %d, // smarto_ini_cy\n' % int(smarto.position.y / 8)
	s += '    %d, // smarto_ini_lookleft\n' % (1 if smarto.lookleft else 0)
	s += '    %d, // dumber_ini_cx\n' % int(dumber.position.x / 8)
	s += '    %d, // dumber_ini_cy\n' % int(dumber.position.y / 8)
	s += '    %d, // dumber_ini_lookleft\n' % (1 if dumber.lookleft else 0)
	s += '    %d, // object_count\n' % objcount
	s += '    {\n'
	
	for enemyH in enemyArrH:
		var typespeed = enemyH.getAnimIndex() << 4 | int(enemyH.advmask) & 0x0F
		s += '        { // BEGIN OBJECT\n'
		s += '            %d, // OTYPE_ENEMYH\n' % OTYPE_ENEMYH
		s += '            %d, // ini_cx\n' % int(enemyH.position.x / 8)
		s += '            %d, // ini_cy\n' % int(enemyH.position.y / 8)
		s += '            %d, // type|speed (extra1)\n' % typespeed
		s += '            %d, // xpre (extra2)\n' % int(enemyH.xpre)
		s += '            %d, // xpost (extra3)\n' % int(enemyH.xpost)
		s += '        }, // END OBJECT\n'
		
	for enemyV in enemyArrV:
		var typespeed = enemyV.getAnimIndex() << 4 | int(enemyV.advmask) & 0x0F
		s += '        { // BEGIN OBJECT\n'
		s += '            %d, // OTYPE_ENEMYV\n' % OTYPE_ENEMYV
		s += '            %d, // ini_cx\n' % int(enemyV.position.x / 8)
		s += '            %d, // ini_cy\n' % int(enemyV.position.y / 8)
		s += '            %d, // type|speed (extra1)\n' % typespeed
		s += '            %d, // xpre (extra2)\n' % int(enemyV.ypre)
		s += '            %d, // xpost (extra3)\n' % int(enemyV.ypost)
		s += '        }, // END OBJECT\n'
		
	for enemyD in enemyArrD:
		var type = enemyD.getAnimIndex()
		s += '        { // BEGIN OBJECT\n'
		s += '            %d, // OTYPE_ENEMYD\n' % OTYPE_ENEMYD
		s += '            %d, // ini_cx\n' % int(enemyD.position.x / 8)
		s += '            %d, // ini_cy\n' % int(enemyD.position.y / 8)
		s += '            %d, // type (extra1)\n' % type
		s += '            %d, // advx (extra2)\n' % int(enemyD.advx)
		s += '            %d, // advy (extra3)\n' % int(enemyD.advy)
		s += '        }, // END OBJECT\n'
		
	for enemyS in enemyArrS:
		s += '        { // BEGIN OBJECT\n'
		s += '            %d, // OTYPE_SPIKES\n' % OTYPE_SPIKES
		s += '            %d, // ini_cx\n' % int(enemyS.position.x / 8)
		s += '            %d, // ini_cy\n' % int(enemyS.position.y / 8)
		s += '            %d, // unused (extra1)\n' % 0
		s += '            %d, // unused (extra2)\n' % 0
		s += '            %d, // unused (extra3)\n' % 0
		s += '        }, // END OBJECT\n'
		
	for door in doorArr:
		s += '        { // BEGIN OBJECT\n'
		s += '            %d, // OTYPE_DOOR\n' % OTYPE_DOOR
		s += '            %d, // ini_cx\n' % int(door.position.x / 8)
		s += '            %d, // ini_cy\n' % int(door.position.y / 8)
		s += '            %d, // closed (extra1)\n' % (1 if door.closed else 0)
		s += '            %d, // unused (extra2)\n' % 0
		s += '            %d, // unused (extra3)\n' % 0
		s += '        }, // END OBJECT\n'
		
	for lever in leverArr:
		s += '        { // BEGIN OBJECT\n'
		s += '            %d, // OTYPE_LEVER\n' % OTYPE_LEVER
		s += '            %d, // ini_cx\n' % int(lever.position.x / 8)
		s += '            %d, // ini_cy\n' % int(lever.position.y / 8)
		s += '            %d, // closed (extra1)\n' % (1 if lever.closed else 0)
		s += '            %d, // isLeft (extra2)\n' % (1 if lever.isLeft() else 0)
		s += '            %d, // unused (extra3)\n' % 0
		s += '        }, // END OBJECT\n'
		
	for conv in convArr:
		s += '        { // BEGIN OBJECT\n'
		s += '            %d, // OTYPE_CONV\n' % OTYPE_CONV
		s += '            %d, // ini_cx1\n' % int(conv.x1)
		s += '            %d, // ini_cy\n' % int(conv.y)
		s += '            %d, // ini_cx2 (extra1)\n' % int(conv.x2)
		s += '            %d, // isLeft (extra2)\n' % (1 if conv.left else 0)
		s += '            %d, // unused (extra3)\n' % 0
		s += '        }, // END OBJECT\n'
		
	s += '    }\n'

	var f = File.new()
	f.open(get_levdata_fn(), File.WRITE)
	f.store_string(s)
	f.close()
			
################################################################################
################################################################################
################################################################################
################################################################################

func onDeadPlayerExited():
	if deadPlayerExited: return
	deadPlayerExited = true
	G.livesLost += 1
	$FadeRect.startFadeOut()

func onFadeOutFinished():
	if exiting_to_menu:
		G.exitToMenu()
	elif not levelExited:
		get_tree().reload_current_scene()
	elif not G.isLevSelMode:
		G.nextLevnum = 1 + levnum
		if G.nextLevnum == 21 or G.nextLevnum == 41:
			get_tree().change_scene("res://scenes/Final1.tscn")
		else:
			get_tree().change_scene('res://scenes/Inter.tscn')
			
	else:
		get_tree().change_scene('res://scenes/LevelSelect.tscn')
		

################################################################################
################################################################################

func exit_level():
#	$frame/smarto.killPlayer()
#	$frame/dumber.killPlayer()
	levelExited = true
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
