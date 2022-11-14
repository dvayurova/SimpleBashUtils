#define _GNU_SOURCE
#include "s21_grep.h"

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  char *fileForF[1024] = {0};
  char *patterns[1024] = {0};
  grepOptions opt = {0};
  int numOfpatterns = 0; // количество шаблонов для поиска
  int PatternFiles = 0; // количество файлов, содержащих шаблоны для поиска
  int emptystr = 0;
  int err = 0;
  char inv_opt = ' ';
  getOption(argc, argv, &opt, patterns, &numOfpatterns, fileForF, &PatternFiles,
            &emptystr, &err, &inv_opt);
  if (opt.e_flag == 0 && opt.f_flag == 0) {
    if (patternWithoutE(argc, argv, patterns, &err))
      numOfpatterns = 1;
  }
  if (correctInput(argc, err, inv_opt)) {
    reader(argc, argv, opt, numOfpatterns, patterns, emptystr);
  }
  cleanMemory(numOfpatterns, patterns, fileForF, PatternFiles);
  return 0;
}

int correctInput(int argc, int err, char inv_opt) {
  int res = 1;
  if (argc == 1 || err) {
    res = 0;
    if (!err) {
      printf("usage: grep [-ivclnhs] [-e pattern] [-f file] [--null] [pattern] "
             "[file ...]");
    } else {
      if (err == 1) {
        printf("grep: invalid option -- %c\nusage: grep [-ivclnhs]\n"
               "[-e pattern] [-f file] [--null] [pattern] [file ...]",
               inv_opt);
      } else if (err == 2) {
        printf("grep: option requires an argument");
      } else if (err == 3) {
        printf("Insufficient memory available");
      }
    }
  }
  return res;
}

void getOption(int argc, char **argv, grepOptions *opt, char **patterns,
               int *numOfpatterns, char **fileForF, int *PatternFiles,
               int *emptystr, int *err, char *inv_opt) {
  int i = 1;
  while (i < argc) {
    if (argv[i][0] == '-') {
      parser(opt, i, argv, numOfpatterns, patterns, fileForF, PatternFiles,
             argc, emptystr, err, inv_opt);
      memset(argv[i], '\0', strlen(argv[i]));
    }
    i++;
  }
}

int parser(grepOptions *opt, int i, char **argv, int *numOfpatterns,
           char **patterns, char **fileForF, int *PatternFiles, int argc,
           int *emptystr, int *err, char *inv_opt) {
  int k = 1;
  while (argv[i][k] != '\0') {
    if (argv[i][k] == 'e') {
      opt->e_flag = 1;
      if (i == (argc - 1) && argv[i][k + 1] == '\0') {
        *err = 2;
      } else {
        patternEorFileF(argv, i, k, numOfpatterns, patterns, err);
      }
    } else if (argv[i][k] == 'i')
      opt->i_flag = 1;
    else if (argv[i][k] == 'v')
      opt->v_flag = 1;
    else if (argv[i][k] == 'c')
      opt->c_flag = 1;
    else if (argv[i][k] == 'l')
      opt->l_flag = 1;
    else if (argv[i][k] == 'n')
      opt->n_flag = 1;
    else if (argv[i][k] == 'h')
      opt->h_flag = 1;
    else if (argv[i][k] == 's')
      opt->s_flag = 1;
    else if (argv[i][k] == 'f') {
      opt->f_flag = 1;
      if (i == (argc - 1) && argv[i][k + 1] == '\0') {
        *err = 2;
      } else {
        patternEorFileF(argv, i, k, PatternFiles, fileForF, err);
        patternsFromFile(*PatternFiles, fileForF, numOfpatterns, patterns,
                         emptystr, err);
      }
    } else {
      *err = 1;
      *inv_opt = argv[i][k];
    }
    k++;
  }
  return *err;
}

int patternWithoutE(int argc, char **argv, char **patterns, int *err) {
  int i = 1, flag = 1, res = 0;
  while (i < (argc - 1) && flag) {
    if (argv[i][0] != '\0') {
      patterns[0] = malloc((strlen(argv[i]) + 1) * sizeof(char));
      if (patterns[0] != NULL)
        strcpy(patterns[0], argv[i]);
      else
        *err = 3;
      memset(argv[i], '\0', strlen(argv[i]));
      flag = 0;
      res = 1;
    }
    i++;
  }
  return res;
}

void patternEorFileF(char **argv, int i, int k, int *l, char **str,
                     int *err) { // парсим шаблон после флага -е или название
                                 // файла после флага -f
  if (argv[i][k + 1] !=
      '\0') { // копируем часть строки i после флага -е в паттерн
    str[*l] = malloc(((strlen(argv[i]) - k) + 1) * sizeof(char));
    if (str[*l] != NULL) {
      copyEOstr(str[*l], argv[i], k + 1);
    } else
      *err = 3;
    memset(argv[i], '\0', strlen(argv[i]));
  } else {
    str[*l] = malloc((strlen(argv[i + 1]) + 1) * sizeof(char));
    if (str[*l] != NULL)
      strcpy(str[*l], argv[i + 1]);
    else
      *err = 3;
    memset(argv[i + 1], '\0', strlen(argv[i + 1]));
  }
  *l += 1;
}

