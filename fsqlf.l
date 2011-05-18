/*
Thanks to http://vsbabu.org/software/lsqlb.html
Vattekkat Satheesh Babu, on Dec 30, 1998
Helped to learn about flex a bit
*/


DIGIT   [0-9]
NUMBER  {DIGIT}+([.]{DIGIT}+)?
ID      [A-Za-z_][A-Za-z0-9_]*
SPACE   [ \t\n]
NONSPACE [^ \n\r\t]

DBOBJECT    {ID}[.]{ID}|{ID}

LEFTP   [(]
RIGHTP  [)]

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
EXISTS  (?i:exists)
IN      (?i:in)

COMMA [,]

COMMENT_ONE_LINE [-]{2,}[^\n]*[\n]
COMMENT_ML_START [/][*]+
COMMENT_ML_PART1 [^*]+
COMMENT_ML_PART2 [*]+[^/]
COMMENT_ML_END   [*]+[/]

%{
#include "print_keywords.h"
#include "settings.h"
#include "global_variables.h"


%}


%s special
%x stSELECT stFROM stWHERE stON stEXISTS stLEFTP stJOIN stIN stCOMMA stINLIST stCOMMENTML stSTRING

%%
                /* SELECT ... FROM */
<INITIAL>{SELECT}           {BEGIN_STATE(stSELECT); print_SELECT(); };
<stSELECT,stCOMMA>{COMMA}   {BEGIN_STATE(stCOMMA ); print_COMMA_stSELECT();};
<stSELECT,stCOMMA>{LEFTP}   {BEGIN_STATE(stLEFTP ); print_inc_LEFTP();  };
<stLEFTP>{LEFTP}            {debug_match("{LEFTP}");print_inc_LEFTP();  };
<stLEFTP>{FROM}             {debug_match("{FROM}" );print_FROM();};
<stLEFTP>{RIGHTP}           {if(p_level()==1){ BEGIN_STATE(stSELECT);}
                                print_inc_RIGHTP(); };
<stSELECT,stCOMMA>{FROM}    {BEGIN_STATE(stFROM  ); print_stFROM()  ; };

                /* FROM ... JOIN ... ON ... WHERE */
<stON,stFROM>{IJOIN} { BEGIN_STATE(stJOIN)  ;print_IJOIN(); };
<stON,stFROM>{LJOIN} { BEGIN_STATE(stJOIN)  ;print_LJOIN(); };
<stON,stFROM>{RJOIN} { BEGIN_STATE(stJOIN)  ;print_RJOIN(); };
<stON,stFROM>{FJOIN} { BEGIN_STATE(stJOIN)  ;print_FJOIN(); };
<stON,stFROM>{CJOIN} { BEGIN_STATE(stJOIN)  ;print_CJOIN(); };

<stJOIN>{ON}    {BEGIN_STATE(stON);   print_ON(); };
<stON>{AND}     {debug_match("{AND}");print_AND();};

<stFROM,stJOIN,stON>{WHERE} {BEGIN_STATE(stWHERE ); print_WHERE() ; };

<stWHERE>{AND}     {debug_match("{AND}");print_AND();};
<stWHERE>{EXISTS}   {PUSH_STATE(stEXISTS); print_EXISTS(); };
<stEXISTS>{LEFTP}   {PUSH_STATE(INITIAL ); print_LEFTP_begin_SUB(); };

<stWHERE>{IN}    { PUSH_STATE(stIN); print_IN();};
<stIN>{LEFTP}    { ; }; // at this point its not clear if its subquery or list expression, '(' will be printed later
<stIN>{SELECT}   { print_LEFTP_begin_SUB(); PUSH_STATE(stSELECT); print_SELECT(); };
<stIN>{NONSPACE} { BEGIN_STATE(stINLIST);print_LEFTP(); ECHO;};

<stLEFTP,stSELECT>{AS}      {debug_match("{AS}"  );print_AS();};



<*>{LEFTP}     { print_inc_LEFTP(); };
<*>{RIGHTP}    { right_p++;
                if(subselect_level && p_level()<=0){printf("sdgsdgdsg");
                    POP_STATE();POP_STATE();
                    end_SUB();
                    new_line();
                }
                print_RIGHTP();
               };

<*>{COMMENT_ML_START}     {PUSH_STATE(stCOMMENTML);printf("\n");ECHO;};
<stCOMMENTML>{COMMENT_ML_PART1}     {debug_match("COMMENT_ML_PART1") ; ECHO;};
<stCOMMENTML>{COMMENT_ML_PART2}     {debug_match("COMMENT_ML_PART2") ; ECHO;};
<stCOMMENTML>{COMMENT_ML_END}       {POP_STATE();ECHO;printf("\n");};

<*>{COMMENT_ONE_LINE}     {ECHO;};

<*>['][^'][']		  {ECHO;};
	
<*>{SPACE}+     printf(" ");
<*>";"          printf("\n;");
<*>.            ECHO;
<*>{DBOBJECT}   ECHO;
<*>{NUMBER}     ECHO;


<<EOF>> {printf("\n"); if(YY_START == stCOMMENTML || YY_START == stSTRING){printf("--unterminated comment or string\n");} return 0 ;}



%%


#include "debuging.h"



int     yywrap (void)
{
	return 1;
}

int     main (int argc, char * argv[])
{
	while (yylex () != 0) ;

	return 0;
}

