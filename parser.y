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
%token <func> INTERVAL
%token <func> INSERT
%token <func> CMP
%token <str> ARROW
%token EOL
%token <str> TERM
%token IF THEN ELSE WHILE DO CMD


//Identifica la precedenza: la regola CMP è quella che ha la piu alta precedenza. Se hai piu alberi esegue quello in cui la regola CMP viene eseguita a una precedenza > : %nonassoc <func> CMP
%right '='
%left '+' '-'

%type <a> exp stmt listStmt explistStmt
%type <sl> argsList
%type <sl> argsListDevice

%start exec

%%

exec:  /* nothing */
    | exec stmt EOL { 
                      char *valEval;
                      valEval = eval($2);
                      if(valEval != NULL){
                            if(!strcmp(valEval,"D")){  
                                 printf("Operazione di inserimento dispositivo completata con successo\n\n> "); 
                            }else{       
                                 printf("%s\n> ", valEval);
                                 printf("\n> ");
                            }
                      }else{
                            printf("\n> ");
                      }

                      treefree($2);
                      
                    }
                         
    | exec CMD NAME '(' argsList ')' '=' listStmt EOL  {             // CREA UNA NUOVA FUNZIONE
                                                            defSymRef($3, $5, $8); 
                                                            printf("Definito %s\n> ", $3->name); 
                                                        }
    
    | exec TERM EOL        { index_file=0; yyin=stdin; yyrestart(yyin);  }       //ogni file deve terminare col carattere TERM, scappatoia: se scrive l'utetnte terminatore crea una variabile nulla. Parola chiave

    | exec error EOL { 
                                printf("\n> ");

                        yyerrok;
                     }
                     
    | NAME           {  yyerrok; }
    
;

 
stmt: IF exp THEN listStmt            { $$ = newContent('I', $2, $4, NULL); }
    | IF exp THEN listStmt ELSE listStmt  { $$ = newContent('I', $2, $4, $6); }
    | WHILE exp DO listStmt           { $$ = newContent('W', $2, $4, NULL); }
    | exp
;


listStmt:  /* nothing */  { $$ = NULL; }
        | stmt ';' listStmt  {
                                
                                 if ($3 == NULL){
                                        $$ = $1;
                                }else{
                                        $$ = newast('L', $1, $3);
                                 }
                             }
;


exp: exp CMP exp         {  $$ = newcmp($2, $1, $3); }
  // | exp '+' exp         { $$ = newast('+', $1,$3); }
  // | exp '-' exp         { $$ = newast('-', $1,$3);}
   | '(' exp ')'         { $$ = $2; }
   | NUMBER              { $$ = newnum($1); }
   | FUNC explistStmt    { $$ = newfunc($1, $2, NULL); }
   | FUNCDEV explistStmt { 
                           $$ = newfunc($1, $2, NULL); 
                          }
    | INSERT STRING ARROW '[' argsListDevice ']'  {       
    //COSTRUISCE LA LISTA DI PUNTATORI AI SIMBOLI CIOÈ AI DEVICE COLLEGATI AL DEVICE CHE SI STA INSERENDO
                                                           defSymRef($2, $5, NULL);
                                                           $$ = newDev($2,$5);
                          }
    | INSERT STRING       { 
                                    
                            defSymRef($2, NULL, NULL);
                            $$ = newDev($2,NULL);
                          }
   | SYSTEM               { $$ = newfuncSystem($1); }
   | STRING               { $$ = newString($1); }
   | NAME                 { printf("ciao\n"); $$ = newref($1); }          //riferimenti a variabili
   | NAME '=' exp         { $$ = newasgn($1, $3); }     //per gli assegmaneti
   | NAME '(' explistStmt ')' { $$ = newcall($1, $3);}  //per le funzioni: Nodo U
   | INTERVAL explistStmt '-' explistStmt { //bug: interval-pi-> si confonde con name, ma è pieno di sti bug (si devono correggere?)
            $$=newfunc($1, $2, $4);

   }
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
