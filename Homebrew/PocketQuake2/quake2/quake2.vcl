<html>
<body>
<pre>
<h1>Build Log</h1>
<h3>
--------------------Configuration: quake2 - Win32 (WCE MIPS) Release--------------------
</h3>
<h3>Command Lines</h3>
Creating temporary file "C:\DOCUME~1\DANEAS~1.INS\LOCALS~1\Temp\RSP158.tmp" with contents
[
/nologo /W3 /O2 /D _WIN32_WCE=300 /D "WIN32_PLATFORM_PSPC" /D "MIPS" /D "_MIPS_" /D UNDER_CE=300 /D "UNICODE" /D "_UNICODE" /D "NDEBUG" /Fo"MIPSRel/" /Oxs /MC /c 
"D:\cpp\PocketQuake2\quake2\win32\cd_win.c"
"D:\cpp\PocketQuake2\quake2\client\cl_cin.c"
"D:\cpp\PocketQuake2\quake2\client\cl_ents.c"
"D:\cpp\PocketQuake2\quake2\client\cl_fx.c"
"D:\cpp\PocketQuake2\quake2\client\cl_input.c"
"D:\cpp\PocketQuake2\quake2\client\cl_inv.c"
"D:\cpp\PocketQuake2\quake2\client\cl_main.c"
"D:\cpp\PocketQuake2\quake2\client\cl_newfx.c"
"D:\cpp\PocketQuake2\quake2\client\cl_parse.c"
"D:\cpp\PocketQuake2\quake2\client\cl_pred.c"
"D:\cpp\PocketQuake2\quake2\client\cl_scrn.c"
"D:\cpp\PocketQuake2\quake2\client\cl_tent.c"
"D:\cpp\PocketQuake2\quake2\client\cl_view.c"
"D:\cpp\PocketQuake2\quake2\qcommon\cmd.c"
"D:\cpp\PocketQuake2\quake2\qcommon\cmodel.c"
"D:\cpp\PocketQuake2\quake2\qcommon\common.c"
"D:\cpp\PocketQuake2\quake2\client\console.c"
"D:\cpp\PocketQuake2\quake2\qcommon\crc.c"
"D:\cpp\PocketQuake2\quake2\qcommon\cvar.c"
"D:\cpp\PocketQuake2\quake2\qcommon\files.c"
"D:\cpp\PocketQuake2\quake2\win32\in_win.c"
"D:\cpp\PocketQuake2\quake2\client\keys.c"
"D:\cpp\PocketQuake2\quake2\client\menu.c"
"D:\cpp\PocketQuake2\quake2\qcommon\net_chan.c"
"D:\cpp\PocketQuake2\quake2\win32\net_wins.c"
"D:\cpp\PocketQuake2\quake2\qcommon\pmove.c"
"D:\cpp\PocketQuake2\quake2\win32\q_shwin.c"
"D:\cpp\PocketQuake2\quake2\client\qmenu.c"
"D:\cpp\PocketQuake2\quake2\client\snd_dma.c"
"D:\cpp\PocketQuake2\quake2\client\snd_mem.c"
"D:\cpp\PocketQuake2\quake2\client\snd_mix.c"
"D:\cpp\PocketQuake2\quake2\win32\snd_win.c"
"D:\cpp\PocketQuake2\quake2\server\sv_ccmds.c"
"D:\cpp\PocketQuake2\quake2\server\sv_ents.c"
"D:\cpp\PocketQuake2\quake2\server\sv_game.c"
"D:\cpp\PocketQuake2\quake2\server\sv_init.c"
"D:\cpp\PocketQuake2\quake2\server\sv_main.c"
"D:\cpp\PocketQuake2\quake2\server\sv_send.c"
"D:\cpp\PocketQuake2\quake2\server\sv_user.c"
"D:\cpp\PocketQuake2\quake2\server\sv_world.c"
"D:\cpp\PocketQuake2\quake2\win32\sys_win.c"
"D:\cpp\PocketQuake2\quake2\win32\vid_dll.c"
"D:\cpp\PocketQuake2\quake2\win32\vid_menu.c"
"D:\cpp\PocketQuake2\quake2\client\x86.c"
]
Creating command line "clmips.exe @C:\DOCUME~1\DANEAS~1.INS\LOCALS~1\Temp\RSP158.tmp" 
Creating temporary file "C:\DOCUME~1\DANEAS~1.INS\LOCALS~1\Temp\RSP159.tmp" with contents
[
winsock.lib commctrl.lib coredll.lib aygshell.lib note_prj.lib /nologo /base:"0x00010000" /stack:0x100000,0x10000 /entry:"WinMainCRTStartup" /incremental:no /pdb:"MIPSRel/pocketquake2.pdb" /nodefaultlib:"libc.lib /nodefaultlib:libcd.lib /nodefaultlib:libcmt.lib /nodefaultlib:libcmtd.lib /nodefaultlib:msvcrt.lib /nodefaultlib:msvcrtd.lib /nodefaultlib:oldnames.lib" /out:"MIPSRel/pocketquake2.exe" /subsystem:windowsce,3.00 /MACHINE:MIPS 
.\MIPSRel\cd_win.obj
.\MIPSRel\cl_cin.obj
.\MIPSRel\cl_ents.obj
.\MIPSRel\cl_fx.obj
.\MIPSRel\cl_input.obj
.\MIPSRel\cl_inv.obj
.\MIPSRel\cl_main.obj
.\MIPSRel\cl_newfx.obj
.\MIPSRel\cl_parse.obj
.\MIPSRel\cl_pred.obj
.\MIPSRel\cl_scrn.obj
.\MIPSRel\cl_tent.obj
.\MIPSRel\cl_view.obj
.\MIPSRel\cmd.obj
.\MIPSRel\cmodel.obj
.\MIPSRel\common.obj
.\MIPSRel\conproc.obj
.\MIPSRel\console.obj
.\MIPSRel\crc.obj
.\MIPSRel\cvar.obj
.\MIPSRel\files.obj
.\MIPSRel\in_win.obj
.\MIPSRel\keys.obj
.\MIPSRel\m_flash.obj
.\MIPSRel\md4.obj
.\MIPSRel\menu.obj
.\MIPSRel\net_chan.obj
.\MIPSRel\net_wins.obj
.\MIPSRel\pmove.obj
.\MIPSRel\q_shared.obj
.\MIPSRel\q_shwin.obj
.\MIPSRel\qmenu.obj
.\MIPSRel\quake2.obj
.\MIPSRel\snd_dma.obj
.\MIPSRel\snd_mem.obj
.\MIPSRel\snd_mix.obj
.\MIPSRel\snd_win.obj
.\MIPSRel\StdAfx.obj
.\MIPSRel\sv_ccmds.obj
.\MIPSRel\sv_ents.obj
.\MIPSRel\sv_game.obj
.\MIPSRel\sv_init.obj
.\MIPSRel\sv_main.obj
.\MIPSRel\sv_send.obj
.\MIPSRel\sv_user.obj
.\MIPSRel\sv_world.obj
.\MIPSRel\sys_win.obj
.\MIPSRel\vid_dll.obj
.\MIPSRel\vid_menu.obj
.\MIPSRel\x86.obj
.\MIPSRel\quake2.res
]
Creating command line "link.exe @C:\DOCUME~1\DANEAS~1.INS\LOCALS~1\Temp\RSP159.tmp"
<h3>Output Window</h3>
Compiling...
cd_win.c
cl_cin.c
cl_ents.c
cl_fx.c
cl_input.c
cl_inv.c
cl_main.c
cl_newfx.c
cl_parse.c
cl_pred.c
cl_scrn.c
cl_tent.c
cl_view.c
cmd.c
cmodel.c
common.c
console.c
crc.c
cvar.c
files.c
Generating Code...
Compiling...
in_win.c
keys.c
menu.c
net_chan.c
net_wins.c
pmove.c
q_shwin.c
qmenu.c
snd_dma.c
snd_mem.c
snd_mix.c
snd_win.c
sv_ccmds.c
sv_ents.c
sv_game.c
sv_init.c
sv_main.c
sv_send.c
sv_user.c
sv_world.c
Generating Code...
Compiling...
sys_win.c
vid_dll.c
vid_menu.c
x86.c
Generating Code...
Linking...



<h3>Results</h3>
pocketquake2.exe - 0 error(s), 0 warning(s)
</pre>
</body>
</html>
