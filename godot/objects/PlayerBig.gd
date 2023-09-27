extends Node2D


# Declare member variables here. Examples:
# var a = 2
# var b = "text"

export var anim_phase = 0

const HAT_LEN = 32
const HA_PHASE = 11
const HA_MASK =  0x1F

var head_anim_table = [
	0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
]

var head_anim_frame = 0

# Called when the node enters the scene tree for the first time.
func _ready():
	head_anim_frame = 0
	
func _physics_process(delta):
	$head.position.y = head_anim_table[(head_anim_frame + anim_phase) & HA_MASK]
	head_anim_frame += 1;
	
# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
