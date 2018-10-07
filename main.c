#include "levenstein.h"
#include "similarity.h"
#include "strlib.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {

  string *file = readFile(argv[1]);

  printf("%d\n", file_file(file, argv[2]));
}
