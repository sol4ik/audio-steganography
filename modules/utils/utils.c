#include "utils.h"

double sin(double x)
{
    int halfPeriodsNum = x / PI;
    x = x - halfPeriodsNum * PI;
    doudle toReturn = 0;
    for (size_t i = 0; i < N; i++)
    {
        toReturn += power(-1, (i - 1) / 2) * power(x, POWERS[i]) / factorial(i);
    }
    if (halfPeriodsNum % 2 == 1)
    {
        toReturn *= -1;
    }
    return toReturn;
}

// helper functions
double power(double x, int a)
{
    double toReturn = x;
    for (size_t i = 0; i < a; i++)
    {
        toReturn *= x;
    }
    return toReturn;
}

int factorial(int x)
{
    int toReturn = 1;
    for (size_t i = 2; i < x + 1; i++)
    {
        toReturn *= i;
    }
    return toReturn;
}