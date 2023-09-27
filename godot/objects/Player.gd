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

export var active = false
export var lookleft = false
export var dbgname = 'player'

# collision tiles
var tcol = null

# graphic tiles (for crumb tiles)
var tgra = null

# other player
var other = null

# level object
var level = null

# inhibition for info screen
var inhibited = false

# Called when the node enters the scene tree for the first time.
func _ready():
	$AnimatedSprite.animation = 'idle'
	$AnimatedSprite.play()
	cx = 0
	cy = 0
	dx = int(position.x)
	dy = int(position.y)
	adjustX()
	adjustY()
	updatePos()
	printLogicPos()
	
	pass # Replace with function body.

##################################################################################################
##################################################################################################

# logic position: made from tile coord [i,j] and deltas
var cx = 0
var cy = 0
var dx = 0
var dy = 0

# logic position: made from tile coord [i,j] and deltas
func adjustX():
	while dx > G.SDXR:
		dx -= G.TW
		cx += 1
	while dx < G.SDXL:
		dx += G.TW
		cx -= 1
func adjustY():
	while dy > G.SDYR:
		dy -= G.TH
		cy += 1
	while dy < G.SDYL:
		dy += G.TH
		cy -= 1
		
func updatePos():
	position.x = G.TW * cx + dx
	position.y = G.TH * cy + dy
		
func printLogicPos():
	print("%s: coord = %2d, %2d - disp = %2d, %2d" % [dbgname, cx, cy, dx, dy])

##################################################################################################
##################################################################################################

var actionLeft  = false
var actionRight = false
var actionUp    = false
var actionDown  = false
var swap        = false
	
# read input controls
func readInput():
	if inhibited: return
	actionLeft  = Input.is_action_pressed('play_left')  or Input.is_action_pressed('play_left2')
	actionRight = Input.is_action_pressed('play_right') or Input.is_action_pressed('play_right2')
	actionUp    = Input.is_action_pressed('play_up')
	actionDown  = Input.is_action_pressed('play_down')
	swap        = Input.is_action_just_pressed('play_down')

##################################################################################################
##################################################################################################

var dead = false
var airborne = false
var jumptable_old = [-3,-3,-3,-3,-2,-2,-2,-1,-1,-1,-1,-1,-1,-1,-1,0,0,0,0,0,1,1,1,1,1,1,1,1,2,2,2,3,3,3,3,4]
var jumptable = [
	-3,-3,-3,
	-2,-2,-2,-2,
	-1,-1,-1,-1,-1,-1,-1,
	-1,-1,
	0,0,0,0,0,0,0,0,
	1,1,
	1,1,1,1,1,1,1,
	2,2,2,2,
	3,3,3,4]
const JUMPIDX = 0
const FALLIDX_OLD = 20
const FALLIDX = 24
var jumpidx = 0
var vx
var vy

const CRUMB_CTR_MAX = 3
var crumb_ctr = 0

func _physics_process(delta):
	if inhibited:
		$AnimatedSprite.flip_h = lookleft
		return
	
	# pause
	$AnimatedSprite.playing = not G.paused
	if G.paused:
		return
	
	# slow motion
	if G.mustSkipDueToSlowMotion():
		$ColorRect.visible = true
		return
	else:
		$ColorRect.visible = false
	
	if G.DUSE_DEBUG_KEYS and active and Input.is_action_pressed("debug_kill"):
		killPlayer()
		
	if dead:
		processDead()
		return
		
	if into_nasty():
		killPlayer()
		return
	
	vx = 0
	vy = 0

	readInput()
	if active:
		if actionLeft and not actionRight:
			$AnimatedSprite.animation = 'walk'
			lookleft = true
			vx -= 1
		elif actionRight and not actionLeft:
			$AnimatedSprite.animation = 'walk'
			lookleft = false
			vx += 1
		else:
			$AnimatedSprite.animation = 'idle'
			
		if not airborne and actionUp and not up_blocked():
			airborne = true
			jumpidx = JUMPIDX
	else:
		$AnimatedSprite.animation = 'asleep'
	
	$AnimatedSprite.flip_h = lookleft
	
	if on_conveyor() and not (vx != 0 and not dx & 1):
		if on_conveyor_left():
			vx -= 1
		else:
			vx += 1

	while vx < 0 and not left_blocked():
		vx += 1
		dx -= 1
		adjustX()
	while vx > 0 and not right_blocked():
		vx -= 1
		dx += 1		
		adjustX()
	
	if airborne:
		if jumpidx < jumptable.size():
			vy = jumptable[jumpidx]
		else:
			vy = jumptable[jumptable.size() - 1]
		jumpidx += 1
		dy += vy
	else:
		jumpidx = FALLIDX
	
	if swap:
		active = !active

	adjustY()
	
	if vy >= 0 and is_on_other() and not down_blocked():
		airborne = other.airborne
		dy = other.dy
		cy = other.cy - 2
		jumpidx = other.jumpidx
	if vy < 0 and other.is_on_other() and not other.up_blocked():
		other.dy = dy
		other.cy = cy - 2
		other.jumpidx = jumpidx
