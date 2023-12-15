#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int FileIsNull(FILE *input){
    return (input == NULL) ? 1 : 0;
}

void removerX(char *str) { // Remove a parte variável da string da instrução
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

void removerNewLine(char *str) { // Remove o \n do final de string
    size_t comprimento = strlen(str);

    // Verifica se a string tem pelo menos um caractere e se o último é '\n'
    if (comprimento > 0 && str[comprimento - 1] == '\n') {
        // Substitui o '\n' pelo caractere nulo '\0'
        str[comprimento - 1] = '\0';
    }
}

void line (char *symbols, int length) {
    // faz uma linha de tamanho 'length' com os caracteres de 'symbols'

    int x = strlen(symbols);

    for (int i = 0; i < length/x; i++) {
        printf("%s", symbols);
    }
    printf("\n");
}