GizMenu is a launcher for the Gizmondo, it allows you to place links to all of your games / apps in a nice looking, skinable menu system.

Previous versions of GizMenu were an attempt at a shell replacement, this new version is an "Autorun Launcher".
This is how the change came about:

After looking at the new gmenu2x and porting it to the Giz, I was struck with an idea.
Instead of making GizMenu a full shell, why not have it as an Autorun launcher, then you get the best of both worlds: All standard capabilities and shell, plus a really kick-ass launcher.

So.. what I did was make a small Autorun program that does the following:

1. Checks an ini file for a menu to launch.
2. Creates eCursor (hidden) through eCursorLib
3. Creates the menu process
4. Waits for the menu process to exit
5. Checks some shared memory to see what the menu wanted to launch
6. Launches the process, conditionally enabling eCursor
7. Wait for the process to exit.
8. Relaunches the menu.

So without too much further ado, I present:

GizMenu 0.9, based on gmenu2x 0.9
and
Autorun for GizMenu.

Unrar the archive to the ROOT of your SD card (backing up any other autorun you may have as it will overwrite it).

Launch the Autorun in the standard manner.

Keys in GizMenu:

PLAY: Launch selected link / Confirm action.
FORWARD: Cancel action.
FORWARD: Clear Link text item
FORWARD: Goes up one directory in file browser.
L, R: Switch between sections - PageUp/PageDown on lists.
STOP: Bring up the manual/readme.
REWIND+L: Decrease volume for the selected link
REWIND+R: Increase volume for the selected link
BRIGHT: Bring up the contextual menu.
HOME: GMenu2X options.
REWIND: Help

NEW (v10): Wallpaper is now handled by the skin settings dialog. !WALLPAPER command removed

NEW (v6): Special "Bang" commands can be used in the exec= link of a link:

CODE
!EXPLORER   : Launch the built in explorer
!MENUOPT    : Edit GizMenu settings
!SKIN       : Edit skin settings
!ABOUT      : Show the About screen
!LOGVIEWER  : Run the log viewer (not very useful at the moment)
!EXIT       : Exit GizMenu (as before)


NEW (v5): "Four-Finger-Salute" Force Kill ™ for launched programs: L + R + RIGHT + REWIND
(Keys must be pressed and held for a minimum of 500ms or the value KillTimer in the [Main] section in Autorun.ini)


The rest is pretty self explanitory.

Look in the Sections\System folder under GizMenu for "Exit" which is an example of the !EXIT bang command and in the Sections\Applicatios folder for "tre" which launches Tascal Registry Editor with eCursor enabled.

As eCursor is running under the Autorun process, you can enable it using the hot key (modify eCursor.ini in the root to change the hot key as per usual).

Suggestions, bug report etc. welcome.
I've been using this now for a while and its really useful.
