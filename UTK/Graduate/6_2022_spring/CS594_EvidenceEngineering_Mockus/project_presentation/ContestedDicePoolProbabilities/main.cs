using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Numerics;

namespace OmniEssence
{
    using experimental = ContestedProbabilities.Experimental;
    using theoretical = ContestedProbabilities.Theoretical;
    using u = Utilities;

    public static class main
    {

        //public static bool debug = true;

        /// <summary>
        /// stuipd little test. Ignore this crappola.
        /// </summary>
        /// <param name="args"></param>
        private static void localThreatTest(string[] args)
        {

            ThreadTesting t = new ThreadTesting();
            long iterations = 10000000000;
            t.threadTest3(iterations, args[0]);
        }

        private static void printArgs(string[] args)
        {
            Console.WriteLine($"Number of arguments: {args.Length}");
            for (int i = 0; i < args.Length; i++)
            {
                Console.WriteLine($"args[{i}]: {args[i]}");
            }
            Console.WriteLine($""); //extra newline 

        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="pc"></param>
        /// <param name="npc"></param>
        private static void printExperimentalResults(Character pc, Character npc)
        {
            u.print.consoleForegroundColor("green");
            pc.printAllExperimental_debug();
            
            u.print.consoleForegroundColor("yellow");
            Console.WriteLine($"pc.eResults.winCnt + pc.eResults.tieCnt + pc.eResults.lossCnt   : {pc.eResults.winCnt + pc.eResults.tieCnt + pc.eResults.lossCnt:n0}");
            Console.WriteLine($"npc.eResults.winCnt + npc.eResults.tieCnt + npc.eResults.lossCnt: {npc.eResults.winCnt + npc.eResults.tieCnt + npc.eResults.lossCnt:n0}");

            long sumCnts = 0;
            double sumFractionsAllRolls = 0d;
            for (int i = 0; i < pc.eResults.successCnts.Count; i++)
            {
                sumCnts += pc.eResults.successCnts[i];
                sumFractionsAllRolls += pc.eResults.successFractionFromAllRolls[i];
            }

            Console.WriteLine($"\n(PC) sumCnts (should == totalDicePoolRolls)  : {sumCnts:n0} ");
            Console.WriteLine($"(PC) sumFractionsAllRolls (should == 100.00%): {sumFractionsAllRolls:p2} ");
            
            u.print.consoleForegroundColor("");
        }

        /*
          cd "/mnt/f/Computer_Science/Back2School/UTK/GraduateSchool/_classes/6_2022_spring/CS594_EvidenceEngineering_Mockus/project_presentation/ContestedDicePoolProbabilities/bin/Debug/net5.0"
        */
        /// <summary>
        /// blah
        /// </summary>
        /// <param name="args">
        /// args[0]: "ey" == threaded execution of experimental dice rolls, "en" == non-threaded execution of experimental dice rolls, "t" == theoretical execution using binomial probability functions (NOTE: no threaded version at this point). <br></br><br></br>
        /// args[1]: integer value == totalRolls <br></br><br></br>
        /// </param>
        static void Main(string[] args)
        {
            if (args.Length != 0) 
            {
                Console.WriteLine($"START==========================================================================\n");
                printArgs(args);

                // create variables 
                Character pc = new Character();
                Character npc = new Character();
                experimental e = new experimental();
                theoretical t = new theoretical();
                string fileName = @"F:\Computer_Science\Back2School\UTK\GraduateSchool\_classes\6_2022_spring\CS594_EvidenceEngineering_Mockus\project_presentation\ContestedDicePoolProbabilities\__project\input\characters.md";
                string[] lines, delimitedLine;
                long totalDicePoolRolls = (args.Length >= 2 && long.TryParse(args[1], out long result)) ? result: 0;
                int lineNum = 0, threads = 0;
                
                // read in character data from file
                // NOTE: If I don't want rerolls enabled, rerollNum == 101 in file.
                bool debugFileReadInMain = false;
                lines = File.ReadAllLines( fileName );
                foreach ( string line in lines )
                {
                    delimitedLine = line.Split(',');
                    
                    if (debugFileReadInMain) {lineNum++; Console.WriteLine($"line {lineNum}: {line}");}

                    if      (delimitedLine[0] == "pc")
                    {
                        // read lines from file
                        pc.type = "PC";
                        pc.fullDiceQty = int.Parse(delimitedLine[1]);
                        pc.fracDieNum = int.Parse(delimitedLine[2]);
                        pc.successNum = int.Parse(delimitedLine[3]);
                        pc.rerollNum = int.Parse(delimitedLine[4]);
                        
                        // derive character variables from file data
                        pc.rollFracDie = (pc.fracDieNum >= pc.successNum) ? true : false;
                        pc.dicePoolSize = (pc.rollFracDie) ? pc.fullDiceQty + 1 : pc.fullDiceQty;
                        
                        // NOTE: Vector<T> SIMD is not worth using for this purpose
                        //       Probably too much overhead copying array contents to Vector<T> ...
                        //       ... and individual elements (i.e., myVector[i]) are read only, so can't assign to.
                        //pc.diceRoll = (pc.rollFracDie) ? new Vector<int>(new int[pc.fullDiceQty + 1]) : new Vector<int>(new int[pc.fullDiceQty]);

                        if (debugFileReadInMain) pc.print_debug();
                    }
                    else if (delimitedLine[0] == "npc")
                    {
                        // read lines from file
                        npc.type = "NPC";
                        npc.fullDiceQty = int.Parse(delimitedLine[1]);
                        npc.fracDieNum = int.Parse(delimitedLine[2]);
                        npc.successNum = int.Parse(delimitedLine[3]);
                        npc.rerollNum = int.Parse(delimitedLine[4]);

                        // derive character variables from file data
                        npc.rollFracDie = (npc.fracDieNum >= npc.successNum) ? true : false;
                        npc.dicePoolSize = (npc.rollFracDie) ? npc.fullDiceQty + 1 : npc.fullDiceQty;

                        // NOTE: Vector<T> SIMD is not worth using for this purpose
                        //       Probably too much overhead copying array contents to Vector<T> ...
                        //       ... and individual elements (i.e., myVector[i]) are read only, so can't assign to.
                        //npc.diceRoll = (npc.rollFracDie) ? new Vector<int>(new int[npc.fullDiceQty + 1]) : new Vector<int>(new int[npc.fullDiceQty]);

                        if (debugFileReadInMain) npc.print_debug();
                    }
                    else if (delimitedLine[0] == "threads")
                    {
                        threads = int.Parse(delimitedLine[1]);
                    }
                }

                // initialize each Character to capture EXPERIMENTAL results 
                pc.eResults = new Character._eResults(pc.dicePoolSize, npc);
                pc.eResults.zeroOutLists();
                npc.eResults = new Character._eResults(npc.dicePoolSize, pc);
                npc.eResults.zeroOutLists();

                // initialize each Character to capture THEORETICAL results 
                pc.tResults = new Character._tResults(pc.dicePoolSize, npc);
                pc.tResults.AssignDefaultsToLists();
                npc.tResults = new Character._tResults(npc.dicePoolSize, pc);
                npc.tResults.AssignDefaultsToLists();

                // start rolling and printing statistics
                // NOTE: "pc" and "npc" objects are passed by reference!
                u.print.consoleForegroundColor("yellow");
                Console.WriteLine($"\n============= totalDicePoolRolls: {totalDicePoolRolls:n0}");
                Console.WriteLine($"============= threads: {threads:n0}");

                var watch = new System.Diagnostics.Stopwatch();
                watch.Start();
                switch (args[0])
                {
                    case "en":
                        e.nonThreadedExecution(pc, npc, totalDicePoolRolls);
                        break;
                    case "ey":
                        e.threadedExecution(pc, npc, totalDicePoolRolls, threads);
                        break;
                    case "ey2":
                        //u.print.consoleForegroundColor("red");
                        //Console.WriteLine($"RECURSIVE BINOMIAL THEORY APPROACH HAS FAILED! Changing strategy:\nIn 'characters.md' file, I will use pc/npc characters to compare between NO REROLL and REROLL scenarios. pc and npc will have same stats, except one will have rerollNum < 101 and other will have rerollNum == 101. In this way, I can directly compare how unlimited rerolling affects experimental results. Hopefully, I can find a trend in how unlimited rerolls affects NO REROLL scenario.\nAlso, use 1,000,000,000 instead of 10,000,000,000. The results are pretty much identical and 1,000,000,000 is 10x faster.");
                        //u.print.consoleForegroundColor("");
                        e.threadedExecution(pc, npc, totalDicePoolRolls, threads);
                        break;
                    case "t":
                        u.print.consoleForegroundColor("red");
                        Console.WriteLine($"NOTE: The recursive sequencing of binomials for unlimited rerolling DOES NOT WORK!!!");
                        u.print.consoleForegroundColor("");
                        t.execute(pc, npc);
                        break;
                    default:
                        Console.WriteLine($"args[0] == {args[0]} not recognized in switch. Default reached.");
                        break;
                }
                watch.Stop();

                //PRINT RESULTS
                u.print.consoleForegroundColor("blue");
                Console.WriteLine($"\nExecution Time: {watch.ElapsedMilliseconds} ms");
                switch (args[0])
                {
                    case "en":
                        printExperimentalResults(pc, npc);
                        break;
                    case "ey":
                        printExperimentalResults(pc, npc);
                        break;
                    case "ey2":
                        printExperimentalResults(pc, npc);
                        break;
                    case "t":
                        u.print.consoleForegroundColor($"red");
                        Console.WriteLine($"\n REROLLING THEORY HAS FAILED. Would still be nice to have 'Theory vs. Experimental' for NO REROLLS though.. ");
                        break;
                    default:
                        Console.WriteLine($"args[0] == {args[0]} not recognized in switch. Default reached.");
                        break;
                }
            }
            else
            {
                int stupidErrorCode = 6969;
                Console.WriteLine($"No command line arguments supplied. Exiting with code {stupidErrorCode}!!");
            }

            u.print.consoleForegroundColor("");
            Console.WriteLine($"\nEND===========================================================================");

        }
    }
}