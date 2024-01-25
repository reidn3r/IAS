#ifndef UTILS_h
#define UTILS_h

#include <stdio.h>

int FileIsNull(FILE *input);
int removerX(char *str);
void removerNewLine(char *str);
unsigned int strToBin(const char *strBinario);
void printBinary(long long n);
void line (char *symbols, int length);

#endif