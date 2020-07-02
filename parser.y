/* Parser per Progetto Serra - Gruppo La Bua, Caruso, La Mantia */

%{
#  include <stdio.h>
#  include <stdlib.h>
#  include "serra.h"
%}

%union {
  struct ast *a;
  double d;
  char * str;
  struct symbol *s;       /* Specifica quale simbolo si sta passando*/
  struct argsList *sl;
  int  func;              /* Specifica quale funzione built-in deve essere eseguita */
}

/* declare tokens */
%token <d> NUMBER
%token <str> STRING
%token <s> NAME
%token <func> FUNC
%token <func> SYSTEM
%token <func> FUNCDEV
%token <func> INSERT
%token <str> ARROW
%token EOL

%token IF THEN ELSE WHILE DO CMD


%nonassoc <func> CMP
%right '='
%left '+' '-'

%type <a> exp stmt listStmt explistStmt
%type <sl> argsList
%type <sl> argsListDevice

%start exec

%%

exec: /* nothing */
    | exec stmt EOL {
                      char *valEval= eval($2);
                      if(valEval != NULL){
                            printf("%s\n> ", valEval);
                      }else{
                            printf("\n> ");
                      }

                      treefree($2);
                    }
                         
    | exec CMD NAME '(' argsList ')' '=' listStmt EOL  {             // CREA UNA NUOVA FUNZIONE
                                                            defSymRef($3, $5, $8); 
                                                            printf("Definito %s\n> ", $3->name); 
                                                        }
    | INSERT STRING ARROW '[' argsListDevice ']' EOL  {       
    //COSTRUISCE LA LISTA DI PUNTATORI AI SIMBOLI CIOÈ AI DEVICE COLLEGATI AL DEVICE CHE SI STA INSERENDO
                                                           defSymRef($2, $5, NULL);
                                                           newDev($2,$5);
                                                           printf("Operazione di inserimento dispositivo e relativi collegamenti completata con successo\n> "); 
                          }
    | INSERT STRING EOL    { 
                                         defSymRef($2, NULL, NULL);
                                         newDev($2,NULL);
                                         printf("Operazione di inserimento completata con successo\n> "); 
                          }
    | NAME           {  yyerrok; }
    | exec error     {                 //GESTIONE DEGLI ERRORI
                        yyerrok;
                     }
;

 
stmt: IF exp THEN listStmt            { $$ = newContent('I', $2, $4, NULL); }
    | IF exp THEN listStmt ELSE listStmt  { $$ = newContent('I', $2, $4, $6); }
    | WHILE exp DO listStmt           { $$ = newContent('W', $2, $4, NULL); }
    | exp
;


listStmt: /* nothing */  { $$ = NULL; }
        | stmt ';' listStmt  {  if ($3 == NULL)
                                    $$ = $1;
                                else
                                    $$ = newast('L', $1, $3);
                             }
;


exp: exp CMP exp         { $$ = newcmp($2, $1, $3); }
  // | exp '+' exp         { $$ = newast('+', $1,$3); }
  // | exp '-' exp         { $$ = newast('-', $1,$3);}
   | '(' exp ')'         { $$ = $2; }
   | NUMBER              { $$ = newnum($1); }
   | FUNC explistStmt    { $$ = newfunc($1, $2); }
   | FUNCDEV explistStmt { 
                           $$ = newfunc($1, $2); 
                          }
   | SYSTEM               { $$ = newfuncSystem($1); }
   | STRING               { $$ = newString($1); }
   | NAME                 { $$ = newref($1); }
   | NAME '=' exp         { $$ = newasgn($1, $3); }
   | NAME '(' explistStmt ')' { $$ = newcall($1, $3); }
;


explistStmt: exp
           | exp ',' explistStmt  { $$ = newast('L', $1, $3); }
;


argsList: NAME       { $$ = newargsList($1, NULL); }
        | NAME ',' argsList { $$ = newargsList($1, $3); }
;


argsListDevice: STRING    { $$ = newargsList($1, NULL); }
| STRING ',' argsListDevice { $$ = newargsList($1, $3); }
;


%%
