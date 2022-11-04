#include <stdio.h>
#include <stdlib.h>
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

int getOption(int argc, char** argv, grepOptions *opt, char** patterns, int *numOfpatterns);
int parser(grepOptions *opt, int i, char** argv, int *numOfpatterns, char** patterns);
void patternForE(char** argv, int i, int k, int* l, char** patterns);
void copyEOstr (char *dest, char *src, int k);

int main(int argc, char **argv) {
    char* patterns[10];
    for (int i = 0; i < 10; i++)
        patterns[i] = NULL;
    grepOptions opt = {0};
    int numOfpatterns = 0;
    
    getOption(argc, argv, &opt, patterns, &numOfpatterns);
    
//    for (int i = 0; i < argc; i++)
//      printf("i = %d: %s \n", i, argv[i]);
    printf("e_flag = %d\nc_flag = %d\no_flag = %d\nn_flag = %d\nl_flag = %d\ni_flag = %d\n", opt.e_flag, opt.c_flag, opt.o_flag, opt.n_flag, opt.l_flag, opt.i_flag);
    printf("patterns[0] = %s\n", patterns[0]);
    printf("patterns[1] = %s\n", patterns[1]);
    printf("patterns[2] = %s\n", patterns[2]);
    printf("numOfpatterns = %d\n", numOfpatterns);
    for (int i = 0; i < numOfpatterns; i++)  {
        printf("patterns[%d] was freed= %s\n", i, patterns[i]);
        free(patterns[i]);
            }
  return 0;
}



int getOption(int argc, char** argv, grepOptions *opt, char** patterns, int *numOfpatterns) {
    int i = 1;
    while(i < argc) {
        if(argv[i][0] == '-') {
            parser(opt, i, argv, numOfpatterns, patterns);
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


// копирует src в dest начиная с k-го элемента
void copyEOstr (char *dest, char *src, int k) {
int m = 0;
while(src[k] != '\0') {
    dest[m] = src[k];
    m++;
    k++;
}
//    memset(src, '\0', strlen(src)); // лишнее?
}
