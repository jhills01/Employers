#include "pa2.h"

using namespace std;

//PRINTS------------------------------------------------------------------------------------------------------------
//print instructions that are dependent on ROBe results
void printROBeDependences(pa2::ROBentry *ROBe)
{
    if (ROBe == NULL) cout << "ROBe is NULL!!!\n";
    else if (ROBe->value.value1 != NULL) 
    {printf("value1: %s\n", ROBe->value.value1->i->fullInstruction.c_str());}
    else if (ROBe->value.value2 != NULL) 
    {printf("value2: %s\n", ROBe->value.value2->i->fullInstruction.c_str());}
    else if (ROBe->value.sameMemoryAccess != NULL) 
    {printf("sameMemoryAccess: %s\n", ROBe->value.sameMemoryAccess->i->fullInstruction.c_str());}
}

//print instructions that the ROBe depends on for results 
void printROBeDependents(pa2::ROBentry *ROBe)
{
    if (ROBe == NULL) cout << "ROBe is NULL!!!\n";
    else if (ROBe->value.dependentValue1 != NULL) 
    {printf("dependentValue1: %s\n", ROBe->value.dependentValue1->i->fullInstruction.c_str());}
    else if (ROBe->value.dependentValue2 != NULL) 
    {printf("dependentValue2: %s\n", ROBe->value.dependentValue2->i->fullInstruction.c_str());}
    else if (ROBe->value.dependentSameMemoryAccess != NULL) 
    {printf("dependentSameMemoryAccess: %s\n", ROBe->value.dependentSameMemoryAccess->i->fullInstruction.c_str());}
}

void printInstruction(pa2::instruction *i)
{
    cout << i->fullInstruction << endl;
    
    printf("%s-%s-%s-%s-%s-\n", 
    i->op.c_str(), i->destRegister.c_str(), i->sourceRegister1.c_str(), i->sourceRegister2.c_str(), 
    i->branchLabel.c_str());
    
    cout << i->executeLatency << " " << i->address << endl << endl;
}

void printReservationStationEntry(pa2::RS *rse)
{
    cout << "____________________________________________________________________________________\n";
    printf("%-10s %-5c %-10s %-10s %-20s ", 
    rse->name.c_str(), rse->busy, rse->op.c_str(), rse->Vj.c_str(), rse->Vk.c_str());

    if(rse->Qj != NULL) printf(" %-5d", rse->Qj->entry);
    else                printf("-NULL-");
    if(rse->Qk != NULL) printf("  %-5d", rse->Qk->entry);
    else                printf(" -NULL-");
    if(rse->Dest != NULL) printf("  %-5d", rse->Dest->entry);
    else                printf(" -NULL-");
    
    printf(" %-5d\n", rse->address);
}

//Prints ROBentry members. For "value", it simply prints the "entry" member to ensure correct entry is pointed to
void printROBentry(pa2::ROBentry *re)
{
    cout << "____________________________________________________________________________________\n";
    
    if (re == NULL) {cout << "ROBentry is NULL!!!\n";}
    else if (re->i == NULL)
    {
        cout << "instruction pointer is NULL! Not suppose to happen\n" << endl;
    }
    else if (re->value.value1 != NULL && re->value.value2 != NULL)
    {
        printf("%-5d %-5c %-30s %-10s %-3s  %-5d  %-5d\n",
        re->entry, re->busy, re->i->fullInstruction.c_str(), re->state.c_str(), re->destination.c_str(), 
        re->value.value1->entry, re->value.value2->entry);
    }
    else if (re->value.value1 == NULL && re->value.value2 != NULL)
    {
        printf("%-5d %-5c %-30s %-10s %-3s -NULL-  %-5d\n",
        re->entry, re->busy, re->i->fullInstruction.c_str(), re->state.c_str(), re->destination.c_str(), 
        re->value.value2->entry);
    }
    else if (re->value.value1 != NULL && re->value.value2 == NULL)
    {
        printf("%-5d %-5c %-30s %-10s %-3s  %-5d -NULL-\n",
        re->entry, re->busy, re->i->fullInstruction.c_str(), re->state.c_str(), re->destination.c_str(), 
        re->value.value1->entry);
    }
    else if (re->value.value1 == NULL && re->value.value2 == NULL)
    {
        printf("%-5d %-5c %-30s %-10s %-3s -NULL- -NULL-\n",
        re->entry, re->busy, re->i->fullInstruction.c_str(), re->state.c_str(), re->destination.c_str());
    }
}

void printInstructionQueue(vector <pa2::instruction *> &iq)
{
    printf("\nPRINTING INSTRUCTION QUEUE:\n");
    for (int i = 0; i < iq.size(); ++i) printInstruction(iq[i]); 
}

void printReservationStation(vector <pa2::RS *> *rs)
{
    for (int i = 0; i < rs->size(); ++i) printReservationStationEntry((*rs)[i]);
}

void printAllReservationStations(vector < vector <pa2::RS *> * > &RSall)
{
    printf("\nPRINT ALL RESERVATION STATIONS:\n");
    printf("Name       Busy  Op         Vj         Vk                    Qj     Qk     Dest   A\n");
    for (int i = 0; i < RSall.size(); ++i) printReservationStation(RSall[i]);
}

//Prints all ROBentry's member data from circular Queue.
//For the "value" member, the "entry" integer is printed to ensure correct correlation to dependent ROBentry 
void printROB(pa2::cQueue *ROB)
{
    printf("\nPRINTING ROB:\n");
    printf("front: %d, rear: %d, size: %d\n", ROB->front, ROB->rear, ROB->size);
    printf("Entry Busy  Instruction                    State      Dest Value1 Value2\n");
    for (int i = 0; i < ROB->size; ++i) printROBentry(ROB->cqArray[i]); 
}

