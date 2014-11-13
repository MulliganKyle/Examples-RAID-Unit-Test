#include "raidLibrary01.h"



void main()
{
if xorLBA(/*put some files in here to create XOR encoded parity*/)
    printf("XORencode was successful");

if mirror (/*put some files in here to create mirrored files*/)
    printf("mirror was seuccessful");

if readInput(/*put location of file to be read, in here*/)
    printf("read was successful");

if writeToBuffer(/*put location of file to be written to and the contents of the file being written, in here*/)
    printf("writeToBuffer was successful");

if rebuildLBA(/*location of file being rebuilt, file(s) being used for rebuild, and method being used for rebuild*/)
    printf("rebuild was successful");

if readDuringRebuild(/*put location of file to be read, in here*/)
    printf("readDuringRebuild was successful");

if writeBufferToFile(/*no input required because files to be written are in the buffer*/)
    printf("writeBufferToFile was successful");


//timer start
xorLBA()
//timer stop
//compute IOPS
if (/*computed Iops > required minimum iops*/ 1)
    printf("XORencode satisfied performance requirement");

//timer start
mirror()
//timer stop
//compute IOPS
if (/*computed Iops > required minimum iops*/ 1)
    printf("mirror satisfied performance requirement");

//timer start
rebuildLBA()
//timer stop
//compute IOPS
if (/*computed Iops > required minimum iops*/ 1)
    printf("rebuild satisfied performance requirement");

//timer start
readInput()
//timer stop
//compute elapsed time
if (/*computed elapsed time < required maximum time*/ 1)
    printf("read satisfied performance requirement");

//timer start
writeToBuffer()
//timer stop
//compute elapsed time
if (/*computed elapsed time < required maximum time*/ 1)
    printf("writeToBuffer satisfied performance requirement");

//timer start
writeBufferToFile()
//timer stop
//compute elapsed time
if (/*computed elapsed time < required maximum time*/ 1)
    printf("writeBufferToFile satisfied performance requirement");


//delete file
singleRecovery(/*put location of deleted file here*/);
if dataCompare(/*location of rebuilt file*/)
    printf("singleRecovery was successful");

//delete two files
doubleRecovery(/*put locations of deleted files here*/);
if dataCompare(/*location of rebuilt file*/)
    printf("doubleRecovery was successful");

if noWrite(/*no input, will be called if write attempt during rebuild*/)
    printf("noWrite was successful");

if replaceCorrupt(/*location of corrupt file*/);
    printf("replaceCorrupt was successful");
}
