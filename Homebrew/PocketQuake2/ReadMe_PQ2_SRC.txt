Dan East, Jan 21, 2002

mailto:quake@dex-plor.com
http://quake.pocketmatrix.com
http://dex-plor.com

Here are the sources for my port of Quake II to the Pocket PC platform.  This was a pretty clean, straightforward port, especially compared to Quake 1.  :)  id Software is definitely getting better at writing portable software (or at least software that builds cleanly with Microsoft compilers).  Pocket Quake II builds with Microsoft's eVC++ (which is free).  The Pocket PC SDK is required.  You also need the GAPI SDK.  Notice that the GAPI header file needs tweaking to properly compile under C.  I don't know why MS created such a header that requires C++.

I attempted to make polite changes to the source code.  All Pocket PC / Windows CE specific modifications have been wrapped with appropriate precompiler directives.  So, these sources should still build fine under VC++ for Windows / PC targets (although I've not tried it, so there may be a loose end or two I missed).

I have also applied one of my Fixed Point conversions from Quake 1.  There are several more fixed point conversions from Quake 1 that are applicable to Quake II.  I'll get around to adding them later.  Hopefully they will increase the fps a few points.

Sound is currently not working.  id Software had existing routines in place for WaveOut APi support.  I simply used directives to prevent the compilation of all the DirectX sound routines, which of course is not supported under Windows CE.  I may have inadvertently blocked out a critical portion of the WaveOut routines as well.  I can't really think of any other reason the sound would not work.

Various Unicode conversions were provided, as required by certain Windows CE APIs.

Compact Flash support was added to the Pak search paths.

All display output is performed via GAPI direct screen access.  Additional video modes were created to fit the Pocket PC display: 240x180 portrait, 320x240 landscape left, and 320x240 landscape right.  The actual flipping is performed when the back buffer is blitted to the display hardware.  I basically used the display routines from my Pocket Quake 1 port for PQ2.

The input handlers are also borrowed from the PQ1 source (why would I want to write the same thing twice? :) ).  


So what is needed?  

1) Fix the sound routines

2) Pak file compression (I really don't like the simplistic approach I took with Pocket Quake 1. While it is nice to simply gzip up a pak, and not require a proprietary packing utility, the loading is way too slow. With the additional sizes of Quake II's data files the problem would be exacerbated).

3) Fixed point integer conversions, fixed point integer conversions, fixed point integer conversions!  I am confident the FPS can at LEAST be doubled through extensive conversion of the existing floating point math to fixed point.

I'd like to thank id Software and John Carmack for releasing the source to Quake II.

Finally, thanks to Randall Schleufer for the awesome skins.  Notice that the skins (controls.bmp and titlebar.bmp) are copyright, and may not be used in any commercial products, which would include a commercial, id Software-licensed build of PQ2

Dan East