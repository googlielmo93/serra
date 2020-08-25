/* Lexer per Progetto Serra - Gruppo La Bua, Caruso, La Mantia */

%option noyywrap nodefault yylineno
%{
# include "serra.h"
# include "parser.tab.h"
%}


%%
 /* Operazioni a singolo carattere */
"+" { return yytext[0]; }
"-" { return yytext[0]; }
"=" { return yytext[0]; }
"," { return yytext[0]; }
";" { return yytext[0]; }
"(" { return yytext[0]; }
")" { return yytext[0]; }
"." { return yytext[0]; }
"[" { return yytext[0]; }
"]" { return yytext[0]; }


 /* Operazioni di comparazione */
">"     { yylval.func = 1; return CMP; }
"<"     { yylval.func = 2; return CMP; }
"<>"    { yylval.func = 3; return CMP; }
"=="    { yylval.func = 4; return CMP; }
">="    { yylval.func = 5; return CMP; }
"<="    { yylval.func = 6; return CMP; }


 /* Parole chiavi */
"if"    {  return IF; }
"then"  {  return THEN; }
"else"  { return ELSE; }
"while" { return WHILE; }
"do"    { return DO; }


 /* Funzioni incorporate */
"print" { yylval.func = B_print; return FUNC; }
"readFile" { printf("Sto leggendo il file\n"); yylval.func= B_readFile; return FUNC; }
"match" { return TERM; }


 /* Funzioni per il tipo device */
"connect" { yylval.func = B_connect; return FUNCDEV; }
"reconnect" { yylval.func = B_reconnect; return FUNCDEV; }
"status" { yylval.func = B_status; return FUNCDEV; }
"switchOn" { yylval.func = B_switchOn; return FUNCDEV; }
"switchOff" { yylval.func = B_switchOff; return FUNCDEV; }
"diagnostic" { yylval.func = B_diagnostic; return FUNCDEV; }
"archive" { yylval.func = B_archive; return FUNCDEV; }
"interval" { yylval.func = B_interval; return INTERVAL; }
"newDevice" { yylval.func = B_insertDevice; return INSERT; }

 /* ARROW necessario per le associazioni tra dispositivi*/
"->" { return ARROW;}


 /* Modalità User */
"CMD" { printf("User Mode:\t"); }


 /* Funzioni senza parametri, funzionalità di sistema */
"clear" { yylval.func = B_clear; return SYSTEM; }
"help" { yylval.func = B_help; return SYSTEM; }


 /* Stringhe alfanumeriche.*/
\"([^\\\"]|\\.)*\" {            
                                int dimString= strlen(yytext)+1;
                                char subbuff[dimString-2];
                                memmove( subbuff, &yytext[1], dimString-3 );
                                subbuff[dimString-3] = '\0';
                                yylval.s = search(subbuff, NULL); 
                                return STRING; 
}


 /* SERVE PER DEFINIRE I SIMBOLI PER I NOMI DELLE FUNZIONI */
[a-zA-Z][a-zA-Z0-9]*  { yylval.s = search(yytext, NULL); return NAME; }            

 /* Numeri in virgola mobile */
 /* Prende la prima cifra intera dopo la virgola. */
[0-9]+"."[0-9] |
"."?[0-9]? { yylval.d = atof(yytext); return NUMBER; }


 
[ \t]   /*IGNORA SPAZIO E TAB*/ 
\\\n    printf("continue... > ");    /* IGNORA CONTINUAZIONE DI LINEA A CAPO*/
\n    { return EOL; }

.       { yyerror("Carattere Sconosciuto %c\n", *yytext); }


%%
