GizRACE 0.1.0
---------------

Gizmondo port by critical of RACE!, originally written by Judge,
ported to GP2x and GP32 by Flavor and Thor, who both also enhanced it.

Full credits are in the readme.txt that sits alongside this file.

It emulates Neo Geo Pocket (NGP) and Neo Geo Pocket Color (NGPC) handhelds.

The gx.dll in this distribution is the same one provided by thereaper, which
is based on Thierry Tremblay's GAPI emulator.

The sdl libraries are courtesy of sweetlilmre.

The zlib library is from Jean-Loup Gailly and Mark Adler.


This software is licensed under the GNU Generic Public License(GPL).
Source is available on request to the contact address below, or from sourceforge
at:

http://sourceforge.net/projects/koyote-land

Contact: criticalhippo@gmail.com

Installation
------------

Put GizRACE.exe in the root of your SD card, along with the included gx.dll,
SDL.dll, SDL_image.dll, SDL_mixer.dll and zlib.dll.

Put the other files in a directory called "GizRACE" in the root of your SD card.


Usage
-----

ROMs must be in a directory called:

GizRACE

in the root of your SD Card.  They must have the suffix ".ngp" or ".npc".

The filenames MAY NOT CONTAIN SPACES.

Buttons
-------

The joypad does what you'd expect.  The other buttons are:

PLAY	A
FFWD	B
STOP	Start
HOME	Quit

Whilst in the file selector, the STOP key exits back to the Gizmondo system.

Up and Down on the joypad in the main menu select the ROM, PLAY starts it.

TODO
----

Scaling.
Proper menu.
Better controls.
Allow spaces in filenames.