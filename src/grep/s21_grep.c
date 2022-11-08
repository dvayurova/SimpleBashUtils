#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<regex.h>

int main(int argc, char** argv) {
    char* fileForF[100] = {0}; //  названия файлов для флага -f поменять на динамический чтобы не использовать лишнюю память
    char* patterns[100]= {0};  // поменять на динамический чтобы не использовать лишнюю память + в файле мб много паттернов
    grepOptions opt = {0};
    int numOfpatterns = 0;  // количество шаблонов для поиска
    int PatternFiles = 0;  // количество файлов, содержащих шаблоны для поиска

    if(opt.e_flag == 0 && opt.f_flag == 0) {
      patternWithoutE (argc, argv, patterns);
      numOfpatterns = 1;
    }
}

void onePattern(int argc, char** argv, char** patterns) {
    int i = 1;
    while(i < argc - 1) {
        if (argv[i][0] != '\0') {
            patterns[0] = realloc((patterns[0]), ((strlen(argv[i])) * sizeof(char)));
            strcpy(patterns[0], argv[i]);
            memset(argv[i], '\0', strlen(argv[i]));
    }
        i++;
    }
}

int parser(grepOptions *opt, int i, char** argv, int *numOfpatterns, char** patterns, char** fileForF, int* PatternFiles, int argc) {
    int i = 1, k = 1, err = 1;
    while(i < argc) {
        if(argv[i][0] == '-') {
            while (argv[i][k] != '\0') {
                switch (argv[i][k]) {
                    case 'e':
                        opt->e_flag = 1;
                        if(i == argc - 1) {
                            printf("grep: option requires an argument -- e");
                            err = 0;
                        }
                        getPatternsEF(argv, i, k, numOfpatterns, patterns);
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
                        getPatternsEF(argv, i, k, PatternFiles, fileForF);
                        patternsFromFile(*PatternFiles, fileForF, numOfpatterns, patterns);
                        break;
                    case 'o':
                        opt->o_flag = 1;
                        break;
                    default:
                        err = 0;   // Добавить печать ошибки об отсутствии опции. См. ./a.out -iONE test.txt text.txt
                }
                k++;
            }
            memset(argv[i], '\0', strlen(argv[i]));
        }
        i++;
    }
}

// парсим шаблон после флага -е или название файла после флага -f
void getPatternsEF(char** argv, int i, int k, int* l, char** str) {
    if (argv[i][k + 1] != '\0') {
        str[*l] = realloc((str[*l]), ((strlen(argv[i])) - k) * sizeof(char));
        copyEndOfstr (str[*l], argv[i], k+1);
        memset(argv[i], '\0', strlen(argv[i]));
    } else {
        str[*l] = realloc((str[*l]), (strlen(argv[i+1])) * sizeof(char));
        strcpy(str[*l], argv[i+1]);
        memset(argv[i+1], '\0', strlen(argv[i+1]));
    }
    *l+=1; // l = numOfpatterns
}

// копирует src в dest начиная с k-го элемента
void copyEndOfstr (char *dest, char *src, int k) {
    int m = 0;
    while(src[k] != '\0') {
        dest[m] = src[k];
        m++;
        k++;
    }
}

int patternsFromFile(int fFilesCount, char** fileForF, int *l, char** patterns) {
    FILE *f;
    char* buffer = NULL;
    size_t len = 0;
    ssize_t read;
    for(int i = 0; i < fFilesCount; i++) {
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
            return 0;
            // fprintf(stderr, "grep: %s: No such file or directory\n", fileForF[i]);  // поменять на return 0 и добавить в output - если !input то печатать  эту ошибку
        }
    }
    return 1;
}
            
int grepFunc(char *line, char** patterns, grepOptions opt, int patternsCount, int *countC, int *countL) {
    regex_t reg;
    int r = 1;
    for(int i = 0; i < patternsCount; i++) {
        regcomp(&reg, patterns[i], opt.i_flag ? REG_ICASE | REG_EXTENDED : REG_EXTENDED);
        r = regexec(&reg, line, 0, NULL, 0);
        regfree(&reg);
    }
    if(r == 0 && !opt.v_flag) {
        *countC++;
        *countL = 1;
    } else if(r != 0 && opt.v_flag){
        *countC;
        *countL = 1;
    }
    return r;
}

void reader() {
    FILE *f;
    int SearchFilesCount = numberOfFiles(argc, argv);
    int currentFile = 1;
    char* buffer = NULL;
    size_t len = 0;
    ssize_t read;
    int grep_res = 1;
    while (currentFile < argc) {
        if (argv[currentFile][0] != '\0') {
            f = fopen(argv[currentFile], "rb");
            int num = 1, countC = 0, countL = 0;
            if (f != NULL) {
            while((read = getline(&buffer, &len, f)) != -1) {
                grep_res = grep(buffer, patterns, opt, numOfpatterns, &countC, &countL);
//                output(buffer, grep_res, argv[currentFile]);
                num++;
            }
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

void output(char* buffer, int grep_res, char* fileName, int num) {
    if(SearchFilesCount > 1 && !opt.h_flag)
        printf("%s:", fileName);
    if (opt.c_flag && opt.l_flag) {
        printf("%d", countL);
    } else if(opt.c_flag && !opt.l_flag) {
        printf("%d", countC);
    } else {
        if(opt.n_flag) {
          printf("%d:", num);
        }
        printf("%s", buffer);
    }

    
    
    } else if(opt.o_flag && !opt.v_flag) {
        oFunc(buffer, patterns, opt, numOfpatterns);
    }
    if(opt.l_flag) {
        printf("%s", fileName);
    }
}






void oFunc(char *line, char** patterns, grepOptions opt, int numOfpatterns) {
    int r = 0, index = 0;
    regmatch_t match;
    for(int i = 0; i < numOfpatterns; i++) {
        do {
            regex_t reg;
            regcomp(&reg, patterns[i], opt.i_flag ? REG_ICASE | REG_EXTENDED : REG_EXTENDED);
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

