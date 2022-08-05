//Tom Hills (jhills)
//CS594 Advanced Algorithms
//Lab-1-FAT
//Lab Summary: Creates and reads in text files using the jdisk structure to simulate hard disk operations.

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <sstream> 
#include <cstring>
#include <cmath>
#include "jdisk.h"

using namespace std;

typedef struct 
{
  unsigned long size;  
  int fd;
  char *fn;
  int reads;
  int writes;
} Disk;

void usage(char *s)
{
  fprintf(stderr, "usage: FATRW diskfile import input-file\n");
  fprintf(stderr, "       FATRW diskfile export starting-block output-file\n");
  if (s != NULL) fprintf(stderr, "%s\n", s);
  exit(1);
}

//reads FAT sector from jdisk into read/write jdisk buffer, 
//then copies (ala memcpy) to newly allocated "fat" element for buffering
void jdiskRead(void *jd, unsigned int fatIndex, void *rwJdiskBuffer, unsigned char **fat, char *e)
{
    if ( jdisk_read(jd, fatIndex, rwJdiskBuffer) == 0 )
    {
        fat[fatIndex] = (unsigned char *) malloc(JDISK_SECTOR_SIZE + 1); //plus 1 to match # of bytes from rwJdiskbuffer
        memcpy( fat[fatIndex], rwJdiskBuffer, (size_t)(JDISK_SECTOR_SIZE + 1) );
    }
    else
    {
        fprintf(stderr, "jdisk_read() failed: %s!\n", e);
        exit(0);
    }
}

//mode: 2 == bytes printed in sequential order, 4 == 2 bytes together in little endian
//bytes: number of bytes to print
//bytesPerRow: number of bytes to print before newline
void printBuffer(int mode, int bytes, int bytesPerRow, unsigned char buffer[])
{
    int byteCnt = 0;
    long long totByteCnt = 0;
    
    //print total byte count up to this point
    printf("%010Xh ::: ", totByteCnt);
    for (int i = 0; i < bytes; i+=2)
    {
        totByteCnt += 2;
        if (mode == 4) printf("%02X%02X ", buffer[i+1], buffer[i]);
        else if (mode == 2) printf("%02X%02X ", buffer[i], buffer[i+1]);
        else printf("%02X %02X ", buffer[i], buffer[i+1]);
        byteCnt++;
        if (byteCnt == bytesPerRow/2 && i != bytes-2) 
        {
            printf("\n"); 
            byteCnt = 0; 
            //print total byte count up to this point
            printf("%010Xh ::: ", totByteCnt);
        }
    }

    printf("\n");
}

//reads 1024 Bytes of "inFile" data from "f" pointer to read/write jdisk buffer,
//...then writes it to attached Jdisk at correct "dBlock". This occurs for all entries in "dBlocks" array...
//...until -1 is reached.
void writeFileDataToJdisk(long long dBlocksSize, int *dBlocks, unsigned char rwJdiskBuffer[], Disk *jdisk, FILE *f, long long bytesToWrite)
{
    for (int i = 0; i < dBlocksSize; ++i)
    {
        //write all except the last block
        if (dBlocks[i+1] != -1) 
        {
            //read data block from file to read/write jdisk buffer
            if (fread(rwJdiskBuffer, 1, JDISK_SECTOR_SIZE, f) != JDISK_SECTOR_SIZE) 
            {fprintf(stderr, "fread failed in FINAL WRITE TO JDISK\n"); exit(1);}

            //write file contents to jdisk where lba == dBlocks in order from first to next to last
            jdisk_write(jdisk, dBlocks[i], rwJdiskBuffer);
        }
        //write last block
        else
        {
            //read LAST BLOCK from file to read/write jdisk buffer, then modify as needed
            if (bytesToWrite == 0) //read last 1024 bytes
            {
                if (fread(rwJdiskBuffer, 1, JDISK_SECTOR_SIZE, f) != JDISK_SECTOR_SIZE) 
                {fprintf(stderr, "fread failed in FINAL WRITE TO JDISK\n"); exit(1);}
            }
            else if (bytesToWrite == 1023) //read last 1023 bytes
            {
                if (fread(rwJdiskBuffer, 1, bytesToWrite, f) != bytesToWrite) 
                {fprintf(stderr, "fread failed in FINAL WRITE TO JDISK\n"); exit(1);}
                //write 0xff to last byte in data block
                rwJdiskBuffer[1023] = 0xff;
            }
            else if (bytesToWrite <= 1022) //read last bytesToWrite bytes
            {
                if (fread(rwJdiskBuffer, 1, bytesToWrite, f) != bytesToWrite) 
                {fprintf(stderr, "fread failed in FINAL WRITE TO JDISK\n"); exit(1);}
                //write bytesToWrite to last two bytes in data block
                rwJdiskBuffer[1022] = bytesToWrite;      //reassign least signficant byte
                rwJdiskBuffer[1023] = bytesToWrite >> 8; //reassign most significant byte
            }

            //write LAST BLOCK of file contents to jdisk where lba == dBlocks
            jdisk_write(jdisk, dBlocks[i], rwJdiskBuffer);
            break;
        }
    }
}

