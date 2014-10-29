#ifndef RAIDLIB_H
#define RAIDLIB_H

#define OK (0)
#define ERROR (-1)
#define TRUE (1)
#define FALSE (0)

#define SECTOR_SIZE (512)

void openFiles(int fd[]);

void closeFiles(int fd[]);

void readInput(unsigned char *file1Buff,
               unsigned char *file2Buff,
               unsigned char *file3Buff,
               unsigned char *file4Buff,
               int fd[]);


void stripeRaidFiles(unsigned char *file1Buff,
                     unsigned char *file2Buff,
                     unsigned char *file3Buff,
                     unsigned char *file4Buff,
                     int fd[]);

void writeXOR(unsigned char *fileXORBuff,int fd[]);


void readRaidFiles(unsigned char *file1Buff,
                   unsigned char *file2Buff,
                   unsigned char *file3Buff,
                   unsigned char *file4Buff,
                   unsigned char *fileXORBuff,
                   int fd[]);

void writeOutputFile(unsigned char *file1Buff,
                     unsigned char *file2Buff,
                     unsigned char *file3Buff,
                     unsigned char *file4Buff,
                     int fd[]);





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
