extends Node2D

onready var G = get_node('/root/Global')

export var advmask = 1
export var ypre = -8
export var ypost = 8

# collision tiles
var tcol = null

var anims = ['enemy1', 'enemy2', 'enemy3']

# Called when the node enters the scene tree for the first time.
func _ready():
	if advmask > 0:
		fper = advmask
		deltay = 1
	else:
		fper = -advmask
		deltay = -1
	fctr = fper
	ymin = position.y + 8 * ypre
	ymax = position.y + 8 * ypost
	$anisprite.play()

var ymin
var ymax
var deltay
var fper
var fctr

func getType():
	return 'enemyv'
	
func getAnimIndex():
	for i in range(anims.size()):
		if anims[i] == $anisprite.animation:
			return i
	return 0

func _physics_process(delta):
	# pause
	$anisprite.playing = not G.paused
	if G.paused: return
	
	# slow motion
	if G.mustSkipDueToSlowMotion():
		$ColorRect.visible = true
		return
	else:
		$ColorRect.visible = false

	fctr -= 1
	if fctr > 0: return
	fctr = fper
		
	position.y += deltay
	
	if position.y >= ymax:
		deltay = -1
	if position.y <= ymin:
		deltay = 1
		
##################################################################################################
##################################################################################################

# collision rectangle
func getCollRect():
	var r = Rect2()
	r.position = position + Vector2( 1,  1)
	r.end      = position + Vector2(14, 14)
	return r