//ERROR CHECKS-------------------------------------------------------------------------------------------------------
void sourceRegisterCheck(pa2::instruction *i)
{
    if (i->sourceRegister1.size() == 0 || 
    (i->sourceRegister1.find("x") == string::npos &&  i->sourceRegister1.find("f") == string::npos) )
    {printf("%s: instruction has invalid register\n", i->fullInstruction.c_str()); exit(0);}
    else if (i->sourceRegister2.size() == 0 || 
    (i->sourceRegister2.find("x") == string::npos && i->sourceRegister2.find("f") == string::npos) )
    {printf("%s: instruction has invalid register\n", i->fullInstruction.c_str()); exit(0);}
    
    //int load/store refers to the wrong register type
    else if ( (i->op == "lw" || i->op == "sw") && i->sourceRegister1.find("f") != string::npos )
    {printf("%s: instruction has invalid register\n", i->fullInstruction.c_str()); exit(0);}
    else if ( (i->op == "flw" || i->op == "fsw") && i->sourceRegister1.find("f") != string::npos )
    {printf("%s: instruction has invalid register\n", i->fullInstruction.c_str()); exit(0);}

    else if ( (i->op == "lw" || i->op == "sw") && i->sourceRegister2.find("f") != string::npos )
    {printf("%s: instruction has invalid register\n", i->fullInstruction.c_str()); exit(0);}
    else if ( (i->op == "flw" || i->op == "fsw") && i->sourceRegister2.find("f") != string::npos )
    {printf("%s: instruction has invalid register\n", i->fullInstruction.c_str()); exit(0);}

    //int add/sub refers to wrong registers
    else if ( (i->op == "add" || i->op == "sub") && i->sourceRegister1.find("f") != string::npos )
    {printf("%s: instruction has invalid register\n", i->fullInstruction.c_str()); exit(0);}
    else if ( (i->op == "add" || i->op == "sub") && i->sourceRegister2.find("f") != string::npos )
    {printf("%s: instruction has invalid register\n", i->fullInstruction.c_str()); exit(0);}

    //fp add/sub/mul/div refers to wrong operators
    else if ( (i->op == "fadd.s" || i->op == "fsub.s" || i->op == "fmul.s" || i->op == "fdiv.s") && 
               i->sourceRegister1.find("x") != string::npos )
    {printf("%s: instruction has invalid register\n", i->fullInstruction.c_str()); exit(0);}
    
    else if ( (i->op == "fadd.s" || i->op == "fsub.s" || i->op == "fmul.s" || i->op == "fdiv.s") && 
               i->sourceRegister2.find("x") != string::npos )
    {printf("%s: instruction has invalid register\n", i->fullInstruction.c_str()); exit(0);}

    //control bne/beq refers to wrong registers
    //Only int registers are allowed
    else if ( (i->op == "beq" || i->op == "bne") && i->sourceRegister1.find("f") != string::npos )
    {printf("%s: instruction has invalid register\n", i->fullInstruction.c_str()); exit(0);}
    else if ( (i->op == "beq" || i->op == "bne") && i->sourceRegister2.find("f") != string::npos )
    {printf("%s: instruction has invalid register\n", i->fullInstruction.c_str()); exit(0);}
}

void destRegisterCheck(pa2::instruction *i)
{
    if (i->destRegister.size() == 0 || 
    (i->destRegister.find("x") == string::npos && i->destRegister.find("f") == string::npos) )
    {printf("%s: instruction has invalid register\n", i->fullInstruction.c_str()); exit(0);}

    //int load/store refers to the wrong register type
    else if ( (i->op == "lw" || i->op == "sw") && i->destRegister.find("f") != string::npos )
    {printf("%s: instruction has invalid register\n", i->fullInstruction.c_str()); exit(0);}
    else if ( (i->op == "flw" || i->op == "fsw") && i->destRegister.find("x") != string::npos )
    {printf("%s: instruction has invalid register\n", i->fullInstruction.c_str()); exit(0);}

    //int add/sub refers to wrong registers
    else if ( (i->op == "add" || i->op == "sub") && i->destRegister.find("f") != string::npos )
    {printf("%s: instruction has invalid register\n", i->fullInstruction.c_str()); exit(0);}

    //fp add/sub/mul/div refers to wrong operators
    else if ( (i->op == "fadd.s" || i->op == "fsub.s" || i->op == "fmul.s" || i->op == "fdiv.s") && 
               i->destRegister.find("x") != string::npos )
    {printf("%s: instruction has invalid register\n", i->fullInstruction.c_str()); exit(0);}
}

void opCheck(pa2::instruction *i)
{
    if (i->op.size() == 0 && 
        i->op != "lw" && i->op != "flw" && i->op != "sw" && i->op != "fsw" &&
        i->op != "add" && i->op != "sub" &&
        i->op != "beq" && i->op != "bne" &&
        i->op != "fadd.s" && i->op != "fsub.s" && i->op != "fmul.s" && i->op != "fdiv.s")
    {printf("unknown inst type: %s\n", i->fullInstruction.c_str()); exit(0);}
}

void addressCheck(pa2::instruction *i)
{
    if (i->address < 0)
    {printf("%s: instruction has invalid address\n", i->fullInstruction.c_str()); exit(0);}
}

// ':', '(', ')', ',', 'L'
void symbolsCheck(pa2::instruction *i)
{

    if (i->op == "flw" || i->op == "lw" || i->op == "fsw" || i->op == "sw")
    {
        if (i->fullInstruction.find(":") == string::npos || 
            i->fullInstruction.find(")") == string::npos ||
            i->fullInstruction.find("(") == string::npos)
        {printf("%s: could not parse load/store instruction\n", i->fullInstruction.c_str()); exit(0);}
    }
    
    if (i->op == "bne" || i->op == "beq")
    {
        if (i->fullInstruction.find("L") == string::npos)
        {printf("%s: could not parse branch instruction\n", i->fullInstruction.c_str()); exit(0);}
    }

    if (i->fullInstruction.find(",") == string::npos)
    {printf("%s: could not parse instruction\n", i->fullInstruction.c_str()); exit(0);}

}

void arithmaticInstructionCheck(pa2::instruction *i)
{
    //If string has size zero, it wasn't
    opCheck(i);
    symbolsCheck(i);
    sourceRegisterCheck(i);
    destRegisterCheck(i);
}

void branchInstructionCheck(pa2::instruction *i)
{
    opCheck(i);
    symbolsCheck(i);
    sourceRegisterCheck(i);
}

void loadStoreInstructionCheck(pa2::instruction *i)
{
    opCheck(i);
    symbolsCheck(i);
    sourceRegisterCheck(i);
    destRegisterCheck(i);
}

//HELPER FUNCTIONS---------------------------------------------------------------------------------------------------
//REVIEW: Unfortunately, this probably isn't needed. I already do that with rs->dest==NULL
bool isRSFull(vector <pa2::RS *> *rs)
{
    for (int i = 0; i < rs->size(); ++i)
    {
        if (  (*rs)[i]->op == "" ) return false; 
    }

    return true;
}

