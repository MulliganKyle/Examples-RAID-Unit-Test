#include <sys/time.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "raidlib.h"



int main(int argc, char *argv[])
{

	int EOFfound, amountRead, sectorsReadIn=0, extraBytesToRead=0, sectorsReadOut;
	
	unsigned char fileBuff[SECTOR_SIZE];


   if(argc<2)
   {
      printf("ERROR usage of this program requires a file to be input. EX: raidtest Baby-Musk-Ox.ppm\n");
      return -1;
   }
   else
   {
      printf("will use %s for raidimplementation.\n", argv[1]);
   }




	//
	//TEST CASE 3
	//CREATE AND USE FILES FOR RAID
	
	for(amountRead=SECTOR_SIZE; amountRead==SECTOR_SIZE;)
	{
		amountRead=readInput(&(fileBuff[0]),
				     argv[1]);

		stripeRaidFiles(&(fileBuff[0]),
				amountRead);
	       
	       //
	       //adds one to the sectors read in only if a full sector was read.
	       if(amountRead==SECTOR_SIZE)
	       {
		  sectorsReadIn++;
	       }
	       //
	       //saves the extra amount of bytes that were read after the last full sector
	       else
	       {
		  extraBytesToRead=amountRead;
	       }
		memset(fileBuff, 0, sizeof(fileBuff));

		// Consider alternating 1s and 0s instead of 0s for lower probability
	}

	//
	//END TEST CASE 3



	//TEST CASE 4
	//OUTPUT STRIPED FILES

	for(sectorsReadOut=0; sectorsReadOut<sectorsReadIn;)
        {

		amountRead=readRaidFiles(&(fileBuff[0]));

                writeOutputFile(&(fileBuff[0]),
                                amountRead);
	       sectorsReadOut++;

	       //
	       //reads and writes the extra bytes after the last full sector that needs to be read
	       if(sectorsReadOut==sectorsReadIn)
	       {
		  readRaidFiles(&(fileBuff[0]));
		  writeOutputFile(&(fileBuff[0]),
				  extraBytesToRead);
	       }

                memset(fileBuff, 0, sizeof(fileBuff));
        }

	 //TEST CASE 5
	 //XOR STRIPED FILES
	 for(EOFfound=0; !EOFfound;)
	 {
	    EOFfound=createXORStripe(&(fileBuff[0]));
	    
	    writeXOR(&(fileBuff[0]),
		     EOFfound);

	    memset(fileBuff, 0, sizeof(fileBuff));
	 }
	 //
	 //END TEST CASE 5


	 //TEST CASE 6
	 //CREATE REBUILT RAID FILE 4
	 for(EOFfound=0; !EOFfound;)
	 {
	    EOFfound=rebuildRaidStripe(&(fileBuff[0]));
	    
	    rebuildRaidFile(&(fileBuff[0]),
			    EOFfound);

	    memset(fileBuff, 0, sizeof(fileBuff));
	 }
	 //
	 //END TEST CASE 6


	 //TEST CASE 7
	 //CREATE OUTPUT FILE USING REBUILT FILE 4
	 for(sectorsReadOut=0; sectorsReadOut<sectorsReadIn;)
	 {
		amountRead=readRebuiltRaidFiles(&(fileBuff[0]));

                writeRebuiltOutputFile(&(fileBuff[0]),
                                amountRead);
	       sectorsReadOut++;

	       //
	       //reads and writes the extra bytes after the last full sector that needs to be read
	       if(sectorsReadOut==sectorsReadIn)
	       {
		  readRebuiltRaidFiles(&(fileBuff[0]));
		  writeRebuiltOutputFile(&(fileBuff[0]),
				         extraBytesToRead);
	       }

                memset(fileBuff, 0, sizeof(fileBuff));
        }
	//
	//END TEST CASE 6




}
