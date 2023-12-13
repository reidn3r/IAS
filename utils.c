#include "utils.h"
#include <stdio.h>

int FileIsNull(FILE *input){
    return (input == NULL) ? 1 : 0;
}