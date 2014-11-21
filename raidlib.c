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
int  readInput(unsigned char *file1Buff,
	       unsigned char *file2Buff,
	       unsigned char *file3Buff,
      	       unsigned char *file4Buff)
{
  static int fd, first=1;
  int readAmmount, readSoFar, toRead;

//
//open input file
//
   if( first)
   {
      if( (fd= open("raidFileInput.bin", O_RDWR | O_CREAT, 00644))<0)  perror("open");
      first=0;
   }

//
//read one sector at a time from the input file into buffers
//

   for( readAmmount=0, readSoFar=0, toRead=SECTOR_SIZE; readSoFar<SECTOR_SIZE;)
   {
      readAmmount=read(fd, &file1Buff[readSoFar], toRead);
      if( readAmmount==0)
      {
	 close(fd);
	 return 1;
      }
      else
      {
	 toRead=toRead-readAmmount;
         readSoFar=readSoFar+readAmmount;
      }

   }
   for( readAmmount=0, readSoFar=0, toRead=SECTOR_SIZE; readSoFar<SECTOR_SIZE;)
   {
      readAmmount=read(fd, &file2Buff[readSoFar], toRead);
      if( readAmmount==0)
      {
	 close(fd);
	 return 1;
      }
      else
      {
	 toRead=toRead-readAmmount;
	 readSoFar=readSoFar+readAmmount;
      }
   }

   for( readAmmount=0, readSoFar=0, toRead=SECTOR_SIZE; readSoFar<SECTOR_SIZE;)
   {
      readAmmount=read(fd, &file3Buff[readSoFar], toRead);
      if( readAmmount==0)
      {
	 close(fd);
	 return 1;
      }
      else
      {
	 toRead=toRead-readAmmount;
	 readSoFar=readSoFar+readAmmount;
      }
   }

   for( readAmmount=0, readSoFar=0, toRead=SECTOR_SIZE; readSoFar<SECTOR_SIZE;)
   {
      readAmmount=read(fd, &file4Buff[readSoFar], toRead);
      if( readAmmount==0)
      {
	 close(fd);
	 return 1;
      }
      else
      {
	 toRead=toRead-readAmmount;
	 readSoFar=readSoFar+readAmmount;
      }
   }
   return 0;

}

void stripeRaidFiles(unsigned char *file1BuffPtr,
		     unsigned char *file2BuffPtr,
		     unsigned char *file3BuffPtr,
		     unsigned char *file4BuffPtr,
		     int EOFfound)
{

   static int fd[4], first=1;
   int writeAmmount, idx;

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
   writeAmmount=write(fd[0], file1BuffPtr, SECTOR_SIZE);
   assert(writeAmmount == SECTOR_SIZE);

   writeAmmount=write(fd[1], file2BuffPtr, SECTOR_SIZE);
   assert(writeAmmount == SECTOR_SIZE);
   
   writeAmmount=write(fd[2], file3BuffPtr, SECTOR_SIZE);
   assert(writeAmmount == SECTOR_SIZE);
   //printf("[]\n");
  // printBuffer((char*) file3BuffPtr);
  //printBuffer((char*) file4BuffPtr); 
   writeAmmount=write(fd[3], file4BuffPtr, SECTOR_SIZE);
   if(writeAmmount<0) printf("\n%d\n, %s\n",errno, strerror(errno));
   assert(writeAmmount == SECTOR_SIZE);
//
//close raid files
//
   if(EOFfound) for(idx=0; idx<3;idx++) close(fd[idx]);
   
}


void writeXOR(unsigned char *fileXORBuff)
{
   int fd, writeAmmount;
//
//open XOR file

   if( (fd= open("raidFileXOR.bin", O_RDWR | O_CREAT, 00644))<0) perror("open");

//
//write XOR file
//
   writeAmmount=write(fd, fileXORBuff, SECTOR_SIZE);
   assert(writeAmmount == SECTOR_SIZE);

//
//close XOR file
//
   close(fd);

}


//
//read from the raid files
//
void readRaidFiles(unsigned char *file1BuffPtr,
		   unsigned char *file2BuffPtr,
		   unsigned char *file3BuffPtr,
		   unsigned char *file4BuffPtr)
{

   static int fd[3];
   int idx, readAmmount, readSoFar, toRead;

//
//open raid files
//
   if( (fd[0]= open("raidFile1.bin", O_RDWR | O_CREAT, 00644))<0) perror("open");
   if( (fd[1]= open("raidFile2.bin", O_RDWR | O_CREAT, 00644))<0) perror("open");
   if( (fd[2]= open("raidFile3.bin", O_RDWR | O_CREAT, 00644))<0) perror("open");
   if( (fd[3]= open("raidFile4.bin", O_RDWR | O_CREAT, 00644))<0) perror("open");



//
//read raid files
//
   for( readAmmount=0, readSoFar=0, toRead=SECTOR_SIZE; readSoFar<SECTOR_SIZE;)
   {
      readAmmount=read(fd[0], &file1BuffPtr[readSoFar], toRead);
      toRead=toRead-readAmmount;
      readSoFar=readSoFar+readAmmount;

   }
   for( readAmmount=0, readSoFar=0, toRead=SECTOR_SIZE; readSoFar<SECTOR_SIZE;)
   {
      readAmmount=read(fd[1], &file2BuffPtr[readSoFar], toRead);
      toRead=toRead-readAmmount;
      readSoFar=readSoFar+readAmmount;
   }

   for( readAmmount=0, readSoFar=0, toRead=SECTOR_SIZE; readSoFar<SECTOR_SIZE;)
   {
      readAmmount=read(fd[2], &file3BuffPtr[readSoFar], toRead);
      toRead=toRead-readAmmount;
      readSoFar=readSoFar+readAmmount;
   }

   for( readAmmount=0, readSoFar=0, toRead=SECTOR_SIZE; readSoFar<SECTOR_SIZE;)
   {
      readAmmount=read(fd[3], &file4BuffPtr[readSoFar], toRead);
      toRead=toRead-readAmmount;
      readSoFar=readSoFar+readAmmount;
   }

//
//close raid files
//
   for(idx=0; idx<3;idx++) close(fd[idx]);

}	   

void writeOutputFile(unsigned char *file1BuffPtr,
                     unsigned char *file2BuffPtr,
                     unsigned char *file3BuffPtr,
                     unsigned char *file4BuffPtr,
		     int EOFfound)
{
   static int fd;
   int writeAmmount;

//
//open output file
//
   if( (fd= open("raidFileOutput.bin", O_RDWR | O_CREAT, 00644))<0) perror("open");

//
//write output file
//
   writeAmmount=write(fd, file1BuffPtr, SECTOR_SIZE);
   assert(writeAmmount == SECTOR_SIZE);

   writeAmmount=write(fd, file2BuffPtr, SECTOR_SIZE);
   assert(writeAmmount == SECTOR_SIZE);

   writeAmmount=write(fd, file3BuffPtr, SECTOR_SIZE);
   assert(writeAmmount == SECTOR_SIZE);

   writeAmmount=write(fd, file4BuffPtr, SECTOR_SIZE);
   assert(writeAmmount == SECTOR_SIZE);

//
//close output file
//
	if(EOFfound) close(fd);

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
