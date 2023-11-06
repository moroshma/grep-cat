#ifndef C3_SIMPLEBASHUTILS_1_GREP_H
#define C3_SIMPLEBASHUTILS_1_GREP_H
#include <ctype.h>
#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
enum ERROR {
  MALLOC_ERROR = 1,
  READFILE_ERROR = 2,
  PARSE_ERROR = 3,
  READ_FLAG_ERROR = 4,
  UNKNOWN_ERROR = 5,
  OPEN_FILE_ERROR = 6,
  SUCCESS = 0
};
typedef struct Flag {
  bool e;
  bool i;
  bool v;
  bool c;
  bool l;
  bool n;
  bool h;
  bool s;
  bool f;
  bool o;
  char *e_pattern;
  char *f_pattern;
} Flag;

typedef struct Paterns {
  int countPatterns;
  int countFile;
  char Patterns[255][512];
  char *File[128];
} Paterns;

int handlerReg(char *file, regex_t *regex, Flag *flag, Paterns *pat);
int execFlag(Flag *fl, Paterns *pat);
int parseFileName(int argc, char **argv, Paterns *patterns);
int parseFlag(int argc, char **argv, Flag *flag);
int parsePatterns(Flag *fl, Paterns *pat, int argc, char **argv);
void changeEnterInPattern(char *pattern);
void printOpenFileError(char *file, Flag *flag);
void printMatch(char *line, char *file, Flag *flag, size_t countString,
                Paterns *pat, char *matchSubStr);
void printOptionError();

#endif  // C3_SIMPLEBASHUTILS_1_GREP_H
