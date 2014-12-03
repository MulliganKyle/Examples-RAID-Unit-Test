#include <sys/time.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>

#include <errno.h>

#include "raidlib.h"

int sectorsRead=0;


void printBuffer(char *bufferToPrint)
{
   int idx;

   for(idx=0; idx < SECTOR_SIZE; idx++)
   printf("%x ", bufferToPrint[idx]);

   printf("\n");
}
	



//
//read the input file and stripe across 4 files
//
int  readInput(unsigned char *fileBuff,
	       char *inputFile)
{
  static int fd, first=1;
  int readAmount, readSoFar, toRead;

//
//open input file
//
   if( first)
   {
      if( (fd= open(inputFile, O_RDWR | O_CREAT, 00644))<0)  perror("open");
      first=0;
   }

//
//read one sector at a time from the input file into buffers
//

   for(readAmount=0, readSoFar=0, toRead=SECTOR_SIZE; readSoFar<SECTOR_SIZE;)
   {
      readAmount=read(fd, &fileBuff[readSoFar], toRead);
      if( readAmount==0)
      {
	 close(fd);
	 return readSoFar;
      }
      else
      {
	 toRead=toRead-readAmount;
	 readSoFar=readSoFar+readAmount;
      }
   }
   return readSoFar;
}

void stripeRaidFiles(unsigned char *fileBuffPtr,
		     int amountToWrite)
{

   static int fd[4], first=1, idx1=0;
   int writeAmount, idx;

//
//open raid files
//
   if(first)
   {
      if( (fd[0]= open("raidFile1.bin", O_RDWR | O_CREAT, 00644))<0) perror("open");
      if( (fd[1]= open("raidFile2.bin", O_RDWR | O_CREAT, 00644))<0) perror("open");
      if( (fd[2]= open("raidFile3.bin", O_RDWR | O_CREAT, 00644))<0) perror("open");
      if( (fd[3]= open("raidFile4.bin", O_RDWR | O_CREAT, 00644))<0) perror("open");
      first=0;
   }

//
//write the buffers to the files 1 thru 4
//

   writeAmount=write(fd[idx1], fileBuffPtr, SECTOR_SIZE);
   assert(writeAmount == SECTOR_SIZE);
   idx1= (idx1+1)%4;
   if(amountToWrite < SECTOR_SIZE)
   {
      memset(fileBuffPtr, 0, SECTOR_SIZE);

      for(;idx1!=0 && idx1<4;idx1++)
      {
	 writeAmount=write(fd[idx1], fileBuffPtr, SECTOR_SIZE);
	 assert(writeAmount == SECTOR_SIZE);
      }
      for(idx=0;idx<3;idx++) close(fd[idx]);
   }   
}


void writeXOR(unsigned char *fileXORBuff,
	      int EOFfound)
{
   static int fd, first=1;
   int writeAmount;
//
//open XOR file
   if(first)
   {
      if( (fd= open("raidFileXOR.bin", O_RDWR | O_CREAT, 00644))<0) perror("open");
      first=0;
   }
//
//write XOR file
//as long as EOF has not been found. 
//note, nothing extra needs to be written since EOF will always
//be on a sector boundary
   if(!EOFfound)
   {
      writeAmount=write(fd, fileXORBuff, SECTOR_SIZE);
      assert(writeAmount == SECTOR_SIZE);
   }

//
//close XOR file
//
   if(EOFfound) close(fd);

}


//
//read from the raid files
//
int readRaidFiles(unsigned char *fileBuffPtr)
{

   static int fd[4], first=1, idx1=0;
   int idx, readAmount, readSoFar, toRead;

//
//open raid files
//
   if(first)
   {
      if( (fd[0]= open("raidFile1.bin", O_RDWR | O_CREAT, 00644))<0) perror("open");
      if( (fd[1]= open("raidFile2.bin", O_RDWR | O_CREAT, 00644))<0) perror("open");
      if( (fd[2]= open("raidFile3.bin", O_RDWR | O_CREAT, 00644))<0) perror("open");
      if( (fd[3]= open("raidFile4.bin", O_RDWR | O_CREAT, 00644))<0) perror("open");
      first=0;
   }


//
//read raid files
//

   for(readAmount=0, readSoFar=0, toRead=SECTOR_SIZE; readSoFar<SECTOR_SIZE;)
   {
      readAmount=read(fd[idx1], &fileBuffPtr[readSoFar], toRead);
      if( readAmount==0)
      {
	 for(idx=0; idx<4; idx++) close(fd[idx]);
	 return readSoFar;
      }
      else
      {
	 toRead=toRead-readAmount;
	 readSoFar=readSoFar+readAmount;
      }
   }
   idx1=(idx1+1)%4;
   return readSoFar;
}	   

