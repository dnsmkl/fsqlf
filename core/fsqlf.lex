/*
Thanks to http://vsbabu.org/software/lsqlb.html
Vattekkat Satheesh Babu, on Dec 30, 1998
Helped to learn about flex a bit
*/

%top{
    /* This code goes at the "top" of the generated file. */
#include <stdio.h>
#include "global_variables.h"
#include "print_keywords.h"
#include "settings.h"
void debug_stchange(int);
void debug_match(char*);

#define ITEM_T int
#include "stack.h"
int_stack state_stack;
}


%{
char * state_to_char(int);
//#define YY_USER_ACTION fprintf(yyout,"\n %10s - rule (%d) - line(%d) " ,state_to_char(YY_START),yy_act, __LINE__);
#define DMATCH(name) fprintf(yyout,"%20s is rule (%d) : ", name , yy_act);


#define BEGIN_STATE(NEWSTATE) debug_stchange(NEWSTATE); BEGIN (NEWSTATE);
#define PUSH_STATE(NEWSTATE)  int_stack_push(&state_stack, YY_START); BEGIN_STATE(NEWSTATE);
#define POP_STATE(); BEGIN_STATE(int_stack_peek(&state_stack)); int_stack_pop(&state_stack);

// YY_USER_INIT is lex macro executed before initialising parser
#define YY_USER_INIT \
    int_stack_init(&state_stack); \
    pair_stack_init(&sub_openings);
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

CREATE  (?i:create)
DROP    (?i:drop)
TABLE   (?i:table)
VIEW    (?i:view)
IFEXISTS (?i:if{SPACE}+exists)
SELECT  (?i:select|sel)
AS      (?i:as)
FROM    (?i:from)

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

STRING ([xX]?['][^']*['])+
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

%s stSELECT stFROM stWHERE stON stEXISTS stLEFTP stJOIN stIN stCOMMA stINLIST stFROM_LEFTP stP_SUB stORDERBY stGROUPBY stINSERT stINSCOLLIST stUPDATE stSET stDELETE stIN_CONSTLIST
%x stCOMMENTML stSTRING

%%

{DELETEFROM}  { BEGIN_STATE(stDELETE);handle_kw(yyout,yytext,kw_deletefrom); }
{DELETE}      { BEGIN_STATE(stDELETE);handle_kw(yyout,yytext,kw_deletefrom); }
{INSERTINTO}  { BEGIN_STATE(stINSERT);handle_kw(yyout,yytext,kw_insertinto); }
{UPDATE}      { BEGIN_STATE(stUPDATE);handle_kw(yyout,yytext,kw_update); }
<stUPDATE,stFROM>{SET} { BEGIN_STATE(stSET);handle_kw(yyout,yytext,kw_set); }
<stSET>{COMMA} { handle_kw(yyout,yytext,kw_comma_set); }
                /* SET operations */
{CREATE}     {BEGIN_STATE(INITIAL);handle_kw(yyout,yytext,kw_create)   ; };
{DROP}       {BEGIN_STATE(INITIAL);handle_kw(yyout,yytext,kw_drop)     ; };
{TABLE}      {BEGIN_STATE(INITIAL);handle_kw(yyout,yytext,kw_table)    ; };
{IFEXISTS}   {BEGIN_STATE(INITIAL);handle_kw(yyout,yytext,kw_ifexists) ; };
{VIEW}       {BEGIN_STATE(INITIAL);handle_kw(yyout,yytext,kw_view)     ; };
{UNION}      {BEGIN_STATE(INITIAL);handle_kw(yyout,yytext,kw_union)    ; };
{UNION_ALL}  {BEGIN_STATE(INITIAL);handle_kw(yyout,yytext,kw_union_all); };
{MINUS}      {BEGIN_STATE(INITIAL);handle_kw(yyout,yytext,kw_minus);     };
{INTERSECT}  {BEGIN_STATE(INITIAL);handle_kw(yyout,yytext,kw_intersect); };
{EXCEPT}     {BEGIN_STATE(INITIAL);handle_kw(yyout,yytext,kw_except);    };

                /* SELECT ... FROM */
<INITIAL,stINSERT>{SELECT}           {BEGIN_STATE(stSELECT); handle_kw(yyout,yytext,kw_select); };
<stSELECT,stCOMMA>{COMMA}   {BEGIN_STATE(stCOMMA);  handle_kw(yyout,yytext,kw_comma);  };

{IN}    { handle_kw(yyout,yytext,kw_in); };

<stSELECT,stCOMMA>{LEFTP}   {PUSH_STATE(stLEFTP );  handle_kw(yyout,yytext,kw_left_p); };
<stLEFTP>{LEFTP}            {PUSH_STATE(stLEFTP ); debug_match("{LEFTP}");handle_kw(yyout,yytext,kw_left_p);  };
<stLEFTP>{COMMA}            {handle_text(yyout,yytext); };
<stLEFTP>{ORDERBY}          {handle_text(yyout,yytext); };
<stLEFTP>{FROM}             {debug_match("{FROM}" ); handle_kw(yyout,yytext,kw_from_2);  };
<stLEFTP>{RIGHTP}           {POP_STATE();            handle_kw(yyout,yytext,kw_right_p); };
<stSELECT,stCOMMA,stUPDATE>{FROM} {BEGIN_STATE(stFROM);  handle_kw(yyout,yytext,kw_from);    };
<stLEFTP,stSELECT>{AS}      {debug_match("{AS}"  );  handle_kw(yyout,yytext,kw_as);      };

                /* FROM ... JOIN ... ON ... WHERE */
<stON,stFROM,stJOIN>{IJOIN} { BEGIN_STATE(stJOIN);  handle_kw(yyout,yytext,kw_inner_join); };
<stON,stFROM,stJOIN>{LJOIN} { BEGIN_STATE(stJOIN);  handle_kw(yyout,yytext,kw_left_join ); };
<stON,stFROM,stJOIN>{RJOIN} { BEGIN_STATE(stJOIN);  handle_kw(yyout,yytext,kw_right_join); };
<stON,stFROM,stJOIN>{FJOIN} { BEGIN_STATE(stJOIN);  handle_kw(yyout,yytext,kw_full_join ); };
<stON,stFROM,stJOIN>{CJOIN} { BEGIN_STATE(stJOIN);  handle_kw(yyout,yytext,kw_cross_join); };
<stON,stFROM,stJOIN>{COMMA} { handle_kw(yyout,yytext,kw_comma_join); };

<stJOIN>{ON}    {BEGIN_STATE(stON);   handle_kw(yyout,yytext,kw_on); };



                /* WHERE ... (also join conditions) */
<stFROM,stJOIN,stON,stSET,stDELETE>{WHERE} {BEGIN_STATE(stWHERE );  handle_kw(yyout,yytext,kw_where); };
<stWHERE,stON,stJOIN>{AND}  { debug_match("{AND}");  handle_kw(yyout,yytext,kw_and);   };
<stWHERE,stON,stJOIN>{OR}   { debug_match("{OR}");   handle_kw(yyout,yytext,kw_or);    };

<stWHERE>{EXISTS}   {handle_kw(yyout,yytext,kw_exists); };


{GROUPBY}    {BEGIN_STATE(stGROUPBY); handle_kw(yyout,yytext,kw_groupby); };
{ORDERBY}    {BEGIN_STATE(stORDERBY); handle_kw(yyout,yytext,kw_orderby); };
<stORDERBY>{COMMA}   { handle_kw(yyout,yytext,kw_comma_ordby); };
<stGROUPBY>{COMMA}   { handle_kw(yyout,yytext,kw_comma_grpby); };
{HAVING}     {BEGIN_STATE(stWHERE); handle_kw(yyout,yytext,kw_having);  };
{QUALIFY}    {BEGIN_STATE(stWHERE); handle_kw(yyout,yytext,kw_qualify); };


<stINSERT>{LEFTP}        { PUSH_STATE(stINSCOLLIST); handle_kw(yyout,yytext,kw_left_p_ins ); };
<stINSCOLLIST>{COMMA}    { handle_kw(yyout,yytext,kw_comma_ins ); }
<stINSCOLLIST>{RIGHTP}   { POP_STATE();              handle_kw(yyout,yytext,kw_right_p_ins ); };

<stP_SUB>{LEFTP}                      { BEGIN_STATE(int_stack_peek(&state_stack)); handle_kw(yyout,yytext,kw_left_p    ); PUSH_STATE(stP_SUB);  };
{LEFTP}                               { PUSH_STATE(stP_SUB); };
<stP_SUB>{SELECT}                     { BEGIN_STATE(stSELECT);     handle_kw(yyout,"(",kw_left_p_sub); handle_kw(yyout,yytext,kw_select);};
<stP_SUB>{NUMBER}|{STRING}|{DBOBJECT} {
    if( int_stack_peek(&state_stack) == stFROM
        || int_stack_peek(&state_stack) == stJOIN )
    { BEGIN_STATE(int_stack_peek(&state_stack)); handle_kw(yyout,"(",kw_left_p    ); handle_text(yyout,yytext);}
    else
    { BEGIN_STATE(stIN_CONSTLIST); handle_kw(yyout,"(",kw_left_p    ); handle_text(yyout,yytext); }
    };
 /* <stP_SUB>{NUMBER}|{STRING}|{DBOBJECT} { BEGIN_STATE(stIN_CONSTLIST); handle_kw(yyout,"(",kw_left_p    ); handle_text(yyout,yytext);}; */
<stP_SUB>{COMMENT_ML_START}           { handle_text(yyout,""); PUSH_STATE(stCOMMENTML)  ; handle_text(yyout,yytext);};
<stP_SUB>{COMMENT_ONE_LINE}           { handle_text(yyout,""); handle_text(yyout,yytext);};
<stP_SUB>{SPACE}                      { handle_text(yyout,""); };
<stP_SUB>{RIGHTP}                     { handle_kw(yyout,"(",kw_left_p    ); POP_STATE(); handle_kw(yyout,yytext,kw_right_p); }
<stP_SUB>.                            { BEGIN_STATE(int_stack_peek(&state_stack)); handle_kw(yyout,"(",kw_left_p    ); handle_text(yyout,yytext); };

{RIGHTP}    {
                POP_STATE();
                if(!pair_stack_empty(&sub_openings)
                    && left_p - pair_stack_peek(&sub_openings).left == (right_p+1) - pair_stack_peek(&sub_openings).right - 1 ){
                    handle_kw(yyout,yytext,kw_right_p_sub);
                } else {
                    debug_match("<wtf-leftp>");
                    handle_kw(yyout,yytext,kw_right_p);
                }

            };

{CASE}  { handle_kw(yyout,yytext,kw_case); currindent++;}
{WHEN}  { handle_kw(yyout,yytext,kw_when); }
{THEN}  { handle_kw(yyout,yytext,kw_then); }
{ELSE}  { handle_kw(yyout,yytext,kw_else); }
{END}   { currindent--; handle_kw(yyout,yytext,kw_end); }



{COMMENT_ML_START}     {PUSH_STATE(stCOMMENTML); handle_text(yyout,yytext);};
<stCOMMENTML>{COMMENT_ML_PART1}     {debug_match("COMMENT_ML_PART1") ; handle_text(yyout,yytext);};
<stCOMMENTML>{COMMENT_ML_PART2}     {debug_match("COMMENT_ML_PART2") ; handle_text(yyout,yytext);};
<stCOMMENTML>{COMMENT_ML_END}       {POP_STATE(); handle_text(yyout,yytext);};

{COMMENT_ONE_LINE}     {handle_text(yyout,yytext);};
    /* Exeption to one-line-comment: comment on last line, without new-line after it */
{COMMENT_ONE_LINE_LAST_LINE_IN_FILE}    {handle_text(yyout,yytext);};


{STRING}     {handle_text(yyout,yytext);};

{SPACE}+     {/* discard spaces */;};
{DBOBJECT}   {handle_text(yyout,yytext);};
{NUMBER}     {handle_text(yyout,yytext);};
{SEMICOLON}  {BEGIN_STATE(INITIAL); handle_kw(yyout,yytext,kw_semicolon);};
<*>.         {debug_match("<*>."); handle_text(yyout,yytext); };


<<EOF>> {
            fprintf(yyout,"\n");
            switch(YY_START){
                case stCOMMENTML: fprintf(yyout,"--unterminated comment \n"); break;
                case stSTRING: fprintf(yyout,"--unterminated  string\n"); break;
                default: ;
            }
            return 0 ;
        }

%%





#include "debuging.h"
#include "cli.h"

int main(int argc, char **argv)
{
    // initialise with STD I/O, later changed by command line options (if any)
    yyin  = stdin;
    yyout = stdout;

    init_all_settings();            // init default configs
    read_configs();                 // read configs from file
    read_cli_options(argc,argv);    // read configs from command line

    while (yylex () != 0) ;

    return 0;
}

