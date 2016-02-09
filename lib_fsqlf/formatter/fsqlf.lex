/*
Thanks to http://vsbabu.org/software/lsqlb.html
Vattekkat Satheesh Babu, on Dec 30, 1998
Helped to learn about flex a bit
*/

%top{
// This code goes at the "top" of the generated file.
// Also it gets into generated header file.
#include <stdio.h>      // fprintf, stdin, stdout
#include "globals.h"    // pair_stack, FSQLF_sub_openings, currindent, left_p, right_p
#include "tokque.h"     // FSQLF_tokque_putthrough

// Actual formatter
// (only needed to hide private formatter's arguments)
extern int fsqlf_format_file();

// Private formatter (flex lexer)
// It accepts various paramters only to avoid global variables.
extern int FSQLF_flex(int currindent, int left_p, int right_p);
#define YY_DECL int FSQLF_flex(int currindent, int left_p, int right_p)


}


%{
// This does not get into generated header file.


// YY_USER_INIT is lex macro executed before initialising parser
// It run inside of FSQLF_flex.
#define YY_USER_INIT \
    FSQLF_stack_init(&FSQLF_state_stack, sizeof(int)); \
    FSQLF_stack_init(&FSQLF_sub_openings, sizeof(pair)); \
    currindent = 0; \
    left_p = 0; \
    right_p = 0;


int fsqlf_format_bytes(const char *bytes, int len)
{
    yy_scan_bytes(bytes, len);
    return FSQLF_flex(0, 0, 0);
}


int fsqlf_format_file(FILE *fin, FILE *fout)
{
    yyin = fin; // FIXME: does not seem like thread safe.
    yyout = fout; // FIXME: does not seem like thread safe.
    return FSQLF_flex(0, 0, 0);
}


#define BEGIN_STATE(NEWSTATE) BEGIN (NEWSTATE);

#define PUSH_STATE(NEWSTATE) \
do { \
    FSQLF_stack_push(&FSQLF_state_stack, &(int){YY_START}); \
    BEGIN_STATE(NEWSTATE); \
} while (0)

#define POP_STATE() \
do { \
    BEGIN_STATE(*(int*)FSQLF_stack_peek(&FSQLF_state_stack)); \
    FSQLF_stack_pop(&FSQLF_state_stack); \
} while (0)

// Use KW with ability to change state.
#define TUSE_W_STATES(TKW) \
do { \
    struct FSQLF_state_change sc = FSQLF_tokque_putthrough( \
        yyout, &currindent, yytext, yyleng, TKW, YY_START \
    ); \
    if (sc.state_change_action == FSQLF_SCA_BEGIN) { \
        BEGIN (sc.new_state); \
    } \
} while (0)

// Use KW without ability to change state.
#define TUSE_SIMPLE(TKW) \
do { \
    FSQLF_tokque_putthrough(yyout, &currindent,  yytext, yyleng, TKW, YY_START); \
} while (0)

%}


DIGIT   [0-9]
NUMBER  {DIGIT}+([.]{DIGIT}+)?([eE][+-]?{DIGIT}+)?
ID      [A-Za-z_][A-Za-z0-9_]*
SPACE   [ \t\n]
DBOBJECT    ({ID}[.]){0,2}{ID}


LEFTP   [(]
RIGHTP  [)]

UNION     (?i:union)
UNION_ALL (?i:union{SPACE}+all)
INTERSECT (?i:intersect)
EXCEPT    (?i:except)
MINUS     (?i:minus)

TABLE_OPT (?i:global|volatile|set|multiset|temporary)
CREATE  (?i:create)
DROP    (?i:drop)
TABLE   (?i:table)
VIEW    (?i:view)
IFEXISTS (?i:if{SPACE}+exists)
SELECT  (?i:select|sel)
AS      (?i:as)
FROM    (?i:from)
USING   (?i:using)

IJOIN   (?i:(inner{SPACE}+)?JOIN)
LJOIN   (?i:left{SPACE}+(OUTER{SPACE}+)?JOIN)
RJOIN   (?i:right{SPACE}+(OUTER{SPACE}+)?JOIN)
FJOIN   (?i:full{SPACE}+(OUTER{SPACE}+)?JOIN)
CJOIN   (?i:cross{SPACE}+JOIN{SPACE}+)
ANYJOIN ({LJOIN}|{RJOIN}|{FJOIN}|{CJOIN})

