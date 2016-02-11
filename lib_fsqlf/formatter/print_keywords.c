#include <string.h>     // strlen
#include <ctype.h>      // tolower, toupper
#include <stdio.h>      // fprintf, fputs
#include <assert.h>      // assert
#include "print_keywords.h"


// Return max argument.
// Strange name is chosen just to make it unique,
// so it would not clash with some compiler/platform specific macros.
static int max_2args_(int a, int b)
{
    return a > b ? a : b;
}


// based on 3rd (bool) arg return
// either max(1st, 2nd)
// or 2nd arg
static int max_or_current(int prev_count, int curr_count, char use_only_curr_ind)
{
    if (use_only_curr_ind) {
        return curr_count;
    }
    else{
        return max_2args_(prev_count, curr_count);
    }
}


static struct fsqlf_spacing calculate_spacing(
    struct fsqlf_spacing afterspacing_of_prev,
    unsigned short int isword_of_prev,
    struct fsqlf_spacing beforespacing_of_current,
    unsigned short int isword_of_current,
    int global_indent_level)
{
    /* Combine spacings of adjacent words (previous and current).
        Combinations are done in two ways:
            - take MAX
            - use only current words settings
            (one more option could be to
            first print previous word's 'after' spacing,
            then print current word's 'before' spacing, but this could/would
            result in trailing spaces on a line)
        Decision, which of them to use, is made so:
            There are 3 types of spacings.
            They are ranked this way: space-1, indentation-2, new line-3.
            If current word has some non-zero spacing setting,
            then for spacings of lower ranks use conf only from current word.
        Rationale for such logic:
            If word has in its settings new-line=1 and indent/spaces=0,
            then there is expectation that keyword will be
            at the start of the line, no matter what previous fsqlf_kw_get setting
            for indentation after it was.
            (except for global indent level, because of subselect)
    */
    struct fsqlf_spacing r; // result to be built


    r.new_line = max_2args_(afterspacing_of_prev.new_line, beforespacing_of_current.new_line);

    r.indent = max_or_current(
            afterspacing_of_prev.indent
            , beforespacing_of_current.indent
            , beforespacing_of_current.new_line>1);
    r.indent += r.new_line ? global_indent_level : 0;

    r.space = max_or_current(
            afterspacing_of_prev.space
            , beforespacing_of_current.space
            , beforespacing_of_current.new_line || beforespacing_of_current.indent);


    // adjacent words MUST have some spacing
    if (!r.new_line && !r.indent && !r.space &&
        isword_of_prev && isword_of_current) {
        r.space = 1;
    }

    return r;
}


char *FSQLF_stocase(const char *s_text, enum fsqlf_kwcase s_case)
{
    size_t len = strlen(s_text);
    char *formatted_result = malloc(len+1);
    assert(formatted_result);

    switch (s_case) {
        case FSQLF_KWCASE_LOWER:
            for (int i = 0; i<len; i++) {
                formatted_result[i] = tolower(s_text[i]);
            }
            break;
        case FSQLF_KWCASE_UPPER:
            for (int i = 0; i<len; i++) {
                formatted_result[i] = toupper(s_text[i]);
            }
            break;
        case FSQLF_KWCASE_INITCAP:
            formatted_result[0] = toupper(s_text[0]);
            for (int i = 1; i<len; i++) {
                formatted_result[i] = tolower(s_text[i]);
            }
            break;
        case FSQLF_KWCASE_ORIGINAL:
            memcpy(formatted_result, s_text, len);
    }
    formatted_result[len] = '\0';
    return formatted_result;
}


static void memcpy_ntimes(char *dst, const char *src, size_t len, size_t count)
{
    for (int i = 0; i < count; i++) {
        memcpy(dst+i*len, src, len);
    }
}


static char * struct_spacing_to_str(struct fsqlf_spacing cnt)
{
    const char *newline = "\n";
    int len_n = strlen(newline);
    const char *indent = "    "; // 4 spaces.
    int len_i = strlen(indent);
    const char *space = " ";
    int len_s = strlen(space);

    size_t len = cnt.new_line * len_n;
    len += cnt.indent * len_i;
    len += cnt.space * len_s;

    char *text = malloc(len+1);
    assert(text);
    char *tmp = text;

    memcpy_ntimes(tmp, newline, len_n, cnt.new_line);
    tmp += cnt.new_line * len_n;
    memcpy_ntimes(tmp, indent, len_i, cnt.indent);
    tmp += cnt.indent * len_i;
    memcpy_ntimes(tmp, space, len_s, cnt.space);
    text[len] = '\0';

    return text;
}


// All printing of spacing goes through here.
// Except when space is inside the multiword-keyword (e.g. "LEFT JOIN"),
// those will not be printed by this function.
// ('spacing' means new lines, tabs and spaces)
static char * get_spacing_str(
    FILE *yyout,
    struct fsqlf_kw_conf current_settings,
    int global_indent_level)
{
    // keep track of 'after' spacing from previous call
    static struct fsqlf_spacing from_previous__scounts = {0, 0, 0, 0};
    // keep track of previous 'is_word'
    static unsigned short int from_previous__isword = 0;

    struct fsqlf_spacing spacing =
        calculate_spacing(
            from_previous__scounts,
            from_previous__isword,
            current_settings.before,
            current_settings.is_word,
            global_indent_level
        );

    char *spacing_txt = struct_spacing_to_str(spacing);

    // Save settings for next function call - overwrite
    from_previous__scounts = current_settings.after;
    from_previous__isword = current_settings.is_word;
    return spacing_txt;
}


void FSQLF_kw_print(FILE *yyout, size_t indent, const char *yytext,
                        struct fsqlf_kw_conf s)
{
    // Print spacing.
    char *spacing_txt = get_spacing_str(yyout, s, indent); // print spacing before keyword

    // Print text:
    // .. first decide what text to use (original or default)
    const char *text_nocase;
    switch (s.print_original_text) {
        case FSQLF_KWTEXT_USE_ORIGINAL:
            text_nocase = yytext;
            break;
        case FSQLF_KWTEXT_USE_HARDCODED_DEFAULT:
            text_nocase = s.text;
            break;
        default:
            assert(0);
    }
    // .. then handle its case
    char *text = FSQLF_stocase(text_nocase, s.print_case);

    fprintf(yyout, "%s", spacing_txt);
    fprintf(yyout, "%s", text);

    free(spacing_txt);
    free(text);
    // return text;
}


void FSQLF_echo_print(FILE *yyout, size_t indent, char *txt)
{
    int length; // length of the input text string
    length = strlen(txt);

    // Adjustment for single line comments.
    // Necessary for keeping indentation and new lines right.
    int pos_last_char = length - 1; // position of last character

    // Printing of spacing is delegated to get_spacing_str(),
    // which requires as input struct fsqlf_kw_conf.
    struct fsqlf_kw_conf s = {{0, 0, 0, 0}, {0, 0, 0, 0}, 0, 0, 0, 0};

    // Delegate to get_spacing_str() printing of the new line.
    if (txt[pos_last_char] == '\n') {
        // Shorten the string - overwrite \n with \0 (end of string mark).
        txt[pos_last_char] = '\0';
        // Adjust setting used by get_spacing_str()
        s.after.new_line = 1;
    }

    // Word-vs-operator check.
    // Ensure that two adjacent words have spacing inbetween.
    s.is_word = !(length == 1 && !isalnum(txt[0]));

    // print spacing then text
    char *spacing_txt = get_spacing_str(yyout, s, indent);
    fprintf(yyout, "%s", spacing_txt);
    fputs(txt, yyout);

    free(spacing_txt);
}
