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

//NON-CLASS METHODS-------------------------------------------------------------------------------------------
//string s is string to be printed with string literal sName, given by caller.
void printStr(string s, string sName)
{
    printf("%s: %s\n", sName.c_str(), s.c_str());
}

void printIntVector(vector<int> &v, string s)
{
    for (int i = 0; i < v.size(); ++i) printf("%s[%d]: %d\n", s.c_str(), i, v[i]);
    printf("\n");
}

//string s is the string name of vector
void printCharVector(vector<char> &v, string s)
{
    for (int i = 0; i < v.size(); ++i) printf("%s[%d]: %c\n", s.c_str(), i, v[i]);
    printf("\n");
}

//string s is the string name of vector
void printStrVector(vector<string> &v, string s)
{
    for (int i = 0; i < v.size(); ++i) printf("%s[%d]: %s\n", s.c_str(), i, v[i].c_str());
    printf("\n");
}

//converts binary string into an integer
int binaryToInt(string binary)
{
    int currInt = 1; //initialize with 2^0.
    int rTotal = 0; //running total to return

    //Start with right-most element of "binary" and move to first element.
    for (int i = binary.length()-1; i >=0; --i)
    {
        //advance to the NEXT power of 2 integer after updating total
        if (binary[i] == '1') {rTotal += currInt; currInt *= 2;}
        //advance to the NEXT power of 2 integer
        else if (binary[i] == '0') currInt *= 2; 
    }

    return rTotal;
}

//converts hex string to binary string
string hexToBinary(string hex)
{
    string binary = "";

    for (int i = 0; i < hex.length(); ++i)
    {
        switch ( tolower(hex [i]) )
        {
            case '0': binary.append ("0000"); break;
            case '1': binary.append ("0001"); break;
            case '2': binary.append ("0010"); break;
            case '3': binary.append ("0011"); break;
            case '4': binary.append ("0100"); break;
            case '5': binary.append ("0101"); break;
            case '6': binary.append ("0110"); break;
            case '7': binary.append ("0111"); break;
            case '8': binary.append ("1000"); break;
            case '9': binary.append ("1001"); break;
            case 'a': binary.append ("1010"); break;
            case 'b': binary.append ("1011"); break;
            case 'c': binary.append ("1100"); break;
            case 'd': binary.append ("1101"); break;
            case 'e': binary.append ("1110"); break;
            case 'f': binary.append ("1111"); break;
        }
    }

    return binary;
}

//converts hex string to binary string
string binaryToHex(string binary)
{
    int fourBits_i;
    string hex = "";
    string fourBits_s;

    for (int i = 0; i < binary.length(); i+=4)
    {
        fourBits_s = binary.substr(i, 4); //4 chars at a time starting @ i
        fourBits_i = binaryToInt(fourBits_s);
        switch (fourBits_i)
        {
            case  0: hex.append ("0"); break;
            case  1: hex.append ("1"); break;
            case  2: hex.append ("2"); break;
            case  3: hex.append ("3"); break;
            case  4: hex.append ("4"); break;
            case  5: hex.append ("5"); break;
            case  6: hex.append ("6"); break;
            case  7: hex.append ("7"); break;
            case  8: hex.append ("8"); break;
            case  9: hex.append ("9"); break;
            case 10: hex.append ("a"); break;
            case 11: hex.append ("b"); break;
            case 12: hex.append ("c"); break;
            case 13: hex.append ("d"); break;
            case 14: hex.append ("e"); break;
            case 15: hex.append ("f"); break;
        }
    }

    return hex;
}

//splits a string in half at the given position (pos). returns the two halves.
//"pos" value == number of characters needed in pair.second half.
//string s will be altered so DO NOT PASS BY REFERENCE!!
pair<string, string> splitStr(string s, int pos)
{
    // "s.length() - pos"" gives index of first character of string in the 2nd half (right-most)
    string s1 = s.substr(0, s.length() - pos); 
    string s2 = s.substr( s.length() - pos);
    return pair<string, string>(s1, s2);
}

//returns 1 if number i is not zero and also a power of 2. 0 otherwise. 
int isPowerOf2(int i)
{
    //"i" can't be zero for this to work
    //"i>0 && ((i & (i-1))" will be ZERO if "i" is a power of 2 b/c one less than a "power of 2" number is all 1s...
    //...in all the bits below the power of 2 bit. Thus, &ing it bitwise will produce ZERO.  
    if ( i>0 && ((i & (i-1)) == 0) ) return 1;
    else return 0;
}

