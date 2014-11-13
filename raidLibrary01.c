//=======================================================================================
//
//Function and Feature Requirements
//
//======================================================================================

int xorLBA(/* The sectors that will be XOR'ed and the XOR parity sector */)
{
    printf("This sub system is where the parity of the files will be created");
//XORencode has completed successfully
    return 1;
}

int mirror(/* The sector buffers that are to be written to the file */)
{
    printf("This sub system is where the mirror of the files including the parity will be created");
//mirror has completed successfully
    return 1;
}

int readInput(/* The sector buffers for the reads to go */)
{
    printf("This sub system is where the system will read from the selected file(s)");
//read has completed successfully
    return 1;
}

int writeToBuffer()
{
    printf("This sub system is where the system will write to the buffer (some specified size)");
//write has completed successfully
    return 1;
}

int rebuildLBA(/* The sector to be rebuilt along with the other sectors */)
{
    printf("This sub system is where the system will rebuild a missing file");
//rebuild has completed successfully
    return 1;
}

int readDuringRebuild()
{
    printf("This sub system will read from missing files that are currently being rebuilt");
//readDuringRebuild has completed successfully
    return 1;
}


//========================================================================================
//
//Performance functions
//
//========================================================================================

int howLong()
{
    printf("this sub system will time either XORencode, mirror, read, write, or rebuild and returns elapsed time for testing purposes");
}

int writeBufferToFile()
{
    printf("this sub system will write the data in the write buffer, into the files and then instigate encode and mirror");
//writeBuffer has completed successfully
   return 1;
}

//=======================================================================================
//
//Error Handling, Recover, Ease of Use Requirements
//
//=======================================================================================

int singleRecovery()
{
    printf("this sub system will instigate rebuild for a single missing file using the mirrored data");
//singleRecovery has completed successfully
    return 1;
}

int doubleRecovery()
{
    printf("this sub system will instigate rebuild for two missing files using mirror if the files are not the same, or use the parity if they are the same");
//doubleRecovery has completed successfully
    return 1;
}

int noWrite()
{
    printf("this sub system will disallow writes to a file currently being rebuilt.");
//noWrite has completed successfully
    return 1;
}

int dataCompare()
{
    printf("this sub system will compare a file to both its mirror and itself using the XOR parity");
//dataCompare has completed successfully and the files in question are the same
    return 1;
}

void replaceCorrupt()
{
    printf("this sub system will remove and rebuild a file that is deemed corrupt");
//replaceCorrupt has completed successfully
    return 1;
}







