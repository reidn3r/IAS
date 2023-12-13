#include <stdio.h>
#include <string.h>
#include "utils.h"
#define MAX_TEXT_LENGTH 40

int main(){
    
    FILE *input = fopen("input.txt", "r");
    FILE *output = fopen("output.txt", "w+b");
    char buffer[MAX_TEXT_LENGTH];
    char *address, *instruction;

    if(FileIsNull(input)) printf("Arquivo de instrucoes nao foi aberto");

    while(fgets(buffer, MAX_TEXT_LENGTH, input)){
        /* 
            - identifica a instrução/opcode
            - 8 bits opcode
            - 12 bits restante
            - Escreve em um .dat 2 instruções lado a lado em binário
                - Posteriormente converte os 40 bits para decimal
        */

        // printf("buffer: %s", buffer);
        instruction = strtok(buffer, " ");
        address = strtok(NULL, "");
        
        // printf("instrucao: %s\n", instruction);
        // printf("endereco: %s\n", address);

    }
    return 0;
}