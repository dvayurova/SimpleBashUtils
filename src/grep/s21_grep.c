#define _GNU_SOURCE
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
int grepFunc(char *line, char** patterns, grepOptions opt, int numOfpatterns, int* counter, int* match);
void reader(FILE *f, int (*grep)(char*, char**, grepOptions, int, int*, int*), char** patterns, grepOptions opt, int cntFilesForSearch, char* nameOfFile, int* counter,  int numOfpatterns);
int getOption(int argc, char** argv, grepOptions *opt, char** patterns, int *numOfpatterns, char** fileForF, int* PatternFiles);
int parser(grepOptions *opt, int i, char** argv, int *numOfpatterns, char** patterns, char** fileForF, int* PatternFiles, int argc);
void getPatternEOrFileF(char** argv, int i, int k, int* l, char** str);
void copyEOstr (char *dest, char *src, int k);
int numberOfFiles(int argc, char** argv);
void patternsFromFile(int PatternFiles, char** fileForF, int *l, char** patterns);
void oFunc(char *line, char** patterns, grepOptions opt, int numOfpatterns);
void print(char* buffer);


int main(int argc, char** argv) {
    char* fileForF[100] = {0}; //  названия файлов для флага -f поменять на динамический чтобы не использовать лишнюю память
    char* patterns[100]= {0};  // поменять на динамический чтобы не использовать лишнюю память + в файле мб много паттернов
    grepOptions opt = {0};
    int numOfpatterns = 0;  // количество шаблонов для поиска
    int PatternFiles = 0;  // количество файлов, содержащих шаблоны для поиска
    getOption(argc, argv, &opt, patterns, &numOfpatterns, fileForF, &PatternFiles);
    if(opt.e_flag == 0 && opt.f_flag == 0) {
      patternWithoutE (argc, argv, patterns);
      numOfpatterns = 1;
    }
    for (int i = 0; i < numOfpatterns; i++)  {
        free(patterns[i]);
    }
 for (int i = 0; i < 100; i++)  {
        free(fileForF[i]); // исправить
    }
    return 0;
}



int grepFunc(char *line, char** patterns, grepOptions opt, int numOfpatterns, int* counter, int* isMatch) {
    int r = 0, res = 0;
    if(strcmp(patterns[i], ".") == 0) {
        r = regcomp(&reg, patterns[i], REG_NEWLINE);
    } else {
        r = regcomp(&reg, patterns[i], opt.i_flag ? REG_ICASE | REG_EXTENDED : REG_EXTENDED);
    }
    if (r == 0) {
        res = regexec(&reg, line, 0, NULL, 0);
        if(res == 0) {
            *isMatch = 1;
        }
        regfree(&reg);
    }
}



void reader() {
    FILE *f = NULL;
    char* buffer = NULL;
    size_t len = 0;
    ssize_t read;
    int num = 1;
    int match = 0;
    regex_t reg;
    
    int cntFilesForSearch = numberOfFiles(argc, argv);
    int currentFile = 1;
    while (currentFile < argc) {
        if (argv[currentFile][0] != '\0') {
            f = fopen(argv[currentFile], "rb");
            int counter = 0; //  для фалага -с
            if (f != NULL) {
                while((read = getline(&buffer, &len, f)) != -1) {
                    match = 0;
                    for(int i = 0; i < numOfpatterns && *isMatch == 0; i++) {
                        grep(reg, buffer, patterns[i], opt, &match);
                    }
                    if(opt.v_flag)
                        v_flag(&match);
                    if(opt.c_flag || opt.l_flag)
                        cl_flag();
                    num++;
                }
                if(buffer)
                    free(buffer);
                fclose(f);
             } else {
                 if(!opt.s_flag) {
                 fprintf(stderr, "grep: %s: No such file or directory\n",
                     argv[currentFile]);  // поменять на return 0 и добавить в output - если !input то печатать  эту ошибку
                 }
             }
        }
     currentFile++;
    }
    if(opt.c_flag || opt.l_flag)
        cl_output();
}
    
void v_flag(int* match) {
    if(*match == 1)
        *match = 0;
    else
        *match = 1;
}

void cl_flag(int match, int* counter) {
    if(match)
        counter += 1;
}

void cl_output() {
    
}


void output(int cntFilesForSearch, grepOptions opt) {
    if(cntFilesForSearch > 1 && !opt.h_flag)
        printf("%s:", nameOfFile);
    if(opt.n_flag)
        printf("%d:", num);
    print(buffer);
}

void print(char* buffer) {
    for(int i = 0; buffer[i] != '\0'; i++) {
        printf("%c", buffer[i]);
        if(buffer[i + 1] == '\0' && buffer[i] != '\n')
            printf("\n");
    }
}

int getOption(int argc, char** argv, grepOptions *opt, char** patterns, int *numOfpatterns, char** fileForF, int* PatternFiles) {
    int i = 1;
    while(i < argc) {
        if(argv[i][0] == '-') {
            parser(opt, i, argv, numOfpatterns, patterns, fileForF, PatternFiles, argc);
            memset(argv[i], '\0', strlen(argv[i]));
        }
        i++;
    }
    return 1;
}

int parser(grepOptions *opt, int i, char** argv, int *numOfpatterns, char** patterns, char** fileForF, int* PatternFiles, int argc) {
    int k = 1, err = 1;
    while (argv[i][k] != '\0') {
     switch (argv[i][k]) {
      case 'e':
        opt->e_flag = 1;
             if(i == argc - 1) {
                 printf("grep: option requires an argument -- e");
                 exit(1);  // убрать exit, использовать флаг
             }
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
        getPatternEOrFileF(argv, i, k, PatternFiles, fileForF);
        patternsFromFile(*PatternFiles, fileForF, numOfpatterns, patterns);
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


// парсим шаблон после флага -е или название файла после флага -f
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


//  доработать -f, работает не всегда корректно:
void patternsFromFile(int PatternFiles, char** fileForF, int *l, char** patterns) {
    FILE *f;
    char* buffer = NULL;
    size_t len = 0;
    ssize_t read;
    for(int i = 0; i < PatternFiles; i++) {
        f = fopen(fileForF[i], "r");
        if (f != NULL) {
            while((read = getline(&buffer, &len, f)) != -1) {
                patterns[*l] = malloc((strlen(buffer) + 1) * sizeof(char));
                strcpy(patterns[*l], buffer);
                if(patterns[*l][strlen(buffer) - 1] == '\n') {
                    patterns[*l][strlen(buffer) - 1] = '\0';
                }
                *l = *l + 1;
            }
            free(buffer);
            fclose(f);
        } else {
            fprintf(stderr, "grep: %s: No such file or directory\n",
                fileForF[i]);  // поменять на return 0 и добавить в output - если !input то печатать  эту ошибку
        }
    }
}


void oFunc(char *line, char** patterns, grepOptions opt, int numOfpatterns) {
    int r = 0, index = 0;
    regmatch_t match;  //  не указывать []
    for(int i = 0; i < numOfpatterns; i++) {
        do {
            regex_t reg;
            regcomp(&reg, patterns[i], opt.i_flag ? REG_ICASE : 0);
            r = regexec(&reg, line + index, 1, &match, 0);
            if(r == 0) {
                for(int j = index + match.rm_so; j < index + match.rm_eo; j++) {
                    printf("%c", line[j]);
                }
                printf("\n");
            }
            index += match.rm_eo;
            regfree(&reg);
        } while(r == 0);
    }
}
