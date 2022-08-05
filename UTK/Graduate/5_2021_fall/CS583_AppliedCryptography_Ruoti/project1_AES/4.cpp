#include "4.h"
#include "1.h"
#include "3.h"

// HELPER FUNCTIONS -------------------------------------------------------------------------------------------------------
void finalPrintDecrypt(uint8_t arr8[], bool isTranspose)
{
    if (isTranspose)
    {
        for (int i = 0; i < 4; ++i)
        {
            printf("%02x%02x%02x%02x",
                arr8[i],
                arr8[4 + i],
                arr8[8 + i],
                arr8[12 + i]);
        }
        printf("\n");
    }
    else
    {
        for (int i = 0; i < 16; ++i) printf("%02x", arr8[i]);
        printf("\n");
    }
}

// This transformation substitutes each byte in the State with its corresponding value from the inverse S-Box, 
// thus reversing the effect of a subBytes() operation.
void invSubBytes(vector<uint8_t>* state)
{
    uint8_t b;
    uint8_t sBox[] =
    {
        //       0,    1,    2,    3,    4,    5,    6,    7,    8,    9,    a,    b,    c,    d,    e,    f
        /*0*/ 0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
        /*1*/ 0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
        /*2*/ 0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
        /*3*/ 0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
        /*4*/ 0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
        /*5*/ 0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
        /*6*/ 0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
        /*7*/ 0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
        /*8*/ 0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
        /*9*/ 0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
        /*a*/ 0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
        /*b*/ 0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
        /*c*/ 0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
        /*d*/ 0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
        /*e*/ 0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
        /*f*/ 0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
    };

    for (int i = 0; i < state->size(); ++i)
    {
        // sBox is indexed via "row*16 + column" where "row == first 4 bits of b" and "column == last 4 bits of b" 
        // (in hex, this means: b == 0x[1st 4 bits][last 4 bits] ).
        b = (*state)[i];
        (*state)[i] = sBox[(b >> 4) * 16 + (b & 0x0f)];
    }

}

// This transformation performs the inverse of shiftRows() on each row in the State(see Section 5.3.1)
void invShiftRows(vector<uint8_t>* state)
{
    uint8_t row, temp;
    int i, j;

    // right shift the bytes of the last 3 rows of state "row" number of times 
    for (i = 1; i < 4; ++i)
    {
        // perform "i" number of right shifts on "row" of state 
        for (j = 0; j < i; ++j)
        {
            row = 4 * i;
            temp = (*state)[row + 3];
            (*state)[row + 3] = (*state)[row + 2];
            (*state)[row + 2] = (*state)[row + 1];
            (*state)[row + 1] = (*state)[row    ];
            (*state)[row    ] = temp;
        }
    }
}

// This transformation is the inverse of mixColumns(see Section 5.3.3).
void invMixColumns(vector<uint8_t>* state)
{
    int c;
    vector<uint8_t> origState = *state;

    for (c = 0; c < 4; ++c)
    {
        (*state)[0 + c] = 
            ffMultiply(14, origState[0 + c]) ^ ffMultiply(11, origState[4 + c]) ^ ffMultiply(13, origState[8 + c]) ^ ffMultiply(9, origState[12 + c]);
        
        (*state)[4 + c] = 
            ffMultiply(9, origState[0 + c]) ^ ffMultiply(14, origState[4 + c]) ^ ffMultiply(11, origState[8 + c]) ^ ffMultiply(13, origState[12 + c]);
        
        (*state)[8 + c] = 
            ffMultiply(13, origState[0 + c]) ^ ffMultiply(9, origState[4 + c]) ^ ffMultiply(14, origState[8 + c]) ^ ffMultiply(11, origState[12 + c]);
        
        (*state)[12 + c] = 
            ffMultiply(11, origState[0 + c]) ^ ffMultiply(13, origState[4 + c]) ^ ffMultiply(9, origState[8 + c]) ^ ffMultiply(14, origState[12 + c]);
    }
}

// PRIMARY FUNCTION -------------------------------------------------------------------------------------------------------
// This function is specified in Section 5.3. 
// It reverses the effect of the cipher function. 
// Its implementation is quite simple once the three above HELPER transformation functions are created.
void invCipher(vector<uint8_t>* in, vector<uint8_t>* out, vector<uint32_t>* words32)
{
    bool debug = false;
    int round = 0, maxRounds = (words32->size() / 4) - 1, invRound = maxRounds;
    uint32_t* words32ArrPtr = &(*words32)[0];
    vector<uint8_t> state(in->size());

    // "in" is already in transposed form due to being the "out" result from Cipher()! 
    // Just simply copy it to "state"
    state = *in;

    printf("round[%2d].iinput    ", round); finalPrintDecrypt(&(*in)[0], true);
    
    printf("round[%2d].ik_sch    ", round);
    addRoundKey(&state, words32ArrPtr + 4 * invRound);
    --invRound;

    for (round = 1; round < maxRounds; ++round, --invRound)
    {
        printf("round[%2d].istart    ", round); finalPrintDecrypt(&state[0], true);

        invShiftRows(&state);
        printf("round[%2d].is_row    ", round); finalPrintDecrypt(&state[0], true);

        invSubBytes(&state);
        printf("round[%2d].is_box    ", round); finalPrintDecrypt(&state[0], true);


        printf("round[%2d].ik_sch    ", round);
        addRoundKey(&state, words32ArrPtr + 4 * invRound);
        printf("round[%2d].ik_add    ", round); finalPrintDecrypt(&state[0], true);

        invMixColumns(&state);
    }

    printf("round[%2d].istart    ", round); finalPrintDecrypt(&state[0], true);

    invShiftRows(&state);
    printf("round[%2d].is_row    ", round); finalPrintDecrypt(&state[0], true);

    invSubBytes(&state);
    printf("round[%2d].is_box    ", round); finalPrintDecrypt(&state[0], true);

    printf("round[%2d].ik_sch    ", round);
    addRoundKey(&state, words32ArrPtr + 4 * invRound);

    printf("round[%2d].ioutput   ", round); finalPrintDecrypt(&state[0], true);

    // COPY final result from "state" to "out"
    // "state" memory should be cleaned up automatically when function returns
    *out = state;
}