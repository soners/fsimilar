#include <stdio.h>
#include <stdlib.h>

typedef struct string {
  char *data;
  int length;
} string;

int len(char *data) {
  int counter = 0;
  for (counter = 0; data[counter] != NULL; counter++)
    ;
  return counter;
}

char *copystr(char *data) {
  char *temp = malloc(sizeof(char) * (len(data) + 1));
  int i = 0;
  for (i = 0; data[i] != NULL; i++) {
    temp[i] = data[i];
  }
  temp[i] = '\0';
  return temp;
}

string *String(char *data) {
  string *new = malloc(sizeof(string));
  new->length = len(data);

  char *temp = copystr(data);
  new->data = temp;
  return new;
}

void concat(string *str, string *conc) {
  if (!conc || !str) {
    return;
  }
  int len0 = str_len(str);
  int len1 = str_len(conc);
  char *temp = malloc(sizeof(char) * (len0 + len1 + 1));
  for (int i = 0; i < len0; i++) {
    temp[i] = char_at(str, i);
  }
  for (int i = 0; i < len1; i++) {
    temp[i + len0] = char_at(conc, i);
  }
  temp[len0 + len1] = '\0';
  str->data = temp;
  str->length = len0 + len1;
}

char *tocharptr(string *str) { return copystr(str->data); }

int str_len(string *str) { return str->length; }

int char_at(string *str, int i) { return str->data[i]; }

void printString(string *str) { printf("%s\n", str->data); }

void freeString(string *str) {
  if (!str) {
    return;
  }
  free(str->data);
  str->data = NULL;
  free(str);
  str = NULL;
}
