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
int grepFunc(char *line, char** patterns, grepOptions opt, int numOfpatterns);
void reader(FILE *f, int (*grep)(char*, char**, grepOptions, int), char** patterns, grepOptions opt, int numOfFiles, char* nameOfFile, int* counter,  int numOfpatterns);
int getOption(int argc, char** argv, grepOptions *opt, char** patterns, int *numOfpatterns, int *numOffiles, char** fileForF);
int parser(grepOptions *opt, int i, char** argv, int *numOfpatterns, char** patterns, int *numOffiles, char** fileForF);
void getPatternEOrFileF(char** argv, int i, int k, int* l, char** str);
void copyEOstr (char *dest, char *src, int k);
int numberOfFiles(int argc, char** argv);

int main(int argc, char** argv) {
    FILE *f;
    char* nameOfFile = NULL;
    char* fileForF[50]; //  названия файлов для флага -f поменять на динамический чтобы не использовать лишнюю память
    char* patterns[50];  // поменять на динамический чтобы не использовать лишнюю память + в файле мб много паттернов
    for (int i = 0; i < 50; i++) {
        patterns[i] = NULL;
        fileForF[i] = NULL;
    }
    grepOptions opt = {0};
    int numOfpatterns = 0, numOffiles = 0;
    getOption(argc, argv, &opt, patterns, &numOfpatterns, &numOffiles, fileForF);
    if(opt.e_flag == 0 && opt.f_flag == 0) {
      patternWithoutE (argc, argv, patterns);
      numOfpatterns = 1;
    }
//    printf("pattern = %s\n", patterns[0]);
    int numOfFiles = numberOfFiles(argc, argv);
    int currentFile = 1;
    while (currentFile < argc) {
        if (argv[currentFile][0] != '\0') {
            f = fopen(argv[currentFile], "rb");
            int counter = 0; //  для фалага -с
            nameOfFile = realloc(nameOfFile, (strlen(argv[currentFile])) * sizeof(char));
            strcpy(nameOfFile, argv[currentFile]);
             if (f != NULL) {
                 reader(f, grepFunc, patterns, opt, numOfFiles, nameOfFile, &counter, numOfpatterns);
                 if (opt.c_flag) {
                     if(numOfFiles > 1)
                         printf("%s:", nameOfFile);
                     printf("%d\n", counter);
                 }
                 fclose(f);
             } else {
                 fprintf(stderr, "grep: %s: No such file or directory\n",
                     argv[currentFile]);  // поменять на return 0 и добавить в output - если !input то печатать  эту ошибку
             }
        }
     currentFile++;
    }
//    printf("file: %s\n", fileForF[0]);
//    printf("file: %s\n", fileForF[1]);
    for (int i = 0; i < numOfpatterns; i++)  {
        free(patterns[i]);
        free(fileForF[i]);
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
//            printf("pattern = %s", patterns[0]);
            return 1;
    }
        i++;
}
    return 0;
}

int grepFunc(char *line, char** patterns, grepOptions opt, int numOfpatterns) {
    regex_t reg;
    regmatch_t match[5];  // сколько указать?
    int r, result = 0;
    for(int i = 0; i < numOfpatterns; i++) {
        regcomp(&reg, patterns[i], opt.i_flag ? REG_ICASE | REG_EXTENDED : REG_EXTENDED);
        r = regexec(&reg, line, 5, match, 0);
        if(r == 0) {
            result =  1;
        }
        regfree(&reg);
    }
    return result;
}


void reader(FILE *f, int (*grep)(char*, char**, grepOptions, int), char** patterns, grepOptions opt, int numOfFiles, char* nameOfFile, int* counter,  int numOfpatterns) {
    char* buffer = NULL;
    size_t len = 0;
    ssize_t read;
    int num = 1;
    while((read = getline(&buffer, &len, f)) != -1) {
        if(grep(buffer, patterns, opt, numOfpatterns) == 1) {
            if (opt.c_flag) {
                *counter+=1;
            } else {
              if (opt.n_flag) {
                if(numOfFiles > 1)
                    printf("%s:", nameOfFile);
                printf("%d:%s", num, buffer);
              } else {
                if(numOfFiles > 1)
                    printf("%s:", nameOfFile);
                printf("%s", buffer);
            }
           }
        }
        num++;
    }
    free(buffer);
}


int getOption(int argc, char** argv, grepOptions *opt, char** patterns, int *numOfpatterns, int *numOffiles, char** fileForF) {
    int i = 1;
    while(i < argc) {
        if(argv[i][0] == '-') {
            parser(opt, i, argv, numOfpatterns, patterns, numOffiles, fileForF);
            memset(argv[i], '\0', strlen(argv[i]));
        }
        i++;
    }
    return 1;
}

int parser(grepOptions *opt, int i, char** argv, int *numOfpatterns, char** patterns, int *numOffiles, char** fileForF) {
    int k = 1, err = 1;
    while (argv[i][k] != '\0') {
     switch (argv[i][k]) {
      case 'e':
        opt->e_flag = 1;
        getPatternEOrFileF(argv, i, k, numOfpatterns, patterns);
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
        getPatternEOrFileF(argv, i, k, numOffiles, fileForF);
        break;
      case 'o':
        opt->o_flag = 1;
        break;
      default:
            err = 0;
             exit(1);   // убрать exit, использовать флаг + проверять сработал ли флаг -е. Добавить печать ошибки об отсутствии опции. См. ./a.out -iONE test.txt text.txt
}
        k++;
}
    return err;
}

// парсим шаблон после флага -е
void getPatternEOrFileF(char** argv, int i, int k, int* l, char** str) {
    if (argv[i][k + 1] != '\0') {
// копируем часть строки i после флага -е в паттерн
        str[*l] = realloc((str[*l]), ((strlen(argv[i])) - k) * sizeof(char));
        copyEOstr (str[*l], argv[i], k+1);
        memset(argv[i], '\0', strlen(argv[i]));
    } else {
    str[*l] = realloc((str[*l]), (strlen(argv[i+1])) * sizeof(char));
    strcpy(str[*l], argv[i+1]);
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
}

int numberOfFiles(int argc, char** argv) {
    int numfiles = 0, i = 1;
    while (i < argc) {
        if (argv[i][0] != '\0')
            numfiles++;
        i++;
    }
    return numfiles;
}

