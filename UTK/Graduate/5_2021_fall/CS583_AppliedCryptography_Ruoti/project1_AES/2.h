#include "stdHeaders.h"

// HELPER FUNCTIONS -------------------------------------------------------------------------------------------------------
// takes a four - byte input word and substitutes each byte in that word with its appropriate value from the S - Box.The S - box is provided(see Section 5.1.1).
int32_t subWord(int32_t word);

// performs a cyclic permutation on its input word
int32_t rotWord(int32_t word);

// PRIMARY FUNCTION -------------------------------------------------------------------------------------------------------
// Notice that the key for use with this algorithm is given as a byte[], but both cipher and invCipher require a word[] as input. 
// The key expansion algorithm (see Section 5.2) performs this conversion. 
// Appendix A gives excellent examples of the Key expansion algorithm. 
// The two HELPER functions above are needed by this algorithm
vector<uint32_t> keyExpansion(vector<uint8_t> cipherKey, vector<uint32_t> words, uint8_t nk);
