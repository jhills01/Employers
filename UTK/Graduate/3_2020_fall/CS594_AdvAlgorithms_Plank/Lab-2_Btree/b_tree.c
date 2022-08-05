// NAME: Tom Hills
// NETID: jhills
// CS 494: Lab2 - Btree
// DESCRIPTION:
// This lab implements the "B+ Tree" variant on jdisks, where internal nodes only hold keys,
// and pointers to other nodes, and external nodes hold keys and pointers to values. 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "jdisk.h"
#include "b_tree.h"

typedef struct 
{
  unsigned long size;  
  int fd;
  char *fn;
  int reads;
  int writes;
} Disk;

typedef struct tnode 
{
  unsigned char bytes[JDISK_SECTOR_SIZE+256]; /* This holds the sector for reading and writing.  
                                                 It has extra room because your internal representation  
                                                 will hold an extra key. */
  unsigned char nkeys;                      /* Number of keys in the node */
  unsigned char flush;                      /* Should I flush this to disk at the end of b_tree_insert()? */
  unsigned char internal;                   /* Internal or external node */
  unsigned int lba;                         /* LBA when the node is flushed */
  unsigned char **keys;                     /* Pointers to the keys.  Size = MAXKEY+1 */
  unsigned int *lbas;                       /* Pointer to the array of LBA's.  Size = MAXKEY+2 */
  struct tnode *parent;                     /* Pointer to my parent -- useful for splitting */
  int parent_index;                         /* My index in my parent */
  struct tnode *ptr;                        /* Free list link */

  unsigned int free;                        /* 1 == free, 0 == not free*/
} Tree_Node;

typedef struct 
{
  int key_size;                 /* These are the first 16/12 bytes in sector 0 */
  unsigned int root_lba;
  unsigned long first_free_block;

  Disk *disk;                   /* The jdisk */
  unsigned long size;           /* The jdisk's size */
  unsigned long num_lbas;       /* size/JDISK_SECTOR_SIZE */
  int keys_per_block;           /* MAXKEY */
  int lbas_per_block;           /* MAXKEY+1 */
  Tree_Node *free_list;         /* Free list of nodes */
  
  Tree_Node *tmp_e;             /* When find() fails, this is a pointer to the external node */
  int tmp_e_index;              /* and the index where the key should have gone */
 
  int flush;                    /* Should I flush sector[0] to disk after b_tree_insert() */

  unsigned long numOfInsertions;
  unsigned int previous_root_lba;
  Tree_Node *lba0; //address of sector 0 on disk;
} B_Tree;

void jdiskWrite(void *jd, unsigned int lba, void *buf, char *s)
{
    long result;

    result = jdisk_write(jd, lba, buf);
    if (result == -1) 
    {fprintf(stderr, "%s: write(d->fd, buf, JDISK_SECTOR_SIZE) != JDISK_SECTOR_SIZE\n", s); exit(1);} 
    else if (result == -2) 
    {fprintf(stderr, "%s: lba >= (d->size / JDISK_SECTOR_SIZE)\n", s); exit(1);} 
}

void jdiskRead(void *jd, unsigned int lba, void *buf, char *s)
{
    long result;

    result = jdisk_read(jd, lba, buf);
    if (result == -1) 
    {fprintf(stderr, "%s: read(d->fd, buf, JDISK_SECTOR_SIZE) != JDISK_SECTOR_SIZE\n",s); exit(1);} 
    else if (result == -2) 
    {fprintf(stderr, "%s: lba >= (d->size / JDISK_SECTOR_SIZE) in jdisk_read()\n",s); exit(1);} 
}

unsigned int INT_littleToBigEndian(unsigned char buffer[])
{
    return buffer[3] << 24 | buffer[2] << 16 | buffer[1] << 8 | buffer[0];
}

unsigned long LONG_littleToBigEndian(unsigned char buffer[])
{
    return buffer[7] << 56 | buffer[6] << 48 | buffer[5] << 40 | buffer[4] << 32 | buffer[3] << 24 | buffer[2] << 16 | buffer[1] << 8 | buffer[0];
}

