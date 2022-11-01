#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include<regex.h>

//typedef struct {
//  int e_flag;
//  int i_flag;
//  int v_flag;
//  int c_flag;
//  int l_flag;
//  int n_flag;
//} grepOptions;

void reader(FILE *f, int (*grep)(char**, char*), char **argv);
int grep_func(char **argv, char *line);


int main(int argc, char **argv) {
  FILE *f;
//  int currentFile = 1 + numOfFlags(argc, argv);
    int currentFile = 2;
//  grepOptions opt = {0};
//  options(argc, argv, &opt);
  if (argc > 1) {
    while (currentFile < argc) {
      f = fopen(argv[currentFile], "rb");
      if (f == NULL) {
        fprintf(stderr, "grep: %s: No such file or directory\n",
                argv[currentFile]);
      } else {
          reader(f, grep_func, argv);
//        grep_func(argc, argv);
      }
      fclose(f);
      currentFile++;
    }
  }
  return 0;
}


//int numOfFlags(int argc, char **argv) {
//  int countOfFlags = 0, i = 1;
//  while (i < argc) {
//    if (argv[i][0] == '-') {
//      countOfFlags++;
//    } else
//      i = argc;
//    i++;
//  }
//  return countOfFlags;
//}
//
//int parser() {
//
//}


int grep_func(char **argv, char *line) {
//    char pattern[] = "en"; // argv[1]
//    char line[] = "twenty one"; // f
    regex_t reg;
    regmatch_t match[1];
    regcomp(&reg, argv[1], REG_EXTENDED);
    int r = regexec(&reg, line, 1, match, 0);
    if(r == 0) {
        return 1;
    } else {
        return 0;
      }
}

void reader(FILE *f, int (*grep)(char**, char*), char **argv) {
    char buffer[4096];
    int i = 0;
    while(fgets(buffer, 4096, f)) {
        i++;
        if(grep(argv, buffer))
            printf("%d:%s", i, buffer);
    }
}
