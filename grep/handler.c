#include "grep.h"
int parseFlag(int argc, char **argv, Flag *flag) {
  int option_index;
  int c;
  while ((c = getopt_long(argc, argv, "e:ivclnhsf:o", 0, &option_index)) !=
         -1) {
    switch (c) {
      case 'e':
        flag->e = true;
        flag->e_pattern = optarg;
        break;
      case 'i':
        flag->i = true;
        break;
      case 'v':
        flag->v = true;
        break;
      case 'c':
        flag->c = true;
        break;
      case 'l':
        flag->l = true;
        break;
      case 'n':
        flag->n = true;
        break;
      case 'h':
        flag->h = true;
        break;
      case 's':
        flag->s = true;
        break;
      case 'o':
        flag->o = true;
        break;
      case 'f':
        flag->f = true;
        flag->f_pattern = optarg;
        break;
      case '?':
        printOptionError();
        return READ_FLAG_ERROR;
      default:
        return UNKNOWN_ERROR;
    }
  }
  return SUCCESS;
}

int parseFileName(int argc, char **argv, Paterns *patterns) {
  int ret = SUCCESS;
  for (int i = optind; i < argc; i++) {
    patterns->File[patterns->countFile] = argv[i];
    patterns->countFile++;
  }
  if (patterns->countFile == 0) {
    printOptionError();
    ret = PARSE_ERROR;
  }
  return ret;
}

int parsePatterns(Flag *fl, Paterns *pat, int argc, char **argv) {
  if (fl->f) {
    FILE *fs = fopen(fl->f_pattern, "r");
    if (NULL == fs) {
      printOpenFileError(fl->f_pattern, fl);
      return OPEN_FILE_ERROR;
    } else {
      char *line = NULL;
      size_t len = 0;
      ssize_t read;
      while ((read = getline(&line, &len, fs)) != -1) {
        changeEnterInPattern(line);
        strcpy(pat->Patterns[pat->countPatterns], line);
        pat->countPatterns++;
      }
      free(line);
    }
  } else if (fl->e) {
    strcpy(pat->Patterns[0], fl->e_pattern);
    pat->countPatterns = 1;
  } else {
    if (argc == 1) {
      printOptionError();
      return PARSE_ERROR;
    }
    strcpy(pat->Patterns[0], argv[optind]);
    pat->countPatterns = 1;
    optind++;
  }
  return SUCCESS;
}
int handlerReg(char *file, regex_t *regex, Flag *flag, Paterns *pat) {
  FILE *fl = fopen(file, "r");
  if (NULL == fl) {
    printOpenFileError(file, flag);
    return READ_FLAG_ERROR;
  }
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  size_t countMatch = 0;
  size_t countString = 0;
  regmatch_t match;

  while ((read = getline(&line, &len, fl)) != -1) {
    countString++;
    if (!regexec(regex, line, 1, &match, 0)) {
      int start = match.rm_so;
      int end = match.rm_eo;
      int length = end - start;
      char *substr = calloc(length + 1, sizeof(char));
      if (NULL == substr) {
        return MALLOC_ERROR;
      }
      strncpy(substr, line + start, length);
      substr[length] = '\0';

      if (!(flag->v)) {
        countMatch++;
        if (flag->l) {
          printf("%s\n", file);
          return SUCCESS;
        }
        printMatch(line, file, flag, countString, pat, substr);
      }
      free(substr);
    } else if (flag->v) {
      countMatch++;
      if (flag->l) {
        printf("%s\n", file);
        return SUCCESS;
      }
      printMatch(line, file, flag, countString, pat, "");
    }
  }
  if (flag->c && !(flag->l)) {
    if (flag->h != true && pat->countFile > 1) {
      printf("%s:", file);
    }
    printf("%zu\n", countMatch);
  }
  free(line);

  return SUCCESS;
}