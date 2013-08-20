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
}


%{
char * state_to_char(int);
//#define YY_USER_ACTION fprintf(yyout,"\n %10s - rule (%d) - line(%d) " ,state_to_char(YY_START),yy_act, __LINE__);
#define DMATCH(name) fprintf(yyout,"%20s is rule (%d) : ", name , yy_act);


#define BEGIN_STATE(NEWSTATE) debug_stchange(NEWSTATE); BEGIN (NEWSTATE);
#define PUSH_STATE(NEWSTATE)  push_stack(YY_START); /*printf("\nPUSH");*/ BEGIN_STATE(NEWSTATE);
#define POP_STATE(); /*printf("\nPOP");*/ BEGIN_STATE(peek_stack()); pop_stack();

// YY_USER_INIT is lex macro executed before initialising parser
#define YY_USER_INIT

%}


DIGIT   [0-9]
NUMBER  {DIGIT}+([.]{DIGIT}+)?
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
COMMENT_ML_START [/][*]+
COMMENT_ML_PART1 [^*]+
COMMENT_ML_PART2 [*]+[^/]
COMMENT_ML_END   [*]+[/]

STRING (['][^']*['])+
SEMICOLON ;

INSERTINTO (?i:(ins|insert){SPACE}+into)
UPDATE (?i:upd|update)
SET (?i:set)
DELETEFROM (?i:(del|delete){SPACE}+from)


%option noyywrap nounput noinput

%s stSELECT stFROM stWHERE stON stEXISTS stLEFTP stJOIN stIN stCOMMA stINLIST stFROM_LEFTP stP_SUB stORDERBY stGROUPBY stINSERT stINSCOLLIST stUPDATE stSET stDELETE stIN_CONSTLIST
%x stCOMMENTML stSTRING

%%

{DELETEFROM}  { BEGIN_STATE(stDELETE);kw_print(yyout,yytext,kw_deletefrom); }
{INSERTINTO}  { BEGIN_STATE(stINSERT);kw_print(yyout,yytext,kw_insertinto); }
{UPDATE}      { BEGIN_STATE(stUPDATE);kw_print(yyout,yytext,kw_update); }
<stUPDATE,stFROM>{SET} { BEGIN_STATE(stSET);kw_print(yyout,yytext,kw_set); }
<stSET>{COMMA} { kw_print(yyout,yytext,kw_comma_set); }
                /* SET operations */
{UNION}      {BEGIN_STATE(INITIAL);kw_print(yyout,yytext,kw_union)    ; };
{UNION_ALL}  {BEGIN_STATE(INITIAL);kw_print(yyout,yytext,kw_union_all); };
{MINUS}      {BEGIN_STATE(INITIAL);kw_print(yyout,yytext,kw_minus);     };
{INTERSECT}  {BEGIN_STATE(INITIAL);kw_print(yyout,yytext,kw_intersect); };
{EXCEPT}     {BEGIN_STATE(INITIAL);kw_print(yyout,yytext,kw_except);    };

                /* SELECT ... FROM */
<INITIAL,stINSERT>{SELECT}           {BEGIN_STATE(stSELECT); kw_print(yyout,yytext,kw_select); };
<stSELECT,stCOMMA>{COMMA}   {BEGIN_STATE(stCOMMA);  kw_print(yyout,yytext,kw_comma);  };

{IN}    { kw_print(yyout,yytext,kw_in); };

<stSELECT,stCOMMA>{LEFTP}   {PUSH_STATE(stLEFTP );  kw_print(yyout,yytext,kw_left_p); };
<stLEFTP>{LEFTP}            {PUSH_STATE(stLEFTP ); debug_match("{LEFTP}");kw_print(yyout,yytext,kw_left_p);  };
<stLEFTP>{COMMA}            {echo_print(yyout,yytext); };
<stLEFTP>{FROM}             {debug_match("{FROM}" ); kw_print(yyout,yytext,kw_from_2);  };
<stLEFTP>{RIGHTP}           {POP_STATE();            kw_print(yyout,yytext,kw_right_p); };
<stSELECT,stCOMMA,stUPDATE>{FROM} {BEGIN_STATE(stFROM);  kw_print(yyout,yytext,kw_from);    };
<stLEFTP,stSELECT>{AS}      {debug_match("{AS}"  );  kw_print(yyout,yytext,kw_as);      };

                /* FROM ... JOIN ... ON ... WHERE */
<stON,stFROM,stJOIN>{IJOIN} { BEGIN_STATE(stJOIN);  kw_print(yyout,yytext,kw_inner_join); };
<stON,stFROM,stJOIN>{LJOIN} { BEGIN_STATE(stJOIN);  kw_print(yyout,yytext,kw_left_join ); };
<stON,stFROM,stJOIN>{RJOIN} { BEGIN_STATE(stJOIN);  kw_print(yyout,yytext,kw_right_join); };
<stON,stFROM,stJOIN>{FJOIN} { BEGIN_STATE(stJOIN);  kw_print(yyout,yytext,kw_full_join ); };
<stON,stFROM,stJOIN>{CJOIN} { BEGIN_STATE(stJOIN);  kw_print(yyout,yytext,kw_cross_join); };
<stON,stFROM,stJOIN>{COMMA} { kw_print(yyout,yytext,kw_comma_join); };

<stJOIN>{ON}    {BEGIN_STATE(stON);   kw_print(yyout,yytext,kw_on); };



                /* WHERE ... (also join conditions) */
<stFROM,stJOIN,stON,stSET,stDELETE>{WHERE} {BEGIN_STATE(stWHERE );  kw_print(yyout,yytext,kw_where); };
<stWHERE,stON,stJOIN>{AND}  { debug_match("{AND}");  kw_print(yyout,yytext,kw_and);   };
<stWHERE,stON,stJOIN>{OR}   { debug_match("{OR}");   kw_print(yyout,yytext,kw_or);    };

<stWHERE>{EXISTS}   {kw_print(yyout,yytext,kw_exists); };


{GROUPBY}    {BEGIN_STATE(stGROUPBY); kw_print(yyout,yytext,kw_groupby); };
{ORDERBY}    {BEGIN_STATE(stORDERBY); kw_print(yyout,yytext,kw_orderby); };
<stORDERBY>{COMMA}   { kw_print(yyout,yytext,kw_comma_ordby); };
<stGROUPBY>{COMMA}   { kw_print(yyout,yytext,kw_comma_grpby); };
{HAVING}     {BEGIN_STATE(stWHERE); kw_print(yyout,yytext,kw_having);  };
{QUALIFY}    {BEGIN_STATE(stWHERE); kw_print(yyout,yytext,kw_qualify); };


<stINSERT>{LEFTP}        { PUSH_STATE(stINSCOLLIST); kw_print(yyout,yytext,kw_left_p_ins ); };
<stINSCOLLIST>{COMMA}    { kw_print(yyout,yytext,kw_comma_ins ); }
<stINSCOLLIST>{RIGHTP}   { POP_STATE();              kw_print(yyout,yytext,kw_right_p_ins ); };

<stP_SUB>{LEFTP}                      { BEGIN_STATE(peek_stack()); kw_print(yyout,yytext,kw_left_p    ); PUSH_STATE(stP_SUB);  };
{LEFTP}                               { PUSH_STATE(stP_SUB); };
<stP_SUB>{SELECT}                     { BEGIN_STATE(stSELECT);     kw_print(yyout,"(",kw_left_p_sub); kw_print(yyout,yytext,kw_select);};
<stP_SUB>{NUMBER}|{STRING}|{DBOBJECT} {
    if( peek_stack() == stFROM
        || peek_stack() == stJOIN )
    { BEGIN_STATE(peek_stack()); kw_print(yyout,"(",kw_left_p    ); echo_print(yyout,yytext);}
    else
    { BEGIN_STATE(stIN_CONSTLIST); kw_print(yyout,"(",kw_left_p    ); echo_print(yyout,yytext); }
    };
 /* <stP_SUB>{NUMBER}|{STRING}|{DBOBJECT} { BEGIN_STATE(stIN_CONSTLIST); kw_print(yyout,"(",kw_left_p    ); echo_print(yyout,yytext);}; */
<stP_SUB>{COMMENT_ML_START}           { echo_print(yyout,""); PUSH_STATE(stCOMMENTML)  ; echo_print(yyout,yytext);};
<stP_SUB>{COMMENT_ONE_LINE}           { echo_print(yyout,""); echo_print(yyout,yytext);};
<stP_SUB>{SPACE}                      { echo_print(yyout,""); };
<stP_SUB>{RIGHTP}                     { kw_print(yyout,"(",kw_left_p    ); POP_STATE(); kw_print(yyout,yytext,kw_right_p); }
<stP_SUB>.                            { BEGIN_STATE(peek_stack()); kw_print(yyout,"(",kw_left_p    ); echo_print(yyout,yytext); };

{RIGHTP}    {
                POP_STATE();
                if(subselect_level>0 && left_p - peek_sub_stack().left == (right_p+1) - peek_sub_stack().right - 1 ){
                    kw_print(yyout,yytext,kw_right_p_sub);
                } else {
                    debug_match("<wtf-leftp>");
                    kw_print(yyout,yytext,kw_right_p);
                }

            };




{COMMENT_ML_START}     {PUSH_STATE(stCOMMENTML); echo_print(yyout,yytext);};
<stCOMMENTML>{COMMENT_ML_PART1}     {debug_match("COMMENT_ML_PART1") ; echo_print(yyout,yytext);};
<stCOMMENTML>{COMMENT_ML_PART2}     {debug_match("COMMENT_ML_PART2") ; echo_print(yyout,yytext);};
<stCOMMENTML>{COMMENT_ML_END}       {POP_STATE(); echo_print(yyout,yytext);};

{COMMENT_ONE_LINE}     {echo_print(yyout,yytext);};


{STRING}     {echo_print(yyout,yytext);};

{SPACE}+     {/* discard spaces */;};
{DBOBJECT}   {echo_print(yyout,yytext);};
{NUMBER}     {echo_print(yyout,yytext);};
{SEMICOLON}  {BEGIN_STATE(INITIAL); kw_print(yyout,yytext,kw_semicolon);};
<*>.         {debug_match("<*>."); echo_print(yyout,yytext); };


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

