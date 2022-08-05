using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace OmniEssence
{
    public class Misc
    {
        public void dndRolls()
        {
            int sum = 0, lowest, r, iterations = 0;
            Random rand = new System.Random();

            while (sum != 18)
            {
                Console.Write($"New Roll: ");

                lowest = 6;
                sum = 0;
                for (int i = 0; i < 4; ++i)
                {
                    //returns signed integer between [1, diceSides)
                    r = rand.Next(1, 7);  
                    Console.Write($"{r} ");
                    if (lowest > r) lowest = r;
                    sum += r;
                }
                    
                sum -= lowest;
                iterations++;
                    
                Console.WriteLine($"==> lowest: {lowest}, sum: {sum}, iterations: {iterations}");

            }
        }
    }
}