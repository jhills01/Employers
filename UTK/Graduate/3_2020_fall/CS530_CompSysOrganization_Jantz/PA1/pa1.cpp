#include <cstdlib>
#include <iostream>
#include <sstream> 
#include <string>
#include <vector>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <map>
#include <bitset>
#include "pa1.h"

using namespace std;

//TLB-------------------------------------------------------------------------------------
void pa1::dataTLB::printConfigInfo()
{
    //maximum number of DTLB sets == 256
    if (numOfSets > 256 || numOfSets < 0) 
    {
        //printf("DTLB sets out of bounds (positive & <= 256): %d\n\n", p->dTLB.numOfSets);
        printf("hierarchy: error - number of dtlb sets requested exceeds MAXTLBSETS\n");
        exit(0);
    }
    
    //maximum associativity level of DTLB sets == 8
    else if (setSize > 8 || setSize < 0) 
    {
        printf("hierarchy: error - dtlb set size requested exceeds MAXSETSIZE\n");
        exit(0);
    }

    else if ( !isPowerOf2(numOfSets) )
    {
        printf("Data TLB contains %d sets.\n", numOfSets);
        printf("hierarchy: number of dtlb sets is not a power of two\n");
        exit(0);
    }

    //If no errors, print results
    printf("Data TLB contains %d sets.\n", numOfSets);
    printf("Each set contains %d entries.\n", setSize);
    printf("Number of bits used for the index is %d.\n\n", numOfIndexBits); //This is log2(numOfSets)
}

//PAGE TABLE------------------------------------------------------------------------------
void pa1::pageTable::printConfigInfo()
{
    //maximum number of virtual pages == 8192
    if (numOfVPs > 8192 || numOfVPs < 0) 
    {
        printf("hierarchy: error - number of virtual pages requested exceeds MAXVIRTPAGES\n");
        exit(0);
    }
    //maximum number of physical pages == 1024
    else if (numOfPPs > 1024 || numOfPPs < 0) 
    {
        printf("hierarchy: error - number of physical pages requested exceeds MAXPHYPAGES\n");
        exit(0);
    }

    else if ( !isPowerOf2(numOfVPs) )
    {
        printf("Number of virtual pages is %d.\n", numOfVPs);
        printf("hierarchy: number of virtual pages is not a power of two\n");
        exit(0);
    }
    else if ( !isPowerOf2(pageSize) )
    {
        printf("Number of virtual pages is %d.\n", numOfVPs);
        printf("Number of physical pages is %d.\n", numOfPPs);
        printf("Each page contains %d bytes.\n", pageSize);
        printf("hierarchy: page size is not a power of two\n");
        exit(0);
    }


    //If no errors, print results
    printf("Number of virtual pages is %d.\n", numOfVPs);
    printf("Number of physical pages is %d.\n", numOfPPs);
    printf("Each page contains %d bytes.\n", pageSize);
    printf("Number of bits used for the page table index is %d.\n", numOfPageTableIndexBits); //log2(numOfVPs)
    printf("Number of bits used for the page offset is %d.\n\n", numOfPageOffsetBits); //log2(pageSize)
}

int pa1::pageTable::PPNassign(pa1 *p)
{
    p->pTable.roundRobin++;
    //p->pTable.roundRobinStr = bitset< 64 >( p->pTable.roundRobin ).to_string();
    if (p->pTable.roundRobin == p->pTable.numOfPPs - 1) 
    {p->allowRoundRobin = 'n';}//max roundRobin == p->pTable.numOfPPs - 1
    
    return p->pTable.roundRobin;
}

pa1::PTentry* pa1::pageTable::findTranslation(pa1 *p)
{
    pa1::PTentry* pte = NULL;

    //determine if VPN already exists in PT
    //NOTE: p->pTable.vpn is current line's VPN
    //NOTE: For ROUND ROBIN PHASE, I will fill up PT from bottom to top. Shouldn't matter...
    //      ...since it's a linear search regardless!!!
    for (int i = 0; i < p->pTable.PT.size(); ++i)
    {
        if (p->pTable.PT[i].vpn == p->pTable.vpn) {return (pa1::PTentry*)&p->pTable.PT[i];}
        else if (p->pTable.PT[i].vpn == -1) {pte = (pa1::PTentry*)&p->pTable.PT[i];}
    }

    return pte;
}

pa1::PTentry* pa1::pageTable::findLRU_PTE()
{
    unsigned long long lowestLRU = __LONG_LONG_MAX__;//initialize to highest posible value and work down from there
    pa1::PTentry* PTEToEvict;

    for (int i = 0; i < PT.size(); ++i)
    { 
        if (PT[i].LRUnum < lowestLRU)//not "<=" to avoid performing extra operations...
        {                                //...if more than one PTE have the same lowestLRU
            lowestLRU = PT[i].LRUnum;
            PTEToEvict = (pa1::PTentry*)&PT[i];
        } 
    }

    return PTEToEvict;
}