//converts string "line" from hex to binary, from binary to int, ...
//...then finally back to hex. 
void printTestForConvertingStrings(string line)
{
    printf("START-------------------------------------------------------\n");
    printf("line:                     %s\n", line.c_str());
    line = hexToBinary(line);
    printf("hexToBinary(line_hex):    %s\n", line.c_str());
    printf("binaryToInt(line_binary): %d\n", binaryToInt(line));
    line = binaryToHex(line);
    printf("binaryToHex(line_binary): %s\n", line.c_str());
    printf("END---------------------------------------------------------\n\n");
}

void resetFrame(pa1::frame* f)
{
    f->dIndex = -1; 
    f->dirtyBit = 0; 
    f->dTag = -1; 
    f->l2Index = -1;
    f->l2Tag = -1;
    f->LRUnum = 0;
    f->TLBdata = -1;
    f->TLBindex = -1;
    f->TLBtag = -1;
}

int main()
{
    unsigned long long LRUcounter = 0;//incremented once per input file line. Passed down to update/assign to all relevant LRUnum variables
    string line;
    ifstream configFile ("trace.config");
    pa1 *p = new pa1(); 
    pair<string, string> str1Str2;

    //PROCESS "TRACE.CONFIG" FILE----------------------------------------------------------------------
    if (configFile.is_open())
    {
        while(getline(configFile, line))
        {
            if (line.find("Data TLB") != string::npos)
            {
                getline(configFile, line);
                p->dTLB.numOfSets = atoi( line.substr(line.find(":") + 2).c_str() );
                p->dTLB.TLB.resize(p->dTLB.numOfSets);

                getline(configFile, line);
                p->dTLB.setSize = atoi( line.substr(line.find(":") + 2).c_str() );
                for (int i = 0; i < p->dTLB.TLB.size(); ++i)
                {p->dTLB.TLB[i].frames.resize(p->dTLB.setSize);}

                p->dTLB.numOfIndexBits = log2(p->dTLB.numOfSets);
            }

            else if (line.find("Page Table") != string::npos)
            {
                getline(configFile, line);
                p->pTable.numOfVPs = atoi( line.substr(line.find(":") + 2).c_str() );
                //p->pTable.PT.resize(p->pTable.numOfVPs);

                getline(configFile, line);
                p->pTable.numOfPPs = atoi( line.substr(line.find(":") + 2).c_str() );
                p->pTable.PT.resize(p->pTable.numOfPPs);//NOTE: Dr. Jantz said you could do it this way


                getline(configFile, line);
                p->pTable.pageSize = atoi( line.substr(line.find(":") + 2).c_str() );

                p->pTable.numOfPageTableIndexBits = log2(p->pTable.numOfVPs);
                p->pTable.numOfVpnBits = p->pTable.numOfPageTableIndexBits; //NOTE: I believe what Dr. Jantz...
                                                                            //...calls page table index is...
                                                                            //...also the vpn.
                p->pTable.numOfPageOffsetBits = log2(p->pTable.pageSize);
            }

            else if (line.find("Data Cache") != string::npos)
            {
                getline(configFile, line);
                p->dCache.numOfSets = atoi( line.substr(line.find(":") + 2).c_str() );
                p->dCache.dCacheSets.resize(p->dCache.numOfSets);

                getline(configFile, line);
                p->dCache.setSize = atoi( line.substr(line.find(":") + 2).c_str() );
                for (int i = 0; i < p->dCache.dCacheSets.size(); ++i)
                {p->dCache.dCacheSets[i].frames.resize(p->dCache.setSize);}
                
                getline(configFile, line);
                p->dCache.lineSize = atoi( line.substr(line.find(":") + 2).c_str() );
            
                getline(configFile, line);
                //grab 1st character from returned string (should be either 'y' or 'n')
                p->dCache.wt_nwa = line.substr(line.find(":") + 2).c_str()[0];

                p->dCache.numOfIndexBits = log2(p->dCache.numOfSets);
                p->dCache.numOfOffsetBits = log2(p->dCache.lineSize);
            }

            else if (line.find("L2 Cache") != string::npos)
            {
                getline(configFile, line);
                p->l2Cache.numOfSets = atoi( line.substr(line.find(":") + 2).c_str() );
                p->l2Cache.l2CacheSets.resize(p->l2Cache.numOfSets);
                
                getline(configFile, line);
                p->l2Cache.setSize = atoi( line.substr(line.find(":") + 2).c_str() );
                for (int i = 0; i < p->l2Cache.l2CacheSets.size(); ++i)
                {p->l2Cache.l2CacheSets[i].frames.resize(p->l2Cache.setSize);}

                getline(configFile, line);
                p->l2Cache.lineSize = atoi( line.substr(line.find(":") + 2).c_str() );
            
                getline(configFile, line);
                //grab 1st character from returned string (should be either 'y' or 'n')
                p->l2Cache.wt_nwa = line.substr(line.find(":") + 2).c_str()[0]; 

                p->l2Cache.numOfIndexBits = log2(p->l2Cache.numOfSets);
                p->l2Cache.numOfOffsetBits = log2(p->l2Cache.lineSize);
            }

            else if (line.find("Virtual addresses:") != string::npos) 
            {
                p->allowVA = line.substr(line.find(":") + 2).c_str()[0];
            }

            else if (line.find("TLB:") != string::npos) 
            {
                p->allowTLB = line.substr(line.find(":") + 2).c_str()[0];
            }

            else if (line.find("L2 cache:") != string::npos) 
            {
                p->allowL2Cache = line.substr(line.find(":") + 2).c_str()[0];
            }
        }
    }
    else
    {
        printf("ERROR: \"trace.config\" could not be opened. Exiting...\n");
        exit(1);
    }
    configFile.close();

    //PRINT ALL CONFIGURATION FILE INFORMATION---------------------------------------------------------
    p->printAllConfigInfo();

    //PRINT "TABLE HEADER" OF HIEARCHY RESULTS-----------------------------------------------------------
    {
        if (p->allowVA == 'y') 
        {
            printf("Virtual  Virt.  Page TLB    TLB TLB  PT   Phys        DC  DC          L2  L2\n");
            printf("Address  Page # Off  Tag    Ind Res. Res. Pg # DC Tag Ind Res. L2 Tag Ind Res.\n");
        }
        
        else
        {
            printf("Physical Virt.  Page TLB    TLB TLB  PT   Phys        DC  DC          L2  L2\n");
            printf("Address  Page # Off  Tag    Ind Res. Res. Pg # DC Tag Ind Res. L2 Tag Ind Res.\n");
        }                      
            
            printf("-------- ------ ---- ------ --- ---- ---- ---- ------ --- ---- ------ --- ----\n");
    }

    //PROCESS "TRACE.DAT" INPUT STREAM. PRINT "TABLE CONTENTS" OF THOSE RESULTS------------------------
    while (cin >> line)
    {
        //increment LRUcounter once for every input file line read in
        LRUcounter += 1;

        //process read/write type and increment appropriate counter
        p->accessType = line[0];
        if (tolower(p->accessType) == 'r') p->readTotal++;
        else if (tolower(p->accessType) == 'w') p->writeTotal++;

        line = line.substr( line.find(":") + 1 );//line is now just reference address
        //Error check to ensure 32 bit address
        // if (line.length() > 8) // (8 hex digits) * (4 bits / hex digit) == 32 bits 
        // {
        //     printf("ERROR: reference address > 32 bits! NEED TO CHANGE THIS PRINT STATEMENT!!\n");
        //     exit(0);
        // }

        //store HEX and BINARY representations of file line address
        p->lineHex = line; // assign unaltered hex line
        p->lineBinary = hexToBinary(line); //assigned unaltered binary line
        p->lineHex32 = line.insert(0, 8 - line.length(), '0'); // 8 == # of hex digits in 32 bit address
        p->lineBinary32 = hexToBinary(p->lineHex32);
        p->lineInt = binaryToInt(p->lineBinary);

        //INPUT ADDRESSES == physical address
        if (p->allowVA == 'n')
        {
            //check if physical address bits exceed 
            if ( (float)log2(p->lineInt) > (float)(log2(p->pTable.numOfPPs) + p->pTable.numOfPageOffsetBits) ) 
            {printf("hierarchy: physical address %s is too large\n", p->lineHex.c_str()); exit(1);}

            //split physical address into PFN and page offset
            str1Str2 = splitStr(p->lineBinary32, p->pTable.numOfPageOffsetBits);
            p->pTable.ppnBinaryStr = str1Str2.first;
            p->pTable.pageOffsetBinaryStr = str1Str2.second;

            //split physical addresses into dCache Tag, Index, and block offset
            str1Str2 = splitStr(p->lineBinary32, p->dCache.numOfOffsetBits);
            p->dCache.offsetBinaryStr = str1Str2.second;
            line = str1Str2.first;//store binary string containing tag and index bits, split again
            str1Str2 = splitStr(line, p->dCache.numOfIndexBits);
            p->dCache.indexBinaryStr = str1Str2.second;
            p->dCache.tagBinaryStr = str1Str2.first;
            p->dCache.index = binaryToInt(p->dCache.indexBinaryStr);//dCache index integer
            p->dCache.tag = binaryToInt(p->dCache.tagBinaryStr);//dCache tag integer

            if (p->allowL2Cache == 'n')
            {
                //resolve dCache results
                // p->s = &p->dCache.dCacheSets[p->dCache.index];//pointer to indexed set
                // p->s->updateCacheAndHierarchy(p->dCache.tag, p->dCache.index, p->dCache.wt_nwa, LRUcounter, p, "dCache");
                p->s = &p->dCache.dCacheSets[p->dCache.index];//pointer to indexed set
                p->s->updateCacheAndHierarchy(p->dCache.tag, p->dCache.index, p->dCache.wt_nwa, LRUcounter, p, "dCache");

                //final print
                printf("%08x %6s %4x %6s %3s %4s %4s %4x %6x %3x %-4s %6s %3s %4s\n",
                       binaryToInt(p->lineBinary32), "", 
                       binaryToInt(p->pTable.pageOffsetBinaryStr), "", "", "", "",
                       binaryToInt(p->pTable.ppnBinaryStr),
                       p->dCache.tag,
                       p->dCache.index,
                       p->dCache.resultStr.c_str(), "", "",
                       p->l2Cache.resultStr.c_str());
            }
            else if (p->allowL2Cache == 'y')
            {
                //split physical addresses into L2 Tag, Index, and block offset
                str1Str2 = splitStr(p->lineBinary32, p->l2Cache.numOfOffsetBits);
                p->l2Cache.offsetBinaryStr = str1Str2.second;
                line = str1Str2.first;//store binary string containing tag and index bits, split again
                str1Str2 = splitStr(line, p->l2Cache.numOfIndexBits);
                p->l2Cache.indexBinaryStr = str1Str2.second;
                p->l2Cache.tagBinaryStr = str1Str2.first;
                p->l2Cache.index = binaryToInt(p->l2Cache.indexBinaryStr);//l2Cache index integer
                p->l2Cache.tag = binaryToInt(p->l2Cache.tagBinaryStr);//l2Cache tag integer

                //NOTES: Ok, here's my assumption --> Check L1 first. If L1 hit, then L2 gets updated depending on L1 write-policy. If L1 miss, 
                //       ...then L1 is unaltered and we check L2. If L2 hit, then main memory gets updated depending on L2 write policy. If L2 miss,
                //       ...then we go to main memory and pull data into L1.
                //IN OTHER WORDS: Data never flows from L2 to L1. Only from L1 to L2. For completion, "L1 -> L2 -> mainMem -> back to L1" loop
                    
                //resolve dCache, l2Cache results
                //REVIEW: I need to check if the READ lines will be handled correctly by the mess below
                p->dCache.resultStr = "";
                p->l2Cache.resultStr = "";
                p->s = &p->dCache.dCacheSets[p->dCache.index];//pointer to indexed set
                p->s->updateCacheAndHierarchy(p->dCache.tag, p->dCache.index, p->dCache.wt_nwa, LRUcounter, p, "dCache");

                //final print
                if (p->l2Cache.resultStr == "")
                {
                    printf("%08x %6s %4x %6s %3s %4s %4s %4x %6x %3x %-4s %6s %3s %-4s\n",
                        binaryToInt(p->lineBinary32), "", 
                        binaryToInt(p->pTable.pageOffsetBinaryStr), "", "", "", "",
                        binaryToInt(p->pTable.ppnBinaryStr),
                        p->dCache.tag,
                        p->dCache.index,
                        p->dCache.resultStr.c_str(), 
                        "", 
                        "",
                        p->l2Cache.resultStr.c_str());
                }
                else
                {
                        printf("%08x %6s %4x %6s %3s %4s %4s %4x %6x %3x %-4s %6x %3x %-4s\n",
                        binaryToInt(p->lineBinary32), "", 
                        binaryToInt(p->pTable.pageOffsetBinaryStr), "", "", "", "",
                        binaryToInt(p->pTable.ppnBinaryStr),
                        p->dCache.tag,
                        p->dCache.index,
                        p->dCache.resultStr.c_str(), 
                        p->l2Cache.tag, 
                        p->l2Cache.index,
                        p->l2Cache.resultStr.c_str());
                }
            }
        }

        //INPUT ADDRESSES == virtual address 
        else if (p->allowVA == 'y')
        {
            //DEBUG
            //p->debug = 'p';
            // p->debug = 'r';
            p->upperB = 10;
            p->lowerB = 0;

            //check if physical address bits exceed 
            if ( (float)log2(p->lineInt) > (float)(log2(p->pTable.numOfVPs) + p->pTable.numOfPageOffsetBits) ) 
            {printf("hierarchy: virtual address %s is too large\n", p->lineHex.c_str()); exit(1);}

            //split virtual address: Virtual Page Number (VPN) and Page Offset (PO)
            str1Str2 = splitStr(p->lineBinary32, p->pTable.numOfPageOffsetBits);
            p->pTable.vpnBinaryStr = str1Str2.first;
            //p->pTable.pageTableIndexBinaryStr = p->pTable.vpnBinaryStr;//NOTE: probaly not necessary
            p->pTable.pageOffsetBinaryStr = str1Str2.second;
            p->pTable.vpn = binaryToInt(p->pTable.vpnBinaryStr);

            //split VPN: TLB tag address and TLB index address
            str1Str2 = splitStr(p->pTable.vpnBinaryStr, p->dTLB.numOfIndexBits);
            p->dTLB.tagBinaryStr = str1Str2.first;
            p->dTLB.indexBinaryStr = str1Str2.second;
            p->dTLB.tag = binaryToInt(p->dTLB.tagBinaryStr);
            p->dTLB.index = binaryToInt(p->dTLB.indexBinaryStr);

            //split page offset: L1 cache index address and block offset address 
            str1Str2 = splitStr(p->pTable.pageOffsetBinaryStr, p->dCache.numOfOffsetBits);
            p->dCache.indexBinaryStr = str1Str2.first;
            p->dCache.offsetBinaryStr = str1Str2.second;
            p->dCache.index = binaryToInt(p->dCache.indexBinaryStr);//dCache index integer

            //DEBUG
            if ( (p->debug == 'p' || p->debug == 'o' || p->debug == 'e') && p->upperB >= LRUcounter && p->lowerB <= LRUcounter)
            {printf("Printing PT(START)\n"); p->printObj("PT", p);}
            if ( (p->debug == 't' || p->debug == 'o' || p->debug == 'e') && p->upperB >= LRUcounter && p->lowerB <= LRUcounter)
            {printf("Printing TLB(START)\n"); p->printObj("TLB", p);}
  
            //create PPN --- NOTE:Ithink 
            // if (p->allowRoundRobin == 'y') 
            // {p->pTable.ppn = p->pTable.PPNassign(p);}//NOTE: this function determines when ROUND ROBIN PHASE has ended!
            
            //TLB: 
            p->s_TLB = &p->dTLB.TLB[p->dTLB.index];
            p->f_TLB = p->s_TLB->findFrameTag(p->dTLB.tag, "TLB");

            //NOTE: This will happen if PT is bigger than TLB
            if (p->f_TLB == NULL)//TLB MISS: TLB full
            {
                //update TLB statistics
                p->dTLB.misses++; 
                p->dTLB.resultStr = "miss";

                //create PPN if in ROUND ROBIN PHASE
                //NOTE: PT could still not be full yet. Keep this here
                if (p->allowRoundRobin == 'y') 
                {p->pTable.ppn = p->pTable.PPNassign(p);}//NOTE: this function determines when ROUND ROBIN PHASE has ended!

                //check PT 
                p->result = p->pTable.accessPT(p, LRUcounter);//NOTE: don't know if I need to store result yet!

                //PAGE FAULT: Invalidate TLB entry and dCache/l2Cache lines associated with EVICTED PAGE
                //            The indices for these items are in "p->pageFault_evicted....." variables
                if (p->pTable.pageFaultOccurred == 'y')
                {
                    //NOTE: It makes sense to not have to invalidate if items are already gone form data stucture
                    //RESET TLB ENTRY
                    p->s_evictedTLB = &p->dTLB.TLB[p->pageFault_evictTLBindex]; 
                    p->f_evictedTLB = p->s_evictedTLB->findFrameTag(p->pageFault_evictTLBtag, "TLB");
                    //If TLB entry associated with EVICTED PAGE is still in TLB, reset to default
                    if (p->f_evictedTLB != NULL) resetFrame(p->f_evictedTLB);

                    //RESET D-CACHE LINE
                    p->s = &p->dCache.dCacheSets[p->pageFault_evictdIndex];//pointer to indexed set
                    p->f = p->s->findFrameTag(p->pageFault_evictdTag, "dCache");
                    //If dCache line associated with EVICTED PAGE is still in dCache, reset to default
                    if (p->f != NULL) resetFrame(p->f);

                    if (p->allowL2Cache == 'y')
                    {
                        //RESET D-CACHE LINE
                        p->s = &p->l2Cache.l2CacheSets[p->pageFault_evictl2Index];//pointer to indexed set
                        p->f = p->s->findFrameTag(p->pageFault_evictl2Tag, "l2Cache");
                        //If l2Cache line associated with EVICTED PAGE is still in l2Cache, reset to default
                        if (p->f != NULL) resetFrame(p->f);
                    }

                    //reset flag to complete invalidation process
                    p->pTable.pageFaultOccurred == 'n';
                }

                //find LRU TLB frame to evict
                //NOTE: Do I still find LRU TLB frame even though I just invalidated one?
                p->f_TLB = p->s_TLB->findLRUFrame();
                
                //update TLB
                p->f_TLB->TLBdata = p->pte->ppn;//p->pte holds updated PT entry data. Thanks accessPT()!
                p->f_TLB->TLBindex = p->dTLB.index;
                p->f_TLB->TLBtag = p->dTLB.tag;


                p->dCache.tag = p->f_TLB->TLBdata;
                p->s = &p->dCache.dCacheSets[p->dCache.index];//pointer to indexed set
                p->result = p->s->updateCacheAndHierarchy(p->dCache.tag, p->dCache.index, p->dCache.wt_nwa, LRUcounter, p, "dCache");
                //NOTE: I don't believe we need to do anything with the return result from updateCacheAndHierachy(..), but just in case...
                if (p->result == 'h') {/*DO NOTHING --- DATA IS JUST SENT TO CPU*/} //TLB data hit in cache
                else {/*From the notes, it seems like updateCacheAndHierachy(..) should handle everything from here*/}//TLB data miss in cache

                //DEBUG
                if ( (p->debug == 'p' || p->debug == 'o' || p->debug == 'e') && p->upperB >= LRUcounter && p->lowerB <= LRUcounter)
                {printf("Printing PT(TLB MISS: NULL)\n"); p->printObj("PT", p);}
                if ( (p->debug == 't' || p->debug == 'o' || p->debug == 'e') && p->upperB >= LRUcounter && p->lowerB <= LRUcounter)
                {printf("Printing TLB(TLB MISS: NULL)\n"); p->printObj("TLB", p);}
            }
            
            else if (p->f_TLB->TLBtag == -1)//TLB MISS: TLB not full
            {
                //update TLB statistics
                p->dTLB.misses++; 
                p->dTLB.resultStr = "miss";

                //create PPN if in ROUND ROBIN PHASE
                if (p->allowRoundRobin == 'y') 
                {p->pTable.ppn = p->pTable.PPNassign(p);}//NOTE: this function determines when ROUND ROBIN PHASE has ended!

                //
                p->result = p->pTable.accessPT(p, LRUcounter);//NOTE: don't know if I need to store result yet!


                //update TLB
                //p->f_TLB->TLBdata = p->pTable.ppn;
                p->f_TLB->TLBdata = p->pte->ppn;//p->pte holds updated PT entry data. Thanks accessPT()!
                p->f_TLB->TLBindex = p->dTLB.index;
                p->f_TLB->TLBtag = p->dTLB.tag;

                p->dCache.tag = p->f_TLB->TLBdata;
                p->s = &p->dCache.dCacheSets[p->dCache.index];//pointer to indexed set
                p->result = p->s->updateCacheAndHierarchy(p->dCache.tag, p->dCache.index, p->dCache.wt_nwa, LRUcounter, p, "dCache");
                //NOTE: I don't believe we need to do anything with the return result from updateCacheAndHierachy(..), but just in case...
                if (p->result == 'h') {/*DO NOTHING --- DATA IS JUST SENT TO CPU*/} //TLB data hit in cache
                else {/*From the notes, it seems like updateCacheAndHierachy(..) should handle everything from here*/}//TLB data miss in cache

                //DEBUG
                if ( (p->debug == 'p' || p->debug == 'o' || p->debug == 'e') && p->upperB >= LRUcounter && p->lowerB <= LRUcounter)
                {printf("Printing PT(TLB MISS: -1)\n"); p->printObj("PT", p);}
                if ( (p->debug == 't' || p->debug == 'o' || p->debug == 'e') && p->upperB >= LRUcounter && p->lowerB <= LRUcounter)
                {printf("Printing TLB(TLB MISS: -1)\n"); p->printObj("TLB", p);}
            }
            
            else//TLB HIT: simply send results onto Cache Hiearchy
            {
                //update TLB statistics
                p->dTLB.hits++; 
                p->dTLB.resultStr = "hit";

                p->result = p->pTable.accessPT(p, LRUcounter);//NOTE: don't know if I need to store result yet!

                //update PT statistics
                p->pTable.resultStr = "";

                p->dCache.tag = p->f_TLB->TLBdata;
                p->s = &p->dCache.dCacheSets[p->dCache.index];//pointer to indexed set
                p->result = p->s->updateCacheAndHierarchy(p->dCache.tag, p->dCache.index, p->dCache.wt_nwa, LRUcounter, p, "dCache");
                //NOTE: I don't believe we need to do anything with the return result from updateCacheAndHierachy(..), but just in case...
                if (p->result == 'h') {/*DO NOTHING --- DATA IS JUST SENT TO CPU*/} //TLB data hit in cache
                else {/*From the notes, it seems like updateCacheAndHierachy(..) should handle everything from here*/}//TLB data miss in cache
                
                //DEBUG
                if ( (p->debug == 'p' || p->debug == 'o' || p->debug == 'e') && p->upperB >= LRUcounter && p->lowerB <= LRUcounter)
                {printf("Printing PT(TLB HIT)\n"); p->printObj("PT", p);}
                if ( (p->debug == 't' || p->debug == 'o' || p->debug == 'e') && p->upperB >= LRUcounter && p->lowerB <= LRUcounter)
                {printf("Printing TLB(TLB HIT)\n"); p->printObj("TLB", p);}
            }

            //DEBUG
            if ( (p->debug == 'r') && p->upperB >= LRUcounter && p->lowerB <= LRUcounter)
            {printf("Printing PT(roundRobin IF)\n"); p->printObj("PT", p);}


            //final print
            if (p->l2Cache.resultStr == "")
            {
                printf("%08x %6x %4x %6x %3x %-4s %-4s %4x %6x %3x %-4s %6s %3s %-4s\n",
                    binaryToInt(p->lineBinary32), 
                    p->pTable.vpn, 
                    binaryToInt(p->pTable.pageOffsetBinaryStr), 
                    p->dTLB.tag, 
                    p->dTLB.index, 
                    p->dTLB.resultStr.c_str(), 
                    p->pTable.resultStr.c_str(),
                    p->pte->ppn,
                    p->dCache.tag,
                    p->dCache.index,
                    p->dCache.resultStr.c_str(), 
                    "", 
                    "",
                    p->l2Cache.resultStr.c_str());
            }
        }
    }

    //PRINT "SIMULATION RESULTS"-------------------------------------------------------------------------
    printf("\nSimulation statistics\n\n");
    
    printf("dtlb hits        : %d\n", p->dTLB.hits);
    printf("dtlb misses      : %d\n", p->dTLB.misses);
    if (p->allowTLB == 'y') printf("dtlb hit ratio   : %f\n\n", (double)p->dTLB.hits / (double)(p->dTLB.hits + p->dTLB.misses) );
    else printf("dtlb hit ratio   : N/A\n\n");

    printf("pt hits          : %d\n", p->pTable.hits);
    printf("pt faults        : %d\n", p->pTable.faults);
    if (p->allowVA == 'y') printf("pt hit ratio     : %f\n\n", (double)p->pTable.hits / (double)(p->pTable.hits + p->pTable.faults) );
    else printf("pt hit ratio     : N/A\n\n");

    printf("dc hits          : %d\n", p->dCache.hits);
    printf("dc misses        : %d\n", p->dCache.misses);
    printf("dc hit ratio     : %f\n\n", (double)(p->dCache.hits)/(double)(p->dCache.hits + p->dCache.misses));

    printf("L2 hits          : %d\n", p->l2Cache.hits);
    printf("L2 misses        : %d\n", p->l2Cache.misses);
    if (p->allowL2Cache == 'y') printf("L2 hit ratio     : %f\n\n", (double)p->l2Cache.hits / (double)(p->l2Cache.hits + p->l2Cache.misses));
    else printf("L2 hit ratio     : N/A\n\n");

    printf("Total reads      : %d\n", p->readTotal);
    printf("Total writes     : %d\n", p->writeTotal);
    printf("Ratio of reads   : %f\n\n", (double)(p->readTotal)/(double)(p->readTotal + p->writeTotal));

    printf("main memory refs : %d\n", p->mainMemoryRefs);
    printf("page table refs  : %d\n", p->pageTableRefs);
    printf("disk refs        : %d\n", p->diskRefs);

    //free memory and return
    delete p;
    return 0;
  
}