//Returns pointer to RS slot that is available, i.e., empty
//Else, NULL is returned
pa2::RS * issue_getAvailableRS(pa2::instruction *i, vector < vector <pa2::RS *> * > RSall)
{
    pa2::RS * rs;
    bool turnOn = false;

    //LOADs-------------------------------------------------------------------
    if (i->op == "lw" || i->op == "flw" || i->op == "sw" || i->op == "fsw")
    {   
        if ( isRSFull(RSall[0]) && turnOn) return NULL;
        
        for (int i = 0; i < RSall[0]->size(); ++i)
        {
            //extract reservation station (RS) entry
            rs = ( *RSall[0] )[i];
            
            //if Dest == NULL, then RS entry is vacant
            if (rs->Dest == NULL) {return rs;}            
        }    

        //available RS entry not found
        return NULL;
    }

    //fpADDs-----------------------------------------------------------------
    else if (i->op == "fadd.s" || i->op == "fsub.s")
    {
        if ( isRSFull(RSall[1]) && turnOn) return NULL;

        for (int i = 0; i < RSall[1]->size(); ++i)
        {
            //extract reservation station (RS) entry
            rs = ( *RSall[1] )[i];

            //if Dest == NULL, then RS entry is vacant
            if (rs->Dest == NULL) {return rs;}
        }  
        
        //available RS entry not found
        return NULL;
    }
    
    //fpMULs-----------------------------------------------------------------
    else if (i->op == "fmul.s" || i->op == "fdiv.s")
    {
        if( isRSFull(RSall[2]) && turnOn ) return NULL;

        for (int i = 0; i < RSall[2]->size(); ++i)
        {
            //extract reservation station (RS) entry
            rs = ( *RSall[2] )[i];
            
            //if Dest == NULL, then RS entry is vacant
            if (rs->Dest == NULL) {return rs;}
        }  
        
        //available RS entry not found
        return NULL;
    }
    
    //INTS-----------------------------------------------------------------
    else if (i->op == "add" || i->op == "sub" || i->op == "beq" || i->op == "bne")
    {
        if ( isRSFull(RSall[3]) && turnOn ) return NULL;

        for (int i = 0; i < RSall[3]->size(); ++i)
        {
            //extract reservation station (RS) entry
            rs = ( *RSall[3] )[i];
            
            //if Dest == NULL, then RS entry is vacant
            if (rs->Dest == NULL) {return rs;}            
        }  
        
        //available RS entry not found
        return NULL;
    }
    else
    {
        printf("getRS: failed to identify op. Exiting...\n");
        exit(0);
    }

    //fail-safe
    printf("getRS: skipped all condtionals somehow. Not suppose to happen. Exiting...\n");
    exit(0);
}

//checks dependences of source registers from "ROBe" on destination registers of "priorROBeWithNeededDestReg"
void setExecutionAndMemoryDependenceForSingleROBentry(pa2::ROBentry *ROBe, pa2::ROBentry *priorROBeWithNeededDestReg)
{
    //Loads and Stores - only one source register to check
    if (ROBe->i->op.find("lw") != string::npos || ROBe->i->op.find("sw") != string::npos)
    {
        //DATA DEPENDENCE
        //check for NULL b/c I only want to assign CLOSEST dependent
        //Exclude STORES b/c their destination register is not something needed by other instructions
        if (priorROBeWithNeededDestReg->i->op.find("sw") == string::npos)
        {
            if ( (ROBe->i->sourceRegister1 == priorROBeWithNeededDestReg->i->destRegister) && ROBe->value.value1 == NULL)
            {
                ROBe->value.value1 = priorROBeWithNeededDestReg;
                priorROBeWithNeededDestReg->value.dependentValue1 = ROBe;//Lets ROB entries know who depends on them
            } 

            if ( (ROBe->i->sourceRegister2 == priorROBeWithNeededDestReg->i->destRegister) && ROBe->value.value2 == NULL )
            {
                ROBe->value.value2 = priorROBeWithNeededDestReg;
                priorROBeWithNeededDestReg->value.dependentValue2 = ROBe;
            }
        }
        
        //MEMORY DEPENDENCE
        if ( (ROBe->i->address == priorROBeWithNeededDestReg->i->address) && ROBe->value.sameMemoryAccess == NULL)
        {
            ROBe->value.sameMemoryAccess = priorROBeWithNeededDestReg;
            priorROBeWithNeededDestReg->value.dependentSameMemoryAccess = ROBe;
        }
    }
    
    //all other instructions - two source registers to check, no memory dependence
    //NOTE: doing IF-IF instead of IF-ELSE internally to be safe. Could have situation where source1 == source2... 
    //Exclude STORES b/c their destination register is not something needed by other instructions
    else if (priorROBeWithNeededDestReg->i->op.find("sw") == string::npos)
    {
        //DATA DEPENDENCE
        if ( (ROBe->i->sourceRegister1 == priorROBeWithNeededDestReg->i->destRegister) && ROBe->value.value1 == NULL)
        {
            ROBe->value.value1 = priorROBeWithNeededDestReg;
            priorROBeWithNeededDestReg->value.dependentValue1 = ROBe;//Lets ROB entries know who depends on them
        } 
        
        if ( (ROBe->i->sourceRegister2 == priorROBeWithNeededDestReg->i->destRegister) && ROBe->value.value2 == NULL )
        {
            ROBe->value.value2 = priorROBeWithNeededDestReg;
            priorROBeWithNeededDestReg->value.dependentValue2 = ROBe;
        }
    }
}

//Goes through ROB starting @ ROBe->entry + 1 and ending @ front 
void issue_setExecutionAndMemoryDependencesInROB(pa2::ROBentry *ROBe, pa2::cQueue *ROB)
{
    //Start at ROBe and work towards "front"
    //If any dependences are found (I think only RAW hazards are considered
    //... == only need to check Destination Reg of preceding ROB entries)
    
    int front = ROB->front;
    int rear = ROB->rear;
    int entry = ROBe->entry;

    //ROB is empty or ROBe is the only entry
    if (front == -1 || (front == entry) )  {/*no possible dependents. Do nothing.*/ return;}
    
    //ROB has more than one entry
    while (1)
    {
        entry--;
        
        if (entry < 0) entry = ROB->size - 1;

        if (entry == front) 
        {
            setExecutionAndMemoryDependenceForSingleROBentry(ROBe, ROB->cqArray[front]);
            return;
        }
        else
        { 
            setExecutionAndMemoryDependenceForSingleROBentry(ROBe, ROB->cqArray[entry]);
        }
    }
}

//returns TRUE if value1 state is NOT in "commit" stage.
//...Thus, TRUE == value1 does NOT have available results.
//returns FALSE if value1 state is in "commit" stage
//...Thus, FALSE == value1 does have available results
bool checkValue1ExecuteDependenceStatus(pa2::ROBentry *ROBe)
{
    if (ROBe->value.value1->state == "commit") return false;
    else return true;
}

//returns TRUE if value2 state is NOT in "commit" stage.
//...Thus, TRUE == value2 does NOT have available results.
//returns FALSE if value2 state is in "commit" stage
//...Thus, FALSE == value2 does have available results
bool checkValue2ExecuteDependenceStatus(pa2::ROBentry *ROBe)
{
    if (ROBe->value.value2->state == "commit") return false;
    else return true;
}

