using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OmniEssence
{
    public static class Utilities
    {
        public static class Math
        {
            public static class Probabilities
            {
                /// <summary>
                /// Calculates factorial series for x. <br></br>
                /// NOTE: Should I convert "retVal" to array and do multiple multiplications per pass in WHILE loop?
                /// </summary>
                /// <param name="x"></param>
                /// <returns></returns>
                public static long factorial(long x)
                {
                    // check to see if I should exit early
                    if (x <= 1) return 1;

                    long retVal = 1;

                    while (x >= 1)
                    {
                        retVal *= x;
                        --x;
                    }

                    return retVal;
                    //return x <= 1 ? 1 : x * factorial(x - 1);
                }

                // Checks for parameter errors in binomialPDF() function
                private static string[] bPDFErrorChk(int nDice, int xSuccesses, double pSuccess)
                {
                    // Errors
                    if      (nDice < 1)
                        return new string[] { "y", string.Format("nDice < 1 (nDice == {0})", nDice) };
                    else if (xSuccesses < 0)
                        return new string[] { "y", string.Format("xSuccesses < 0 (xSuccesses == {0})", xSuccesses) };
                    else if (pSuccess > 1f || pSuccess < 0f)
                        return new string[] { "y", string.Format("pSuccess > 1f || pSuccess < 0f (pSuccess == {0:N4}f)", pSuccess) };
                    else if (nDice < xSuccesses)
                        return new string[] { "y", string.Format("nDice < xSuccesses ({0} < {1})", nDice, xSuccesses)};

                    // No errors. 
                    return new string[] { "n", ""};
                }

                /// <summary>
                /// Returns probability that "nDice" dice rolled will produce "xSuccesses" successes where "pSuccess" is the probability of each die being >= Success Number.
                /// </summary>
                /// <param name="nDice">The quantity of dice to be rolled. </param>
                /// <param name="xSuccesses">The number of successes desired from rolling "nDice" dice.</param>
                /// <param name="pSuccess">The probability of each die being >= Success Number. </param>
                /// <returns></returns>
                //public static float binomialPDF4DiceRolls(int nDice, int xSuccesses, float pSuccess)
                public static double binomialPDF(int nDice, int xSuccesses, double pSuccess)
                {
                    // error checking
                    string[] e = bPDFErrorChk(nDice, xSuccesses, pSuccess);
                    if (e[0] == "y")
                    {
                        OE_CombatSimulator.consoleForegroundColor("red");
                        Console.WriteLine("\nERROR in binomailPDF(): {0}.", e[1]);
                        Console.WriteLine("Returning 0f.");
                        OE_CombatSimulator.consoleForegroundColor();
                        return 0f;
                    }

                    double qFailure = 1f - pSuccess;
                    long combinationCoefficient = factorial(nDice) / (factorial(xSuccesses) * factorial(nDice - xSuccesses));
                    
                    return combinationCoefficient * System.Math.Pow(pSuccess, xSuccesses) * System.Math.Pow(qFailure, nDice - xSuccesses);

                    // NOTE: If I use double type, I don't need to cast which I believe has a negative performance impact un-necessarily
                    //return (float)combinationCoefficient * (float)System.Math.Pow(pSuccess, xSuccesses) * (float)System.Math.Pow(qFailure, nDice - xSuccesses);
                }
            }
        }

        public static class conversions
        {
            public static float convertIntegerDecimalToFloat(int Integer, int Decimal)
            {
                return ( ((float)Integer) + ((float)Decimal) / 100f );
            }

            public static int[] convertFloatToIntegerDecimal(float f)
            {
                f = MathF.Round(f, 2, MidpointRounding.AwayFromZero); // round float to nearest 2 decimal float away from zero at midpoints
                return new int[2] { (int)f, (int)(f*100f)%100 }; // NOTE: (int)f should return only the integer part of float
            }
        }
    
        public static class testing
        {
            public static long sumIterations(long iterations)
            {
                long sum = 0;

                for (long i = 0; i < iterations; ++i)
                {
                    sum += 1;
                }

                return sum;
            }
        }
    
        public static class print
        {
            /// <summary>
            /// Changes background and foreground colors of console based on "color" parameter. <br></br>
            /// Options include: <br></br>
            ///     "green", "red", "blue", "purple", or "yellow". <br></br>
            ///     Anything else resets to default
            /// </summary>
            public static void consoleForegroundColor(string color = "")
            {
                if      (color.ToLower() == "green")  Console.ForegroundColor = ConsoleColor.Green;
                else if (color.ToLower() == "red")    Console.ForegroundColor = ConsoleColor.Red;
                else if (color.ToLower() == "blue")   Console.ForegroundColor = ConsoleColor.Blue;
                else if (color.ToLower() == "purple") Console.ForegroundColor = ConsoleColor.Magenta;
                else if (color.ToLower() == "yellow") Console.ForegroundColor = ConsoleColor.DarkYellow;

                // default
                else Console.ResetColor();
            }

            public static void printIntList(List<int> list)
            {
                for (int i = 0; i < list.Count; ++i)
                {
                    Console.WriteLine($"[{i}]: {list[i]}");
                }
            }
        }
    }
}
