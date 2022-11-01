#include<stdio.h>
#include<regex.h>

int main() {
    char pattern[] = "en";
    char line[] = "twenty one";
    regex_t reg;
    regmatch_t match[1];
    regcomp(&reg, pattern, REG_EXTENDED);
    int r = regexec(&reg, line, 1, match, 0);
    if(r == 0) {
        printf("Match!\n");
    } else {
        printf("No match\n");
      }
}
