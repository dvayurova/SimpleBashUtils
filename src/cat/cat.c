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

void output(char c, int *position, int *num, catOptions opt, int *empty_count);
int options(int argc, char **argv, catOptions *opt);

int main(int argc, char **argv) {
    char c;
    FILE *f;
    int position = 0, num = 1, currentFile = 1, empty_count = 0;
    catOptions opt = {0};
    int flags = options(argc, argv, &opt);
    if (argc > 1) {
        if (flags)
            currentFile++;
        while(currentFile < argc) {
            f = fopen(argv[currentFile], "r");
            if (f == NULL) {
              printf("%s: %s: No such file or directory\n", argv[0], argv[currentFile]);
            } else {
                while((c = fgetc(f)) != EOF) {
                    if (flags)
                        output(c, &position, &num, opt, &empty_count);
                    else
                        printf("%c", c);
                }
            }
            fclose(f);
            num = 1;
            position = 0;
            currentFile++;
        }
    }
    return 0;
}

void output(char c, int *position, int *num, catOptions opt, int *empty_count) {
    if(opt.b_flag) {
      if(c != '\n' && *position == 0) {
            printf("%6d\t", *num);
            *position+=1;
            *num+=1;
      }
        if(c == '\n')
            *position = 0;
    }
    if(opt.n_flag && !(opt.b_flag)) {
      if(*position == 0) {
            printf("%6d\t", *num);
            *position+=1;
            *num+=1;
        }
        if(c == '\n')
            *position = 0;
    }
    if(opt.e_flag) {
      if(c == '\n') {
        printf("$\n");
      } else {
          printf("%c", c);
      }
    }
  if(!(opt.e_flag) && !(opt.t_flag) && !(opt.s_flag)) {
    printf("%c", c);
  }
    if(opt.s_flag) {
        if(c == '\n') {
            if(*position == 0) {
              *empty_count+=2;
              *position+=1;
            } else
              *empty_count+=1;
        } else
            *empty_count = 0;
        if(*empty_count > 2) {
            ;
        } else {
            printf("%c", c);
        }
    }
    if(opt.t_flag) {
      if(c == '\t') {
        printf("^I");
      } else {
          printf("%c", c);
      }
    }
}

int options(int argc, char **argv, catOptions *opt) {
    char ch;
    int flag = 0;
    static struct option long_option[] = {
        {"number-nonblank", 0, NULL, 'b'},
        {"number", 0, NULL, 'n'},
        {"squeeze-blank", 0, NULL, 's'},
        {NULL, 0, NULL, 0}
    };
    while((ch = getopt_long(argc, argv, "+benstET", long_option, NULL)) != -1) {
      switch(ch) {
        case 'b':
              opt->b_flag = 1;
              flag++; break;
        case 'e':
              opt->e_flag = 1;
              opt->v_flag = 1;
              flag++; break;
        case 'n':
              opt->n_flag = 1;
              flag++; break;
        case 's':
              opt->s_flag = 1;
              flag++; break;
        case 't':
              opt->t_flag = 1;
              opt->v_flag = 1;
              flag++; break;
        case 'E':
              opt->e_flag = 1;
              flag++; break;
        case 'T':
              opt->t_flag = 1;
              flag++; break;
          default:
              
              exit(1);
      }
    }
    return flag;
}