void writeOutputFile(unsigned char *fileBuffPtr,
		     int amountToWrite)
{
   static int fd, first=1;
   int writeAmount;

//
//open output file
//
   if(first)
   {
      if( (fd= open("raidFileOutput.bin", O_RDWR | O_CREAT, 00644))<0) perror("open");
      first=0;
   }
//
//write output file
//

   writeAmount=write(fd, fileBuffPtr, amountToWrite);
   assert(writeAmount == amountToWrite);
   if(amountToWrite < SECTOR_SIZE) close(fd);

}

int createXORStripe(unsigned char *fileBuffPtr)
{
   static int fd[4], first=1;
   int readAmount, idx, EOFfound=0;
   unsigned char XORBuff1[SECTOR_SIZE];
   unsigned char XORBuff2[SECTOR_SIZE];
   unsigned char XORBuff3[SECTOR_SIZE];
   unsigned char XORBuff4[SECTOR_SIZE];

   memset(XORBuff1, 0, sizeof(XORBuff1));
   memset(XORBuff2, 0, sizeof(XORBuff2));
   memset(XORBuff3, 0, sizeof(XORBuff3));
   memset(XORBuff4, 0, sizeof(XORBuff4));

   if(first)
   {
      if( (fd[0]= open("raidFile1.bin", O_RDWR | O_CREAT, 00644))<0) perror("open");
      if( (fd[1]= open("raidFile2.bin", O_RDWR | O_CREAT, 00644))<0) perror("open");
      if( (fd[2]= open("raidFile3.bin", O_RDWR | O_CREAT, 00644))<0) perror("open");
      if( (fd[3]= open("raidFile4.bin", O_RDWR | O_CREAT, 00644))<0) perror("open");
      first=0;
   }

   readAmount=read(fd[0], &XORBuff1[0], SECTOR_SIZE);
   if(readAmount<SECTOR_SIZE) EOFfound=1;
   
   readAmount=read(fd[1], &XORBuff2[0], SECTOR_SIZE);
   if(readAmount<SECTOR_SIZE) EOFfound=1;
   
   readAmount=read(fd[2], &XORBuff3[0], SECTOR_SIZE);
   if(readAmount<SECTOR_SIZE) EOFfound=1;  
   
   readAmount=read(fd[3], &XORBuff4[0], SECTOR_SIZE);
   if(readAmount<SECTOR_SIZE) EOFfound=1;
   
   
   for(idx=0; idx<SECTOR_SIZE; idx++)
   {
      *(fileBuffPtr+idx)=(XORBuff1[idx])^(XORBuff2[idx])^(XORBuff3[idx])^(XORBuff4[idx]);
   }
   
   if(EOFfound) return 1;
   else return 0;

}

int rebuildRaidStripe(unsigned char *fileBuffPtr)
{
   static int fd[4], first=1;
   int readAmount, idx, EOFfound=0;
   unsigned char XORBuff1[SECTOR_SIZE];
   unsigned char XORBuff2[SECTOR_SIZE];
   unsigned char XORBuff3[SECTOR_SIZE];
   unsigned char parityBuff[SECTOR_SIZE];
   unsigned char parityCheck;

   memset(XORBuff1, 0, sizeof(XORBuff1));
   memset(XORBuff2, 0, sizeof(XORBuff2));
   memset(XORBuff3, 0, sizeof(XORBuff3));
   memset(parityBuff, 0, sizeof(parityBuff));

   if(first)
   {
      if( (fd[0]= open("raidFile1.bin", O_RDWR | O_CREAT, 00644))<0) perror("open");
      if( (fd[1]= open("raidFile2.bin", O_RDWR | O_CREAT, 00644))<0) perror("open");
      if( (fd[2]= open("raidFile3.bin", O_RDWR | O_CREAT, 00644))<0) perror("open");
      if( (fd[3]= open("raidFileXOR.bin", O_RDWR | O_CREAT, 00644))<0) perror("open");
      first=0;
   }

   readAmount=read(fd[0], &XORBuff1[0], SECTOR_SIZE);
   if(readAmount<SECTOR_SIZE) EOFfound=1;
   
   readAmount=read(fd[1], &XORBuff2[0], SECTOR_SIZE);
   if(readAmount<SECTOR_SIZE) EOFfound=1;
   
   readAmount=read(fd[2], &XORBuff3[0], SECTOR_SIZE);
   if(readAmount<SECTOR_SIZE) EOFfound=1;  
   
   readAmount=read(fd[3], &parityBuff[0], SECTOR_SIZE);
   if(readAmount<SECTOR_SIZE) EOFfound=1;
   
   
   for(idx=0; idx<SECTOR_SIZE; idx++)
   {
      parityCheck=(XORBuff1[idx])^(XORBuff2[idx])^(XORBuff3[idx]);
      *(fileBuffPtr+idx)=(parityBuff[idx])^(parityCheck);

   }
   
   if(EOFfound) return 1;
   else return 0;

}



