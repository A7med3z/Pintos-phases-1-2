#include "fixed_point.h"
#include <stdio.h>
#include <stdlib.h>


real add_real_to_real (real a, real b) {
    real c = {a.value + b.value};
    return c;
}

real add_real_to_integer (real a, int64_t b) {
    real c = {a.value + get_real_value (b).value};
    return c;
}

real mul_real_by_real (real a, real b) {
    int64_t x = a.value * b.value;
    real c = {x >> 14};
    return c;
}

real mul_real_by_integer (real a, int64_t b) {
    real c = {a.value * b};
    return c;
}

real sub_real_from_real (real a, real b) {
    real c = {a.value - b.value};
    return c;
}

real sub_int_from_real (real a, int64_t b) {
    real c = {a.value - get_real_value (b).value};
    return c;
}

real sub_real_from_int (int64_t a, real b) {
    real c = {get_real_value (a).value - b.value};
    return c;
}

real div_real_by_real (real a, real b) {
    int64_t x = a.value << 14;
    real c = {x / b.value};
    return c;
}

real div_real_by_int (real a, int64_t b) {
    real c = {a.value / b};
    return c;
}

real div_int_by_real (int64_t a, real b) {
    real c = {(get_real_value (a).value / b.value) << 14};
    return c;
}


real get_real_value(int64_t a) {
    real c = {a << 14};
    return c;
}

int get_int_value (real a) {
    return (int) (a.value >> 14);
}

/* int main() {
    real a = get_real_value (900);
    real b = get_real_value (800);
    printf("\na = %d", get_int_value (a));
    printf("\nb = %d", get_int_value (b));
    printf("\nreal answer = %ld", mul_real_by_real (a, b).value);
    printf("\ninteger answer = %d\n", get_int_value (mul_real_by_real (a, b)));
    return 0;
} */