#include<stdio.h>
#include<getopt.h>

typedef struct {
    int b_flag;
    int e_flag;
    int n_flag;
    int s_flag;
    int t_flag;
    int v_flag;
    int E_flag;
    int T_flag;
} catOptions;

void output(char c, int *position, int *num, catOptions opt);
int options(int argc, char **argv, catOptions *opt);

int main(int argc, char **argv) {
    char c;
    FILE *f;
    int position = 0, num = 1, currentFile = 1;
    catOptions opt = {0};
    if (argc > 1) {
        options(argc, argv, &opt);
        while(currentFile < argc) {
            f = fopen(argv[currentFile], "r");
            if (f == NULL) {
              printf("%s: %s: No such file or directory\n", argv[0], argv[currentFile]);
            } else {
                while((c = fgetc(f)) != EOF) {
                    output(c, &position, &num, opt);
                    
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

void output(char c, int *position, int *num, catOptions opt) {
    if(opt.b_flag) {
      if(c != '\n' && *position == 0) {
            printf("%6d\t", *num);
            *position+=1;
            *num+=1;
      }
          printf("%c", c);
        if(c == '\n')
            *position = 0;
    }
    if(opt.e_flag) {
      if(c == '\n') {
        printf("$");
      }
    printf("%c", c);
    }
    if(opt.n_flag) {
      if(*position == 0) {
            printf("%6d\t", *num);
            *position+=1;
            *num+=1;
        }
            printf("%c", c);

        if(c == '\n')
            *position = 0;
    }
//    if(opt.s_flag) {
//      if(c == '\n' && *position == 0) {
//          while(c == '\n') {
//              break;
//          }
//      *position+=1;
//      printf("\n");
//      } else {
//         printf("%c", c);
//      }
//      if(c == '\n')
//        *position = 0;
//    }
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
    static struct option long_option[] = {
        {"number-nonblank", 0, NULL, 'b'},
        {"number", 0, NULL, 'n'},
        {"squeeze-blank", 0, NULL, 's'},
        {NULL, 0, NULL, 0}
    };
    while((ch = getopt_long(argc, argv, "+benstET", long_option, NULL)) != -1) {
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
              opt->E_flag = 1; break;
        case 'T':
              opt->T_flag = 1; break;
          default:
              return 0;
      }
    }
    return 1;
}

