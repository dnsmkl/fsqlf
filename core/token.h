#ifndef token_h
#define token_h


typedef int token_class_t;
#define TC_KW 1
#define TC_SP 2
#define TC_CMT 3


#ifndef TOKEN_KW_SETTING_T
#error TOKEN_KW_SETTING_T not defined, but required by token.h
#endif
typedef struct
{
    token_class_t token_class;
    const char * yytext;
    int yyleng;
    TOKEN_KW_SETTING_T kw_setting;
} token_t;


token_t * make_token(
    const token_class_t token_class
    , const char * yytext
    , const int yyleng
    , const TOKEN_KW_SETTING_T kw_setting
)
{
    // TODO: Allocate new token.
    // Currently just definining interface.
    static token_t token;
    token.token_class = token_class;
    // TODO: Allocate space for string and copy it.
    token.yytext = yytext;
    token.yyleng = yyleng;
    token.kw_setting = kw_setting;
    return &token;
}


#endif
