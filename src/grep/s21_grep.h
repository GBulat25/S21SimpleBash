#include <ctype.h>
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct flags {
  int e_counter;
  int flags_flag;
  int multi_file_flag;
  int num;
  int flag_cnt;
  int f_line_counter;
  int f_files;
  char last_char;
} flag;
typedef struct flags_in_file {
  int count;
  int success;
  int flag_l;
  int flag_h;
  int flag_c;
  int cur_flag_cnt;
} in_file;
typedef struct options {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
} opt;
void Parser(int argc, char** argv, opt* options, flag* flags);
void FileOpen(char* file_name, char* search_string, opt options, flag* flags);
void Flags(opt options, flag* flags, in_file* flags_in_file, char* tmp_string,
           char* file_name);
void FileNamePrint(flag flags, in_file flags_in_file, char* file_name);
void OpenWithFlagE(char* search_string, char** argv, int argc, flag* flags);
void OpenWithFlagF(char** argv, char* search_string, int argc, flag* flags);
void delete_control_characters(char* str);
void PrintWithFlagO(regex_t regex, char* tmp_string, flag flags,
                    in_file flags_in_file, char* file_name);
void PrintWithFlagC(flag flags, in_file flags_in_file, char* file_name);
void RegComp(opt options, regex_t* regex, char* search_string);
void PrintLastN(flag flags);
void FlagsFlag(flag* flags);
void FilesCountFlags(int argc, flag* flags);
void FilesSeparetor(in_file* flags_in_file, flag* flags);
void PrintWithFlagV(flag* flags, in_file* flags_in_file, char* file_name,
                    char* tmp_string);
void PrintWithFlagN(flag* flags, char* file_name, char* tmp_string,
                    in_file* flags_in_file);
void PrintLines(flag* flags, char* tmp_string, char* file_name,
                in_file flags_in_file);
void DefinePattern(opt options, char* search_string, int argc, char** argv,
                   flag* flags);
void PrintWithNoFlags(regex_t regex, in_file* flags_in_file, char* tmp_string,
                      flag* flags, char* file_name);
void ChoosePrint(regex_t regex, in_file* flags_in_file, char* tmp_string,
                 flag* flags, char* file_name, opt options);
void StopWork(int argc);