tool

extends Node2D

export var closed = true setget set_closed
export var flip = false setget set_flip
var touch = false

func set_closed(c):
	closed = c
	updateClosedState()

func set_flip(f):
	flip = f
	$anisprite.flip_h = flip

var G

func _ready():
	if Engine.editor_hint: return
	G = get_node('/root/Global')
#	updateClosedState()
		
func _physics_process(delta):
	if Engine.editor_hint: return
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