char pa1::pageTable::accessPT(pa1 *p, unsigned long long LRUcounter)
{
    //check if page table entry already exists in PT
    p->pte = p->pTable.findTranslation(p);

    //NOTE: p->pte = NULL SHOULD NOT HAPPEN IN ROUND ROBIN PHASE
    if (p->pte == NULL)//PAGE FAULT: PT FULL --- need to evict
    {
        //Find page to evict from PT
        p->pte = p->pTable.findLRU_PTE();

        //prepare evicted page references for invalidating TLB entries and cache lines
        p->pTable.pageFaultOccurred = 'y';
        p->pageFault_evictTLBindex = p->pte->TLBindex;
        p->pageFault_evictdIndex = p->pte->dIndex;
        p->pageFault_evictl2Index = p->pte->l2Index;
        
        //update PT statistics
        if (p->pte->dirtyBit = 1)//write OLD PAGE to disk if dirty
        {
            p->diskRefs++;//write dirty page to disk
        } 
        p->pageTableRefs++;
        p->pTable.faults++; 
        p->diskRefs++;//read NEW PAGE from disk...
        //p->mainMemoryRefs++;//into memory
        p->pTable.resultStr = "miss";

        //Update PT Data Structure
        //NOTE: I'm not sure about using ACCESSTYPE to immediately set dirtyBit like this!!!
        if (tolower(p->accessType) == 'w') p->pte->dirtyBit = 1;
        else if (tolower(p->accessType) == 'r') p->pte->dirtyBit = 0;
        p->pte->LRUnum = LRUcounter;
        p->pte->pageValid = 1;
        //p->pte->ppn = p->pTable.ppn; //NOTE: Don't need to do this after ROUND ROBIN PHASE. Only the VPN changes!!
        p->pte->vpn = p->pTable.vpn;

        p->pte->TLBindex = p->dTLB.index;//used for future eviction purposes
        p->pte->TLBtag = p->dTLB.tag;

        p->pte->dIndex = p->dCache.index;
        p->pte->dTag = p->dCache.tag;

        p->pte->l2Index = p->l2Cache.index;
        p->pte->l2Tag = p->l2Cache.tag;
        
        return 'm';
    }
    
    else if (p->pte->vpn == -1)//NO PAGE FAULT: PT NOT FULL
    {
        //update PT statistics
        p->pageTableRefs++;
        p->pTable.faults++; //NOTE: Actually, I'm not sure this is a fault. I'll count it for now
        p->diskRefs++;//read from disk...
        //p->mainMemoryRefs++;//into memory

        p->pTable.resultStr = "miss";

        //Update PT Data Structure
        //NOTE: I'm not sure about using ACCESSTYPE to immediately set dirtyBit like this!!!
        //      However, this is the first time the page is brought into memory, so I should check both WRITES/READS
        if (tolower(p->accessType) == 'w') p->pte->dirtyBit = 1;
        else if (tolower(p->accessType) == 'r') p->pte->dirtyBit = 0;
        p->pte->LRUnum = LRUcounter;
        p->pte->pageValid = 1;
        p->pte->ppn = p->pTable.ppn;//NOTE: This only works in ROUND ROBIN PHASE. Leave it out when NULL is returned (i.e, PT is FULL). 
        //p->pte->ppnStr = bitset< 64 >( p->pTable.ppn ).to_string();
        p->pte->vpn = p->pTable.vpn;
        
        p->pte->TLBindex = p->dTLB.index;//used for eviction purposes
        p->pte->dIndex = p->dCache.index;
        p->pte->l2Index = p->l2Cache.index;

        return 'm';
    }
    
    else if (p->pte->vpn == p->pTable.vpn)//HIT
    {
        //update PT statistics
        if (p->dTLB.resultStr == "miss") {p->pTable.hits++; p->pageTableRefs++;}
        //p->mainMemoryRefs++;//PT is in main memory

        p->pTable.resultStr = "hit";

        //Update PT Data Structure
        //NOTE: I'm not sure about using ACCESSTYPE to immediately set dirtyBit like this!!!
        if (tolower(p->accessType) == 'w' && p->pte->dirtyBit == 0) p->pte->dirtyBit = 1;//Only switch to dirty if undirty to start with

        return 'h';
    }
}

//DATA CACHE------------------------------------------------------------------------------
void pa1::dataCache::printConfigInfo()
{
    //maximum number of DC sets == 8192
    if (numOfSets > 8192 || numOfSets < 0) 
    {
        printf("hierarchy: error - number of dc sets requested exceeds MAXDCSETS\n");
        exit(0);
    }

    //maximum associativity level of DC sets == 8
    else if (setSize > 8 || setSize < 0) 
    {
        printf("hierarchy: error - data cache set size requested exceeds MAXSETSIZE\n");
        exit(0);
    }

    //minimum data line size for DC sets == 8
    else if (lineSize < 8 || lineSize < 0) 
    {
        printf("hierarchy: data cache line size is less than MINDCLINESIZE\n");
        exit(0);
    }


    else if ( !isPowerOf2(numOfSets) )
    {
        printf("D-cache contains %d sets.\n", numOfSets);
        printf("hierarchy: number of dc sets is not a power of two\n");
        exit(0);
    }
    else if ( !isPowerOf2(lineSize) )
    {
        printf("D-cache contains %d sets.\n", numOfSets);
        printf("Each set contains %d entries.\n", setSize);
        printf("Each line is %d bytes.\n", lineSize);
        printf("hierarchy: number of bytes in dc line is not a power of two\n");
        exit(0);
    }

    //If no errors, print results
    printf("D-cache contains %d sets.\n", numOfSets);
    printf("Each set contains %d entries.\n", setSize);
    printf("Each line is %d bytes.\n", lineSize);
    if (wt_nwa == 'n') printf("The cache uses a write-allocate and write-back policy.\n");
    else printf("The cache uses a no write-allocate and write-through policy.\n");
    printf("Number of bits used for the index is %d.\n", numOfIndexBits); //log2(numOfSets)
    printf("Number of bits used for the offset is %d.\n\n", numOfOffsetBits); //log2(lineSize)
}

//SET/FRAMES------------------------------------------------------------------------------
//returns 1 of 3 things listed below in order of priority:
//  1) frame* with tag == newTag
//  2) frame* with tag == -1
//  3) frame* == NULL if either 1) or 2) fail
pa1::frame* pa1::set::findFrameTag(int newTag, string cache)//NOTE: I think this error is bogus
{
    pa1::frame* rFrame = NULL;
    
    if (cache == "dCache")
    {
        for (int i = 0; i < frames.size(); ++i)
        {
            if ( frames[i].dTag == newTag ) { return (pa1::frame*)&frames[i]; }
            if ( frames[i].dTag == -1) { rFrame = (pa1::frame*)&frames[i]; }
        }
    }    
    else if (cache == "l2Cache")   
    {        
        for (int i = 0; i < frames.size(); ++i)
        {
            if ( frames[i].l2Tag == newTag ) { return (pa1::frame*)&frames[i]; }
            if ( frames[i].l2Tag == -1) { rFrame = (pa1::frame*)&frames[i]; }
        }
    }
    else if (cache == "TLB")
    {
        for (int i = 0; i < frames.size(); ++i)
        {
            if ( frames[i].TLBtag == newTag ) { return (pa1::frame*)&frames[i]; }
            if ( frames[i].TLBtag == -1) { rFrame = (pa1::frame*)&frames[i]; }
        }
    }

    return rFrame;
}

