/*
Thanks to http://vsbabu.org/software/lsqlb.html
Vattekkat Satheesh Babu, on Dec 30, 1998
Helped to learn about flex a bit
*/


/*
-compile using: 
    flex sqlb.l ; gcc -o sqlb lex.yy.c -lfl; rm lex.yy.c
    (needs flex; works on ubuntu; actualy there is/should be  makefile)
-execute using:
    cat test_text.sql | ./sqlb

  
*/
DIGIT   [0-9]
NUMBER  {DIGIT}+([.]{DIGIT}+)?
ID      [A-Za-z_][A-Za-z0-9_]*
SPACE   [ \t\n]

DBOBJECT    {ID}[.]{ID}|{ID}

LEFTP   [(]
RIGHTP  [)]

SELECT  (?i:select|sel)
AS      (?i:as)
FROM    (?i:from)

IJOIN   (?i:(INNER{SPACE}+)?JOIN)
LJOIN   (?i:LEFT{SPACE}+(OUTER{SPACE}+)?JOIN)
RJOIN   (?i:RIGHT{SPACE}+(OUTER{SPACE}+)?JOIN)
FJOIN   (?i:FULL{SPACE}+(OUTER{SPACE}+)?JOIN)
CJOIN   (?i:CROSS{SPACE}+JOIN{SPACE}+)
ANYJOIN ({LJOIN}|{RJOIN}|{FJOIN}|{CJOIN})

ON      (?i:on)
WHERE   (?i:where)
SAMPLE  (?i:sample)
AND     (?i:and)
EXISTS  (?i:exists)
IN      (?i:in)

COMMA [,]

%{
#include "settings.h"

int currindent=0;
int left_p  = 0;
int right_p = 0;
int subselect_level = 0;

#include "print_keywords.h"
%}


%s special
%x stSELECT stFROM stWHERE stON stEXISTS stLEFTP stJOIN stCOMMA

%%
                /* SELECT ... FROM */
<INITIAL>{SELECT}           {BEGIN_STATE(stSELECT); print_SELECT(); };
<stSELECT,stCOMMA>{COMMA}   {BEGIN_STATE(stCOMMA ); print_COMMA_stSELECT();};
<stSELECT,stCOMMA>{LEFTP}   {BEGIN_STATE(stLEFTP ); print_LEFTP() ; left_p++; };
<stLEFTP>{LEFTP}            {debug_match("{LEFTP}");debug_p(); print_LEFTP(); left_p++; };
<stLEFTP>{FROM}             {debug_match("{FROM}" );print_FROM();};
<stLEFTP>{RIGHTP}           {if(p_level()==1)
                             BEGIN_STATE(stSELECT); print_RIGHTP(); right_p++; debug_p();};
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

<stWHERE>{EXISTS}   {BEGIN_STATE(stEXISTS); print_EXISTS(); };
<stEXISTS>{LEFTP}   {BEGIN_STATE(INITIAL ); print_LEFTP_begin_SUB(); };


<stLEFTP,stSELECT>{AS}      {debug_match("{AS} "  );print_AS();};


<*>{LEFTP}     { left_p++; ECHO; };
<*>{RIGHTP}    { right_p++;
                if(subselect_level && p_level()<=0){
                    currindent--;
                new_line();
                }
                ECHO;
               };
<*>{SPACE}+     printf("");
<*>.            ECHO;


<<EOF>> {printf("\n"); return 0 ;}
.           printf( "Unrecognized character(s) : %s\n", yytext ); // just checking.. :)
%%


#include "debuging.h"







