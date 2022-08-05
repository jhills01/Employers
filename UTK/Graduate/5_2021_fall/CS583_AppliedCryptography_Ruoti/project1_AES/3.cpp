#include "3.h"
#include "1.h"

// PRINT FUNCTIONS
void finalPrint(uint8_t arr8[], bool isTranspose)
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

// HELPER FUNCTIONS -------------------------------------------------------------------------------------------------------
// This transformation substitutes each byte in the State with its corresponding value from the S-Box.
void subBytes(vector<uint8_t>* state)
{
    uint8_t b;
    uint8_t sBox[] =
    {
        //       0,    1,    2,    3,    4,    5,    6,    7,    8,    9,    a,    b,    c,    d,    e,    f
        /*0*/ 0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
        /*1*/ 0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
        /*2*/ 0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
        /*3*/ 0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
        /*4*/ 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
        /*5*/ 0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
        /*6*/ 0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
        /*7*/ 0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
        /*8*/ 0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
        /*9*/ 0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
        /*a*/ 0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
        /*b*/ 0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
        /*c*/ 0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
        /*d*/ 0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
        /*e*/ 0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
        /*f*/ 0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
    };

    for (int i = 0; i < state->size(); ++i)
    {
        // sBox is indexed via "row*16 + column" where "row == first 4 bits of b" and "column == last 4 bits of b" 
        // (in hex, this means: b == 0x[1st 4 bits][last 4 bits] ).
        b = (*state)[i];
        (*state)[i] = sBox[(b >> 4) * 16 + (b & 0x0f)];
    }
}

// This transformation performs a circular shift on each row in the State(see Section 5.1.2)
void shiftRows(vector<uint8_t>* state)
{
    uint8_t row, temp;
    int i, j;

    // left shift the bytes of the last 3 rows of state "row" number of times 
    for (i = 1; i < 4; ++i)
    {
        // perform "i" number of left shifts on "row" of state 
        for (j = 0; j < i; ++j)
        {
            row = 4 * i;
            temp = (*state)[row];
            (*state)[row    ] = (*state)[row + 1];
            (*state)[row + 1] = (*state)[row + 2];
            (*state)[row + 2] = (*state)[row + 3];
            (*state)[row + 3] = temp;
        }
    }
} 

// This transformation treats each column in state as a four-term polynomial.
// This polynomial is multiplied (modulo another polynomial) by a fixed polynomial with coefficients(see Sections 4.3 and 5.1.3).
void mixColumns(vector<uint8_t>* state)
{
    int c;
    vector<uint8_t> origState = *state;

    for (c = 0; c < 4; ++c)
    {
        (*state)[0  + c] = ffMultiply(2,origState[0 + c]) ^ ffMultiply(3, origState[4 + c]) ^ origState[8 + c] ^ origState[12 + c];
        (*state)[4  + c] = origState[0 + c] ^ ffMultiply(2, origState[4 + c]) ^ ffMultiply(3, origState[8 + c]) ^ origState[12 + c];
        (*state)[8  + c] = origState[0 + c] ^ origState[4 + c] ^ ffMultiply(2, origState[8 + c]) ^ ffMultiply(3, origState[12 + c]);
        (*state)[12 + c] = ffMultiply(3, origState[0 + c]) ^ origState[4 + c] ^ origState[8 + c] ^ ffMultiply(2, origState[12 + c]);
    }
}

// This transformation adds a round key to the State using XOR.
void addRoundKey(vector<uint8_t> *state, uint32_t arr32[])
{
    int i;
    uint8_t arr8[16], arr8Transpose[16];

    // create array of 8 byte elements from array of 32 byte elements
    for (i = 0; i < 4; i++)
    {
        arr8[4 * i    ] = arr32[i] >> 24;
        arr8[4 * i + 1] = arr32[i] >> 16;
        arr8[4 * i + 2] = arr32[i] >> 8;
        arr8[4 * i + 3] = arr32[i];
    }

    finalPrint(arr8, false);

    // transpose newly created arr8 
    for (i = 0; i < 16; i++) arr8Transpose[(i % 4) * 4 + (i / 4)] = arr8[i];

    // add round key value to state
    for (i = 0; i < 16; i++) (*state)[i] ^= arr8Transpose[i];

}

// PRIMARY FUNCTION -------------------------------------------------------------------------------------------------------
// specified in Section 5.1, and an example is given in appendix B. 
// Its implementation is quite simple once the four HELPER transformation functions above are created.
// NOTE: According to FIPS, "in" and "out" always have 4*nb == 16 elements where where nb == 4 for 128, 192, and 256 cipher keys
void cipher(vector<uint8_t> *in, vector<uint8_t> *out, vector<uint32_t> *words32)
{
    int round = 0, maxRounds = (words32->size() / 4) - 1;
    uint32_t* words32ArrPtr = &(*words32)[0];
    vector<uint8_t> state(in->size());

    // transpose "in" vector to "state" 
    for (int i = 0; i < 16; i++) state[(i % 4) * 4 + (i / 4)] = (*in)[i];

    printf("round[%2d].input     ", round); finalPrint( &(*in)[0], false );
    printf("round[%2d].k_sch     ", round);

    addRoundKey(&state, words32ArrPtr + 4 * round);

    for (round = 1; round < maxRounds; ++round)
    {
        printf("round[%2d].start     ", round); finalPrint( &state[0], true );

        subBytes(&state);
        printf("round[%2d].s_box     ", round); finalPrint(&state[0], true);

        shiftRows(&state);
        printf("round[%2d].s_row     ", round); finalPrint(&state[0], true);

        mixColumns(&state);
        printf("round[%2d].m_col     ", round); finalPrint(&state[0], true);

        printf("round[%2d].k_sch     ", round);
        addRoundKey(&state, words32ArrPtr + 4 * round);
    }

    printf("round[%2d].start     ", round); finalPrint(&state[0], true);

    subBytes(&state);
    printf("round[%2d].s_box     ", round); finalPrint(&state[0], true);

    shiftRows(&state);
    printf("round[%2d].s_row     ", round); finalPrint(&state[0], true);

    printf("round[%2d].k_sch     ", round);
    addRoundKey(&state, words32ArrPtr + 4 * round);


    // COPY final result from "state" to "out"
    // "state" memory should be cleaned up automatically when function returns
    *out = state;

    printf("round[%2d].output    ", round); finalPrint( &(*out)[0], true);
}