ON      (?i:on)
WHERE   (?i:where)
SAMPLE  (?i:sample)
AND     (?i:and)
OR      (?i:or)
NOT     (?i:not)
EXISTS  (?i:exists)
IN      (?i:in)
LIKE    (?i:like)

GROUPBY (?i:group{SPACE}+by)
ORDERBY (?i:order{SPACE}+by)
HAVING  (?i:having)
QUALIFY (?i:qualify)

COMMA [,]
COMP_EQ (=)
COMP_NE (<>)
COMP_LE (<=)
COMP_GE (>=)
COMP_LT (<)
COMP_GT (>)

COMMENT_ONE_LINE [-]{2,}[^\n]*[\n]
COMMENT_ONE_LINE_LAST_LINE_IN_FILE [-]{2,}[^\n]*
COMMENT_ML_START [/][*]+
COMMENT_ML_PART1 [^*]+
COMMENT_ML_PART2 [*]+[^/]
COMMENT_ML_END   [*]+[/]

STRING ([xX]?['][^'']*['])+
SEMICOLON ;
OP_PLUS   (\+)
OP_MINUS  (-)
OP_MULT   (\*)
OP_DIV    (\/)
OP_CONCAT (\|\|)


INSERTINTO (?i:(ins|insert){SPACE}+into)
UPDATE (?i:upd|update)
SET (?i:set)
DELETEFROM (?i:(del|delete){SPACE}+from)
DELETE (?i:(del|delete))

CASE (?i:case)
WHEN (?i:when)
THEN (?i:then)
ELSE (?i:else)
END (?i:end)


%option noyywrap
%option nounput
%option noinput
/* always-interactive - removes usage of fileno, isatty (not C99 compliant).
 * See details at:
 *     http://flex.sourceforge.net/manual/Why-do-flex-scanners-call-fileno-if-it-is-not-ANSI-compatible_003f.html
 */
%option always-interactive

%s stSELECT stFROM stWHERE stON stEXISTS stLEFTP stJOIN stIN stCOMMA stINLIST stFROM_LEFTP stP_SUB stORDERBY stGROUPBY stINSERT stINSCOLLIST stUPDATE stSET stDELETE stIN_CONSTLIST stCREATE stTAB_COL_LIST
%x stCOMMENTML stSTRING

%%

{DELETEFROM}  { TUSE_W_STATES(fsqlf_kw_get("kw_deletefrom")); }
{DELETE}      { TUSE_W_STATES(fsqlf_kw_get("kw_deletefrom")); }
{INSERTINTO}  { TUSE_W_STATES(fsqlf_kw_get("kw_insertinto")); }
{UPDATE}      { TUSE_W_STATES(fsqlf_kw_get("kw_update")); }
<stUPDATE,stFROM>{SET} { BEGIN_STATE(stSET);TUSE_SIMPLE(fsqlf_kw_get("kw_set") ); }
<stSET>{COMMA} { TUSE_SIMPLE(fsqlf_kw_get("kw_comma_set")); }
                /* SET operations */

{CREATE}     { TUSE_W_STATES(fsqlf_kw_get("kw_create")); }
{DROP}       { TUSE_W_STATES(fsqlf_kw_get("kw_drop")); }
{TABLE}      { TUSE_W_STATES(fsqlf_kw_get("kw_table")); }
{IFEXISTS}   { TUSE_W_STATES(fsqlf_kw_get("kw_ifexists")); }
{VIEW}       { TUSE_W_STATES(fsqlf_kw_get("kw_view")); }
{UNION}      { TUSE_W_STATES(fsqlf_kw_get("kw_union")); }
{UNION_ALL}  { TUSE_W_STATES(fsqlf_kw_get("kw_union_all")); }
{MINUS}      { TUSE_W_STATES(fsqlf_kw_get("kw_minus")); }
{INTERSECT}  { TUSE_W_STATES(fsqlf_kw_get("kw_intersect")); }
{EXCEPT}     { TUSE_W_STATES(fsqlf_kw_get("kw_except")); }

                /* SELECT ... FROM */
