extends Node2D

signal fadeInFinished
signal fadeOutFinished

func startFadeIn():
	fadeState = FADEIN
	
func startFadeOut():
	fadeState = FADEOUT

enum { FADEIDLE, FADEIN, FADEOUT}
var fadeState = FADEIDLE
var fadeDur = 0.25

onready var faderect = $FadeRect

func _ready():
#	print('FadeRect._ready')
	faderect.visible = true
	fadeState = FADEIN

func _process(delta):
	processFade(delta)

func processFade(delta):
	match fadeState:
		FADEIN : processFadeIn (delta)
		FADEOUT: processFadeOut(delta)
	pass
	
func processFadeIn(delta):
	if not faderect.visible:
		faderect.visible = true
		faderect.color.a = 1.0
		return
	
	var a = faderect.color.a
	a -= delta / fadeDur
	if a > 0.0:
		faderect.color.a = a
	else:
		faderect.color.a = 0.0
		fadeState = FADEIDLE
		faderect.visible = false
		emit_signal('fadeInFinished')
	
func processFadeOut(delta):
	if not faderect.visible:
		faderect.visible = true
		faderect.color.a = 0.0
		return
	
	var a = faderect.color.a
	a += delta / fadeDur
	if a < 1.0:
		faderect.color.a = a
	else:
		faderect.color.a = 1.0
		fadeState = FADEIDLE
		emit_signal('fadeOutFinished')
