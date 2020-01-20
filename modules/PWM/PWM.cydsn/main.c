/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"

double PI = 3.14;
int N = 4;
static const double POWERS[] = {1, 3, 5, 7};
static const double FACTORIALS[] = {1, 6, 120, 5040};


double sin(double x)
{
    int halfPeriodsNum = x / PI;
    x = x - halfPeriodsNum * PI;
    double toReturn = 0;
    for (int i = 0; i < N; i++)
    {
        toReturn +=  power(-1, (i - 1) / 2) * power(x, POWERS[i]) / FACTORIALS[i];
    }
    if (halfPeriodsNum % 2 == 1)
    {
        toReturn *= -1;
    }
    return toReturn;
}

 //helper functions
int power(int x, double a)
{
    double toReturn = x;
    for (int i = 0; i < a; i++)
    {
        toReturn *= x;
    }
    return toReturn;
}

double Sine_Lookup[64] = {0x19,0x1b,0x1e,0x20,0x23,0x25,0x27,0x29, 0x2b,0x2c,0x2e,0x2f,0x30,0x31,0x32,0x32, 0x32,0x32, 0x32,0x31,0x30,0x2f,0x2e,0x2c, 0x2b,0x29,0x27,0x25,0x23,0x20,0x1e,0x1b, 0x19,0x17, 0x14,0x12,0xf, 0xd,0xb,0x9, 0x7,0x6,0x4,0x3,0x2,0x1,0x0,0x0, 0x0,0x0,0x0,0x1,0x2,0x3,0x4,0x6, 0x7,0x9,0xb,0xd,0xf,0x12,0x14,0x17};
static const uint16_t sinetable[256] = {31, 32, 33, 33, 34, 35, 36, 36, 37, 38, 39, 39, 40, 41, 41, 42, 43, 43, 44, 45, 45, 46, 46, 47, 48, 48, 49, 49, 50, 50, 51, 51, 51, 52, 52, 53, 53, 53, 54, 54, 54, 55, 55, 55, 55, 56, 56, 56, 56, 56, 56, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 58, 58, 58, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 56, 56, 56, 56, 56, 56, 55, 55, 55, 55, 54, 54, 54, 53, 53, 53, 52, 52, 51, 51, 51, 50, 50, 49, 49, 48, 48, 47, 46, 46, 45, 45, 44, 43, 43, 42, 41, 41, 40, 39, 39, 38, 37, 36, 36, 35, 34, 33, 33, 32, 31, 30, 29, 29, 28, 27, 26, 26, 25, 24, 23, 23, 22, 21, 21, 20, 19, 19, 18, 17, 17, 16, 16, 15, 14, 14, 13, 13, 12, 12, 11, 11, 11, 10, 10, 9, 9, 9, 8, 8, 8, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 11, 11, 11, 12, 12, 13, 13, 14, 14, 15, 16, 16, 17, 17, 18, 19, 19, 20, 21, 21, 22, 23, 23, 24, 25, 26, 26, 27, 28, 29, 29, 30};


static const char LETTERS[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 
    'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    //char msg[5] = {'a', 'b', 'c', 'd', 'e', 'f'};
    
    //int numbers[5];
    //for (int i = 0; i < 5; i+=1) {
      //  for (int j = 0; j < 26; j+=1) {
        ///    if (LETTERS[j] == msg[i]) {
           ///     numbers[i] = j;
              //  break;
    //        }
      //  }
    //}
    
   // int coded_msg[25];
    //for (int i = 0; i < 5; i+=1) {
     //   for (int j = 0; j < 5; j+=1) {
       //     coded_msg[(i + 1) * j] = LETTERS[j];
    //    } 
    ///}
    
    int codedMsg[25] = {0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0};
    
    PWM_1_Start();
    int k = -1;
    int readRate = -1;
    double modulationCoeff = 0.5;
    for (;;)
    {
        readRate += 1;
        if(readRate == 4) {
            readRate = -1;
            k += 1;
        } 
        if (codedMsg[k % 15] == 1) {
            modulationCoeff = 0.0;
        }
        else {
            modulationCoeff = 0.5;
        }
        for(int i = 0; i < 256 * 2; i+= 1 ) {
            double value = sinetable[i / 2];
            //double value = (sinetable[i / 2] - 29) * (modulationCoeff + codedMsg[k % 15]) + 29;
            PWM_1_WriteCompare(value);
        }
}
}
