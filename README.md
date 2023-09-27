# Bean Brothers remake
Bean Brothers is a 2023 platform game for ZX Spectrum Next / N-GO. It is based on Bean Brothers for Spectrum 48K/128K, created in 2018 by Dave Hughes. I contacted him and he kindly gave me permission for this remake.

This game runs on:
- Spectrum Next
- Windows (emulated with CSpect)
- Windows (native Godot executable)
- OSX (native Godot executable)
- Linux (native Godot executable)
- Browsers (modern browsers with HTML5)

For more info and downloads, see the [game page at itch.io](https://davidprograma.itch.io/bean-brothers).

There is an [HTML5 version you can try](https://dcrespo3d.github.io/BeanBrosRemake/). It may be outdated, the official version is that of the game page.

The game has been developed using:

- [Godot Engine](https://godotengine.org/) __(3.5.1)__ for prototyping.
- The [Z88DK C compiler](https://z88dk.org/site/).
- [Aseprite](https://www.aseprite.org/) for pixel art editing.
- [Arkos Tracker 2](https://www.julien-nevo.com/arkostracker/) for music/chiptunes.
- Custom [Python 3](https://www.python.org/) scripts for levels and art conversion.

There are two code trees:
- The Godot tree, with the main art, and levels and game logic in GDScript.
- The Next tree, with C code and python scripts for conversion into a format usable by the Next, to be compiled using the Z88DK.

You can use either tree or both:
- You can use the open the godot project usign Godot Editor, tweak the levels, run the project, and even generate an executable.
- You can compile the Next tree using z88dk, all needed resources are already converted into the `res` subdirectory. Just compile and run (see below).
- You could tweak levels using Godot Editor (which generates level*.txt files each time a level is run from the editor), convert the resources using the provided scripts, and compile the Next tree.

## Godot tree

You need Godot 3.5.1 for this project. You should scan the folder containing the project, and open it from the Godot Project Manager.

### Artists:
You should be able to directly modify files at `godot/art` and see your changes running the project (F5 key on Windows).

### Level designers:
On Godot FileSystem, open folder res://levels and you will find Level01.tscn thru Level40.tscn, you should be able to tweak them.

Open any level and you will be able to modify it, moving enemies and things. When moving enemies, beware: when a enemy has 'editable children', be sure to move the parent object, not the child sprite. Doing wrong will result in a collision inconsistency (the enemy is displayed in some place but its kill rectangle is in some other place). Drag it only from the '+' symbol at its top-left corner.

You can also modify floors and walls using the tilemap editor. But there are also pitfalls: Only base styles are usable (gray colored stone, light blue platforms). Using others will fail collision, do not use tiles with index > 30 (except 47 and 86).

Also conveyors have a pitfall: you should use only the first set (7-10) for right conveyor, and the third (15-18) for left conveyor.

All game objects should hang from `/frame` node at hyerarchy. You can add new objects right clicking `/frame` and selecting `Instance Child Scene`, and then choosing any preset object (scene in Godot terminology) at `/objects/****`.

If something does not work as expected, see included levels for reference.

### Programmers

Feel free to study the code and modify it. The player logic is not as clean as I would have liked, but works well. Maybe begin by tweaking the jump table at Player.gd...

Please note that a lot of GDScript code (specially player logic) has a 1:1 C equivalent on the Next tree. Godot collision system is not used, I have written a simple logic in GDScript with the idea of being able to translate it into C.

If you want to make changes to the Next version, it would be much easier to prototype your changes in GDScript and then translate them into C.

## Next tree

For compiling, you will need the z88dk compiler, I have used version z88dk-win32-20220704, a bit old but it works for me.

I have used two ways of compiling:
- Pure Windows using .bat files
- WSL on Windows using Makefile (which should extrapolate to Linux and OSX)

### Pure windows

Unzip z88dk at `C:\z88dk`.
Unzip CSpect at `C:\z88dk\cspect`

Open a command window.

Execute `z88dk_set_path.bat`

Execute `compile.bat`

Execute `run.bat`

If everything is OK, you should have generated a .nex file and it should be running on the CSpect emulator.

If you used other paths than indicated, you should modify `z88dk_set_path` and `run.bat` accordingly.

Everytime you compile, all files will be compiled, and it can take some time.

### WSL on Windows / Linux / OSX

First of all, install WSL1 and Ubuntu. WSL2 is sluggish on windows partitions. Beware, WSL2 is the default on Windows11.

Then, install z88dk, this [Youtube Video by NCOT Technology](https://www.youtube.com/watch?v=R-JD8CQ5zTc) was all I needed.

Put your freshly compiled z88dk in `$HOME/z88dk` and the downloaded CSPect in `$HOME/z88dk/cspect`

At this point, `make` and `m4` shoud be installed.

Then, just run `make` to compile the project and `make run` to compile and run in the emulator.

Next times you compile, make will do his work and compile only what's changed. But beware: header (.h) dependencies are not present, so if you change a header file, you will need to `make clean` before `make`.

## Conversion scripts

For conversion, you should enter the `next/scripts` subdirectory.
The conversion step has two parts:

- Copy level*.txt from godot tree using either `bring.bat` or `bring.sh`
- Call all conversion scripts with `python all_convert.py` or `python3 all_convert.py` depending on your installation (did I say you need Python 3 for this?)

The conversion scripts spit a lot of information, some of it is needed for manually changing things in code. Not everything is automated.

But if you modified levels in Godot, you should be able to compile and see the results on Next.

## Epilog
This game has been programmed in 3 months, from scratch. I have not used (nor requested) code, music or graphics from the original author (but I have studied the original game A LOT).

Not everything is perfect, but it can serve as a starting point for people into game development for Next or other systems. At least I hope so.