#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct 
{
   int64_t value;
} real;

real add_real_to_real (real a, real b);
real add_real_to_integer (real a, int64_t b);
real mul_real_by_real (real a, real b);
real mul_real_by_integer (real a, int64_t b);
real sub_real_from_real (real a, real b);
real sub_int_from_real (real a, int64_t b);
real sub_real_from_int (int64_t a, real b);
real div_real_by_real (real a, real b);
real div_real_by_int (real a, int64_t b);
real div_int_by_real (int64_t a, real b);

real get_real_value(int64_t a);
int get_int_value (real a);
