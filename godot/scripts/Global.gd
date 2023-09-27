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

extends Node

###############################################################################
###############################################################################

const LEVEL_COUNT = 40

var RUNNING_IN_EDITOR
var DUSE_DEBUG_KEYS

# collision layer definitions
#const LPLATFORM = 0x01
#const LPLAYER   = 0x02
#const LENEMY    = 0x04
#const LSLABOT   = 0x08
#const LANYBOT   = 0x0E  # PLAYER | ENEMY | SLABOT
#const LLADDER   = 0x10
#const LENEMSTOP = 0x40

# tile dimensions
const TW = 8
const TH = 8

# tile count
const NX = 40
const NY = 32

# sprite deltas
const SDXL = 0
const SDXR = 7
const SDYL = 0
const SDYR = 7

# tile masks
const TMD = 1  # collision in down direction only
const TMA = 2  # collision in all directions
const TMC = 4  # conveyor flag
const TML = 8  # conveyor left flag
const TMG = 16 # door (g for gate, d is already used)
const TME = 32 # exit
const TMN = 64 # nasty (kills player)
const TMR = 128 # cRumb tile

var nextLevnum = 1

var paused = false

var isExtLevMode = false
var isLevSelMode = false
var livesLost = 0

var unlockedLevel = { 'ori' : 1, 'ext' : 21 }

var version_string = '1.0 RC2'
var version_visible = false

###############################################################################
###############################################################################

func _ready():
	print('BeanBros / Godot 3.5.1 - (c)2023 David Crespo [github.com/dcrespo3d]')
	inisz = OS.get_window_size()
	print('initial window size: ', inisz)
	RUNNING_IN_EDITOR = OS.has_feature('editor')
	DUSE_DEBUG_KEYS = RUNNING_IN_EDITOR
	loadUnlockedLevel()

###############################################################################
###############################################################################

func onEnterScreen():
	resizeWindow()
	paused = false
	_anykey = false
	_anybut = false

###############################################################################
###############################################################################

var inisz
var zoomFactor = 0

func resizeWindow():
	var wsz = OS.get_window_size()
	if wsz.x > inisz.x and wsz.y > inisz.y:
		return
	var ssz = OS.get_screen_size()
	print('screen size: ', ssz)
	
	var zcalc
	if zoomFactor == 0:
		var zx = ssz.x / inisz.x
		var zy = ssz.y / inisz.y
		zcalc = int(min(zx, zy))
		print('calculated zoom factor: ', zcalc)
	else:
		zcalc = zoomFactor
		print('hardwired zoom factor: ', zcalc)

	wsz = Vector2(zcalc * inisz.x, zcalc * inisz.y)
	print('scaled window size: ', wsz)

	OS.set_window_size(wsz)
	OS.set_window_position((ssz - wsz) * 0.5)
	
###############################################################################
###############################################################################

var slowValue = 30 # number of frames to skip during slow motion
var slowCount = 0
var slowFrame = slowCount
var slowSkip = false

func mustSkipDueToSlowMotion():
	if not DUSE_DEBUG_KEYS:
		return
	return slowSkip
		
func processSkipDueToSlowMotion():
	slowCount = slowValue if Input.is_action_pressed("debug_slomo") else 0
			
	if slowCount != 0:
		if slowFrame > 0:
			slowFrame -= 1
			slowSkip = true
			return
		else:
			slowFrame = slowCount
	slowSkip = false
	
func _physics_process(delta):
	processSkipDueToSlowMotion()
	
###############################################################################
###############################################################################

func _process(frame):
	if RUNNING_IN_EDITOR and Input.is_action_just_pressed("debug_exit"):
		get_tree().notification(MainLoop.NOTIFICATION_WM_QUIT_REQUEST)
		return
	if DUSE_DEBUG_KEYS and Input.is_action_just_pressed("debug_reload"):
		get_tree().reload_current_scene()
		
	if Input.is_action_just_pressed("toggle_fullscreen"):
		OS.window_fullscreen = !OS.window_fullscreen
	
###############################################################################
###############################################################################

func toNextLevel():
	if nextLevnum > LEVEL_COUNT: nextLevnum = 1
	var sceneName = 'res://levels/Level%02d.tscn' % nextLevnum
	print('changing to scene: ' + sceneName);
	get_tree().change_scene(sceneName)

###############################################################################
###############################################################################

func exitToMenu():
	if not self.isLevSelMode:
		get_tree().change_scene('res://scenes/MainMenu.tscn')
	else:
		get_tree().change_scene('res://scenes/LevelSelect.tscn')
		

###############################################################################
###############################################################################

var _anykey = false
var _anybut = false

func _input(event):
	if event is InputEventKey:
		if event.pressed:
			_anykey = true
	if event is InputEventJoypadButton:
		if event.pressed:
			_anybut = true

func isPressedAnyKeyOrButton():
	if _anykey or _anybut:
		_anykey = false
		_anybut = false
		return true
	return false
	
###############################################################################
###############################################################################

const savegame_fn = 'user://levels.save'

func loadUnlockedLevel():
	var savegame = File.new()
	if not savegame.file_exists(savegame_fn):
		return
	savegame.open(savegame_fn, File.READ)
	self.unlockedLevel = parse_json(savegame.get_line())
	print('loaded save game: ', self.unlockedLevel)
	savegame.close()

func saveUnlockedLevel():
	var savegame = File.new()
	savegame.open(savegame_fn, File.WRITE)
	savegame.store_line(to_json(self.unlockedLevel))
	savegame.close()	
	
