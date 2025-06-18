#include "s21_cat.h"
int main(int argc, char* argv[]) {
  opt options = {0};
  int error_flag = 0;
  int linescounter = 1;
  int emptylncnt = 0;
  int prev = '\n';
  Parser(argc, argv, &options, &error_flag);
  int num = optind;
  if (error_flag == 0) {
    while (num < argc) {
      FileOpen(argv, options, num, &linescounter, &emptylncnt, &prev);
      num++;
    }
  } else {
    perror("Error");
  }
  return 0;
}
void FileOpen(char** argv, opt options, int num, int* linescounter,
              int* emptylncnt, int* prev) {
  FILE* f = fopen(argv[num], "r");
  if (f != NULL) {
    int curr = fgetc(f);
    int flagt = 0;
    int flag_v = 0;
    while (curr != EOF) {
      Flags(linescounter, options, emptylncnt, &curr, prev, &flagt, &flag_v);
      if (*emptylncnt < 2 && flagt == 0 && flag_v == 0) {
        putc(curr, stdout);
      }
      *prev = curr;
      curr = fgetc(f);
      flagt = 0;
      flag_v = 0;
    }
  } else {
    perror("Error");
  }
  fclose(f);
}
void Parser(int argc, char** argv, opt* options, int* error_flag) {
  const char* short_options = "+bevnstET";
  const struct option long_options[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {NULL, 0, NULL, 0}};
  int rez = 0;
  int option_index = 0;
  while ((rez = getopt_long(argc, argv, short_options, long_options,
                            &option_index)) != -1) {
    switch (rez) {
      case 'b':
        options->b = 1;
        break;
      case 'e':
        options->e = 1;
        options->v = 1;
        break;
      case 'E':
        options->e = 1;
        break;
      case 'n':
        options->n = 1;
        break;
      case 's':
        options->s = 1;
        break;
      case 't':
        options->t = 1;
        options->v = 1;
        break;
      case 'T':
        options->t = 1;
        break;
      case 'v':
        options->v = 1;
        break;
      default:
        *error_flag = 1;
        break;
    }
  }
}
void Flags(int* linescounter, opt options, int* emptylncnt, int* curr,
           int* prev, int* flagt, int* flag_v) {
  FlagB(options, curr, prev, linescounter);
  FlagE(options, curr);
  FlagN(options, prev, linescounter);
  FlagS(options, curr, prev, emptylncnt);
  FlagT(options, curr, flagt);
  FlagV(options, curr, flag_v);
}
void FlagB(opt options, int* curr, int* prev, int* linescounter) {
  if (options.b) {
    if (*curr != '\n' && *prev == '\n') {
      printf("%6d\t", *linescounter);
      *linescounter += 1;
    }
  }
}
void FlagE(opt options, int* curr) {
  if (options.e) {
    if (*curr == '\n') {
      printf("$");
    }
  }
}
void FlagN(opt options, int* prev, int* linescounter) {
  if (options.n) {
    if (*prev == '\n') {
      printf("%6d\t", *linescounter);
      *linescounter += 1;
    }
  }
}
void FlagS(opt options, int* curr, int* prev, int* emptylncnt) {
  if (options.s) {
    if (*curr == '\n' && *prev == '\n') {
      *emptylncnt += 1;
    } else {
      *emptylncnt = 0;
    }
  }
}
void FlagT(opt options, int* curr, int* flagt) {
  if (options.t) {
    if (*curr == '\t') {
      printf("^I");
      *flagt += 1;
    }
  }
}
void FlagV(opt options, int* curr, int* flag_v) {
  if (options.v) {
    if (*curr < 32 && *curr != '\t' && *curr != '\n') {
      printf("^%c", *curr + 64);
      *flag_v = 1;
    } else if (*curr == 127) {
      printf("^?");
      *flag_v = 1;
    } else if (*curr > 127 && *curr < 160) {
      printf("M-^%c", *curr - 64);
      *flag_v = 1;
    } else if (*curr >= 160) {
      if (*curr - 128 == 127) {
        printf("M-^?");
      } else {
        printf("M-%c", *curr - 128);
      }
      *flag_v = 1;
    } else {
      *flag_v = 0;
    }
  }
}