//At issue stage, if value1 and value2 == NULL, then there is no data dependence.
//...Register data is already available.
//If value1 and/or value2 != NULL, then we need to check the state of the dependence to see if...
//...register data is available.
void issue_UseExecuteDependencesToUpdateRS(pa2::ROBentry *ROBe, pa2::RS *RSe)
{
    //NOTE: During file read-in phase, LOADs/STOREs have both sourceRegs...
    //      ...hold the same single memory reg argument. So code below works for all!

    //VALUE 1=============================================================
    if (ROBe->value.value1 == NULL) 
    { 
        //no dependence on sourceRegister1. Leave Qj == NULL.  
        RSe->Vj = ROBe->i->sourceRegister1;
    }
    else if ( checkValue1ExecuteDependenceStatus(ROBe) ) 
    {
        //value1 results NOT available. Update Qj and Vj.
        RSe->Qj = ROBe->value.value1;
        RSe->Vj = "";
    }
    else 
    {
        //value1 results available
        RSe->Vj = ROBe->value.value1->i->destRegister; 
        RSe->Qj = NULL;
    }
    
    //VALUE 2=============================================================
    if (ROBe->value.value2 == NULL) 
    {   
        //no dependence on sourceRegister2. Leave Qj == NULL.
        RSe->Vk = ROBe->i->sourceRegister2;
    }
    else if ( checkValue2ExecuteDependenceStatus(ROBe) ) 
    {
        //value2 results NOT available. Update Qj and Vj.
        RSe->Qk = ROBe->value.value2;
        RSe->Vk = "";
    }
    else 
    {
        //value2 results available
        RSe->Vk = ROBe->value.value2->i->destRegister; 
        RSe->Qk = NULL;
    }
}

//Finds and returns ROB entries in given "state"
//"entry" parameter always starts as "front" entry in ROB
//...but subsequent entry values are previously returned ROBe's
pa2::ROBentry * findROBentryByStateFromROB(pa2::cQueue *ROB, int entry, string state)
{
    int front = ROB->front;
    int rear = ROB->rear;
    pa2::ROBentry * ROBe;

    //ROB is empty!
    if (front == -1) return NULL;

    //traverse ROB until an entry has the matching "state"
    while (1)
    {
        ROBe = ROB->cqArray[ROB->runningIndex];

        if (ROB->runningIndex == rear && ROBe->state != state) return NULL;
        else if (ROB->runningIndex == rear && ROBe->state == state) 
        {
            //No need to increment runningIndex. We're at the last one.
            return ROBe;
        }
        else if (ROBe->state == state)
        {
            ROB->runningIndex++;
            if (ROB->runningIndex > ROB->size - 1) ROB->runningIndex = 0;
            return ROBe;
        }
        else
        {
            ROB->runningIndex++;
            if (ROB->runningIndex > ROB->size - 1) ROB->runningIndex = 0;
        }
    }
}

//ROBe: Set value1 and value2 to NULL (not sameMemoryAccess - resolved in commit)
//Loads: writes ROBe's loaded destReg data to dependent instruction's source register in RS...
//      (compare load's destReg with dependent's two sourceRegs to know which one.)
//Stores: N/A. Stores don't go through writeResult stage.
//Branch: N/A. Branches don't go through writeResult stage.
//arithmatic: writes ROBe's calculated destReg data to dependent instruction's source reg in RS...
//            (again, compare ROBe->destination w/ dependents sourceReg's for a match.)
void writeResult_updateROBandRS(pa2::ROBentry * ROBe)
{
    pa2::ROBentry *dependentValue1 = ROBe->value.dependentValue1;
    pa2::ROBentry *dependentValue2 = ROBe->value.dependentValue2;
    pa2::instruction *di1 = NULL;
    pa2::instruction *di2 = NULL;

    //set boolean to say results are available. This is necessary b/c if ROB is small,...
    //...then some dependents might not be in ROB until after dependentsValues are set.
    ROBe->resultsAvailable = true;

    //extract instruction from existing dependents    
    if (dependentValue1 != NULL) di1 = ROBe->value.dependentValue1->i;
    if (dependentValue2 != NULL) di2 = ROBe->value.dependentValue2->i;
    
    //Update dependentValue1's source operands in RS with ROBe's destination reg if sourceReg == destReg
    if (di1 != NULL && (di1->sourceRegister1 == ROBe->destination)) 
    { 
        ((pa2::RS *)dependentValue1->RSe)->Vj = ROBe->destination; 
        ((pa2::RS *)dependentValue1->RSe)->Qj = NULL;
    }

    if (di1 != NULL && (di1->sourceRegister2 == ROBe->destination)) 
    { 
        ((pa2::RS *)dependentValue1->RSe)->Vk = ROBe->destination; 
        ((pa2::RS *)dependentValue1->RSe)->Qk = NULL;
    }

    //Update dependentValue2's source operands in RS with ROBe's destination reg if sourceReg == destReg
    if (di2 != NULL && (di2->sourceRegister1 == ROBe->destination)) 
    { 
        ((pa2::RS *)dependentValue2->RSe)->Vj = ROBe->destination; 
        ((pa2::RS *)dependentValue2->RSe)->Qj = NULL;
    }

    if (di2 != NULL && (di2->sourceRegister2 == ROBe->destination)) 
    { 
        ((pa2::RS *)dependentValue2->RSe)->Vk = ROBe->destination; 
        ((pa2::RS *)dependentValue2->RSe)->Qk = NULL;
    }
}


//use destination reg of ROBe to update all dependent entries in ROB
//REVIEW: This quick fix should work b/c my code only forms dependences between instructions that are in ROB
///*
void writeResult(pa2::cQueue *ROB, pa2::ROBentry * ROBe)
{
    int entry = ROBe->entry;
    pa2::ROBentry *possibleDependent;
    pa2::instruction *pdI;
    bool breakLoop = false;

    if (ROB->front == -1 || ROB->front == ROB->rear) return;

    //run through ROB STARTING @ ROBe and traverse towards rear updating all dependent instructions along the way
    while (!breakLoop)
    {
        entry++;
        if (entry > ROB->size - 1) entry = 0;
        if (entry == ROB->rear) breakLoop = true;

        possibleDependent = ROB->cqArray[entry];

        //check for dependences and update accordingly
        if (possibleDependent != NULL)
        {
            pdI = possibleDependent->i;
            
            if (pdI->destRegister == ROBe->destination) return;

            //1st source register
            if ( (pdI != NULL) && (pdI->sourceRegister1 == ROBe->destination) )
            {
                ((pa2::RS *)possibleDependent->RSe)->Vj = ROBe->destination; 
                ((pa2::RS *)possibleDependent->RSe)->Qj = NULL;
            }
            
            //2nd source register
            if ( (pdI != NULL) && (pdI->sourceRegister2 == ROBe->destination) )
            {
                ((pa2::RS *)possibleDependent->RSe)->Vk = ROBe->destination; 
                ((pa2::RS *)possibleDependent->RSe)->Qk = NULL;
            }
        }
    }
    
    return;
}
//*/