void INT_bigToLittleEndian(unsigned char buffer[], int b2LInt)
{
    buffer[3] = (b2LInt >> 24);
    buffer[2] = (b2LInt >> 16);
    buffer[1] = (b2LInt >> 8 );
    buffer[0] =  b2LInt       ;
}

void LONG_bigToLittleEndian(unsigned char buffer[], long b2LLong)
{
    buffer[7] = (b2LLong >> 56);
    buffer[6] = (b2LLong >> 48);
    buffer[5] = (b2LLong >> 40);
    buffer[4] = (b2LLong >> 32);
    buffer[3] = (b2LLong >> 24);
    buffer[2] = (b2LLong >> 16);
    buffer[1] = (b2LLong >> 8 );
    buffer[0] =  b2LLong       ;
}

//endian: 'b' == big endian, 'l' == little endian, 'n' == none
//block: number of bytes that should be grouped together in printing
//blocksPerRow: number of blocks to print per row
//lba: logical block address within jdisk. Each lba == 1024 bytes.
//lowByteIndex: index of buffer to start with
//hightByteIndex: index of buffer to stop at
//buffer: contains a jdkisk sector, or 1024 bytes
void printBuffer(char endian, int block, int blocksPerRow, int lba, int lowByteIndex, int highByteIndex, unsigned char buffer[])
{
    int i, j;
    int blockCnt = 0;
    long totByteCnt = lowByteIndex + lba*JDISK_SECTOR_SIZE;

    if (lowByteIndex%block != 0) 
    {
        printf("\nERROR: lowByteIndex not a multiple of block. Converting to next lowest multiple\n\n");
        lowByteIndex = (lowByteIndex/block)*block;
    }

    printf("\n%010Xh ::: ", totByteCnt);
    
    for (i = lowByteIndex; i < highByteIndex; i+=block)
    {
        switch (endian)
        {
            case 'b':
                for (j = i + block -1; j >= i; --j)  printf("%02X", buffer[j]);
                break;
            
            case 'l':
                for (j = i; j < i + block; ++j)  printf("%02X", buffer[j]);
                break;

            case 'n':
                for (j = i; j < i + block; ++j)  printf("%02X ", buffer[j]);
                break;

            default:
                printf("\nERROR: endian not a legal value in printBuffer\n\n");
                break;
        }

        printf(" ");
        totByteCnt += block;
        blockCnt += 1;

        if ( (blocksPerRow < blockCnt + 1) && (i + block < highByteIndex) )
        {
            printf("\n");
            blockCnt = 0;
            printf("%010Xh ::: ", totByteCnt);
        }
    }

    printf("\n\n");
}

void printNode(Tree_Node *t)
{
    printf("nkeys: %d, internal: %d, lba: %d",
            t->nkeys, t->internal, t->lba);
}

void printTree(B_Tree *b)
{
    printf("\nkeysize: %d, rootLBA: %d, firstFreeBLock: %d, numLBAs: %lu, numOfInsertions: %d, tmpEIndex: %d\n",
            b->key_size, b->root_lba, b->first_free_block, b->num_lbas, b->numOfInsertions, b->tmp_e_index);
    if (b->tmp_e != NULL)
    {
        printf("tmp_e node: ");
        printNode(b->tmp_e);
        printf("\n");
    }
}

void printKeys(Tree_Node *t, int maxkey)
{
    int i;
    for (i = 0; i < maxkey; ++i) printf("key[%d]: %s\n", i, t->keys[i]);
}

