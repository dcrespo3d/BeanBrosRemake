extends Node2D


# Declare member variables here. Examples:
# var a = 2
# var b = "text"

var songs = []
var currentSong = null

# Called when the node enters the scene tree for the first time.
func _ready():
	for song in $songs.get_children():
		songs.append(song)
	print('BGMusic: song count = ', len(songs))

func playLevelSong(levnum):
	var songidx = (levnum - 1) % len(songs)
	var newSong = songs[songidx]
	if newSong == currentSong:
		return
		
	if currentSong != null:
		currentSong.stop()
		
	currentSong = newSong
	currentSong.play()
