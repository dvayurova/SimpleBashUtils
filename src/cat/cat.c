#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>


int options(int argc, char **argv);
void output(FILE *fp);
void readFile(FILE **fp, int argc, char **argv, int currentFile);

int main(int argc, char **argv) {
    FILE *fp;
    
    int currentFile = (argc > 1 ? 1 : 0);
    if (options(argc, argv)) {
      while (currentFile < argc) {
        readFile(&fp, argc, argv, currentFile);
        currentFile++;
      }
    }
    return 0;
}

int options(int argc, char **argv) {
    int bflag = 0, eflag = 0, nflag = 0, sflag = 0, tflag = 0, vflag = 0;
//    int uflag = 0;
    int opt;
    int flag = 1;

    while ((opt = getopt(argc, argv, "benstuv?")) != -1) {
        switch(opt) {
            case 'b':
                bflag++; break;
            case 'e':
                eflag++; break;
            case 'n':
                nflag++; break;
            case 's':
                sflag++; break;
            case 't':
                tflag++; break;
//            case 'u':
//                uflag++; break;
            case 'v':
                vflag++; break;
            case '?':
                printf("usage: cat [-benstuv] [file ...]\n");
                flag = 0;
        }
    }
    return flag;
}

void output(FILE *fp) {
//    int size = 1;
    int i = 0;
    char ch;
    while ((ch = fgetc(fp)) != EOF) {
      fprintf(stdout, "%c", ch);
        i++;
//        size++;
//        buffer = realloc(buffer, size);
    }
}

void readFile(FILE **fp, int argc, char **argv, int currentFile) {
    
//    buffer = malloc(sizeof(char));
    if (argc > 1) {
      *fp = fopen(argv[currentFile], "r");
      if (*fp == NULL) {
        fprintf(stderr, "cat: %s: No such file or directory\n", argv[currentFile]);
      }
    }
    output(*fp);
    fclose(*fp);
//    free(buffer);
}
