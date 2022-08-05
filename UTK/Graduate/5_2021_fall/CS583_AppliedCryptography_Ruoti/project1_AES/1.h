#include "stdHeaders.h"

// adds two finite fields (see Section 4.1)
uint8_t ffadd(uint8_t ff1, uint8_t ff2);

// multiplies a finite field (ff) by x (see Section 4.2.1)
void xtime(uint8_t* ff);

// uses "xtime" to multiply any finite field (ff) by any other finite field. (see Section 4.2.1)
uint8_t ffMultiply(uint8_t ff1, uint8_t ff2);
