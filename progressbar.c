#include <stdio.h>

void request_progress_bar(int current, int total, char *message) {

  printf("\r");
  for (size_t progress = 1; progress <= 100; progress++) {
    printf(" ");
  }

  printf("\r");
  printf("%s: [", message);
  size_t count = ((double)current / total) * 50;
  size_t max = 50;
  for (size_t progress = 1; progress <= max; ++progress) {
    if (progress <= count) {
      printf("#");
    } else {
      printf(" ");
    }
  }
  printf("]");
  printf("  [%d/%d]", current, total);
  fflush(stdout);
}
