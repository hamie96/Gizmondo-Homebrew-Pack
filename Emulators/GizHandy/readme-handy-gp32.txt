GPLynx - Handy port 2 v0.04 - 2003-2005 by Per Sterner/TuskenRaider

Thank's to Keith wilkins and chris :-)

v0.04:
I was quite busy and further more frustrated about the flickering isues. The savegame problems with the previous versions are solved.
But there are still bugs:
- HEAVY flickering, I hope this problem will be solved ;-/

But some games work in fullspeed (not their intros):
- Chip's Challenge
- Gates of Zendocon
- Klax
and perhaps more :)

- Slime World - Todd's Adventure runs quite nice

With these settings:
- 156mhz
- Stretched
- Frameskip 2
- Sound set to "on with sync"

I compiled this version with and without doublebuffering. But it doesn't make any huge difference with the flickering isue :-/.

--- Previous versions ---

v0.03f:
- bugfixes
- new background picture ;)

v0.03e:
- bugfixes
- Screenshots for savegames
- removed unneeded code, the fxe is smaller
- renamed the fxe to GPLynx
- new background picture

v0.03d:
- Startup much faster

v0.03c:
- fixed problems with the new gui
- ZIP-File parsing is faster now
- Added quicksave/quickload (stores one savegame in the memory of the GP32)
- Sound!!

In the gui:
- Use left/right for view the roms or the savegames
- Use The left button to jump to the previous letter in the game list,
  the right button to jump to the next letter in the game list
- Up/down for scrolling up and down
- press the A-button to start the game

v0.03b:
- Savegame support!
- Gamma correction added
- New Loading GUI
- BLU+ support (?) copy the file "blu.txt" in the directory gpmm\handygp

After starting the emu for the first time, you should use "Store db"
to speed up the startup time of the emu.

Everytime you start a new game, some screenshots are taken. With
"Store db" these pictures are stored.

The saved are games can be loaded with "restart". A directory called
"saves" must be in the handygp directory.

v0.03:
- faster!! (At 156Mhz many games are nearly with fullspeed!)
- sound is disabled

You can edit the file "gpmm\handygp\autorun.txt" and edit some lines.
For example:
With the line "emu/handy/CpuClockSpeed=5" the CpuClockSpeed will be 156Mhz from the beginning

v0.02:
- tweaked a bit
- sound can be activated (clicks/stutters)
- CpuClockSpeed 40mhz - 200mhz

Start the config:
- press the 'Select' button and goto 'config'
- In the game press the 'Start' button and goto 'config'

v0.01c:

I don't think the lynx emu will ever run fluent on the gp32,
but perhaps some games :)

Chip' Challenge runs 40%-50% faster. This is nearly nice :]
Slimeworld also works, other ones I've not tried (perhaps I've
inserted some bugs :o) )

Some words:
- If you want a background picture you can replace logo.png, but why?! :) [no picture at all will work fine too]
- With the Left fire button pressed you can cycle throught different background colors (if you like)
- I think the DrawMode is okay, but you can activate differnet ones throught right fire button (or
  in the config-menu)
  But only the modes 'Nothing';-) , "Interlaced XY", "Interlaced Y" are improved a bit.. The other ones are slowER
(you need to hold the left/right fire button for a half second for the last to options)
- To restart the gamepark, press start inside the game, then 'exit', then one key-click and finally select 'quit'
- in the config you can set 'nSkipFrameValue' to 1, will improve the speed a bit

& more words:
- ZipFiles doesn't work yet, it works under 'windows', must be some BIG_ENDIAN issues? I included unzip.h from the zlib.. (png works fine)
- I have included the gxb-file to, so you can upload it directly :o)

The changes:

I tried to optimize some things in the code, but only one had a little speed up :)
Further changes:
The grafix are now directly written in the mikie.h & the Loop for processing the emu
is nearly never left. So the extern Loop is only reached 20 times or so a second.. this
last thing brought 5-10%, perhaps caching&function calls ? :)

---- original readme.txt ----

Handy was written by Keith Wilkins
http://homepage.ntlworld.com/dystopia/

GP32 and Dreamcast ports by Christian Nowak
http://chn.roarvgm.com/

The ports run very slow on the GP32 as well as the Dreamcast, so be 
patient when testing them :) To compile for the GP32, use devkitadv 
and the addon which is available on my website. You need to change 
the file gp32.mk so that it links the emulator with -lstdc++, such as:

GPLIBS=-lgpsdk -lgpgraphic -lgpmem -lgpos -lgpstdlib -lgpstdio -lgpsound -lgpfont -lgpg_ex01 -lstdc++

Have a look or two at dcmain.cpp and gpmain.cpp as a starting point.

Have fun.

/chris
