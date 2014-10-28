#include<stdio.h>
#include"raidlib.h"
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>


#define TEST_RAID_STRING                                                                                                                                   "#0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ##0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ##0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ##0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ##0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ##0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ##0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ##0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ#"


//
//This will create a new file called raidFileInput with 4 sectors worth of binary data in it.
//This is for testing purposes
//



int main()
{
   unsigned char testLBA[SECTOR_SIZE];
   
   int fileDescriptor, idx;
   memcpy(&testLBA, TEST_RAID_STRING, SECTOR_SIZE);
   fileDescriptor=open("raidFileInput.bin", O_RDWR | O_CREAT, 00644);
   write(fileDescriptor, &testLBA[0], SECTOR_SIZE);
   write(fileDescriptor, &testLBA[0], SECTOR_SIZE);
   write(fileDescriptor, &testLBA[0], SECTOR_SIZE);
   write(fileDescriptor, &testLBA[0], SECTOR_SIZE);
   close(fileDescriptor);


}
