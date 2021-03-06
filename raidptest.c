#include <sys/time.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#ifdef RAID64
#include "raidlib64.h"
#define PTR_CAST (unsigned long long *)
#else
#include "raidlib.h"
#define PTR_CAST (unsigned char *)
#endif

#define TEST_ITERATIONS (1000)
#define MAX_LBAS (1000)

#define TEST_RAID_STRING "#0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ##0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ##0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ##0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ##0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ##0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ##0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ##0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ#"

#define NULL_RAID_STRING "#FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF#"



static unsigned char testRebuild[MAX_LBAS][SECTOR_SIZE];
static unsigned char testLBA1[MAX_LBAS][SECTOR_SIZE];
static unsigned char testLBA2[MAX_LBAS][SECTOR_SIZE];
static unsigned char testLBA3[MAX_LBAS][SECTOR_SIZE];
static unsigned char testLBA4[MAX_LBAS][SECTOR_SIZE];

#if 0
static unsigned char file1Buff[SECTOR_SIZE];
static unsigned char file2Buff[SECTOR_SIZE];
static unsigned char file3Buff[SECTOR_SIZE];
static unsigned char file4Buff[SECTOR_SIZE];
static unsigned char fileXORBuff[SECTOR_SIZE];
static unsigned char file4Rebuild[SECTOR_SIZE];
#endif

char testPLBA[MAX_LBAS][SECTOR_SIZE];


void modifyBuffer(unsigned char *bufferToModify)
{
    int idx;

    for(idx=0; idx < SECTOR_SIZE; idx++)
        bufferToModify[idx] = (bufferToModify[idx]+1) % 100;
}

#if 0
void printBuffer(char *bufferToPrint)
{
    int idx;

    for(idx=0; idx < SECTOR_SIZE; idx++)
        printf("%c ", bufferToPrint[idx]);

    printf("\n");
}
#endif

void dumpBuffer(unsigned char *bufferToDump)
{
    int idx;

    for(idx=0; idx < SECTOR_SIZE; idx++)
        printf("%x ", bufferToDump[idx]);

    printf("\n");
}



