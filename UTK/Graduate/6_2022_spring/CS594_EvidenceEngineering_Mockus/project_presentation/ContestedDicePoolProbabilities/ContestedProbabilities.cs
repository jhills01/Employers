using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;


namespace OmniEssence
{
    using u = Utilities;

    /// <summary>
    /// Calculates contested probabilities for all parties (or at least one party) and stores results in associated Character objects
    /// </summary>
    public class ContestedProbabilities 
    {
        /// <summary>
        /// Performs threaded or non-threaded dice rolls using law of large numbers to get proper statisitcs
        /// </summary>
        public class Experimental
        {
            /// <summary>
            /// 
            /// </summary>
            /// <param name="pc"></param>
            /// <param name="npc"></param>
            /// <param name="totalRolls"></param>
            public void threadedExecution(Character pc, Character npc, long totalDicePoolRolls, long threads)
            {
                // NOTE: create PC/NPC Character objects for each thread and then pass that to each nonThreadedExecution() call within that thread. This will avoid different threads writing to same memory locations simulataneously or other racing condition problems. 
                Character[] pcCopies  = new Character[threads];
                Character[] npcCopies = new Character[threads];
                // IMPORTANT: "threads" must be such that it evenly divides INTO "totalDicePoolRolls"
                long totalDicePoolRolls_threadFraction = totalDicePoolRolls / threads;

                [MethodImpl(MethodImplOptions.AggressiveInlining)]
                void experimentalResults(Character c)
                {
                    c.eResults.winFraction = (double)c.eResults.winCnt / (double)totalDicePoolRolls;
                    c.eResults.tieFraction = (double)c.eResults.tieCnt / (double)totalDicePoolRolls;
                    c.eResults.lossFraction = (double)c.eResults.lossCnt / (double)totalDicePoolRolls;
                    
                    // NOTE: This only works b/c all Lists have same Count
                    for (int i = 0; i < c.eResults.netSuccessCnts.Count; ++i)
                    {
                        c.eResults.successFractionFromAllRolls[i] =
                            (double)c.eResults.successCnts[i] / (double)totalDicePoolRolls;

                        c.eResults.netSuccessFractionFromAllRolls[i] =
                            (double)c.eResults.netSuccessCnts[i] / (double)totalDicePoolRolls;

                        c.eResults.netSuccessFractionFromAllWins[i] =
                            (double)c.eResults.netSuccessCnts[i] / (double)c.eResults.winCnt;
                    }
                }

                // initalize stuff
                // NOTE: Character objects have internal "eResults" updated
                //       Just need to make sure each Character[] element gets a deep copy
                pc.threadId_debug = 0;
                npc.threadId_debug = 0;
                for (int i = 0; i < threads; i++)
                {
                    // create DEEP COPIES of "pc" and "npc" Characters to each array element
                    // NOTE: each Character element will have completed "eResults" when done.
                    pcCopies[i] = new Character(pc);
                    npcCopies[i] = new Character(npc);

                    // set up objects for capturing results for each Character
                    pcCopies[i].eResults = new Character._eResults(pcCopies[i].dicePoolSize, npcCopies[i]);
                    npcCopies[i].eResults = new Character._eResults(npcCopies[i].dicePoolSize, pcCopies[i]);

                    // assign thread Ids to each copy for debugging
                    pcCopies[i].threadId_debug = i+1;
                    npcCopies[i].threadId_debug = i+1;
                }

                // perform threaded execution
                // REVIEW: Should this be "Parallel.For(0, threads - 1, i => {...});" b/c I'm zero indexed?
                //         I didn't do this in "threadTest3()" and it seemed to work out. Hmm... 
                Parallel.For(0, threads, i =>
                {
                    //NOTE: pcCopies[] and npcCopies[] are being passed BY REFERENCE
                    nonThreadedExecution(pcCopies[i], npcCopies[i], totalDicePoolRolls_threadFraction);
                });

                // update statistics for PC and NPC from each pc/npc copy sent to threads
                //for (int i = 0; i < pcCopies.Length; i++)
                for (int i = 0; i < threads; i++)
                {
                    // update "pc"
                    pc.eResults.winCnt += pcCopies[i].eResults.winCnt;
                    pc.eResults.tieCnt += pcCopies[i].eResults.tieCnt;
                    pc.eResults.lossCnt += pcCopies[i].eResults.lossCnt;
                    for (int j = 0; j < pc.eResults.netSuccessCnts.Count; ++j)
                    { 
                        pc.eResults.netSuccessCnts[j] += pcCopies[i].eResults.netSuccessCnts[j];
                        pc.eResults.successCnts[j] += pcCopies[i].eResults.successCnts[j];
                    }

                    // update "npc"
                    npc.eResults.winCnt += npcCopies[i].eResults.winCnt;
                    npc.eResults.tieCnt += npcCopies[i].eResults.tieCnt;
                    npc.eResults.lossCnt += npcCopies[i].eResults.lossCnt;
                    for (int j = 0; j < npc.eResults.netSuccessCnts.Count; ++j)
                    { 
                        npc.eResults.netSuccessCnts[j] += npcCopies[i].eResults.netSuccessCnts[j];
                        npc.eResults.successCnts[j] += npcCopies[i].eResults.successCnts[j];
                    }
                }

                experimentalResults(pc);
                experimentalResults(npc);
            }
            /// <summary>
            /// 
            /// </summary>
            /// <param name="pc"></param>
            /// <param name="npc"></param>
            /// <param name="totalDicePoolRolls"></param>
            public void nonThreadedExecution(Character pc, Character npc, long totalDicePoolRolls)
            {
                // SET UP ===================================================================================
                // Establish random generator object 
                bool seedRandom = false;
                Random r = (seedRandom) ? new Random(0) : new Random();
                int dicePoolroll, die, dieVal;

                // local function for a single roll of a dice pool for provided Character. 
                // local functions benefit from inline function performance within calling function
                // Also, notice how I can access local variables of calling function as well as local function ...
                // ... parameters!!! It's perfect!!!
                // REVIEW: This is really a do...while(dieVal >= rerollNum) loop, but I'll leave it for now.
                [MethodImpl(MethodImplOptions.AggressiveInlining)]
                void rollDicePool(Character c)
                {
                    //NOTE: Do NOT initialize any variables here.
                    //      Need to minimize overhead as function will be called millons/billons of times. 

                    // perform single dice pool roll.
                    c.winningDiceInOneRoll = 0;
                    c.rerolledDiceInOneRoll = 0;

                    //NOTE: "c.dicePoolSize-1" b/c I need to check if last die is fractional or not
                    for (die = 0; die < (c.dicePoolSize - 1); ++die)
                    {
                        // roll single die
                        dieVal = r.Next(1, 101);

                        if (dieVal >= c.rerollNum)
                        {
                            c.winningDiceInOneRoll++;
                            c.rerolledDiceInOneRoll++;

                            //reroll die until it's less than rerollNum while counting additional successes
                            while (dieVal >= c.rerollNum)
                            {
                                dieVal = r.Next(1, 101);

                                if (dieVal >= c.rerollNum)
                                {
                                    c.winningDiceInOneRoll++;
                                    c.rerolledDiceInOneRoll++;
                                }
                                else if (dieVal >= c.successNum)
                                {
                                    c.winningDiceInOneRoll++;
                                }
                            }
                        }
                        else if (dieVal >= c.successNum)
                        {
                            c.winningDiceInOneRoll++;
                        }
                    }

                    // roll last die. NOTE: And no, there is no way around repeating this much code that I can see
                    dieVal = (c.rollFracDie) ? r.Next(1, (int)c.fracDieNum + 1) : r.Next(1, 101);

                    if (dieVal >= c.rerollNum)
                    {
                        c.winningDiceInOneRoll++;
                        c.rerolledDiceInOneRoll++;

                        //reroll die
                        while (dieVal >= c.rerollNum)
                        {
                            dieVal = (c.rollFracDie) ? r.Next(1, (int)c.fracDieNum + 1) : r.Next(1, 101); ;

                            if (dieVal >= c.rerollNum)
                            {
                                c.winningDiceInOneRoll++;
                                c.rerolledDiceInOneRoll++;
                            }
                            else if (dieVal >= c.successNum)
                            {
                                c.winningDiceInOneRoll++;
                            }
                        }
                    }
                    else if (dieVal >= c.successNum)
                    {
                        c.winningDiceInOneRoll++;
                    }

                    // BUG ALERT: I accidentally updated "successCnts" incorrectly in EXECUTION section
                    //            To fix, I"m commenting out what I did before and simply updating "successCnts"
                    //            ... here. Makes more sense and is easier. 
                    c.eResults.successCnts[ (int)c.winningDiceInOneRoll ]++;
                }

                [MethodImpl(MethodImplOptions.AggressiveInlining)]
                void experimentalResults(Character c)
                {
                    c.eResults.winFraction = (double)c.eResults.winCnt / (double)totalDicePoolRolls;
                    c.eResults.tieFraction = (double)c.eResults.tieCnt / (double)totalDicePoolRolls;
                    c.eResults.lossFraction = (double)c.eResults.lossCnt / (double)totalDicePoolRolls;
                    
                    // NOTE: This loop works b/c all Lists have same Count
                    for (int i = 0; i < c.eResults.netSuccessCnts.Count; ++i)
                    {
                        c.eResults.successFractionFromAllRolls[i] =
                            (double)c.eResults.successCnts[i] / (double)totalDicePoolRolls;

                        c.eResults.netSuccessFractionFromAllRolls[i] =
                            (double)c.eResults.netSuccessCnts[i] / (double)totalDicePoolRolls;

                        c.eResults.netSuccessFractionFromAllWins[i] =
                            (double)c.eResults.netSuccessCnts[i] / (double)c.eResults.winCnt;
                    }
                }

                // EXECUTION ===============================================================================
                // perform "totalDicePoolRolls" contested rolls 
                for (dicePoolroll = 0; dicePoolroll < totalDicePoolRolls; ++dicePoolroll)
                {
                    // call local functions
                    rollDicePool(pc);
                    rollDicePool(npc);

                    // record result of single roll for each character
                    // PC WINS --------------------------------------------------
                    if (pc.winningDiceInOneRoll > npc.winningDiceInOneRoll)
                    {
                        // update pc experimental results
                        pc.eResults.winCnt++;
                        //pc.eResults.successCnts[(int)pc.winningDiceInOneRoll]++;
                        pc.eResults.netSuccessCnts[(int)(pc.winningDiceInOneRoll - npc.winningDiceInOneRoll)]++;

                        // update npc experimental results
                        //npc.eResults.successCnts[(int)npc.winningDiceInOneRoll]++;
                        npc.eResults.lossCnt++;
                    }
                    // TIE ------------------------------------------------------
                    else if (pc.winningDiceInOneRoll == npc.winningDiceInOneRoll)
                    {
                        // BUG ALERT: "successCnts" is handled in correctly here. Index should not be ZERO!
                        //            Correcting by just updating "successCnts" in "rollDicePool". It's easier. 

                        // update pc experimental results
                        pc.eResults.tieCnt++;
                        //pc.eResults.successCnts[0]++;
                        pc.eResults.netSuccessCnts[0]++;

                        // update npc experimental results
                        npc.eResults.tieCnt++;
                        //npc.eResults.successCnts[0]++;
                        npc.eResults.netSuccessCnts[0]++;
                    }
                    // NPC WINS -------------------------------------------------
                    else if (npc.winningDiceInOneRoll > pc.winningDiceInOneRoll)
                    {
                        // update pc experimental results
                        //pc.eResults.successCnts[(int)pc.winningDiceInOneRoll]++;
                        pc.eResults.lossCnt++;

                        // update npc experimental results
                        npc.eResults.winCnt++;
                        //npc.eResults.successCnts[(int)npc.winningDiceInOneRoll]++;
                        npc.eResults.netSuccessCnts[(int)(npc.winningDiceInOneRoll - pc.winningDiceInOneRoll)]++;
                    }
                }

                // NOTE: Everything above was tested and WORKS!!

                // calculate final experimental statistics 
                experimentalResults(pc);
                experimentalResults(npc);
            }



