#include <stdlib.h> // NULL
#include <stdio.h> // puts()
#include <assert.h> // assert()
#include <string.h> // strlen()
#include "registered_testcases.h"
#include "tools/file_compare.h"


#define VERBOSE (0)


void alloc_join_4(char **dest,
    const char *src1, size_t len1,
    const char *src2, size_t len2,
    const char *src3, size_t len3,
    const char *src4, size_t len4
)
{
    size_t rlen = len1 + len2 + len3 + len4 + 3;
    *dest = malloc(rlen+1);
    assert(*dest);
    char *desttmp = *dest;

    memcpy(desttmp, src1, len1);
    desttmp += len1;
    memcpy(desttmp, " ", 1);
    desttmp += 1;
    memcpy(desttmp, src2, len2);
    desttmp += len2;
    memcpy(desttmp, " ", 1);
    desttmp += 1;
    memcpy(desttmp, src3, len3);
    desttmp += len3;
    memcpy(desttmp, " ", 1);
    desttmp += 1;
    memcpy(desttmp, src4, len4);
    desttmp += len4;
    (*desttmp) = '\0';
    assert((*dest)[rlen] == '\0');
}


// Iterate over all test-cases.
// See if actual formatting output matches expected.
int main(int argc, char **argv)
{
    int return_code = 0; // success unless proven otherwise

    for (int i = 0; i < TC_COUNT; i++) {
        char *cmd;
        const char *fsqlf = "../fsqlf";
        const char *lib_path = "LD_LIBRARY_PATH=..";
        alloc_join_4(&cmd,
            lib_path, strlen(lib_path),
            fsqlf, strlen(fsqlf),
            tcs[i][TC_INPUT], strlen(tcs[i][TC_INPUT]),
            tcs[i][TC_ACTUAL], strlen(tcs[i][TC_ACTUAL])
        );

        if (VERBOSE) puts(cmd);
        system(cmd);

        enum fcomp_result m = file_compare(
            tcs[i][TC_EXPECTED],
            tcs[i][TC_ACTUAL]
        );
        switch (m) {
            case FCOMP_MATCH:
                printf("file_compare: MATCH  (%s)\n", tcs[i][TC_ACTUAL]);
                break;
            case FCOMP_DIFFER:
                printf("file_compare: DIFFER  (%s)\n", tcs[i][TC_ACTUAL]);
                return_code = 1; // failure
                break;
            case FCOMP_ERROR:
                printf("file_compare: ERROR  (%s)\n", tcs[i][TC_ACTUAL]);
                return_code = 1; // failure
                break;
            default:
                assert(0);
        }
        free(cmd);
    }

    return return_code;
}
