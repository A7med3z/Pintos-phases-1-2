#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct 
{
   int value;
} real;

real add_real_to_real (real a, real b);
real add_real_to_integer (real a, int b);
real mul_real_by_real (real a, real b);
real mul_real_by_integer (real a, int b);
real sub_real_from_real (real a, real b);
real sub_int_from_real (real a, int b);
real sub_real_from_int (int a, real b);
real div_real_by_real (real a, real b);
real div_real_by_int (real a, int b);
real div_int_by_real (int a, real b);

real get_real_value(int a);
int get_int_value (real a);

