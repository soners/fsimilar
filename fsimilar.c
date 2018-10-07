#include "config.h"
#include "similarity.h"
#include "strlib.h"
#include <ctype.h>
#include <dirent.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define FILE_FILE 0
#define FILE_PATH 1
#define PATH_PATH 2

void printInfo() {
  printf("Usage: fsimilar -s {source} -d {destination} -t {type} -m "
         "{similarity} {-q}\n\n");
  printf("{source} is the base path for file/directory\n");
  printf("{destination} is the path for comparing\n");
  printf("{similarity} is the minimum similarity to print similar files\n");
  printf("{type} is for to compare what to what? {ff, fp, pp}\n");
  printf("{-q} enables quick search (available for file_path)\n");
  exit(1);
}

int main(int argc, char *argv[]) {
  configuration *config = malloc(sizeof(configuration));

  int c;

  int sim = 80;
  int quick = 0;
  int type = FILE_PATH;
  char *source;
  char *dest;
  char *temp;

  if (argc < 2) {
    printInfo();
  }

  while ((c = getopt(argc, argv, "qm:s:d:t:")) != -1) {
    switch (c) {
    case 'q':
      quick = 1;
      break;
    case 'm':
      sim = atoi(optarg);
      sim = sim >= 20 && sim <= 100 ? sim : 80;
      break;
    case 's':
      source = optarg;
      break;
    case 'd':
      dest = optarg;
      break;
    case '?':
      if (optopt == 'c') {
        fprintf(stderr, "Option -%c requires an argument.\n", optopt);
        return 1;
      } else if (isprint(optopt)) {
        fprintf(stderr, "Unknown option `-%c'.\n", optopt);
        return 1;
      } else {
        fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
        return 1;
      }
      break;
    case 't':
      temp = optarg;
      if (!strcmp(temp, "ff")) {
        type = FILE_FILE;
      } else if (!strcmp(temp, "fp")) {
        type = FILE_PATH;
      } else if (!strcmp(temp, "pp")) {
        type = PATH_PATH;
      } else {
        fprintf(stderr, "Possible options: {ff, fp, pp}\n");
        return 1;
      }
      break;
    }
  }

  config->min_similarity = sim;
  config->quicksearch = quick;
  config->type = type;
  config->source = source;
  config->destination = dest;
  execute(config);
}
