#include <sys/time.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>


#include "raidlib.h"




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
void readInput(unsigned char file1Buff[],
	       unsigned char file2Buff[],
	       unsigned char file3Buff[],
      	       unsigned char file4Buff[],
	       int fd[])
{
   int readAmmount;
//
//read one sector at a time from the input file into buffers
//

   readAmmount=read(fd[0], &file1Buff, SECTOR_SIZE);
   //assert(readAmmount == SECTOR_SIZE);
   
   readAmmount=read(fd[0], &file2Buff, SECTOR_SIZE);
   //assert(readAmmount == SECTOR_SIZE);
   
   readAmmount=read(fd[0], &file3Buff, SECTOR_SIZE);
   //assert(readAmmount == SECTOR_SIZE);

   readAmmount=read(fd[0], &file4Buff, SECTOR_SIZE);
   //assert(readAmmount == SECTOR_SIZE);
}

void stripeRaidFiles(unsigned char *file1Buff,
		     unsigned char *file2Buff,
		     unsigned char *file3Buff,
		     unsigned char *file4Buff,
		     int fd[])
{
//
//write the buffers to the files 1 thru 4
//

   int writeAmmount;

   writeAmmount=write(fd[1], &file1Buff, SECTOR_SIZE);
   assert(writeAmmount == SECTOR_SIZE);

   writeAmmount=write(fd[2], &file2Buff, SECTOR_SIZE);
   assert(writeAmmount == SECTOR_SIZE);
   
   writeAmmount=write(fd[3], &file3Buff, SECTOR_SIZE);
   assert(writeAmmount == SECTOR_SIZE);
   
   writeAmmount=write(fd[4], &file4Buff, SECTOR_SIZE);
   assert(writeAmmount == SECTOR_SIZE);
}

//
//write to the XOR file
//
void writeXOR(unsigned char *fileXORBuff,int fd[])
{
   int writeAmmount;

   writeAmmount=write(fd[5], &fileXORBuff, SECTOR_SIZE);
   assert(writeAmmount == SECTOR_SIZE);

}


//
//read from the raid files and the XOR file
//
void readRaidFiles(unsigned char *file1Buff,
		   unsigned char *file2Buff,
		   unsigned char *file3Buff,
		   unsigned char *file4Buff,
		   unsigned char *fileXORBuff,
		   int fd[])
{

   int readAmmount;

   readAmmount=read(fd[1], &file1Buff, SECTOR_SIZE);
   assert(readAmmount == SECTOR_SIZE);

   readAmmount=read(fd[2], &file2Buff, SECTOR_SIZE);
   assert(readAmmount == SECTOR_SIZE);

   readAmmount=read(fd[3], &file3Buff, SECTOR_SIZE);
   assert(readAmmount == SECTOR_SIZE);

   readAmmount=read(fd[4], &file4Buff, SECTOR_SIZE);
   assert(readAmmount == SECTOR_SIZE);

   readAmmount=read(fd[5], &fileXORBuff, SECTOR_SIZE);
//   assert(readAmmount == SECTOR_SIZE);
}	   

void writeOutputFile(unsigned char *file1Buff,
                     unsigned char *file2Buff,
                     unsigned char *file3Buff,
                     unsigned char *file4Buff,
		     int fd[])
{
   int writeAmmount;

   writeAmmount=write(fd[6], &file1Buff, SECTOR_SIZE);
   assert(writeAmmount == SECTOR_SIZE);

   writeAmmount=write(fd[6], &file2Buff, SECTOR_SIZE);
   assert(writeAmmount == SECTOR_SIZE);

   writeAmmount=write(fd[6], &file3Buff, SECTOR_SIZE);
   assert(writeAmmount == SECTOR_SIZE);

   writeAmmount=write(fd[6], &file4Buff, SECTOR_SIZE);
   assert(writeAmmount == SECTOR_SIZE);

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
