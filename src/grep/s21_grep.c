#include "s21_grep.h"
int main(int argc, char* argv[]) {
  StopWork(argc);
  opt options = {0};
  flag flags = {0};
  flags.f_files = 2;
  flags.last_char = '\n';
  Parser(argc, argv, &options, &flags);
  char search_string[4096];
  FlagsFlag(&flags);
  DefinePattern(options, search_string, argc, argv, &flags);
  while (flags.num < (argc)) {
    FileOpen(argv[flags.num], search_string, options, &flags);
    flags.num += 1;
  }
  PrintLastN(flags);
  return 0;
}
void Parser(int argc, char** argv, opt* options, flag* flags) {
  const char* opts = "e:ivclnhf:so";
  int rez = 0;
  while ((rez = getopt(argc, argv, opts)) != -1) {
    switch (rez) {
      case 'e':
        options->e = 1;
        flags->e_counter += 1;
        break;
      case 'i':
        options->i = 1;
        break;
      case 'v':
        options->v = 1;
        break;
      case 'c':
        options->c = 1;
        break;
      case 'l':
        options->l = 1;
        break;
      case 'n':
        options->n = 1;
        break;
      case 's':
        options->s = 1;
        break;
      case 'f':
        options->f = 1;
        break;
      case 'o':
        options->o = 1;
        break;
      case 'h':
        options->h = 1;
        break;
      default:
        perror("Error");
        break;
    }
  }
}
void FileOpen(char* file_name, char* search_string, opt options, flag* flags) {
  FILE* f = fopen(file_name, "r");
  if (f != NULL) {
    regex_t regex;
    RegComp(options, &regex, search_string);
    ssize_t read_string = 1;
    char* tmp_string = NULL;
    size_t len = 0;
    in_file flags_in_file = {0};
    flags_in_file.count = 1;
    flags_in_file.flag_h = options.h;
    while (read_string != EOF && flags_in_file.flag_l == 0) {
      read_string = getline(&tmp_string, &len, f);
      if (tmp_string && (read_string != EOF)) {
        ChoosePrint(regex, &flags_in_file, tmp_string, flags, file_name,
                    options);
        Flags(options, flags, &flags_in_file, tmp_string, file_name);
      }
    }
    if (options.c) {
      PrintWithFlagC(*flags, flags_in_file, file_name);
    }
    free(tmp_string);
    regfree(&regex);
  } else {
    if (options.s == 0) {
      perror("Error");
    } else {
      PrintLastN(*flags);
      exit(-1);
    }
  }
  fclose(f);
}
void Flags(opt options, flag* flags, in_file* flags_in_file, char* tmp_string,
           char* file_name) {
  if (options.v) {
    PrintWithFlagV(flags, flags_in_file, file_name, tmp_string);
  }
  if (options.n) {
    PrintWithFlagN(flags, file_name, tmp_string, flags_in_file);
  }
  if (options.l) {
    if (flags_in_file->success == 0) {
      printf("%s\n", file_name);
      flags_in_file->flag_l = 1;
    }
  }
  if (options.c) {
    if (flags_in_file->success == 0) {
      flags_in_file->flag_c += 1;
    }
  }
  if (flags_in_file->success == 0) {
    if (options.i) {
      PrintLines(flags, tmp_string, file_name, *flags_in_file);
    }
    if (options.h) {
      PrintLines(flags, tmp_string, file_name, *flags_in_file);
    }
    if (options.e) {
      PrintLines(flags, tmp_string, file_name, *flags_in_file);
    }
    if (options.s) {
      PrintLines(flags, tmp_string, file_name, *flags_in_file);
    }
    if (options.f) {
      PrintLines(flags, tmp_string, file_name, *flags_in_file);
    }
  }
}
void FileNamePrint(flag flags, in_file flags_in_file, char* file_name) {
  if (flags.multi_file_flag && flags_in_file.flag_h == 0) {
    printf("%6s:", file_name);
  }
}
void OpenWithFlagE(char* search_string, char** argv, int argc, flag* flags) {
  int i = 1;
  int e_cat = 4;
  strcpy(search_string, argv[2]);
  for (; i < flags->e_counter; i++) {
    strcat(search_string, "|");
    strcat(search_string, argv[e_cat]);
    e_cat += 2;
  }
  if (argc - optind + 1 > 2) {
    flags->multi_file_flag = 1;
  }
  flags->num = optind;
}
void delete_control_characters(char* str) {
  char* src;
  char* dst;
  for (src = dst = str; *src != '\0'; src++) {
    if (*src != '\n') {
      *dst++ = *src;
    }
  }
  *dst = '\0';
}
void OpenWithFlagF(char** argv, char* search_string, int argc, flag* flags) {
  FILE* f = fopen(argv[flags->f_files], "r");
  if (f != NULL) {
    ssize_t patten_line = 0;
    size_t len = 0;
    char* line = NULL;
    while ((patten_line = getline(&line, &len, f)) != EOF) {
      delete_control_characters(line);
      if (flags->f_line_counter == 0) {
        strcpy(search_string, line);
      } else {
        strcat(search_string, "\\|");
        strcat(search_string, line);
      }
      flags->f_line_counter += 1;
    }
    free(line);
  } else {
    perror("Error");
  }
  if (argc - optind + 1 > 2) {
    flags->multi_file_flag = 1;
  }
  flags->num = optind;
  fclose(f);
}
void PrintWithFlagO(regex_t regex, char* tmp_string, flag flags,
                    in_file flags_in_file, char* file_name) {
  regmatch_t pmatch[1];
  char* tmp_line = tmp_string;
  while (regexec(&regex, tmp_line, 1, pmatch, 0) == 0) {
    FileNamePrint(flags, flags_in_file, file_name);
    printf("%.*s\n", (int)(pmatch[0].rm_eo - pmatch[0].rm_so),
           &tmp_line[pmatch[0].rm_so]);
    tmp_line += pmatch[0].rm_eo;
  }
}
void PrintWithFlagC(flag flags, in_file flags_in_file, char* file_name) {
  FileNamePrint(flags, flags_in_file, file_name);
  printf("%d\n", flags_in_file.flag_c);
}
void RegComp(opt options, regex_t* regex, char* search_string) {
  if (options.i) {
    regcomp(regex, search_string, REG_ICASE);
  } else if (options.e) {
    regcomp(regex, search_string, REG_EXTENDED);
  } else {
    regcomp(regex, search_string, 0);
  }
}
void PrintLastN(flag flags) {
  if (flags.last_char != '\n') {
    printf("\n");
  }
}
void FlagsFlag(flag* flags) {
  if (optind != 1) {
    flags->flags_flag = 2;
  }
}
void FilesCountFlags(int argc, flag* flags) {
  if (argc - optind > 2) {
    flags->multi_file_flag = 1;
  }
  flags->num = optind + 1;
}
void FilesSeparetor(in_file* flags_in_file, flag* flags) {
  if (flags_in_file->cur_flag_cnt == 0 && flags->flag_cnt != 0 &&
      flags->last_char != '\n') {
    printf("\n");
  }
  flags_in_file->cur_flag_cnt += 1;
  flags->flag_cnt += 1;
}
void PrintWithFlagV(flag* flags, in_file* flags_in_file, char* file_name,
                    char* tmp_string) {
  int compare = REG_NOMATCH;
  if (flags_in_file->success == compare && flags_in_file->success != 0) {
    FilesSeparetor(flags_in_file, flags);
    FileNamePrint(*flags, *flags_in_file, file_name);
    printf("%s", tmp_string);
    flags->last_char = tmp_string[strlen(tmp_string) - 1];
  }
}
void PrintWithFlagN(flag* flags, char* file_name, char* tmp_string,
                    in_file* flags_in_file) {
  if (flags_in_file->success == 0) {
    flags->flag_cnt -= 1;
    FileNamePrint(*flags, *flags_in_file, file_name);
    printf("%d:", flags_in_file->count);
    printf("%s", tmp_string);
    flags->last_char = tmp_string[strlen(tmp_string) - 1];
  }
  flags_in_file->count += 1;
}
void PrintLines(flag* flags, char* tmp_string, char* file_name,
                in_file flags_in_file) {
  FilesSeparetor(&flags_in_file, flags);
  FileNamePrint(*flags, flags_in_file, file_name);
  printf("%s", tmp_string);
  flags->last_char = tmp_string[strlen(tmp_string) - 1];
}
void DefinePattern(opt options, char* search_string, int argc, char** argv,
                   flag* flags) {
  if (options.e) {
    OpenWithFlagE(search_string, argv, argc, flags);
  } else if (options.f) {
    while (flags->f_files < optind) {
      OpenWithFlagF(argv, search_string, argc, flags);
      flags->f_files += 2;
    }
  } else {
    strcpy(search_string, argv[optind]);
    FilesCountFlags(argc, flags);
  }
}
void PrintWithNoFlags(regex_t regex, in_file* flags_in_file, char* tmp_string,
                      flag* flags, char* file_name) {
  flags_in_file->success = regexec(&regex, tmp_string, 0, NULL, 0);
  if (flags_in_file->success == 0 && flags->flags_flag == 0) {
    PrintLines(flags, tmp_string, file_name, *flags_in_file);
  }
}
void ChoosePrint(regex_t regex, in_file* flags_in_file, char* tmp_string,
                 flag* flags, char* file_name, opt options) {
  if (options.o) {
    PrintWithFlagO(regex, tmp_string, *flags, *flags_in_file, file_name);
  } else {
    PrintWithNoFlags(regex, flags_in_file, tmp_string, flags, file_name);
  }
}
void StopWork(int argc) {
  if (argc < 3) {
    fprintf(stderr, "Not enough arguments\n");
    exit(1);
  }
}