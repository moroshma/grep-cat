#ifndef C3_SIMPLEBASHUTILS_1_S21_CAT_H
#define C3_SIMPLEBASHUTILS_1_S21_CAT_H
#include <ctype.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "string.h"

enum ERROR {
  MALLOC_ERROR = 1,
  READFILE_ERROR = 2,
  PARSE_ERROR = 3,
  READ_FLAG_ERROR = 4,
  UNKNOWN_ERROR = 5,
  SUCCESS = 0
};
typedef struct Flag {
  bool b;
  bool e;
  bool n;
  bool s;
  bool t;
  bool v;
} Flag;
int parseFileName(int argc, char **argv, char ***file);
int parseFlag(int argc, char **argv, Flag *flag);
void cat(FILE *file, Flag flag);
void handlerFileOutput(int size, char **file, Flag flag);
void v_flag_handle(int *ch, bool *print);
void t_flag_handle();
#endif