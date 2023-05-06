%{
/*Declarations section*/
#include <stdio.h>
#include "tokens.hpp"
%}

%option yylineno
%option noyywrap

digit	([0-9])  
letter	([a-zA-Z])   
whitespace	([\t\n\r ])   
binop	([\+\-\*\/])
relop	([=!<>]=|<|>)
letter_or_digit	([a-zA-Z0-9])
positif_digit	([1-9]) 
escape_sequence (\\[nrt0x\"\\])


%x BEGIN_ST


%%
void                                     								     return VOID;
int                                      								     return INT;
byte                                      								     return BYTE;
b                                         								     return B;
bool                                       									 return BOOL;
override                                   									 return OVERRIDE;
and                                        									 return AND;
or                                         									 return OR;
not                                        									 return NOT;
true                                        								 return TRUE;
false                                       								 return FALSE;
return                                     									 return RETURN;
if                                         								     return IF;
else                                      								     return ELSE;
while                                     								     return WHILE;
break                                     								     return BREAK;
continue                                  								     return CONTINUE;
;                                       							         return SC;
,                                     								         return COMMA;
\(                                          								 return LPAREN;
\)                                          								 return RPAREN;
\{                                         								     return LBRACE;
\}                                          								 return RBRACE;
=                                       								     return ASSIGN;
{relop}                                    									 return RELOP;
{binop}                                    									 return BINOP;
\/\/[^\r\n]*                                                                 return COMMENT;
{letter}{letter_or_digit}*              								     return ID;
0|{positif_digit}{digit}*               							         return NUM;  
\"                                                                           BEGIN(BEGIN_ST); return  OPEN;
<BEGIN_ST>([^(\r\n)\r\n\"]*[\n\r][^(\r\n)\r\n\"]*)+\"       {BEGIN(INITIAL); return UNCLOSED_STRING;}
<BEGIN_ST>([^\"\\]|{escape_sequence})*\"                                     BEGIN(INITIAL); return STRING;
<BEGIN_ST>([^\"\\]|{escape_sequence})*                                       BEGIN(INITIAL); return UNCLOSED_STRING;
<BEGIN_ST>([^\"\\]|(\\.))*\"                                                 BEGIN(INITIAL); return ESCAPE_ERROR; 
<BEGIN_ST>.                                                                  BEGIN(INITIAL); return UNDEFINED_CHARACTER;
{whitespace}                                                                 ;
.                                                                            return UNDEFINED_CHARACTER;

%%
