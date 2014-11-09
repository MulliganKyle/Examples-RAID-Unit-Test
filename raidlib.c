#include <sys/time.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>



#include "raidlib.h"


void printBuffer(char *bufferToPrint)
{
   int idx;

   for(idx=0; idx < SECTOR_SIZE; idx++)
   printf("%c ", bufferToPrint[idx]);

   printf("\n");
}
		    





//
// open the files used for RAID
//
void openFiles(int fd[])
{
   fd[0]= open("raidFileInput.bin", O_RDWR | O_CREAT, 00644);
   fd[1]= open("raidFile1.bin", O_RDWR | O_CREAT, 00644);
   fd[2]= open("raidFile2.bin", O_RDWR | O_CREAT, 00644);
   fd[3]= open("raidFile3.bin", O_RDWR | O_CREAT, 00644);
   fd[4]= open("raidFile4.bin", O_RDWR | O_CREAT, 00644);
   fd[5]= open("raidFileXOR.bin", O_RDWR | O_CREAT, 00644);
   fd[6]= open("raidFileOutput.bin", O_RDWR | O_CREAT, 00644);
   fd[7]= open("raidFileReconstruct.bin", O_RDWR | O_CREAT, 00644);
}

//
//close the files used for RAID
//
void closeFiles(int fd[])
{
   int idx;
   for(idx=0; idx<8;idx++) close(fd[idx]);
}




//
//read the input file and stripe across 4 files
//
void readInput(unsigned char *file1Buff,
	       unsigned char *file2Buff,
	       unsigned char *file3Buff,
      	       unsigned char *file4Buff)
{
   int fd, readAmmount, readSoFar, toRead;

//
//open input file
//
   if( (fd= open("raidFileInput.bin", O_RDWR | O_CREAT, 00644))<0)  perror("open");


//
//read one sector at a time from the input file into buffers
//

   for( readAmmount=0, readSoFar=0, toRead=SECTOR_SIZE; readSoFar<SECTOR_SIZE;)
   {
      readAmmount=read(fd, &file1Buff[readSoFar], toRead);
      toRead=toRead-readAmmount;
      readSoFar=readSoFar+readAmmount;

   }
   for( readAmmount=0, readSoFar=0, toRead=SECTOR_SIZE; readSoFar<SECTOR_SIZE;)
   {
      readAmmount=read(fd, &file2Buff[readSoFar], toRead);
      toRead=toRead-readAmmount;
      readSoFar=readSoFar+readAmmount;
   }

   for( readAmmount=0, readSoFar=0, toRead=SECTOR_SIZE; readSoFar<SECTOR_SIZE;)
   {
      readAmmount=read(fd, &file3Buff[readSoFar], toRead);
      toRead=toRead-readAmmount;
      readSoFar=readSoFar+readAmmount;
   }

   for( readAmmount=0, readSoFar=0, toRead=SECTOR_SIZE; readSoFar<SECTOR_SIZE;)
   {
     readAmmount=read(fd, &file4Buff[readSoFar], toRead);
     toRead=toRead-readAmmount;
     readSoFar=readSoFar+readAmmount;
   }
//   readAmmount=read(fd, &file2Buff, SECTOR_SIZE);
//   assert(readAmmount == SECTOR_SIZE);
   
//   readAmmount=read(fd, &file3Buff, SECTOR_SIZE);
//   assert(readAmmount == SECTOR_SIZE);

//   readAmmount=read(fd, &file4Buff, SECTOR_SIZE);
//   assert(readAmmount == SECTOR_SIZE);

//
//close input file
//
   close(fd);
}

