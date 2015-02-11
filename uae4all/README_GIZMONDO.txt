uae4allgizmondo0.6.3
----------------

This is an Amiga emulator for the Gizmondo. This port is maintained by critical (criticalhippo@gmail.com).  
If you feel strangely compelled to donate, you may do so via paypal to: criticalhippo@gmail.com

This version shares the same codebase as uae4all gp2x 0.6.3, including the FAME/C core that the mighty
Chui provided.  Sound works now.

This is a port of Chui's uae4all code, using the SDL libraries ported to the Gizmondo by sweetlilmre.


Installation
------------

The data directory should be placed on your SD card like this:

SD Card Root 
  !
  !--- uae4all
          !
          !----- data

If using firefly's launcher, it's best to put uae4all.exe and the supplied dlls 
into the uae4all directory, otherwise put them in the root of your SD card.

Put your kick.rom file in the uae4all directory.

The kick.rom file should be a kickstart 1.3 image, and overdumped (512K, rather than 256K).
It is not legal to distribute the kickstart rom, hence it is not included with this release.

You may put your adf Amiga disk images wherever you like on the SD card.

Controls
--------

At the menu, use the FFWD button to select menu items.

Whilst playing, go to the menu by pressing RWND.

Whilst playing, press STOP to toggle between joystick and mouse mode.

Whilst playing, when in joystick mode, press the right shoulder button to bring up the
virtual keyboard.  Press FFWD to press a key.  Press the left should button to dismiss it.  

Whilst playing, when in joystick mode, press FFWD to press the fire button

Whilst playing, when in mouse mode, press the left shoulder button to click the left mouse button.
Whilst playing, when in mouse mode, press the right shoulder button to click the right mouse button.
Whilst playing, when in mouse mode, hold down PLAY whilst using the DPAD to 3X accelerate mouse pointer.

If you have trouble loading games, set the 'Emu Bias' fully to CPU in the main menu and retry.

Issues
------

Sound doesn't currently work.
Buttons aren't always mapped properly.
No visual indication of input mode switch.
Buttons don't always respond properly.

Release history
---------------

20061112 - version 0.6.3	Now with added FAME/C goodness, and working sound.

20060601 - version 0.5.1	Initial release for Gizmondo from GP2X 0.5.1 codebase.

The rest
--------

The original code I ported it from (a Dreamcast port) is available here:

http://chui.dcemu.co.uk/uae4all.html

zlib is used in this release.  zlib is copyright (C) 1995-2005 Jean-loup Gailly and Mark Adler

My full source is available on request to the email address below:

Email: criticalhippo@gmail.com

This is GPL'ed software... respect the license.