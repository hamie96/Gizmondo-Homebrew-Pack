
===========================================================
MAME GP2X 2.3 (May 07, 2006) by Franxis (fjmar@hotmail.com)
===========================================================


1. INTRODUCTION
---------------

It is a port of MAME 0.34 emulator by Nicola Salmoria for GP2X portable console.
To see MAME license see the end of this document (chapter 16).
It emulates almost all arcade games supported by original MAME for DOS.

This version emulates 1128 different romsets.

http://www.talfi.net/gp32_franxis/


2. CONTROLS
-----------

The emulator controls are the next ones:
- Joystick: Movement in pads (1, 2, 3 and 4), mouse and analog control. 
- Buttons A,Y,B,X,L,R,VolUp,VolDown,PUSH,START: Buttons A,B,C,D,E,F,G,H,I,J.
- Button SELECT: Insert credits. (SELECT+Up=credits 2P, SELECT+Right=credits 3P, SELECT+Down=credits 4P)
- Button L+R: Change screen orientation and activate/deactivate screen scale/stretch.
- Button L+R pressed during one second: Pause.
- Button START: Play. (START+Up=Start 2P, START+Right=Start 3P, START+Down=Start 4P)
- Buttons L+R+PUSH simultaneously: Exit to selection menu to select another game.
- Buttons START+SELECT simultaneously: Reset the current game.
- Buttons VolUp and VolDown: Increase / decrease sound volume.

3. EMULATION OPTIONS
--------------------

After selecting a game on the list next configuration options are available:

- GP2X Clock:
200 to 300 MHz options are available. Performance of the emulator is better with bigger values.
200 MHz is the standard value. 250 MHz seems to run ok with all GP2X consoles (but batteries duration is reduced).
Use greater values at your own risk!.

- Frame-Skip:
0 to 5 Frame-Skip options are available. Less frame-skip produce better game video experience, 
but also the emulator is slowed... 1 to 2 seem to be Ok for almost every game.
Additionally Auto Frame-Skip is implemented: <=1, <=2, <=3, <=4; <=5 (in this cases,
frame-skip will be adjusted automatically by the emulator, up to the maximum selected frame-skip).

- Video Clock:
It permits to change underclock / overclocking option for CPUs.
It is 80% by default, but has problems with some games (i.e. Trojan, Breakthru, Mikie, Super Qix, ...)
The perfect video clock is 100%, but it is slower. Emulator is faster with smaller values and slower with higher values.

- Video Sync:
It permits to configure video synchronization options.
None: No video synchronization.
DblBuf: Video Double Buffer activated.
VSync: VSync activated.
VSync+DblBuf: Both options activated.

- Sound:
Sound ON: Standard sound option is available.
Sound OFF: Sound is not emulated and is not activated. Emulator is fastest.
Emulated OFF: Sound is emulated but it is deactivated.
Accurate ON: Accurate sound option.

- Audio Clock:
It permits to change underclock / overclocking option for audio CPUs.
It is 80% by default, but has problems with some games. The perfect sound clock is 100%, but it is slower.
Emulator is faster with smaller values and slower with higher values.

- Controller Configuration:
It permits to change controller configuration (fire buttons assignment), analog controls sensitivity and X and Y axis
inversion, auto-fire option, etc.

- Save Configuration:
No Save Configuration: Do NOT save game configuration.
Save Configuration: Save configuration for the current game.
Save as Default: Save the default configuration for all games.

- Press A to play the selected game or B to go back to game selection menu.

- In the game selection menu if L+R are pressed simultaneously, GP2X is reset.


4. INSTALLATION
---------------

/mamegp2x/mame_gp2x.gpe 		-> General frontend to select games
/mamegp2x/mame_gp2x_compatible.fxe	-> General frontend to select games (more compatible).
/mamegp2x/mame_gp2x_fast.fxe		-> General frontend to select games (faster, with DrZ80 core).
/mamegp2x/cfg/				-> Configuration Files
/mamegp2x/hi/				-> High Scores
/mamegp2x/roms/				-> ROM's directory, as ZIP files.
/mamegp2x/skins/			-> Directory for frontend skins.


5. SUPPORTED GAMES
------------------

There are 1128 different supported romsets. For more details, see "gamelist.txt" file.
Games have to be copied into /mamegp2x/roms/ folder on the SD card.


6. ROM NAMES
------------

Folder names or ZIP file names are listed on "gamelist.txt" file.
Romsets have to be MAME 0.34 final version ones (December 1998).

