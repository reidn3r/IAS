#ifndef UTILS_h
#define UTILS_h

#include <stdio.h>

int FileIsNull(FILE *input);
int removerX(char *str);
void removerNewLine(char *str);
unsigned int binarioParaInteiro(const char *strBinario);
void line (char *symbols, int length);

#endif