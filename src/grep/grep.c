#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<regex.h>

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


int patternWithoutE (int argc, char** argv, char** patterns);
int grepFunc(char *line, char** patterns);
void reader(FILE *f, int (*grep)(char*, char**), char** patterns, grepOptions opt);
int getOption(int argc, char** argv, grepOptions *opt, char** patterns, int *numOfpatterns);
int parser(grepOptions *opt, int i, char** argv, int *numOfpatterns, char** patterns);
void patternForE(char** argv, int i, int k, int* l, char** patterns);
void copyEOstr (char *dest, char *src, int k);

int main(int argc, char** argv) {
    FILE *f;
    char* patterns[50];  // поменять на динамический чтобы не использовать лишнюю память + в файле мб много паттернов
    for (int i = 0; i < 50; i++)
        patterns[i] = NULL;
    grepOptions opt = {0};
    int numOfpatterns = 0;
    getOption(argc, argv, &opt, patterns, &numOfpatterns);
    
    if(opt.e_flag == 0) {
      patternWithoutE (argc, argv, patterns);
    }
//    printf("pattern = %s\n", patterns[0]);
    int currentFile = 1;  // для варианта где больше 1 файла добавить вывод названия файла в функции output
    while (currentFile < argc) {
        if (argv[currentFile][0] != '\0') {
            f = fopen(argv[currentFile], "rb");
             if (f != NULL) {
                 reader(f, grepFunc, patterns, opt);
                 fclose(f);
             } else {
                 fprintf(stderr, "grep: %s: No such file or directory\n",
                     argv[currentFile]);  // поменять на return 0 и добавить в output - если !input то печатать  эту ошибку
             }
        }
     currentFile++;
    }
    for (int i = 0; i < numOfpatterns; i++)  {
        free(patterns[i]);
    }
    return 0;
}



int patternWithoutE (int argc, char** argv, char** patterns) {
    int i = 1;
    while(i < argc - 1) {
        if (argv[i][0] != '\0') {
            patterns[0] = realloc((patterns[0]), ((strlen(argv[i])) * sizeof(char)));
            strcpy(patterns[0], argv[i]);
            memset(argv[i], '\0', strlen(argv[i]));
            return 1;
    }
        i++;
}
    return 0;
}

//int grep_func(char *line, char** patterns, grepOptions opt) {  // пока убрала  grepOptions opt
int grepFunc(char *line, char** patterns) {
    regex_t reg;
    regmatch_t match[5];
//    if(!opt.e_flag)
      regcomp(&reg, patterns[0], REG_EXTENDED); // вместо argv[1] передаем или char* patternWithoutE (int argc, char** argv) ИЛИ массив char* patterns[100]
    int r = regexec(&reg, line, 5, match, 0);
    if(r == 0) {
        return 1;
    } else {
        return 0;
      }
    regfree(&reg);
}


void reader(FILE *f, int (*grep)(char*, char**), char** patterns, grepOptions opt) {
    char* buffer = NULL;
    size_t len = 0;
    ssize_t read;
    int num = 1;
    while((read = getline(&buffer, &len, f)) != -1) {
        if(grep(buffer, patterns)) {
            if (opt.n_flag) {
              printf("%d:%s", num, buffer);
            } else {
                printf("%s", buffer);
            }
        }
        num++;
    }
    free(buffer);
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