Please use "clrmame.dat" file to convert romsets from other MAME versions to the ones used by
this version for GP2X, using ClrMAME Pro utility, available in next webpage:

http://www.clrmame.com/

NOTE: File and directory names in Linux are case-sensitive. Put all file and directory names
using low case!.

7. ORIGINAL CREDITS
-------------------

- MAME 0.34 original version for DOS by Nicola Salmoria and the MAME Team (http://www.mame.net).

- Z80Em Portable Zilog Z80 Emulator Copyright (C) Marcel de Kogel 1996,1997
  Note: the version used in MAME is slightly modified. You can find the
  original version at http://www.komkon.org/~dekogel/misc.html.

- M6502 emulator by Juergen Buchmueller.
	
- I86 emulator by David Hedley, modified by Fabrice Frances (frances@ensica.fr)
	
- M6809 emulator by John Butler, based on L.C. Benschop's 6809 Simulator V09.
	
- M6808 based on L.C. Benschop's 6809 Simulator V09.
	
- 80x86 asm M6808 emulator Copyright 1998, Neil Bradley, All rights reserved

- M68000 emulator taken from the System 16 Arcade Emulator by Thierry Lescot.

- 8039 emulator by Mirko Buffoni, based on 8048 emulator by Dan Boris.

- T-11 emulator Copyright (C) Aaron Giles 1998

- TMS34010 emulator by Alex Pasadyn and Zsolt Vasvari.

- TMS9900 emulator by Andy Jones, based on original code by Ton Brouwer.

- TMS5220 emulator by Frank Palazzolo.

- AY-3-8910 emulation based on various code snippets by Ville Hallik,
  Michael Cuddy, Tatsuyuki Satoh, Fabrice Frances, Nicola Salmoria.

- YM-2203 and YM-2151 emulation by Tatsuyuki Satoh.

- OPL based YM-2203 emulation by Ishmair (ishmair@vnet.es).

- POKEY emulator by Ron Fries (rfries@aol.com).
  Many thanks to Eric Smith, Hedley Rainnie and Sean Trowbridge for information
  on the Pokey random number generator.

- NES sound hardware info by Jeremy Chadwick and Hedley Rainne.

- YM3812 and YM3526 emulation by Carl-Henrik Skårstedt.

- YM2610 emulation by Hiromitsu Shioya.


8. GP2X PORT CREDITS
--------------------

- Port to GP2X by Franxis (fjmar@hotmail.com) based on source code
  MAME 0.34 final version (dated on december 1998).

- TheGrimReaper (m_acky@hotmail.com) has colaborated with a lot of
  things since GP32 MAME 1.3, i.e. Vector graphics support, high scores,
  general frontend, frontend improvements, bugfixes, etc. Thank you!!!
  
- Pepe_Faruk (joserod@ya.com) has colaborated with new screen centering
  code. A very good start!!!

- Reesy (drsms_reesy@yahoo.co.uk) has developed DrZ80 (Z80 ASM ARM core) and has helped
  a lot with core integration into MAME. You are the best!!! Also Flubba has done some
  optimizations to the code.

- Dave (dev@finalburn.com) has developed Cyclone (M68000 ASM ARM core).
  Also Reesy (drsms_reesy@yahoo.co.uk) and Notaz (notasas@gmail.com) have fixed some bugs
  in the source code. http://uosis.mif.vu.lt/~grig2790/Cyclone/

- Chui (sdl_gp32@yahoo.es) has developed MAME4ALL, the MAME GP2X port for Dreamcast, Windows
  and Linux. Also he has done several optimizations aplicable to all targets.
  http://chui.dcemu.co.uk/mame4all.html

- Slaanesh (astaude@hotmail.com) has continued my work on MAME GP32 and he has done several
  improvements aplicable to all targets.
  http://users.bigpond.net.au/mame/gp32/
  

9. DEVELOPMENT
--------------

May 07, 2006:
- Version 2.3. Bugfix. Añadido core H6280.

May 01, 2006:
- Version 2.2. Merged with Chui's MAME4ALL.

April 07, 2006:
- Version 2.1. Optimizations.

March 26, 2006:
- Version 2.0. Bug fixing and support for extra 32 Mb of RAM.

March 16, 2006:
- Version 1.9. Bug fixing and support for extra 16 Mb of RAM.

March 05, 2006:
- Version 1.8. Bug fixing and Auto-Fire support.

February 23, 2006:
- Version 1.7. Bug fixing and TV-Out support.

February 18, 2006:
- Version 1.6. Bug fixing, overclocking and configuration saving.

February 05, 2006:
- Version 1.5. Hardware video scaling. New games added (MAME 0.35b3).

January 23, 2006:
- Version 1.4. New games added (MAME 0.35b1-b5).

January 08, 2006:
- Version 1.3. Cyclone compatibility improvements.

January 02, 2006:
- Version 1.2. Support for complete MAME 0.34 romset.

December 28, 2005:
- Versión 1.1. Faster, better sound, etc.

December 24, 2005:
- Version 1.0. First version.

Developed with:
- DevKitGP2X rc2 (http://sourceforge.net/project/showfiles.php?group_id=114505)
- GP2X minimal library SDK v0.A by Rlyeh (http://www.retrodev.info/)
- GpBinConv by Aquafish (www.multimania.com/illusionstudio/aquafish/)


10. KNOWN PROBLEMS
------------------

- Not perfect sound or incomplete in some games.

- Slow playability in modern games.

- Memory eating after some games played, most noticeable if there is an error during
ROMs loading or during emulation. In that case, reset GP2X and test again please ;-).


11. TO BE IMPROVED
------------------

- Use second GP2X processor.

- Future HH development kit without Linux?.

- Improve sound.

- Improve speed.

- Update romsets to actual MAME ones, or update MAME version to actual one.

- Add support to more games.


12. THANKS TO
-------------

- TheGrimReaper: Thanks for your hard work with the vector graphics library, among other
things... You are the best !!!.

- Reesy: Thanks for the great core Z80 ASM ARM (DrZ80).

- Pepe_Faruk: Thank for the new screen centering code ;-)

- Unai: Thanks Unai due to a lot of hours helping me with MAME, optimizations...

- Dave, Reesy, Notaz: Thank you due to a fantastic M68000 ASM ARM core (Cyclone).

- Chui: Thanks for MAME4ALL and all the great optimizations and improvements!!!.

- Slaanesh: Thanks for continuing my work on MAME GP32 and all the interesting improvements!!!.

- Talfi: Friend who gives me the webspace for my MAME port for GP32.

- Chicho: Great friend with blind faith on my MAME port...

- Anarchy (gp32spain.com): Thanks, Anarchy, for the GP2X development unit you sent me to port MAME.

- Gamepark Holdings: Thank you people for releasing GP2X console, as well as providing some GP2X development
units for programmers some weeks before GP2X official launch. One of them was finally mine through Anarchy
mediation (gp32spain.com).

- Rlyeh: Master of GP32 emulation... Creator of GP2X minimal library SDK.

- Hermes PS2R: Thanks, Hermes, for the CpuCtrl library (to modify GP2X clock frequency).

- Kounch: Information about the TV-Out in GP2X. Thank you!.

- Antiriad: Thank you for the excelent artwork for GP32 version ;-).