//finds frame within set with lowest LRUnum and evicts(i.e., replace oldTag with newTag, set LRUnum == LRUcounter)
//LRUcounter is incremented by 1 per input line in Main(), and passed into class structures
pa1::frame* pa1::set::findLRUFrame()
{
    unsigned long long lowestLRU = __LONG_LONG_MAX__;//initialize to highest posible value and work down from there
    pa1::frame* frameToEvict;

    for (int i = 0; i < frames.size(); ++i)
    { 
        if (frames[i].LRUnum < lowestLRU)//not "<=" to avoid performing extra operations...
        {                                //...if more than one frame have the same lowestLRU
            lowestLRU = frames[i].LRUnum;
            frameToEvict = (pa1::frame*)&frames[i];
        } 
    }

    return frameToEvict;
}

//updates set in cache
char pa1::set::updateCacheAndHierarchy(int newTag, int newIndex, char wt_nwa, unsigned long long LRUcounter, pa1 *p, string cache)
{
    pa1::frame* f = findFrameTag(newTag, cache);

//L1 CACHE-------------------------------------------------------------------------------------------
    if (cache == "dCache")
    {
        //DEBUG
        if ( (p->debug == 'd' || p->debug == 'b') && LRUcounter <= p->upperB && LRUcounter >= p->lowerB) 
        {printf("\n(START)Printing L1 cache (cache == %s): memRefs: %-3d, dHits: %-3d, dMisses: %-3d\n", 
                 cache.c_str(), p->mainMemoryRefs, p->dCache.hits, p->dCache.misses); p->printObj("dCache", p);}

        if (f == NULL)//MISS: "frames" of indexed "set" FULL----------------------------------------------------------------------------------
        {
            //finds and returns LRU frame that must be evicted
            f = findLRUFrame();

            //WRITES and READS for wt_nwa == 'n'. BUT, only WRITES for wt_nwa == 'y'. 
            if (wt_nwa == 'n' && f->dirtyBit == 1)//write-back, write-allocate. If dCache and L2 allowed, then dCache updates L2 with evicted frame
            {
                if ((p->allowL2Cache == 'n' && cache == "dCache") || (p->allowL2Cache == 'y' && cache == "l2Cache"))
                {
                    p->mainMemoryRefs++;//OLD frame read back memory from cache
                    p->mainMemoryRefs++;//NEW frame read from memory into cache
                }
                else if (p->allowL2Cache == 'y' && cache == "dCache")
                {
                    //Update L2 cache with evicted frame b/c evicted frame has dirty bit == 1
                    p->L1writeBackToL2 = 'y';
                    p->evicted_dTag = f->dTag;
                    p->evicted_dIndex = f->dIndex;
                    p->evicted_l2Tag = f->l2Tag;
                    p->evicted_l2Index = f->l2Index;

                    //DEBUG
                    if ( (p->debug == 'd' || p->debug == 'b') && LRUcounter <= p->upperB && LRUcounter >= p->lowerB) 
                    printf("\nEVICTING FROM L1 TO L2(cache == %s)--> dTag: %x, dIndex: %x, l2Tag: %x, l2Index: %x\n", cache.c_str(), p->evicted_dTag, p->evicted_dIndex, p->evicted_l2Tag, p->evicted_l2Index);

                    p->s = &p->l2Cache.l2CacheSets[f->l2Index];//pointer to indexed set
                    p->s->updateCacheAndHierarchy(f->l2Tag, f->l2Index, p->l2Cache.wt_nwa, LRUcounter, p, "l2Cache");

                    p->L1writeBackToL2 = 'n';
                }

                //write allocate == update date cache with NEW frame information
                if (tolower(p->accessType) == 'w') f->dirtyBit = 1;//set NEW frame to dirty for WRITES. Main memory isn't updated yet. 
                else if (tolower(p->accessType) == 'r') f->dirtyBit = 0;//reset dirtyBit == 0 for READS.
                f->LRUnum = LRUcounter;
                f->dTag = p->dCache.tag;
                f->dIndex = p->dCache.index;
                f->l2Tag = p->l2Cache.tag;
                f->l2Index = p->l2Cache.index;

                //NOTE: should only do this for write-through
                if (p->allowL2Cache == 'y')
                {
                    p->s = &p->l2Cache.l2CacheSets[p->l2Cache.index];//pointer to indexed set
                    p->s->updateCacheAndHierarchy(p->l2Cache.tag, p->l2Cache.index, p->l2Cache.wt_nwa, LRUcounter, p, "l2Cache");
                }
            }
            else if (wt_nwa == 'n' && f->dirtyBit == 0)//write-back, write-allocate
            {
                if ((p->allowL2Cache == 'n' && cache == "dCache") || (p->allowL2Cache == 'y' && cache == "l2Cache"))
                {p->mainMemoryRefs++;}//NEW frame read from memory into cache
                //NOTE: NICE! No ELSE IF here for calling L2 b/c dirty bit == 0!

                //write allocate == update date cache with NEW frame information
                if (tolower(p->accessType) == 'w') f->dirtyBit = 1;//NEW frame is automatically dirty b/c after read in, then it is written to. Main memory has different copy.
                f->LRUnum = LRUcounter;
                f->dTag = p->dCache.tag;
                f->dIndex = p->dCache.index;
                f->l2Tag = p->l2Cache.tag;
                f->l2Index = p->l2Cache.index;

                //NOTE: testing L1: 4, 2, n. Commenting out this out had NO EFFECT!!
                //NOTE: should only do this for write-through 
                if (p->allowL2Cache == 'y')
                {
                    p->s = &p->l2Cache.l2CacheSets[p->l2Cache.index];//pointer to indexed set
                    p->s->updateCacheAndHierarchy(p->l2Cache.tag, p->l2Cache.index, p->l2Cache.wt_nwa, LRUcounter, p, "l2Cache");
                }
            }
            else if (wt_nwa == 'y' && tolower(p->accessType) == 'w')//write-through, no-write-allocate. No update to cache. No dirty bit to check
            {
                if ((p->allowL2Cache == 'n' && cache == "dCache") || (p->allowL2Cache == 'y' && cache == "l2Cache"))
                {p->mainMemoryRefs++;}//OLD frame read back to memory from cache

                //write through the same reference address data to L2 cache 
                if (p->allowL2Cache == 'y')
                {
                    p->s = &p->l2Cache.l2CacheSets[p->l2Cache.index];//pointer to indexed set
                    p->s->updateCacheAndHierarchy(p->l2Cache.tag, p->l2Cache.index, p->l2Cache.wt_nwa, LRUcounter, p, "l2Cache");
                }
            }

            //READS for wt_nwa == 'y'. 
            else if (tolower(p->accessType) == 'r')//reads are always the same 
            {
                if ((p->allowL2Cache == 'n' && cache == "dCache") || (p->allowL2Cache == 'y' && cache == "l2Cache"))
                {p->mainMemoryRefs++;}//NEW frame read from memory into cache

                //write allocate == update date cache with NEW frame information
                f->dirtyBit = 0;
                f->LRUnum = LRUcounter;
                f->dTag = p->dCache.tag;
                f->dIndex = p->dCache.index;
                f->l2Tag = p->l2Cache.tag;
                f->l2Index = p->l2Cache.index;

                if (p->allowL2Cache == 'y')
                {
                    p->s = &p->l2Cache.l2CacheSets[p->l2Cache.index];//pointer to indexed set
                    p->s->updateCacheAndHierarchy(p->l2Cache.tag, p->l2Cache.index, p->l2Cache.wt_nwa, LRUcounter, p, "l2Cache");
                }
            }

            //DEBUG
            if ( (p->debug == 'd' || p->debug == 'b') && LRUcounter <= p->upperB && LRUcounter >= p->lowerB) 
            {printf("\n(MISS NULL)Printing L1 cache (cache == %s): memRefs: %-3d, dHits: %-3d, dMisses: %-3d\n", 
                 cache.c_str(), p->mainMemoryRefs, p->dCache.hits, p->dCache.misses+1); p->printObj("dCache", p);}

            //update result of cache access
            if (cache == "dCache") {p->dCache.resultStr = "miss"; p->dCache.misses++; return 'm';}
            else if (cache == "l2Cache") {p->l2Cache.resultStr = "miss"; p->l2Cache.misses++; return 'm';}
        }
        else if (f->dTag == -1)//MISS: "frames" of indexed "set" NOT FULL--------------------------------------------------------------------
        {
            //WRITES
            //if (wt_nwa == 'n')//write-back, write-allocate. Dirty bit == 0 b/c it's empty frame.
            if (wt_nwa == 'n' && tolower(p->accessType) == 'w')//write-back, write-allocate. Dirty bit == 0 b/c it's empty frame.
            {
                if ((p->allowL2Cache == 'n' && cache == "dCache") || (p->allowL2Cache == 'y' && cache == "l2Cache"))
                {p->mainMemoryRefs++;}//NEW frame read from memory into cache

                //write allocate == update date cache with NEW frame information
                if (tolower(p->accessType) == 'w') f->dirtyBit = 1;//NEW frame is automatically dirty b/c after read in, then it is written to. Main memory has different copy.
                f->LRUnum = LRUcounter;
                f->dTag = p->dCache.tag;
                f->dIndex = p->dCache.index;
                f->l2Tag = p->l2Cache.tag;
                f->l2Index = p->l2Cache.index;

                //On dCache miss, update L2 also
                //NOTE: This doesn't feel right. 
                if (p->allowL2Cache == 'y')
                {   
                    p->s = &p->l2Cache.l2CacheSets[p->l2Cache.index];//pointer to indexed set
                    p->s->updateCacheAndHierarchy(p->l2Cache.tag, p->l2Cache.index, p->l2Cache.wt_nwa, LRUcounter, p, "l2Cache");
                }
            }
            
            else if (wt_nwa == 'y' && tolower(p->accessType) == 'w')//write-through, no-write-allocate. 
            {                                                       //Write to lower level AND empty cache block. No dirty bit.
                if ((p->allowL2Cache == 'n' && cache == "dCache") || (p->allowL2Cache == 'y' && cache == "l2Cache"))
                {p->mainMemoryRefs++;}//NEW frame read from memory into cache

                f->LRUnum = LRUcounter;

                //write through the same reference address data to L2 cache 
                if (p->allowL2Cache == 'y')
                {   
                    p->s = &p->l2Cache.l2CacheSets[p->l2Cache.index];//pointer to indexed set
                    p->s->updateCacheAndHierarchy(p->l2Cache.tag, p->l2Cache.index, p->l2Cache.wt_nwa, LRUcounter, p, "l2Cache");
                }
            }
            
            //READS
            else if (tolower(p->accessType) == 'r')
            {
                if ((p->allowL2Cache == 'n' && cache == "dCache") || (p->allowL2Cache == 'y' && cache == "l2Cache"))
                {p->mainMemoryRefs++;}//NEW frame read from memory into cache

                //write allocate == update date cache with NEW frame information
                f->dirtyBit = 0;
                f->LRUnum = LRUcounter;
                f->dTag = p->dCache.tag;
                f->dIndex = p->dCache.index;
                f->l2Tag = p->l2Cache.tag;
                f->l2Index = p->l2Cache.index;
            }

            //DEBUG
            if ( (p->debug == 'd' || p->debug == 'b') && LRUcounter <= p->upperB && LRUcounter >= p->lowerB) 
            {printf("\n(MISS -1)Printing L1 cache (cache == %s): memRefs: %-3d, dHits: %-3d, dMisses: %-3d\n", 
                 cache.c_str(), p->mainMemoryRefs, p->dCache.hits, p->dCache.misses+1); p->printObj("dCache", p);}

            if (cache == "dCache") {p->dCache.resultStr = "miss"; p->dCache.misses++; return 'm';}
            else if (cache == "l2Cache") {p->l2Cache.resultStr = "miss"; p->l2Cache.misses++; return 'm';}
        }
        else if (f->dTag == newTag)//HIT---------------------------------------------------------------------------------------
        {
            //WRITES
            //write-back, write-allocate == don't update main memory. Only dirty bit.
            if (wt_nwa == 'n' && tolower(p->accessType) == 'w' && f->dirtyBit == 0) f->dirtyBit = 1;
            
            //write-through, no-write-allocate == update cache AND update main memory
            else if ( ((p->allowL2Cache == 'n' && cache == "dCache") || (p->allowL2Cache == 'y' && cache == "l2Cache")) 
                        && (wt_nwa == 'y') && tolower(p->accessType) == 'w') 
            {p->mainMemoryRefs++;} //NOTE: no need to update tag. It's the same b/c of the hit! And LRUnum is updated below.
            
            //READS == do nothing

            f->LRUnum = LRUcounter;

            //DEBUG
            if ( (p->debug == 'd' || p->debug == 'b') && LRUcounter <= p->upperB && LRUcounter >= p->lowerB) 
            {printf("\n(HIT)Printing L1 cache (cache == %s): memRefs: %-3d, dHits: %-3d, dMisses: %-3d\n", 
                 cache.c_str(), p->mainMemoryRefs, p->dCache.hits+1, p->dCache.misses); p->printObj("dCache", p);}

            if (cache == "dCache") {p->dCache.resultStr = "hit"; p->dCache.hits++; return 'h';}
            else if (cache == "l2Cache") {p->l2Cache.resultStr = "hit"; p->l2Cache.hits++; return 'h';}
        }
    }

//L2 CACHE-------------------------------------------------------------------------------------------
    else if (cache == "l2Cache")
    {

        //DEBUG
        if ( (p->debug == 'l' || p->debug == 'b') && LRUcounter <= p->upperB && LRUcounter >= p->lowerB) 
        {printf("\n(START)Printing L2 cache (cache == %s): memRefs: %-3d, l2Hits: %-3d, l2Misses: %-3d\n", 
                 cache.c_str(), p->mainMemoryRefs, p->l2Cache.hits, p->l2Cache.misses); p->printObj("l2Cache", p);}


        if (f == NULL)//MISS: "frames" of indexed "set" FULL----------------------------------------------------------------------------------
        {
            //finds and returns LRU frame that must be evicted
            f = findLRUFrame();

            //NOTE: I don't know if this IF/ELSE IF for writebacks from L1 is necessary
            //if (p->L1writeBackToL2 == 'y' && p->l2Cache.wt_nwa == 'n')//write-back, write allocate
            if (p->L1writeBackToL2 == 'y' && wt_nwa == 'n')//write-back, write allocate
            {                
                if ((p->allowL2Cache == 'n' && cache == "dCache") || (p->allowL2Cache == 'y' && cache == "l2Cache"))
                {p->mainMemoryRefs++;}//NEW frame read from memory into cache

                f->LRUnum = LRUcounter;

                f->dTag = p->evicted_dTag;
                f->dIndex = p->evicted_dIndex;
                f->l2Tag = p->evicted_l2Tag;
                f->l2Index = p->evicted_l2Index;

                f->dirtyBit = 1;//always do this b/c Dr. Jantz says "You will only write back to L2 if the data in L1 is different than L2. 
                                //So, yes, you need to set the dirty bit in L2 when a block gets written back."

                //p->l2Cache.resultStr = "hit"; //NOTE: don't think this is needed. Dr. Jantz says writebacks aren't
                                                //... outputted in results table
                p->l2Cache.hits++;//NOTE: I do NOT understand how this counts as a hit. 

                //DEBUG
                if ( (p->debug == 'l' || p->debug == 'b') && LRUcounter <= p->upperB && LRUcounter >= p->lowerB) 
                {printf("\n(MISS NULL: L1writebackL2)Printing L2 cache (cache == %s): memRefs: %-3d, l2Hits: %-3d, l2Misses: %-3d\n", 
                        cache.c_str(), p->mainMemoryRefs, p->l2Cache.hits, p->l2Cache.misses); p->printObj("l2Cache", p);}

                return 'h';
            }

            //else if (p->L1writeBackToL2 == 'y' && p->l2Cache.wt_nwa == 'y') 
            else if (p->L1writeBackToL2 == 'y' && wt_nwa == 'y') 
            {
                if ((p->allowL2Cache == 'n' && cache == "dCache") || (p->allowL2Cache == 'y' && cache == "l2Cache"))
                {p->mainMemoryRefs++;}//NEW frame read from memory into cache

                f->LRUnum = LRUcounter;
                
                p->l2Cache.hits++;//NOTE: I do NOT understand how this counts as a hit. 

                //DEBUG
                if (p->debug == 'y' && LRUcounter <= p->upperB && LRUcounter >= p->lowerB) 
                {printf("\n(MISS NULL: L1writebackL2)Printing L2 cache (cache == %s): memRefs: %-3d, l2Hits: %-3d, l2Misses: %-3d\n", 
                        cache.c_str(), p->mainMemoryRefs, p->l2Cache.hits, p->l2Cache.misses); p->printObj("l2Cache", p);}

                return 'h';
            }

            ////WRITES and READS for wt_nwa == 'n'. BUT, only WRITES for wt_nwa == 'y'.
            //if (wt_nwa == 'n' && f->dirtyBit == 1)//write-back, write-allocate. If dCache and L2 allowed, then dCache updates L2 with evicted frame
            else if (wt_nwa == 'n' && f->dirtyBit == 1)//write-back, write-allocate. If dCache and L2 allowed, then dCache updates L2 with evicted frame
            {
                if ((p->allowL2Cache == 'n' && cache == "dCache") || (p->allowL2Cache == 'y' && cache == "l2Cache"))
                {
                    p->mainMemoryRefs++;//OLD frame read back memory from cache
                    p->mainMemoryRefs++;//NEW frame read from memory into cache
                }

                p->l2Cache.hits++;//NOTE: Added this to match Dr. Jantz output. I DON'T understand why though....

                //write allocate == update date cache with NEW frame information
                if (tolower(p->accessType) == 'w') f->dirtyBit = 1;//set NEW frame to dirty for WRITES. Main memory isn't updated yet. 
                else if (tolower(p->accessType) == 'r') f->dirtyBit = 0;//reset dirtyBit == 0 for READS.
                f->LRUnum = LRUcounter;
                f->dTag = p->dCache.tag;
                f->dIndex = p->dCache.index;
                f->l2Tag = p->l2Cache.tag;
                f->l2Index = p->l2Cache.index;
            }
            else if (wt_nwa == 'n' && f->dirtyBit == 0)//write-back, write-allocate
            {
                if ((p->allowL2Cache == 'n' && cache == "dCache") || (p->allowL2Cache == 'y' && cache == "l2Cache"))
                {p->mainMemoryRefs++;}//NEW frame read from memory into cache
                //NOTE: NICE! No ELSE IF here for calling L2 b/c dirty bit == 0!

                //write allocate == update date cache with NEW frame information
                if (tolower(p->accessType) == 'w') f->dirtyBit = 1;//NEW frame is automatically dirty b/c after read in, then it is written to. Main memory has different copy.
                f->LRUnum = LRUcounter;
                f->dTag = p->dCache.tag;
                f->dIndex = p->dCache.index;
                f->l2Tag = p->l2Cache.tag;
                f->l2Index = p->l2Cache.index;
            }
            else if (wt_nwa == 'y' && tolower(p->accessType) == 'w')//write-through, no-write-allocate. No update to cache. No dirty bit to check
            {
                if ((p->allowL2Cache == 'n' && cache == "dCache") || (p->allowL2Cache == 'y' && cache == "l2Cache"))
                {p->mainMemoryRefs++;}//OLD frame read back to memory from cache
            }
            
            //READS for wt_nwa == 'y'.
            else if (tolower(p->accessType) == 'r')//reads are always the same 
            {
                if ((p->allowL2Cache == 'n' && cache == "dCache") || (p->allowL2Cache == 'y' && cache == "l2Cache"))
                {p->mainMemoryRefs++;}//NEW frame read from memory into cache

                //write allocate == update date cache with NEW frame information
                f->dirtyBit = 0;
                f->LRUnum = LRUcounter;
                // f->tag = newTag;
                // f->index = newIndex;
                f->dTag = p->dCache.tag;
                f->dIndex = p->dCache.index;
                f->l2Tag = p->l2Cache.tag;
                f->l2Index = p->l2Cache.index;
            }


            //DEBUG
            if ( (p->debug == 'l' || p->debug == 'b') && LRUcounter <= p->upperB && LRUcounter >= p->lowerB) 
            {printf("\n(MISS NULL)Printing L2 cache (cache == %s): memRefs: %-3d, l2Hits: %-3d, l2Misses: %-3d\n", 
                        cache.c_str(), p->mainMemoryRefs, p->l2Cache.hits, p->l2Cache.misses+1); p->printObj("l2Cache", p);}

            //update result of cache access
            if (cache == "dCache") {p->dCache.resultStr = "miss"; p->dCache.misses++; return 'm';}
            else if (cache == "l2Cache") {p->l2Cache.resultStr = "miss"; p->l2Cache.misses++; return 'm';}
        }
        else if (f->l2Tag == -1)//MISS: "frames" of indexed "set" NOT FULL--------------------------------------------------------------------
        {
            //WRITES

            //NOTE: I don't know if this IF/ELSE IF for writebacks from L1 is necessary
            //if (p->L1writeBackToL2 == 'y')
            if (p->L1writeBackToL2 == 'y' && p->l2Cache.wt_nwa == 'n')//write-back, write allocate
            {                
                if ((p->allowL2Cache == 'n' && cache == "dCache") || (p->allowL2Cache == 'y' && cache == "l2Cache"))
                {p->mainMemoryRefs++;}//NEW frame read from memory into cache

                f->LRUnum = LRUcounter;

                f->dTag = p->evicted_dTag;
                f->dIndex = p->evicted_dIndex;
                f->l2Tag = p->evicted_l2Tag;
                f->l2Index = p->evicted_l2Index;

                f->dirtyBit = 1;//always do this b/c Dr. Jantz says "You will only write back to L2 if the data in L1 is different than L2. 
                                //So, yes, you need to set the dirty bit in L2 when a block gets written back."

                //p->l2Cache.resultStr = "hit"; //NOTE: don't think this is needed. Dr. Jantz says writebacks aren't
                                                //... outputted in results table
                p->l2Cache.hits++;//NOTE: I do NOT understand how this counts as a hit. 

                //DEBUG
                if ( (p->debug == 'l' || p->debug == 'b') && LRUcounter <= p->upperB && LRUcounter >= p->lowerB) 
                {printf("\n(MISS -1: L1writebackL2)Printing L2 cache (cache == %s): memRefs: %-3d, l2Hits: %-3d, l2Misses: %-3d\n", 
                        cache.c_str(), p->mainMemoryRefs, p->l2Cache.hits, p->l2Cache.misses); p->printObj("l2Cache", p);}

                return 'h';
            }

            else if (p->L1writeBackToL2 == 'y' && p->l2Cache.wt_nwa == 'y') 
            {
                if ((p->allowL2Cache == 'n' && cache == "dCache") || (p->allowL2Cache == 'y' && cache == "l2Cache"))
                {p->mainMemoryRefs++;}//NEW frame read from memory into cache

                f->LRUnum = LRUcounter;
                
                p->l2Cache.hits++;//NOTE: I do NOT understand how this counts as a hit. 

                //DEBUG
                if ( (p->debug == 'l' || p->debug == 'b') && LRUcounter <= p->upperB && LRUcounter >= p->lowerB) 
                {printf("\n(MISS -1: L1writebackL2)Printing L2 cache (cache == %s): memRefs: %-3d, l2Hits: %-3d, l2Misses: %-3d\n", 
                        cache.c_str(), p->mainMemoryRefs, p->l2Cache.hits, p->l2Cache.misses); p->printObj("l2Cache", p);}

                return 'h';
            }


            //if (wt_nwa == 'n')//write-back, write-allocate. Dirty bit == 0 b/c it's empty frame.
            else if (wt_nwa == 'n' && tolower(p->accessType) == 'w')//write-back, write-allocate. Dirty bit == 0 b/c it's empty frame.
            {
                if ((p->allowL2Cache == 'n' && cache == "dCache") || (p->allowL2Cache == 'y' && cache == "l2Cache"))
                {p->mainMemoryRefs++;}//NEW frame read from memory into cache

                //write allocate == update date cache with NEW frame information
                if (tolower(p->accessType) == 'w') f->dirtyBit = 1;//NEW frame is automatically dirty b/c after read in, then it is written to. Main memory has different copy.
                f->LRUnum = LRUcounter;
                f->dTag = p->dCache.tag;
                f->dIndex = p->dCache.index;
                f->l2Tag = p->l2Cache.tag;
                f->l2Index = p->l2Cache.index;
            }
            
            else if (wt_nwa == 'y' && tolower(p->accessType) == 'w')//write-through, no-write-allocate. 
            {                                                       //Write to lower level AND empty cache block. No dirty bit.
                if ((p->allowL2Cache == 'n' && cache == "dCache") || (p->allowL2Cache == 'y' && cache == "l2Cache"))
                {p->mainMemoryRefs++;}//NEW frame read from memory into cache

                f->LRUnum = LRUcounter;
            }

            //READS
            else if (tolower(p->accessType) == 'r')
            {
                if ((p->allowL2Cache == 'n' && cache == "dCache") || (p->allowL2Cache == 'y' && cache == "l2Cache"))
                {p->mainMemoryRefs++;}//NEW frame read from memory into cache

                //write allocate == update date cache with NEW frame information
                f->dirtyBit = 0;
                f->LRUnum = LRUcounter;
                // f->tag = newTag;
                // f->index = newIndex;
                f->dTag = p->dCache.tag;
                f->dIndex = p->dCache.index;
                f->l2Tag = p->l2Cache.tag;
                f->l2Index = p->l2Cache.index;
            }


            //DEBUG
            if ( (p->debug == 'l' || p->debug == 'b') && LRUcounter <= p->upperB && LRUcounter >= p->lowerB) 
            {printf("\n(MISS -1)Printing L2 cache (cache == %s): memRefs: %-3d, l2Hits: %-3d, l2Misses: %-3d\n", 
                        cache.c_str(), p->mainMemoryRefs, p->l2Cache.hits, p->l2Cache.misses+1); p->printObj("l2Cache", p);}

            if (cache == "dCache") {p->dCache.resultStr = "miss"; p->dCache.misses++; return 'm';}
            else if (cache == "l2Cache") {p->l2Cache.resultStr = "miss"; p->l2Cache.misses++; return 'm';}
        }
        else if (f->l2Tag == newTag)//HIT---------------------------------------------------------------------------------------
        {
            //WRITES
            //write-back, write-allocate == don't update main memory. Only dirty bit.
            if (wt_nwa == 'n' && tolower(p->accessType) == 'w' && f->dirtyBit == 0) f->dirtyBit = 1;


            //write-through, no-write-allocate == update cache AND update main memory
            else if ( ((p->allowL2Cache == 'n' && cache == "dCache") || (p->allowL2Cache == 'y' && cache == "l2Cache")) 
                        && (wt_nwa == 'y') && tolower(p->accessType) == 'w') 
            {p->mainMemoryRefs++;} //NOTE: no need to update tag. It's the same b/c of the hit! And LRUnum is updated below.
            
            //READS == do nothing

            f->LRUnum = LRUcounter;

            //DEBUG
            if ( (p->debug == 'l' || p->debug == 'b') && LRUcounter <= p->upperB && LRUcounter >= p->lowerB) 
            {printf("\n(HIT)Printing L2 cache (cache == %s): memRefs: %-3d, l2Hits: %-3d, l2Misses: %-3d\n", 
                        cache.c_str(), p->mainMemoryRefs, p->l2Cache.hits+1, p->l2Cache.misses); p->printObj("l2Cache", p);}

            if (cache == "dCache") {p->dCache.resultStr = "hit"; p->dCache.hits++; return 'h';}
            else if (cache == "l2Cache") {p->l2Cache.resultStr = "hit"; p->l2Cache.hits++; return 'h';}
        }
    }
}

