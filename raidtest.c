#include <sys/time.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "raidlib.h"



int main(int argc, char *argv[])
{

	int amountRead;
	
	unsigned char fileBuff[SECTOR_SIZE];

	//
	//TEST CASE 3
	//CREATE AND USE FILES FOR RAID
	
	for(amountRead=SECTOR_SIZE; amountRead==SECTOR_SIZE;)
	{
		amountRead=readInput(&(fileBuff[0]));

		stripeRaidFiles(&(fileBuff[0]),
				amountRead);


		memset(fileBuff, 0, sizeof(fileBuff));

		// Consider alternating 1s and 0s instead of 0s for lower probability
	}

	//
	//END TEST CASE 3



	//TEST CASE 4
	//OUTPUT STRIPED FILES

	for(amountRead=SECTOR_SIZE; amountRead==SECTOR_SIZE;)
        {
                amountRead=readRaidFiles(&(fileBuff[0]));

                writeOutputFile(&(fileBuff[0]),
                                amountRead);


                memset(fileBuff, 0, sizeof(fileBuff));
        }
}
