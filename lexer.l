/* Lexer per Progetto Serra - Gruppo La Bua, Caruso, La Mantia */

%option noyywrap nodefault yylineno
%{
# include "serra.h"
# include "parser.tab.h"
%}


%%
 /* Operazioni a singolo carattere */
"+" | "-" | "=" | "," | ";" | "(" | ")" | "."     { return yytext[0]; }

 /* Operazioni di comparazione *
">"     { yylval. func = 1; return CMP; }
"<"     { yylval. func = 2; return CMP; }
"<>"    { yylval. func = 3; return CMP; }
"=="    { yylval. func = 4; return CMP; }
">="    { yylval. func = 5; return CMP; }
"<="    { yylval. func = 6; return CMP; }
*/

 /* Parole chiavi */
"if"    { return IF; }
"then"  { return THEN; }
"else"  { return ELSE; }
"while" { return WHILE; }
"do"    { return DO; }

 /* Funzioni incorporate */
"print" { yylval.func = B_print; return FUNC; }


 /* Funzioni di sistema */
"clear" { yylval.func = B_clear; return SYSTEM; }


 /* Funzioni per il tipo device */
"connect" { yylval.func = B_connect; return FUNCDEV; }
"reconnect" { yylval.func = B_reconnect; return FUNCDEV; }
"status" { yylval.func = B_status; return FUNCDEV; }
"switchOn" { yylval.func = B_switchOn; return FUNCDEV; }
"switchOff" { yylval.func = B_switchOff; return FUNCDEV; }
"diagnostic" { yylval.func = B_diagnostic; return FUNCDEV; }
"archive" { yylval.func = B_archive; return FUNCDEV; }
"interval" { yylval.func = B_interval; return FUNCDEV; }



"newDevice" { yylval.func = B_insertDevice; return INSERT; }


 /* ARROW */
"->" { return ARROW;}


"CMD" { printf("User Mode:\t"); }


 /* Stringhe alfanumeriche */
["][a-zA-Z][a-zA-Z0-9]*["]  { yylval.s = search(yytext); return STRING; }
[a-zA-Z][a-zA-Z0-9]*  { yylval.s = search(yytext); return NAME; }            /* SERVE PER DEFINIRE I SIMBOLI PER I NOMI DELLE FUNZIONI */

 /* Numeri in virgola mobile */
[0-9]+"."[0-9] |
"."?[0-9]? { yylval.d = atof(yytext); return NUMBER; }

 
[ \t]   /*IGNORA SPAZIO E TAB*/ 
\\\n    printf("continue... > ");    /* IGNORA CONTINUAZIONE DI LINEA A CAPO*/
"\n"    { return EOL; }

.       { yyerror("Carattere Sconosciuto %c\n", *yytext); }


%%
