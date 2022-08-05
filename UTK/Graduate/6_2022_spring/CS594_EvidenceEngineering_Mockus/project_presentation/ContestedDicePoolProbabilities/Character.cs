using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace OmniEssence
{
    using experimental = ContestedProbabilities.Experimental;
    using theoretical = ContestedProbabilities.Theoretical;
    using u = Utilities;

    /// <summary>
    /// 
    /// </summary>
    public class Character
    {
        // public Vector<int> diceRoll; //NOTE: Using Vector<T> for SIMD is not worth it.
        public _eResults eResults = null;
        public _tResults tResults = null;
        /// <summary>
        /// PC, NPC, Companion, or whatever else. Nothing set
        /// </summary>
        public string type = ""; 
        /// <summary>
        /// REMEMBER: rerollNum >= successNum!!
        /// </summary>
        public long fullDiceQty = -1, fracDieNum = -1, successNum = -1, rerollNum = -1;
        public long dicePoolSize = -1;
        public long winningDiceInOneRoll = -1, rerolledDiceInOneRoll = -1;
        public long threadId_debug = 0; // 0 == primary Character object. 1,2,3... == thread Character objects
        /// <summary>
        /// Value determined in "Main" when reading input file
        /// </summary>
        public bool rollFracDie = false;
        
        // "DO NOTHING" CONSTRUCTOR
        public Character() { }

        // COPY CONSTRUCTOR
        public Character(Character c)
        {
            //NOTE: In C#, assignment operator copies Classes by reference, but structs by value.

            // copy each member manually.
            // CLASS COPIES (by refernce, so CAN'T just use "=")
                //NOTE: "eResults" is assigned in "nonThreadedExecution..." functions so leave it alone here.
            //eResults = new _eResults(c.dicePoolSize, new Character (c.eResults.opponent));
            //eResults = new _eResults(c.dicePoolSize, c.eResults.opponent);
            type = string.Copy(c.type);

            // STRUCT COPIES (by value, so I CAN just use "=")
            fullDiceQty = c.fullDiceQty;
            fracDieNum = c.fracDieNum;
            successNum = c.successNum;
            rerollNum = c.rerollNum;
            dicePoolSize = c.dicePoolSize;
            winningDiceInOneRoll = c.winningDiceInOneRoll;
            rerolledDiceInOneRoll = c.rerolledDiceInOneRoll;
            rollFracDie = c.rollFracDie;
        }

        /// <summary>
        /// Experimental results for associated Character. <br></br><br></br>
        /// </summary>
        public class _eResults
        {
            public Character opponent;
            
            public List<long> successCnts;
            /// <summary>
            /// List index ::: 0 == no net sucesses (tie or loss), 1 == only one net success, etc<br></br>
            /// Element   ::: stores the count of net success occurences accumulated during "totalDicePoolRolls" contested rolls.
            /// </summary>
            public List<long> netSuccessCnts;

            public List<double> successFractionFromAllRolls;
            /// <summary>
            /// List index ::: 0 == no net sucesses (tie or loss), 1 == only one net success, etc<br></br>
            /// Element   ::: stores the percentage of net successes from "totalDicePoolRolls" contested rolls. <br></br> 
            /// NOTE: If you perform "netSuccessFractionFromAllRolls[i] / winFraction", you get "i net sucesses / total wins"!!
            /// </summary>
            public List<double> netSuccessFractionFromAllRolls;
            public List<double> netSuccessFractionFromAllWins;

            public double winFraction = 0d;
            public double lossFraction = 0d;
            public double tieFraction = 0d;

            public long winCnt = 0;
            public long lossCnt = 0;
            public long tieCnt = 0;

            // constructors
            /// <summary>
            /// 
            /// </summary>
            /// <param name="dicePoolSize"></param>
            /// <param name="_opponent"></param>
            public _eResults(long dicePoolSize, Character _opponent)
            {
                // Create temporary array of needed size and use to initlize internal array of List
                // NOTE: There's no way to do this with List constructors. They just set capacity, not internal array size
                // NOTE: I'm initially doing DOUBLE "dicePoolSize" for List count b/c there are rerolls. 
                //long multiplier = 3;
                //long[] tempInt = new long[dicePoolSize * multiplier];
                
                long size = 100;
                long[] tempInt = new long[size];
                netSuccessCnts = new List<long>(tempInt);
                successCnts = new List<long>(tempInt);

                //double[] tempDouble = new double[dicePoolSize * multiplier];
                double[] tempDouble = new double[size];
                successFractionFromAllRolls = new List<double>(tempDouble);
                netSuccessFractionFromAllRolls = new List<double>(tempDouble);
                netSuccessFractionFromAllWins = new List<double>(tempDouble);

                opponent = _opponent;
            }

            public void zeroOutLists()
            {
                // Console.WriteLine($"netSuccessCnts.Capacity: {netSuccessCnts.Capacity}, netSuccessCnts.Count: {netSuccessCnts.Count}");
                
                // NOTE: this only works b/c all Lists have same Count.
                for (int i = 0; i < netSuccessCnts.Capacity; ++i)
                //for (int i = 0; i < netSuccessCnts.Count; ++i)
                {
                    successCnts[i] = 0;
                    successFractionFromAllRolls[i] = 0d;

                    netSuccessCnts[i] = 0;
                    netSuccessFractionFromAllRolls[i] = 0d;
                    netSuccessFractionFromAllWins[i] = 0d;
                }
            }

        }

        /// <summary>
        /// Theoretical results for associated Character (NOT IMPLIMENTED YET!). <br></br><br></br>
        /// </summary>
        public class _tResults
        {
            public Character opponent = null;
            
            public List<float> fullDiceBPs = null;
            public List<float> fracDieBPs = null;
            public List<float> netSuccessBPsOverall = null;
            public List<float> netSuccessBPsPerWinBP = null;

            // Versions of Lists above without element values of 0.
            // Created in "calculate_fullDiceBPs_fracDieBPs" after binomial probabilities (BPs) are determined
            public List<float> fullDiceBPs_noZeroes = new List<float>();
            public List<float> fracDieBPs_noZeroes = new List<float>();
            public List<float> netSuccessBPsOverall_noZeroes = new List<float>();
            public List<float> netSuccessBPsPerWinBP_noZeroes = new List<float>();

            public float winBP = 0f;
            public float loseBP = 0f;
            public float tieBP = 0f;

            public long dicePoolSize = -1;
            /// <summary>
            /// True if there exists a rerollNum >= successNum. False otherwise. 
            /// </summary>
            public bool rerollSuccessesAllowed = false;
            /// <summary>
            /// True if rerolling failures is desired. False otherwise. <br></br>
            /// NOTE: This is not being implemented at this time. Things to consider would be: 1) How many rounds of rerolling failures is allowed (i.e., could you reroll failures again when rerolling initial failed dice?), 2) Are all, or just some, of the failed dice of a dice pool roll allowed for reroll?
            /// </summary>
            public bool rerollFailuresAllowed = false;

            // constructor
            public _tResults() { /*EMPTY CONSTRUCTOR*/ }
            /// <summary>
            /// 
            /// </summary>
            /// <param name="_dicePoolSize"></param>
            /// <param name="_opponent"></param>
            public _tResults(long _dicePoolSize, Character _opponent)
            {
                // Create temporary array of needed size and use to initlize internal array of List
                // NOTE: There's no way to do this with List constructors. They just set capacity, not internal array size
                long multiplier = 5;

                //determine whether PC or NPC have largest dice pool and assign that.
                //This will be used to size bPDF List.
                // NOTE: Don't need to 
                long largestDicePool = (_dicePoolSize > _opponent.dicePoolSize) ? _dicePoolSize : _opponent.dicePoolSize;
                float[] temp = new float[largestDicePool * multiplier];
                fullDiceBPs = new List<float>(temp);
                fracDieBPs = new List<float>(temp);
                netSuccessBPsOverall = new List<float>(temp);
                netSuccessBPsPerWinBP = new List<float>(temp);

                // assign other members. 
                dicePoolSize = _dicePoolSize;
                opponent = _opponent;
            }

            // METHODS ---------------------------------------------------------------------------
            /// <summary>
            /// -1f assigned to "fullDiceBPs" and "fullDiceBPs" lists <br></br>
            /// 0f assigned to "netSuccessBPsOverall" and "netSuccessBPsPerWinBP" Lists
            /// </summary>
            public void AssignDefaultsToLists()
            {
                for (int i = 0; i < fullDiceBPs.Count; ++i)
                {
                    fullDiceBPs[i] = -1f;
                    fracDieBPs[i] = -1f;
                    netSuccessBPsOverall[i] = 0f;
                    netSuccessBPsPerWinBP[i] = 0f;
                }
            }
            
            public float calculateSuccessOrRerollProbabilities(float SuccessOrRerollNum, float maxDieNum)
            {
                // Subtracted 1f from successNum to account for a success occuring if roll is EQUAL to SuccessOrRerollNum (e.g., if successNum == 70, then 1 - 69 dice sides count as failures. 
                // NOTE: If "SuccessOrRerollNum - 1f" > "maxDieNum", then sp will be negative!
                float sp = 1f - (((float)SuccessOrRerollNum - 1f) / maxDieNum);
                //return (sp > 0f && sp <= 100f) ? sp : 0f; //Oops... 100f?!?!
                return (sp > 0f && sp <= 1f) ? sp : 0f;
            }

            /// <summary>
            /// Calculates and populates "fullDiceBPs" and "fracDieBPs" Lists based no 4 scenarios:<br></br>
            /// 1) rerollSuccessesAllowed == false AND rerollFailuresAllowed == false <br></br>
            /// 2) rerollSuccessesAllowed == true AND rerollFailuresAllowed == false <br></br>
            /// 3) rerollSuccessesAllowed == false AND rerollFailuresAllowed == true <br></br>
            /// 4) rerollSuccessesAllowed == true AND rerollFailuresAllowed == true
            /// </summary>
            /// <param name="dicePoolSize"></param>
            /// <param name=""></param>
            public void calculate_fullDiceBPs_fracDieBPs(Character c, int maxOverallSuccesses)
            {
                bool debug = true;

                Console.WriteLine($"calculate_fullDiceBPs_fracDieBPs(Character c):");

                // NOTE: This is going to be more difficult than I thought.
                //       1) if rerolling successes is not allowed, then it's just like the "No reroll" 
                //          ... excel sheet. However, if rerolling dice >= rerollNum (where rerollNum >= successNum) is allowed, then I have to do that recursive procedure for filling "binomialProbabilities" List where the Count of the list could technically be infinite. Need to choose a cut off percentage where it's too small to care about. 
                //       2) Once I get "binomialProbabilities" filled properly, then luckily, finding contested probabilities (both total and net) are the same!!!!

                float successProbability_fullDice = calculateSuccessOrRerollProbabilities((float)c.successNum, 100f);
                float successProbability_fracDie = calculateSuccessOrRerollProbabilities((float)c.successNum, (float)c.fracDieNum);
                float rerollProbability_fullDice = calculateSuccessOrRerollProbabilities((float)c.rerollNum, 100f);
                float rerollProbability_fracDie = calculateSuccessOrRerollProbabilities((float)c.rerollNum, (float)c.fracDieNum);
                float runningSum = 0f;
                
                if (debug)
                {                    
                    u.print.consoleForegroundColor("yellow");
                    Console.WriteLine($"\n{c.type.ToUpper()}:\nsuccessProbability_fullDice: {successProbability_fullDice:n3}, " +
                        $"successProbability_fracDie: {successProbability_fracDie:n3}");
                    Console.WriteLine($"\n{c.type.ToUpper()}:\nrerollProbability_fullDice: {rerollProbability_fullDice:n3}, " +
                        $"rerollProbability_fracDie: {rerollProbability_fracDie:n3}");

                    u.print.consoleForegroundColor("");
                }

                if      (rerollSuccessesAllowed == false && rerollFailuresAllowed == false)
                {
                    u.print.consoleForegroundColor("purple");
                    Console.WriteLine($"rerollSuccessesAllowed == false && rerollFailuresAllowed == false\n" +
                        $"NOTE: If rerollNums are < 101 in input file, you'll get out of range exception when 'rerollSuccessesAllowed' == true!");
                    u.print.consoleForegroundColor("");

                    // NOTE: both fracDieBPs and fullDiceBPs have same elements, but they are all -1f
                    //       ... UNLESS, they are assigned proper values. Help with checking in loops
                    // Calculate the only two probabilities possible for a single fracDie roll. 
                    fracDieBPs[0] = (float)u.Math.Probabilities.binomialPDF(1,0,successProbability_fracDie);
                    fracDieBPs[1] = (float)u.Math.Probabilities.binomialPDF(1,1,successProbability_fracDie);

                    if (debug)
                    { 
                        Console.WriteLine($"\nfracDieBPs[0]: {fracDieBPs[0]:p2}\n" +
                            $"fracDieBPs[1]: {fracDieBPs[1]:p2}\n");
                    }

                    // Calculate applicable probabilities for full dice rolls. 
                    for (int successes = 0; successes <= (int)c.fullDiceQty; ++successes)
                    {
                        fullDiceBPs[successes] =
                            (float)u.Math.Probabilities.binomialPDF((int)c.fullDiceQty,successes, successProbability_fullDice);

                        if (debug)
                        { Console.WriteLine($"fullDiceBPs[{successes}]: {fullDiceBPs[successes]:p2}"); }
                    }
                }
                else if (rerollSuccessesAllowed ==  true && rerollFailuresAllowed == false)
                {
                    u.print.consoleForegroundColor("purple");
                    Console.WriteLine($"rerollSuccessesAllowed == true && rerollFailuresAllowed == false");
                    u.print.consoleForegroundColor("");

                    // NOTE: both fracDieBPs and fullDiceBPs have same elements, but they are all -1f
                    //       ... UNLESS, they are assigned proper values. Help with checking in loops
                    // Calculate the only two probabilities possible for a single fracDie roll. 

                    // setup "ContestedRollStatistics" members. 
                    // REVIEW: At a later time, I should integrate this more with the project instead of calling it from old

                    // local function: resets cache index and cache elements to zero for new "initial Ps" roll
                    [MethodImpl(MethodImplOptions.AggressiveInlining)]
                    void resetRecursionCacheAndIndex(ref int cacheIndex, ref double[] cache)
                    {
                        cacheIndex = 0;
                        for(int i = 0; i < cache.Length; ++i) cache[i] = 0d;
                    }

                    [MethodImpl(MethodImplOptions.AggressiveInlining)]
                    double sumCache(ref int cacheIndex, ref double[] cache)
                    {
                        double sum = 0;
                        
                        for (int i = 0; i < ContestedRollStatistics.ASCombosMemCacheIndex; ++i)
                        {
                            if (ContestedRollStatistics.ASCombosMemCache[i] != 0d)
                            {
                                sum += ContestedRollStatistics.ASCombosMemCache[i];
                            }
                        }

                        return sum;
                    }

                    // FRAC DIE ======================================================================================
                    int fracDieQty = 1; //NOTE: I know, it's dumb to assign this, but I want readability.

                    // Not possible for fracDieNum to be rerolled. 
                    if (c.fracDieNum < c.rerollNum)
                    {
                        fracDieBPs[0] = (float)u.Math.Probabilities.binomialPDF(1, 0, successProbability_fracDie);
                        fracDieBPs[1] = (float)u.Math.Probabilities.binomialPDF(1, 1, successProbability_fracDie);
                    }
                    else 
                    { 
                        // Can't have rerolls on 0 successes, so handle it immediately
                        // REVIEW: If fracDieNum < rerollNum, then we can't include it in recursive call. We just use normal 0,1 assignments against the successNum
                        fracDieBPs[0] = (float)u.Math.Probabilities.binomialPDF(1, 0, successProbability_fracDie);
                        if (debug)
                        { Console.WriteLine($"fracDieBPs[0]: {fracDieBPs[0]:p2}"); }

                        // Since there is only 1 fractional die, then our "starting dice pool roll" is always just that one die.
                        //float fracDieBP_1success = (float)u.Math.Probabilities.binomialPDF(1, 1, successProbability_fracDie);

                        // continue calculating BPs with further successes. 
                        // REVIEW: SHIT!! This loop is fucked. It's more complicated than this.
                        //         I can't have "nStartingDice" of "additionalSuccessCombos4OneRoll_Recursive" be greater than
                        //         ... the dice pool (1 for fracDie, fullDiceQty for fullDice)
                        // Outter loop must remove dice pool quanities from "maxSuccesses" with inner loop going through each roll removed.
                        // ABOVE IS WRONG!!! "maxSuccesses" just tells me how long I need to keep recursing. HOWEVER, The successes of any one roll can never be greater than the dice pool.
                        for (int overallSuccesses = 1; overallSuccesses <= maxOverallSuccesses; ++overallSuccesses)
                        {
                            runningSum = 0f;
                            // prepare recursion cache and index for new initial roll BP
                            resetRecursionCacheAndIndex(ref ContestedRollStatistics.ASCombosMemCacheIndex, ref ContestedRollStatistics.ASCombosMemCache);

                            // got through all success scenarios of rolling dice pool
                            for (int successes = 1; successes <= fracDieQty; successes++)
                            { 
                                if (successes > overallSuccesses) break;

                                ContestedRollStatistics.additionalSuccessCombos4OneRoll_Recursive
                                (
                                    int.MaxValue,
                                    overallSuccesses - successes,
                                    // fracDieBP_1success,
                                    u.Math.Probabilities.binomialPDF(fracDieQty, successes, successProbability_fracDie),
                                    successes, 
                                    rerollProbability_fracDie,
                                    true, 
                                    $"b({fracDieQty},{successes},Ps)[{overallSuccesses}]"
                                );

                                runningSum += 
                                    (float)sumCache(ref ContestedRollStatistics.ASCombosMemCacheIndex, ref ContestedRollStatistics.ASCombosMemCache);

                                if (debug)
                                {
                                    u.print.consoleForegroundColor("green");

                                    double sumDebug = 0;
                                    for (int i = 0; i < ContestedRollStatistics.ASCombosMemCacheIndex; ++i)
                                    {
                                        if (ContestedRollStatistics.ASCombosMemCache[i] != 0d)
                                        {
                                            Console.WriteLine($"ContestedRollStatistics.ASCombosMemCache[{i}]: " +
                                                $"{ContestedRollStatistics.ASCombosMemCache[i]:p5}");

                                            sumDebug += ContestedRollStatistics.ASCombosMemCache[i];
                                        }
                                    }
                                
                                    Console.WriteLine($"CACHE SUM (debug): {sumDebug:p5}");
                                    u.print.consoleForegroundColor("");
                                }
                            }

                            fracDieBPs[overallSuccesses] = runningSum;
                        }
                    }

                    // FULL DICE ======================================================================================

                    // Can't have rerolls on 0 successes, so handle it immediately
                    //fullDiceBPs[0] = (float)u.Math.Probabilities.binomialPDF((int)c.fullDiceQty, 0, successProbability_fullDice);
                    //if (debug)
                    //{ Console.WriteLine($"fullDiceBPs[0]: {fullDiceBPs[0]:p2}"); }

                    // Calculate applicable probabilities for full dice rolls. 
                    for (int overallSuccesses = 1; overallSuccesses <= maxOverallSuccesses; ++overallSuccesses)
                    {
                        //runningSum = 0f;
                        
                        resetRecursionCacheAndIndex(ref ContestedRollStatistics.ASCombosMemCacheIndex, ref ContestedRollStatistics.ASCombosMemCache);

                        if (debug) Console.WriteLine($"-----------resetRecursionCacheAndIndex() called-----------");

                        // got through all success scenarios of rolling dice pool
                        for (int successes = 1; successes <= (int)c.fullDiceQty; successes++)
                        {
                            if (successes > overallSuccesses) break;

                            else
                            {
                                //// Recursion cache must be reset before each "additionalSuccessCombos4OneRoll_Recursive" call!
                                //resetRecursionCacheAndIndex(ref ContestedRollStatistics.ASCombosMemCacheIndex, ref ContestedRollStatistics.ASCombosMemCache);

                                //if (debug) Console.WriteLine($"-----------resetRecursionCacheAndIndex() called-----------");
                                
                                // EXAMPLE CACHE CONTENTS after recursive calls!: 
                                // NOTE: All items in sequence (seq.) line are multiplied together, and each seq. line summed together to give
                                //       ... probability for overallSuccesses.
                                // RCN, overallSuccesses, line sequence (see NOTE above)
                                // 5    5                 b(5, 5, Ps)   b(5, 0, Pr)   X
                                // 4    5                 b(5, 4, Ps)   b(4, 1, Pr)   b(1, 0, Pr)   X
                                // 3    5                 b(5, 3, Ps)   b(3, 2, Pr)   b(2, 0, Pr)   X
                                // 3    5                 b(5, 3, Ps)   b(3, 1, Pr)   b(1, 1, Pr)   b(1, 0, Pr)   X
                                // 2    5                 b(5, 2, Ps)   b(2, 2, Pr)   b(2, 1, Pr)   b(1, 0, Pr)   X
                                // 2    5                 b(5, 2, Ps)   b(2, 1, Pr)   b(1, 1, Pr)   b(1, 1, Pr)   b(1, 0, Pr)   X
                                // 1    5                 b(5, 1, Ps)   b(1, 1, Pr)   b(1, 1, Pr)   b(1, 1, Pr)   b(1, 1, Pr)   b(1, 0, Pr)   X
                                // RCN == recursion call number. If two seq. lines have the same RCN, it means they were created during the same
                                // ... recursion call!!!!
                                ContestedRollStatistics.additionalSuccessCombos4OneRoll_Recursive
                                (
                                    int.MaxValue, // means unlimited! //NOTE: I don't think limiting rerolls is implemented yet.
                                    overallSuccesses - successes,
                                    u.Math.Probabilities.binomialPDF((int)c.fullDiceQty, successes, successProbability_fullDice),
                                    //u.Math.Probabilities.binomialPDF((int)c.fullDiceQty, successes, rerollProbability_fullDice),
                                    successes,
                                    rerollProbability_fullDice,
                                    true,
                                    $"b({c.fullDiceQty},{successes},Ps)[{overallSuccesses}]"
                                    //$"b({c.fullDiceQty},{successes},Pr)[{overallSuccesses}]"
                                );

                                // "additionalSuccessCombos4OneRoll_Recursive" fills cache with additional rerolled successes FOR THE SAME 
                                // ... b(n,x,Ps) term!!! "additionalSuccessCombos4OneRoll_Recursive" could fill more than one cache line so 
                                // ... we need to sum them here. NOTE: b(n,x,Ps) IS INCLUDED IN "finalProbability"!!! Just sum the cache!!!
                                //runningSum += (float)
                                //    sumCache(ref ContestedRollStatistics.ASCombosMemCacheIndex, ref ContestedRollStatistics.ASCombosMemCache);

                                if (debug && false)
                                //if (debug)
                                {
                                    u.print.consoleForegroundColor("green");

                                    double sumDebug = 0;
                                    for (int i = 0; i < ContestedRollStatistics.ASCombosMemCacheIndex; ++i)
                                    {
                                        if (ContestedRollStatistics.ASCombosMemCache[i] != 0d)
                                        {
                                            Console.WriteLine($"ContestedRollStatistics.ASCombosMemCache[{i}]: " +
                                                $"{ContestedRollStatistics.ASCombosMemCache[i]:p5}");

                                            sumDebug += ContestedRollStatistics.ASCombosMemCache[i];
                                        }
                                    }

                                    Console.WriteLine($"CACHE SUM: {sumDebug:p5}");
                                    u.print.consoleForegroundColor("");
                                }
                            }
                        }

                        //fullDiceBPs[overallSuccesses] = runningSum;
                        fullDiceBPs[overallSuccesses] = (float)sumCache(ref ContestedRollStatistics.ASCombosMemCacheIndex, ref ContestedRollStatistics.ASCombosMemCache);

                        if (debug)
                        //// REVIEW: I'm adding "&& false" to always fail this chk b/c I relocated outside this FOR
                        //if (debug && false)
                        {
                            u.print.consoleForegroundColor("green");

                            double sumDebug = 0;
                            for (int i = 0; i < ContestedRollStatistics.ASCombosMemCacheIndex; ++i)
                            {
                                if (ContestedRollStatistics.ASCombosMemCache[i] != 0d)
                                {
                                    Console.WriteLine($"ContestedRollStatistics.ASCombosMemCache[{i}]: " +
                                        $"{ContestedRollStatistics.ASCombosMemCache[i]:p5}");

                                    sumDebug += ContestedRollStatistics.ASCombosMemCache[i];
                                }
                            }

                            Console.WriteLine($"CACHE SUM: {sumDebug:p5}");
                            u.print.consoleForegroundColor("");
                        }
                    }
                }

                //NOTE: NOT implemented ===================================================
                else if (rerollSuccessesAllowed == false && rerollFailuresAllowed == true)
                {
                    u.print.consoleForegroundColor("red");
                    Console.WriteLine($"rerollSuccessesAllowed == false && rerollFailuresAllowed == true");
                    Console.WriteLine($"NOT IMPLEMENTED!!!");
                    u.print.consoleForegroundColor("");

                }
                else if (rerollSuccessesAllowed ==  true && rerollFailuresAllowed == true)
                {
                    u.print.consoleForegroundColor("red");
                    Console.WriteLine($"rerollSuccessesAllowed == true && rerollFailuresAllowed == true");
                    Console.WriteLine($"NOT IMPLEMENTED!!!");
                    u.print.consoleForegroundColor("");

                }

                // create "no zero" versions of Lists
                for (int i = 0; i < fullDiceBPs.Count; ++i)
                { 
                    if (fullDiceBPs[i] != -1f) 
                        fullDiceBPs_noZeroes.Add(fullDiceBPs[i]);
                    if (fracDieBPs[i] != -1f) 
                        fracDieBPs_noZeroes.Add(fracDieBPs[i]);
                    //if (netSuccessBPsOverall[i] != 0f) 
                    //    netSuccessBPsOverall_noZeroes.Add(netSuccessBPsOverall[i]);
                    //if (netSuccessBPsPerWinBP[i] != 0f) 
                    //    netSuccessBPsPerWinBP_noZeroes.Add(netSuccessBPsPerWinBP[i]);
                }

                if (debug)
                {
                    u.print.consoleForegroundColor("green");
                    Console.WriteLine($"\nTYPE: {c.type}");

                    for (int i = 0; i < fullDiceBPs_noZeroes.Count; ++i)
                    {
                        Console.Write(
                            $"fullDiceBPs[{i}]: {fullDiceBPs[i]:p5} " +
                            $"===> fullDiceBPs_noZeroes[{i}] {fullDiceBPs_noZeroes[i]:p5} ::: ");

                        if (i < fracDieBPs_noZeroes.Count)
                        { 
                            Console.WriteLine($"fracDieBPs[{i}]: {fracDieBPs[i]:p5} " +
                                $"===> fracDieBPs_noZeroes[{i}]: {fracDieBPs_noZeroes[i]:p5}");
                        }
                        else
                        { 
                            Console.WriteLine($"fracDieBPs[{i}]: -" +
                                $"===> fracDieBPs_noZeroes[{i}]: -");
                        }
                    }
                    
                    u.print.consoleForegroundColor("");
                    Console.WriteLine($"");
                }
            }

            public void calculate_netSuccessBPsOverall_netSuccessBPsPerWinBP_WinTieLoseBPs(Character c, float percentLimit, int maxNetSuccessBPCalculated)
            {
                Console.WriteLine($"calculate_netSuccessBPsOverall_netSuccessBPsPerWinBP(Character c):");
                Console.WriteLine($"\nNOTE:\nGot problem. 'no rerolls' is easy b/c netSuccesses are limited by dicePoolSize (fracDie or not). However, 'rerolls' technically allows unlimited number of netSuccesses even though the probabilities become essentially zero at some point.\nPOSSIBLE SOLUTION:\n add parameter to '' which indicates how many netSuccesses you want to calculate when rerolls are allowed!");

                bool debug = true;

                int successDifference;
                int cSuccesses, oSuccesses;

                // "netSuccessBPsOverall" ===================================================================================
                // Character c successes (reminder: SUCCESSES == INDICES!!!)
                for (int cFullDiceSuccesses = 0; cFullDiceSuccesses < fullDiceBPs_noZeroes.Count; cFullDiceSuccesses++)
                {
                    for (int cFracDieSuccesses = 0; cFracDieSuccesses < fracDieBPs_noZeroes.Count; cFracDieSuccesses++)
                    {
                        cSuccesses = cFullDiceSuccesses + cFracDieSuccesses;

                        // Oppoenent successes
                        for (int oFullDiceSuccesses = 0; oFullDiceSuccesses < opponent.tResults.fullDiceBPs_noZeroes.Count; oFullDiceSuccesses++)
                        {
                            for (int oFracDieSuccesses = 0; oFracDieSuccesses < opponent.tResults.fracDieBPs_noZeroes.Count; oFracDieSuccesses++)
                            {
                                oSuccesses = oFullDiceSuccesses + oFracDieSuccesses;
                                successDifference = cSuccesses - oSuccesses;

                                // We only care about Character c net successes or ties
                                if (successDifference >= 0)
                                {
                                    //Console.WriteLine(
                                    //    $"successDifference: {successDifference}, " +
                                    //    $"cFullDiceSuccesses: {cFullDiceSuccesses}, " +
                                    //    $"cFracDieSuccesses: {cFracDieSuccesses}, " +
                                    //    $"oFullDiceSuccesses: {oFullDiceSuccesses}, " +
                                    //    $"oFracDieSuccesses: {oFracDieSuccesses}");
                                    netSuccessBPsOverall[successDifference] +=
                                        fullDiceBPs_noZeroes[cFullDiceSuccesses] *
                                        fracDieBPs_noZeroes[cFracDieSuccesses] *
                                        opponent.tResults.fullDiceBPs_noZeroes[oFullDiceSuccesses] *
                                        opponent.tResults.fracDieBPs_noZeroes[oFracDieSuccesses];
                                }
                            }
                        }
                    }
                }

                // create "netSuccessBPsOverall_noZeroes" and winBP
                for (int i = 0; i < netSuccessBPsOverall.Count; ++i)
                {
                    if (netSuccessBPsOverall[i] != 0f)
                    { 
                        netSuccessBPsOverall_noZeroes.Add(netSuccessBPsOverall[i]);
                        if ( i != 0) winBP += netSuccessBPsOverall[i];
                    }
                    // else break;
                }

                // derive loseBP from winBP and tieBP 
                tieBP = netSuccessBPsOverall_noZeroes[0];
                loseBP = 1f - (tieBP + winBP);

                // "netSuccessBPsPerWinBP_noZeroes" ===================================================================================
                // NOTE: "netSuccessBPsOverall_noZeroes[0] / winBP" doesn't make sense b/c I'm dividing tieBP by winBP.
                for (int i = 0; i < netSuccessBPsOverall_noZeroes.Count; ++i)
                {
                    netSuccessBPsPerWinBP_noZeroes.Add( netSuccessBPsOverall_noZeroes[i] / winBP);
                }

                if (debug)
                {
                    u.print.consoleForegroundColor("red");
                    Console.WriteLine($"\nYEEEESSSS!! THIS IS CORRECT!!! However, it's only been tested for what's currently in 'characters.md right now. Still have a ways to go testing here...  but yay!!@#!@#@!'");
                    u.print.consoleForegroundColor("yellow");
                    Console.WriteLine($"ALSO, I THINK MY SOLUTION IS HANDLES ALL CASES@!!!!! NEED TO CONFIRM THIS, BUT IT LOOKS GOOD!!! ");
                    u.print.consoleForegroundColor("green");
                    Console.WriteLine($"winBP: {winBP:p5}\nloseBP: {loseBP:p5}\ntieBP: {tieBP:p5}");
                    for (int i = 0; i < netSuccessBPsOverall_noZeroes.Count; ++i)
                    {
                        Console.WriteLine($"netSuccessBPsOverall_noZeroes[{i}]: {netSuccessBPsOverall_noZeroes[i]:p5} ===> " +
                                              $"netSuccessBPsPerWinBP_noZeroes[{i}]: {netSuccessBPsPerWinBP_noZeroes[i]:p5}" );
                    }

                    Console.WriteLine("");
                    u.print.consoleForegroundColor("");
                }

                /*
                //// 'maxNetSuccessBPCalculated' is parameter indicating the maximum net success
                //// ... element we should calculate. For 'no reroll' scenarios, this is simply dicePoolSize.
                //// However, since 'reroll allowed' scenarios technically have no limit to net successes, we
                //// ... must use this parameter to limit when we stop calculating net success BPs. 
                //// IMPORTANT NOTE: We should not just rely on this parameter to stop calculating. We should
                ////                 ... also set a limit on how small a BP probabilitiy can be and stop
                ////                 ... calculating if we go below that value. See "percentLimit"
                //maxNetSuccessBPCalculated = 
                //    (!rerollSuccessesAllowed) ? (int)dicePoolSize : maxNetSuccessBPCalculated;

                //if (rerollSuccessesAllowed == false && rerollFailuresAllowed == false)
                //{
                //    u.print.consoleForegroundColor("purple");
                //    Console.WriteLine($"rerollSuccessesAllowed == false && rerollFailuresAllowed == false");
                //    u.print.consoleForegroundColor("");

                //    // NOTE:
                //    // OK OK OK OK
                //    // Here's where to start:
                //    // Character c parameter is our focus
                //    // We get two FOR loop to go through all elements of fracDiceBPs and fullDiceBPs for Character c. REMEMBER! INDICES == SUCCESSES!!!
                //    // We add every COMBINATION pair of successes between the two Lists WHERE CHARACTER C SUCCESSES >= OPPONENT SUCCESSES!!!
                //    // For each combination pair, we then have a 3rd (and 4th?) loop that runs through opponent's fracDiceBPs and fullDiceBPs Lists and only calculates into "netSuccessBPsOverall" for all OPPONENT SUCCESS COMBINATIONS less than or equal to Character c's. 
                //    // Wow, this is challenging. However, at least running through fracDiceBPs and fullDiceBPs for both Character c and opponent sounds similar, though opponent's loops are limited to successes being equal or less than Character c!! 
                    
                //    //int maxSuccesses = maxNetSuccessBPCalculated;
                //    //int successDifference = 0;
                //    int successDifference;
                //    int cSuccesses, oSuccesses;
                    
                //    // Character c successes
                //    // SUCCESSES == INDICES!!!
                //    // Also, WE ONLY CARE ABOUT Character c's SUCCESSES/TIES, NOT OPPONENT'S
                //    for (int cFullDiceSuccesses = 0; cFullDiceSuccesses < fullDiceBPs_noZeroes.Count; cFullDiceSuccesses++)
                //    {
                //        for (int cFracDieSuccesses = 0; cFracDieSuccesses < fracDieBPs_noZeroes.Count; cFracDieSuccesses++)
                //        {
                //            cSuccesses = cFullDiceSuccesses + cFracDieSuccesses;

                //            // Oppoenent successes
                //            for (int oFullDiceSuccesses = 0; oFullDiceSuccesses < opponent.tResults.fullDiceBPs_noZeroes.Count; oFullDiceSuccesses++)
                //            {
                //                for (int oFracDieSuccesses = 0; oFracDieSuccesses < opponent.tResults.fracDieBPs_noZeroes.Count; oFracDieSuccesses++)
                //                {
                //                    oSuccesses = oFullDiceSuccesses + oFracDieSuccesses;
                //                    successDifference = cSuccesses - oSuccesses;
                                    
                //                    // We only care about Character c net successes or ties
                //                    if (successDifference >= 0)
                //                    { 
                //                        //Console.WriteLine(
                //                        //    $"successDifference: {successDifference}, " +
                //                        //    $"cFullDiceSuccesses: {cFullDiceSuccesses}, " +
                //                        //    $"cFracDieSuccesses: {cFracDieSuccesses}, " +
                //                        //    $"oFullDiceSuccesses: {oFullDiceSuccesses}, " +
                //                        //    $"oFracDieSuccesses: {oFracDieSuccesses}");
                //                        netSuccessBPsOverall[successDifference] += 
                //                            fullDiceBPs_noZeroes[cFullDiceSuccesses] * 
                //                            fracDieBPs_noZeroes[cFracDieSuccesses] * 
                //                            opponent.tResults.fullDiceBPs_noZeroes[oFullDiceSuccesses] * 
                //                            opponent.tResults.fracDieBPs_noZeroes[oFracDieSuccesses];
                //                    }
                //                }
                //            }
                //        }
                //    }

                //    if (debug)
                //    {
                //        u.print.consoleForegroundColor("red");
                //        Console.WriteLine($"\nYEEEESSSS!! THIS IS CORRECT!!! However, it's only been tested for what's currently in 'characters.md right now. Still have a ways to go testing here...  but yay!!@#!@#@!'"); 
                //        u.print.consoleForegroundColor("yellow");
                //        Console.WriteLine($"ALSO, I THINK MY SOLUTION IS HANDLES ALL CASES@!!!!! NEED TO CONFIRM THIS, BUT IT LOOKS GOOD!!! ");
                //        u.print.consoleForegroundColor("green");

                //        for (int i = 0; i < netSuccessBPsOverall.Count; ++i)
                //        {
                //            if (netSuccessBPsOverall[i] != 0f)
                //                Console.WriteLine($"netSuccessBPsOverall[{i}]: {netSuccessBPsOverall[i]:p5}");
                //            else
                //                break;
                //        }

                //        Console.WriteLine("");
                //        u.print.consoleForegroundColor("");
                //    }

                //}
                //else if (rerollSuccessesAllowed == true && rerollFailuresAllowed == false)
                //{
                //    u.print.consoleForegroundColor("purple");
                //    Console.WriteLine($"rerollSuccessesAllowed == true && rerollFailuresAllowed == false");
                //    u.print.consoleForegroundColor("");

                //}

                ////NOTE: NOT implemented ===================================================
                //else if (rerollSuccessesAllowed == false && rerollFailuresAllowed == true)
                //{
                //    u.print.consoleForegroundColor("red");
                //    Console.WriteLine($"rerollSuccessesAllowed == false && rerollFailuresAllowed == true");
                //    Console.WriteLine($"NOT IMPLEMENTED!!!");
                //    u.print.consoleForegroundColor("");

                //}
                //else if (rerollSuccessesAllowed == true && rerollFailuresAllowed == true)
                //{
                //    u.print.consoleForegroundColor("red");
                //    Console.WriteLine($"rerollSuccessesAllowed == true && rerollFailuresAllowed == true");
                //    Console.WriteLine($"NOT IMPLEMENTED!!!");
                //    u.print.consoleForegroundColor("");

                //}

                */
            }

            /// <summary>
            /// Use all results from "calculateWinProbability4GivenSuccess" to derive this
            /// </summary>
            public void calcualteWinProbabilityOverall()
            {

            }
        }
        
        /// <summary>
        /// Obsolete at the moment. Use "printAll()"
        /// </summary>
        public void print_debug()
        {
            Console.WriteLine($"CHECKING type");
            if (type == null) Console.WriteLine($"type is null!!!");

            var table = new ConsoleTables.ConsoleTable("VARIABLE", "VALUE");
            
            Console.WriteLine($"CHECKING table");
            if (table == null) Console.WriteLine($"TABLE IS NULL!!!");

            table.AddRow("type", $"{type}")
                 .AddRow("fullDiceQty", $"{fullDiceQty}")
                 .AddRow("fracDieNum", $"{fracDieNum}")
                 .AddRow("successNum", $"{successNum}")
                 .AddRow("rerollNum", $"{rerollNum}")
                 .AddRow("dicePoolSize", $"{dicePoolSize}")
                 //.AddRow("winningDiceInOneRoll", $"{winningDiceInOneRoll}")
                 //.AddRow("rerolledDiceInOneRoll", $"{rerolledDiceInOneRoll}")
                 .AddRow("rollFracDie", $"{rollFracDie}");
                 //.AddRow("eResults.opponent", $"{eResults.opponent}")
                 //.AddRow("eResults.netSuccessCnts", $"{eResults.netSuccessCnts}")
                 //.AddRow("eResults.winCnt", $"{eResults.winCnt}")
                 //.AddRow("eResults.lossCnt", $"{eResults.lossCnt}")
                 //.AddRow("eResults.tieCnt", $"{eResults.tieCnt}");
            table.Write();
            Console.WriteLine();
        }

        public void printAllExperimental_debug()
        {
            //// NOTE: Holding off on this idea of generic List local function for adding rows.
            //void addListRows(ConsoleTables.ConsoleTable t, string rowName, List<dynamic> list1, List<dynamic> list2)
            //{

            //}

            Character opponent = eResults.opponent;
            long largestArraySize = 0;
            long playerNSListCnt = eResults.netSuccessCnts.Count;
            long opponentNSListCnt = opponent.eResults.netSuccessCnts.Count;
            long playerSListCnt = eResults.successCnts.Count;
            long opponentSListCnt = opponent.eResults.successCnts.Count;

            var table = new ConsoleTables.ConsoleTable("VARIABLE", $"PLAYER", $"OPPONENT", "VARIABLE", $"PLAYER", $"OPPONENT", "VARIABLE", $"PLAYER", $"OPPONENT");
            table.AddRow("type", $"{type}".ToUpper(), $"{opponent.type}".ToUpper(), "-", "-", "-", "-", "-", "-")
                 .AddRow("fullDiceQty", $"{fullDiceQty}", $"{opponent.fullDiceQty}", "-", "-", "-", "-", "-", "-")
                 .AddRow("fracDieNum", $"{fracDieNum}", $"{opponent.fracDieNum}", "-", "-", "-", "-", "-", "-")
                 .AddRow("successNum", $"{successNum}", $"{opponent.successNum}", "-", "-", "-", "-", "-", "-")
                 .AddRow("rerollNum", $"{rerollNum}", $"{opponent.rerollNum}", "-", "-", "-", "-", "-", "-")
                 .AddRow("dicePoolSize", $"{dicePoolSize}", $"{opponent.dicePoolSize}", "-", "-", "-", "-", "-", "-")
                 .AddRow("rollFracDie", $"{rollFracDie}", $"{opponent.rollFracDie}", "-", "-", "-", "-", "-", "-")
                 .AddRow("winCnt", $"{eResults.winCnt:n0}", $"{opponent.eResults.winCnt:n0}", "winFraction", $"{eResults.winFraction:p5}", $"{opponent.eResults.winFraction:p5}", "-", "-", "-")
                 .AddRow("lossCnt", $"{eResults.lossCnt:n0}", $"{opponent.eResults.lossCnt:n0}", "lossFraction", $"{eResults.lossFraction:p5}", $"{opponent.eResults.lossFraction:p5}", "-", "-", "-")
                 .AddRow("tieCnt", $"{eResults.tieCnt:n0}", $"{opponent.eResults.tieCnt:n0}", "tieFraction", $"{eResults.tieFraction:p5}", $"{opponent.eResults.tieFraction:p5}", "-", "-", "-");

//table.Write();
//Console.WriteLine();
//return;
            // NET SUCCESSES

            // determine whether PLAYER or OPPONENT has larger "netSuccessCnt" List. 
            // NOTE: Above, I thought about using local function to take dynamic typed list. Holdng off for now....
            largestArraySize = (playerNSListCnt > opponentNSListCnt) ? playerNSListCnt : opponentNSListCnt;
            
            if (largestArraySize == 0)
            {
                table.AddRow("-", $"-", $"-",
                             "-", $"-", $"-",
                             "-", $"-", $"-");
            }
            else
            {
                table.AddRow($"netSuccessCnts[0]", 
                    (playerNSListCnt >= 1 && eResults.netSuccessCnts[0] != 0) ? $"{eResults.netSuccessCnts[0]:n0}" : "-", 
                    (opponentNSListCnt >= 1 && opponent.eResults.netSuccessCnts[0] != 0d) ? $"{opponent.eResults.netSuccessCnts[0]:n0}" : "-",

                    $"netSuccessFractionFromAllRolls[0]",
                    (playerNSListCnt >= 1 && eResults.netSuccessFractionFromAllRolls[0] != 0d) ? $"{eResults.netSuccessFractionFromAllRolls[0]:p5}" : "-",
                    (opponentNSListCnt >= 1 && opponent.eResults.netSuccessFractionFromAllRolls[0] != 0d) ? $"{opponent.eResults.netSuccessFractionFromAllRolls[0]:p5}" : "-",

                    "netSuccessFractionFromAllWins[0]", $"-", $"-");

                    // NOTE: It doesn't make sense to divide "ties" by "wins". 
                    //$"netSuccessFractionFromAllWins[0]",
                    //(playerNSListCnt >= 1) ? $"{eResults.netSuccessFractionFromAllWins[0]:p5}" : "-",
                    //(opponentNSListCnt >= 1) ? $"{opponent.eResults.netSuccessFractionFromAllWins[0]:p5}" : "-");
                
                for (int i = 1; i < largestArraySize; ++i)
                {
                    if ( ((playerNSListCnt >= i + 1) && (eResults.netSuccessCnts[i] == 0)) &&
                         ((opponentNSListCnt >= i + 1) && (opponent.eResults.netSuccessCnts[i] == 0)) &&
                         ((playerNSListCnt >= i + 1) && eResults.netSuccessFractionFromAllRolls[i] == 0d) &&
                         ((opponentNSListCnt >= i + 1) && opponent.eResults.netSuccessFractionFromAllRolls[i] == 0d) &&
                         ((playerNSListCnt >= i + 1) && (eResults.netSuccessFractionFromAllWins[i] == 0d)) &&
                         ((opponentNSListCnt >= i + 1) && (opponent.eResults.netSuccessFractionFromAllWins[i] == 0d))
                        )
                    {
                        // All zeroes indicate that netSuccessCnts[i] for PC and NPC were 0 and thus,
                        // ... all further elements will be zero too. No need to continue building table.
                        break;
                    }
                    else
                    {
                        // REVIEW: Since I seemingly did all these checks already above, I can probably get
                        //         ... rid of them here and just add the rows. 
                        table.AddRow($"netSuccessCnts[{i}]", 
                            ((playerNSListCnt >= i + 1) && eResults.netSuccessCnts[i] != 0) ? $"{eResults.netSuccessCnts[i]:n0}" : "-",
                            ((opponentNSListCnt >= i + 1) && opponent.eResults.netSuccessCnts[i] != 0) ? $"{opponent.eResults.netSuccessCnts[i]:n0}" : "-",
                        
                            $"netSuccessFractionFromAllRolls[{i}]",
                            ((playerNSListCnt >= i + 1) && eResults.netSuccessFractionFromAllRolls[i] != 0d) ? $"{eResults.netSuccessFractionFromAllRolls[i]:p5}" : "-",
                            ((opponentNSListCnt >= i + 1) && opponent.eResults.netSuccessFractionFromAllRolls[i] != 0d) ? $"{opponent.eResults.netSuccessFractionFromAllRolls[i]:p5}" : "-",

                            $"netSuccessFractionFromAllWins[{i}]",
                            ((playerNSListCnt >= i + 1) && (eResults.netSuccessFractionFromAllWins[i] != 0d)) ? $"{eResults.netSuccessFractionFromAllWins[i]:p5}" : "-",
                            ((opponentNSListCnt >= i + 1) && (opponent.eResults.netSuccessFractionFromAllWins[i] != 0d)) ? $"{opponent.eResults.netSuccessFractionFromAllWins[i]:p5}" : "-");
                    }
                }
            }


//table.Write();
//Console.WriteLine();
//return;

            // SUCCESSES

            // determine whether PLAYER or OPPONENT has larger "netSuccessCnt" List. 
            largestArraySize = (playerSListCnt > opponentSListCnt) ? playerSListCnt : opponentSListCnt;

            if (largestArraySize == 0)
            {
                table.AddRow("SUCCESSES", $"-", $"-",
                             "-", $"-", $"-",
                             "-", $"-", $"-");
            }
            else
            {
                table.AddRow($"successCnts[0]",
                    (playerSListCnt >= 1 && eResults.successCnts[0] != 0) ? $"{eResults.successCnts[0]:n0}" : "-",
                    (opponentSListCnt >= 1 && opponent.eResults.successCnts[0] != 0) ? $"{opponent.eResults.successCnts[0]:n0}" : "-",

                    $"successFractionFromAllRolls[0]",
                    (playerSListCnt >= 1 && eResults.successFractionFromAllRolls[0] != 0d) ? $"{eResults.successFractionFromAllRolls[0]:p5}" : "-",
                    (opponentSListCnt >= 1 && opponent.eResults.successFractionFromAllRolls[0] != 0d) ? $"{opponent.eResults.successFractionFromAllRolls[0]:p5}" : "-",
                    
                    $"-", "-", "-");
                

                for (int i = 1; i < largestArraySize; ++i)
                {
                    if 
                    (
                        ((playerSListCnt >= i + 1) && (eResults.successCnts[i] == 0)) &&
                        ((opponentSListCnt >= i + 1) && (opponent.eResults.successCnts[i] == 0)) &&
                        ((playerSListCnt >= i + 1) && eResults.successFractionFromAllRolls[i] == 0d) &&
                        ((opponentSListCnt >= i + 1) && opponent.eResults.successFractionFromAllRolls[i] == 0d)
                    )

                    {
                        // All zeroes indicate that netSuccessCnts[i] for PC and NPC were 0 and thus,
                        // ... all further elements will be zero too. No need to continue building table.
                        break;
                    }
                    else
                    {
                        // REVIEW: Since I seemingly did all these checks already above, I can probably get
                        //         ... rid of them here and just add the rows. 
                        table.AddRow($"successCnts[{i}]",
                            ((playerSListCnt >= i + 1) && eResults.successCnts[i] != 0) ? $"{eResults.successCnts[i]:n0}" : "-",
                            ((opponentSListCnt >= i + 1) && opponent.eResults.successCnts[i] != 0) ? $"{opponent.eResults.successCnts[i]:n0}" : "-",

                            $"successFractionFromAllRolls[{i}]",
                            ((playerSListCnt >= i + 1) && eResults.successFractionFromAllRolls[i] != 0d) ? $"{eResults.successFractionFromAllRolls[i]:p5}" : "-",
                            ((opponentSListCnt >= i + 1) && opponent.eResults.successFractionFromAllRolls[i] != 0d) ? $"{opponent.eResults.successFractionFromAllRolls[i]:p5}" : "-",

                            $"-", "-", "-");
                    }
                }
            }

            table.Write();
            Console.WriteLine();
        }
    }
}