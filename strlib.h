typedef struct string {
} string;

string *String(char *);
int str_len(string *);
int char_at(string *, int);
void concat(string *, string *);
char *tocharptr(string *);
void printString(string *);
void freeString(string *);
