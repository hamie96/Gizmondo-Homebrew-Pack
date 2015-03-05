GizBoy 0.1.0
---------------

Gizmondo port by critical of gnuboyce 0.9.13-0.7, which in turn is based on gnuboy.

It emulates Nintendo GameBoy and GameBoy Color systems, complete with sound, SRAM and
savestates.

It uses 1.5x anti-aliased scaling.  It is *extremely* unlikely that I will change that.

It DOES NOT emulate the GameBoy Advance, and never will.

gnuboy is licensed under GNU Generic Public License(GPL), as is GizBoy.
Source is available on request to the contact address below.

The original licence files and documentation may be found in the doc directory of 
the zip file that this readme file came in.


Source is available on request, or at:

http://www.rxshorty.nl/gizmondo/homebrew/source/critical/GizBoy-0.1.0-src-release.zip


Contact: criticalhippo@gmail.com

Installation
------------

Put GizBoy.exe in the root of your SD card, along with the included gx.dll 
if you don't already have it.  If you're using firefly's launcher, put everything in
a directory called GizBoy in the root of your SD card.


Usage
-----

GameBoy and GameBoy Color roms must be in a directory called:

GizBoy

in the root of your SD Card.

The GameBoy ROMs must have the extension '.gb' and the GameBoy Color ROMs must
have the extension '.gbc'.


Buttons
-------

The joypad does what you'd expect.  The other buttons are:

STOP	START
PLAY	B
FFWD	A
RWND	SELECT

During emulation, the home key toggles main menu on and off, which will also 
pause/resume the emulator.

Load/save state has been implemented.

Press the Speaker key to load state, and the Asterisk (*) key to save state.

If there is no state to load, and you try to load state, it will say that
it succeeded, but obviously it won't have.

Whilst in the main menu, the STOP key exits back to the Gizmondo system.

Up and Down on the joypad in the main menu select the ROM, PLAY starts
L and R triggers will page through the ROMs 10 at a time.
