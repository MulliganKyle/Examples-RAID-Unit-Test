#ifndef RAIDLIB_H
#define RAIDLIB_H

#define OK (0)
#define ERROR (-1)
#define TRUE (1)
#define FALSE (0)

#define SECTOR_SIZE (512)



void printBuffer(char *bufferToPrint);


int  readInput(unsigned char *fileBuff);


void stripeRaidFiles(unsigned char *fileBuff,
		     int amountToWrite);

void writeXOR(unsigned char *fileXORBuff,
	      int EOFfound);


int  readRaidFiles(unsigned char *fileBuff);

void writeOutputFile(unsigned char *fileBuff,
		     int amountToWrite);

int createXORStripe(unsigned char *fileBuff);

int rebuildRaidStripe(unsigned char *fileBuff);

void rebuildRaidFile(unsigned char *fileBuff,
		     int EOFfound);



void xorLBA(unsigned char *LBA1,
	    unsigned char *LBA2,
	    unsigned char *LBA3,
	    unsigned char *LBA4,
	    unsigned char *PLBA);

void rebuildLBA(unsigned char *LBA1,
	        unsigned char *LBA2,
	        unsigned char *LBA3,
	        unsigned char *PLBA,
	        unsigned char *RLBA);


int checkEquivLBA(unsigned char *LBA1,
		  unsigned char *LBA2);

#endif
