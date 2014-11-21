#include <sys/time.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "raidlib.h"



int main(int argc, char *argv[])
{

	int fd[7], EOFfound;
	
	unsigned char file1Buff[SECTOR_SIZE];
	unsigned char file2Buff[SECTOR_SIZE];
	unsigned char file3Buff[SECTOR_SIZE];
	unsigned char file4Buff[SECTOR_SIZE];
	unsigned char fileXORBuff[SECTOR_SIZE];
	unsigned char file4Rebuild[SECTOR_SIZE];

	//
	//TEST CASE 3
	//CREATE AND USE FILES FOR RAID
	
	for(EOFfound=0;!EOFfound;)
	{
		EOFfound=readInput(&(file1Buff[0]),
				   &(file2Buff[0]),
				   &(file3Buff[0]),
				   &(file4Buff[0]));

		stripeRaidFiles(&(file1Buff[0]),
                                &(file2Buff[0]),
                                &(file3Buff[0]),
                                &(file4Buff[0]),
				EOFfound);


		memset(file1Buff, 0, sizeof(file1Buff));
		memset(file2Buff, 0, sizeof(file2Buff));
		memset(file3Buff, 0, sizeof(file3Buff));
		memset(file4Buff, 0, sizeof(file4Buff));
	}

	//
	//END TEST CASE 3
	
#if 0
	//TEST CASE 4
	//OUTPUT STRIPED FILES

	for(EOFfound=0;!EOFfound;)
        {
                EOFfound=readInput(&(file1Buff[0]),
                                   &(file2Buff[0]),
                                   &(file3Buff[0]),
                                   &(file4Buff[0]));

                writeOutputFile(&(file1Buff[0]),
                                &(file2Buff[0]),
                                &(file3Buff[0]),
                                &(file4Buff[0]),
                                EOFfound);


                memset(file1Buff, 0, sizeof(file1Buff));
                memset(file2Buff, 0, sizeof(file2Buff));
                memset(file3Buff, 0, sizeof(file3Buff));
                memset(file4Buff, 0, sizeof(file4Buff));
        }
#endif 
}
