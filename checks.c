#include "checks.h"
#include <math.h>

int check_hfe(unsigned int hfe1, unsigned int hfe2) {
    return abs(hfe1 - hfe2) < 8;
}

int check_input_r(unsigned int input_r1, unsigned int input_r2) {
    return abs(input_r1 - input_r2) < 2500;
}