void copyEOstr(char *dest, char *src,
               int k) { // копирует src в dest начиная с k-го элемента
  int m = 0;
  while (src[k] != '\0') {
    dest[m] = src[k];
    m++;
    k++;
  }
  dest[m] = '\0';
}

int numberOfFiles(int argc, char **argv) {
  int numfiles = 0, i = 1;
  while (i < argc) {
    if (argv[i][0] != '\0')
      numfiles++;
    i++;
  }
  return numfiles;
}

void patternsFromFile(int PatternFiles, char **fileForF, int *l,
                      char **patterns, int *emptystr, int *err) {
  FILE *f = NULL;
  char *buffer = NULL;
  size_t len = 0;
  ssize_t read = 0;
  for (int i = 0; i < PatternFiles; i++) {
    f = fopen(fileForF[i], "r");
    if (f != NULL) {
      while ((read = getline(&buffer, &len, f)) != -1) {
        if (buffer[0] == '\n')
          *emptystr = 1;
        patterns[*l] = malloc((strlen(buffer) + 1) * sizeof(char));
        if (patterns[*l] != NULL)
          strcpy(patterns[*l], buffer);
        else
          *err = 3;
        if (patterns[*l][strlen(buffer) - 1] == '\n') {
          patterns[*l][strlen(buffer) - 1] = '\0';
        }
        *l = *l + 1;
      }
      if (buffer)
        free(buffer);
      fclose(f);
    } else {
      fprintf(stderr, "grep: %s: No such file or directory\n", fileForF[i]);
    }
  }
}

void grepFunc(char *line, char *patterns, grepOptions opt, int *isMatch) {
  int r = 0, res = 0;
  regex_t reg;
  if (strcmp(patterns, ".") == 0) {
    r = regcomp(&reg, patterns, REG_NEWLINE);
  } else {
    r = regcomp(&reg, patterns,
                opt.i_flag ? REG_ICASE | REG_EXTENDED : REG_EXTENDED);
  }
  if (r == 0) {
    res = regexec(&reg, line, 0, NULL, 0);
    if (res == 0) {
      *isMatch = 1;
    }
    regfree(&reg);
  }
}

void reader(int argc, char **argv, grepOptions opt, int numOfpatterns,
            char **patterns, int emptystr) {
  FILE *f = NULL;
  size_t len = 0;
  ssize_t read;
  int num = 1; // для вывода номера строки
  int match = 0;
  int counter = 0; //  для флага -с
  int cntFilesForSearch = numberOfFiles(argc, argv);
  int currentFile = 1;
  while (currentFile < argc) {
    if (argv[currentFile][0] != '\0') {
      f = fopen(argv[currentFile], "rb");
      counter = 0;
      if (f != NULL) {
        char *buffer = NULL;
        num = 1;
        while ((read = getline(&buffer, &len, f)) != -1) {
          match = 0;
          for (int i = 0; i < numOfpatterns && match == 0; i++) {
            grepFunc(buffer, patterns[i], opt, &match);
          }
          if (emptystr)
            match = 1;
          if (opt.v_flag)
            v_flag(&match);
          if (opt.c_flag || opt.l_flag)
            cl_flag(match, &counter, opt);
          if (!opt.c_flag && !opt.l_flag)
            output(cntFilesForSearch, opt, argv[currentFile], num, buffer,
                   match);
          num++;
        }
        if (opt.c_flag || opt.l_flag)
          cl_output(opt, cntFilesForSearch, argv[currentFile], counter);
        if (buffer)
          free(buffer);
        fclose(f);
      } else {
        if (!opt.s_flag)
          fprintf(stderr, "grep: %s: No such file or directory\n",
                  argv[currentFile]);
      }
    }
    currentFile++;
  }
}

void v_flag(int *match) {
  if (*match == 1)
    *match = 0;
  else
    *match = 1;
}

void cl_flag(int match, int *counter, grepOptions opt) {
  if (match) {
    *counter += 1;
    if (opt.l_flag)
      *counter = 1;
  }
}

void cl_output(grepOptions opt, int cntFilesForSearch, char *nameOfFile,
               int counter) {
  if (opt.c_flag) {
    if (cntFilesForSearch > 1 && !opt.h_flag)
      printf("%s:", nameOfFile);
    printf("%d\n", counter);
  }
  if (counter && opt.l_flag) {
    printf("%s\n", nameOfFile);
  }
}

void output(int cntFilesForSearch, grepOptions opt, char *nameOfFile, int num,
            char *buffer, int match) {
  if (match) {
    if (cntFilesForSearch > 1 && !opt.h_flag)
      printf("%s:", nameOfFile);
    if (opt.n_flag)
      printf("%d:", num);
    print(buffer);
  }
}

void print(char *buffer) {
  for (int i = 0; buffer[i] != '\0'; i++) {
    printf("%c", buffer[i]);
    if (buffer[i + 1] == '\0' && buffer[i] != '\n')
      printf("\n");
  }
}

void cleanMemory(int numOfpatterns, char **patterns, char **fileForF,
                 int PatternFiles) {
  for (int i = 0; i < numOfpatterns; i++) {
    free(patterns[i]);
  }
  for (int i = 0; i < PatternFiles; i++) {
    free(fileForF[i]);
  }
}
