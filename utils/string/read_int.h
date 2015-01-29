#include <stddef.h>
#include <ctype.h>


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
                tmp_res *= 10;
                tmp_res += digit;
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
