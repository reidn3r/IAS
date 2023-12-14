#include "utils.h"
#include <stdio.h>
#include <string.h>

int FileIsNull(FILE *input){
    return (input == NULL) ? 1 : 0;
}

void line (char *symbols, int length) {
    // faz uma linha de tamanho 'length' com os caracteres de 'symbols'

    int x = strlen(symbols);

    for (int i = 0; i < length/x; i++) {
        printf("%s", symbols);
    }
    printf("\n");
}