<INITIAL,stINSERT>{SELECT}  {BEGIN_STATE(stSELECT); TUSE_SIMPLE(fsqlf_kw_get("kw_select")); };
<stSELECT,stCOMMA>{COMMA}   {BEGIN_STATE(stCOMMA);  TUSE_SIMPLE(fsqlf_kw_get("kw_comma"));  };

{IN}    { TUSE_W_STATES(fsqlf_kw_get("kw_in")); }
{LIKE}  { TUSE_SIMPLE(fsqlf_kw_get("kw_like")); };

{COMP_EQ}    { TUSE_SIMPLE(fsqlf_kw_get("kw_comp_eq")); };
{COMP_NE}    { TUSE_SIMPLE(fsqlf_kw_get("kw_comp_ne")); };
{COMP_LE}    { TUSE_SIMPLE(fsqlf_kw_get("kw_comp_le")); };
{COMP_GE}    { TUSE_SIMPLE(fsqlf_kw_get("kw_comp_ge")); };
{COMP_LT}    { TUSE_SIMPLE(fsqlf_kw_get("kw_comp_lt")); };
{COMP_GT}    { TUSE_SIMPLE(fsqlf_kw_get("kw_comp_gt")); };

<stSELECT,stCOMMA>{LEFTP}   {PUSH_STATE(stLEFTP ); TUSE_SIMPLE(fsqlf_kw_get("kw_left_p")); left_p++; };
<stLEFTP>{LEFTP}            {PUSH_STATE(stLEFTP ); TUSE_SIMPLE(fsqlf_kw_get("kw_left_p")); left_p++; };
<stLEFTP>{COMMA}            {TUSE_SIMPLE( NULL); };
<stLEFTP>{ORDERBY}          {TUSE_SIMPLE( NULL); };
<stLEFTP>{FROM}             {TUSE_SIMPLE(fsqlf_kw_get("kw_from_2"));  };
<stLEFTP>{RIGHTP}           {POP_STATE(); TUSE_SIMPLE(fsqlf_kw_get("kw_right_p")); right_p++; };
<stSELECT,stCOMMA,stUPDATE>{FROM} {BEGIN_STATE(stFROM);  TUSE_SIMPLE(fsqlf_kw_get("kw_from"));    };
<stLEFTP,stSELECT>{AS}      {TUSE_SIMPLE(fsqlf_kw_get("kw_as"));      };


                /* FROM ... JOIN ... ON ... WHERE */
<stON,stFROM,stJOIN>{IJOIN} { BEGIN_STATE(stJOIN);  TUSE_SIMPLE(fsqlf_kw_get("kw_inner_join")); };
<stON,stFROM,stJOIN>{LJOIN} { BEGIN_STATE(stJOIN);  TUSE_SIMPLE(fsqlf_kw_get("kw_left_join") ); };
<stON,stFROM,stJOIN>{RJOIN} { BEGIN_STATE(stJOIN);  TUSE_SIMPLE(fsqlf_kw_get("kw_right_join")); };
<stON,stFROM,stJOIN>{FJOIN} { BEGIN_STATE(stJOIN);  TUSE_SIMPLE(fsqlf_kw_get("kw_full_join") ); };
<stON,stFROM,stJOIN>{CJOIN} { BEGIN_STATE(stJOIN);  TUSE_SIMPLE(fsqlf_kw_get("kw_cross_join")); };
<stON,stFROM,stJOIN>{COMMA} { TUSE_SIMPLE(fsqlf_kw_get("kw_comma_join")); };

<stJOIN>{ON}    {BEGIN_STATE(stON);   TUSE_SIMPLE(fsqlf_kw_get("kw_on")); };



                /* WHERE ... (also join conditions) */
<stFROM,stJOIN,stON,stSET,stDELETE>{WHERE} {BEGIN_STATE(stWHERE );  TUSE_SIMPLE(fsqlf_kw_get("kw_where")); };
<stWHERE,stON,stJOIN>{AND}  { TUSE_SIMPLE(fsqlf_kw_get("kw_and"));   };
<stWHERE,stON,stJOIN>{OR}   { TUSE_SIMPLE(fsqlf_kw_get("kw_or"));    };
{NOT}    { TUSE_SIMPLE(fsqlf_kw_get("kw_not")); };