#		other.airborne = airborne
		
	if airborne:
		if vy > 0:
			if into_floor() and not into_ceil():
				airborne = false
				cy -= 1
				dy = 0
			elif dy < 4 and down_blocked():
				airborne = false
				dy = 0
		elif vy < 0 and into_ceil():
			dy = 8   # commenting this makes it more benevolous
			jumpidx = FALLIDX
	else:
		if not down_blocked() and not is_on_other():
#			print('airborne due to not blocked')
			airborne = true
			
	if self.into_exit() and other.into_exit():
		level.exit_level()
			
	adjustY()
	updatePos()
	
	if airborne:
		crumb_ctr = 0
	else:
		process_on_crumb_floor()
			
	if G.DUSE_DEBUG_KEYS and Input.is_action_pressed("debug_slomo"):
		print('%s: air=%d, cx=%2d,cy=%2d, dx=%1d,dy=%1d, px=%3d,py=%3d, vx=%2d,vy=%2d' % [dbgname, 1 if airborne else 0, cx, cy, dx, dy, position.x, position.y, vx, vy])
	
func down_blocked():
	var A = tcol.get_cell(cx, cy+2)
	var B = tcol.get_cell(cx+1, cy+2)
	var C = tcol.get_cell(cx+2, cy+2)
#	if active: print(A, ' ', B, ' ', C, ' ', A&G.TMD)
	if dx == 0:
		if A & G.TMD or B & G.TMD: return true
	else:
		if A & G.TMD or B & G.TMD or C & G.TMD: return true
	return false
	
func is_on_other():
	var o = other.position
	var p = position
	var x = p.x - o.x
	var y = p.y - o.y
#	print(y)
	if x < -12 or x >  12: return false
	if y < -18 or y > -14: return false
	return true
	
func into_ceil():
	var A = tcol.get_cell(cx, cy)
	var B = tcol.get_cell(cx+1, cy)
	var C = tcol.get_cell(cx+2, cy)
	if dx == 0:
		if A & G.TMA or B & G.TMA: return true
	else:
		if A & G.TMA or B & G.TMA or C & G.TMA: return true
	return false
	
func into_floor():
	var A = tcol.get_cell(cx, cy+1)
	var B = tcol.get_cell(cx+1, cy+1)
	var C = tcol.get_cell(cx+2, cy+1)
	if dx == 0:
		if A & G.TMA or B & G.TMA: return true
	else:
		if A & G.TMA or B & G.TMA or C & G.TMA: return true
	return false
	
func up_blocked():
	var A = tcol.get_cell(cx, cy-1)
	var B = tcol.get_cell(cx+1, cy-1)
	var C = tcol.get_cell(cx+2, cy-1)
	if dx == 0:
		if A & G.TMA or B & G.TMA: return true
	else:
		if A & G.TMA or B & G.TMA or C & G.TMA: return true
	return false
	
func left_blocked():
	var mask = G.TMA | G.TMG
	var A = tcol.get_cell(cx-1, cy)
	var B = tcol.get_cell(cx-1, cy+1)
	var C = tcol.get_cell(cx-1, cy+2)
#	if active: print(A, ' ', B, ' ', C)
	if dx != 0: return false
	if dy == 0:
		if A & mask or B & mask: return true
	else:
		if A & mask or B & mask or C & mask: return true
	return false
	
func right_blocked():
	var mask = G.TMA | G.TMG
	var A = tcol.get_cell(cx+2, cy)
	var B = tcol.get_cell(cx+2, cy+1)
	var C = tcol.get_cell(cx+2, cy+2)
#	if active: print(A, ' ', B, ' ', C)
	if dx != 0: return false
	if dy == 0:
		if A & mask or B & mask: return true
	else:
		if A & mask or B & mask or C & mask: return true
	return false
	