void *b_tree_create(char *filename, long size, int key_size)
{
    int i, j, lbaStartIndex;
    long result;
    B_Tree *b = malloc(sizeof(B_Tree));
    Tree_Node *tnode = malloc(sizeof(Tree_Node));

    //capture LBA 0 in b_tree
    b->lba0 = tnode;

    //create jdisk file
    b->disk = (Disk *)jdisk_create(filename, size);
    if (b->disk == NULL) {fprintf(stderr, "b_tree_create: jdisk_create returned NULL\n"); exit(1);}
    
    //update b_tree object members
    b->key_size = key_size;
    b->root_lba = 1;
    b->previous_root_lba = b->root_lba;
    b->first_free_block = 1;
    
    b->size = size;
    b->num_lbas = b->size/((unsigned long)JDISK_SECTOR_SIZE);
    b->keys_per_block = (JDISK_SECTOR_SIZE - 6) / (b->key_size + 4);
    b->lbas_per_block = b->keys_per_block + 1;
    
    // create num_lbas - 2 Tree_Node objects for free_list
    // -2 b/c LBA 0 and 1 won't be free in a newly created b_tree
    b->free_list = (Tree_Node*)malloc((b->num_lbas - 1)*sizeof(Tree_Node));
    for (i = 0; i < (b->num_lbas - 2); ++i)
    {
        //allocate and point KEYs to the correct position in BYTES
        b->free_list[i].keys = (unsigned char **)malloc(b->lbas_per_block * sizeof(unsigned char *));//MAXKEY + 1
        for (j = 0; j < b->lbas_per_block; ++j)
        {
            b->free_list[i].keys[j] = &b->free_list[i].bytes[2+(j*b->key_size)];
        }

        //allocate LBAs
        b->free_list[i].lbas = (unsigned int *)&b->free_list[i].bytes[2+((b->lbas_per_block - 1)*b->key_size) + b->key_size + 2];
    }

    //Update remaining b_tree object members
    b->tmp_e = NULL;
    b->tmp_e_index = -1;
    b->flush = 0;
    b->numOfInsertions = 0;

    // Update LBA 0 via jdisk_write
    if (sizeof(b->key_size) == 4) INT_bigToLittleEndian(&tnode->bytes[0], b->key_size);
    else {printf("PROBLEM: b->key_size is not 4 bytes! Exiting....\n"); exit(0);}
        
    if (sizeof(b->root_lba) == 4) INT_bigToLittleEndian(&tnode->bytes[4], b->root_lba);
    else {printf("PROBLEM: b->root_lba is not 4 bytes! Exiting....\n"); exit(0);}

    if (sizeof(b->first_free_block) == 8) LONG_bigToLittleEndian(&tnode->bytes[8], b->first_free_block);
    else if (sizeof(b->first_free_block) == 4) INT_bigToLittleEndian(&tnode->bytes[8], b->first_free_block);
    else {printf("PROBLEM: b->first_free_block is not 8 or 4 bytes! Exiting....\n"); exit(0);}
    
    result = jdisk_write(b->disk, 0, tnode->bytes);
    if (result == -1) 
    {fprintf(stderr, "b_tree_create->jdisk_write: write(d->fd, buf, JDISK_SECTOR_SIZE) != JDISK_SECTOR_SIZE\n"); exit(1);} 
    else if (result == -2) 
    {fprintf(stderr, "b_tree_create->jdisk_write: lba >= (d->size / JDISK_SECTOR_SIZE)\n"); exit(1);} 

    //Update LBA 1 with default/no insertion values
    //set default values of "bytes"
    for (i = 0; i < JDISK_SECTOR_SIZE; ++i) tnode->bytes[i] = 0x0;

    result = jdisk_write(b->disk, b->first_free_block, tnode->bytes);
    if (result == -1) 
    {fprintf(stderr, "b_tree_create->jdisk_write: write(d->fd, buf, JDISK_SECTOR_SIZE) != JDISK_SECTOR_SIZE\n"); exit(1);} 
    else if (result == -2) 
    {fprintf(stderr, "b_tree_create->jdisk_write: lba >= (d->size / JDISK_SECTOR_SIZE)\n"); exit(1);} 

    return (void *)b;
}

