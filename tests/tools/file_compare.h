#ifndef FILE_COMPARE_H
#define FILE_COMPARE_H


enum fcomp_result
{
    FCOMP_MATCH = 0,
    FCOMP_DIFFER = 1,
    FCOMP_ERROR = 2
};


// Char-bychar comparison of two files.
enum fcomp_result file_compare(const char *path_left, const char *path_right);


#endif
