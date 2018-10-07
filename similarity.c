#include "config.h"
#include "dll.h"
#include "levenstein.h"
#include "progressbar.h"
#include "strlib.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define THRESHOLD 4096 * 4

#define FILE_FILE 0
#define FILE_PATH 1
#define PATH_PATH 2

string *file_path(char *str, char *path, configuration *config);
string *quick_pp(char *path0, char *path1, configuration *config);
string *path_path(char *path0, char *path1, configuration *config);
string *quick_fp(char *str, char *path1, configuration *config);
string *readFile(char *path);

void execute(configuration *config) {
  int type = config->type;
  int quicksearch = config->quicksearch;

  char *source = config->source;
  char *dest = config->destination;

  string *result;

  if (type == FILE_FILE) {
    int result = file_file(readFile(source), dest);
    printf("%s is %s%d similar with %s\n", source, "%", result, dest);
  } else if (type == FILE_PATH) {
    if (quicksearch) {
      result = quick_fp(source, dest, config);
      printString(result);
    } else {
      result = file_path(source, dest, config);
      printString(result);
    }
  } else {
    result = path_path(source, dest, config);
    printString(result);
  }
}

void iterateFiles(char *path, DLL *dll) {
  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir(path)) != NULL) {
    while ((ent = readdir(dir)) != NULL) {
      if ((strcmp(ent->d_name, "..") != 0) && (strcmp(ent->d_name, ".") != 0)) {

        string *fullpath = String(path);
        string *slash = String("/");
        string *d_name = String(ent->d_name);
        concat(fullpath, slash);
        concat(fullpath, d_name);
        if (ent->d_type == DT_DIR) {
          iterateFiles(tocharptr(fullpath), dll);
        } else {

          char *cur_file = tocharptr(fullpath);
          struct stat st;
          stat(cur_file, &st);
          size_t size = st.st_size;

          if (size < THRESHOLD) {
            addLast(dll, fullpath);
          }
        }
        // free space management
        freeString(d_name);
        freeString(slash);
      }
    }
    closedir(dir);
  }
}

DLL *allFiles(char *path) {
  DLL *dll = initDLL();
  iterateFiles(path, dll);
  return dll;
}

string *readFile(char *path) {
  FILE *fp;

  string *result = String("");
  if ((fp = fopen(path, "r")) == NULL) {
  } else {
    char buff[255];

    while (fgets(buff, 255, fp) != NULL) {
      char *word = &buff[0];
      string *str2 = String(word);
      concat(result, str2);
    }
    fclose(fp);
  }

  return result;
}

int file_file(string *first, char *second) {
  string *result = readFile(second);

  if (!str_len(result)) {
    return 0;
  }
  if (!first || !result) {
    return 0;
  }
  int sim = similarity(first, result);

  freeString(result);
  return sim;
}

string *file_path(char *str, char *path, configuration *config) {
  if (!(config->quicksearch)) {
    printf("Reading files...\n");
  }

  string *buffer = String("");

  DLL *files = allFiles(path);
  Iterator *iter = iterator(files);
  string *cont = readFile(str);
  int min_similarity = config->min_similarity;
  int counter = 0;

  if (!str_len(cont)) {
    return String("Error while opening file\n");
  }

  while (hasNext(iter)) {
    counter++;
    request_progress_bar(counter, sizeOfList(files), "Processing files...");

    string *file = next(iter);
    char *cur_file = tocharptr(file);
    struct stat st;
    stat(cur_file, &st);
    size_t size = st.st_size;
    // A simple and effective optimization
    if (size < str_len(cont) * ((double)min_similarity / 100) ||
        size > str_len(cont) * ((double)100 / min_similarity)) {
      continue;
    }

    int similarity = file_file(cont, cur_file);
    if (similarity > min_similarity) {

      // first possible similar file for quicksearch
      if (config->quicksearch) {
        return file;
      }
      if (!str_len(buffer)) {
        concat(buffer,
               String("\nThis file may be similar with following files: "));
        concat(buffer, String(str));
        concat(buffer, String("\n"));
      }
      char *temp = malloc(sizeof(char) * (strlen(cur_file) + 7));
      sprintf(temp, "%s : %d\n", cur_file, similarity);
      concat(buffer, String(temp));
    }
  }

  return buffer;
}

string *path_path(char *path0, char *path1, configuration *config) {

  printf("Reading files...\n");

  string *buffer = String("");

  DLL *f_files = allFiles(path0);
  DLL *s_files = allFiles(path1);

  Iterator *f_iter = iterator(f_files);

  int min_similarity = config->min_similarity;

  int counter = 0;

  while (hasNext(f_iter)) {

    string *f_file = next(f_iter);
    string *f_cont = readFile(tocharptr(f_file));
    string *temp_buffer = String("");

    Iterator *s_iter = iterator(s_files);

    while (hasNext(s_iter)) {
      counter++;

      request_progress_bar(counter, sizeOfList(f_files) * sizeOfList(s_files),
                           "Processing files...");

      string *s_file = next(s_iter);

      char *comp_file = tocharptr(s_file);

      struct stat st;
      stat(comp_file, &st);
      size_t size = st.st_size;

      // A simple and effective optimization
      if ((size < str_len(f_cont) * ((double)min_similarity / 100) &&
           min_similarity != 100 && min_similarity != 0) ||
          (size > str_len(f_cont) * ((double)100 / min_similarity) &&
           min_similarity != 100 && min_similarity != 0)) {
        continue;
      }

      int similarity = file_file(f_cont, comp_file);
      if (similarity > min_similarity) {
        if (temp_buffer && !str_len(temp_buffer)) {
          string *aux =
              String("\nThis file may be similar with following files:");
          concat(temp_buffer, aux);
          concat(temp_buffer, String(tocharptr(f_file)));
          concat(temp_buffer, String("\n"));
          freeString(aux);
        }
        char *temp = malloc(sizeof(char) * (str_len(comp_file) + 7));
        sprintf(temp, "%s : %d\n", comp_file, similarity);
        string *writ = String(temp);
        concat(temp_buffer, String(tocharptr(writ)));
        freeString(writ);
      }
    }
    concat(buffer, temp_buffer);
    freeString(temp_buffer);
    freeString(f_file);
    freeString(f_cont);
  }
  printf("\n");
  return buffer;
}

string *quick_fp(char *str, char *path, configuration *config) {

  string *result = String("");
  string *buffer = String("\nPossible similar files to file:");
  concat(buffer, String(str));
  concat(buffer, String("\n"));

  string *cont = readFile(str);

  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir(path)) != NULL) {
    while ((ent = readdir(dir)) != NULL) {
      if ((strcmp(ent->d_name, "..") != 0) && (strcmp(ent->d_name, ".") != 0)) {

        string *fullpath = String(path);
        string *slash = String("/");
        string *d_name = String(ent->d_name);
        concat(fullpath, slash);
        concat(fullpath, d_name);

        char *fullp = tocharptr(fullpath);

        if (ent->d_type == DT_DIR) {
          result = file_path(str, fullp, config);
          if (result && str_len(result)) {
            concat(buffer, result);
            concat(buffer, String("\n"));
          }
        } else {

          struct stat st;
          stat(fullp, &st);
          size_t size = st.st_size;

          if (size > THRESHOLD) {
            continue;
          }

          int similarity = file_file(cont, fullp);
          if (similarity > config->min_similarity) {
            concat(buffer, fullpath);
            concat(buffer, "\n");
          }
        }
      }
    }
  }
  return buffer;
}
