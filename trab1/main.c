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

char *OP_ARRAY[] = {"LOAD MQ", "LOAD MQ,M()", "STOR M()", "LOAD M()", "LOAD -M()", "LOAD |M()|", "LOAD -|M()|", "JUMP M(,0:19)", "JUMP M(,20:39)", "JUMP+ M(,0:19)","JUMP+ M(,20:39)", "ADD M()", "ADD |M()|", "SUB M()", "SUB |M()|", "MUL M()", "DIV M()", "LSH", "RSH", "STOR M(,8:19)", "STOR M(,28:39)"};

char *BINARY[] = {"00001010", "00001001", "00100001", "00000001", "00000010", "00000011", "00000100", "00001101", "00001110", "00001111", "00010000", "00000101", "00000111", "00000110", "00001000", "00001011", "00001100", "00010100", "00010101", "00010010", "00010011"};

int CYCLES[21]; // Será preenchido depois 

int main (int argc, char *argv[]){
    FILE *input;
    FILE *output;
    IAS ias;
    PIPELINE pip;
    
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
    char clock_opcode[MAX_TEXT_LENGTH], *opcode_buffer;
    int clock_opcode_counter, clocks, clock_counter= 0; 0; 0;
    int isnumeric_opcode = 0;
    do{
        fgets(clock_opcode, MAX_TEXT_LENGTH, input);
        opcode_buffer = strtok(clock_opcode, ": "); //Pega o opcode, parte textual de opcode: ciclos
        removerNewLine(opcode_buffer);

        isnumeric_opcode = isNumeric(opcode_buffer);
        char *clocks_string = strtok(NULL, " "); //Pega o numero que representa o clock
        if(!isnumeric_opcode){
            if(clocks_string == NULL){
                break;
            }

            clocks = atoi(clocks_string);
            assign_clock_array(clocks, opcode_buffer, CYCLES, OP_ARRAY); //associa os clocks no vetor CYCLE
            clock_counter++; //Conta a qtde. de linhas do tipo opcode:clocks
        }
        else{
            break;
        }
    }while(!isnumeric_opcode);

    for(int i=0; i<21; i++){
        printf("%d ", CYCLES[i]);
    }
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

    // Mostra o que está na memória
    line("~", 30);
    printf("      Memória compilada\n");
    for (int i = 0; i < memorylimit; i++) {
        printf("-\t%ld\t\t-\n", ias.memory[i]);
    }

    /* * * * * * * * * * * * * * * * *
    *         HORA DE SIMULAR!       *
     * * * * * * * * * * * * * * * * */

    ias.PC = atoi(argv[4]);
    int statusER = 0, statusEX = 0, statusBO = 0, statusDC = 0, statusBM = 0;
    while (ias.PC < 10 /* há instruções a executar */) {
        statusER = escreverRes(ias, pip);
        statusEX = statusER ? executar(ias, pip) : 0;
        statusBO = statusEX ? buscarOperandos(ias, pip) : 0;
        statusDC = statusBO ? decodificar(ias, pip) : 0;
        
        if (statusDC) {
            if (ias.PC % 2 == 0) {
                statusBM = extrairInstrucaoDaEsquerda(ias.memory[ias.PC]);
            } else {
                statusBM = extrairInstrucaoDaDireita(ias.memory[ias.PC]);
            }
            if (statusBM) {
                buscarNaMemoria(ias, pip);
                pip.DC = pip.BM;
            }
        }

        // Atualize o contador do programa, verifique se há mais instruções para executar
        // e libere os estágios do pipeline conforme necessário.

        line("-=", 20);
        printf("BM -> %lld\n", pip.BM);
        printf("DC -> %lld\n", pip.DC);
        printf("BO -> %lld\n", pip.BO);
        printf("EX -> %lld\n", pip.EX);
        printf("ER -> %lld\n", pip.ER);
        line("~", 20);
        printf("PC -> %lld\n", ias.PC);

        ias.PC++;
    }

    // Finaliza
    fclose(input);
    fclose(output);
    return 0;
}
