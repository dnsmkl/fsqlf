#ifndef READ_INT_H
#define READ_INT_H


#include <stddef.h> // size_t


size_t FSQLF_read_int(const char *text, size_t lim, int *result);
size_t FSQLF_read_int_array(const char *text, size_t lim, size_t n, int *result);


#endif
