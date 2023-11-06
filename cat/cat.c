#include "cat.h"

int main(int argc, char **argv) {
  char **fileName = NULL;
  int retCode = SUCCESS;
  Flag fl = {false, false, false, false, false, false};

  if ((retCode = parseFlag(argc, argv, &fl)) != SUCCESS) {
    return retCode;
  }
  if ((retCode = parseFileName(argc, argv, &fileName)) != SUCCESS) {
    return retCode;
  }
  handlerFileOutput(argc - optind, fileName, fl);
  free(fileName);
  return retCode;
}