            //OLD CODE ===============================================================================================================
            /*
            public void nonThreadedExecution_threadDebug(Character pc, Character npc, int totalDicePoolRolls, int threadNum)
            {
                Utilities.print.consoleForegroundColor("green");
                Console.WriteLine($"START ===> pc.threadId_debug: {pc.threadId_debug}, npc.threadId_debug: {npc.threadId_debug}, totalDicePoolRolls: {totalDicePoolRolls:n0}, threadNum: {threadNum}");

                // SET UP =========================================
                int dicePoolroll, die, dieVal;

                // DEBUG TEST
                Random r4Debug = new Random(0);

                //// set up objects for capturing results for each Character
                //pc.eResults = new Character._eResults(pc.dicePoolSize, npc);
                //npc.eResults = new Character._eResults(npc.dicePoolSize, pc);

                // local function for a single roll of a dice pool for provided Character. 
                // local functions benefit from inline function performance within calling function
                // Also, notice how I can access local variables of calling function as well as local function ...
                // ... parameters!!! It's perfect!!!
                // REVIEW: This is really a do...while(dieVal >= rerollNum) loop, but I'll leave it for now.
                [MethodImpl(MethodImplOptions.AggressiveInlining)]
                void rollDicePool(Character c)
                {
                    //NOTE: Do NOT initialize any variables here.
                    //      Need to minimize overhead as function will be called millons/billons of times. 

                    // perform single dice pool roll.
                    c.winningDiceInOneRoll = 0;
                    c.rerolledDiceInOneRoll = 0;

                    //NOTE: "c.dicePoolSize-1" b/c I need to check if last die is fractional or not
                    for (die = 0; die < (c.dicePoolSize - 1); ++die)
                    {
                        // roll single die
                        dieVal = r4Debug.Next(1, 101);

                        if (dieVal >= c.rerollNum)
                        {
                            c.winningDiceInOneRoll++;
                            c.rerolledDiceInOneRoll++;

                            //reroll die
                            while (dieVal >= c.rerollNum)
                            {
                                dieVal = r4Debug.Next(1, 101);

                                if (dieVal >= c.rerollNum)
                                {
                                    c.winningDiceInOneRoll++;
                                    c.rerolledDiceInOneRoll++;
                                }
                                else if (dieVal >= c.successNum)
                                {
                                    c.winningDiceInOneRoll++;
                                }
                            }
                        }
                        else if (dieVal >= c.successNum)
                        {
                            c.winningDiceInOneRoll++;
                        }
                    }

                    // roll last die. NOTE: And no, there is no way around repeating this much code that I can see
                    dieVal = (c.rollFracDie) ? r4Debug.Next(1, c.fracDieNum + 1) : r4Debug.Next(1, 101);

                    if (dieVal >= c.rerollNum)
                    {
                        c.winningDiceInOneRoll++;
                        c.rerolledDiceInOneRoll++;

                        //reroll die
                        while (dieVal >= c.rerollNum)
                        {
                            dieVal = (c.rollFracDie) ? r4Debug.Next(1, c.fracDieNum + 1) : r4Debug.Next(1, 101); ;

                            if (dieVal >= c.rerollNum)
                            {
                                c.winningDiceInOneRoll++;
                                c.rerolledDiceInOneRoll++;
                            }
                            else if (dieVal >= c.successNum)
                            {
                                c.winningDiceInOneRoll++;
                            }
                        }
                    }
                    else if (dieVal >= c.successNum)
                    {
                        c.winningDiceInOneRoll++;
                    }
                }

                [MethodImpl(MethodImplOptions.AggressiveInlining)]
                void experimentalResults(Character c)
                {
                    c.eResults.winFraction = (double)c.eResults.winCnt / (double)totalDicePoolRolls;
                    c.eResults.tieFraction = (double)c.eResults.tieCnt / (double)totalDicePoolRolls;
                    c.eResults.lossFraction = (double)c.eResults.lossCnt / (double)totalDicePoolRolls;
                    for (int i = 0; i < c.eResults.netSuccessCnts.Count; ++i)
                    {
                        c.eResults.netSuccessFractionFromAllRolls[i] =
                            (double)c.eResults.netSuccessCnts[i] / (double)totalDicePoolRolls;

                        c.eResults.netSuccessFractionFromAllWins[i] =
                            (double)c.eResults.netSuccessCnts[i] / (double)c.eResults.winCnt;
                    }
                }

                // EXECUTION ===============================================================================
                // perform "totalDicePoolRolls" contested rolls 
                for (dicePoolroll = 0; dicePoolroll < totalDicePoolRolls; ++dicePoolroll)
                {
                    // call local functions
                    rollDicePool(pc);
                    rollDicePool(npc);

                    // record result of single roll for each character
                    // PC WINS --------------------------------------------------
                    if (pc.winningDiceInOneRoll > npc.winningDiceInOneRoll)
                    {
                        // update pc experimental results
                        pc.eResults.winCnt++;
                        pc.eResults.netSuccessCnts[pc.winningDiceInOneRoll - npc.winningDiceInOneRoll]++;

                        // update npc experimental results
                        npc.eResults.lossCnt++;
                    }
                    // TIE ------------------------------------------------------
                    else if (pc.winningDiceInOneRoll == npc.winningDiceInOneRoll)
                    {
                        // update pc experimental results
                        pc.eResults.tieCnt++;
                        pc.eResults.netSuccessCnts[0]++;

                        // update npc experimental results
                        npc.eResults.tieCnt++;
                        npc.eResults.netSuccessCnts[0]++;
                    }
                    // NPC WINS -------------------------------------------------
                    else if (npc.winningDiceInOneRoll > pc.winningDiceInOneRoll)
                    {
                        // update pc experimental results
                        pc.eResults.lossCnt++;

                        // update npc experimental results
                        npc.eResults.winCnt++;
                        npc.eResults.netSuccessCnts[npc.winningDiceInOneRoll - pc.winningDiceInOneRoll]++;
                    }
                }

                // NOTE: Everything above was tested and WORKS!!

                // calculate final experimental statistics 
                experimentalResults(pc);
                experimentalResults(npc);

                Utilities.print.consoleForegroundColor("red");
                Console.WriteLine($"END =====> pc.threadId_debug: {pc.threadId_debug}, npc.threadId_debug: {npc.threadId_debug}, totalDicePoolRolls: {totalDicePoolRolls:n0}, threadNum: {threadNum}");
            }

            /// <summary>
            /// 
            /// </summary>
            /// <param name="pc"></param>
            /// <param name="npc"></param>
            /// <param name="totalRolls"></param>

            public void nonThreadedExecution_debug(Character pc, Character npc, int totalDicePoolRolls)
            {
                int dicePoolroll, die, dieVal;
                bool printDiceRolls = true;

                // set up objects for capturing results for each Character
                pc.eResults = new Character._eResults(pc.dicePoolSize, npc);
                npc.eResults = new Character._eResults(npc.dicePoolSize, pc);

                u.print.consoleForegroundColor("yellow");
                Console.WriteLine($"Starting Non-threaded Experimental Run:");
                u.print.consoleForegroundColor("");

                // local function for a single roll of a dice pool for provided Character. 
                // local functions benefit from inline function performance within calling function
                // Also, notice how I can access local variables of calling function as well as local function ...
                // ... parameters!!! It's perfect!!!
                [MethodImpl(MethodImplOptions.AggressiveInlining)]
                void rollDicePool(Character c)
                {
                    //NOTE: Do NOT initialize any variables here.
                    //      Need to minimize overhead as function will be called millons/billons of times. 

                    // perform single dice pool roll.
                    c.winningDiceInOneRoll = 0;
                    c.rerolledDiceInOneRoll = 0;

                    //NOTE: "c.dicePoolSize-1" b/c I need to check if last die is fractional or not
                    for (die = 0; die < (c.dicePoolSize - 1); ++die)
                    {
                        // roll single die
                        dieVal = r.Next(1, 101);

                        if (dieVal >= c.rerollNum)
                        {
                            u.print.consoleForegroundColor("blue");
                            Console.Write($"{dieVal,3} ");

                            c.winningDiceInOneRoll++;
                            c.rerolledDiceInOneRoll++;

                            //reroll die
                            while (dieVal >= c.rerollNum)
                            {
                                dieVal = r.Next(1, 101);

                                if (dieVal >= c.rerollNum)
                                {
                                    u.print.consoleForegroundColor("blue");
                                    Console.Write($"{dieVal,3} ");

                                    c.winningDiceInOneRoll++;
                                    c.rerolledDiceInOneRoll++;
                                }
                                else if (dieVal >= c.successNum)
                                {
                                    u.print.consoleForegroundColor("green");
                                    Console.Write($"{dieVal,3} ");

                                    c.winningDiceInOneRoll++;
                                }
                                else
                                {
                                    u.print.consoleForegroundColor("red");
                                    Console.Write($"{dieVal,3} ");
                                }

                            }
                        }
                        else if (dieVal >= c.successNum)
                        {
                            u.print.consoleForegroundColor("green");
                            Console.Write($"{dieVal,3} ");

                            c.winningDiceInOneRoll++;
                        }
                        else
                        {
                            u.print.consoleForegroundColor("red");
                            Console.Write($"{dieVal,3} ");
                        }
                        ////NOTE: this won't work. "cases" need to be compared to "constant" values known at compile time.
                        //switch (dieVal) 
                        //{ 
                        //    case >= sn:
                        //        break;
                        //    default:
                        //        break;
                        //}
                    }

                    // roll last die. NOTE: And no, there is no way around repeating this much code that I can see
                    dieVal = (c.rollFracDie) ? r.Next(1, c.fracDieNum + 1) : r.Next(1, 101);

                    if (dieVal >= c.rerollNum)
                    {
                        u.print.consoleForegroundColor("blue");
                        Console.Write($"{dieVal,3} ");

                        c.winningDiceInOneRoll++;
                        c.rerolledDiceInOneRoll++;

                        //reroll die
                        while (dieVal >= c.rerollNum)
                        {
                            dieVal = (c.rollFracDie) ? r.Next(1, c.fracDieNum + 1) : r.Next(1, 101); ;

                            if (dieVal >= c.rerollNum)
                            {
                                u.print.consoleForegroundColor("blue");
                                Console.Write($"{dieVal,3} ");

                                c.winningDiceInOneRoll++;
                                c.rerolledDiceInOneRoll++;
                            }
                            else if (dieVal >= c.successNum)
                            {
                                u.print.consoleForegroundColor("green");
                                Console.Write($"{dieVal,3} ");

                                c.winningDiceInOneRoll++;
                            }
                            else
                            {
                                u.print.consoleForegroundColor("red");
                                Console.Write($"{dieVal,3} ");
                            }

                        }
                    }
                    else if (dieVal >= c.successNum)
                    {
                        u.print.consoleForegroundColor("green");
                        Console.Write($"{dieVal,3} ");

                        c.winningDiceInOneRoll++;
                    }
                    else
                    {
                        u.print.consoleForegroundColor("red");
                        Console.Write($"{dieVal,3} ");
                    }

                    u.print.consoleForegroundColor("");
                    if (c.rollFracDie) Console.Write($" === last die is FRAC ({c.fracDieNum,-2}) ===> ");
                    else Console.Write($" === last die is FULL ({c.fracDieNum,-2}) ===> ");

                    u.print.consoleForegroundColor("green");
                    Console.Write($"successes: {c.winningDiceInOneRoll,-2}, ");

                    u.print.consoleForegroundColor("blue");
                    Console.Write($"rerolls: {c.rerolledDiceInOneRoll,-2}, ");

                    u.print.consoleForegroundColor("red");
                    Console.WriteLine($" failures: {c.dicePoolSize - c.winningDiceInOneRoll,-2}");
                }

                // perform "totalRolls" dice pool rolls 
                u.print.consoleForegroundColor("purple");
                Console.WriteLine($"CHARACTER TYPE: PC, DICE POOL: {pc.dicePoolSize}");
                //Console.WriteLine($"CHARACTER TYPE: NPC, DICE POOL: {npc.dicePoolSize}");
                for (dicePoolroll = 0; dicePoolroll < totalDicePoolRolls; ++dicePoolroll)
                {
                    // call local functions
                    rollDicePool(pc);
                    //rollDicePool(npc);
                }
            }
            //*/
        }

