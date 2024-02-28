#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <ctype.h>
#include <stdint.h>
#include <inttypes.h>

#include "utils.h"

#define MAX_TEXT_LENGTH 40
#define DATA_SIZE 500
#define PROGRAM_ADDR_START 500

typedef struct IAS {
    int64_t MBR;    // Memory buffer register
    int64_t MAR;    // Memory address register

    int64_t IR;     // Instruction register
    int64_t IBR;    // Instruction buffer register

    int64_t PC;     // Program counter
    int64_t AC;     // Accumulator
    int64_t MQ;     // Multiplier quotient 

    int64_t memory[4096]; // Memória tem tamanho de 4096 palavras
    int memorylimit; // Quanto da memória realmente está sendo usada
} IAS;

typedef struct PIPELINE {
    int64_t BM;
    int64_t DC;
    int64_t BO;
    int64_t EX;
    int64_t ER;
} PIPELINE;

char *OP_ARRAY[] = {"LOAD MQ", "LOAD MQ,M()", "STOR M()", "LOAD M()", "LOAD -M()", "LOAD |M()|", "LOAD -|M()|", "JUMP M(,0:19)", "JUMP M(,20:39)", "JUMP+ M(,0:19)","JUMP+ M(,20:39)", "ADD M()", "ADD |M()|", "SUB M()", "SUB |M()|", "MUL M()", "DIV M()", "LSH", "RSH", "STOR M(,8:19)", "STOR M(,28:39)"};

char *BINARY[] = {"00001010", "00001001", "00100001", "00000001", "00000010", "00000011", "00000100", "00001101", "00001110", "00001111", "00010000", "00000101", "00000111", "00000110", "00001000", "00001011", "00001100", "00010100", "00010101", "00010010", "00010011"};

int CYCLES[21]; // Será preenchido depois 

char *opcode_index(char *op, char *op_list[], char *binary_opcode[]);
void write_output(IAS ias, FILE *output, long long words[], int size);

int main (int argc, char *argv[]){
    FILE *input;
    FILE *output;
    IAS ias;
    
    // Verifica se o programa foi chamado corretamente
    if (argc == 5 && strcmp(argv[1], "-p") == 0 && strcmp(argv[3], "-i") == 0) {
        input = fopen(argv[2], "r");
        output = fopen(strcat(argv[2],".out"), "w");
    } else {
        fprintf(stderr, "Argumentos incorretos!\n");
        fprintf(stderr, "Modo de uso:\n");
        fprintf(stderr, "$ %s -p arquivo_de_entrada -i endereco_inicial_de_PC\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if(FileIsNull(input)) printf("Arquivo de instrucoes nao foi aberto");

    /*
    * Lê quantos ciclos cada instrução leva
    */

        // TODO
        // fgets lê até onde começam os dados

    /*
    * Armazena dados em memória
    */

    int data;
    char buffer[MAX_TEXT_LENGTH];

    int memorylimit = 0;
    fgets (buffer, MAX_TEXT_LENGTH, input);

    while (isdigit(*buffer)) { // Lê até encontrar algo que não é número
        ias.memory[memorylimit] = atoi(buffer);
        memorylimit++;
        fgets (buffer, MAX_TEXT_LENGTH, input);
    }
    
    /*
    * Armazena instruções em memória
    */

    char instructionText[MAX_TEXT_LENGTH];
    char *argument;
    int lineCounter = 0;
    int address = -1;

    char *binary_opcode;
    int instructionInt;
    int instructionsInt[256];

    do {
        instructionInt = 0;

        line("-", 30);
        printf("-=-=-=-=-=Linha %d=-=-=-=-=-\n", lineCounter+1);

        // Remove \n
        removerNewLine(buffer);
        printf("buffer:\t\t%s\n", buffer);

        // Separa intrucao de endereco de memoria
        strcpy(instructionText, buffer);
        address = removerX(instructionText);
        binary_opcode = opcode_index(instructionText, OP_ARRAY, BINARY);
        printf("opcode[txt]:\t%s\n", instructionText);
        printf("endereco:\t%d\n", address);

        // Adiciona opcode binario na instrucao
        instructionInt = instructionInt | strToBin(binary_opcode);
        printf("binary_opcode: %s\n", binary_opcode);
        printf("strToBin: %d\n", strToBin(binary_opcode));
        printf("opcode[bin]:\t");
        printBinary(instructionInt);

        // Adiciona endereco binario na instrucao
        instructionInt = instructionInt << 12;
        instructionInt = instructionInt | address;
        printf("instrucao:\t");
        printBinary(instructionInt);

        // Salva em vetor de instrucoes
        instructionsInt[lineCounter] = instructionInt;

        lineCounter++;
    } while(fgets(buffer, MAX_TEXT_LENGTH, input));

    // Enumera as instrucoes compiladas e constroi as 'words' do IAS juntando duas instrucoes
    line("~", 30);
    printf("\tInstrucoes\n");
    long long words[256];
    for (int i = 0; i < lineCounter; i++) {
        printf("Instrucao %d: ", i+1);
        printBinary(instructionsInt[i]);

        if (i % 2 == 0) {
            words[i/2] = instructionsInt[i];
            words[i/2] = words[i/2] << 20;
        } else {
            words[i/2] = words[i/2] | instructionsInt[i];
        }

        if ((i % 2 != 0) || (i == lineCounter-1)) {
            ias.memory[memorylimit] = words[i/2];
            memorylimit++;
        }
    }

    line("~", 30);
    printf("\tPalavras\n");
    for (int i = 0; i < lineCounter/2 + 1; i++) {
        line("-", 20);
        printf("Palavra %d: %lld\n", i+1, words[i]);
        printBinary(words[i]);
    }

    int size = sizeof(words)/sizeof(long long);
    printf("SIZE: %d\n", size);
    write_output(ias, output, words, lineCounter/2);
    // write_output(ias, output, words, size);

    /* * * * * * * * * * * * * * * * *
    *         HORA DE SIMULAR!       *
     * * * * * * * * * * * * * * * * */

    /*
    int PC;
    int statusER, statusEX, statusBO, statusDC, status BM = 0; // Significa se o prioxmo passo do pipeline está livre e pode receber outra instrução
    while (tem instrução as executar) {
        statusER, statusEX, statusBO, statusDC, status BM = 0; // ?

        statusER = escreverRes(ER); // ?

        if (statusER) {
            statusEX = executar(EX);
            ER = EX;
        }

        if (statusEX) {
            statusBO = buscarOperandos(BO);
            EX = BO;
        }

        if (statusBO) {
            decodificar(DC);
            BO = DC;

        if (statusDC) {
            if (PC % 2 == 0) {
                BM = extrairInstruçãoDaEsquerda();
            } else {
                BM = extrairInstruçãoDaDireita();
            }
            buscarNaMemo(BM);
            DC = BM;
        }
    }
    */

    // Mostra o que está na memória
    line("~", 30);
    printf("      Memória compilada\n");
    for (int i = 0; i < memorylimit; i++) {
        printf("-\t%ld\t-\n", ias.memory[i]);
    }

    // Finaliza
    fclose(input);
    fclose(output);
    return 0;
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
    for(int j=0; j<DATA_SIZE; j++){
        fprintf(output, "%" PRId64 "\n", ias.memory[j]);
    }
    for(int i=0; i<size; i++){
        // fprintf(output, "%lld\n", words[i]);
    }
}
