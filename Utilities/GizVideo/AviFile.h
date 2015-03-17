// AviFile.h: interface for the AviFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVIFILE_H__87C55D34_C6FB_401E_A4FB_4F66916F17B0__INCLUDED_)
#define AFX_AVIFILE_H__87C55D34_C6FB_401E_A4FB_4F66916F17B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AviFile  
{
public:
	int getImageSize();
	void addNewFrame(short* frameData);
	FILE* m_cAVIFile;
	AviFile(char* fileName);
	AviFile();
	virtual ~AviFile();

protected:
	void createNewHeader();
	
typedef struct
{ 
   DWORD cc4;
} FourCC;

typedef struct
{
	byte blue;
	byte green;
	byte red;
	byte reserved;
} RGBQuad;

typedef struct
{
    FourCC id;
    DWORD cbSize;
} RiffChunkHeader;

typedef struct {
    DWORD  Size;
    LONG   Width;
    LONG   Height;
    WORD   Planes;
    WORD   BitCount;
    DWORD  Compression;
    DWORD  SizeImage;
    LONG   XPelsPerMeter;
    LONG   YPelsPerMeter;
    DWORD  ColourUsed;
    DWORD  ColourImportant;
} BitmapInfoHeader;


typedef struct { 
   int Size;
   int Width;
   int Height;
   short Planes;
   short BitCount;
   DWORD Compression;
   int SizeImage;
   int XPelsPerMeter;
   int YPelsPerMeter;
   int ColourUsed;
   int ColourImportant;
} BACKUPBitmapInfoHeader; 

typedef struct {
   int MicroSecPerFrame;
   int MaxBytesPerSec;
   int ReservedFirst;
   int Flags;
   int TotalFrames;
   int InitialFrames;
   int Streams;
   int SuggestedBufferSize;
   int Width;
   int Height;
   int Reserved[4];
} MainAVIHeader;

typedef struct {
    short Left;
    short Top;
    short Right;
    short Bottom;
} Rect;

typedef struct {
   FourCC fccType;
   FourCC fccHandler;
   int Flags;
   int InitialFrames;
   int Scale;
   int Rate;
   int Start;
   int Length;
   int SuggestedBufferSize;
   int Quality;
   int SampleSize;
   Rect rcFrame;
} AVIStreamHeader;

private:
	void initializeState();
	int m_nImageSize;
	int m_nHeight;
	int m_nWidth;
};

#endif // !defined(AFX_AVIFILE_H__87C55D34_C6FB_401E_A4FB_4F66916F17B0__INCLUDED_)