//returns true if previous STORE instruction exists in ROB
//false otherwise. 
bool previousStoreWithSameAddressExists(pa2::cQueue *ROB, pa2::ROBentry * ROBe)
{
    //Start @ front and work towards entry right before ROBe. Break when ROBe is reached.
    int front = ROB->front;
    int rear = ROB->rear;
    int entry = front;
    int ROBeAddress = ROBe->i->address;
    pa2::ROBentry *possibleStore;

    while(1)
    {
        possibleStore = ROB->cqArray[entry];

        if (possibleStore->entry == ROBe->entry) break;

        //if previous ROB entry is a STORE, check if it has same address as ROBe. 
        //If so, return true. Otherwise, return false.
        if (possibleStore != NULL && 
            possibleStore->i != NULL && 
            possibleStore->i->op.find("sw") != string::npos &&
            possibleStore->entry != ROBe->entry)
        {
            if (possibleStore->i->address == ROBeAddress) 
            {
                return true;
            }
        }

        entry++;
        if (entry > ROB->size - 1) entry = 0;
    }

    return false;
}

//return true if I found previous STORE starting from ROBe and heading to front
//any STORE found is not yet commited so it's fair game to stall for
//stalling is accomplished by NOT running IF-ELSE in memoryRead if true
//when 
bool previousStoreInROBExists(pa2::cQueue *ROB, pa2::ROBentry * ROBe)
{
    //Start @ front and work towards entry right before ROBe. Break when ROBe is reached.
    int front = ROB->front;
    int rear = ROB->rear;
    int entry = ROBe->entry;
    pa2::ROBentry *possibleStore;

    if (front == -1 || front == rear) return false;

    while(1)
    {
        entry--;
        if (entry < 0) entry = ROB->size - 1;

        possibleStore = ROB->cqArray[entry];

        if (possibleStore != NULL && 
            possibleStore->i != NULL && 
            possibleStore->entry == front &&
            possibleStore->i->op.find("sw") != string::npos &&
            possibleStore->i->address == ROBe->i->address) {return true;}
        
        else if (possibleStore != NULL && 
            possibleStore->i != NULL && 
            possibleStore->entry == front &&
            possibleStore->i->op.find("sw") != string::npos &&
            possibleStore->i->address != ROBe->i->address) {return false;}

        else if (possibleStore != NULL && 
            possibleStore->i != NULL && 
            possibleStore->entry == front &&
            possibleStore->i->op.find("sw") == string::npos) {return false;}
        
        else if (possibleStore != NULL && 
            possibleStore->i != NULL && 
            possibleStore->i->op.find("sw") != string::npos &&
            possibleStore->i->address == ROBe->i->address) {return true;}
    }

    return false;
}

pa2::ROBentry * findROBeByInstruction(pa2::cQueue *ROB, string instruction)
{
    pa2::ROBentry * ROBe;

    for (int i = 0; i < ROB->size; ++i)
    {
        ROBe = ROB->cqArray[i];
        if (ROBe != NULL && ROBe->i->fullInstruction == instruction) return ROBe;
    }

    return NULL;
}