//L2 CACHE--------------------------------------------------------------------------------
void pa1::L2Cache::printConfigInfo()
{
    //maximum associativity level of L2 sets == 8
    if (setSize > 8 || setSize < 0) 
    {
        printf("hierarchy: error - L2 cache set size requested exceeds MAXSETSIZE\n");
        exit(0);
    }

    else if ( !isPowerOf2(numOfSets) )
    {
        printf("L2-cache contains %d sets.\n", numOfSets);
        printf("hierarchy: number of L2 sets is not a power of two\n");
        exit(0);
    }
    else if ( !isPowerOf2(lineSize) )
    {
        printf("L2-cache contains %d sets.\n", numOfSets);
        printf("Each set contains %d entries.\n", setSize);
        printf("Each line is %d bytes.\n", lineSize);
        printf("hierarchy: number of bytes in L2 line is not a power of two\n");
        exit(0);
    }

    //If no errors, print results
    printf("L2-cache contains %d sets.\n", numOfSets);
    printf("Each set contains %d entries.\n", setSize);
    printf("Each line is %d bytes.\n", lineSize);
    if (wt_nwa == 'n') printf("The cache uses a write-allocate and write-back policy.\n");
    else printf("The cache uses a no write-allocate and write-through policy.\n");
    printf("Number of bits used for the index is %d.\n", numOfIndexBits); //log2(numOfSets)
    printf("Number of bits used for the offset is %d.\n\n", numOfOffsetBits); //log2(lineSize)
}

