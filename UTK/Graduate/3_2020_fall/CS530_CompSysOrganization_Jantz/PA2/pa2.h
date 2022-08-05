#include <cstdlib>
#include <iostream>
#include <sstream> 
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <cstdio>
#include <cstdio>

using namespace std;

class pa2
{
    public:
        class instruction
        {
            public:
                //TRACE.DAT LOOP------------------------------------------
                int executeLatency = -1; //holds cycle latency which decrements in execute state
                //
                string fullInstruction;
                string rsName;//name of associated reservation station 
                string op, destRegister, sourceRegister1, sourceRegister2, branchLabel;
                int address = -1;

                //PROCESSOR LOOP-----------------------------------------
                //cycle numbers. Used for FINAL PRINT
                long long issue = -1, executeStart = -1, executeEnd = -1;
                long long memoryRead = -1, writeResult = -1, commit = -1;

                //NOTe: Not sure if these will be needed
                instruction *dependence1 = NULL; 
                instruction *dependence2 = NULL; 

                //pointer to instructions own ROBentry and RS entry
                //ROBentry *rEntry; //can't have this b/c class is below
                void *ROBe = NULL; //Must cast this to ROBentry pointer
                void *RSe = NULL; //Must cast this to RS pointer
        };

        class ROBentry
        {
            public:
                class ROBvalue
                {
                    public:
                        ROBentry *value1 = NULL;
                        ROBentry *value2 = NULL;

                        ROBentry *sameMemoryAccess = NULL;//This ROBe is dependent on sameMemoryAccess's memory access

                        //These instructions are dependent on destination reg data from this ROBe
                        ROBentry *dependentValue1 = NULL;
                        ROBentry *dependentValue2 = NULL;
                        ROBentry *dependentSameMemoryAccess = NULL;//for memory address dependence
                };

                int entry = -1;//index number into ROB array.
                char busy = 'n';//busy == 'y' until stage "commited" is reached
                instruction *i = NULL;
                string state;//issue, execute, memoryRead, writeResult, commit
                string destination;

                //pointer to corresponding RS entry
                void *RSe = NULL; //Must cast this to pa2::RS pointer

                ROBvalue value;

                //Load instructions
                bool memoryRW_Available = false;
                int memoryReadCycle = -1;

                //booleans to handle state transitions
                bool IssueToExecuteSameCycle = false;
                bool ExecuteToMemoryreadSameCycle = false;
                bool ExecuteToCommitSameCycle = false;
                bool ExecuteToWriteResultsSameCycle = false;
                bool MemoryreadToWriteresultSameCycle = false;
                bool WriteresultToCommitSameCycle = false;

                //boolean to handle data availability
                bool resultsAvailable = false;
                bool removeFromRS = false;
        };

        //reservation station (RS)
        //Each RS is actually just one entry in an array
        class RS 
        {
            public:
                string name; 
                char busy = 'n'; 
                string op, Vj, Vk;

                instruction *i = NULL;

                ROBentry * Qj = NULL;
                ROBentry * Qk = NULL;
                ROBentry * Dest = NULL;

                int address = -1;
        };

        //Circular Queue class
        class cQueue 
        {
            public:
                int rear, front; 
                int size; 

                int runningIndex;

                ROBentry ** cqArray; //pointer to an array of ROBentry pointers
            
                cQueue(int s) 
                { 
                    front = rear = -1; 
                    size = s; 
                    cqArray = new ROBentry* [s]; 
                    for (int i = 0; i < size; ++i) cqArray[i] = NULL;
                } 
            
                int pushBack(ROBentry *e); 
                ROBentry * popFront(); 
                bool isFull();
        };

};
