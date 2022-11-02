#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include<regex.h>

#include<string.h>

typedef struct {
  int e_flag;
  int i_flag;
  int v_flag;
  int c_flag;
  int l_flag;
  int n_flag;
  int h_flag;
  int s_flag;
  int f_flag;
  int o_flag;
} grepOptions;

void reader(FILE *f, int (*grep)(char**, char*), char **argv);
int grep_func(char **argv, char *line);

int getOption(int argc, char** argv, grepOptions *opt, char** patterns);
int parser(grepOptions *opt, int i, char** argv, int *numOfpatterns, char** patterns);
void copyEOstr (char *dest, char *src, int k);
void patternForE(char** argv, int i, int k, int* n, char** patterns);
char* patternWithoutE (int argc, char** argv);

int main(int argc, char **argv) {
    FILE *f;
    char* patterns[100];
    for (int i = 0; i < 100; i++)
        patterns[i] = NULL;
    grepOptions opt = {0};
    getOption(argc, argv, &opt, patterns);
    if(!opt.e_flag) int currentFile = 2;
  if (argc > 1) {
    while (currentFile < argc) {
      f = fopen(argv[currentFile], "rb");
      if (f == NULL) {
        fprintf(stderr, "grep: %s: No such file or directory\n",
                argv[currentFile]);
      } else {
          reader(f, grep_func, argv);
          fclose(f);
      }
      currentFile++;
    }
  }
    for (int i = 0; i < 100; i++)
        free(patterns[i]);
  return 0;
}



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
    regfree(&reg);
}

void reader(FILE *f, int (*grep)(char**, char*), char **argv) {
    char* buffer = NULL;
    size_t len = 0;
    
    int i = 0;
    while(fgets(buffer, 4096, f)) {
        i++;
        if(grep(argv, buffer))
            printf("%d:%s", i, buffer);
    }
}




int getOption(int argc, char** argv, grepOptions *opt, char** patterns) {
    int i = 1, numOfpatterns = 0;
    while(i < argc) {
        if(argv[i][0] == '-') {
            parser(opt, i, argv, &numOfpatterns, patterns);
            memset(argv[i], '\0', strlen(argv[i]));
        }
        i++;
    }
    return 1;
}

int parser(grepOptions *opt, int i, char** argv, int *numOfpatterns, char** patterns) {
    int k = 1, err = 1;
    while (argv[i][k] != '\0') {
     switch (argv[i][k]) {
      case 'e':
        opt->e_flag = 1;
        patternForE(argv, i, k, numOfpatterns, patterns);
        break;
      case 'i':
        opt->i_flag = 1;
        break;
      case 'v':
        opt->v_flag = 1;
        break;
      case 'c':
        opt->c_flag = 1;
        break;
      case 'l':
        opt->l_flag = 1;
        break;
      case 'n':
        opt->n_flag = 1;
        break;
      case 'h':
        opt->h_flag = 1;
        break;
      case 's':
        opt->s_flag = 1;
        break;
      case 'f':
        opt->f_flag = 1;
        break;
      case 'o':
        opt->o_flag = 1;
        break;
      default:
            err = 0;
}
        k++;
}
    return err;
}


// копирует src в dest начиная с k-го элемента
void copyEOstr (char *dest, char *src, int k) {
int m = 0;
while(src[k] != '\0') {
    dest[m] = src[k];
    m++;
    k++;
}
    memset(src, '\0', strlen(src));
}

    
// парсим шаблон после флага -е
void patternForE(char** argv, int i, int k, int* l, char** patterns) {
    if (argv[i][k + 1] != '\0') {
// копируем часть строки i после флага -е в паттерн
        patterns[*l] = realloc((patterns[*l]), ((strlen(argv[i])) - k) * sizeof(char));
        copyEOstr (patterns[*l], argv[i], k+1);
        memset(argv[i], '\0', strlen(argv[i]));
    } else {
    patterns[*l] = realloc((patterns[*l]), (strlen(argv[i+1])) * sizeof(char));
    strcpy(patterns[*l], argv[i+1]);
    memset(argv[i+1], '\0', strlen(argv[i+1]));
    }
    *l+=1; // l = numOfpatterns
}

char* patternWithoutE (int argc, char** argv) {
    while(i < argc) {
        if (argv[i][0] != '\0')
            return argv[i];
    }
}
