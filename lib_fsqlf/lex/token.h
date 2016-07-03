#ifndef token_h
#define token_h


#include <lib_fsqlf.h>  // kw_conf, struct FSQLF_prev_kw prev


// Currently token.h is not used, but there is a plan for it.

// GOAL
// is make it possible to specify syntax at runtime.
// This could be split into some milestones:
// A. Keywords that require only trivial spacing (before, after).
// B. Keywords that affect spacing of other keywords (global indent)
// C. things like string definition
//      (motivation for this: c-like-escaped-quote - '\'' is valid
//       string literal in mysql, while unfinished string in some others)

// OBSTACLE
// at the moment keyword recognition is done completely
// by flex-generated lexer and it requires to know all definitions
// at lexer generation time (this means even before compilation).

// PLAN
// to achieve it:
//  - Reduce responsibility of flex lexer to recognizing only 'token classes':
//    comments, spacing, word, punctuation, string, parenthesis.
//  - Further recognition of word tokens (and possibly punctuation),
//    will be done outside of flex-generated lexer (lets say in quasiparser).
//  - Quasiparser will:
//      1. Accept the token with it's class
//      2. Place it on the queue
//      3. Perform recognition
//          (if lookahead is needed, it hold processing
//          until enough lookahead available)
//      4. Remove recognized tokens from the queue and pass them to printing
//          routines.

// Additional notes:
// - no additional external dependencies are allowed
//      (it is needed to use fsqlf at work and can not add external deps)
// - using flex generated lexer possibly gave quite good performance,
//      it would be strange to expect that performance will stay as is
//      after these changes.
//      Though goal is to preserve usablity of fsqlf (too slow == unusable).



enum FSQLF_token_class
{
    FSQLF_TOKEN_CLASS_TXT = 0,  // Non-keyword text
    FSQLF_TOKEN_CLASS_KW = 1,   // Keyword
    FSQLF_TOKEN_CLASS_SP = 2,   // Spacing
    FSQLF_TOKEN_CLASS_CMT = 3   // Comment
};


struct FSQLF_token
{
    enum FSQLF_token_class token_class;
    char *text;
    int leng;
    struct fsqlf_kw_conf *kw_setting;
    size_t indent;
};


struct FSQLF_token *FSQLF_make_token(
    const enum FSQLF_token_class token_class,
    const char *text,
    const int leng,
    struct fsqlf_kw_conf *kw_setting,
    const size_t indent);


void FSQLF_delete_token(struct FSQLF_token **tok);


void FSQLF_set_token(struct FSQLF_token * tok,
    const enum FSQLF_token_class token_class,
    const char *text,
    const int leng,
    struct fsqlf_kw_conf *kw_conf,
    const size_t indent);


void FSQLF_clear_token(struct FSQLF_token *tok);


#endif
