float dotprod(float x[], float y[], int n)
{
    int i;
    float sum, sum1, sum2, sum3;

    sum = 0.0, sum1 = 0.0, sum2 = 0.0, sum3 = 0.0;
    for (i = 0; i < n; i += 4)
    {
        sum += x[i] * y[i];
        sum1 += x[i+1] * y[i+1];
        sum2 += x[i+2] * y[i+2];
        sum3 += x[i+3] * y[i+3];
    }
    
    return (sum + sum1 + sum2 + sum3);

}
