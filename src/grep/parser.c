#include<stdio.h>
#include<string.h>
#include<stdlib.h>

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



int getOption(int argc, char** argv, char* opts, grepOptions *opt, char** patterns);
void parser(char flag, grepOptions *opt);
void copyEOstr (char *dest, char *src, int k);

int main(int argc, char** argv) {
    char options[100];
    char* patterns[100];
    for (int i = 0; i < 100; i++)
        patterns[i] = NULL;
    grepOptions opt = {0};
    getOption(argc, argv, options,  &opt, patterns);
    for (int i = 0; i < (int)strlen(options); i++)
      printf("%c\n", options[i]);
    for (int i = 0; i < argc; i++)
      printf("i = %d: %s \n", i, argv[i]);
    printf("e_flag = %d\nc_flag = %d\no_flag = %d\nn_flag = %d\n", opt.e_flag, opt.c_flag, opt.o_flag, opt.n_flag);
    printf("patterns[0] = %s\n", patterns[0]);
    printf("patterns[1] = %s\n", patterns[1]);
    printf("patterns[2] = %s\n", patterns[2]);
    for (int i = 0; i < 100; i++)
        free(patterns[i]);
}

int getOption(int argc, char** argv, char* opts, grepOptions *opt, char** patterns) {
    int i = 1, j = 0, l = 0;
    char pgrepOpts[] = "eivclnhsfo";
    while(i < argc) {
        int k = 1;
        if(argv[i][0] == '-') {
            while (argv[i][k] != '\0') {
                if(strchr(pgrepOpts, argv[i][k]) != NULL) {
                    opts[j] = argv[i][k];
                    parser(opts[j], opt);
                    if(opt->e_flag) {
                        if (argv[i][k + 1] != '\0') {
// копируем часть строки i после флага е в паттерн
                            patterns[l] = (char*)realloc(patterns[l], ((strlen(argv[i])) - k));
                            copyEOstr (patterns[l], argv[i], k+1);
                        } else {
                        patterns[l] = (char*)realloc(patterns[l], (strlen(argv[i+1])));
                        strcpy(patterns[l], argv[i+1]);
                        memset(argv[i+1], '\0', strlen(argv[i+1]));
                        }
                        l++;
                    }
                  j++;
                }
                k++;
            }
            memset(argv[i], '\0', strlen(argv[i]));
        }
        i++;
    }
    return 1;
}

void parser(char flag, grepOptions *opt) {
    switch (flag) {
      case 'e':
        opt->e_flag = 1;
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
        fprintf(stderr, "usage: ....");
}
}


// копирует src в dest начиная с k-го элемента
void copyEOstr (char *dest, char *src, int k) {
int m = 0;
while(src[k] != '\0') {
    dest[m] = src[k];
    m++;
    k++;
}
}