//Returns previously created b_tree. 
void *b_tree_attach(char *filename)
{
    int i, j;
    long result;
    char *ptr;
    B_Tree *b = malloc(sizeof(B_Tree));
    Tree_Node *tnode = malloc(sizeof(Tree_Node));

    //attach existing jdisk file and update b_tree object with jdisk information
    b->disk = (Disk *)jdisk_attach(filename); 
    if (b->disk == NULL) {fprintf(stderr, "b_tree_attach: jdisk_attach returned NULL\n"); exit(1);}
    b->size = b->disk->size;
    b->num_lbas = b->size/((unsigned long)JDISK_SECTOR_SIZE);

    //read sector 0 to "bytes" in tnode
    result = jdisk_read(b->disk, 0, tnode->bytes);
    if (result == -1) 
    {fprintf(stderr, "b_tree_attach: read failed in jdisk_read()\n"); exit(1);} 
    else if (result == -2) 
    {fprintf(stderr, "b_tree_attach: lba >= (d->size / JDISK_SECTOR_SIZE) in jdisk_read()\n"); exit(1);} 

    b->lba0 = tnode;

    //build b_tree object with sector 0 information
    b->key_size = INT_littleToBigEndian(&tnode->bytes[0]);
    b->keys_per_block = (JDISK_SECTOR_SIZE - 6) / (b->key_size + 4);
    b->lbas_per_block = b->keys_per_block + 1;
    b->root_lba = INT_littleToBigEndian(&tnode->bytes[4]);
    b->first_free_block = LONG_littleToBigEndian(&tnode->bytes[8]);

    //build remaining b_tree objects
    b->tmp_e = NULL;//default value
    b->tmp_e_index = -1;//default value
    b->flush = 0;//default value

    ptr = (char *)b->lba0;
    ptr += b->first_free_block * JDISK_SECTOR_SIZE;
    b->free_list = (Tree_Node*)ptr;

exit(0);//DOESN'T WORK!
    return (void *)b;
}

unsigned int b_tree_insert_recursionHelper(void *b_tree, void *key, void *record)
{

    return 0;
}

//-------------------------------------------------------------------------------------------
unsigned int b_tree_insert(void *b_tree, void *key, void *record)
{
    unsigned int val_lba, record_lba, i;
    long result;
    B_Tree *b = (B_Tree*)b_tree;//simply typing
    Tree_Node *tRootNode = &b->free_list[b->root_lba - 1]; //capture root node
    Tree_Node *tNextFreeNode; //capture first free node

    val_lba = b_tree_find(b_tree, key);

    //early check to see if b_tree is full and provided key was NOT found in tree
    if (b->numOfInsertions == b->num_lbas*b->keys_per_block && val_lba == 0) return 0;

    //"key" NOT found in b_tree. INSERT key/val pair.
    if (val_lba == 0) 
    {
        //update insertion statisitics
        b->numOfInsertions++;

        //update next free node with record/val
        b->first_free_block++;
        tNextFreeNode = &b->free_list[b->first_free_block - 1];
        memcpy(&tNextFreeNode->bytes[0], record, JDISK_SECTOR_SIZE);
        jdiskWrite(b->disk, b->first_free_block, tNextFreeNode->bytes, "b_tree_insert -> val_lba == 0 -> WRITE nextFreeNode");

        //update node captured by b_tree_find()
        b->tmp_e->nkeys++;
        b->tmp_e->bytes[1] = b->tmp_e->nkeys;
        memcpy(b->tmp_e->keys[b->tmp_e_index], key, b->key_size);
        INT_bigToLittleEndian((unsigned char *)&b->tmp_e->lbas[b->tmp_e_index], b->first_free_block);
        jdiskWrite(b->disk, b->root_lba, b->tmp_e->bytes, "b_tree_insert -> val_lba == 0 -> WRITE tmp_e");

        //prepare for next insertion and return value
        record_lba = b->first_free_block;

        //update sector 0
        b->tmp_e = b->lba0; 
        jdiskRead(b->disk, 0, b->tmp_e->bytes, "b_tree_insert -> val_lba == 0 -> READ tmp_e");
        //first free block always changes with an insertion
        if (sizeof(b->first_free_block) == 8) LONG_bigToLittleEndian(&b->tmp_e->bytes[8], b->first_free_block+1);
        else if (sizeof(b->first_free_block) == 4) INT_bigToLittleEndian(&b->tmp_e->bytes[8], b->first_free_block+1);
        else {printf("PROBLEM: b->first_free_block is not 8 or 4 bytes! Exiting....\n"); exit(0);}
        if (b->previous_root_lba == b->root_lba) {/*do nothing*/}
        else //root lba changed. 
        {
            INT_bigToLittleEndian(&b->tmp_e->bytes[4], b->root_lba);
            b->previous_root_lba = b->root_lba; 
        }
        jdiskWrite(b->disk, 0, b->tmp_e->bytes, "b_tree_insert -> val_lba == 0 -> WRITE tmp_e");      
    }
    
    //NOTE: Not going to implement this yet until I get insertion example from NOTEs working
    //NOTE: DO NOT increment b->numOfInsertions in this branch. You are only replacing val of already inserted key
    //"key" found in b_tree. REPLACE val with new key/val pair.
    else 
    {
        return val_lba;
    }

    return record_lba;
}