<stWHERE>{EXISTS}   {TUSE_SIMPLE(fsqlf_kw_get("kw_exists")); };


{GROUPBY}    { TUSE_W_STATES(fsqlf_kw_get("kw_groupby")); }
{ORDERBY}    { TUSE_W_STATES(fsqlf_kw_get("kw_orderby")); }
<stORDERBY>{COMMA}   { TUSE_SIMPLE(fsqlf_kw_get("kw_comma_ordby")); };
<stGROUPBY>{COMMA}   { TUSE_SIMPLE(fsqlf_kw_get("kw_comma_grpby")); };
{HAVING}     { TUSE_W_STATES(fsqlf_kw_get("kw_having")); }
{QUALIFY}    { TUSE_W_STATES(fsqlf_kw_get("kw_qualify")); }


<stINSERT>{LEFTP}        { PUSH_STATE(stINSCOLLIST); TUSE_SIMPLE(fsqlf_kw_get("kw_left_p_ins") ); };
<stINSCOLLIST>{COMMA}    { TUSE_SIMPLE(fsqlf_kw_get("kw_comma_ins") ); }
<stINSCOLLIST>{RIGHTP}   { POP_STATE();              TUSE_SIMPLE(fsqlf_kw_get("kw_right_p_ins") ); };

<stCREATE>{LEFTP}  { PUSH_STATE(stTAB_COL_LIST); TUSE_SIMPLE(fsqlf_kw_get("kw_left_p_create") ); };
<stTAB_COL_LIST>{COMMA}    { TUSE_SIMPLE(fsqlf_kw_get("kw_comma_create") ); }
<stTAB_COL_LIST>{RIGHTP}   { POP_STATE();              TUSE_SIMPLE(fsqlf_kw_get("kw_right_p_create") ); };

<stP_SUB>{LEFTP}   {
    BEGIN_STATE(*(int*)FSQLF_stack_peek(&FSQLF_state_stack));
    TUSE_SIMPLE(fsqlf_kw_get("kw_left_p"));
    left_p++;
    PUSH_STATE(stP_SUB);  };
{LEFTP}            { PUSH_STATE(stP_SUB); };
<stP_SUB>{SELECT}  {
        BEGIN_STATE(stSELECT);
        FSQLF_tokque_putthrough(yyout, &currindent, "(", 1, fsqlf_kw_get("kw_left_p_sub"), YY_START);
        FSQLF_stack_push(&FSQLF_sub_openings, &(pair){left_p, right_p}); // begin sub
        currindent++; // begin sub
        TUSE_SIMPLE(fsqlf_kw_get("kw_select"));
    };
<stP_SUB>{NUMBER}|{STRING}|{DBOBJECT} {
        if (*(int*)FSQLF_stack_peek(&FSQLF_state_stack) == stFROM
                || *(int*)FSQLF_stack_peek(&FSQLF_state_stack) == stJOIN) {
            BEGIN_STATE(*(int*)FSQLF_stack_peek(&FSQLF_state_stack));
            FSQLF_tokque_putthrough(yyout, &currindent, "(", 1, fsqlf_kw_get("kw_left_p"), YY_START);
            left_p++;
            TUSE_SIMPLE(NULL);
        } else {
            BEGIN_STATE(stIN_CONSTLIST);
            FSQLF_tokque_putthrough(yyout, &currindent, "(", 1, fsqlf_kw_get("kw_left_p"), YY_START);
            left_p++;
            TUSE_SIMPLE(NULL);
        }
    };
<stP_SUB>{COMMENT_ML_START}           { FSQLF_tokque_putthrough(yyout, &currindent, "", 0,  NULL, YY_START); PUSH_STATE(stCOMMENTML)  ; TUSE_SIMPLE( NULL);};
<stP_SUB>{COMMENT_ONE_LINE}           { FSQLF_tokque_putthrough(yyout, &currindent, "", 0,  NULL, YY_START); TUSE_SIMPLE( NULL);};
<stP_SUB>{SPACE}                      { FSQLF_tokque_putthrough(yyout, &currindent, "", 0,  NULL, YY_START); };
<stP_SUB>{RIGHTP}  {
        FSQLF_tokque_putthrough(yyout, &currindent, "(", 1, fsqlf_kw_get("kw_left_p"), YY_START);
        left_p++;
        POP_STATE();
        TUSE_SIMPLE(fsqlf_kw_get("kw_right_p"));
        right_p++;
    }
