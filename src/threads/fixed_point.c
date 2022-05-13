#include "fixed_point.h"
#include <stdio.h>
#include <stdlib.h>

real add_real_to_real (real a, real b) {
    real c = {a.value + b.value};
    return c;
}

real add_real_to_integer (real a, int b) {
    real c = {a.value + get_real_value (b).value};
    return c;
}

real mul_real_by_real (real a, real b) {
    real c = {(((int64_t)a.value) * b.value) >> 14};
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
    real c = {a.value - get_real_value (b).value};
    return c;
}

real sub_real_from_int (int a, real b) {
    real c = {get_real_value (a).value - b.value};
    return c;
}

real div_real_by_real (real a, real b) {
    real c = {((((int64_t)a.value)  << 14 )/ b.value)};
}

real div_real_by_int (real a, int b) {
    real c = {a.value / b};
    return c;
}

real div_int_by_real (int a, real b) {
    real c = {(get_real_value (a).value / b.value) << 14};
    return c;
}


real get_real_value(int a) {
    real c = {a << 14};
    return c;
}

int get_int_value (real a) {
    return a.value >> 14;
}