- Zenzuke, Chipan, Dokesman, Enkonsierto, Quest, Sttraping and Sike for the frontend skins for GP2X :-).

- Flubba: Some optimizations to DrZ80 ASM Core.

- Baktery: Sound advices.

- Groepaz: More sound advices.

- Woogal: Big help with the games selection frontend... 

- Alien8: Programmer of GpMameLauncher utility.

- LDChen: Help with some ASM code for the vector graphics library.

- D_Skywalk : Multipac runs now due his help. He teached me a lot of GP32 programming.

- Locke : Advices, beta-testing...

- Sssuco, [MaD]: Thank you por providing me romsets of old MAME version, as well as DATs.

- Ron: Really mad man, but good old one ;-). MadriDC organizer.

- Ilarri: Cheers ;-).

- Fox68k: Good advices.

- DaveC: Searching bugs in my MAME port. Thanks ;-).

- Creature XL: Some testing with ASM for the video output.

- EvilDragon: Creator of MAMEGP-COPIER utility. Also moderator & news-poster of gp32x.com

- WarmFluffyUK: MAME GP32 Compatibility List: http://www.berzerk.co.uk/gp32/

- BobBorakovitz, frolik, Alan: GameProbe32 webpage: http://gameprobe32.blogspot.com/

- People on IRC-Hispano #retrodev beta-testing: Xenon, Mortimor, Nestruo, Dj_Syto, K-Teto,
Ilarri, Enkonsierto, Soup, joanvr, amkan, etc.

- People on EFNet IRC #gp2xdev: DJWillis, Woogal, etc.

- More beta-testing: Locke, nullEX, Propeller, Ozius, etc.

