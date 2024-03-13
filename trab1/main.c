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

char *OP_ARRAY[] = {"LOAD MQ", "LOAD MQ,M()", "STOR M()", "LOAD M()", "LOAD -M()", "LOAD |M()|", "LOAD -|M()|", "JUMP M(,0:19)", "JUMP M(,20:39)", "JUMP+ M(,0:19)","JUMP+ M(,20:39)", "ADD M()", "ADD |M()|", "SUB M()", "SUB |M()|", "MUL M()", "DIV M()", "LSH", "RSH", "STOR M(,8:19)", "STOR M(,28:39)", "EXIT"};

char *BINARY[] = {"00001010", "00001001", "00100001", "00000001", "00000010", "00000011", "00000100", "00001101", "00001110", "00001111", "00010000", "00000101", "00000111", "00000110", "00001000", "00001011", "00001100", "00010100", "00010101", "00010010", "00010011", "10000000"};


int CYCLES[] = {1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1, 1}; // Será alterado depois

int main (int argc, char *argv[]){
    FILE *input;
    FILE *midput;
    FILE *output;
    IAS ias;
    PIPELINE pip;
    
    // Verifica se o programa foi chamado corretamente
    if (argc == 5 && strcmp(argv[1], "-p") == 0 && strcmp(argv[3], "-i") == 0) {
        input = fopen(argv[2], "r");
        midput = fopen("memo.ias", "w");
        output = fopen(strcat(argv[2],".out"), "w");
        ias.PC = atoi(argv[4]); // PC onde começam as instruções = argumento da linha de comando
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
    printf("-> Lendo quantos ciclos cada instrucao leva...\n");
    char clock_opcode[MAX_TEXT_LENGTH], *opcode_buffer;
    int clock_opcode_counter, clocks, clock_counter= 0; 0; 0;
    char *clocks_string;
    int isnumeric_opcode = 0;
    do{
        fgets(clock_opcode, MAX_TEXT_LENGTH, input);
        opcode_buffer = strtok(clock_opcode, ": "); //Pega o opcode, parte textual de opcode: ciclos
        printf("clock_opcode: %s\topcode_buffer: %s\n", clock_opcode, opcode_buffer);
        removerNewLine(opcode_buffer);

        isnumeric_opcode = isNumeric(opcode_buffer);
        clocks_string = strtok(NULL, " "); //Pega o numero que representa o clock
        printf("clocks_string: %s\n", clocks_string);
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
    } printf("\n");
    /*
    * Armazena dados em memória
    */

    int data;
    char buffer[MAX_TEXT_LENGTH];

    int memorylimit = 0;
    // fgets (buffer, MAX_TEXT_LENGTH, input);
    strcpy(buffer, clock_opcode);

    while (isdigit(*buffer)) { // Lê até encontrar algo que não é número
        ias.memory[memorylimit] = strtoll(buffer, NULL, 10);
        memorylimit++;
        fgets (buffer, MAX_TEXT_LENGTH, input);
    }
    ias.instructionsStart = memorylimit;
    
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
    int64_t words[256];
    for (int i = 0; i < lineCounter; i++) {
        printf("Instrucao %d:\t%d\n", i+1, instructionsInt[i]);
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
        printf("Palavra %d: " PRId64 "\n", i+1, words[i]);
        printBinary(words[i]);
    }

    int size = sizeof(words)/sizeof(long long);
    printf("SIZE: %d\n", size);
    // write_output(ias, output, words, size);

    // Mostra o que está na memória
    line("~", 30);
    printf("      Memória compilada\n");
    for (int i = 0; i < memorylimit; i++) {
        printf("-\t%" PRId64 "\t-\n", ias.memory[i]);
    }

    write_output(&ias, midput);
    /* * * * * * * * * * * * * * * * *
    *         HORA DE SIMULAR!       *
     * * * * * * * * * * * * * * * * */

    pip.BM = 0, pip.BO = 0, pip.DC = 0, pip.ER = 0, pip.EX = 0;
    int statusER = 0, statusEX = 0, statusBO = 0, statusDC = 0, statusBM = 0; // 1, se esse passo do pipeline conseguiu executar
    int totalCycles = 1, exCycles = 1;
    int pipelineCleared = 0, firstTime = 1;
    while (!pipelineCleared) { // Se o pip está limpo, finaliza

        escreverRes(&ias, &pip);
        statusEX = executar(&ias, &pip, &exCycles);
        if (statusEX) {
            // Busca quantos ciclos a instrução que está no BO leva, e armazena em exCycles
            int64_t bo_instrucao = pip.BO;
            char *bo_string_binario = intParaStringBinario(bo_instrucao);
            char *opcode_binario = buscaOpcodeDoBinario(bo_string_binario);
            int index_opcode = buscaOpcodeIndex(opcode_binario, BINARY);
            // retorna o indice do opcode no array de binários definido no inicio do arquivo

            exCycles = 0;
            if(index_opcode != -1){
                exCycles = CYCLES[index_opcode];
            }
            printf("ciclos da instrucao: %d\n", exCycles);
        }

        buscarOperandos(&ias, &pip);
        decodificar(&ias, &pip);
    
        statusBM = buscarNaMemoria(&ias, &pip);
        if (ias.memory[ias.PC] == 0b10000000) { // Acabaram as intruções
            pip.BM = 0;
        } else if (pip.BM == 0) { // Não acabou e temos mais instruções
            ias.IR = extrairInstrucao (&ias);
            pip.BM = ias.IR;
            ias.PC++;
        }

        line("-=", 20);
        printf("cycle: %d\n", totalCycles);
        line("~", 15);
        printf("BM -> %" PRId64 "\n", pip.BM);
        printf("DC -> %" PRId64 "\n", pip.DC);
        printf("BO -> %" PRId64 "\n", pip.BO);
        printf("EX -> %" PRId64 "\n", pip.EX);
        printf("ER -> %" PRId64 "\n", pip.ER);
        line("~", 15);
        printf("AC -> %" PRId64 "\n", ias.AC);
        printf("PC -> %" PRId64 "\n", ias.PC);

        pipelineCleared = isPipelineCleared(pip);
        if (firstTime) firstTime = 0;
        totalCycles++;
    }

    write_output(&ias, output);


    for(int i=0; i<15; i++){
        printf("memo idx: %d: " PRId64 "\n", i, ias.memory[i]);
    }

    // Finaliza
    fclose(input);
    fclose(output);
    return 0;
}
