#ifndef S21_GREP_H
#define S21_GREP_H

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

int correctInput(int argc, int err, char inv_opt);
void getOption(int argc, char** argv, grepOptions* opt, char** patterns,
               int* numOfpatterns, char** fileForF, int* PatternFiles,
               int* emptystr, int* err, char* inv_opt);
int parser(grepOptions* opt, int i, char** argv, int* numOfpatterns,
           char** patterns, char** fileForF, int* PatternFiles, int argc,
           int* emptystr, int* err, char* inv_opt);
int patternWithoutE(int argc, char** argv, char** patterns, int* err);
void patternEorFileF(char** argv, int i, int k, int* l, char** str, int* err);
void copyEOstr(char* dest, char* src, int k);
int numberOfFiles(int argc, char** argv);
void patternsFromFile(int PatternFiles, char** fileForF, int* l,
                      char** patterns, int* emptystr, int* err);
void grepFunc(char* line, char* patterns, grepOptions opt, int* isMatch);
void reader(int argc, char** argv, grepOptions opt, int numOfpatterns,
            char** patterns, int emptystr);
void v_flag(int* match);
void cl_flag(int match, int* counter, grepOptions opt);
void cl_output(grepOptions opt, int cntFilesForSearch, char* nameOfFile,
               int counter);
void output(int cntFilesForSearch, grepOptions opt, char* nameOfFile, int num,
            char* buffer, int match);
void print(char* buffer);
void cleanMemory(int numOfpatterns, char** patterns, char** fileForF,
                 int PatternFiles);

#endif  // S21_GREP_H
