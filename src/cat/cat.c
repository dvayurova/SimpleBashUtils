#include<stdio.h>
#include<getopt.h>
#include<stdlib.h>

typedef struct {
    int b_flag;
    int e_flag;
    int n_flag;
    int s_flag;
    int t_flag;
    int v_flag;
} catOptions;

void options(int argc, char **argv, catOptions *opt);
int numOfFlags(int argc, char** argv);
void cat_func(FILE *f, catOptions opt);

int main(int argc, char **argv) {
    FILE *f;
    int currentFile = 1 + numOfFlags(argc, argv);
    catOptions opt = {0};
    options(argc, argv, &opt);
    if (argc > 1) {
        while(currentFile < argc) {
            f = fopen(argv[currentFile], "rb");
            if (f == NULL) {
                fprintf(stderr, "cat: %s: No such file or directory\n", argv[currentFile]);
            } else {
                cat_func(f, opt);
            }
            fclose(f);
            currentFile++;
        }
    }
    return 0;
}

void cat_func(FILE *f, catOptions opt) {
    int position = 0, num = 1, beginOfFile = 0;
    char c;
    char prevCh = '\n', prevPrev = '\n';
    while((c = fgetc(f)) != EOF) {
        if(opt.s_flag && prevCh == '\n' && c == '\n' && prevPrev == '\n' && beginOfFile != 0) {
            continue;
        }
        if((opt.b_flag && c != '\n' && position == 0) ||
           (opt.n_flag && !(opt.b_flag) && position == 0)) {
            printf("%6d\t", num);
            num++;
            position++;
        }
        
        if(opt.e_flag && c == '\n') {
            printf("$");
        }
        if(opt.t_flag && c == '\t') {
            printf("^");
            c = 'I';
        }
        if(opt.v_flag) {
            if((c >= 0 && c < 9) || (c >= 11 && c <= 31)) {
                printf("^");
                c += 64;
            }
            else if(c == 127) {
                printf("^");
                c -= 64;
            }
        }
        prevPrev = prevCh;
        prevCh = c;
        beginOfFile++;
        if (c == '\n')
            position = 0;
        printf("%c", c);
    }
}

void options(int argc, char **argv, catOptions *opt) {
    char ch;
    static struct option long_option[] = {
        {"number-nonblank", 0, NULL, 'b'},
        {"number", 0, NULL, 'n'},
        {"squeeze-blank", 0, NULL, 's'},
        {NULL, 0, NULL, 0}
    };
    while((ch = getopt_long(argc, argv, "+benstvET", long_option, NULL)) != -1) {
      switch(ch) {
        case 'b':
              opt->b_flag = 1; break;
        case 'e':
              opt->e_flag = 1;
              opt->v_flag = 1; break;
        case 'n':
              opt->n_flag = 1; break;
        case 's':
              opt->s_flag = 1; break;
        case 't':
              opt->t_flag = 1;
              opt->v_flag = 1; break;
        case 'E':
              opt->e_flag = 1; break;
        case 'T':
              opt->t_flag = 1; break;
        case 'v':
                opt->v_flag = 1; break;
          default:
              fprintf(stderr, "usage: %s [-benstuv] [file ...]", argv[0]);
              exit(1);
      }
    }
}

int numOfFlags(int argc, char** argv) {
    int countOfFlags = 0, i = 1;
    while(i < argc) {
        if(argv[i][0] == '-') {
            countOfFlags++;
        } else
            i = argc;
        i++;
    }
    return countOfFlags;
}
