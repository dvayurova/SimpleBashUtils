#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<regex.h>

int patternWithoutE (int argc, char** argv, char** patterns);
int grep_func(char *line, char** patterns);
void reader(FILE *f, int (*grep)(char*, char**), char** patterns);

int main(int argc, char** argv) {
    FILE *f;
    char* patterns[10];
    for (int i = 0; i < 10; i++)
        patterns[i] = NULL;
    patternWithoutE (argc, argv, patterns);
//    printf("pattern = %s\n", patterns[0]);
    int currentFile = 1;  // для варианта где больше 1 файла добавить вывод названия файла в функции output
    while (currentFile < argc) {
        if (argv[currentFile][0] != '\0') {
            f = fopen(argv[currentFile], "rb");
             if (f != NULL) {
//                 reader(*f, grep_func, patterns, opt); // пока убрала opt
                 reader(f, grep_func, patterns);
                 fclose(f);
             } else {
                 fprintf(stderr, "grep: %s: No such file or directory\n",
                     argv[currentFile]);  // поменять на return 0 и добавить в output - если !input то печатать  эту ошибку
             }
        }
     currentFile++;
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
int grep_func(char *line, char** patterns) {
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

//void reader(FILE *f, int (*grep)(char*, char**, grepOptions), char** patterns,  grepOptions opt) {   // пока убрала  grepOptions opt
void reader(FILE *f, int (*grep)(char*, char**), char** patterns) {
    char* buffer = NULL;
    size_t len = 0;
    ssize_t read;
    while((read = getline(&buffer, &len, f)) != -1) {
        if(grep(buffer, patterns))
            printf("%s", buffer);
    }
    free(buffer);
}

