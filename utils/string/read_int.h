#include <stddef.h> // NULL
#include <ctype.h>  // isdigit
#include <limits.h> // INT_MAX


// Parse positive integer. Skip leading spaces.
// Only leading spaces and adjacent digits are read.
// @text - text from which we want to extract integer
// @lim - limit characters of text length
// @result - output param for parsed integer
//      (if value in string is too large to fit into int, then INT_MAX is set)
// return value - number of chars read (on error 0 is returned)
int read_int(const char *text, size_t lim, int *result)
{
    assert(text != NULL);
    assert(result != NULL);

    int chars_read = 0;
    int digits_read = 0;
    int tmp_res = 0;

    char no_digit_yet = 1; // bool - no digits found yet?
    for (size_t i = 0; i < lim; ++i) {
        char c = text[i];
        no_digit_yet = no_digit_yet && !isdigit(c); // Mark found digit.

        if (no_digit_yet) {
            if (c == ' ') {
                ++chars_read;
                continue; // Skip leading spaces.
            } else {
                return 0; // 0 read.
            }
        } else {
            if (isdigit(c)) {
                // Read the digits number
                int digit = c - '0';
                assert(digit < 10);
                assert(digit >= 0);

                if (tmp_res <= INT_MAX / 10) {
                    tmp_res *= 10;
                } else {
                    tmp_res = INT_MAX;
                }

                if (tmp_res < INT_MAX - digit) {
                    tmp_res += digit;
                } else {
                    tmp_res = INT_MAX;
                }

                ++chars_read;
                ++digits_read;
            } else {
                break;  // Number has ended.
            }
        }
    }

    if (digits_read > 0) {
        *result = tmp_res;
        return chars_read;
    } else {
        return 0;
    }
}