//PA1-------------------------------------------------------------------------------------
void pa1::printAllConfigInfo()
{
    //print and error check all configuration info from "trace.config" file------------------------
    dTLB.printConfigInfo();
    pTable.printConfigInfo();
    dCache.printConfigInfo();
    if (l2Cache.lineSize < dCache.lineSize) //NOTE: Had to put this error external b/c...
    {                                       //...I could not access dCache linesize inside...
                                            //...l2 printConfigInfo()!!!
        printf("hierarchy: L2 cache line size must be >= to the data cache line size\n");
        exit(0);
    }
    l2Cache.printConfigInfo();

    //check VIRTUAL ADDRESS tag-----------------------------------------------------------------------
    if (allowVA == 'y') printf("The addresses read in are virtual addresses.\n");
    else if (allowVA == 'n') printf("The addresses read in are physical addresses.\n");

    //check TLB tag --- print nothing if TLB is enabled-----------------------------------------------
    if (allowTLB == 'n') printf("TLB is disabled in this configuration.\n");
    else if (allowVA == 'n' && allowTLB == 'y')
    {
        printf ("hierarchy: TLB cannot be enabled when virtual addresses are disabled\n");
        exit(0);
    }

    //check L2 CACHE tag --- print nothing if L2 CACHE is enabled-------------------------------------
    if (allowL2Cache == 'n') printf("L2 cache is disabled in this configuration.\n");

    printf("\n");
}

