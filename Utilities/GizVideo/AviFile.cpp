// AviFile.cpp: implementation of the AviFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AviFile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AviFile::AviFile()
{
	initializeState();
}

AviFile::~AviFile()
{
	fclose(m_cAVIFile);
}


/*
   
   for (; frame < 3; frame++)
   {

  	
   	int x=0, y=0;
   	unsigned char curClr=rand(255);
   	
   	for (y=0; y< 240; y++)
   	{
   	    for (x=0; x < 320; x++)
   	    {
   	    	fwrite(&curClr, 1, sizeof(curClr), m_cAVIFile);
   	    	fwrite(&curClr, 1, sizeof(curClr), m_cAVIFile);
   	    	fwrite(&curClr, 1, sizeof(curClr), m_cAVIFile);
   	    }

   	    int i=0; char cStride='0';
   	    for (; i < stride; i++)
	   	    {
   	        fwrite(&cStride, 1, sizeof(cStride), m_cAVIFile);
   	    }
   	    
   	}
   }
      
*/
  
AviFile::AviFile(char *fileName)
{
	initializeState();

	m_cAVIFile= fopen(fileName, "w");

	createNewHeader();
}


void AviFile::createNewHeader()
{
   
   
   MainAVIHeader main= 
   	{ 
   	  (int)((5/60)*1000),
   	  m_nImageSize,
   	  m_nImageSize*5,
   	  0x00010000,
   	  200,
   	  0,
   	  1,
   	  m_nImageSize,	
   	  m_nWidth, m_nHeight
   	  
   	};
   
   AVIStreamHeader aheader= 
   {
        'sdiv', ' BID',
        0,
        1,
        0,
        1, 5, 0,0,
        m_nImageSize,
        9,
         {0, 0, m_nWidth, m_nHeight}
   };
   

/*   RGBQuad rgb=
   {
	   5,6,5,0
   };
  */ 
   BitmapInfoHeader bhi= {
      40, 
      m_nWidth, m_nHeight, 
      1, 16,
      0 /*'PBGR' '565R'*/, m_nImageSize,
      2000,2000, 
      0,
      0
   };
//   char[4]= "RIFF";
   RiffChunkHeader rch= { 'FFIR', -1 };   

   RiffChunkHeader avih = {'hiva', sizeof(main) };   
   RiffChunkHeader inSh = {'hrts', sizeof(aheader) };
   RiffChunkHeader inSf = {'frts', 40 };
   RiffChunkHeader rinternHeader= {'TSIL', 2 };   
   RiffChunkHeader rmainList= {'TSIL', 2 };
   RiffChunkHeader rstart= {'TSIL', 30};

  

   fwrite(&rch, 1, sizeof(rch), m_cAVIFile);
   fprintf(m_cAVIFile,"AVI ");
   fwrite(&rmainList,1,sizeof(rmainList), m_cAVIFile);
   fprintf(m_cAVIFile, "hdrl");   

   fwrite(&avih, 1, sizeof(avih), m_cAVIFile);   
   fwrite(&main, 1, sizeof(main), m_cAVIFile);

   fwrite(&rinternHeader, 1, sizeof(rinternHeader), m_cAVIFile);
   fprintf(m_cAVIFile, "strl");
   
   fwrite(&inSh, 1, sizeof(inSh), m_cAVIFile);
   fwrite(&aheader, 1, sizeof(aheader), m_cAVIFile);
   
   fwrite(&inSf, 1, sizeof(inSf), m_cAVIFile);
   fwrite(&bhi, 1, sizeof(bhi), m_cAVIFile);
  // fwrite(&rgb, 1, sizeof(rgb), m_cAVIFile);
   
   
   fwrite(&rstart, 1, sizeof(rstart), m_cAVIFile);
   fprintf(m_cAVIFile, "movi"); 

}

void AviFile::addNewFrame(short* frameData)
{
   RiffChunkHeader introChunk= {'bd00', m_nImageSize};
   fwrite(&introChunk, 1, sizeof(introChunk), m_cAVIFile);
   fwrite(frameData, 1, /*sizeof(frameData)*/ m_nImageSize, m_cAVIFile);
}

void AviFile::initializeState()
{
	m_cAVIFile= NULL;
	
	m_nWidth= 320;
	m_nHeight= 240;

  int stride =
	  (int)((m_nWidth * (1/8) + 3) & ~3);
	//stride = (biWidth * (biBitCount / 8) + 3) & ~3.

//	printf("---Stride: %d---\n", stride);
   m_nImageSize= sizeof(short)*m_nHeight* (stride + m_nWidth);
}

int AviFile::getImageSize()
{

	return m_nImageSize;
}
