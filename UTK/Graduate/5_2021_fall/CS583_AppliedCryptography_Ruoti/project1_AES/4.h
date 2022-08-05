#include "stdHeaders.h"

// HELPER FUNCTIONS -------------------------------------------------------------------------------------------------------
// This transformation substitutes each byte in the State with its corresponding value from the inverse S-Box, 
// thus reversing the effect of a subBytes() operation.
void invSubBytes(vector<uint8_t>* state);

// This transformation performs the inverse of shiftRows() on each row in the State(see Section 5.3.1)
void invShiftRows(vector<uint8_t>* state);

// This transformation is the inverse of mixColumns(see Section 5.3.3).
void invMixColumns(vector<uint8_t>* state);

// PRIMARY FUNCTION -------------------------------------------------------------------------------------------------------
// This function is specified in Section 5.3. 
// It reverses the effect of the cipher function. 
// Its implementation is quite simple once the three above HELPER transformation functions are created.
void invCipher(vector<uint8_t>* in, vector<uint8_t>* out, vector<uint32_t>* words32);