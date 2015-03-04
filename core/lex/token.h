#ifndef token_h
#define token_h


#include "../kw/kw.h"  // kw_conf


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
//    comments, spacing, word, punctuation, string, paranthesis.
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



typedef int token_class_t;
#define TC_KW 1     // Keyword
#define TC_SP 2     // Spacing
#define TC_CMT 3    // Comment


struct token
{
    token_class_t token_class;
    char *text;
    int leng;
    const struct kw_conf *kw_setting;
    size_t indent;
};


struct token *make_token(
    const token_class_t token_class,
    const char *text,
    const int leng,
    const struct kw_conf *kw_setting,
    const size_t indent);


void delete_token(struct token **tok);


void set_token(struct token * tok,
    const token_class_t token_class,
    const char *text,
    const int leng,
    const struct kw_conf *kw_conf,
    const size_t indent);
void clear_token(struct token *tok);


#endif