//returns val if key is already in node
unsigned int findKeyIndex(void *b_tree, void *key)
{
    int i, j, n;
    B_Tree *b = (B_Tree*)b_tree;//simply typing

    for (i = 0; i < b->tmp_e->nkeys; ++i)
    {
        n = memcmp(key, b->tmp_e->keys[i], b->key_size);

        if (n > 0) continue;
        
        //key == b->tmp_e->keys[i]
        else if (n == 0)
        {
            b->tmp_e_index = i;
            INT_bigToLittleEndian((unsigned char *)&n, b->tmp_e->lbas[i]);
            return n; 
        }
        
        //key < b->tmp_e->keys[i]
        else if (n < 0) 
        {
            //shift keys/vals down to make room for new key/val
            for (j = b->tmp_e->nkeys-1; j >= i; --j)
            {
                memcpy(b->tmp_e->keys[j+1], b->tmp_e->keys[j], b->key_size);
                INT_bigToLittleEndian((unsigned char *)&b->tmp_e->lbas[j+1], b->tmp_e->lbas[j]);
            }
            b->tmp_e_index = i;
            return 0;
        }
    }

    b->tmp_e_index = i;
    return 0;
}

unsigned int b_tree_find(void *b_tree, void *key)
{
    B_Tree *b = (B_Tree*)b_tree;//simply typing
    Tree_Node *tRootNode = &b->free_list[b->root_lba - 1]; //capture root node

    b->tmp_e = tRootNode;

    if (b->tmp_e->internal == 0)//root node is the only node in tree
    {   
        if (b->tmp_e->nkeys == 0) //no keys in root node
        {
            b->tmp_e->bytes[0] = 0; //root node still external
            b->tmp_e_index = 0;//key should go in first key
            return 0; //key not in b_tree
        }
        else if (b->tmp_e->nkeys < b->keys_per_block)
        {
            //find where to place new key in root node
            b->tmp_e->bytes[0] = 0; //root node still external
            b->tmp_e->lba = findKeyIndex(b_tree, key);
            if (b->tmp_e->lba > 0) return b->tmp_e->lba; //KEY found!
            else //KEY not found
            {
                return 0;
            }
            
        }
        else if (b->tmp_e->nkeys == b->keys_per_block)
        {
            //time to split
            b->tmp_e->bytes[0] = 1; //new root node is internal

        }       
    }

    else if (b->tmp_e->internal == 1)
    {
        exit(0);//DOESN'T WORK
        b->tmp_e->lba == 0;
        while (b->tmp_e->lba == 0)
        {
            b->tmp_e->lba = findKeyIndex(b_tree, key);
            if(b->tmp_e->lba == 0 ) //key not found
            {
                b->tmp_e = &b->free_list[b->tmp_e_index - 1];
            }
        }

    }

    //key not found in b_tree
    return 0;
}

void *b_tree_disk(void *b_tree)
{
    B_Tree* b = (B_Tree *)b_tree;
    return b->disk;
}

int b_tree_key_size(void *b_tree)
{
    B_Tree* b = (B_Tree *)b_tree;
    return b->key_size;
}

void b_tree_print_tree(void *b_tree)
{
    return;
}

//OLD CODE STORAGE---------------------------------------------------------------------------------------------
// printBuffer('n', 4, 4, 0, 0, 10*16, tnode->bytes);
