/*
Thanks to http://vsbabu.org/software/lsqlb.html
Vattekkat Satheesh Babu, on Dec 30, 1998
Helped to learn about flex a bit
*/

%top{
    /* This code goes at the "top" of the generated file. */
#include <stdio.h>      // fprintf, stdin, stdout
#include "globals.h"    // pair_stack, sub_openings, currindent, left_p, right_p
#include "tokque.h"     // tokque_putthrough
void debug_stchange(int);
void debug_match(char*);
}


%{
char * state_to_char(int);
//#define YY_USER_ACTION fprintf(yyout,"\n %10s - rule (%d) - line(%d) " ,state_to_char(YY_START),yy_act, __LINE__);
#define DMATCH(name) fprintf(yyout,"%20s is rule (%d) : ", name , yy_act);


#define BEGIN_STATE(NEWSTATE) debug_stchange(NEWSTATE); BEGIN (NEWSTATE);
#define PUSH_STATE(NEWSTATE)  stack_push(&state_stack, &(int){YY_START}); BEGIN_STATE(NEWSTATE);
#define POP_STATE(); BEGIN_STATE(*(int*)stack_peek(&state_stack)); stack_pop(&state_stack);
#define TUSE(YOUT, YTEXT, YLENG, TKW) \
do { \
    struct state_change sc = tokque_putthrough(YOUT, YTEXT, YLENG, TKW, YY_START); \
    if (sc.change_needed) { \
        BEGIN (sc.new_state); \
    } \
} while (0)

#define TUSE_DEF(TKW) \
do { \
    tokque_putthrough(yyout, yytext, yyleng, TKW, YY_START); \
} while (0)


// YY_USER_INIT is lex macro executed before initialising parser
#define YY_USER_INIT \
    stack_init(&state_stack, sizeof(int)); \
    stack_init(&sub_openings, sizeof(pair));
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
CREATE_TABLE (?i:create{SPACE}+({TABLE_OPT}{SPACE}+)*table)
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
EXISTS  (?i:exists)
IN      (?i:in)
COMPARISON (=|<>|<=|>=|<|>)

GROUPBY (?i:group{SPACE}+by)
ORDERBY (?i:order{SPACE}+by)
HAVING  (?i:having)
QUALIFY (?i:qualify)

COMMA [,]

COMMENT_ONE_LINE [-]{2,}[^\n]*[\n]
COMMENT_ONE_LINE_LAST_LINE_IN_FILE [-]{2,}[^\n]*
COMMENT_ML_START [/][*]+
COMMENT_ML_PART1 [^*]+
COMMENT_ML_PART2 [*]+[^/]
COMMENT_ML_END   [*]+[/]

STRING ([xX]?['][^'']*['])+
SEMICOLON ;

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


%option noyywrap nounput noinput

/* always-interactive - removes usage of fileno, isatty (not C99 compliant).
 * See details at:
 *     http://flex.sourceforge.net/manual/Why-do-flex-scanners-call-fileno-if-it-is-not-ANSI-compatible_003f.html
 */
%option always-interactive

%s stSELECT stFROM stWHERE stON stEXISTS stLEFTP stJOIN stIN stCOMMA stINLIST stFROM_LEFTP stP_SUB stORDERBY stGROUPBY stINSERT stINSCOLLIST stUPDATE stSET stDELETE stIN_CONSTLIST stCREATE_TABLE stTAB_COL_LIST
%x stCOMMENTML stSTRING

%%

{DELETEFROM}  { TUSE(yyout,yytext,yyleng,kw("kw_deletefrom")); }
{DELETE}      { TUSE(yyout,yytext,yyleng,kw("kw_deletefrom")); }
{INSERTINTO}  { TUSE(yyout,yytext,yyleng,kw("kw_insertinto")); }
{UPDATE}      { TUSE(yyout,yytext,yyleng,kw("kw_update")); }
<stUPDATE,stFROM>{SET} { BEGIN_STATE(stSET);TUSE_DEF(kw("kw_set") ); }
<stSET>{COMMA} { TUSE_DEF(kw("kw_comma_set")); }
                /* SET operations */

{CREATE_TABLE} { TUSE(yyout,yytext,yyleng,kw("kw_create_table")); }
{DROP}       { TUSE(yyout,yytext,yyleng,kw("kw_drop")); }
{TABLE}      { TUSE(yyout,yytext,yyleng,kw("kw_table")); }
{IFEXISTS}   { TUSE(yyout,yytext,yyleng,kw("kw_ifexists")); }
{VIEW}       { TUSE(yyout,yytext,yyleng,kw("kw_view")); }
{UNION}      { TUSE(yyout,yytext,yyleng,kw("kw_union")); }
{UNION_ALL}  { TUSE(yyout,yytext,yyleng,kw("kw_union_all")); }
{MINUS}      { TUSE(yyout,yytext,yyleng,kw("kw_minus")); }
{INTERSECT}  { TUSE(yyout,yytext,yyleng,kw("kw_intersect")); }
{EXCEPT}     { TUSE(yyout,yytext,yyleng,kw("kw_except")); }

                /* SELECT ... FROM */
<INITIAL,stINSERT>{SELECT}           {BEGIN_STATE(stSELECT); TUSE_DEF(kw("kw_select")); };
<stSELECT,stCOMMA>{COMMA}   {BEGIN_STATE(stCOMMA);  TUSE_DEF(kw("kw_comma"));  };

{IN}    { TUSE(yyout,yytext,yyleng,kw("kw_in")); }

<stSELECT,stCOMMA>{LEFTP}   {PUSH_STATE(stLEFTP );  TUSE_DEF(kw("kw_left_p")); };
<stLEFTP>{LEFTP}            {PUSH_STATE(stLEFTP ); debug_match("{LEFTP}");TUSE_DEF(kw("kw_left_p"));  };
<stLEFTP>{COMMA}            {TUSE_DEF( NULL); };
<stLEFTP>{ORDERBY}          {TUSE_DEF( NULL); };
<stLEFTP>{FROM}             {debug_match("{FROM}" ); TUSE_DEF(kw("kw_from_2"));  };
<stLEFTP>{RIGHTP}           {POP_STATE();            TUSE_DEF(kw("kw_right_p")); };
<stSELECT,stCOMMA,stUPDATE>{FROM} {BEGIN_STATE(stFROM);  TUSE_DEF(kw("kw_from"));    };
<stLEFTP,stSELECT>{AS}      {debug_match("{AS}"  );  TUSE_DEF(kw("kw_as"));      };

                /* FROM ... JOIN ... ON ... WHERE */
<stON,stFROM,stJOIN>{IJOIN} { BEGIN_STATE(stJOIN);  TUSE_DEF(kw("kw_inner_join")); };
<stON,stFROM,stJOIN>{LJOIN} { BEGIN_STATE(stJOIN);  TUSE_DEF(kw("kw_left_join") ); };
<stON,stFROM,stJOIN>{RJOIN} { BEGIN_STATE(stJOIN);  TUSE_DEF(kw("kw_right_join")); };
<stON,stFROM,stJOIN>{FJOIN} { BEGIN_STATE(stJOIN);  TUSE_DEF(kw("kw_full_join") ); };
<stON,stFROM,stJOIN>{CJOIN} { BEGIN_STATE(stJOIN);  TUSE_DEF(kw("kw_cross_join")); };
<stON,stFROM,stJOIN>{COMMA} { TUSE_DEF(kw("kw_comma_join")); };

<stJOIN>{ON}    {BEGIN_STATE(stON);   TUSE_DEF(kw("kw_on")); };



                /* WHERE ... (also join conditions) */
<stFROM,stJOIN,stON,stSET,stDELETE>{WHERE} {BEGIN_STATE(stWHERE );  TUSE_DEF(kw("kw_where")); };
<stWHERE,stON,stJOIN>{AND}  { debug_match("{AND}");  TUSE_DEF(kw("kw_and"));   };
<stWHERE,stON,stJOIN>{OR}   { debug_match("{OR}");   TUSE_DEF(kw("kw_or"));    };

<stWHERE>{EXISTS}   {TUSE_DEF(kw("kw_exists")); };


{GROUPBY}    { TUSE(yyout,yytext,yyleng,kw("kw_groupby")); }
{ORDERBY}    { TUSE(yyout,yytext,yyleng,kw("kw_orderby")); }
<stORDERBY>{COMMA}   { TUSE_DEF(kw("kw_comma_ordby")); };
<stGROUPBY>{COMMA}   { TUSE_DEF(kw("kw_comma_grpby")); };
{HAVING}     { TUSE(yyout,yytext,yyleng,kw("kw_having")); }
{QUALIFY}    { TUSE(yyout,yytext,yyleng,kw("kw_qualify")); }


<stINSERT>{LEFTP}        { PUSH_STATE(stINSCOLLIST); TUSE_DEF(kw("kw_left_p_ins") ); };
<stINSCOLLIST>{COMMA}    { TUSE_DEF(kw("kw_comma_ins") ); }
<stINSCOLLIST>{RIGHTP}   { POP_STATE();              TUSE_DEF(kw("kw_right_p_ins") ); };

<stCREATE_TABLE>{LEFTP}  { PUSH_STATE(stTAB_COL_LIST); TUSE_DEF(kw("kw_left_p_create_table") ); };
<stTAB_COL_LIST>{COMMA}    { TUSE_DEF(kw("kw_comma_create_table") ); }
<stTAB_COL_LIST>{RIGHTP}   { POP_STATE();              TUSE_DEF(kw("kw_right_p_create_table") ); };

<stP_SUB>{LEFTP}                      { BEGIN_STATE(*(int*)stack_peek(&state_stack)); TUSE_DEF(kw("kw_left_p")    ); PUSH_STATE(stP_SUB);  };
{LEFTP}                               { PUSH_STATE(stP_SUB); };
<stP_SUB>{SELECT}                     { BEGIN_STATE(stSELECT); tokque_putthrough(yyout,"(",1,kw("kw_left_p_sub"),YY_START); begin_SUB(); TUSE_DEF(kw("kw_select"));};
<stP_SUB>{NUMBER}|{STRING}|{DBOBJECT} {
    if (*(int*)stack_peek(&state_stack) == stFROM
        || *(int*)stack_peek(&state_stack) == stJOIN)
    { BEGIN_STATE(*(int*)stack_peek(&state_stack)); tokque_putthrough(yyout,"(",1,kw("kw_left_p"),YY_START    ); TUSE_DEF( NULL);}
    else
    { BEGIN_STATE(stIN_CONSTLIST); tokque_putthrough(yyout,"(",1,kw("kw_left_p"),YY_START    ); TUSE_DEF( NULL); }
    };
 /* <stP_SUB>{NUMBER}|{STRING}|{DBOBJECT} { BEGIN_STATE(stIN_CONSTLIST); tokque_putthrough(yyout,"(",1,kw("kw_left_p")    ); TUSE_DEF( NULL);}; */
<stP_SUB>{COMMENT_ML_START}           { tokque_putthrough(yyout,"",0, NULL, YY_START); PUSH_STATE(stCOMMENTML)  ; TUSE_DEF( NULL);};
<stP_SUB>{COMMENT_ONE_LINE}           { tokque_putthrough(yyout,"",0, NULL, YY_START); TUSE_DEF( NULL);};
<stP_SUB>{SPACE}                      { tokque_putthrough(yyout,"",0, NULL, YY_START); };
<stP_SUB>{RIGHTP}                     { tokque_putthrough(yyout,"(",1,kw("kw_left_p"),YY_START); POP_STATE(); TUSE_DEF(kw("kw_right_p")); }
<stP_SUB>.                            { BEGIN_STATE(*(int*)stack_peek(&state_stack)); tokque_putthrough(yyout,"(",1,kw("kw_left_p"),YY_START); TUSE_DEF( NULL); };

{RIGHTP}    {
                POP_STATE();
                if (!stack_empty(&sub_openings) &&
                    left_p -(*(pair*)stack_peek(&sub_openings)).left == (right_p+1) -(*(pair*)stack_peek(&sub_openings)).right - 1) {
                    end_SUB();
                    TUSE_DEF(kw("kw_right_p_sub"));
                } else {
                    debug_match("<wtf-leftp>");
                    TUSE_DEF(kw("kw_right_p"));
                }

            };

{CASE}  { TUSE_DEF(kw("kw_case")); }
{WHEN}  { TUSE(yyout,yytext,yyleng,kw("kw_when")); }
{THEN}  { TUSE(yyout,yytext,yyleng,kw("kw_then")); }
{ELSE}  { TUSE(yyout,yytext,yyleng,kw("kw_else")); }
{END}   { TUSE_DEF(kw("kw_end")); }

{USING} { TUSE(yyout,yytext,yyleng,kw("kw_using")); }


{COMMENT_ML_START}     {PUSH_STATE(stCOMMENTML); TUSE_DEF( NULL);};
<stCOMMENTML>{COMMENT_ML_PART1}     {debug_match("COMMENT_ML_PART1") ; TUSE_DEF( NULL);};
<stCOMMENTML>{COMMENT_ML_PART2}     {debug_match("COMMENT_ML_PART2") ; TUSE_DEF( NULL);};
<stCOMMENTML>{COMMENT_ML_END}       {POP_STATE(); TUSE_DEF( NULL);};

{COMMENT_ONE_LINE}     {TUSE_DEF( NULL);};
    /* Exeption to one-line-comment: comment on last line, without new-line after it */
{COMMENT_ONE_LINE_LAST_LINE_IN_FILE}    {TUSE_DEF( NULL);};


{STRING}     { TUSE(yyout,yytext,yyleng,NULL); }
{SPACE}+     {/* discard spaces */;};
{DBOBJECT}   { TUSE(yyout,yytext,yyleng,NULL); }
{NUMBER}     { TUSE(yyout,yytext,yyleng,NULL); }
{SEMICOLON}  { TUSE(yyout,yytext,yyleng,kw("kw_semicolon")); }
<*>.         { TUSE(yyout,yytext,yyleng,NULL); }


<<EOF>> {
            tokque_finish_out(yyout);
            fprintf(yyout,"\n");
            switch (YY_START) {
                case stCOMMENTML: fprintf(yyout,"--unterminated comment \n"); break;
                case stSTRING: fprintf(yyout,"--unterminated  string\n"); break;
                default: ;
            }
            return 0 ;
        }

%%
