#ifndef UTILS_h
#define UTILS_h

#include <stdio.h>
#include <inttypes.h>

typedef struct IAS {
    int64_t MBR;    // Memory buffer register
    int64_t MAR;    // Memory address register

    int64_t IR;     // Instruction register
    int64_t IBR;    // Instruction buffer register

    int64_t PC;     // Program counter
    int64_t AC;     // Accumulator
    int64_t MQ;     // Multiplier quotient 

    int64_t memory[4096]; // Memória tem tamanho de 4096 palavras
    int64_t instructionsStart; // Onde começam as instruções
    int memorylimit; // Quanto da memória realmente está sendo usada
} IAS;

typedef struct PIPELINE {
    int64_t BM;     // Busca na Memória
    int64_t DC;     // Decodificação
    int64_t BO;     // Busca dos Operandos
    int64_t EX;     // Execução
    int64_t ER;     // Escrita dos Resultados
} PIPELINE;

int FileIsNull(FILE *input);
int removerX(char *str);
void removerNewLine(char *str);
unsigned int strToBin(const char *strBinario);
void printBinary(long long n);
void line (char *symbols, int length);

char *opcode_index(char *op, char *op_list[], char *binary_opcode[]);
void write_output(IAS ias, FILE *output, long long words[], int size);

int64_t extrairInstrucao (int64_t *memory, int64_t PC);
int64_t absolute(int64_t n);

void assign_clock_array(int clock, char *opcode_buffer, int cicle_array[], char *opcode_array[]);
int isNumeric(const char *str);
void clearPipeline (PIPELINE *pip);
int isPipelineCleared (PIPELINE pip);

int buscarNaMemoria (IAS *ias, PIPELINE *pip);
int decodificar (IAS *ias, PIPELINE *pip);
int buscarOperandos (IAS *ias, PIPELINE *pip);
int executar (IAS *ias, PIPELINE *pip, int *cycles);
int escreverRes (IAS *ias, PIPELINE *pip);


#endif