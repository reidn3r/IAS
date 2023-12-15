#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int FileIsNull(FILE *input){
    return (input == NULL) ? 1 : 0;
}

int removerX(char *str) { // Remove e retorna a parte variável da string da instrução
    int lidos = 0; // Número de caracteres lidos
    int escritos = 0; // Número de caracteres escritos
    char X[4] = "-1";

    while (str[lidos] != '\0' && !isdigit((unsigned char)str[lidos])) {
        // Copia caracteres não numéricos para a posição de escrita
        str[escritos++] = str[lidos++];
    }

    if (str[lidos] != '\0') X[0] = '0'; X[1] = '\0';

    while (str[lidos] != '\0' && isdigit((unsigned char)str[lidos])) {
        // Pula caracteres numéricos
        X[lidos-escritos] = str[lidos];
        lidos++;
    }

    while (str[lidos] != '\0') {
        // Copia o restante da string após os caracteres numéricos
        str[escritos++] = str[lidos++];
    }

    // Adiciona o caractere nulo ao final da string
    str[escritos] = '\0';

    return atoi(X);
}

void removerNewLine(char *str) { // Remove o \n do final de string
    int comprimento = strlen(str);

    // Verifica se a string tem pelo menos um caractere e se o último é '\n'
    if (comprimento > 0 && str[comprimento - 1] == '\n') {
        // Substitui o '\n' pelo caractere nulo '\0'
        str[comprimento - 1] = '\0';
    }
}

unsigned int strToBin(const char *strBinario) {
    return strtoul(strBinario, NULL, 2);
}

void line (char *symbols, int length) {
    // faz uma linha de tamanho 'length' com os caracteres de 'symbols'

    int x = strlen(symbols);

    for (int i = 0; i < length/x; i++) {
        printf("%s", symbols);
    }
    printf("\n");
}

void printBinary(int n) {
    if (n == 0) {
        printf("0");
        return;
    }

    int bits[1024];
    int i = 0;

    while (n > 0) {
        bits[i++] = n % 2;
        n /= 2;
    }

    for (int j = i - 1; j >= 0; j--) {
        printf("%d", bits[j]);
    }
    printf("\n");
}