//
//READ FROM RAID FILES AND REBUILT FILE
//
void rebuildRaidFile(unsigned char *fileXORBuff,
	      int EOFfound)
{
   static int fd, first=1;
   int writeAmount;
//
//open rebuilt file
   if(first)
   {
      if( (fd= open("raidFileRebuilt.bin", O_RDWR | O_CREAT, 00644))<0) perror("open");
      first=0;
   }
//
//write rebuilt file
//only if EOF has not been found
//note nothing needs to be written after EOF
//has been found since EOF will always be on
//a sector boundary.
//
   if(!EOFfound)
   {
      writeAmount=write(fd, fileXORBuff, SECTOR_SIZE);
      assert(writeAmount == SECTOR_SIZE);
   }


//
//close rebuilt file
//
   if(EOFfound) close(fd);

}

int readRebuiltRaidFiles(unsigned char *fileBuffPtr)
{

   static int fd[4], first=1, idx1=0;
   int idx, readAmount, readSoFar, toRead;

//
//open raid files
//
   if(first)
   {
      if( (fd[0]= open("raidFile1.bin", O_RDWR | O_CREAT, 00644))<0) perror("open");
      if( (fd[1]= open("raidFile2.bin", O_RDWR | O_CREAT, 00644))<0) perror("open");
      if( (fd[2]= open("raidFile3.bin", O_RDWR | O_CREAT, 00644))<0) perror("open");
      if( (fd[3]= open("raidFileRebuilt.bin", O_RDWR | O_CREAT, 00644))<0) perror("open");
      first=0;
   }


//
//read raid files
//

   for(readAmount=0, readSoFar=0, toRead=SECTOR_SIZE; readSoFar<SECTOR_SIZE;)
   {
      readAmount=read(fd[idx1], &fileBuffPtr[readSoFar], toRead);
      if( readAmount==0)
      {
	 for(idx=0; idx<4; idx++) close(fd[idx]);
	 return readSoFar;
      }
      else
      {
	 toRead=toRead-readAmount;
	 readSoFar=readSoFar+readAmount;
      }
   }
   idx1=(idx1+1)%4;
   return readSoFar;
}	   

void writeRebuiltOutputFile(unsigned char *fileBuffPtr,
			   int amountToWrite)
{
   static int fd, first=1;
   int writeAmount;

//
//open output file
//
   if(first)
   {
      if( (fd= open("raidFileOutputRebuilt.bin", O_RDWR | O_CREAT, 00644))<0) perror("open");
      first=0;
   }
//
//write output file
//

   writeAmount=write(fd, fileBuffPtr, amountToWrite);
   assert(writeAmount == amountToWrite);
   if(amountToWrite < SECTOR_SIZE) close(fd);

}





// RAID-5 encoding
//
// This is 80% capacity with 1/5 LBAs used for parity.
//
// Only handles single faults.
//
// PRECONDITIONS:
// 1) LBA pointeres must have memory allocated for them externally
// 2) Blocks pointer to by LBAs are initialized with data
//
// POST-CONDITIONS:
// 1) Contents of PLBA is modified and contains the computed parity using XOR
//
void xorLBA(unsigned char *LBA1,
	    unsigned char *LBA2,
	    unsigned char *LBA3,
	    unsigned char *LBA4,
	    unsigned char *PLBA)
{
    int idx;

    for(idx=0; idx<SECTOR_SIZE; idx++)
        *(PLBA+idx)=(*(LBA1+idx))^(*(LBA2+idx))^(*(LBA3+idx))^(*(LBA4+idx));
}


// RAID-5 Rebuild
//
// Provide any 3 of the original LBAs and the Parity LBA to rebuild the RLBA
//
// If the Parity LBA was lost, then it can be rebuilt by simply re-encoding.
// 
void rebuildLBA(unsigned char *LBA1,
	        unsigned char *LBA2,
	        unsigned char *LBA3,
	        unsigned char *PLBA,
	        unsigned char *RLBA)
{
    int idx;
    unsigned char checkParity;

    for(idx=0; idx<SECTOR_SIZE; idx++)
    {
        // Parity check word is simply XOR of remaining good LBAs
        checkParity=(*(LBA1+idx))^(*(LBA2+idx))^(*(LBA3+idx));

        // Rebuilt LBA is simply XOR of original parity and parity check word
        // which will preserve original parity computed over the 4 LBAs
        *(RLBA+idx) =(*(PLBA+idx))^(checkParity);
    }
}


int checkEquivLBA(unsigned char *LBA1,
		  unsigned char *LBA2)
{
    int idx;

    for(idx=0; idx<SECTOR_SIZE; idx++)
    {
        if((*(LBA1+idx)) != (*(LBA2+idx)))
	{
            printf("EQUIV CHECK MISMATCH @ byte %d: LBA1=0x%x, LBA2=0x%x\n", idx, (*LBA1+idx), (*LBA2+idx));
	    return ERROR;
	}
    }

    return OK;
}
