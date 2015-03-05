mame4all gizmondo 0.3.0
-----------------------

This is a Gizmondo port of Chui's MAME4ALL port of Franxis' MAME GP2X 
port which is a port of MAME 0.34 by Nicola Salmoria.  The MAME 
license is at the end of the readme.txt document included with this release.

This port is maintained by critical (criticalhippo@gmail.com).  

This release has sound that isn't too bad, thanks to sweetlilmre's updated SDL libs.

It also includes all of the drivers that are supported on the GP2X (Neo Geo, etc.).

The issue with having to centre the dpad before going in a new direction is now fixed.

Neo Geo support is restricted somewhat by the available memory... don't expect to play
Metal Slug and that sort of thing.

This port uses the SDL libraries ported to the Gizmondo by sweetlilmre.

Installation
------------

The roms directory should be placed on your SD card like this:

SD Card Root 
  !
  !--- mame4all
          !
          !----- roms

It's best to put mame4all.exe and the supplied dlls 
into the mame4all directory and use firefly's launcher.

Put your MAME 0.34 roms (zipped, as for any other MAME) in the mame4all\roms directory.

Controls
--------

At the main rom list menu, use up and down to select a game, or left or right trigger to 
page up/down the rom list, then RWND to move to the options screen.

Press PLAY to exit back to the rom list.

Press HOME at the main rom list to quit mame4all.

At the options screen, press RWND to play the game using the currently-selected options.

Whilst playing, return to the rom list menu by pressing HOME.

Whilst playing, press * to add coins.
Whilst playing, press Speaker for player 1 START.
Whilst playing, RWND is button A, PLAY is button B and FFWD is button C.


Issues
------

Sound is still non-optimal.
Buttons might still not be the best.

Release history
---------------

20060609 - version 0.1.0	Initial release for Gizmondo from mame4all alpha 1 source.
20060613 - version 0.2.0	Fixed up buttons.
20060625 - version 0.3.0	Fixed issue with dpad control.  Added all drivers supported in the GP2X version.

The rest
--------

The original code I ported it from is available here:

http://chui.dcemu.co.uk/mame4all.html

My full source is available on request to the email address below:

Email: criticalhippo@gmail.com
