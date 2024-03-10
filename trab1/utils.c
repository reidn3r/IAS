#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int FileIsNull(FILE *input){
    return (input == NULL) ? 1 : 0;
}

int64_t extrairInstrucao (int64_t *memory, int64_t PC) {
    // TODO verificar se chegou no final da memoria e retornar 0

    int64_t word = memory[PC / 2];

    if (PC % 2 == 0) {
        return word >> 20;

    } else {
        return word & ((1 << 20) -1);
    }
}


int buscarNaMemoria (IAS *ias, PIPELINE *pip) {
    // Busca na memoria
    // Acabados os ciclos da busca, verifica se proximo passo do pipeline == 0
    // Caso verdadeiro, proximo passo recebe o q esta na busca e busca recebe 0
    // Talvez nem precise retornar então

    if (pip->DC == 0 && pip->BM != 0) {
        pip->DC = pip->BM;
        pip->BM = 0;
        return 1;
    }
    return 0;
}

int decodificar (IAS *ias, PIPELINE *pip) {

    if (pip->BO == 0 && pip->DC != 0) {
        pip->BO = pip->DC;
        pip->DC = 0;
        return 1;
    }
    return 0;
}

int buscarOperandos (IAS *ias, PIPELINE *pip) {

    if (pip->EX == 0 && pip->BO != 0) {
        pip->EX = pip->BO;
        pip->BO = 0;
        return 1;
    }
    return 0;
}

int executar (IAS *ias, PIPELINE *pip, int *cycles) {

    (*cycles)--;

    if (!(pip->ER == 0 && pip->EX != 0 && *cycles < 1)) return 0;

    // Executar

    int opcode = pip->EX >> 12; // Extrai o opcode
    int X = pip->EX & ((1 << 12) -1); // Extrai o X

    switch (opcode) {

        // -> Data transfer
        case 0b00001010: // LOAD MQ Transfer contents of register MQ to the accumulator AC
            ias->AC = ias->MQ;
            break;
        case 0b00001001: // LOAD MQ,M(X) Transfer contents of memory location X to MQ
            ias->MQ = ias->memory[X];
            break;
        case 0b00100001: // STOR M(X) Transfer contents of accumulator to memory location X
            ias->memory[X] = ias->AC;
            break;
        case 0b00000001: // LOAD M(X) Transfer M(X) to the accumulator
            ias->AC = ias->memory[X];
            break;
        case 0b00000010: // LOAD –M(X) Transfer –M(X) to the accumulator
            ias->AC = ias->memory[X] * -1;
            break;
        case 0b00000011: // LOAD |M(X)| Transfer absolute value of M(X) to the accumulator
            ias->AC = absolute(ias->memory[X]);
            break;
        case 0b00000100: // LOAD –|M(X)| Transfer –|M(X)| to the accumulator
            ias->AC = absolute(ias->memory[X]) * -1;
            break;

        // -> Unconditional branch
        case 0b00001101: // JUMP M(X,0:19) Take next instruction from left half of M(X)
            clearPipeline(pip);
            ias->PC = X;
            break;
        case 0b00001110: // JUMP M(X,20:39) Take next instruction from right half of M(X)
            clearPipeline(pip);
            ias->PC = X + 1;
            break;

        // -> Conditional branch
        case 0b00001111: // JUMP + M ( X , 0 : 19) If number in the accumulator is nonnegative, take next instruction from left half of M(X)
            break;
        case 0b00010000: // JUMP + M ( X , 20 : 39) If number in the accumulator is nonnegative, take next instruction from right half of M(X)
            break;

        // -> Arithmetic
        case 0b00000101: // ADD M(X) Add M(X) to AC; put the result in AC
            break;
        case 0b00000111: // ADD |M(X)| Add |M(X)| to AC; put the result in AC
            break;
        case 0b00000110: // SUB M(X) Subtract M(X) from AC; put the result in AC
            break;
        case 0b00001000: // SUB |M(X)| Subtract |M(X)| from AC; put the remainder in AC
            break;
        case 0b00001011: // MUL M(X) Multiply M(X) by MQ; put most significant bits of result in AC, put least significant bits in MQ
            break;
        case 0b00001100: // DIV M(X) Divide AC by M(X); put the quotient in MQ and the remainder in AC
            break;
        case 0b00010100: // LSH Multiply accumulator by 2; that is, shift left one bit position
            break;
        case 0b00010101: // RSH Divide accumulator by 2; that is, shift right one position
            break;

        // -> Address modify
        case 0b00010010: // STOR M(X,8:19) Replace left address field at M(X) by 12 rightmost bits of AC
            break;
        case 0b00010011: // STOR M(X,28:39) Replace right address field at M(X) by 12 rightmost bits of AC
            break;

    }

    pip->ER = pip->EX;
    pip->EX = 0;
    return 1;
    
}

int64_t absolute(int64_t n){
    return (n >= 0) ? n : -n;
}

int escreverRes (IAS *ias, PIPELINE *pip) {
    if (pip->ER == 0) return 0;

    // Escrever resultado

    pip->ER = 0;

    return 1;
}

int isPipelineCleared (PIPELINE pip) {
    return (!pip.BM && !pip.DC && !pip.BO && !pip.EX && !pip.ER);
}

void clearPipeline (PIPELINE *pip) {
    pip->BM = 0; pip->BO = 0; pip->DC = 0;
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

int isNumeric(const char *str) {
    // Verifica se a string é vazia
    if (*str == '\0'){
        return 0;
    }

    // Verifica cada caractere da string
    while (*str != '\0') {
        // Se o caractere não for um dígito, retorna 0
        if (!isdigit(*str)){
            return 0;
        }
        str++; // Move para o próximo caractere
    }

    // Se todos os caracteres forem dígitos, retorna 1
    return 1;
}

void assign_clock_array(int clock, char *opcode_buffer, int cicle_array[], char *opcode_array[]){
    char *opcode_token;
    char opcode_copy[32]; //Copia para evitar segmentation fault
    for(int i=0; i<21; i++){
        strcpy(opcode_copy, opcode_array[i]);
        opcode_token = strtok(opcode_copy, " ");
        if(strcmp(opcode_token, opcode_buffer) == 0){
            cicle_array[i] = (clock > 0) ? clock : 0;
        }
    }
}