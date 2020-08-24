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


 /* Funzioni di sistema */
"clear" { yylval.func = B_clear; return SYSTEM; }

"readFile" { printf("Sto leggendo il file\n"); yylval.func= B_readFile; return FUNC; }
"match" { return TERM; }


 /* Funzioni per il tipo device */
"help" { yylval.func = B_help; return FUNCDEV; }
"connect" { yylval.func = B_connect; return FUNCDEV; }
"reconnect" { yylval.func = B_reconnect; return FUNCDEV; }
"status" { yylval.func = B_status; return FUNCDEV; }
"switchOn" { yylval.func = B_switchOn; return FUNCDEV; }
"switchOff" { yylval.func = B_switchOff; return FUNCDEV; }
"diagnostic" { yylval.func = B_diagnostic; return FUNCDEV; }
"archive" { yylval.func = B_archive; return FUNCDEV; }
"interval" { yylval.func = B_interval; return INTERVAL; }



"newDevice" { yylval.func = B_insertDevice; return INSERT; }


 /* ARROW */
"->" { return ARROW;}


"CMD" { printf("User Mode:\t"); }

 /* Stringhe alfanumeriche (quindi contiene anche i caratteri?). Ho cambiato il riconoscimento delle stringhe prima era_["][a-zA-Z][,]?[a-zA-Z0-9]*["] */

\"([^\\\"]|\\.)*\" {            
                                int dimString= strlen(yytext)+1;
                                char subbuff[dimString-2];
                                memmove( subbuff, &yytext[1], dimString-3 );
                                subbuff[dimString-3] = '\0';
                                yylval.s = search(subbuff, NULL); 
                                //printf("SSBUF:%s\n", subbuf);
                                return STRING; 
}



[a-zA-Z][a-zA-Z0-9]*  { yylval.s = search(yytext, NULL); return NAME; }            /* SERVE PER DEFINIRE I SIMBOLI PER I NOMI DELLE FUNZIONI */

 /* Numeri in virgola mobile */
 /*Prende la prima cifra intera dopo la virgola. E i numeri interi li prende solo fino a 10, da discutere*/
[0-9]+"."[0-9] |
"."?[0-9]? { yylval.d = atof(yytext); return NUMBER; }


 
[ \t]   /*IGNORA SPAZIO E TAB*/ 
\\\n    printf("continue... > ");    /* IGNORA CONTINUAZIONE DI LINEA A CAPO*/
\n    { return EOL; }

.       { yyerror("Carattere Sconosciuto %c\n", *yytext); }


%%
