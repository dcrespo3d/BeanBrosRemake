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

# level for signaling exit/restart
var level

func _ready():
	self.visible = false
	inh_frame = inh_frame_count
	
var option

# frame counter for inhibiting too frequent pauses
var inh_frame = 0
const inh_frame_count = 120

	
func _physics_process(delta):
	if not G.paused:
		# NOT paused
		# if inh_frame > inh_frame_count and Input.is_action_just_pressed('play_pause'):
		if Input.is_action_just_pressed('play_pause'):
			G.paused = true
		inh_frame += 1
	else:
		# PAUSED
		if not self.visible:
			# NOT visible
			if Input.is_action_just_released('play_pause'):
				self.visible = true
				option = 0
				update_cursor()
		else:
			# VISIBLE
			if Input.is_action_just_pressed('play_pause') or Input.is_action_just_pressed("ui_accept"):
				G.paused = false
				self.visible = false
				if option == 1:
					level.restartLevelKillingPlayer()
				elif option == 2:
					level.exitToMenu()
				inh_frame = 0
				
			if Input.is_action_just_pressed("ui_up") or Input.is_action_just_pressed("ui_up2"):
				option -= 1
				if option < 0: option = 0
				update_cursor()
				
			if Input.is_action_just_pressed("ui_down") or Input.is_action_just_pressed("ui_down"):
				option += 1
				if option > 2: option = 0
				update_cursor()
			

				
func update_cursor():
	$cursor.rect_position.x = $label1.rect_position.x - 12
	$cursor.rect_position.y = $label1.rect_position.y + 24 * option
	

