#include <emmintrin.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void print128_num(__m128 var)
{
    float val[4];
    memcpy(val, &var, sizeof(val));
    printf("Numerical: %f %f %f %f\n", 
           val[0], val[1], val[2], val[3]);
}

float dotprod(float x[], float y[], int n)
{
    int i;
    __m128 X;
    __m128 Y;
    __m128 sum;
    __attribute__ ((aligned(16))) float s[4] = {0,0,0,0};

    sum = _mm_load_ps(s);
    for (i = 0; i < n; i += 4)
    {
        X = _mm_load_ps(&x[i]);
        Y = _mm_load_ps(&y[i]);
        sum = _mm_add_ps( sum, _mm_mul_ps(X, Y) );
    }

    _mm_store_ps(s, sum);

    return (s[0] + s[1] + s[2] + s[3]);
}
