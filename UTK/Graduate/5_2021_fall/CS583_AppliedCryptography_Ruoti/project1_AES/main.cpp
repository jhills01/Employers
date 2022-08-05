
// only include external cpp files here. stdHeaders.h is in all.
#include "1.h"
#include "2.h"
#include "3.h"
#include "4.h"

int main()
{
    int const nk128 = 4, nb128 = 4, nr128 = 10; // for all nk/nb/nr pairings, see figure 4 on page 18/51 from FIPS pdf. 
    int const nk192 = 6, nb192 = 4, nr192 = 12; // for all nk/nb/nr pairings, see figure 4 on page 18/51 from FIPS pdf. 
    int const nk256 = 8, nb256 = 4, nr256 = 14; // for all nk/nb/nr pairings, see figure 4 on page 18/51 from FIPS pdf. 
    
    // nk = 4, nb = 4, nr = 10
    vector<uint8_t> cipherKey128 =
    {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
    };
    
    // nk = 6, nb = 4, nr = 12
    vector<uint8_t> cipherKey192 =
    {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 
        0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17
    };

    // nk = 8, nb = 4, nr = 14
    vector<uint8_t> cipherKey256 =
    {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
    };

    vector<uint32_t> words128(nb128 * (nr128 + 1));
    vector<uint32_t> words192(nb192 * (nr192 + 1));
    vector<uint32_t> words256 (nb256 * (nr256 + 1));
    
    vector<uint8_t> in = 
    {
        0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
    };

    vector<uint8_t> out;
    vector<uint8_t> out2;
    
    // 128 ------------------------------------------------------------------------------------------------------------------------
    words128 = keyExpansion(cipherKey128, words128, nk128);
    printf("C.1   AES-128 (Nk=4, Nr=10)\n\n");
    printf("PLAINTEXT:          00112233445566778899aabbccddeeff\n");
    printf("KEY:                000102030405060708090a0b0c0d0e0f\n\n");
    printf("CIPHER (ENCRYPT):\n");
    cipher(&in, &out, &words128);
    printf("\nINVERSE CIPHER (DECRYPT):\n");
    invCipher(&out, &out2, &words128);

    // 192 ------------------------------------------------------------------------------------------------------------------------
    words192 = keyExpansion(cipherKey192, words192, nk192);
    printf("\nC.2   AES-192 (Nk=6, Nr=12)\n\n");
    printf("PLAINTEXT:          00112233445566778899aabbccddeeff\n");
    printf("KEY:                000102030405060708090a0b0c0d0e0f1011121314151617\n\n");
    printf("CIPHER (ENCRYPT):\n");
    cipher(&in, &out, &words192);
    printf("\nINVERSE CIPHER (DECRYPT):\n");
    invCipher(&out, &out2, &words192);

    // 256 ------------------------------------------------------------------------------------------------------------------------
    words256 = keyExpansion(cipherKey256, words256, nk256);
    printf("\nC.3   AES-256 (Nk=8, Nr=14)\n\n");
    printf("PLAINTEXT:          00112233445566778899aabbccddeeff\n");
    printf("KEY:                000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f\n\n");
    printf("CIPHER (ENCRYPT):\n");
    cipher(&in, &out, &words256);
    printf("\nINVERSE CIPHER (DECRYPT):\n");
    invCipher(&out, &out2, &words256);

    return 0;
}

// CODE REPOSITORY ==================================================================================================================
    // 1 bit change in first byte: 0x00 -> 0x01
//in =
//{
//    //0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34
//    0x01, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
//};
//printf("\n\nTESTING BIT DIFFERENCE PRESERVATION BETWEEEN TWO DIFFERENT \"IN\"s!!!\n");
//cipher(&in, &out, &words128);
//printf("\nRESULT: Block cipher encryption algorithms completely scramble outputs so 1 bit difference creates completely different cipher text!!!\n");
//


    //printf("ffadd(0x57, 0x83) == %x\n", ffadd(0x57, 0x83));
    //printf("ffMultiply(0x57, 0x83) == 0x%x\n", ffMultiply((uint8_t)0x57, (uint8_t)0x83));


    //print32Vec_main(words128);
    //printf("\n\n");

    //words192 = keyExpansion(cipherKey192, words192, nk192);
    //print32Vec_main(words192);

    //words256 = keyExpansion(cipherKey256, words256, nk256);
    //print32Vec_main(words256);

    // keyExpansion works!!!!!!!!!!
