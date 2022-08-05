using namespace std;

class pa1
{
    public:
        //NESTED CLASSES----------------------------------------------------
        class frame
        {
            public:
                int dirtyBit = 0;

                unsigned long long LRUnum = 0;//If frame is accessed, reset to 0 unless evicted. If frame not accessed, assign LRUcounter value associated with containing data structure. 
                
                //Used only with TLB
                int TLBtag = -1;
                int TLBindex = -1;
                int TLBdata = -1;

                //Only used with L1/L2 caches
                int dTag = -1;//dCache tag
                int dIndex = -1;//dCache index
                int l2Tag = -1;//l2Cache tag
                int l2Index = -1;//l2Cache index
        };

        class set
        {
            public:
                vector<frame>frames;
                
                frame* findFrameTag(int tag, string cache);
                frame* findLRUFrame();
                char updateCacheAndHierarchy(int newTag, int newIndex, char wt_nwa, unsigned long long LRUcounter, pa1 *p, string cache);
        };

        class PTentry//NOTE: Not sure I need more than PPN integer, but just in case
        {
            public:
                
                int vpn = -1;
                int ppn = -1;//-1 means empty

                string ppnStr = "";

                int dirtyBit = 0;//0 == not dirty, 1 == dirty(needs writing to disk b4 eviction)
                int pageValid = 1;//1 == valid, 0 == not valid. Look at "TLB" topic in discussions.

                int LRUnum;

                //eviction variables
                //Used only with TLB
                int TLBtag = -1;
                int TLBindex = -1;
                int TLBdata = -1;

                //Only used with L1/L2 caches
                int dTag = -1;//dCache tag
                int dIndex = -1;//dCache index
                
                int l2Tag = -1;//l2Cache tag
                int l2Index = -1;//l2Cache index
        };

        class dataTLB
        {
            public:
                unsigned int setSize, numOfSets, numOfIndexBits;
                unsigned int hits = 0;
                unsigned int misses = 0;
                unsigned int tag, index, data;

                double hitRatio;
                
                string tagBinaryStr = "";
                string indexBinaryStr = "";
                string dataBinaryStr = "";//equals L1 tag/PPN
                string resultStr = "";

                void printConfigInfo();

                vector<set>TLB;
        };  

        //PageTable only used to convert virtual to physical addresses
        //if virtual is disabled, then this is NOT used!
        class pageTable
        {
            public:
                //virtual page variables
                unsigned int numOfVPs;
                unsigned int numOfVpnBits;
                unsigned int vpn;
                string vpnBinaryStr;//virtual page number

                //physical address variables
                unsigned int numOfPPs;
                unsigned int ppn;
                string ppnBinaryStr;//physical frame number

                //shared variables
                char pageFaultOccurred = 'n';

                unsigned int pageSize;
                unsigned int hits = 0;
                unsigned int faults = 0;//tlb misses == pt hits + pt faults
                unsigned int numOfPageTableIndexBits;
                unsigned int numOfPageOffsetBits;
                int roundRobin = -1;//starts @ -1 to be 0 on first ++
                
                double hitRatio;
                
                string roundRobinStr = "";
                string ppnStr = "";
                string pageTableIndexBinaryStr = "";
                string pageOffsetBinaryStr = "";
                string resultStr = "";

                void printConfigInfo();
                int PPNassign(pa1 *p);
                PTentry * findTranslation(pa1 *p);
                PTentry * findLRU_PTE();
                char accessPT(pa1 *p, unsigned long long LRUcounter);

                vector<PTentry>PT;//turn VPN into integer index, int stored inside is PPN
                PTentry evictedPage;
        };

        class dataCache
        {
            public:
                char wt_nwa;//wt_nwa stands for "write-through, no-write-allocate". if wt_nwa == 'n', then "write-back, write-allocate" is implemented. Otherwise, "write-through, no-write-allocate"

                unsigned int numOfSets, setSize, lineSize;
                unsigned int hits = 0;
                unsigned int misses = 0;
                unsigned int numOfIndexBits;
                unsigned int numOfOffsetBits;
                unsigned int tag, index;

                double hitRatio;

                string tagBinaryStr = "";
                string indexBinaryStr = "";
                string offsetBinaryStr = "";
                string resultStr = "";

                void printConfigInfo();

