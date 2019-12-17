#ifndef POCO_PROJECT_UTILS_H
#define POCO_PROJECT_UTILS_H

// for Maclaurin series approximation of sin()
static const size_t N = 4;
static const double[N] POWERS = {1, 3, 5, 7};
static const double[N] FACTORIALS = {1, 6, 120, 5040};

double PI = 3.141592653589793;


double sin(double x);

double hammingCode();

// helper functions
double power(double x, int a);
int factorial(int x);

#endif //POCO_PROJECT_UTILS_H
