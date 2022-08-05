#include "1.h"

// adds two finite fields (see Section 4.1)
uint8_t ffadd(uint8_t ff1, uint8_t ff2)
{
    return ff1 ^ ff2;
}

// multiplies a finite field (ff) by x (see Section 4.2.1)
void xtime(uint8_t* ff)         
{
    // Most signifiant bit of *ff == 1 before left shift. 
    if (*ff & 0x80) // 0x80 == 1000 0000 in binary. 
    {
        *ff <<= 1;
        *ff ^= 0x1b; // 0x1b is hex equivalent of irreducible polynomial in Section 4.2.
    }
    
    // Most signifiant bit of *ff != 1 before left shift. 
    else *ff <<= 1;
}

// uses "xtime" to multiply any finite field (ff) by any other finite field. (see Section 4.2.1)
uint8_t ffMultiply(uint8_t ff1, uint8_t ff2)
{
    uint8_t result = 0;

    for (int i = 0; i < 8; ++i)
    {
        // 1st iteration, b remains un-shifted by x. For all others, b gets shifted by x. 
        // Right shift by i ensure ith bit of ff1 becomes least significant bit. 
        if (i != 0) xtime(&ff2);

        // true if ith bit in ff1 == 1. 
        if ((ff1 >> i) & 1) result ^= ff2;
    }

    return result;
}
