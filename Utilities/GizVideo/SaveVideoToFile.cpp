// SaveVideoToFile.cpp: implementation of the CSaveVideoToFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SaveVideoToFile.h"
#include <stdio.h>
//#include <commctrl.h>
//#include <winioctl.h>
//#include "oeminc.h"

#include "memory.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//FILE * f;
CSaveVideoToFile::CSaveVideoToFile(CCamera* io, char * fileName) : IObserver(io)
{
	m_cCurrentCamera= io;


	m_cAviFile= new AviFile(fileName);

//	f= fopen("\\SD Card\\testdata", "w");
}

CSaveVideoToFile::~CSaveVideoToFile()
{
//	printf("File Closed\r\n");
	delete m_cAviFile;
}

void CSaveVideoToFile::updateImage()
{
//	DWORD riendutout;	
	//prepNewFrame();
    //WriteFile(m_hFileHandle, m_cCurrentCamera->getLastPreviewBuffer(), 320*240*2, &riendutout, NULL);
	//printf("Header size %d\n", sizeof(m_cCurrentCamera->getLastPreviewBuffer()));

	//printf("FileWritten\r\n");

	m_cAviFile->addNewFrame(convertFrameToAppropritateAviFrame((short*)m_cCurrentCamera->getLastPreviewBuffer()));
}




short* CSaveVideoToFile::convertFrameToAppropritateAviFrame(short* frame)
{
	int nSizeofArray= 
		m_cAviFile->getImageSize()/ sizeof(short);
		//sizeof(frame); //todo: check to make sure sizeofframe is returnning correct
	
//	fprintf(f, "NEW FRAME: ");
//	printf("%d Shorts in frame (short = %d bytes)\n", nSizeofArray, sizeof(short));
	for (int i=0; i < nSizeofArray; i++)
	{
		short colourToConvert= 
			*frame;

		
		*frame= RGB565toRGB555(colourToConvert);
		frame++;
	}

	frame-= nSizeofArray;

//	fprintf(f, "|%d|\n\n\n");
	return frame;
}


short CSaveVideoToFile::RGB565toRGB555(short orgColour)
{
	unsigned short rmask=0, rmasked=0, gmask=0, gmasked=0, 
		bmask=0, bmasked=0, newRGB555= 0;

	//create gb and rmasks 
	rmask = 0x1F << 11;
	bmask= 0x1F;
	gmask= 0x1F << 5;

	//get rmasked
	rmasked = (orgColour & rmask) >> 11;
	
	//get bmasked
	bmasked= (orgColour & bmask) << (16-5);
	
	//get gmasks
	gmasked= orgColour & gmask;

	//combine
	newRGB555= gmasked | rmasked | bmasked;
	

	if (newRGB555 >  1 << 31)
		newRGB555/= 2;

//	if (rand() % 100 == 4 )
//	fprintf(f, "(%d, R|%d:{%d+%d}|, G|%d{%d}|, B|%d{%d+%d}| %d)\n\t", 
//		orgColour, rmasked, rmask, orgColour & rmask, gmasked, gmask, bmasked, bmask, orgColour & bmask, newRGB555);

	return newRGB555;
}
