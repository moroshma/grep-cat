#include "cat.h"

void v_flag_handle(int *ch, bool *print) {
  if (!isascii(*ch) && !isprint(*ch)) {
    putchar('M');
    putchar('-');
    *ch = toascii(*ch);
  }
  if (iscntrl(*ch)) {
    putchar('^');
    putchar(*ch == 127 ? '?' : *ch + 64);
    *print = true;
  }
}

void t_flag_handle() {
  putchar('^');
  putchar('I');
}

int parseFlag(int argc, char **argv, Flag *flag) {
  static struct option long_options[] = {{"number-nonblank", 0, 0, 'b'},
                                         {"number", 0, 0, 'n'},
                                         {"squeeze-blank", 0, 0, 's'},

                                         {0, 0, 0, 0}};
  int option_index;
  int c;

  while ((c = getopt_long(argc, argv, "beEnstvT", long_options,
                          &option_index)) != -1) {
    switch (c) {
      case 'b':
        flag->b = true;
        flag->n = true;
        break;
      case 'e':
        flag->v = true;
        flag->e = true;
        break;
      case 'E':
        flag->e = true;
        break;
      case 'n':
        flag->n = true;
        break;
      case 's':
        flag->s = true;
        break;
      case 't':
        flag->v = true;
        flag->t = true;
        break;
      case 'T':
        flag->t = true;
        break;
      case 'v':
        flag->v = true;
        break;

      case '?':
        return READ_FLAG_ERROR;
      default:
        return UNKNOWN_ERROR;
    }
  }
  return SUCCESS;
}

int parseFileName(int argc, char **argv, char ***file) {
  *file = malloc(sizeof(char *) * (argc - optind));
  if (NULL == *file) {
    return MALLOC_ERROR;
  }
  int FileIndex = 0;
  for (int i = optind; i < argc; i++) {
    (*file)[FileIndex] = argv[i];
    FileIndex++;
  }
  return SUCCESS;
}
bool checkEchoMode(char *file) {
  bool ret = false;
  if (!strcmp(file, "-")) {
    ret = true;
  }
  return ret;
}
void handlerFileOutput(int size, char **file, Flag flag) {
  for (int i = 0; i < size; i++) {
    if (checkEchoMode(file[i])) {
      if (feof(stdin)) clearerr(stdin);
      cat(stdin, flag);
      continue;
    }
    FILE *fs = fopen(file[i], "r");
    if (NULL == fs) {
      printf("cat: %s : No such file or directory\n", file[i]);
    } else {
      cat(fs, flag);
      fclose(fs);
    }
  }
}
void cat(FILE *file, Flag flag) {
  int ch;
  int numberStr = 0;
  bool gobble = false;
  bool print = false;
  for (int previous = '\n'; (ch = fgetc(file)) != EOF; previous = ch) {
    print = false;
    if (previous == '\n') {
      if (flag.s) {
        if (ch == '\n') {
          if (gobble) continue;
          gobble = true;
        } else {
          gobble = false;
        }
      }
      if (flag.n && (!flag.b || '\n' != ch)) {
        printf("%6d\t", ++numberStr);
      }
    }
    if (ch == '\n') {
      if (flag.e) putchar('$');
    } else if (ch == '\t') {
      if (flag.t) {
        print = true;
        t_flag_handle();
      }
    } else if (flag.v) {
      v_flag_handle(&ch, &print);
    }
    if (!print) putchar(ch);
  }  // s -> n -> b -> e -> t
}
