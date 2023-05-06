#include "tokens.hpp"
#include <stdlib.h>
#include <iostream>
#include <string>
#include <cstring>

#define STRING_MAX_LEN 1024

std::string token_names[] = {"0","VOID", "INT", "BYTE", "B","BOOL", "AND", "OR", "NOT", "TRUE", "FALSE",
                             "RETURN", "IF", "ELSE", "WHILE", "BREAK", "CONTINUE",  "SC", "COMMA", "LPAREN", "RPAREN",
                             "LBRACE", "RBRACE", "ASSIGN", "RELOP", "BINOP", "COMMENT", "ID", "NUM", "STRING","OVERRIDE",
                             "UNCLOSED_STRING"};

void printToken(int token);
int hexaToDec(char c);
bool checkCharacterValidity (int c);
bool checkEscapeErrors();
int escape(char to_print[STRING_MAX_LEN] , int index_in_yy, int index_in_to_print, int len);

void printToken(int token)
{

    if(token_names[token]== "COMMENT")
    {
        std::cout  << yylineno<<" " << token_names[token] << " " <<"//" << std::endl;
    }
    else{
    std::cout << yylineno<<" " <<  token_names[token] <<" " <<yytext << std::endl;
    }
}

int hexaToDec(char c)
{
    if (c >= '0' && c <= '9')
    {
        return c - '0';
    }
    if (c >= 'a' && c <= 'f')
    {
        return c - 'a' + 10;
    }
    if (c >= 'A' && c <= 'F')
    {
        return c - 'A' + 10;
    }
    return -1;
}

bool checkCharacterValidity (int c)
{
    if((c>=0x20 && c<=0x7E) || c==0x09 || c==0x0A || c==0x0D)
    {
        return true;
    }
    return false;
}

int escape(char to_print[STRING_MAX_LEN] , int index_in_yy, int index_in_to_print, int len)
{
       index_in_yy++;    //we wnat to pass the "/"
       switch(yytext[index_in_yy])
       {
        case '\\': to_print[index_in_to_print]='\\'; return 1;
        case '\"': to_print[index_in_to_print]='\"'; return 1;
        case 'n':  to_print[index_in_to_print] = '\n';return 1;
        case 't':  to_print[index_in_to_print] = '\t'; return 1;
        case '0':  to_print[index_in_to_print] = '\0'; return 1;
        case 'r':  to_print[index_in_to_print] = '\r'; return 1;
        case 'x':
        {
            if(index_in_yy==len-2 && yytext[len-1]=='\"')  // case of "..../x"
            {
                std::cout << "Error undefined escape sequence x" <<std::endl;
                exit(0);
            }
            if(index_in_yy==len-3 && yytext[len-1]=='\"')   //case of "..../xd"
            {
                std::cout << "Error undefined escape sequence x" << yytext[index_in_yy+1]<<std::endl;
                exit(0);
            }
            int first= hexaToDec(yytext[index_in_yy+1]);
            int second=hexaToDec(yytext[index_in_yy+2]);
            int decimal_value=first*16 + second;

            if(decimal_value<0x00 || decimal_value>0x7F)
            {
                std::cout << "Error undefined escape sequence x" <<yytext[index_in_yy+1] << yytext[index_in_yy+2] <<std::endl;
                exit(0);
            }

            to_print[index_in_to_print]=(char)decimal_value;
            return 3;
        }
        default:
        {
            std::cout << "Error undefined escape sequence" <<" " << yytext[index_in_yy] <<std::endl;
            exit(0);
        }
       }
        std::cout << "Errorrrrrrrr";
       return -1;
}

bool checkEscapeErrors(char c)
{
    if(c=='n' || c=='r' || c=='t' || c=='\\' || c=='\"' ||c=='0' || c=='x')
    {
        return false;
    }
    return true;
}

int main()
{
	int token;


	while ((token = yylex())) {
	    switch(token)
        {
            case VOID: printToken(token);break;
            case INT: printToken(token);break;
            case BYTE: printToken(token);break;
            case B: printToken(token); break;
            case BOOL: printToken(token); break;
            case AND: printToken(token); break;
            case OR: printToken(token); break;
            case NOT: printToken(token); break;
            case TRUE: printToken(token); break;
            case FALSE: printToken(token); break;
            case RETURN:printToken(token); break;
            case IF: printToken(token); break;
            case ELSE: printToken(token); break;
            case WHILE: printToken(token); break;
            case BREAK: printToken(token); break;
            case CONTINUE: printToken(token); break;
            case SC:printToken(token); break;
            case COMMA: printToken(token); break;
            case LPAREN: printToken(token); break;
            case RPAREN: printToken(token); break;
            case LBRACE: printToken(token); break;
            case RBRACE: printToken(token); break;
            case ASSIGN: printToken(token); break;
            case RELOP: printToken(token); break;
            case BINOP: printToken(token); break;
            case COMMENT: printToken(token); break;
            case ID: printToken(token); break;
            case NUM: printToken(token); break;
            case STRING:
            {
                int initial_len= strlen(yytext);
                if(initial_len==2 && yytext[0]=='\"' && yytext[1]=='\"')
                {
                    std::cout << yylineno <<" STRING"<< std::endl;
                }

                char to_print[STRING_MAX_LEN]={0};
                int j=0;      //index in to_print

                for(int i=0; i<initial_len; i++)   //the character is a regular ASCII character
                {
                    if(!checkCharacterValidity(yytext[i]))
                    {

                        std::cout << "Error" << std::endl <<yytext[i];
                        exit(0);
                        break;
                    }
                    else if(yytext[i]!= '\\')
                    {
                        to_print[j]=yytext[i];
                        j++;
                    }
                    else    //we are in the case of escape sequence
                    {
                    i+=escape(to_print, i,j, initial_len);
                    j++;
                    }
                }

                to_print[j-1]='\0';
                std::cout << yylineno << " STRING ";
                for(int i=0; to_print[i] != '\0'; i++)
                {
                    std::cout << to_print[i];
                }
                std::cout << std::endl;
                break;
            }
            case UNDEFINED_CHARACTER:
            {

                std::cout << "Error " << yytext << std::endl;
                exit(0);
                break;
            }
            case OPEN: break;
            case UNCLOSED_STRING:
            {
                std::cout << "Error unclosed string" << std::endl;
                exit(0);
                break;
            }
            case ESCAPE_ERROR:
            {
                int count=strlen(yytext);
                for(int i=0; i < count-1; i++)
                {
                    if(yytext[i]=='\\'  && checkEscapeErrors(yytext[i+1]))
                    {
                        std::cout <<"Error undefined escape sequence "<<yytext[i+1] << std::endl;
                        exit(0);
                    }

                }
                break;
            }
            default:
            {

                printf("Error %s\n", yytext);
                exit(0);
                break;
            }
        }
	}
	return 0;
}