//OLD CODE REPOSITORY--------------------------------------------------------------------------------

// if (p->pTable.ppn == p->pTable.numOfPPs - 1) printf("%d\n", p->pTable.ppn);
// else printf("%d ", p->pTable.ppn);


                // p->allowWriteOnMiss = 'n';

                // p->s = &p->dCache.dCacheSets[p->dCache.index];//pointer to indexed set in dCache
                // if (p->s->updateCacheAndHierarchy(p->dCache.tag, p->dCache.index, p->dCache.wt_nwa, LRUcounter, p, "dCache") == 'h')//"Hit" in dCache.
                // {
                //     if (p->dCache.wt_nwa == 'y' && tolower(p->accessType) == 'w')// Write-through policy == write to L2.
                //     {
                //         //NOTE: l2CacheCheckedAndMissed = 'y' is needed to bypass early return on misses!
                //         p->allowWriteOnMiss = 'y';//NOTE: Need to come up with a better name for this variable
                //         p->s = &p->l2Cache.l2CacheSets[p->l2Cache.index];//pointer to indexed set in l2Cache
                //         p->s->updateCacheAndHierarchy(p->l2Cache.tag, p->l2Cache.index, p->l2Cache.wt_nwa, LRUcounter, p, "l2Cache");
                //     }
                //     // else if (tolower(p->accessType) == 'r')//read hit
                //     // {

                //     // }
                // }
                // else //"miss" in dCache. updateSetAndHiearchy(...) did NOT update dCache block. 
                // {
                    
                //     if (p->dCache.wt_nwa == 'n' && tolower(p->accessType) == 'w')// Write-back policy == write to L1 and write DIRTY evicted frame to L2.
                //     {
                //         p->allowWriteOnMiss = 'y';//Change flag to allow update to L1 and update to L2 via recursive call
                //         p->s->updateCacheAndHierarchy(p->dCache.tag, p->dCache.index, p->dCache.wt_nwa, LRUcounter, p, "dCache");
                //     }
                //     else if (p->dCache.wt_nwa == 'y' && tolower(p->accessType) == 'w')
                //     {
                //         p->allowWriteOnMiss = 'y';//Changing flag to allow update to L2 
                //         p->s->updateCacheAndHierarchy(p->dCache.tag, p->dCache.index, p->dCache.wt_nwa, LRUcounter, p, "dCache");
                //         p->s->updateCacheAndHierarchy(p->l2Cache.tag, p->l2Cache.index, p->l2Cache.wt_nwa, LRUcounter, p, "l2Cache");
                //     }
                //     else if (tolower(p->accessType) == 'r')//read hit
                //     {
                //         p->allowWriteOnMiss = 'y';//Change flag to allow update to L1 and update to L2 via recursive call
                //         p->s->updateCacheAndHierarchy(p->dCache.tag, p->dCache.index, p->dCache.wt_nwa, LRUcounter, p, "dCache");
                //     }


                //     //else if(???)
                //     {
                //         // //NOTE: At this point, I'm assuming a "hit" in L2 means nothing for L1 (i.e., it is not updated to correspond to L2)
                //         // //      This means I only care about a "miss" in L2, which means I need to let dCache proceed unhindered (i.e., change l2CacheChecked flag)
                //         // p->s = &p->l2Cache.l2CacheSets[p->l2Cache.index];//pointer to indexed set in l2Cache
                //         // if (p->s->updateCacheAndHierarchy(p->l2Cache.tag, p->l2Cache.index, p->l2Cache.wt_nwa, LRUcounter, p, "l2Cache") == 'm')
                //         // {
                //         //     //PROBLEM: At this point, L2 will be updated b/c I don't have a check to stop this.
                //         //     //FIXED: Took out (cache == "dCache" && ...) check so that only "l2CacheChecked == 'y'" will allow misses to continue to update caches
                //         //     p->allowWriteOnMiss = 'y';


                //         //     p->s->updateCacheAndHierarchy(p->dCache.tag, p->dCache.index, p->dCache.wt_nwa, LRUcounter, p, "dCache");
                //         //     //QUESTION: Do I stop here and not update L2? I know newTag is already a "miss" in dCache!!! Which means I ONLY write to L2....

                //         //     // if (p->dCache.wt_nwa == 'y' && tolower(p->accessType) == 'w')// Write-through policy == write to L2.
                //         //     // {
                //         //     //     p->s = &p->l2Cache.l2CacheSets[p->l2Cache.index];//pointer to indexed set in l2Cache
                //         //     //     p->s->updateCacheAndHierarchy(p->l2Cache.tag, p->l2Cache.wt_nwa, LRUcounter, p, "l2Cache");
                //         //     // }
                //     }
                // }


//TEST
// printf("p->lineBinary32: %s\n", p->lineBinary32.c_str());
// printf("PFN:             %s\n", str1Str2.first.c_str());
// printf("page offset:     %s\n", str1Str2.second.c_str());
// printf("\n");


// int k = binaryStrToInt("1000");
// printf("k: %d\n", k);
// k = binaryStrToInt("1000 1000");
// printf("k: %d\n", k);
// k = binaryStrToInt("10001000");
// printf("k(int): %d    k(hex): 0x%x --- it works! \n", k, k);
// exit(0);

