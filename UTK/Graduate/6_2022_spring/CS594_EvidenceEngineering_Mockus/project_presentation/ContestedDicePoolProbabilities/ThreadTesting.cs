using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace OmniEssence
{
    using ut = Utilities.testing;

    public class ThreadTesting
    {
        public bool debug = true;

        /// <summary>
        /// This method is only used in threadTest1 and 2!!
        /// </summary>
        /// <param name="iterations"></param>
        /// <returns></returns>
        public long sumIterations(long iterations)
        {
            long sum = 0;

            for (long i = 0; i < iterations; ++i)
            {
                sum += 1;
            }

            return sum;
        }

        public void threadTest1(long iterations, string s)
        {
            long test1 = 0;
            long test2 = 0;

            var watch = new System.Diagnostics.Stopwatch();

            Console.WriteLine($"Starting threadTest1:\n");

            watch.Start();
                
            if (s.ToLower() == "y")
            {
                if (debug) { Console.WriteLine($"Threaded execution\n");}

                Thread thr1 = new Thread( () => { test1 = sumIterations(iterations); } );
                Thread thr2 = new Thread( () => { test2 = sumIterations(iterations); } );
                thr1.Start();
                thr2.Start();
                thr1.Join();
                thr2.Join();
            }
            else 
            {
                if (debug) { Console.WriteLine($"Non-Threaded execution\n");}

                test1 = sumIterations(iterations);
                test2 = sumIterations(iterations);
            }
                
            watch.Stop();
            Console.WriteLine($"Execution Time: {watch.ElapsedMilliseconds} ms");

            Console.WriteLine($"\n test1 + test2 == {test1 + test2}\n");
        }

        /// <summary>
        /// Crap!! This didn't work!! Not sure why yet, but I'm not getting a performance increase by ...
        /// splitting 10,000,000,000 iterations into 10 threads of 1,000,000,000 iterations. Hmmm...
        /// </summary>
        /// <param name="iterations"></param>
        /// <param name="s"></param>
        public void threadTest2(long iterations, string s)
        {
            int maxThreads = 10;
            //List<long> threadResults = new List<long>();
            //List<Thread> threads = new List<Thread>();
            Thread[] threads = new Thread[maxThreads];
            long[] threadResults = new long[maxThreads];
            long finalSum = 0;
            long threadIterations = (iterations / maxThreads); // maxThreads must divide iterations evenly!!
            //long threadIterations = (iterations / maxThreads) + 1; // plus 1 to account for truncation
            var watch = new System.Diagnostics.Stopwatch();

            Console.WriteLine($"Starting threadTest2:\n");

            watch.Start();
                
            if (s.ToLower() == "y")
            {
                if (debug) { Console.WriteLine($"Threaded execution\n");}

                // Bypassed the "no argument, no return" nature of C# Threads below by ...
                // using void lamda functions with needed code tucked inside.
                // source: https://blog.platformular.com/2015/07/05/c-multithreaded-start-thread-with-parameter-and-return-value/
                for (int i = 0; i < (maxThreads - 1); ++i)
                {
                    Console.WriteLine($"i: {i} ==> maxThreads: {maxThreads}");
                    //threads.Add( new Thread(() => {threadResults.Add( sumIterations(iterations) );}) );
                    threads[i] = new Thread(() => 
                    {
                        //threadResults[i] =  sumIterations(threadIterations);
                        finalSum +=  sumIterations(threadIterations);
                    } );
                    
                    //threads[i].Start();

                    ////REVIEW: "Join()" makes each thread wait on each, so it defeats the purpose.
                    ////        However, Join is required to capture return result. hmm....
                    //// https://www.geeksforgeeks.org/joining-threads-in-c-sharp/
                    //threads[i].Join();
                }

                //foreach(long tr in threadResults) { finalSum += tr; }
            }
            else 
            {
                if (debug) { Console.WriteLine($"Non-Threaded execution\n");}
                finalSum = sumIterations(iterations);
            }
                
            watch.Stop();
            Console.WriteLine($"Execution Time: {watch.ElapsedMilliseconds} ms");

            Console.WriteLine($"\n finalSum == {finalSum}\n");
        }

        /// <summary>
        /// This uses the Parallel library and the For and ForEach members
        /// AWESOME SOURCE: https://www.codemag.com/article/1211071/Tasks-and-Parallelism-The-New-Wave-of-Multithreading
        /// </summary>
        /// <param name="iterations"></param>
        /// <param name="s"></param>
        public void threadTest3(long iterations, string s)
        {
            int maxThreads = 20;
            //Thread[] threads = new Thread[maxThreads];
            long[] threadResults = new long[maxThreads];
            long finalSum = 0;
            // maxThreads must divide iterations evenly!!
            long threadIterations = (iterations / maxThreads); 
            var watch = new System.Diagnostics.Stopwatch();

            Console.WriteLine($"Starting threadTest3 using {maxThreads} threads:\n");

            watch.Start();
                
            if (s.ToLower() == "y")
            {
                if (debug) { Console.WriteLine($"Threaded execution\n");}

                Parallel.For(0,maxThreads, i =>
                {
                    threadResults[i] =  ut.sumIterations(threadIterations);
                });

                foreach(long tr in threadResults) { finalSum += tr; }
            }
            else 
            {
                if (debug) { Console.WriteLine($"Non-Threaded execution\n");}
                finalSum = ut.sumIterations(iterations);
            }
                
            watch.Stop();
            Console.WriteLine($"Execution Time: {watch.ElapsedMilliseconds} ms");

            Console.WriteLine($"\n finalSum == {finalSum}\n");
        }

    }
}