<stP_SUB>. {
        BEGIN_STATE(*(int*)FSQLF_stack_peek(&FSQLF_state_stack));
        FSQLF_tokque_putthrough(yyout, &currindent, "(", 1, fsqlf_kw_get("kw_left_p"), YY_START);
        left_p++;
        TUSE_SIMPLE(NULL);
    };

{RIGHTP}    {
                POP_STATE();
                if (!FSQLF_stack_empty(&FSQLF_sub_openings) &&
                    left_p -(*(pair*)FSQLF_stack_peek(&FSQLF_sub_openings)).left == (right_p+1) -(*(pair*)FSQLF_stack_peek(&FSQLF_sub_openings)).right - 1) {
                    FSQLF_stack_pop(&FSQLF_sub_openings); // end sub
                    currindent--; // end sub
                    TUSE_SIMPLE(fsqlf_kw_get("kw_right_p_sub"));
                } else {
                    TUSE_SIMPLE(fsqlf_kw_get("kw_right_p"));
                    right_p++;
                }

            };

{CASE}  { TUSE_SIMPLE(fsqlf_kw_get("kw_case")); }
{WHEN}  { TUSE_W_STATES(fsqlf_kw_get("kw_when")); }
{THEN}  { TUSE_W_STATES(fsqlf_kw_get("kw_then")); }
{ELSE}  { TUSE_W_STATES(fsqlf_kw_get("kw_else")); }
{END}   { TUSE_SIMPLE(fsqlf_kw_get("kw_end")); }

{USING} { TUSE_W_STATES(fsqlf_kw_get("kw_using")); }


{COMMENT_ML_START}     {PUSH_STATE(stCOMMENTML); TUSE_SIMPLE( NULL);};
<stCOMMENTML>{COMMENT_ML_PART1}     { TUSE_SIMPLE( NULL);};
<stCOMMENTML>{COMMENT_ML_PART2}     { TUSE_SIMPLE( NULL);};
<stCOMMENTML>{COMMENT_ML_END}       {POP_STATE(); TUSE_SIMPLE( NULL);};

{COMMENT_ONE_LINE}     {TUSE_SIMPLE( NULL);};
    /* Exeption to one-line-comment: comment on last line, without new-line after it */
{COMMENT_ONE_LINE_LAST_LINE_IN_FILE}    {TUSE_SIMPLE( NULL);};


{STRING}     { TUSE_W_STATES(NULL); }
{SPACE}+     {/* discard spaces */;};
{DBOBJECT}   { TUSE_W_STATES(NULL); }
{NUMBER}     { TUSE_W_STATES(NULL); }
{SEMICOLON}  { TUSE_W_STATES(fsqlf_kw_get("kw_semicolon")); }
{OP_PLUS}    { TUSE_SIMPLE(fsqlf_kw_get("kw_op_plus")); };
{OP_MINUS}   { TUSE_SIMPLE(fsqlf_kw_get("kw_op_minus")); };
{OP_MULT}    { TUSE_SIMPLE(fsqlf_kw_get("kw_op_mult")); };
{OP_DIV}     { TUSE_SIMPLE(fsqlf_kw_get("kw_op_div")); };
{OP_CONCAT}  { TUSE_SIMPLE(fsqlf_kw_get("kw_op_concat")); };
<*>.         { TUSE_W_STATES(NULL); }


<<EOF>> {
            FSQLF_tokque_finish_out(yyout);
            fprintf(yyout,"\n");
            switch (YY_START) {
                case stCOMMENTML: fprintf(yyout,"--unterminated comment \n"); break;
                case stSTRING: fprintf(yyout,"--unterminated  string\n"); break;
                default: ;
            }
            return 0 ;
        }

%%
