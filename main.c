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

int main(){
    
    FILE *input = fopen("input.txt", "r");
    FILE *output = fopen("output.txt", "w+b");
    IAS ias;
        
    char *OP_ARRAY[] = {"LOAD MQ", "LOAD MQ,M", "STOR M", "LOAD M", "LOAD -M", "LOAD |M", "LOAD -|M", "JUMP M", "JUMP M", "JUMP+ M","JUMP+ M", "ADD M", "ADD |M", "SUB M", "SUB |M", "MUL M", "DIV M", "LSH", "RSH", "STOR M", "STOR M"};

    char *BINARY[] = {"00001010", "00001001", "00100001", "00000001", "00000010", "00000011", "00000100", "00001101", "00001110", "00001111", "00010000", "00000101", "00000111", "00000110", "00001000", "00001011", "00001100", "00010100", "00010101", "00010010", "00010011"};
    
    char buffer[MAX_TEXT_LENGTH];
    char *argument, *instruction;
    int lineCounter = 1;
    int data;

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
    while(fgets(buffer, MAX_TEXT_LENGTH, input)){
        /* 
            - Identifica instruções
        */
        line("-=", 18);
        printf("-=-=Linha %d=-=-\n", lineCounter);

        instruction = strtok(buffer, "(");
        binary_opcode = opcode_index(instruction, OP_ARRAY, BINARY);
        printf("%s: %s\n", instruction, binary_opcode);
        lineCounter++;
    }


    fclose(input);
    fclose(output);
    return 0;
}

char* opcode_index(char *op, char *op_list[], char *binary_opcode[]){
    for(int i=0; i<21; i++){
        if(strcmp(op_list[i], op) == 0){
            return binary_opcode[i];
        }
    }
    return "";
}
