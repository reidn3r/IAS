#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"

#define MAX_TEXT_LENGTH 40
#define DATA_SIZE 500
#define PROGRAM_ADDR_START 500

typedef struct IAS {
    long AC;
    long MQ;
    long long int main_memo[1024*4];
    /* Add. outros registradores */
} IAS;

char *opcode_index(char *op, char *op_list[], char *binary_opcode[]);
void write_output(IAS ias, FILE *output, long long words[], int size);

int main(){
    
    FILE *input = fopen("input.txt", "r");
    FILE *output = fopen("output.txt", "w");
    IAS ias;
        
    char *OP_ARRAY[] = {"LOAD MQ", "LOAD MQ,M()", "STOR M()", "LOAD M()", "LOAD -M()", "LOAD |M()|", "LOAD -|M()|", "JUMP M(,0:19)", "JUMP M(,20:39)", "JUMP+ M(,0:19)","JUMP+ M(,20:39)", "ADD M()", "ADD |M()|", "SUB M()", "SUB |M()|", "MUL M()", "DIV M()", "LSH", "RSH", "STOR M(,8:19)", "STOR M(,28:39)"};

    char *BINARY[] = {"00001010", "00001001", "00100001", "00000001", "00000010", "00000011", "00000100", "00001101", "00001110", "00001111", "00010000", "00000101", "00000111", "00000110", "00001000", "00001011", "00001100", "00010100", "00010101", "00010010", "00010011"};
    
    char buffer[MAX_TEXT_LENGTH], instructionText[MAX_TEXT_LENGTH];
    char *argument;
    int lineCounter = 0;
    int data, address = -1;

    if(FileIsNull(input)) printf("Arquivo de instrucoes nao foi aberto");

    for(int i=0; i<DATA_SIZE; i++){
        /* 
            - Armazena dados em memória
        */
        fgets(buffer, MAX_TEXT_LENGTH, input);
        data = atoi(buffer);
        ias.main_memo[i] = data;
    }
    
    char *binary_opcode;

    int instructionInt;
    int instructionsInt[256];

    while(fgets(buffer, MAX_TEXT_LENGTH, input)){
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
    }

    // Enumera as instrucoes compiladas e constroi as 'words' do IAS juntando duas instrucoes
    line("~", 30);
    printf("\tInstrucoes\n");
    long long words[256];
    for (int i = 0; i < lineCounter; i++) {
        printf("Instrucao %d: ", i+1);
        printBinary(instructionsInt[i]);

        if (i % 2 == 0) {
            words[i/2] = instructionsInt[i];
        } else {
            words[i/2] = (words[i/2] << 20) | instructionsInt[i];
        }
    }

    line("~", 30);
    printf("\tPalavras\n");
    for (int i = 0; i < lineCounter/2; i++) {
        line("-", 20);
        printf("Palavra %d: %lld\n", i+1, words[i]);
        printBinary(words[i]);
    }

    int size = sizeof(words)/sizeof(long long);
    printf("SIZE: %d\n", size);
    write_output(ias, output, words, lineCounter/2);
    // write_output(ias, output, words, size);

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
        fprintf(output, "%lld\n", ias.main_memo[j]);
    }
    for(int i=0; i<size; i++){
        fprintf(output, "%lld\n", words[i]);
    }
}
