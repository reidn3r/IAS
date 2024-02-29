#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int FileIsNull(FILE *input){
    return (input == NULL) ? 1 : 0;
}

int extrairInstrucaoDaEsquerda (int instruction) {
    return 1;
}

int extrairInstrucaoDaDireita (int instruction) {
    return 1;
}

int buscarNaMemoria (IAS ias, PIPELINE pip) {
    // Busca na memoria
    // Acabados os ciclos da busca, verifica se proximo passo do pipeline == 0
    // Caso verdadeiro, proximo passo recebe o q esta na busca e busca recebe 0
    // Talvez nem precise retornar então
    return 1;
}

int decodificar (IAS ias, PIPELINE pip) {
    return 1;
}

int buscarOperandos (IAS ias, PIPELINE pip) {
    return 1;
}

int executar (IAS ias, PIPELINE pip) {
    return 1;
}

int escreverRes (IAS ias, PIPELINE pip) {
    return 1;
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

void removerNewLine(char *str) {
    int comprimento = strlen(str);

    // Verifica se a string tem pelo menos um caractere e se o último é '\n'
    if (comprimento > 0 && (str[comprimento - 1] == '\n' || str[comprimento - 1] == '\r')) {
        // Substitui o '\n' ou '\r' pelo caractere nulo '\0'
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

void printBinary(long long n) {
    if (n == 0) {
        printf("0");
        return;
    }

    int bits[64];
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

char* opcode_index(char *op, char *op_list[], char *binary_opcode[]){
    printf("-- comparando...\n");
    for(int i=0; i<21; i++){
        printf("- %s, %s\n", op_list[i], op);
        if(strncmp(op_list[i], op, strlen(op_list[i])) == 0){
            printf("-- achei!\n");
            return binary_opcode[i];
        }
    }
    return "";
}


void write_output(IAS ias, FILE *output, long long words[], int size){
    for(int j=0; j<size; j++){
        fprintf(output, "%" PRId64 "\n", ias.memory[j]);
    }
    for(int i=0; i<size; i++){
        // fprintf(output, "%lld\n", words[i]);
    }
}