- Richard Weeks: Thank you for a new interview about the MAME GP2X port:
http://mygp2x.com/blog_comment.asp?bi=246&m=12&y=2005&d=1&s=

- Keith: Thank you for a new interview: http://www.emulation64.com/spotlights/22/

- Hooka: Thank you for a great interview: http://www3.telus.net/public/hooka/

- Mark Rowley: Thank you for the interview for GamePark Magazine 7:
http://www.gp32x.de/cgi-bin/cfiles.cgi?0,0,0,0,2,630

- Gladiator: Thank you for the spanish interview, as well as the great GP32 article: 
http://www.viciojuegos.com/reportaje.jsp?idReportaje=131

- Thanks to all paypal donators: EvilDragon (gp32x.de), Federico Mazza, Nandove, Videogame Stuff, Denis Evans, Ricardo Cabello,
  Elías Ballesteros, J.Antonio Serralvo Martín, bagmouse7, Suj, funkyferdy, Gieese, Vincent Cleaver, William Burnett, Bleeg,
  Martin Dolphin, Ilarri, Glen Stones, Dr.Konami, Augusto Carlos Pérez Arriaza, Charles Box, Borochi, Kayday, George Tavoulareas,
  Timofonic, Fabrice Canava, Redox, Javitotrader, remowilliams, Scott Contrera, Jinhyun Seo, Anarchy (www.hardcore-gamer.net), 
  Craig (www.gp2x.co.uk), Shane Monroe, Simon Beattie, Stefan Braunstein, DaveC, Colin Bradshaw, Dana Rodolico, Revod, Michael Evers,
  Riccardo Pizzi, Fosfy45, Dj Syto, Rob Pittman, Stefan Mueller, Musa, Unai, Sascha Reuter, Globalwide Technologies Limited, 
  Juan Rivera-Novoa, Mark Carin, Julio Catalina Piedrahita, techFreak (www.gp2xtr.com), Darius Hardy, Charles Andre, Matt Brimelow,
  McOskar, SBock, Daniel PP Saurborn, Picayuco, Kojote (www.pdroms.com), Knoxximus, Tony Watterson, Matthew Forman, naples39,
  NEO (www.elotrolado.net), Patrick Mettes, Angel Molero Grueso, Lubidog, Smiths (www.emuholic.com), retromander, Ruben Villar,
  Snakefd99cb, Harkaitz, BZFrank, Sang Kim, phoda, Caesaris, Furanchu, Selcuk Cegit, K-Navis, Estaño, Jeff Hong, Jasmot, Igboo,
  Sergio Onorati, Julien Perret, Cheap Impostor.


13. INTERESTING WEBPAGES ABOUT MAME
-----------------------------------

- http://www.mame.net/
- http://www.mameworld.net/
- http://www.marcianitos.org/


14. SOME OTHER INTERESTING WEBPAGES
-----------------------------------

- http://www.talfi.net
- http://www.gp32x.com
- http://www.gp32spain.com
- http://www.emulatronia.com
- http://www.emulation64.com


15. SKINS
---------

The frontend graphic skin used in the emulator is located in next files:
/mamegp2x/skins/gp2xsplash.bmp	-> Intro screen
/mamegp2x/skins/gp2xmenu.bmp	-> Screen to select games and options
The design of this skin has been done by Zenzuke.

Bitmaps have to be 320x240 pixels - 256 colors (8 bit). In the game selection screen, the
text is drawn with color 255 of the palette with a right-under border with color 0.

In /mamegp2x/skins/others/ folder there are other alternative skins, done by next people:
Zenzuke, Chipan, Dokesman, Enkonsierto, Quest, Sttraping and Sike. Thanks to everybody !.


16. MAME LICENSE
----------------

http://www.mame.net
http://www.mamedev.com

Copyright © 1997-2005, Nicola Salmoria and the MAME team. All rights reserved. 

Redistribution and use of this code or any derivative works are permitted provided
that the following conditions are met: 

* Redistributions may not be sold, nor may they be used in a commercial product or activity. 

* Redistributions that are modified from the original source must include the complete source
code, including the source code for all components used by a binary built from the modified
sources. However, as a special exception, the source code distributed need not include 
anything that is normally distributed (in either source or binary form) with the major 
components (compiler, kernel, and so on) of the operating system on which the executable
runs, unless that component itself accompanies the executable. 

* Redistributions must reproduce the above copyright notice, this list of conditions and the
following disclaimer in the documentation and/or other materials provided with the distribution. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY 
AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
