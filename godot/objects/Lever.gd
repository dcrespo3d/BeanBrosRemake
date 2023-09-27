extends Node2D

onready var G = get_node('/root/Global')

export var closed = true
var touch = false

# Called when the node enters the scene tree for the first time.
func _ready():
#	updateClosedState()
	pass
		
func _physics_process(delta):
	# pause
	# $anisprite.playing = not G.paused   # has side effects
	if G.paused: return
	
func getType():
	return 'lever'
	
func isLeft():
	return $anisprite.flip_h

func toggle():
	closed = not closed
	updateClosedState()

func updateClosedState():
	if closed: $anisprite.animation = 'clos'
	else:       $anisprite.animation = 'open'
	$anisprite.play()

##################################################################################################
##################################################################################################

# collision rectangle
func getCollRect():
	var r = Rect2()
	r.position = position + Vector2( 1,  1)
	r.end      = position + Vector2(14, 14)
	return r