//returns 1 if number i is not zero and also a power of 2. 0 otherwise. 
unsigned int pa1::isPowerOf2(unsigned int i)
{
    //"i" can't be zero for this to work
    //"i>0 && ((i & (i-1))" will be ZERO if "i" is a power of 2 b/c one less than a "power of 2" number is all 1s...
    //...in all the bits below the power of 2 bit. Thus, &ing it bitwise will produce ZERO.  
    if ( i>0 && ((i & (i-1)) == 0) ) return 1;
    else return 0;
}

void pa1::printObj(string obj, pa1 *p)
{       
    if (obj == "dCache")
    {
        for(int i = 0; i < p->dCache.dCacheSets.size(); ++i)
        {
            for(int j = 0; j < p->dCache.dCacheSets[i].frames.size(); ++j)
            {
                printf("p->dCache.dCacheSets[%d].frames[%d]--> dTag: %x, dIndex: %x, LRUnum: %d. dirtyBit: %d\n", 
                        i, j, p->dCache.dCacheSets[i].frames[j].dTag,
                        p->dCache.dCacheSets[i].frames[j].dIndex,
                        p->dCache.dCacheSets[i].frames[j].LRUnum,
                        p->dCache.dCacheSets[i].frames[j].dirtyBit);
            }
        }
    }

    else if (obj == "l2Cache")
    {
        for(int i = 0; i < p->l2Cache.l2CacheSets.size(); ++i)
        {
            for(int j = 0; j < p->l2Cache.l2CacheSets[i].frames.size(); ++j)
            {
                printf("p->l2Cache.l2CacheSets[%d].frames[%d]--> l2Tag: %x, l2Index: %x, LRUnum: %d. dirtyBit: %d\n", 
                        i, j, p->l2Cache.l2CacheSets[i].frames[j].l2Tag,
                        p->l2Cache.l2CacheSets[i].frames[j].l2Index,
                        p->l2Cache.l2CacheSets[i].frames[j].LRUnum,
                        p->l2Cache.l2CacheSets[i].frames[j].dirtyBit);
            }
        }
    }
    
    else if (obj == "PT")
    {
        for(int i = 0; i < p->pTable.PT.size(); ++i)
        {
            printf("p->pTable.PT[%d]--> VPN: %x, PPN: %X, dirtyBit: %d, pageValid: %d, LRUnum: %d\n", 
                    i, p->pTable.PT[i].vpn,
                    p->pTable.PT[i].ppn,
                    p->pTable.PT[i].dirtyBit,
                    p->pTable.PT[i].pageValid,
                    p->pTable.PT[i].LRUnum);
        }
    }

    else if (obj == "TLB")
    {
        for(int i = 0; i < p->dTLB.TLB.size(); ++i)
        {
            for(int j = 0; j < p->dTLB.TLB[i].frames.size(); ++j)
            {
                printf("p->dTLB.TLB[%d].frames[%d]--> TLBTag: %x, TLBIndex: %x, TLBData: %x, LRUnum: %d. dirtyBit: %d\n", 
                        i, j, p->dTLB.TLB[i].frames[j].TLBtag,
                        p->dTLB.TLB[i].frames[j].TLBindex,
                        p->dTLB.TLB[i].frames[j].TLBdata,
                        p->dTLB.TLB[i].frames[j].LRUnum,
                        p->dTLB.TLB[i].frames[j].dirtyBit);
            }
        }
    }

    printf("\n");
}