        /// <summary>
        /// Performs my mathematical model for calculating proper statistics 
        /// </summary>
        public class Theoretical
        {
            public void execute(Character pc, Character npc)
            {
                u.print.consoleForegroundColor("blue"); 
                Console.WriteLine($"\nStarting Theorectical Run:");
                u.print.consoleForegroundColor("");

                float percentLimit = 0.00000f;
                int maxOverallSuccesses = 5; //5;

                pc.tResults.calculate_fullDiceBPs_fracDieBPs(pc, maxOverallSuccesses);
                npc.tResults.calculate_fullDiceBPs_fracDieBPs(npc, maxOverallSuccesses);

                // NOTE: Character "opponents" can be directly referenced, so no need to pass as parameter. 
                pc.tResults.calculate_netSuccessBPsOverall_netSuccessBPsPerWinBP_WinTieLoseBPs(pc, percentLimit, maxOverallSuccesses);
                npc.tResults.calculate_netSuccessBPsOverall_netSuccessBPsPerWinBP_WinTieLoseBPs(npc, percentLimit, maxOverallSuccesses);


                return;

                // TESTING ==============================================================

                int n = 6, x = 6, totalX = 7, rerollNum = 90;
                double Ps = 31d / 100d, Pf = 1 - Ps;
                double Pr = (100d - (double)rerollNum + 1d) / 100d;
                double startingRollProbability = u.Math.Probabilities.binomialPDF(n, x, Ps);
                Console.WriteLine($"startingRollProbability: b({n},{x},{Ps}) == {startingRollProbability:p5}");
                // NOTE: Don't forget that "nStartingDice" for next recursive call is x from current instance!!
                // NOTE: In "ContestedRollStatistics", int.MaxValue == rerollsRemaining. 
                //       It stands to reason this means unlimited rerolls.
                //       It also stands to reason that perhaps this can be used for "no rerolls"
                //       ... too!

                // IMPORTANT: "additionalSuccessCombos4OneRoll_Recursive" does just that. I need to loop creating the "first term" in each "combo" and THEN send those values to recursive call!!!
                double test = 0;
                //for (int i ) //NOTE: holding off on this...
                { 
                    ContestedRollStatistics.additionalSuccessCombos4OneRoll_Recursive
                            (
                                int.MaxValue, totalX - x,
                                startingRollProbability,
                                x, Pr,
                                true, $"b({n},{x},Ps)[{totalX}]"
                            );
                    
                    for(int i = 0; i <  ContestedRollStatistics.ASCombosMemCacheIndex; ++i)
                    { 
                        if (ContestedRollStatistics.ASCombosMemCache[i] != 0d)
                        { 
                            Console.WriteLine($"ContestedRollStatistics.ASCombosMemCache[{i}]: {ContestedRollStatistics.ASCombosMemCache[i]:p5}");
                            test += ContestedRollStatistics.ASCombosMemCache[i];
                        }
                    }
                    Console.WriteLine($"CACHE SUM: {test:p5}");
                }
            }

        }
    }
}