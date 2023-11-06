#include "grep.h"

void changeEnterInPattern(char *pattern) {
  int len = (int)strlen(pattern);
  if (pattern[len - 1] == '\n' && len != 1) {
    pattern[len - 1] = '\0';
    (len)--;
  }
}

// сюда приходят флаги и готовые регулярки, на выходе статус функции
// в функции выполняется handlerReg, который выполняет регулярки и выводит все
// строки
int execFlag(Flag *fl, Paterns *pat) {
  regex_t regex;
  int ret = SUCCESS;
  int cflag = 0;

  if (fl->i) {
    cflag |= REG_ICASE;
  }
  cflag |= REG_EXTENDED;

  char patternStr[24000] = "";
  for (int i = 0; i < pat->countPatterns; i++) {
    strcat(patternStr, pat->Patterns[i]);
    i != pat->countPatterns - 1 ? strcat(patternStr, "|") : "";
  }
  regcomp(&regex, patternStr, cflag);

  for (int i = 0; i < pat->countFile; i++) {
    handlerReg(pat->File[i], &regex, fl, pat);
  }
  regfree(&regex);
  return ret;
}

void printOpenFileError(char *file, Flag *flag) {
  if (flag->s == false) {
    printf("\bgrep: %s: grep: No such file or directory\n", file);
  }
}
void printOptionError() {
  printf(
      "\bUsage: grep [OPTION]... PATTERNS [FILE]...\n"
      "Try 'grep --help' for more information.\n");
}

void printMatch(char *line, char *file, Flag *flag, size_t countString,
                Paterns *pat, char *matchSubStr) {
  if (flag->c != true) {
    if (flag->h != true && pat->countFile > 1) {
      printf("%s:", file);
    }
    if (flag->n == true) {
      printf("%zu:", countString);
    }
    if (flag->o) {
      printf("%s\n", matchSubStr);
    } else {
      printf("%s", line);
    }
  }
}

int main(int argc, char **argv) {
  int retCode = SUCCESS;
  Paterns pat = {};
  Flag fl = {false, false, false, false, false, false,
             false, false, false, false, "",    ""};

  if ((retCode = parseFlag(argc, argv, &fl)) != SUCCESS) {
    return retCode;
  }
  if ((retCode = parsePatterns(&fl, &pat, argc, argv)) != SUCCESS) {
    return retCode;
  }

  if ((retCode = parseFileName(argc, argv, &pat)) != SUCCESS) {
    return retCode;
  }

  if ((retCode = execFlag(&fl, &pat)) != SUCCESS) {
    return retCode;
  }

  return retCode;
}
