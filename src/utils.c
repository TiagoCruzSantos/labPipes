#include "../include/utils.h"

int vecSum(int size, int vec[size]){
    int k = 0;
    for(int i = 0; i < size; i++){
        k+= vec[i];
    }
    return k;
}