void stripeRaidFiles(unsigned char *file1Buff,
		     unsigned char *file2Buff,
		     unsigned char *file3Buff,
		     unsigned char *file4Buff)
{

   int fd[3], writeAmmount, idx;

//
//open raid files
//
   if( (fd[0]= open("raidFile1.bin", O_RDWR | O_CREAT, 00644))<0) perror("open");
   if( (fd[1]= open("raidFile2.bin", O_RDWR | O_CREAT, 00644))<0) perror("open");
   if( (fd[2]= open("raidFile3.bin", O_RDWR | O_CREAT, 00644))<0) perror("open");
   if( (fd[3]= open("raidFile4.bin", O_RDWR | O_CREAT, 00644))<0) perror("open");



//
//write the buffers to the files 1 thru 4
//
   writeAmmount=write(fd[0], &file1Buff, SECTOR_SIZE);
   assert(writeAmmount == SECTOR_SIZE);

   writeAmmount=write(fd[1], &file2Buff, SECTOR_SIZE);
   assert(writeAmmount == SECTOR_SIZE);
   
   writeAmmount=write(fd[2], &file3Buff, SECTOR_SIZE);
   assert(writeAmmount == SECTOR_SIZE);
   
   writeAmmount=write(fd[3], &file4Buff, SECTOR_SIZE);
   assert(writeAmmount == SECTOR_SIZE);
//
//close raid files
//
   for(idx=0; idx<3;idx++) close(fd[idx]);
   
}


void writeXOR(unsigned char *fileXORBuff)
{
   int fd, writeAmmount;
//
//open XOR file
//
   fd= open("raidFileXOR.bin", O_RDWR | O_CREAT, 00644);

//
//write XOR file
//
   writeAmmount=write(fd, &fileXORBuff, SECTOR_SIZE);
   assert(writeAmmount == SECTOR_SIZE);

//
//close XOR file
//
   close(fd);

}


//
//read from the raid files and the XOR file
//
void readRaidFiles(unsigned char *file1Buff,
		   unsigned char *file2Buff,
		   unsigned char *file3Buff,
		   unsigned char *file4Buff)
{

   int fd[3], idx, readAmmount;

//
//open raid files
//
   fd[0]= open("raidFile1.bin", O_RDWR | O_CREAT, 00644);
   fd[1]= open("raidFile2.bin", O_RDWR | O_CREAT, 00644);
   fd[2]= open("raidFile3.bin", O_RDWR | O_CREAT, 00644);
   fd[3]= open("raidFile4.bin", O_RDWR | O_CREAT, 00644);

//
//read raid files
//
   readAmmount=read(fd[0], &file1Buff, SECTOR_SIZE);
   assert(readAmmount == SECTOR_SIZE);

   readAmmount=read(fd[1], &file2Buff, SECTOR_SIZE);
   assert(readAmmount == SECTOR_SIZE);

   readAmmount=read(fd[2], &file3Buff, SECTOR_SIZE);
   assert(readAmmount == SECTOR_SIZE);

   readAmmount=read(fd[3], &file4Buff, SECTOR_SIZE);
   assert(readAmmount == SECTOR_SIZE);

//
//close raid files
//
   for(idx=0; idx<3;idx++) close(fd[idx]);

}	   

void writeOutputFile(unsigned char *file1Buff,
                     unsigned char *file2Buff,
                     unsigned char *file3Buff,
                     unsigned char *file4Buff)
{
   int fd, writeAmmount;

//
//open output file
//
   fd= open("raidFileOutput.bin", O_RDWR | O_CREAT, 00644);

//
//write output file
//
   writeAmmount=write(fd, &file1Buff, SECTOR_SIZE);
   assert(writeAmmount == SECTOR_SIZE);

   writeAmmount=write(fd, &file2Buff, SECTOR_SIZE);
   assert(writeAmmount == SECTOR_SIZE);

   writeAmmount=write(fd, &file3Buff, SECTOR_SIZE);
   assert(writeAmmount == SECTOR_SIZE);

   writeAmmount=write(fd, &file4Buff, SECTOR_SIZE);
   assert(writeAmmount == SECTOR_SIZE);

//
//close output file
//
   close(fd);

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
