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
%token <func> ADD
%token <func> GET
%token <func> SET
%token <func> REMOVE


%token <func> ARRAY
%token <d> INTEGER
%token <d> CHAR
%token <d> DEVICE
%token <func> FUNCARRAY

%token EOL
%token <str> TERM
%token IF THEN ELSE WHILE DO CMD HELP


//Identifica la precedenza: la regola CMP è quella che ha la piu alta precedenza. Se hai piu alberi esegue quello in cui la regola CMP viene eseguita a una precedenza > : %nonassoc <func> CMP
%right '='
%left '+' '-'

%type <a> exp stmt listStmt explistStmt
%type <a> liste
%type <a> value
%type <sl> argsList
%type <sl> argsListDevice
%type <d> tipes

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
                     
   // | NAME           {  yyerrok; }        // uno shift reduc con riga 136
    
;
 
stmt: IF exp THEN listStmt            { $$ = newContent('I', $2, $4, NULL); }
    | IF exp THEN listStmt ELSE listStmt  { $$ = newContent('I', $2, $4, $6); }
    | WHILE exp DO listStmt           { $$ = newContent('W', $4, $2, NULL); }
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


exp: //exp CMP exp         {  $$ = newcmp($2, $1, $3); }            3 shift rduc
  // | exp '+' exp         { $$ = newast('+', $1,$3); }
  // | exp '-' exp         { $$ = newast('-', $1,$3);}
    '(' exp ')'         { $$ = $2; }  
   | value
   | FUNC explistStmt    { $$ = newfunc($1, $2, NULL, NULL); }                                   
   | FUNCDEV explistStmt {                                                                 //OK
                           $$ = newfunc($1, $2, NULL, NULL); 
                          }
    | INSERT STRING ARROW '[' argsListDevice ']'  {                                         //ok
    //COSTRUISCE LA LISTA DI PUNTATORI AI SIMBOLI CIOÈ AI DEVICE COLLEGATI AL DEVICE CHE SI STA INSERENDO
                                                           defSymRef($2, $5, NULL);
                                                           $$ = newDev($2,$5);
                       }

   | SYSTEM               { $$ = newfuncSystem($1); }                                       
   | NAME                 { $$ = newref($1); }          //riferimenti a variabili              OK
   | NAME '=' exp         { $$ = newasgn($1, $3); }     //per gli assegmaneti               ok
   | NAME '(' explistStmt ')' { $$ = newcall($1, $3);}  //per le funzioni: Nodo U
   | INTERVAL explistStmt '-' explistStmt { //bug: interval-pi-> si confonde con name, ma è pieno di sti bug (si devono correggere?)
            $$=newfunc($1, $2, $4, NULL);                               //ok

   }
   | liste
;


liste: ARRAY tipes NAME '(' NUMBER ')' ';' { 
                                               newArray($3, $5, $2); 
                                               
                                                struct ast *a = malloc(sizeof(struct ast)); //fittizzio
                                                a->nodetype='D'; //fittizzio
                                                $$=a;
                                            }
        | NAME ARROW  ADD '=' value         {
                                            
                                               $$=newfunc($3, $5, $1, NULL);
                                            
                                            }
        
        | NAME ARROW GET                     {
                                                $$=newfunc($3, NULL, $1, NULL);
                                            }
                                            
        | NAME ARROW  GET '=' NUMBER        {
                                                //valu è l'indice
                                                $$=newfunc($3,  (struct ast *) (newnum($5)), $1, NULL);
                                            }
                                            
        | NAME ARROW  SET '=' value ',' NUMBER       {
                                                        //primo parametro è valore, secondo indice
                                                        $$=newfunc($3, $5, $1, (struct ast *) (newnum($7)));
                                                      }
                                                      
        | NAME ARROW REMOVE                 {
                                                        $$=newfunc($3, NULL, $1, NULL);
                                            }
       
;

tipes: 
    CHAR {$$=2;} | INTEGER {$$=1;} | DEVICE {$$=3;}

;

value:
   INSERT STRING       { 
                                    
                            defSymRef($2, NULL, NULL);
                            $$ = newDev($2,NULL);
                          }
 | NUMBER              { $$ = newnum($1); }
 | STRING               { $$ = newString($1); }
;



explistStmt: exp
        //   | exp ',' explistStmt  { $$ = newast('L', $1, $3); }         //1 shift rduc, al momtno l'ho lvato ma non sono sicuro . Srvirbb pr l funzioni, credo si possa levare ma confermate, non so se serve a qualcosa ch e non notoe
;


argsList: NAME       { $$ = newargsList($1, NULL); }
        | NAME ',' argsList { $$ = newargsList($1, $3); }
;


argsListDevice: STRING    { $$ = newargsList($1, NULL); }
               | STRING ',' argsListDevice { $$ = newargsList($1, $3); }
;


%%