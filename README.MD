# Gizmondo Homebrew Pack

Thanks to Critical Hippo, Reesy, sweetlilmre, and Notaz for their support and help in gathering Gizmondo binaries.
Special thanks goes to sweetlilmre who helped me fix issues with running Grodo on my Gizmondo.

## ***Getting Started with FireFly's Launcher***
Firefly's Launcher is a custom Gizmondo launcher that allows you to easily launch homebrew emulators and programs without having to use the WindowsCE mode. Most emulators in this pack will work with Firefly's Launcher, however some (like SquidgEngine.exe) are designed to only work with WindowsCE mode.
<br>
<br>
*How to setup Firefly's Launcher*
1. Download the autorun.exe file from the Utilites folder
2. Rename autorun.exe to FireFlyLauncher.exe
3. Place the FireflyLauncher.exe and gizmoboot.cfg on the root of your SD card
4. Launch FireflyLauncher from the games menu on your Gizmondo

## Updating Your Gizmondo to Service Pack B:
1. Place autorun.exe and install.bin onto the root
of your SD card
2. Run autorun.exe from the Games Directory


## Using LockNLoad with FireFly's Launcher
(LockNLoad is an application that allows you to boot the Gizmondo into WindowsCE)
1. Copy the wince folder to the root of the SD card
2. Copy LocknLoad.exe and LocknLoad.ini to the wince folder
3. Rename LocknLoad.exe to wince.exe
3. Place the gizmoboot.cfg and AUTORUN.EXE on the root of the SD card
4. Launch FireFly Launcher and then scroll down to wince

Backup download links can be found at: http://web.archive.org/web/*/http://gizmondo.stu.ac/TTA/*

##**Recovering a Dead Gizmondo**
(Courtesy of GizGuides)
Common Issues:
- Corrupted SD Card: First try removing the SD card and booting up
- Soft Boot: Press and hold down for upto 10 seconds, [Home] and [Power] buttons together. This should do a warm boot, and reset your Gizmondo.
- Hard Boot: Press and hold for a few seconds [Left Shudder] and [Volume] buttons together, then press the [Power] button. Then let go of the [Power] button whilst still holding down the other two buttons, the Gizmondo will power up and you should then be presented with a message on the screen of the Gizmondo saying “Cold Reset – Press [Play] to start”. When the message is displayed on screen for a short time, release the other two buttons [Left Shudder] and [Volume] and then press [Play] button again.

If none of these fix your issues, then you will have to reset the operating system (This is recommended over attempting a hard boot).

[Link to Homebrew Patch](https://web.archive.org/web/20060720194019/http://gizmondo.stu.ac:80/TTA/Bootloader.zip) (This is an old patcher, do not use unless other methods don't work)
[Link to spb_downgrader.rar](https://archive.org/download/GizmondoSDK/spb%20downgrader.rar)

Wipe your SD card, then install the spb_downgrader.rar contents onto your SD card. spbb_downgrader is a specific service pack that flashes your Gizmondo and reformats the SD Card. Once the OS reloads, it will prompt you to format the filesystem. After this is complete, turn off the system and replace the files with the Service Pack B from this repostiory.

***Pre-Boot Fix (Run OS Update whilst Booting Up)***
This is a last resort fix that may fix issues where the system does not boot to the Gizmondo GUI.

Press the [Power] button, till you get the options to Keylock, Power Settings, FlightMode and insert SD Card with “ServicePack B DownGrader”.
Once you see the final screen, with the green circle Tick “Update Completed Successfully!” Press the [Play] button to restart your Gizmondo and with luck, should fully boot up to the Gizmondo Gui.

***Active Sync***

Active Sync can sometimes be used to repair a dead Gizmondo (low success rate). [Files can be found for it here](https://archive.org/details/gizactivesync_202407). [Guide for ActiveSync here](http://gizguides.wikidot.com/asguide).

***Gizmondo Control Center***
To be updated when I get access to the software.

(Note: psx4giz does not currently work right now. I am unsure if there are files missing or if the emulator is compiled for a different system. Please contact me if you have a working version of the psx4giz file.)

If you have any questions, please contact me at hamie96@gmail.com
