#include <stdio.h>

typedef struct
{ 
   char cc4[4];
} FourCC;

typedef struct
{
    FourCC id;
    int cbSize;
} RiffChunkHeader;

typedef struct { 
   int Size;
   int Width;
   int Height;
   short Planes;
   short BitCount;
   int Compression;
   int SizeImage;
   int XPelsPerMeter;
   int YPelsPerMeter;
   int ColourUsed;
   int ColourImportant;
} BitmapInfoHeader; 

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

int main()
{
//   printf("AviExample. Steven Hicks\n");
   int width = 320,
       height = 240;

   int stride = (int)((width * (1 / 8) + 3) & ~3);
   int imageSize= 3*height * (stride + width);
   
   MainAVIHeader main= 
   	{ 
   	  (int)((5/60)*1000),
   	  imageSize,
   	  imageSize*5,
   	  0x00010000,
   	  200,
   	  0,
   	  1,
   	  imageSize,	
   	  width,
          height
   	  
   	};
   
   AVIStreamHeader aheader= 
   {
        {"vids"}, {"DIB "},
        0,
        1,
        0,
        1, 5, 0,0,
        imageSize,
        9,
         {0, 0, width, height}
   };
   
   BitmapInfoHeader bhi= {
      40, 
      width, height, 
      1, 16,
      0, imageSize,
      2000,2000, 
      256,
      256
   };
 
   RiffChunkHeader rch= { {"RIFF"}, 2000 };   

   RiffChunkHeader avih = {{"avih"}, sizeof(main) };   
   RiffChunkHeader inSh = {{"strh"}, sizeof(aheader) };
   RiffChunkHeader inSf = {{"strf"}, 40 };

   //RiffChunkHeader blankAu= {{"LIST"} , 0};
   
   RiffChunkHeader rinternHeader= {{"LIST"}, 2 /*sizeof(inSh)+inSh.cbSize+ sizeof(inSf)+inSf.cbSize */};   
   
   
   RiffChunkHeader rmainList= {{"LIST"}, 2 /*sizeof(rinternHeader)+ rinternHeader.cbSize+ 4 + sizeof(avih)+ avih.cbSize */};

//   printf("\tavih: %d\n\tstrh: %d\n\tstrf: %d\n\trinternHeader: %d\n\t--------------\n\trmain: %d\n", 
  //      avih.cbSize, inSh.cbSize, inSf.cbSize, rinternHeader.cbSize, rmainList.cbSize);
   printf("Width %d\nHeight %d\nStride %d\n", width, height, stride);
   RiffChunkHeader introChunk= {{"00db"}, imageSize};

 // RiffChunkHeader movi= {{"movi"} , 3*imageSize }; 
   RiffChunkHeader rstart= {{"LIST"}, 30};

//   RiffChunkHeader 
   FILE* f= NULL;
   
   f= fopen("testfile", "w");

   fwrite(&rch, 1, sizeof(rch),f);
   fprintf(f,"AVI ");
   fwrite(&rmainList,1,sizeof(rmainList),f);
   fprintf(f, "hdrl");   

   fwrite(&avih, 1, sizeof(avih), f);   
   fwrite(&main, 1, sizeof(main), f);

   fwrite(&rinternHeader, 1, sizeof(rinternHeader), f);
   fprintf(f, "strl");
   
   fwrite(&inSh, 1, sizeof(inSh), f);
   fwrite(&aheader, 1, sizeof(aheader), f);
   
   fwrite(&inSf, 1, sizeof(inSf), f);
   fwrite(&bhi, 1, sizeof(bhi), f);
   
   fwrite(&rstart, 1, sizeof(rstart), f);
   //fwrite(&movi, 1, sizeof(movi), f);
     fprintf(f, "movi"); 
   int frame =0;

   
   for (; frame < 3; frame++)
   {
   	fwrite(&introChunk, 1, sizeof(introChunk), f);
  	
   	int x=0, y=0;
   	unsigned char curClr=rand(255);
   	
   	for (y=0; y< 240; y++)
   	{
   	    for (x=0; x < 320; x++)
   	    {
   	    	fwrite(&curClr, 1, sizeof(curClr), f);
   	    	fwrite(&curClr, 1, sizeof(curClr), f);
   	    	fwrite(&curClr, 1, sizeof(curClr), f);
   	    }

   	    int i=0; char cStride='0';
   	    for (; i < stride; i++)
	   	    {
   	        fwrite(&cStride, 1, sizeof(cStride), f);
   	    }
   	    
   	}
   }
      

      
   fclose(f);
   return 0;
}
