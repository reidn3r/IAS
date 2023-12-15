#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int FileIsNull(FILE *input){
    return (input == NULL) ? 1 : 0;
}

void removerNumeros(char *str) {
    int lidos = 0; // Número de caracteres lidos
    int escritos = 0; // Número de caracteres escritos

    while (str[lidos] != '\0' && !isdigit((unsigned char)str[lidos])) {
        // Copia caracteres não numéricos para a posição de escrita
        str[escritos++] = str[lidos++];
    }

    while (str[lidos] != '\0' && isdigit((unsigned char)str[lidos])) {
        // Pula caracteres numéricos
        lidos++;
    }

    while (str[lidos] != '\0') {
        // Copia o restante da string após os caracteres numéricos
        str[escritos++] = str[lidos++];
    }

    // Adiciona o caractere nulo ao final da string
    str[escritos] = '\0';
}

void line (char *symbols, int length) {
    // faz uma linha de tamanho 'length' com os caracteres de 'symbols'

    int x = strlen(symbols);

    for (int i = 0; i < length/x; i++) {
        printf("%s", symbols);
    }
    printf("\n");
}