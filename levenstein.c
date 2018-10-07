#include "strlib.h"
#include <stdio.h>
#include <stdlib.h>

int costOfSubstitution(char f, char s) { return f == s ? 0 : 1; }
int min(int f, int s, int t) {
  if (f <= s && f <= t) {
    return f;
  } else if (s <= f && s <= t) {
    return s;
  } else if (t <= f && t <= s) {
    return t;
  }
}

int max(int f, int s) { return f >= s ? f : s; }

int similarity(string *s0, string *s1) {
  return 100 -
         100 * ((double)levenstein_distance(s0, s1) /
                max(str_len(s0), str_len(s1)));
}

int levenstein_distance(string *s0, string *s1) {
  int len0 = str_len(s0);
  int len1 = str_len(s1);

  int **opt = (int **)malloc(sizeof(int *) * (len0 + 1));

  for (int i = 0; i <= len0; i++) {
    opt[i] = malloc(sizeof(int) * (len1 + 1));
  }

  for (int i = 0; i <= len0; i++) {
    for (int j = 0; j <= len1; j++) {
      if (i == 0) {
        opt[i][j] = j;
      } else if (j == 0) {
        opt[i][j] = i;
      } else {
        opt[i][j] =
            min(opt[i - 1][j - 1] +
                    costOfSubstitution(char_at(s0, i - 1), char_at(s1, j - 1)),
                opt[i - 1][j] + 1, opt[i][j - 1] + 1);
      }
    }
  }
  int result = opt[len0][len1];
  for (int i = 0; i <= len0; i++) {
    free(opt[i]);
    opt[i] = NULL;
  }
  free(opt);
  opt = NULL;
  return result;
}
