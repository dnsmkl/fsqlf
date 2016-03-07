#include <stdio.h> // fopen(), getc(), EOF
#include <assert.h> // assert()
#include "file_compare.h"


// Char-bychar comparison of two files.
enum fcomp_result file_compare(const char *path_left, const char *path_right)
{
    FILE *left = fopen(path_left, "r");
    FILE *right = fopen(path_right, "r");
    if (left == NULL) {
        perror("file_compare() - error opening file on the left. Error");
        return FCOMP_ERROR;
    }
    if (right == NULL) {
        perror("file_compare() - error opening file on the right. Error");
        return FCOMP_ERROR;
    }

    int c_l, c_r;
    do {
        c_l = getc(left);
        c_r = getc(right);
        if (c_l != c_r) return FCOMP_DIFFER;
    } while (c_l != EOF && c_r != EOF);
    return FCOMP_MATCH;
}


//  Example:
//  --
//  int main(int argc, char** argv)
//  {
//      enum fcomp_result m = file_compare("file_compare.c", "ffff.c");
//      switch (m) {
//          case FCOMP_MATCH:
//              puts("file_compare: MATCH");
//              break;
//          case FCOMP_DIFFER:
//              puts("file_compare: DIFFER");
//              break;
//          case FCOMP_ERROR:
//              puts("file_compare: ERROR");
//              break;
//          default:
//              assert(0);
//      }
//  }
//  --