//MAIN---------------------------------------------------------------------------------------------------------------
int main()
{
    bool finalPrint = true;
    bool CDB_Available = true;
    bool memoryRW_Available = true;
    bool loadWaitForStore = false;
    bool commit_Available = true;
    long long cycle = 0;
    int instructionQueueIndex = 0;//Only incremented when new instruction is Issued
    int robEntryNum = 1;

    int fp_add, fp_sub, fp_mul, fp_div;
    int effAddr, fpAdds, fpMuls, ints, reorder;
    int memoryReadCycle = -1;
    int lastMemoryReadCycle = -1;

    int ROBdelays = 0;
    int loadsDelays = 0, fpAddsDelays = 0, fpMulsDelays = 0, intsDelays = 0;
    int dataMemoryConflictDelays = 0;
    int trueDependenceDelays = 0;

    stringstream ss;
    string line, lineItem;
    ifstream configFile ("config.txt");    
    vector<string>configPrint;
    
    //temporary variables
    pa2::instruction * i = NULL;
    pa2::ROBentry * ROBe = NULL;
    pa2::ROBentry * ROBe2 = NULL;
    pa2::RS *RSe = NULL;
    pa2::RS *RSe2 = NULL;
    vector <pa2::RS *> *RS = NULL;
    
    //pipeline structures
    vector <pa2::instruction *> instructionQueue; //Reorder Buffer is of limited size so it must be circular queue.
                                                  //But instructionQueue can be vector since it stores ALL instructions
    vector <pa2::ROBentry *> commited; //Holds instructions that have committed and been removed from ROB 

    vector <pa2::RS *> RSloads;
    vector <pa2::RS *> RSfpAdds;
    vector <pa2::RS *> RSfpMuls;
    vector <pa2::RS *> RSints;
    vector < vector <pa2::RS *> * > RSall; //Vector of pointers to all reservation stations
    pa2::cQueue *ROB = NULL; //size determined by config.txt

    //CONFIG.txt: process before trace.dat so buffers and instructions can be constructed during read file read-in
    if (configFile.is_open())
    {
        while(getline(configFile, line))
        {
            configPrint.push_back(line);

            //buffers (these are all the different kind of reservation stations allowed)
            if (line.find("eff addr") != string::npos)     effAddr = atoi( line.substr(line.find(":") + 2).c_str() );
            else if (line.find("fp adds") != string::npos)  fpAdds = atoi( line.substr(line.find(":") + 2).c_str() );
            else if (line.find("fp muls") != string::npos)  fpMuls = atoi( line.substr(line.find(":") + 2).c_str() );
            else if (line.find("ints") != string::npos)       ints = atoi( line.substr(line.find(":") + 2).c_str() );
            else if (line.find("reorder") != string::npos) reorder = atoi( line.substr(line.find(":") + 2).c_str() );

            //latencies (all others not specified are 1)
            else if (line.find("fp_add") != string::npos) fp_add = atoi( line.substr(line.find(":") + 2).c_str() );
            else if (line.find("fp_sub") != string::npos) fp_sub = atoi( line.substr(line.find(":") + 2).c_str() );
            else if (line.find("fp_mul") != string::npos) fp_mul = atoi( line.substr(line.find(":") + 2).c_str() );
            else if (line.find("fp_div") != string::npos) fp_div = atoi( line.substr(line.find(":") + 2).c_str() );

        }
    }
    else {printf("ERROR: config file failed to open. Returning...\n"); return 0;}

    configFile.close();

    //reservation stations and reorder buffer bounds check
    if (effAddr + fpAdds + fpMuls + ints > 10) {printf("ERROR: RS number exceeds 10. Exiting...\n"); return 0;}
    else if (reorder > 10) {printf("too many entries for the reorder buffer\n"); return 0;} 

    //print config.txt info
    printf("Configuration\n");
    printf("-------------\n");
    for (int i = 0; i < configPrint.size(); ++i)
    {
        line = configPrint[i];
        if      (line == "buffers")               printf("%s:", line.c_str());
        else if (line == "latencies")             printf("%s:", line.c_str());
        else if (line.find("_") != string::npos)  printf("   %-12s\n", line.replace(line.find("_"), 1, " ").c_str());
        else                                      printf("%14s\n", line.c_str());
    }

    //TRACE.DAT: build instructionQueue from STDIN
    while(getline(cin, line) )
    {
        ss.clear(); 
        ss << line;
        
        //parse out instruction data and build instruction objects for instructionQueue
        //instructionQueue.push_back();??
        i = new pa2::instruction();
        i->fullInstruction = line;
        ss >> i->op;

        //get rid of whitespace between op and non-op lineItems
        ss >> line;
        ss.clear(); 
        ss << line;

        //load or store instructions
        if (i->op.find("lw") != string::npos || i->op.find("sw") != string::npos)
        {
            //parse lineItems
            getline(ss, lineItem, ',');
            i->destRegister = lineItem;
            
            getline(ss, lineItem, ':');
            lineItem = lineItem.substr(lineItem.find('(') + 1);
            lineItem.pop_back();
            i->sourceRegister1 = lineItem;
            i->sourceRegister2 = lineItem;
            
            ss >> i->address;

            //set latency for executing instruction and name of associated reservation station
            i->executeLatency = 1;
            i->rsName = "effAddr";

            //error check
            loadStoreInstructionCheck(i);
        }
        //branch instructions
        else if (i->op.find("beq") != string::npos || i->op.find("bne") != string::npos)
        {
            //parse non-op lineItems
            getline(ss, lineItem, ',');
            i->sourceRegister1 = lineItem;
            getline(ss, lineItem, ',');
            i->sourceRegister2 = lineItem;
            getline(ss, lineItem, ',');
            i->branchLabel = lineItem;

            //set latency for executing instruction and name of associated reservation station
            i->executeLatency = 1;
            i->rsName = "ints";

            //error check
            branchInstructionCheck(i);
        }
        //all other instructions
        else
        {
            //parse non-op lineItems
            getline(ss, lineItem, ',');
            i->destRegister = lineItem;
            getline(ss, lineItem, ',');
            i->sourceRegister1 = lineItem;
            getline(ss, lineItem, ',');
            i->sourceRegister2 = lineItem;

            //set latency for executing instruction and name of associated reservation station
            if      (i->op == "fadd.s") { i->executeLatency = fp_add; i->rsName = "fpAdds"; }
            else if (i->op == "fsub.s") { i->executeLatency = fp_sub; i->rsName = "fpAdds"; }
            else if (i->op == "fmul.s") { i->executeLatency = fp_mul; i->rsName = "fpMuls"; }
            else if (i->op == "fdiv.s") { i->executeLatency = fp_div; i->rsName = "fpMuls"; }
            
            else if (i->op == "add")    { i->executeLatency = 1;      i->rsName = "ints"; }
            else if (i->op == "sub")    { i->executeLatency = 1;      i->rsName = "ints"; }

            //error check
            arithmaticInstructionCheck(i);
        }
        
        //put new instruction into instructionQueue
        instructionQueue.push_back(i);
    }   

    //establish all buffers
    //Reservation stations (RS) will have static RS objects assigned. Only RS members will need to be updated
    //Reorder buffer (ROB) will push and pop newly allocated ROBentry objects to cqArray member
    RSloads.resize(effAddr);
    for (int i = 0; i < RSloads.size(); ++i)  {RSloads[i] = new pa2::RS(); RSloads[i]->name = "effAddr";}
    
    RSfpAdds.resize(fpAdds);
    for (int i = 0; i < RSfpAdds.size(); ++i) {RSfpAdds[i] = new pa2::RS(); RSfpAdds[i]->name = "fpAdds";}
    
    RSfpMuls.resize(fpMuls);
    for (int i = 0; i < RSfpMuls.size(); ++i) {RSfpMuls[i] = new pa2::RS(); RSfpMuls[i]->name = "fpMuls";}

    RSints.resize(ints);
    for (int i = 0; i < RSints.size(); ++i)   {RSints[i] = new pa2::RS(); RSints[i]->name = "ints";}

    RSall.push_back(&RSloads);
    RSall.push_back(&RSfpAdds);
    RSall.push_back(&RSfpMuls);
    RSall.push_back(&RSints);
    
    ROB = new pa2::cQueue(reorder); //size determined by config.txt

    //Processor loop
    while (1) 
    {
        cycle++;

    //ISSUE----------------------------------------------------------------------------------------------------
        //If ROB is full && there are instructions still in instructionQueue,...
        //...then no new instructions can be issued until free ROB slot becomes available...
        //... (i.e., instruction @ "front" of cQueue has committed and been removed. )
        if (ROB->isFull() && instructionQueueIndex < instructionQueue.size()) 
        {
            ROBdelays++;
        }
        else if (instructionQueueIndex >= instructionQueue.size()) {/*we've run out of instructions. Skip issue stage*/}
        else
        {
            //ROB is not full. Pull out instruction corresponding to current instuctionQueueIndex
            i = instructionQueue[instructionQueueIndex];

            //check for available reservation stations (RS) for this instruction
            RSe = issue_getAvailableRS(i, RSall);

            //available RS NOT found
            if (RSe == NULL)//update collective RS delay count
            {
                if      (i->rsName == "effAddr") {loadsDelays++; }
                else if (i->rsName == "fpAdds")  {fpAddsDelays++; }
                else if (i->rsName == "fpMuls")  {fpMulsDelays++; }
                else if (i->rsName == "ints")    {intsDelays++; }
            }

            //available RS found. Instruction is issued.
            else
            {
                //get ready for next instruction
                //NOTE: Not sure about this step yet. Should be corret though....
                instructionQueueIndex++;

                //create and fill out NEW ROBe object
                ROBe = new pa2::ROBentry();
                ROBe->entry = ROB->pushBack(ROBe);
                ROBe->busy = 'y'; 
                ROBe->i = i;
                ROBe->state = "issue";
                ROBe->destination = i->destRegister;
                ROBe->RSe = (void *)RSe;

                //set "value" member. Unused source registers (dependent on i->op) will remain NULL
                //Remember, only RAW hazards are a concern. Thus, only source registers of current ROBe...
                //...should be checked against destination registers of preceding ROBe's that have same register value
                issue_setExecutionAndMemoryDependencesInROB(ROBe, ROB);

                //update instruction i "PROCESSOR" members
                ROBe->i->issue = cycle;
                ROBe->i->ROBe = (void *)ROBe;
                ROBe->i->RSe = (void *)RSe;

                //update EXISTING RSe object
                //RSe->name has already been set
                RSe->busy = 'y';
                RSe->op = ROBe->i->op;

                //set Vj, Vk, Qj, Qk
                issue_UseExecuteDependencesToUpdateRS(ROBe, RSe);

                RSe->i = i;
                RSe->Dest = ROBe;

                // If ROBe has valid address, update RSe address with it
                if (ROBe->i->address != -1) RSe->address = ROBe->i->address;
            }
        }

    //EXECUTES--------------------------------------------------------------------------------------------------
        ROBe = ROB->cqArray[ROB->front];
        ROB->runningIndex = ROB->front;
        while (1)
        {
            ROBe = findROBentryByStateFromROB(ROB, ROBe->entry, "issue");

            //either no ROB entries were in given "state" || there are no remaining ROB entries in given "state"
            if (ROBe == NULL) break; 
            
            //process ROBe in "issue" state. If all DATA dependencies have resolved, transition to "execute"
            else
            {
                //if RS has all data needed to execute, then arrange to start executing next cycle
                if ( ((pa2::RS *)ROBe->RSe)->Vj != "" && ((pa2::RS *)ROBe->RSe)->Vk != "")
                {
                    ROBe->state = "execute";

                    //if this ROBe had a dependence on prior instruction(s),...
                    //...then we can start executing without having to wait a cycle
                    //Otherwise, we need to wait a cycle.
                    if (ROBe->value.value1 != NULL || ROBe->value.value2 != NULL)
                    { ROBe->IssueToExecuteSameCycle = false; }

                    else ROBe->IssueToExecuteSameCycle = true;
                }
                //if RS does NOT have all data needed AND wasn't issued earlier in the same cycle,
                //...we have a true dependence delay
                else if (cycle != ROBe->i->issue) {trueDependenceDelays++;}
            }

            //if ROBe is the last entry in ROB, then break AFTER processing
            if (ROBe->entry == ROB->rear) break;
        }

        //process ROBe's that are already in "execute" state. 
        //start from "front" of ROB and move towards rear looking for qualifying ROB entries
        ROBe = ROB->cqArray[ROB->front];
        ROB->runningIndex = ROB->front;
        while (1)
        {
            ROBe = findROBentryByStateFromROB(ROB, ROBe->entry, "execute");
            
            //either no ROB entries were in given "state" || there are no remaining ROB entries in given "state"
            if (ROBe == NULL) break; 

            //earlier in this cycle, this ROBe was in "issue" state and got changed to "execute"
            //however, I need to record this ROBe as starting to execute NEXT cycle.
            //so skip this ROBe for now
            else if (ROBe->IssueToExecuteSameCycle == true)
            {
                ROBe->IssueToExecuteSameCycle = false;
            }
            
            else if (ROBe->IssueToExecuteSameCycle == false)
            {
                //can't start execution on the same cycle as instruction was issued.
                //If this is the case, move to rearcheck to delay executing this particular instruction 1 cycle
                if (ROBe->i->executeStart == -1 && ROBe->i->issue == cycle) goto rearcheck_exe2;

                //ROBe is ready to be processed as being executed
                if (ROBe->i->executeLatency > 0) ROBe->i->executeLatency--;
                if (ROBe->i->executeStart == -1) ROBe->i->executeStart = cycle;
                if (ROBe->i->executeLatency == 0)
                {
                    ROBe->i->executeEnd = cycle;

                    //Loads: (need to check for memory access hazards)
                    if(ROBe->i->op.find("lw") != string::npos)
                    {
                        //ROBe instruction can read from memory next cycle
                        ROBe->state = "memoryRead";
                        ROBe->ExecuteToMemoryreadSameCycle = true;

                        if (previousStoreWithSameAddressExists(ROB, ROBe) == true) 
                        {
                            dataMemoryConflictDelays++; 
                            memoryRW_Available = false;
                        }
                        else
                        {
                            memoryRW_Available = true;
                        }
                    }

                    //Stores (need to check for memory access hazards)
                    else if(ROBe->i->op.find("sw") != string::npos)
                    {
                        //remove instruction from RS
                        RSe2 = ((pa2::RS *)ROBe->RSe);
                        line = RSe2->name;
                        delete RSe2;
                        RSe2 = new pa2::RS();
                        RSe2->name = line;

                        //ROBe instruction can write to memory next cycle
                        ROBe->state = "commit";
                        ROBe->ExecuteToCommitSameCycle = true;
                    }
                    
                    //Branches (all DATA dependencies resolved. Move on to "commit" automatically)
                    else if (ROBe->i->op == "bne" || ROBe->i->op == "beq")
                    {
                        ROBe->state = "commit";
                        ROBe->ExecuteToCommitSameCycle = true;

                        //remove instruction from RS
                        RSe2 = ((pa2::RS *)ROBe->RSe);
                        line = RSe2->name;
                        delete RSe2;
                        RSe2 = new pa2::RS();
                        RSe2->name = line;
                    }
                    
                    //all other instructions (all DATA dependencies resolved. Check "writing to CDB" hazards b4 moving on to "commit")
                    else
                    {

                        //Remove instruction from RS
                        RSe2 = ((pa2::RS *)ROBe->RSe);
                        line = RSe2->name;
                        delete RSe2;
                        RSe2 = new pa2::RS();
                        RSe2->name = line;

                        ROBe->state = "writeResult";
                        ROBe->ExecuteToWriteResultsSameCycle = true;
                    }
                }
            }

            //if ROBe is the last entry in ROB, then break WHILE after processing
            rearcheck_exe2:
            if (ROBe->entry == ROB->rear) break;

        }//end of WHILE

    //MEMORY READ (only load instructions - skip stores)--------------------------------------------------------------------
        //process ROBe's that are already in "memoryRead" state. 
        //start from "front" of ROB and move towards rear looking for qualifying ROB entries
        ROBe = ROB->cqArray[ROB->front];
        ROB->runningIndex = ROB->front;
        while (1)
        {
            ROBe = findROBentryByStateFromROB(ROB, ROBe->entry, "memoryRead");
            
            //either no ROB entries were in given "state" || there are no remaining ROB entries in given "state"
            if (ROBe == NULL) {memoryRW_Available = true; break;} 

            //earlier in this cycle, this ROBe was in "execute" state and got changed to "memoryRead"
            //however, I need to record this ROBe as starting the memory read NEXT cycle.
            //so skip this ROBe for now and process next cycle
            else if (ROBe->ExecuteToMemoryreadSameCycle == true) {ROBe->ExecuteToMemoryreadSameCycle = false;}
            
            //process ROBe in "memoryRead" state.
            else if (ROBe->ExecuteToMemoryreadSameCycle == false)
            {                
                if (memoryReadCycle + 5 < cycle) loadWaitForStore = false;
                
                if (loadWaitForStore)
                {
                    if (cycle >= memoryReadCycle)
                    {
                        ROBe->i->memoryRead = memoryReadCycle;
                        lastMemoryReadCycle = memoryReadCycle;

                        ROBe->state = "writeResult";
                        ROBe->MemoryreadToWriteresultSameCycle = true;
                        loadWaitForStore = false;

                        //remove instruction from RS
                        RSe2 = ((pa2::RS *)ROBe->RSe);
                        line = RSe2->name;
                        delete RSe2;
                        RSe2 = new pa2::RS();
                        RSe2->name = line;
                    }
                }
                
                else if (cycle > lastMemoryReadCycle && !previousStoreInROBExists(ROB, ROBe))
                {
                    dataMemoryConflictDelays++;
                    ROBe->i->memoryRead = cycle;
                    lastMemoryReadCycle = cycle;

                    ROBe->state = "writeResult";
                    ROBe->MemoryreadToWriteresultSameCycle = true;

                    //remove instruction from RS
                    RSe2 = ((pa2::RS *)ROBe->RSe);
                    line = RSe2->name;
                    delete RSe2;
                    RSe2 = new pa2::RS();
                    RSe2->name = line;
                }
                else trueDependenceDelays++;
            }
            
            //if ROBe is the last entry in ROB, then break after processing
            if (ROBe->entry == ROB->rear) {memoryRW_Available = true; break;}
        }

    //WRITES RESULT (write to CDB - 1 instruction at a time, SKIP stores)---------------------------------------
        //process ROBe's that are in "writeResult" state. 
        //start from "front" of ROB and move towards rear looking for qualifying ROB entries
        ROBe = ROB->cqArray[ROB->front];
        ROB->runningIndex = ROB->front;
        while (1)
        {
            ROBe = findROBentryByStateFromROB(ROB, ROBe->entry, "writeResult");

            //either no ROB entries were in given "state" || there are no remaining ROB entries in given "state"
            if (ROBe == NULL) 
            {
                CDB_Available = true; break;
            }
            
            //earlier in this cycle, this ROBe was either in "execute" or "memoryRead" state and got changed to "writeResult"
            //however, I need to record this ROBe as starting to write results NEXT cycle if there are no strutural hazards.
            //so skip this ROBe for now and process next cycle
            
            //skip arithmatic
            else if (ROBe->ExecuteToWriteResultsSameCycle == true) {ROBe->ExecuteToWriteResultsSameCycle = false;}
            
            //skip loads
            else if (ROBe->MemoryreadToWriteresultSameCycle == true) 
            {
                ROBe->MemoryreadToWriteresultSameCycle = false; 
                loadWaitForStore = false;
            }
            
            //process ROBe in "writeResult" state.
            else if (ROBe->ExecuteToWriteResultsSameCycle == false || ROBe->MemoryreadToWriteresultSameCycle == false)
            {
                //write the results over CDB, and update RS/ROB. Only 1 instruction can do this per cycle
                if (CDB_Available)
                {
                    //record cycle when writing result was performed
                    ROBe->i->writeResult = cycle;

                    //write results to associated ROBe and RSe
                    writeResult_updateROBandRS(ROBe);

                    //REVIEW: I completely forgot about the possibilty of an instruction have multiple dependent instructions
                    //        this function built in haste will fix it. I will leave "writeResult_updateROBandRS" ..
                    //        ...b/c it won't matter. It'll just perform a small amount of duplicate work.
                    writeResult(ROB, ROBe);

                    ROBe->state = "commit";
                    CDB_Available = false;
                }
            }

            //if ROBe is the last entry in ROB, then break after processing
            if (ROBe->entry == ROB->rear) {CDB_Available = true; break;}            

        }//end of while

    //COMMITS (simply check head of ROB to see if instruction is in "commit" state)------------------------
        if (ROB->cqArray[ROB->front]->state == "commit") 
        {
            ROBe = ROB->popFront();

            if (ROBe->i->op.find("sw") != string::npos)
            {
                memoryRW_Available = true;
                memoryReadCycle = cycle + 2;
                loadWaitForStore = true;
            }

            ROBe->i->commit = cycle + 1; 
            commited.push_back(ROBe);

            //If ROB is empty && there are no more instructions to process, break processor loop
            if (ROB->front == -1 && instructionQueueIndex >= instructionQueue.size()) break;
        }
    }// END OF PROCESSOR LOOP

    if (finalPrint)
    {
        //FINAL PRINT------------------------------------------------------------------------------------------------------
        printf("\n\n");
        printf("                    Pipeline Simulation\n");
        printf("-----------------------------------------------------------\n");
        printf("                                      Memory Writes\n");
        printf("     Instruction      Issues Executes  Read  Result Commits\n");
        printf("--------------------- ------ -------- ------ ------ -------\n");

        for (int i = 0; i < commited.size(); ++i)
        {
            printf("%-20s ", commited[i]->i->fullInstruction.c_str());
            printf("%7d ", commited[i]->i->issue);
            printf("%3d -", commited[i]->i->executeStart);
            printf("%3d ", commited[i]->i->executeEnd);
            if (commited[i]->i->memoryRead != -1) printf("%6d ", commited[i]->i->memoryRead);
            else printf("       ");
            if (commited[i]->i->writeResult != -1) printf("%6d ", commited[i]->i->writeResult);
            else printf("       ");
            printf("%7d\n", commited[i]->i->commit);
        }
        
        printf("\n\n");
        printf("Delays\n");
        printf("------\n");
        printf("reorder buffer delays: %d\n", ROBdelays);
        printf("reservation station delays: %d\n", loadsDelays + fpAddsDelays + fpMulsDelays + intsDelays);
        printf("data memory conflict delays: %d\n", dataMemoryConflictDelays);
        printf("true dependence delays: %d\n", trueDependenceDelays);
    } 
    return 0;
}
