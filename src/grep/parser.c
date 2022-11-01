#include<stdio.h>
#include<string.h>

int parser(int argc, char** argv, char* options);

int main(int argc, char** argv) {
    char opts[100];
    parser(argc, argv, opts);
    for (int i = 0; i < (int)strlen(opts); i++)
      printf("%c\n", opts[i]);
    for (int i = 0; i < argc; i++)
    printf("i = %d: %s \n", i, argv[i]);
}

int parser(int argc, char** argv, char* options) {
    int i = 1, j = 0;
    char pgrepOpts[] = "eivclnhsfo";
    while(i < argc) {
        int k = 1;
        if(argv[i][0] == '-') {
            while (argv[i][k] != '\0') {
                if(strchr(pgrepOpts, argv[i][k]) != NULL) {
                  options[j] = argv[i][k];
                  j++;
                }
                k++;
            }
            memset(argv[i], '\0', strlen(argv[i]));
        }
        i++;
    }
    return 1;
}
