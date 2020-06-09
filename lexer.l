/* Lexer per Progetto Serra - Gruppo La Bua, Caruso, La Mantia */

%option noyywrap nodefault yylineno
%{
# include "serra.h"
# include "parser.tab.h"
%}


%%
 /* Operazioni a singolo carattere */
"+" | "-" | "=" | "," | ";" | "(" | ")"     { return yytext[0]; }

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
"connect" { yylval.func = B_connect; return FUNC; }
"reconnect" { yylval.func = B_reconnect; return FUNC; }
"status" { yylval.func = B_status; return FUNC; }
"switchOn" { yylval.func = B_switchOn; return FUNC; }
"switchOff" { yylval.func = B_switchOff; return FUNC; }
"diagnostic" { yylval.func = B_diagnostic; return FUNC; }
"archive" { yylval.func = B_archive; return FUNC; }
"interval" { yylval.func = B_interval; return FUNC; }
"insertDevice" { yylval.func = B_insertDevice; return FUNC; }


"clear" { yylval.func = B_clear; return SYSTEM; }

"CMD" { printf("User Mode:\t"); }


 /* Stringhe alfanumeriche */
["][a-zA-Z][a-zA-Z0-9]*["]  { yylval.s = search(yytext); return STRING; }
[a-zA-Z][a-zA-Z0-9]*  { yylval.s = search(yytext); return NAME; }            /* SERVE PER DEFINIRE I SIMBOLI PER I NOMI DELLE FUNZIONI */

 /* Numeri in virgola mobile */
[0-9]+"."[0-9] |
"."?[0-9]? { yylval.d = atof(yytext); return NUMBER; }

"//".*  
[ \t]   /*IGNORA SPAZIO E TAB*/ 
\\\n    printf("continue... > ");    /* IGNORA CONTINUAZIONE DI LINEA A CAPO*/
"\n"    { return EOL; }

.       { yyerror("Carattere Sconosciuto %c\n", *yytext); }


%%