func into_exit():
	var A = tcol.get_cell(cx  , cy  )
	var B = tcol.get_cell(cx+1, cy  )
	var D = tcol.get_cell(cx  , cy+1)
	var E = tcol.get_cell(cx+1, cy+1)
	if A & G.TMN or B & G.TME or D & G.TME or E & G.TME: return true
	if dx != 0:
		var C = tcol.get_cell(cx+2, cy  )
		var F = tcol.get_cell(cx+2, cy+1)
		if C & G.TME or F & G.TME: return true
	if dy != 0:
		var GG = tcol.get_cell(cx  , cy+2)
		var H = tcol.get_cell(cx+1, cy+2)
		if GG & G.TME or H & G.TME: return true
	if dx != 0 and dy != 0:
		var I = tcol.get_cell(cx+2, cy+2)
		if I & G.TME: return true
	return false
	
func on_conveyor():
	var A = tcol.get_cell(cx, cy+2)
	var B = tcol.get_cell(cx+1, cy+2)
	var C = tcol.get_cell(cx+2, cy+2)
#	if active: print(A, ' ', B, ' ', C)
	if dx == 0:
		if A & G.TMC or B & G.TMC: return true
	else:
		if A & G.TMC or B & G.TMC or C & G.TMC: return true
	return false
	
func on_conveyor_left():
	var A = tcol.get_cell(cx, cy+2)
	var B = tcol.get_cell(cx+1, cy+2)
	var C = tcol.get_cell(cx+2, cy+2)
#	if active: print(A, ' ', B, ' ', C)
	if dx == 0:
		if A & G.TML or B & G.TML: return true
	else:
		if A & G.TML or B & G.TML or C & G.TML: return true
	return false

func process_on_crumb_floor():
	if crumb_ctr > 0:
		crumb_ctr -= 1
		return
	crumb_ctr = CRUMB_CTR_MAX
	var A = tcol.get_cell(cx, cy+2)
	if A & G.TMR:
		var hl = tgra.get_cell(cx, cy+2)
		var h = hl & 0xF0
		var l = hl & 0x0F
		l -= 1
		tgra.set_cell(cx, cy+2, h|l)
		if l == 0:
			tcol.set_cell(cx, cy+2, 0)
			
	var B = tcol.get_cell(cx+1, cy+2)
	if B & G.TMR:
		var hl = tgra.get_cell(cx+1, cy+2)
		var h = hl & 0xF0
		var l = hl & 0x0F
		l -= 1
		tgra.set_cell(cx+1, cy+2, h|l)
		if l == 0:
			tcol.set_cell(cx+1, cy+2, 0)
	
	if dx == 0:
		return
		
	var C = tcol.get_cell(cx+2, cy+2)
	if C & G.TMR:
		var hl = tgra.get_cell(cx+2, cy+2)
		var h = hl & 0xF0
		var l = hl & 0x0F
		l -= 1
		tgra.set_cell(cx+2, cy+2, h|l)
		if l == 0:
			tcol.set_cell(cx+2, cy+2, 0)
	
func into_nasty():
	var A = tcol.get_cell(cx  , cy  )
	var B = tcol.get_cell(cx+1, cy  )
	var D = tcol.get_cell(cx  , cy+1)
	var E = tcol.get_cell(cx+1, cy+1)
	if A & G.TMN or B & G.TMN or D & G.TMN or E & G.TMN: return true
	if dx != 0:
		var C = tcol.get_cell(cx+2, cy  )
		var F = tcol.get_cell(cx+2, cy+1)
		if C & G.TMN or F & G.TMN: return true
	if dy != 0:
		var GG = tcol.get_cell(cx  , cy+2)
		var H = tcol.get_cell(cx+1, cy+2)
		if GG & G.TMN or H & G.TMN: return true
	if dx != 0 and dy != 0:
		var I = tcol.get_cell(cx+2, cy+2)
		if I & G.TMN: return true
	return false
##################################################################################################
##################################################################################################

# collision rectangle
func getCollRect():
	if dead : return null
	var r = Rect2()
	r.position = position + Vector2( 1,  1)
	r.end      = position + Vector2(14, 14)
	return r

func killPlayer():
	if dead : return
	$AnimatedSprite.animation = 'dead'
	dead = true
	if cy < 16:
		vy = -4 << 2
	else:
		vy = -6 << 2
	
func processDead():
	position.x += vx
	position.y += vy >> 2
	vy += 1
	if position.y > 270 and level != null:
		level.onDeadPlayerExited()

