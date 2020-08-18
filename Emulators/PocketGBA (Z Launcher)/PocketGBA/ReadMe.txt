*------------------------------------------------------------*
    PocketGBA, gameboy advance emulator for PocketPC
						 by trapung
*------------------------------------------------------------*

YOUR OWN RISK!!!

latest version : rel.060526 
codes : C + ASSEM(ARM-codes)
compilier : eVC++ ver.3.0 
assembler : ARM Macro Assembler, ver. 2.50.8684
build : PocketPC2002

homepage : http://www.sunbug.net

Special thanks to Maskin, for detailed bug-reports


*Notes that..
-If you wanna say me some problem with PocketGBA, let me know 
  1. What is your device and OS
  2. Cartridge name
  3. Problem details
  4. Attach [PocketGBA] to email-title, unless I erase email for SPAM.


*Memory allocation
-Internal GBA memory : (445440+?????) bytes

*Cartridges
 Some cartridges work with following options :

   1. Sound On/Off 
   2. Cartridge masking On/Off
   3. Patch cart. On/Off
 
 Try changing these options... if your cart. does't work.
 


*History

-rel. 060723 :
   -support ZIP file
   -support Intel 2700g accelerator
   -added ARM assem. code for GBA's BIOS functions:
      0x0B: CPU Set			
	  0x0C: CPU Fast Set 

-rel. 060526 :
   -fixed some UI
   -fixed sound functions : setFrequency() with Timer0
   -fixed thumb core : added 'r11' for PC 
   -fixed path of state files : \SaveData folder
     => Copy your state file to 'SaveData' folder.
   -added state saving/loading option with 9 slots
   -added cartridge masking option : some cartridges work with cartridge-masking
     => This option will mask cartridge's address with cartridge's size. 
	 => Note that this option will not be saved in config-file.
   -added auto-frameskip option : adjust frameskip rate to near 100%-speed

-rel. 060418 :
   -support cartridges of 32MByte

-rel. 060406 :
   -fixed checking-timer function
      -> PocketMon-Emerald, PocketMon-FireRed, PocketMon-LeafGreen, etc..
   -fixed loading-state function

-rel. 060324 : 
   -fixed Shoulder-R/L button states of VirtualPad
   -fixed dummy address value for LDRB
      -> Legend of Zelda, The - The Minish Cap
   -added full screen drawing
   -fixed some console function
   -added 'Exit' UI button
   -fixed GetOpenFileName (gsGetFile.dll)
      -> VGA Devices

-rel. 051030 :
   -changed ARM7tdmi entire core. some buggy & on debuggggging -_-;
   -added eeprom saving/loading function
   -fixed version name -> 'rel. yymmdd' format (released date)
   -changed 'Exit' menu to 'System' menu
   -added virtual joypad
   -added 'Edit Game Screen' option (in system menu)
   -added game reset option (in system menu)
   -added 'Patch cart' option (in system menu)
      -> for some Hacked Cartridges, 
	    i.e., 
			0775 - Kirby - Nightmare in Dreamland (U) [hI].gba
			0807 - Hajime no Ippo The Fighting (J).gba
			Samurai Deeper Kyo.gba
			and so on.
		With 'Patch cart' option, PocketGBA jumps to original codes. 
		(BUT, not for all hacked cartridegs!!)
		'0775-Kirby-..' is only working with this option.
		Toggle this option before you load a cartridge.
	-added Sound parameters option (in sound menu)
	-added Quick LOAD/SAVE with 2 slots (emulating states)
	-fixed max available memory. upto 24Mega-bytes (for some Korean patched cartridges).
	-added some WindowCE message handler.. (for illegal escape from PocketGBA)
	-released PocketGBA without SOUND version. 
	    -> This version is for devices that have not compatibility with FMODCE library.
		-> Check 'fmodce.dll' at 'http://www.fmod.org' for your device.


-v0.3 alpha :
   -support 16MB cartridge files
   -fixed save/load config file function
   -fixed button-process function
     -> now L&R-shoulder buttons are operated.
   -added sound setting option 
     -> only Sound On/Off
   -added 'Show FPS(Speed)' option (in Video option)
   -added checking memory state of current device
   -added direct entrance assem. codes to ARM loop from THUMB loop
   -added direct entrance assem. codes to THUMB loop from ARM loop
   -fixed cascaded-timer function 
     -> speed improved with 'Hajime no Ippo The Fighting', 'Oni-Musha tatics', 'Samurai deeper Kyo', and so on..  
   -added reset-button in control setup

-v0.2 beta  : 
   -added vertical game screen (UI)
   -fixed read memory BYTE(8bits) function
     -> now working : 'Advance Wars 2', 'Super Robot War OG' 
   -fixed key-scan function
     -> Scancode with 0x5b was fixed.
   -fixed multiple load/store instruction (THUMB state)
     -> LDMIA / STMIA ( 'Shining Soul', 'Full Metal Alchemist' )
   -fixed ARM7tdmi load word immediate offset (THUMB state)
     -> LDR Rd, [Rn, #imm5] ( 'Dragon Quest Monsters' )
	 -> in ARM7tdmi datasheet, operation of 'LDR' in THUMB state is described as follows : 
	      if address[1:0] == 0b00
		      data = Memory[address,4]
		  else 
		      data = UNPREDICTABLE
		  Rd = data
		But, 'Dragon Quest Monsters' runs correctly with
		  if address[1:0] == 0b00
		      data = Memory[address,4]
		  else 
		      data = Memory[address,4] Rotate_Right address[1:0]*8
		  Rd = data

-v0.2 alpha :
   -fixed ARM7tdmi data-processing operands (ARM state)
     -> shift by register instruction
   -fixed ARM7tdmi multiply instruction (ARM state)
     -> MUL,MULS,MLA,MLAS
   -fixed ARM7tdmi multiple load/store instruction (ARM state)
     -> LDM* / STM*
   -fixed screen-drawing function (now, all 160 scanlines will be shown)
   -added 'direct sound A' with DMA mode
     -> not correct 
	 -> only for DMA mode ( direct sound with interrupt mode will be noisy )
	 -> 'fmodce.dll' is required.
  

-v0.1 beta  : 
   -added User Interface(UI)
   -added save/load functions cartridge memroy(sram/flash-rom)
   -added save/load config file - PocketGBA.cfg (binary file)
   -fixed some ARM7tdmi cpu-cycles (not correct)
   -fixed ARM7tdmi load instruction routines of ARM & THUMB
		-> LDRH, LDRSH, LDRSB / ARM & THUMB state

-v0.1 alpha : first public release


*Etc.,
PocketGBA's icons are from
-http://www.ix.sakura.ne.jp/~yoshi/icon/game/
-http://homepage2.nifty.com/tenpurako/icon_factory.html

