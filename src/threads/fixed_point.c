#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "fixed_point.h"


real add_real_to_real (real a, real b) {
    real c = {a.value + b.value};
    return c;
}

real add_real_to_integer (real a, int b) 
{
    real b_ = get_real_value (b);
    return add_real_to_real (a, b_);
}

real mul_real_by_real (real a, real b) {
    int64_t x = a.value * b.value;
    real c = {x >> 14};
    return c;
}

real mul_real_by_integer (real a, int b) {
    real c = {a.value * b};
    return c;
}

real sub_real_from_real (real a, real b) {
    real c = {a.value - b.value};
    return c;
}

real sub_int_from_real (real a, int b) {
    real b_ = get_real_value (b);
    return sub_real_from_real (a, b_);
}

real sub_real_from_int (int a, real b) {
    real a_ = get_real_value (a);
    return sub_real_from_real (a_, b);
}

real div_real_by_real (real a, real b) {
    int64_t x = a.value << 14;
    real c = {x / b.value};
    return c;
}

real div_real_by_int (real a, int b) {
    real c = {a.value / b};
    return c;
}

real div_int_by_real (int a, real b) {
    real a_ = get_real_value (a);
    return div_real_by_real (a_, b);
}


real get_real_value(int a) {
    real c = {(int64_t) a << 14};
    return c;
}

int get_int_value (real a) {
    return (a.value >= 0) ? (int) ((a.value + (1 << 13)) >> 14) : (int) ((a.value - (1 << 13)) >> 14);
}