                vector<set>dCacheSets;//size == numOfSets

                // //NOTE: Sad news, this is a bad idea. I only need to evict frames WITHIN A PARTICULAR SET. This structure puts frames from ALL SETS in which will not work!!
                // //->LRU.begin()/end().first == LRUnum associated with frame*. Note, LRUcounter increments +1 for every dCache access while dCacheSets[dCacheIndex].frames[?].LRUnum increments +1 IF that particular frame is accessed.
                // //->LRU.begin()/end().second == pointer to associated frame object within vector<frame>frames of set object. In order to evict, simply "LRU.begin()->second->tag = newTag;" or maybe LRU.end(), whichever has lowest key. NO NEED TO "ERASE"! In fact, I don't think I need a separate iterator at all!!
                // map<unsigned long long, set*> LRUset;
                // //map<int, frame*>::iterator LRUit;//almost sure this is NOT needed
        };

        class L2Cache
        {
            public:
                char wt_nwa; //wt_nwa stands for "write-through, no-write-allocate"
                                //if wt_nwa == 'n', then "write-back, write-allocate"...
                                //...is implemented. Otherwise, "write-through, no-write-allocate"

                unsigned int numOfSets, setSize, lineSize;
                unsigned int hits = 0;
                unsigned int misses = 0;
                unsigned int numOfIndexBits;
                unsigned int numOfOffsetBits;
                unsigned int tag, index;
                
                double hitRatio;
                
                string tagBinaryStr = "";
                string indexBinaryStr = "";
                string offsetBinaryStr = "";
                string resultStr = "";

                void printConfigInfo();

                vector<set>l2CacheSets;//size == numOfSets

        };

        //CLASS MEMBERS------------------------------------------------------
        //NOTE: if something is disabled, its final printout is BLANK
        char allowVA; //if "allowVA" == 'n', virtual to physical address... 
                      //..translation is disabled. 'y' means its enabled.
        char allowTLB; //if "TLB" == 'n', "data translation look-aside buffer"...
                       //...is disabled. 'y' means it's enabled. 
        char allowL2Cache; //if "L2Cache" == 'n', L2 cache is disabled. 
                           //'y' means it's enabled. 
        char accessType;
        
        char L1writeBackToL2 = 'n';//assign evicted frame's index/tag to "L1ToL2Index" 
        char L1writeThroughToL2 = 'n';

        char result;//free for all char result storage
        char allowRoundRobin = 'y';

        unsigned int readTotal = 0;
        unsigned int writeTotal = 0;
        unsigned int mainMemoryRefs = 0;//can be determined if one or both caches miss!!
        unsigned int pageTableRefs = 0;//pt hits + pt faults
        unsigned int diskRefs = 0;
        unsigned int lineInt;

        unsigned int evicted_dIndex;
        unsigned int evicted_dTag;
        unsigned int evicted_l2Index;
        unsigned int evicted_l2Tag;

        unsigned int pageFault_evictTLBindex;
        unsigned int pageFault_evictTLBtag;
        
        unsigned int pageFault_evictdIndex;
        unsigned int pageFault_evictdTag;
        
        unsigned int pageFault_evictl2Index;
        unsigned int pageFault_evictl2Tag;

        double readRatio;//readTotal / (readTotal + writeTotal)

        string lineHex;
        string lineBinary;
        string lineHex32;
        string lineBinary32;

        //INNER CLASS MEMBERS--------------------------------------------------
        set *s;//temporary storage of set pointers
        set *s_TLB;//temporary storage of set pointers
        set *s_evictedTLB;//temporary storage of set pointers
        frame *f;//temporary storage of frame pointers
        frame *f_TLB;//temporary storage of frame pointers
        frame *f_evictedTLB;//temporary storage of frame pointers
        PTentry *pte;//temporary storage of PTentry pointers
        dataTLB dTLB;
        pageTable pTable;
        dataCache dCache;
        L2Cache l2Cache;

        void printAllConfigInfo();
        static unsigned int isPowerOf2(unsigned int i);
        static void printObj(string s, pa1 *p);

        //DEBUG members
        char debug = 'n';//d == dCache, l == l2Cache, b == both L1/L2, p == PT, t == TLB, o == both PT/TLB, r == roundRobin only, n == none, e == everything
        unsigned long long lowerB;
        unsigned long long upperB;

};

