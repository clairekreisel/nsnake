#include "helper.h"
//TODO: Implement numeric class of max size inf

int power(int base, unsigned int exp){
    int p = base;
    int r = 1;

    while (exp > 0){
        if (exp % 2 == 1){
            r *= p;
        }
        p *= p;
        exp /= 2;
    }
    return r;
}
