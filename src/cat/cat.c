#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>


int readFile(char **argv, int currentFile, FILE **f);
void output(char *buffer, FILE *f, int *num, int b_flag);
void bOpt(char **buffer, int *num);
int options(int argc, char **argv, int* b_flag);

int main(int argc, char **argv) {
    FILE *f;
    char buffer[10000];
    int currentFile = 1;
    int num = 1; // for b opt
    int b_flag = 0; // for b opt
    
    if (argc > 1) {
        if (options(argc, argv, &b_flag))
            currentFile += 1;
        while (currentFile < argc) {
            if(readFile(argv, currentFile, &f)) {
                output(buffer, f, &num, b_flag);
            currentFile++;
            num = 1;
        }
    }
    }
    return 0;
}

int readFile(char **argv, int currentFile, FILE **f) {
  *f = fopen(argv[currentFile], "r");
    if (*f == NULL) {
      printf("%s: %s: No such file or directory\n", argv[0], argv[currentFile]);
      return 0;
    }
    return 1;
}

void output(char *buffer, FILE *f, int *num, int b_flag) {
    while (feof(f) == 0) {
        fgets(buffer, 10000, f);
        if (b_flag) {
            bOpt(&buffer, num);
        }
        printf("%s", buffer);
        }
}

int options(int argc, char **argv, int* b_flag) {
    char opt;
    while((opt = getopt(argc, argv, "benstv")) != -1) {
      switch(opt) {
        case 'b':
              *b_flag+=1; break;
        case 'e':
            break;
        case 'n':
            break;
        case 's':
            break;
        case 't':
            break;
        case 'v':
            break;
          default:
              printf("no flags"); //don't forget to delete
              return 0;
      }
    }
    return *b_flag;
}

void bOpt(char **buffer, int *num) {
    int len = strlen(*buffer);
    char str[10000];
    sprintf(str, "%6d\t", *num);
    if(len != 1) {
        strcat(str, *buffer);
        strcpy(*buffer, str);
        *num+=1;
    }
}
