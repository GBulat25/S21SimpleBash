
#define S21_CAT_H
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
typedef struct options {
  int b;
  int e;
  int v;
  int n;
  int s;
  int t;
} opt;
void Parser(int argc, char** argv, opt* options, int* error_flag);
void FileOpen(char** argv, opt options, int num, int* linescounter,
              int* emptylncnt, int* prev);
void Flags(int* linescounter, opt options, int* emptylncnt, int* curr,
           int* prev, int* flagT, int* flag_v);
void FlagB(opt options, int* curr, int* prev, int* linescounter);
void FlagE(opt optoins, int* curr);
void FlagN(opt options, int* prev, int* linescounter);
void FlagS(opt options, int* curr, int* prev, int* emptylncnt);
void FlagT(opt options, int* curr, int* flagt);
void FlagV(opt options, int* curr, int* flag_v);
