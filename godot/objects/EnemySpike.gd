extends Node2D

onready var G = get_node('/root/Global')

var active = false
var framesOn = 50
var framesOff = 200
var frameCtr = 0

# Called when the node enters the scene tree for the first time.
func _ready():
	$anisprite.animation = 'leave'
	$anisprite.play()
	active = false
	frameCtr = 0

func getType():
	return 'enemys'

func _physics_process(delta):
	# pause
	# $anisprite.playing = not G.paused   # has side effects
	if G.paused: return
		
	if !active:
		if frameCtr >= framesOff:
			$anisprite.animation = 'enter'
			$anisprite.play()
			active = true
			frameCtr = 0
		else:
			frameCtr += 1
	else:
		if frameCtr >= framesOn:
			$anisprite.animation = 'leave'
			$anisprite.play()
			active = false
			frameCtr = 0
		else:
			frameCtr += 1

##################################################################################################
##################################################################################################

# collision rectangle
func getCollRect():
	if not active: return null
	var r = Rect2()
	r.position = position + Vector2( 1,  1)
	r.end      = position + Vector2(14, 14)
	return r