//prints jdisk information
void printJdisk(Disk *d)
{
    printf("size: %-10lu   fd: %-5d   fn: %-20s   reads: %-5d   writes: %-5d\n", d->size, d->fd, d->fn, d->reads, d->writes); 
}

int main(int argc, char *argv[])
{
    // "block" == "sector" == 1024 Bytes segment on jdisk
    unsigned short fatLink, nextFatLink, fatIndex, nextFLStartLink;//FL == freelist 
    int S, D, TDS, startBlock, dBlock, dBLocksIndex, sBlock;
    long long bytesToWrite, dBlocksSize;
    char *jd, *ie, *inFile, *outFile;
    int *dBlocks; 
    unsigned char rwJdiskBuffer[JDISK_SECTOR_SIZE + 1];
    unsigned char **fat; 
    Disk *jdisk;
    FILE *f;

    //check command line arguments. If good, store strings in properly named variables
    if (argc < 4 || argc > 5) usage(NULL);
    else 
    {
        jd = argv[1]; ie = argv[2];
        if (strcmp(ie, "import") == 0) inFile = argv[3];
        else if (strcmp(ie, "export") == 0) { startBlock = atoi(argv[3]); outFile = argv[4]; }
    }
    
    //get access to EXISTING jdisk object
    jdisk = (Disk *)jdisk_attach(jd);
    if (jdisk == NULL) usage((char *) "disk doesn't exist");

    //determine S (FAT sectors), D (Data sectors), and TDS (total disk sectors)
        // Equations used to solve for S: 
        //      1.)   S = [(D + 1) * 2 Bytes] / [1024 Bytes]
        //      2.) TDS = S + D
    TDS = jdisk->size / JDISK_SECTOR_SIZE;
    if ( TDS%513 == 0 ) S = TDS/513; //Don't add 1. There's no missing decimal portion
    else S = TDS/513 + 1; //add 1 b/c decimal portion got cut off. this micmicks "ceil()"  
    D = TDS - S;
    
    //error check startBlock
    if (startBlock < S) {fprintf(stderr, "Error: startBlock < S.\n"); exit(1);}

    //create and initialize "fat" array with S "unsigned char *"" elements
    fat = (unsigned char **) malloc( S*sizeof(unsigned char *) );
    for (int i = 0; i < S; ++i) fat[i] = NULL;

    //IMPORT-----------------------------------------------------------------------------------------
    if (strcmp(ie, "import") == 0)
    {
        //read first sector of FAT table into buffer b/c it has start of free list
        //if read is successful, make copy of "rwJdiskBuffer" into corresponding "fat" element to reduce future reads
        jdiskRead(jdisk, 0, rwJdiskBuffer, fat, (char *)"First read of 1st FAT sector after import"); 
        
        //inFile: open file stream and get total number of bytes. Then reset file position indicator and prepare dBlock buffer
        //dBlock buffer, "dBlocks", will be given a little extra room (+2). All elements initialized to -1 for stopping condition during final write
        f = fopen(inFile, "r");
        if (f == NULL) {fprintf(stderr, "Error opening inFile\n"); exit(0);}
        else
        {
            fseek(f, 0, SEEK_END); //seek to file end
            bytesToWrite = (long long) ftell(f);
            if (bytesToWrite == 0) {fprintf(stdout, "File is empty! bytesToWrite == 0 after opening file on line 159\n"); return 0;}
            rewind(f); //rewind to file start: better than "fseek(f, 0, SEEK_SET)" b/c EOF and error
                       //...indicators are also cleared which fseek(...) does not do
            dBlocksSize = sizeof(int) * ((bytesToWrite / JDISK_SECTOR_SIZE) + 2);
            dBlocks = (int *) malloc( dBlocksSize ); 
            for (int i = 0; i < dBlocksSize/sizeof(int); ++i) dBlocks[i] = -1;
        }

        //writing ONE OR MORE DATA BLOCKS of size 1024 Bytes
        fatLink = 0;
        dBLocksIndex = 0;
        while (bytesToWrite >= JDISK_SECTOR_SIZE)
        {
            //determine which FAT sector the next link is in. 
            //if FAT sector is already stored in "fat", copy contents into read/write jdisk buffer. 
            //Else, read FAT sector and store a reference to it in "fat"
            //get next fatLink for dBlock calc. in next loop. "fatLink - fatIndex * 512" to convert "fatLink" value into correct index for read/write jdisk buffer
            fatIndex = fatLink / 512; //decimal portion should be cutoff
            if (fat[fatIndex] != NULL) memcpy( rwJdiskBuffer, fat[fatIndex], (size_t)(JDISK_SECTOR_SIZE + 1) ); //plus 1 to match # of bytes from rwJdiskbuffer
            else jdiskRead(jdisk, fatIndex, rwJdiskBuffer, fat, (char *)"In bytesToWrite >= JDISK_SECTOR_SIZE loop");
            fatLink = ( rwJdiskBuffer[2*(fatLink - (fatIndex * 512)) + 1] << 8 | rwJdiskBuffer[2*(fatLink - (fatIndex * 512))] );

            //check end of freelist condition. If reached with byteToWrite >= 1024 Bytes, throw error
            if (fatLink == 0) {fprintf(stderr, "ERROR: end of freelist reached in \"bytesToWrite >= JDISK_SECTOR_SIZE\" loop\n"); exit(0);}

            //determine dat block ID for writing (i.e., logical block address [lba])
            dBlock = S + fatLink - 1;
            
            //store dBlock that needs writing and decrement 1024 Bytes from bytesToWrite
            dBlocks[dBLocksIndex] = dBlock;
            dBLocksIndex++;
            if (dBLocksIndex == dBlocksSize) {fprintf(stderr, "ERROR: dBLocksIndex == dBlocksSize. max dBlocksIndex allowed == dBlocksSize - 1\n"); exit(1);}
            bytesToWrite -= JDISK_SECTOR_SIZE;
        }

        //Update fatIndex with fatLink from last data block needing 1024 Bytes written to jdisk. Load FAT sector.
        fatIndex = fatLink / 512;
        if (fat[fatIndex] != NULL) memcpy( rwJdiskBuffer, fat[fatIndex], (size_t)(JDISK_SECTOR_SIZE + 1) ); //plus 1 to match # of bytes from rwJdiskbuffer
        else jdiskRead(jdisk, fatIndex, rwJdiskBuffer, fat, (char *)"In bytesToWrite == (JDISK_SECTOR_SIZE - 1) branch");

        //writing LAST DATA BLOCK whose size < 1024 Bytes 
        if (bytesToWrite == 0) 
        {            
            //Update FAT contents in appropriate FAT sectors
            if (fatIndex == 0) //one write required. Both FAT modifications occur in FAT sector 0
            {
                //Modify rwJdiskBuffer holding the FAT sector containing fatLink which was last written to.
                nextFLStartLink = ( rwJdiskBuffer[2*(fatLink - (fatIndex * 512)) + 1] << 8 | rwJdiskBuffer[2*(fatLink - (fatIndex * 512))] );
                
                rwJdiskBuffer[2*(fatLink - (fatIndex * 512))]     = 0x0000;      //reassign least signficant byte. no shift. Assignment to char cuts off left-most byte
                rwJdiskBuffer[2*(fatLink - (fatIndex * 512)) + 1] = 0x0000 >> 8; //reassign most significant byte. left-most byte becomes right-most byte
                rwJdiskBuffer[0] = nextFLStartLink;      //reassign least signficant byte. 
                rwJdiskBuffer[1] = nextFLStartLink >> 8; //reassign most significant byte. 
                
                jdisk_write(jdisk, fatIndex, rwJdiskBuffer); 
            }
            else //two writes required. FAT sector with last data block link written to != FAT sector 0
            {
                //Update last data block's fatLink with 0 b/c last data block in jdisk is full 
                nextFLStartLink = ( rwJdiskBuffer[2*(fatLink - (fatIndex * 512)) + 1] << 8 | rwJdiskBuffer[2*(fatLink - (fatIndex * 512))] );
                
                rwJdiskBuffer[2*(fatLink - (fatIndex * 512))]     = 0x0000;      //reassign least signficant byte. no shift. Assignment to char cuts off left-most byte
                rwJdiskBuffer[2*(fatLink - (fatIndex * 512)) + 1] = 0x0000 >> 8; //reassign most significant byte. left-most byte becomes right-most byte
                
                jdisk_write(jdisk, fatIndex, rwJdiskBuffer); 
                
                //Update element 0 of FAT sector 0 with nextFLStartLink
                fatIndex = 0;
                memcpy( rwJdiskBuffer, fat[fatIndex], (size_t)(JDISK_SECTOR_SIZE + 1) );
                rwJdiskBuffer[0] = nextFLStartLink;      //reassign least signficant byte. no shift. Assignment to char cuts off left-most byte
                rwJdiskBuffer[1] = nextFLStartLink >> 8; //reassign most significant byte. left-most byte becomes right-most byte
                
                jdisk_write(jdisk, fatIndex, rwJdiskBuffer);
            }

            //Write file contents to appropriate jdisk data sectors
            writeFileDataToJdisk(dBlocksSize, dBlocks, rwJdiskBuffer, jdisk, f, bytesToWrite);
        }
        else if (bytesToWrite <= (JDISK_SECTOR_SIZE - 1) ) // <= 1023 Bytes
        {
            //get fatLink/fatIndex for next block to hold the last 1023 Bytes of file data, find dBlock, and store it
            fatLink = ( rwJdiskBuffer[2*(fatLink - (fatIndex * 512)) + 1] << 8 | rwJdiskBuffer[2*(fatLink - (fatIndex * 512))] );
            if (fatLink == 0) {fprintf(stderr, "ERROR: end of freelist reached in \"bytesToWrite >= JDISK_SECTOR_SIZE\" loop\n"); exit(0);}
            fatIndex = fatLink / 512;
            dBlock = S + fatLink - 1;
            dBlocks[dBLocksIndex] = dBlock; 
            dBLocksIndex++; 
            if (dBLocksIndex == dBlocksSize) 
            {fprintf(stderr, "ERROR: dBLocksIndex == dBlocksSize. max dBlocksIndex allowed == dBlocksSize - 1\n"); exit(1);}

            //Update FAT
            if (fatIndex == 0) //one write required. Both FAT modifications occur in FAT sector 0
            {
                //Load FAT sector 0 into read/write jdisk buffer. 
                memcpy( rwJdiskBuffer, fat[fatIndex], (size_t)(JDISK_SECTOR_SIZE + 1) ); //plus 1 to match # of bytes from rwJdiskbuffer
                
                //get fatLink that will be the new start of the free list
                nextFLStartLink = ( rwJdiskBuffer[2*(fatLink - (fatIndex * 512)) + 1] << 8 | rwJdiskBuffer[2*(fatLink - (fatIndex * 512))] );

                //Update fatLink of last data block with itself b/c data blocked was NOT filled with file data
                rwJdiskBuffer[2*(fatLink - (fatIndex * 512))]     = fatLink;      //reassign least signficant byte. no shift. Assignment to char cuts off left-most byte
                rwJdiskBuffer[2*(fatLink - (fatIndex * 512)) + 1] = fatLink >> 8; //reassign most significant byte. left-most byte becomes right-most byte

                //Update element 0 of FAT sector 0 with nextFLStartLink
                rwJdiskBuffer[0] = nextFLStartLink;      //reassign least signficant byte
                rwJdiskBuffer[1] = nextFLStartLink >> 8; //reassign most significant byte
                
                jdisk_write(jdisk, fatIndex, rwJdiskBuffer);
            }
            else //two writes required. FAT sector with last data block link written to != FAT sector 0
            {
                //check if FAT block is already cached. If not, cache it. Load FAT sector into read/write jdisk buffer
                if (fat[fatIndex] != NULL) memcpy( rwJdiskBuffer, fat[fatIndex], (size_t)(JDISK_SECTOR_SIZE + 1) ); //plus 1 to match # of bytes from rwJdiskbuffer
                else jdiskRead(jdisk, fatIndex, rwJdiskBuffer, fat, (char *)"In bytesToWrite == (JDISK_SECTOR_SIZE - 1) branch");
                
                //get fatLink that will be the new start of the free list
                nextFLStartLink = ( rwJdiskBuffer[2*(fatLink - (fatIndex * 512)) + 1] << 8 | rwJdiskBuffer[2*(fatLink - (fatIndex * 512))] );

                //Update fatLink of last data block with itself b/c data blocked was NOT filled with file data
                rwJdiskBuffer[2*(fatLink - (fatIndex * 512))]     = fatLink;      //reassign least signficant byte. no shift. Assignment to char cuts off left-most byte
                rwJdiskBuffer[2*(fatLink - (fatIndex * 512)) + 1] = fatLink >> 8; //reassign most significant byte. left-most byte becomes right-most byte
                
                jdisk_write(jdisk, fatIndex, rwJdiskBuffer);

                //Update element 0 of FAT sector 0 with nextFLStartLink
                fatIndex = 0;
                memcpy( rwJdiskBuffer, fat[fatIndex], (size_t)(JDISK_SECTOR_SIZE + 1) );
                rwJdiskBuffer[0] = nextFLStartLink;      //reassign least signficant byte. 
                rwJdiskBuffer[1] = nextFLStartLink >> 8; //reassign most significant byte. 
                
                jdisk_write(jdisk, fatIndex, rwJdiskBuffer);
            }

            //Write file contents to appropriate jdisk data sectors
            writeFileDataToJdisk(dBlocksSize, dBlocks, rwJdiskBuffer, jdisk, f, bytesToWrite);
        }

        //final prints and close open file
        printf("New file starts at sector %d\n", dBlocks[0]);
        printf("Reads: %d\n", jdisk->reads);
        printf("Writes: %d\n", jdisk->writes);
        fclose(f);
    }   

    //EXPORT----------------------------------------------------------------------------------------   
    else if (strcmp(ie, "export") == 0)
    {
        //open output-file
        f = fopen(outFile, "w"); //"w" flag allows creation of outFile if it doesn't exist
        if (f == NULL) {fprintf(stderr, "Error opening outFile\n"); exit(0);}

        //get fatLink/fatIndex for given startBlock, read in corresponding FAT sector into read/write buffer. get next fat link.
        fatLink = startBlock - S + 1; 
        fatIndex = fatLink / 512;
        if (fat[fatIndex] != NULL) memcpy( rwJdiskBuffer, fat[fatIndex], (size_t)(JDISK_SECTOR_SIZE + 1) ); //plus 1 to match # of bytes from rwJdiskbuffer
        else jdiskRead(jdisk, fatIndex, rwJdiskBuffer, fat, (char *)"IMPORT: right before while(1) loop");
        nextFatLink = (rwJdiskBuffer[2*(fatLink - (fatIndex * 512)) + 1] << 8 | rwJdiskBuffer[2*(fatLink - (fatIndex * 512))]);
        
        if (nextFatLink == 0) //startBlock == only data sector needing to be written @ full 1024 Bytes
        {
            if (jdisk_read(jdisk, startBlock, rwJdiskBuffer) != 0) 
            {fprintf(stderr, "JDISK_READ failed on startBlock\n"); exit(1);}
            else if (fwrite(rwJdiskBuffer, 1, JDISK_SECTOR_SIZE, f) != JDISK_SECTOR_SIZE)
            {fprintf(stderr, "FWRITE failed on startBlock"); exit(1);}
        }
        else if (nextFatLink == fatLink) //startBlock == only data sector needing to be written @ <= 1023 Bytes
        {
            if (jdisk_read(jdisk, startBlock, rwJdiskBuffer) != 0) 
            {fprintf(stderr, "JDISK_READ failed on dBlock in ELSE IF (nextFatLink == startBlock)\n"); exit(1);}

            if (rwJdiskBuffer[1023] == 0xff) //write 1023 Bytes to output file
            {
                if (fwrite(rwJdiskBuffer, 1, JDISK_SECTOR_SIZE - 1, f) != JDISK_SECTOR_SIZE - 1)
                {fprintf(stderr, "FWRITE failed on dBlock in ELSE IF (nextFatLink == startBlock): 0xff"); exit(1);}
            }
            else //write bytesToWriteBytes to output file
            {
                bytesToWrite = rwJdiskBuffer[1023] << 8 | rwJdiskBuffer[1022];
                if (fwrite(rwJdiskBuffer, 1, bytesToWrite, f) != bytesToWrite)
                {fprintf(stderr, "FWRITE failed on dBlock in ELSE IF (nextFatLink == startBlock): NOT 0xff"); exit(1);}
            }
        }
        else //startBlock != only data sector needing to be written
        {
            //read startBlock data sector contents (1024) from jdisk into read/write jdisk buffer
            //...and write contents to output file.
            if (jdisk_read(jdisk, startBlock, rwJdiskBuffer) != 0) //not "jdiskRead()" which == ONLY for reading FAT sectors 
            {fprintf(stderr, "JDISK_READ failed on startBlock\n"); exit(1);}
            else if (fwrite(rwJdiskBuffer, 1, JDISK_SECTOR_SIZE, f) != JDISK_SECTOR_SIZE)
            {fprintf(stderr, "FWRITE failed on startBlock"); exit(1);}

            if (fat[fatIndex] != NULL) memcpy( rwJdiskBuffer, fat[fatIndex], (size_t)(JDISK_SECTOR_SIZE + 1) ); //plus 1 to match # of bytes from rwJdiskbuffer
            else jdiskRead(jdisk, fatIndex, rwJdiskBuffer, fat, (char *)"IMPORT: right before while(1) loop");

            //loop to follow fatLinks and read the remaining data blocks of requested file
            while (1)
            {
                //get fatLink and fatIndex associated with next data block needing to be written to file
                //then load corresponding FAT sector into read/write jdisk buffer
                fatLink = (rwJdiskBuffer[2*(fatLink - (fatIndex * 512)) + 1] << 8 | rwJdiskBuffer[2*(fatLink - (fatIndex * 512))]);
                fatIndex = fatLink / 512;
                if (fat[fatIndex] != NULL) memcpy( rwJdiskBuffer, fat[fatIndex], (size_t)(JDISK_SECTOR_SIZE + 1) ); //plus 1 to match # of bytes from rwJdiskbuffer
                else jdiskRead(jdisk, fatIndex, rwJdiskBuffer, fat, (char *)"IMPORT: right before while(1) loop");

                //From FAT sector just read into buffer, get next fatLink value in the file data block chain 
                nextFatLink = (rwJdiskBuffer[2*(fatLink - (fatIndex * 512)) + 1] << 8 | rwJdiskBuffer[2*(fatLink - (fatIndex * 512))]);

                //check for final data block conditions and write data to file
                if (nextFatLink == 0) 
                {
                    dBlock = S + fatLink - 1;
                    if (jdisk_read(jdisk, dBlock, rwJdiskBuffer) != 0) 
                    {fprintf(stderr, "JDISK_READ failed on dBlock in ELSE\n"); exit(1);}
                    else if (fwrite(rwJdiskBuffer, 1, JDISK_SECTOR_SIZE, f) != JDISK_SECTOR_SIZE)
                    {fprintf(stderr, "FWRITE failed on dBlock in ELSE"); exit(1);}

                    break;
                }
                else if (nextFatLink == fatLink)
                {
                    dBlock = S + fatLink - 1;

                    if (jdisk_read(jdisk, dBlock, rwJdiskBuffer) != 0) 
                    {fprintf(stderr, "JDISK_READ failed on dBlock in ELSE IF (nextFatLink == fatLink)\n"); exit(1);}

                    if (rwJdiskBuffer[1023] == 0xff) //write 1023 Bytes to output file
                    {
                        if (fwrite(rwJdiskBuffer, 1, JDISK_SECTOR_SIZE - 1, f) != JDISK_SECTOR_SIZE - 1)
                        {fprintf(stderr, "FWRITE failed on dBlock in ELSE IF (nextFatLink == fatLink): 0xff"); exit(1);}
                    }
                    else //write bytesToWriteBytes to output file
                    {
                        bytesToWrite = rwJdiskBuffer[1023] << 8 | rwJdiskBuffer[1022];
                        if (fwrite(rwJdiskBuffer, 1, bytesToWrite, f) != bytesToWrite)
                        {fprintf(stderr, "FWRITE failed on dBlock in ELSE IF (nextFatLink == fatLink): NOT 0xff"); exit(1);}
                    }

                    break;
                }
                else //Not the last data block. 
                {
                    //write 1024 Bytes to dBlock of jdisk
                    dBlock = S + fatLink - 1;

                    if (jdisk_read(jdisk, dBlock, rwJdiskBuffer) != 0) 
                    {fprintf(stderr, "JDISK_READ failed on dBlock in ELSE\n"); exit(1);}
                    else if (fwrite(rwJdiskBuffer, 1, JDISK_SECTOR_SIZE, f) != JDISK_SECTOR_SIZE)
                    {fprintf(stderr, "FWRITE failed on dBlock in ELSE"); exit(1);}

                    //read FAT sector back into read/write jdisk buffer
                    if (fat[fatIndex] != NULL) memcpy( rwJdiskBuffer, fat[fatIndex], (size_t)(JDISK_SECTOR_SIZE + 1) ); //plus 1 to match # of bytes from rwJdiskbuffer
                    else jdiskRead(jdisk, fatIndex, rwJdiskBuffer, fat, (char *)"IMPORT: right before while(1) loop");
                }
            }
        }
        
        //final print and close file   
        //writes == 0 b/c we never wrote to jdisk
        //reads == "# of FAT sectors read into buffer" + "# of Data sectors read into buffer"
        printf("Reads: %d\n", jdisk->reads);
        printf("Writes: %d\n", jdisk->writes);
        fclose(f);
    }
    
    //free heap memory and exit
    if (strcmp(ie, "import") == 0) free(dBlocks);
    free(jdisk);
    return 0;
}
