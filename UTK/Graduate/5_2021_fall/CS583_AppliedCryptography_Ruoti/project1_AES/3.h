#include "stdHeaders.h"

// HELPER FUNCTIONS -------------------------------------------------------------------------------------------------------
// This transformation substitutes each byte in the State with its corresponding value from the S-Box.
void subBytes(vector<uint8_t>* state);

// This transformation performs a circular shift on each row in the State(see Section 5.1.2)
void shiftRows(vector<uint8_t>* state);

// This transformation treats each column in state as a four-term polynomial.
// This polynomial is multiplied (modulo another polynomial) by a fixed polynomial with coefficients(see Sections 4.3 and 5.1.3).
void mixColumns(vector<uint8_t>* state);

// This transformation adds a round key to the State using XOR.
void addRoundKey(vector<uint8_t>* state, uint32_t arr32[]);

// PRIMARY FUNCTION -------------------------------------------------------------------------------------------------------
// specified in Section 5.1, and an example is given in appendix B. 
// Its implementation is quite simple once the four HELPER transformation functions above are created.
void cipher(vector<uint8_t>* in, vector<uint8_t>* out, vector<uint32_t>* words32);
