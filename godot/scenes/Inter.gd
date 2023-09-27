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

var menuExited

const txt1a = "GET READY!"
const txt1b = "LEVEL COMPLETE!"

const txt2 = "NEXT LEVEL: []"

func _ready():
	G.onEnterScreen()
	$FadeRect.connect("fadeOutFinished", self, "onFadeOutFinished")
	menuExited = false
	$frame/label1.text = txt1a if G.nextLevnum == 1 else txt1b
	
	if not G.isExtLevMode:
		$frame/label2.text = txt2.replace('[]', str(G.nextLevnum))
	else:
		$frame/label2.text = txt2.replace('[]', 'X'+str(G.nextLevnum-20))
	
	if G.livesLost > 0:
		$frame/lives.text = String(G.livesLost)
	else:
		$frame/skull.hide()
		$frame/lives.hide()
	
func _process(delta):
	if !menuExited and G.isPressedAnyKeyOrButton():
		menuExited = true
		$FadeRect.startFadeOut()

func onFadeOutFinished():
	G.toNextLevel()
		

################################################################################
################################################################################

