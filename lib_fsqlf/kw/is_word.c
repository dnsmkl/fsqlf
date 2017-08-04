#include <ctype.h>
#include <stddef.h>


// "word" as opposed to "operator"/"separator"/"delimiter".
// "SELECT" is a word, "<=" is not a word.
// It is necessary to separate "SELECT" and "column1" with space.
//
// Side-note:
// "<" and "=" also need space preserved between them.
// (in case such malformed SQL is formatted)
int FSQLF_is_word(char* text, size_t length) {
    if (length <= 2) {
        for (size_t i = 0; i < length; ++i) {
            if (!isalnum(text[i])) {
                return 0;
            }
        }
    }

    return 1;
}