int main(int argc, char *argv[])
{
	int idx, LBAidx, numTestIterations, rc, fd[7], EOFfound;
	double rate=0.0;
	double totalRate=0.0, aveRate=0.0;
	struct timeval StartTime, StopTime;
	unsigned int microsecs;
	unsigned char file1Buff[SECTOR_SIZE];
	unsigned char file2Buff[SECTOR_SIZE];
	unsigned char file3Buff[SECTOR_SIZE];
	unsigned char file4Buff[SECTOR_SIZE];
	unsigned char fileXORBuff[SECTOR_SIZE];
        unsigned char file4Rebuild[SECTOR_SIZE];


        if(argc < 2)
	{
		numTestIterations=TEST_ITERATIONS;
		printf("Will default to %d iterations\n", TEST_ITERATIONS);
        }
        else
        {
            sscanf(argv[1], "%d", &numTestIterations);
       	    printf("Will start %d test iterations\n", numTestIterations);
        }


        // Set all test buffers
	for(idx=0;idx<MAX_LBAS;idx++)
        {
            memcpy(&testLBA1[idx], TEST_RAID_STRING, SECTOR_SIZE);
            memcpy(&testLBA2[idx], TEST_RAID_STRING, SECTOR_SIZE);
            memcpy(&testLBA3[idx], TEST_RAID_STRING, SECTOR_SIZE);
            memcpy(&testLBA4[idx], TEST_RAID_STRING, SECTOR_SIZE);
            memcpy(&testRebuild[idx], NULL_RAID_STRING, SECTOR_SIZE);
        }
            
#if 0
        // TEST CASE #0
        //
        printf("Architecture validation:\n");
        printf("sizeof(unsigned long long)=%d\n", sizeof(unsigned long long));
        printf("\n");
	// Compute XOR from 4 LBAs for RAID-5
        xorLBA(PTR_CAST &testLBA1[0],
	       PTR_CAST &testLBA2[0],
	       PTR_CAST &testLBA3[0],
    	       PTR_CAST &testLBA4[0],
	       PTR_CAST &testPLBA[0]);

	// Now rebuild LBA into test to verify
        rebuildLBA(PTR_CAST &testLBA1[0],
	           PTR_CAST &testLBA2[0],
	           PTR_CAST &testLBA3[0],
	           PTR_CAST &testPLBA[0],
	           PTR_CAST &testRebuild[0]);
printf("position 1\n");
        printBuffer((char *)&testLBA4[0]);
        getchar();
printf("position 2\n");
        printBuffer((char *)&testRebuild[0]);
        getchar();

        assert(memcmp(testRebuild, testLBA4, SECTOR_SIZE) ==0);


        // TEST CASE #1
        //
        //
        // This test case computes the XOR parity from 4 test buffers (512 bytes each) and then
        // it rebuilds a missing buffer (buffer 4 in the test case) and we simple time how long this
        // takes.
        //
	printf("\nRAID Operations Performance Test\n");

	gettimeofday(&StartTime, 0);

	for(idx=0;idx<numTestIterations;idx++)
	{
            LBAidx = idx % MAX_LBAS;

	    // Compute XOR from 4 LBAs for RAID-5
            xorLBA(PTR_CAST &testLBA1[LBAidx],
	           PTR_CAST &testLBA2[LBAidx],
	           PTR_CAST &testLBA3[LBAidx],
    	           PTR_CAST &testLBA4[LBAidx],
	           PTR_CAST &testPLBA[LBAidx]);

	    // Now rebuild LBA into test to verify
            rebuildLBA(PTR_CAST &testLBA1[LBAidx],
	               PTR_CAST &testLBA2[LBAidx],
	               PTR_CAST &testLBA3[LBAidx],
	               PTR_CAST &testPLBA[LBAidx],
	               PTR_CAST &testRebuild[LBAidx]);
	}

	gettimeofday(&StopTime, 0);

        microsecs=((StopTime.tv_sec - StartTime.tv_sec)*1000000);

	if(StopTime.tv_usec > StartTime.tv_usec)
		microsecs+=(StopTime.tv_usec - StartTime.tv_usec);
	else
		microsecs-=(StartTime.tv_usec - StopTime.tv_usec);

	printf("Test Done in %u microsecs for %d iterations\n", microsecs, numTestIterations);

	rate=((double)numTestIterations)/(((double)microsecs)/1000000.0);
	printf("%lf RAID ops computed per second\n", rate);
        //
        // END TEST CASE #1


        // TEST CASE #2
        //
        // Verify that the rebuild actually works and passes a data compare for N unique cases with different data.
        //

	for(idx=0;idx<numTestIterations; idx++)
	{
            LBAidx = idx % MAX_LBAS;

	    // Compute XOR from 4 LBAs for RAID-5
            xorLBA(PTR_CAST &testLBA1[LBAidx],
	           PTR_CAST &testLBA2[LBAidx],
	           PTR_CAST &testLBA3[LBAidx],
    	           PTR_CAST &testLBA4[LBAidx],
	           PTR_CAST &testPLBA[LBAidx]);

	    // Now rebuild LBA into test to verify
            rebuildLBA(PTR_CAST &testLBA1[LBAidx],
	               PTR_CAST &testLBA2[LBAidx],
	               PTR_CAST &testLBA3[LBAidx],
	               PTR_CAST &testPLBA[LBAidx],
	               PTR_CAST &testRebuild[LBAidx]);


            // Compare test to see if testRebuild is in fact the same as testLBA4
            //
            // check for miscompare
            assert(memcmp(&testRebuild[LBAidx], &testLBA4[LBAidx], SECTOR_SIZE) ==0);
            //printf("%d ", idx);


            // For the next case modify the contents of testLBA4 each time
            //
            modifyBuffer(&(testLBA4[LBAidx][0]));

	}
        printf("\n");

        //
        // END TEST CASE #2
#endif
      






      //TEST CASE #3
      //
      //CREATE AND USE FILES FOR RAID
      //
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
	 
	 memset(file1Buff,0, sizeof(file1Buff));
	 memset(file2Buff,0, sizeof(file2Buff));
	 memset(file3Buff,0, sizeof(file3Buff));
	 memset(file4Buff,0, sizeof(file4Buff));
      }

      //END TEST CASE #3
      //
      //
#if 0

      //
      //TEST CASE #4
      //

   readRaidFiles(&(file1Buff[0]),
                 &(file2Buff[0]),
                 &(file3Buff[0]),
	         &(file4Buff[0]));


   xorLBA(file1Buff,
          file2Buff,
          file3Buff,
          file4Buff,
	  fileXORBuff);

   writeXOR(&(fileXORBuff[0]));

   //
   //END TEST CASE #4
   //




//
//TESTCASE #5
//

   rebuildLBA(&(file1Buff[0]),
	      &(file2Buff[0]),
	      &(file3Buff[0]),
	      &(fileXORBuff[0]),
	      &(file4Rebuild[0]));




//
//END TEST CASE #5
//




//
//TEST CASE #6
//
   readRaidFiles(&(file1Buff[0]),
                 &(file2Buff[0]),
                 &(file3Buff[0]),
                 &(file4Buff[0]));


                    
   writeOutputFile(&(file1Buff[0]),
                   &(file2Buff[0]),
                   &(file3Buff[0]),
                   &(file4Buff[0]));
      //
      //END TEST CASE #6
      //

#endif





}
