#  include <stdio.h>
#  include <stdlib.h>
#  include <stdarg.h>
#  include <string.h>
#  include <math.h>
#  include <ctype.h>
#  include <time.h>
#  include "serra.h"

static FILE *temp;


/* Tabella dei Simboli -> Hashing dei Simboli*/
static unsigned symhash(char *sym) {

  unsigned int hash = 0;
  unsigned c;

  while(c = *sym++)    hash = hash*9 ^ c;

  return hash;
}


unsigned char * symhashDev(char * nameSymbol){

  char devhash[DIMHASH];
  sprintf(devhash, "#%u", symhash(nameSymbol) % DIMHASH);

  return nameSymbol= strdup(strcat(nameSymbol, devhash));  
  /* in questa maniera il nome del device è il simbolo che sarebbe già presente nella tabella per causa della stringa,
   * pertanto lo concateniamo a un codice hash */
}



/*Nel programma non posso avere simboli uguali di tipi diversi per come è stata costruita la tabella di lookup. Un po come matlab, ci puo anche stare.*/
struct symbol *search(char* sym, char* type){
 /* puntatore alla cella corrispondente al simbolo cercato della tabella dei simboli dichiarata nell'header serra.h */
  struct symbol *symptr = &symtab[ symhash(sym) % DIMHASH ];
    
  int symcount = DIMHASH;      /* viene passata la dimensione della tabella per cercare in tutte le celle di questa il simbolo cercato */
    
    //Con le prime due operazioni ti sei preso la entry corrispondente al tuo device di fatto, c'è bisogno di scorrerti la tabella dei simboli?
  while(--symcount >= 0) {

    if(symptr->name && !strcmp(symptr->name, sym)) {        
 /* se trova il simbolo cercato ritorna il puntatore alla cella contenente il simbolo cercato */
        return symptr; 
    }

    if(!symptr->name) {             /* NUOVO SIMBOLO */ //se è nullo
      if(!type){    //se è nullo
         symptr->name = strdup(sym);
         symptr->value = 0;
         symptr->dev = NULL;
         symptr->func = NULL;
         symptr->syms = NULL;  //LISTA DI SIMBOLI
          return symptr;
      }
      else{
         //if(!strcmp(type, "searchSym")){
         if(!strcmp(type, "searchSym")){
             return NULL;
         }
      }
    }

    if(++symptr >= symtab + DIMHASH) {      /* Ricomincia da capo se la cella corrente è l'ultima */
        symptr = symtab;                       
    }
  }
  
  yyerror("Errore: Tabella piena\n");
  abort(); 

}



struct ast *newast(int nodetype, struct ast *l, struct ast *r)
{
  struct ast *a = malloc(sizeof(struct ast));
  
  if(!a) {
    yyerror("Spazio di memoria insufficiente\n");
    exit(0);
  }
  
  a->nodetype = nodetype;
  a->l = l;
  a->r = r;
  return a;
}



struct ast *newnum(double d)
{
  struct numval *a = malloc(sizeof(struct numval));
  
  if(!a) {
    yyerror("Spazio di memoria insufficiente\n");
    exit(0);
  }
  
  a->nodetype = 'K';
  a->number = d;
    
  return (struct ast *)a;
}


struct ast *newString(struct symbol *s)
{
  struct stringVal *a = malloc(sizeof(struct stringVal));
  
  if(!a) {
    yyerror("Spazio di memoria insufficiente\n");
    exit(0);
  }
  a->nodetype = 'C';
  a->s = s;
  
  return (struct ast *)a;
}


struct ast *newcmp(int cmptype, struct ast *l, struct ast *r)
{
  struct ast *a = malloc(sizeof(struct ast));
  
  if(!a) {
    yyerror("Spazio di memoria insufficiente\n");
    exit(0);
  }
  a->nodetype = '0' + cmptype;
  a->l = l;
  a->r = r;
  return a;
}


struct ast *newfunc(int functype, struct ast *l, struct ast *r)
{
  struct funcBuiltIn *a = malloc(sizeof(struct funcBuiltIn));
  
  if(!a) {
    yyerror("Spazio di memoria insufficiente\n");
    exit(0);
  }
  a->nodetype = 'F';
  a->l = l;
  a->r = r;
  a->functype = functype;
  //printf("%d", functype);       //DEBUG
    
  return (struct ast *)a;
}

struct ast *newfuncSystem(int functype)
{
  struct funcBuiltInSystem *a = malloc(sizeof(struct funcBuiltInSystem));
  
  if(!a) {
    yyerror("Spazio di memoria insufficiente\n");
    exit(0);
  }
  a->nodetype = 'O';
  a->functype = functype;
  //printf("%d", functype);       //DEBUG
  return (struct ast *)a;
}


struct ast * newDev(struct symbol *ps, struct argsList *l)
{
    
 
  struct device *d = malloc(sizeof(struct device));
  struct argsList *lpt;
  
  if(!d) {
    yyerror("Spazio di memoria insufficiente\n");
    exit(0);
  }
  
  char *nameSymbol;
  char *devNameList;
        
  nameSymbol= ps->name;

  nameSymbol= symhashDev(nameSymbol);
  
  struct symbol *symbolDev= NULL; 
  symbolDev = search(nameSymbol, "searchSym");


  if(symbolDev==NULL)    //SE IL DISPOSITIVO NON ESISTE
  {
       struct symbol *sym= search(nameSymbol, NULL);            //INSERIMENTO DEL SIMBOLO DEL DISPOSITIVO CON #CODICEHASH AGGIUNTO
       //printf("Ho inserito il simbolo con i seguenti valori:%s,%i\n",sym->name ,sym->value);
       d->nodetype = 'D';
       d->status = 0;  //LO PONGO CON STATO SPENTO DI DEFAULT
       d->s= sym;
       d->l = l;
       //printf("il fratello è:%s", (((struct argsList *) (d->l))->sym)->name);
       printf("Dispositivo inserito con successo con ID: %s \n", nameSymbol);

      
      //------>PER TENERE TRACCIA: CONFRONTARSI: aggiunta<-----
      //struct ast *dino = malloc(sizeof(struct ast));
      struct ast *dino= (struct ast *)d;
      (sym->dev)=dino;
      //printf("Ho inserito il dino con i seguenti valori:%s,%i, %i\n",sym->name ,sym->value, sym->dev);
     // printf("caratteristiche device: %c, %i\n", ((struct device *)(sym->dev))->nodetype, ((struct device *)(sym->dev))->status);
      
      //controllo da eliminare:
       // struct device *dino=(struct device *)symbolDev->dev;
      //struct device *w1=(struct device *)sym->dev;
      //printf("%i\n", ((struct device *)(sym->dev))->status);
      //--------------------------------fine aggiunta-------
      
       if(l!= NULL) { 

          struct symbol *ptrSymDevices= search(nameSymbol, "searchSym");     //ptrSymDevices valore di ritorno
         
          printf("%s connesso con ->   ", nameSymbol); 
          int countDeviceUnknown = 0;

          printf("[");

          for( lpt=l; lpt; lpt = lpt->next){
                 devNameList = lpt->sym->name;

                 const char strchSearchChar= '#';
                 if( !strchr(devNameList,strchSearchChar))
                        devNameList = symhashDev(devNameList);

                 if(strcmp(devNameList, nameSymbol)){     //SOLO SE DIVERSI
                
                     printf(" [%s] ", devNameList); 

                     if( !search(devNameList, "searchSym")){   
                      /* SE ptrSymDevices->dev NON È SETTATO, cioè se dev, puntatore ad un nodo struttura device è NULL, ALLORA BISOGNA CREARE ANCORA IL DEVICE, ANCHE SE IL SIMBOLO ESISTE GIÀ, COSA SCONTATA perchè creato in fase di parsing dalla regolaargsListDevice  */
                           printf("* ", ptrSymDevices->dev);
                            countDeviceUnknown++;
                     }

                 
                 }else{ 
                     printf(" itself ");
                 }
                 
                 
                 if(lpt->next != NULL) 
                          printf("-");

          }
          
          printf("]\n"); 

          if(countDeviceUnknown > 0)    printf("Devices con (*) sconosciuti, inserire devices\n");

       }
    
  }else{

    printf("Dispositivo già Esistente con ID: %s\n", nameSymbol);
  }
     
  return (struct ast *)d;

}



struct ast *newcall(struct symbol *s, struct ast *l)
{
  struct userfunc *a = malloc(sizeof(struct userfunc));
  
  if(!a) {
    yyerror("Spazio di memoria insufficiente\n");
    exit(0);
  }
  a->nodetype = 'U';
  a->l = l; //corpo dunzione
  a->s = s; //nome funzione, finisce nella tabella dei simboli
  return (struct ast *)a;
}


struct ast *newref(struct symbol *s)
{
  struct symref *a = malloc(sizeof(struct symref));
  
  if(!a) {
    yyerror("Spazio di memoria insufficiente\n");
    exit(0);
  }
  a->nodetype = 'N';
  a->s = s;
  return (struct ast *)a;
}


/*Nelle assegnazioni le scelt3 che sono state fatte riguardano l'associare al value della tabella dei simboli il riferimento. Che saranno tutti interpretati come puntatori a char anche se in realtà non lo sono */

struct ast *newasgn(struct symbol *s, struct ast *v)
{
  struct symasgn *a = malloc(sizeof(struct symasgn));
  
  if(!a) {
    yyerror("Spazio di memoria insufficiente\n");
    exit(0);
  }
  a->nodetype = '=';
  a->s = s;
  a->v = v;
  return (struct ast *)a;
}


struct ast *newContent(int nodetype, struct ast *cond, struct ast *tl, struct ast *el)
{
  struct content *a = malloc(sizeof(struct content));
  
  if(!a) {
    yyerror("Spazio di memoria insufficiente\n");
    exit(0);
  }
  a->nodetype = nodetype;
  a->cond = cond;
  a->tl = tl;
  a->el = el;
  return (struct ast *)a;
}


struct argsList *newargsList(struct symbol *sym, struct argsList *next)
{
  struct argsList *sl = malloc(sizeof(struct argsList));
  
  if(!sl) {
    yyerror("Spazio di memoria insufficiente\n");
    exit(0);
  }
  
  sl->sym = sym;
  (sl->sym)->name=sym->name;
    printf("ARGSLIST:%s\n", sym->name);
  sl->next = next;
  return sl;
}


void argsListfree(struct argsList *sl)
{
  struct argsList *nsl;

  while(sl) {
    nsl = sl->next;
    free(sl);
    sl = nsl;
  }
}


//Collega nella struct symbol passata come parametro i riferimenti alla argsList e all'AST che definisce
void defSymRef(struct symbol *name, struct argsList *syms, struct ast *func)
{
  if(name->syms) argsListfree(name->syms); 
  if(name->func) treefree(name->func);
  name->syms = syms;
  name->func = func;
}




struct ast * callbuiltin(struct funcBuiltIn *);
struct ast * callbuiltinSystem(struct funcBuiltInSystem *);
static char* calluser(struct userfunc *);


//maggiore (eval(a->l), eval(a->r));
int maggiore (char *sx, char *dx){
    int x=0;
    
    //nel caso dei numeri puoi fare direttamente il confronto tra i char. Nel caso delle stringhe questo confronto già ha poco senso
    
    //nel caso dei device non ha senso. Facciamo che i confronti li effettua solo coi numeri altrimenti il programma dà un errore
    
    
    if(atoi(dx)){
        if (atoi(sx)){
            printf("Sono numeri:%s,%s\n", sx,dx);
            x = ((atoi(sx)>atoi(dx) )? 1 : 0);
        }
    }else{
        printf("I valori passati come parametri non sono numeri\n");
        
         //come se fosse falso di default e quindi non entrerebbe nel corpo dell'if
    }
    return x;
}
    


char* eval(struct ast *a)
{
  char * v, *a1, *a2;
    int h;
  struct symbol *s;
  struct ast *c;

  if(!a) {
    yyerror("Errore interno, null eval");
    return 0;
  }
  switch(a->nodetype) {
    /* Number constant */

      case 'K':
                v= malloc(sizeof((((struct numval *)a)->number)));
                sprintf(v, "%4.4g", ((struct numval *)a)->number);
                //printf("NUMBER: %4.4g\n", ((struct numval *)a)->number);
                s=search("ans", NULL);
                s->value = v;
                break;

        /* String constant */
     case 'C':  
                v= strdup(((struct stringVal *)a)->s->name);
                s=search("ans", NULL);
                s->value = v; 
                break;
          
          

        /* In cordinata con case '='
        Quindi 
        name reference: serve ad accedere alle variabili che sono state dichiarate nel programma. Accende al campo value: è stato stabilito che:
        - le exp che ritornano un tipo char * o un tipo int in value viene memorizzato quel valore 
        - le exp che ritornano un tipo diverso in value viene memorizzato il putnatore 
        - le exp che ritornano un null memorizzano un null
        Esempi:
        - exp come le funzioni che ritornano symbol in value memorizzano l'indirizzo di memoria. Per ex: var = newDevice "ciao" -> in var viene memorizzato l'indirizzo
        - exp come le funzioni che ritornano il char * o l'int in value memorizzano il valore effettivo
        Comportamento: Name prende un symbol nella tabella dei simboli. In symref in s metti un symbol e ci accedi al value. Nel value hai memorizzato i valori delle variabili. Tipo device.
        */
     case 'N':
          //Per lo stesso motivo di case = controlli. Nel caso di funzioni void o funzioni che ritornano null a v viene assegnato NULL. Negli altri casi a v viene assegnato il puntatore al valore. 
          //Quindi in var ci finisce value-> in value tramite un assegnamento precedente, se la variabile non è presente nella tabella dei simboli crea una varaibile NULL. Se non fai un
          //assegamento precedente ci associa un valore a value. Tutte le funzioni embedded create non associano nulla a value. ------>In value ci vanno i valori assegnati<--------. Quindi è necessario a //prescindere un assegmaneto precedente.
          
              if((((struct symref *)a)->s->value))
                  v=strdup((((struct symref *)a)->s->value));
              else{
                  v=malloc(sizeof(char *));
                  v[0]='0';     //valore di default, se associo null rischio segmentazione
              }
              break;
    

        /* assignment: se l'espressione a dx ritorna un valore null non puo essere passato in input a strdup se no viene dato un segmentation fault. A parte questo dettaglio, l'assegnamento viene effettuato valutando l'espressione a destra e assegnadogli il risultato di quel valore a sx. Nel caso in cui l'espressione dx ritorna un ast a v sarà assegnato un tipo symbol mascherato come ast sarà ritornato un tipo symbol. Questo riguarda tutte le funzioni embedded: connect, switchOn ecc che ritornano tutte un tipo symbol. Significa far vedere l'alto livello all'utente. Il modo per risolverlo è far tornare valori diversi a queste funzioni: per ex name di symbol.*/
          
        /*Secondo punto: consideriamo come null anche le funzioni di tipo void come archive, cioè è previsto che le funzioni che non devono ritornare nulla ritornino un tipo null. Quindi eventualmetne anche nella definizione delle funzioni runtume è necessario venga aggiunta. Se non metti alcun tipo è normale che altrimenti andrà in segmentazione quando effettua l'eval. Ex: archive senza valore di ritorno*/
     case '=': 
              v=eval(((struct symasgn *)a)->v);
              ((struct symasgn *)a)->s->value = v;
              if(((struct symasgn *)a)->s->value){
                v= strdup( ((struct symasgn *)a)->s->value); //assegno il valore a value. In realtà assegno il riferimento a value. Poi posso stampare.
              }else{
                  v=NULL;
              }
              break;
          


  /*  comparisons: da rivedere le precedenze: causa reduce*/
      case '1': 
              a1=eval(a->r);
              a2=eval(a->l);
              h= maggiore(a1,a2); break;    
          /*a1=eval(a->l); a2=eval(a->r); h= (maggiore (a1, a2)); printf("FINE:%i\n",h); break; */   //>
      case '2': v = strdup((eval(a->l) < eval(a->r))? 1 : 0); break;      //<
      case '3': v = strdup((eval(a->l) != eval(a->r))? 1 : 0); break;     //==
      case '4': v = strdup((eval(a->l) == eval(a->r))? 1 : 0); break;
      case '5': v = strdup((eval(a->l) >= eval(a->r))? 1 : 0); break;
      case '6': v = strdup((eval(a->l) <= eval(a->r))? 1 : 0); break;
  


  /* control content */
  /* null if/else/do expressions allowed in the grammar, so check for them */
      case 'I': 
          /*((struct symbol *)v)->name: aggiunto 2 righe: 422 e 423*/
          /*prima solo: if( eval( ((struct content *)a)->cond) != 0) { */
           if( eval( ((struct content *)a)->cond)){
              //printf("if valido\n");
              if( ((struct content *)a)->tl) {//se non è nullo allora c'è un corpo e lo eseguo
                    //printf("Then valido\n");
                    v = eval( ((struct content *)a)->tl);
              } else
                    v = 0;        /* a default value */
            } else {
                if( ((struct content *)a)->el) {
                    v = eval(((struct content *)a)->el);
                } else
                    v = 0;        /* a default value */
            }
            break;
     case 'W':
        v = 0;        /* a default value */

        if( ((struct content *)a)->tl) {
          while( eval(((struct content *)a)->cond) != 0)
                     v = eval(((struct content *)a)->tl);
        }

        break;        /* last value is value */

     case 'L': eval(a->l); v = eval(a->r); break;

     case 'F': v = callbuiltin((struct funcBuiltIn *)a);/*AGGIUNTA: return ((struct symbol *)v)->name;*/ break;

     case 'O': v = callbuiltinSystem((struct funcBuiltInSystem *)a); break;

     case 'U': v = calluser((struct userfunc *)a); break;

     case 'D': break;
          


     // DEVO GESTIRMELO COME NEI CASI SOPRA SCENDENDO L'ALBERO AST CON LA RICORSIONE DI EVAL

      default: printf("Errore interno: bad node %c\n", a->nodetype);
  }
  return v;
}


//switchOn
/*FUNZIONANTE
void switchOn(char *v){
    printf("4\n");
    //struct symbol *symDev= (struct symbol *) connect(v);
    char *nameSymbol= symhashDev(v);
    struct symbol *sym=  search(nameSymbol, "searchSym");
    printf("symbol:%s, %i, %i\n",sym->name, sym->value, ((struct device *) (sym->dev))->status);
    printf("5\n");

    if(sym){
        //struct device *d= symDev->dev;
        //d->status=1;
        printf("6\n");
        ((struct device *) (sym->dev))->status =1;
        printf("7\n");
        printf("La connect è andata a buon fine e il dispositivo è stato acceso\n\n");
    }else{
        printf("Dispositivo inesistente\n\n");
        //return NULL;
    }
    //return (struct ast *)symDev;
        
}
*/

//connect
struct ast *connect(char *v){
    
     printf("Ricerca del dispositivo %s in corso...\n", v);
     char *vino=symhashDev(v);
     struct symbol *symDev= search(vino, "searchSym");

     if(symDev){
        printf("Dispositivo Esistente\nRichiesta connessione...\n");     //INSERIRE ENUM CON CODICE DISPOSITIVO COME HTTP 200 AD ESEMPIO
     }else{
        printf("Dispositivo %s: Non Esistente\n", vino);  
        return NULL;
     }

     return (struct ast *) symDev;
    
}

//Accensione del dispositivo:
//Descrizione: L'accensione del dispositivo è stata intesa come la variazione dello status dell'ogetto del device. L'ogetto device è memorizzato all'interno la tabella dei simboli nel campo Dev (è stato aggiunto una cosa piccola dentro search, chiedere conferma a vincenzo). Quindi bisogna effettuare una connect (si puo richiamare dentro il case di B_connect, confrontarsi) che restituisce il simbolo. Se la connessione è andata a buon fine si va a modificare il campo Dev.
struct ast *switchOn(char *v){
    
    printf("\t\t\tVerifica in corso della connessione del dispositivo....\n");
    struct symbol *sym= (struct symbol *) connect(v);

    if(sym){
        ((struct device *) (sym->dev))->status =1;
        printf("\t\t\tLa connect è andata a buon fine e il dispositivo è stato acceso\n\n");
    }else{
        printf("\t\t\tDispositivo inesistente e quindi non acceso\n\n");
        return NULL;
    }
    return (struct ast *)sym;
        
}

struct ast *switchOff(char *v){
    
    printf("\t\t\tVerifica in corso della connessione del dispositivo....\n");
    struct symbol *sym= (struct symbol *) connect(v);

    if(sym){
        ((struct device *) (sym->dev))->status =0;
        printf("\t\t\tIl dispositivo è spento\n\n");
    }else{
        printf("\t\t\tDispositivo inesistente\n\n");
        return NULL;
    }
    return (struct ast *)sym;
        
}


/*Funzione interval che dato un tipo ast in input va a verificare l'esistenza di quel device. Nel caso in cui il device esiste cioè è presente nalla tabella dei simboli va a controllare tramite un parametro passato come parametro, cioè l'istante di tempo, quanti secondi devono passare per mantenere attivo il device. Passato l'intervallo di tempo lo pone a 0. Quindi teoricamente questa funzione pretende in input due parametri rappresenti da una stringa che identifica il periodo di tempo in cui deve stare attivo e il device da attivare. Come parametro prende il device da attivare per quell'intervalo di tempo*/
/*interval "device, 10 minuti"-> formato: device, 10 minuti*/

struct ast *interval (int r, char *v){
    
    //GESTIONE STRINGHE: da 560 a riga 575
    //Estrazione dei paraemtri: devicePippo e 10
    printf("\nAVVIO OPERAZIONE:\n");

        //Operazioni:
    char *temp=malloc((strlen(v)+1)*sizeof(char));
    temp=strcpy(temp, v);

    struct symbol *h= switchOn(v);
    if(h){
        sleep(r); //time.h, 1000 è 1 secondo, aspettiamo min secondi
        switchOff(temp);
    }
    
    return h;

    //return NULL: Se lo metto va in segmentazione, credo per motivi legati al parlellismo tra processi, lasciando il warning risolviamo il problema
    
}


/*Archivia file: dovrebbe essere creato un tipo Device primitivo per applicare la ricorsione. Non avendolo previsto sono costretto a far funzioni che prevedono il passaggio solo di una stringa e non del tipo device. Posso eventualmente passare un intero come id. E dall'id ricavarlo, ma potrei avere piu tipi id uguali. Applico con il passaggio di una stringa. Eventualmente possiamo semplicemente prevedere che il file sia sempre lo stesso e quindi non viene passato come parametro. Credo sarebbe la cosa migliore. Al momento lo prevedo cosi e commento la parte in cui passo il file come parametro. Passo come parametro solo il device da scrivere.*/

void cancella_device (struct symbol *syms){
    
    /*Passo come parametro il simbolo che voglio cancellare. Cancellare vuol dire cancellare il riferimento. Eliminare un elemento dalla tabella dei simboli vuol dire lasciare un buco. Dato che la tabella dei simboli ha una dimensione prefissata. Ponendolo a null non riesco piu ad accedervi. Rimane un campo null nella tabella dei simboli che eventualmente puo essere riutilizzato in futuro per scrivere. Non è gestito il caso in cui la tabella dei simboli è piena. Al momento è cosi. Per com'è stato fatto il search la ricerca viene fatta in base al name */

    (syms->name)=NULL;
    
    
}
    

/*Il file è presupposto sia sempre lo stesso e quindi non c'è bisogno di passarlo come parametro*/
void scrivi_file (struct symbol *syms){
        
    struct device *d=(struct device *) (syms->dev);
    //printf("symbol:%s, %i, %i\n",syms->name, syms->value, d->status);
    
    FILE *fd;
    char *x=(syms->name);

                /* apre il file in scrittura */
    fd=fopen("Device.txt", "a");
    if( fd==NULL ) {
            perror("Errore in apertura del file");
            exit(1);
    }


          /* scrive il numero */
    fprintf(fd, "%s\n", x);


                /* chiude il file */
    fclose(fd);


}



void archivia (char *v){
    
    char *nameSymbol= symhashDev(v);
    struct symbol *symo=  search(nameSymbol, "searchSym");
    
    //struct argsList *n= (symo->syms);
    
    //printf("E' collegato a:%s\n",(n->sym)->name); 
    //printf("E' collegato a:%s\n",(n->sym)->name);
    
    if(symo){
        scrivi_file(symo);
        cancella_device (symo);
    }
    

}



void helpMessage(){

printf("** Manuale di istruzione **\n\n\nElenco comandi base con esempi:\n-------------------------------\n\n-   Inserimento Stringa:\n\n> 'Nuova Stringa' [INVIO]\n\n-   newString \return struct ast \* pointerSimbolo\n\n-   Esempio: 'Sono una Stringa'\n\n\n-   Stampa Stringa:\n\n> print “Stringa” [ INVIO ]\n\n-   callbuiltin return char \* pointerSymbol\n\n-   Esempio: print “Ciao sono una Stringa” [ INVIO ]\n\n-   N.B. Anche l’inserimento dell’istruzione sopra produce la stampa.\n\n\n-   Inserimento Sempice Nuovo Dispositivo:\n\n> newDev “Nome Device” [INVIO]\n\n-   newDev return struct ast \* pointerDevice\n\n-   Esempio: newDevice “dev1” [ INVIO ]\n\n\n-   Inserimento con Collegamenti a Dispositivi di Nuovo Dispositivo:\n\n> newDev “Nome Device” → [ “Nome Device 1”, “Nome Device 2”, …] [ INVIO ]\n\n-   newDev return struct ast \* pointerDevice\n\n-   Esempio: newDevice “dev1” → [“dev2”, “dev3”, “dev4”] [ INVIO ]\n\n\n-   Richiesta Status con Dispositivo:\n\n> status “Nome Device” [ INVIO ]\n\n-   callbuiltin return struct ast \* pointerSymbol\n\n-   Esempio: status “dev1” [ INVIO ]\n\n\n-   Richiesta Connessione con Dispositivo:\n\n> connect “Nome Device” [ INVIO ]\n\n-   callbuiltin return struct ast \* pointerSymbol\n\n-   Esempio: connect “dev1” [ INVIO ]\n\n\n-   Richiesta Riconnessione con Dispositivo:\n\n> reconnect “Nome Device” [ INVIO ]\n\n-   callbuiltin return struct ast \* pointerSymbol\n\n-   Esempio: reconnect “dev1” [ INVIO ]\n\n\n-   Richiesta Riconnessione con Dispositivo:\n\n> reconnect “Nome Device” [ INVIO ]\n\n-   callbuiltin return struct ast \* pointerSymbol\n\n-   Esempio: reconnect “dev1” [ INVIO ]\n\n\n- Accensione status dispositivo\n\n> sintassi: switchOn ”StringaNomeDevice”\n\n-   callbuiltin return struct ast \* pointerSymbol\n\n-   Esempio: > switchOn ”dev1”  [INVIO]\n\n\n- Off status dispositivo\n\n> sintassi: switchOff ”StringaNomeDevice”\n\n-   callbuiltin return struct ast \* pointerSymbol\n\n-   Esempio: Esempio: > switchOff ”dev1”  [INVIO]\n\n\n- Accensione dello status del device per un certo intervallo di tempo:\n\n> sintassi: interval ”NomeDevice”-Secondi  ->note: è un unica stringa dove il primo parametro identicia il device da accendere il \secondo per quanti secondi deve stare acceso.E' un unica stringa che prima della virgola ha il nome del device e dopo la virgola un \intero: ex: (”pippo,10”, ”paperino,20”).\n\n-   callbuiltin return struct ast \* pointerSymbol\n\n-   Esempio: > interval ”pippo”-10  [INVIO] ”diagnostic”\n\n\n- Archiviazione e cancellazione di un device:\n\n> sintassi: archive ”NomeDevice”\n\n-   void\n\n-   Esempio: > archive ”pippo”  [INVIO]\n\n\nDefinizioni variabili e assegnazioni variabili:\n\- +-come matlab\n \n\if-then;\n\n");

}



struct ast * callbuiltin(struct funcBuiltIn *f)
{

  enum builtFunc functype = f->functype;
  //char *v = strdup(eval(f->l));
  char *v;
  double r;
  FILE *fconfig, *let;
    
  struct symbol *symDev;

  switch(functype) {
   case B_print:
         v=strdup(eval(f->l));
         //printf("display = ");
         if(atoi(v) || isalpha(v[0]))
             printf("%s\n", v);
          else
              printf("ERROR: Il parametro passato alla funzione printf non è nè un numero nè una stringa");
         return (struct ast *) v;
         break;

   case B_help:
         v=strdup(eval(f->l));
         helpMessage();
         return NULL;
         break;

   case B_connect:
         v=strdup(eval(f->l));
         printf("Ricerca del dispositivo %s in corso...\n", v);
         v=symhashDev(v);
         symDev= search(v, "searchSym");
         //printf("Sto ritornando symDev:%s\n", symDev->name);

         if(symDev){
            printf("Dispositivo Esistente\nRichiesta connessione...\n");     //INSERIRE ENUM CON CODICE DISPOSITIVO COME HTTP 200 AD ESEMPIO
         }else{
            printf("Dispositivo %s: Non Esistente\n", v);  
            return NULL;
         }
         //printf("Sto ritornando v:%s\n",  symDev->name);

         return (struct ast *) symDev;
         break;

   case B_reconnect:
         v=strdup(eval(f->l));
         printf("Ricerca del dispositivo %s in corso...\n", v);
         v=symhashDev(v);
         symDev= search(v, "searchSym");

         if(symDev){
            printf("Dispositivo Esistente\nRichiesta Riconnessione...\n");     //INSERIRE ENUM CON CODICE DISPOSITIVO COME HTTP 200 AD ESEMPIO
         }else{
            printf("Dispositivo %s: Non Esistente\n", v);  
            return NULL;
         }
         //statusDevice(symDev);
         return (struct ast *) symDev;
         break;

   case B_status:
         v=strdup(eval(f->l));
         v=symhashDev(v);
         printf("Richiesta status in corso per il device %s...\n", v);
         symDev= search(v, "searchSym");

         if(symDev){
            printf("Dispositivo %s:  Esistente-> %s\n", v, symDev-> name);
         }else{
            printf("Dispositivo %s:  Non Esistente\n", v);  
            return NULL;
         }
         //statusDevice(symDev);
         return (struct ast *) symDev;
         break;
         
     case B_switchOn:
         v=strdup(eval(f->l));
         symDev=switchOn(v);
         //Controllo se tutto è andato bene, altrimenti col valore di return di switchOn facciamo prima
         //symDev= search(v, "searchSym");
         //printf ("Il dispositivo è stato aggiornato con stato: %i.",((struct device *) (symDev->dev))->status); 
         return (struct ast *) symDev;
         
     case B_switchOff:
         v=strdup(eval(f->l));
         symDev= switchOff(v);
         //symDev= search(v, "searchSym");
         //int status=((struct device *) (symDev->dev))->status;
         //printf ("Il dispositivo è stato aggiornato con stato: %i.\n",status); 
         return (struct ast *) symDev;
          
      case B_interval:
         v=strdup(eval(f->l));
         r=atoi(strdup(eval(f->r)));
         printf("numero:%g, stringa:%s\n", r, v);
         symDev= interval(r, v);
         return (struct ast *) symDev;
         //break;
          
      case B_archive:
          v=strdup(eval(f->l));
          archivia(v);
          return NULL; //void è la funzione
          break;
          
      case  B_readFile:
            v=strdup(eval(f->l));
          
            let=fopen(v, "r"); //esistenza file
          
            if (let!=NULL){
                //Inserisci il terminatore del file: token TERM
                fconfig=fopen(v, "a");
                fprintf(fconfig,"match\n");
                fclose(fconfig); 
                
                //Effettua la fase di lettura del file:
                index_file=1;
                yyin=let;  
                
            }else{
                printf("file inesistente\n");
            }
                 
            break;


   default:
         yyerror("Funzione built-in sconosciuta %d\n", functype);
         return 0;
 }
}


/*FUNZIONI EMBEDDED*/
struct ast * callbuiltinSystem(struct funcBuiltInSystem *f)
{
  enum builtFuncSystem functype = f->functype;

 switch(functype) {
   case B_clear:
     system("clear");
     return 0;
     break;
   default:
     yyerror("Funzione di sistema built-in sconosciuta %d\n", functype);
 }
}



static char * calluser(struct userfunc *f)
{
  struct symbol *fn = f->s;   /* Nome Funzione */
  struct argsList *sl;        /* Argomenti Lista */
  struct ast *args = f->l;    /* Argomento attuale */
  char *oldval, *newval;      /* Memorizza il valore dell'argomento */
  char *v;
  int nargs;
  int i;

  //da noi sarà sempre inesistente, non scrivi mai dentro func nulla.
  if(!fn->func) {
    yyerror("Chiamata ad una funzione insesistente", fn->name);
    return 0;
  }

  /* Contatore Argomenti */
  sl = fn->syms;
  for(nargs = 0; sl; sl = sl->next)
    nargs++;

  /* Crea Spazio di Memoria adeguato per gli Argomenti */
  oldval = (char *)malloc(nargs * sizeof(char));
  newval = (char *)malloc(nargs * sizeof(char));
  if(!oldval || !newval) {
    yyerror("Fuori Spazio in %s", fn->name); return 0;
  }
  
  /* Valuta gli argomenti */
  for(i = 0; i < nargs; i++) {
    if(!args) {
      yyerror("Troppo pochi argomenti nella chiamata per %s", fn->name);
      free(oldval); 
      free(newval);
      return 0;
    }

    if(args->nodetype == 'L') {        /* se è una lista di nodi */
      sprintf(newval[i], "%s", eval(args->l) );
      args = args->r;

    } else {                           /* se è la fine della lista di nodi */
      sprintf(newval[i], "%s", eval(args) );
      args = NULL;
    }
  }
  
  
  /* salvare i vecchi valori degli argomenti, per assegnarne di nuovi */
  sl = fn->syms;
  
  for(i = 0; i < nargs; i++) {
    struct symbol *s = sl->sym;

    sprintf(oldval[i], "%s", s->value );
    sprintf(newval[i], "%s", s->value );

    sl = sl->next;
  }

  free(newval);

  char *ptrReturnEval= eval(fn->func);
  /* Valutazione della funzione */
  v= malloc(sizeof(ptrReturnEval));
  sprintf(v, "%s", eval(fn->func) );
  
  

  /* Rimette a posto i valori */
  sl = fn->syms;
  for(i = 0; i < nargs; i++) {
    struct symbol *s = sl->sym;

    sprintf(oldval[i], "%s", s->value );
    sl = sl->next;
  }

  free(oldval);
  return v;
}



void treefree(struct ast *a)
{
  switch(a->nodetype) {

    /* two subtrees */
 
  case '1':  case '2':  case '3':  case '4':  case '5':  case '6':
  case 'L': 
    treefree(a->r);
    /* no subtree */
  case 'K': case 'N': case 'C':case 'F':case 'O':case 'U': case 'D':
    break;
  
  case '=':
    free( ((struct symasgn *)a)->v);
    break;

  case 'I': case 'W': 
    free( ((struct content *)a)->cond);
    if( ((struct content *)a)->tl) free( ((struct content *)a)->tl);
    if( ((struct content *)a)->el) free( ((struct content *)a)->el);
    break;

  default: printf("Errore interno: free bad node %c\n\n>", a->nodetype);
  
  }
  
  free(a); /* always free the node itself */

}

void yyerror(char *s, ...)
{
  va_list ap;
  va_start(ap, s);

  fprintf(stderr, "%d: Errore: ", yylineno);
  vfprintf(stderr, s, ap);
  fprintf(stderr, "\n");
  va_end(ap);
}




void welcomeMessage(){
    printf("\n");
    printf("                +-+-+-+-+-+-+-+ +-+-+ +-+-+-+\n");
    printf("                |W|e|l|c|o|m|e| |t|o| |t|h|e|  \n");                          
    printf("                +-+-+-+-+-+-+-+ +-+-+ +-+-+-+\n");
    printf(" ad88888ba   88888888888  88888888ba   88888888ba          db \n");           
    printf("d8'     '8b  88           88      '8b  88      '8b        d88b          \n"); 
    printf("Y8,          88           88      ,8P  88      ,8P       d8'`8b          \n");
    printf("`Y8aaaaa,    88aaaaa      88aaaaaa8P'  88aaaaaa8P'      d8'  `8b         \n");
    printf("  `aaaayy8   88aaaaa      88''''88'    88''''88h       8Yaaaaaa8b       \n");
    printf("        `8b  88           88    `8b    88    `8b      d8''''''''8b       \n");
    printf("Y8a     a8P  88           88     `8b   88     `8b    d8'        `8b      \n");
    printf(" YY88888PP   88888888888  88      `8b  88      `8b  d8'          `8b     \n");
    printf("          +-+-+-+-+-+-+-+-+-+-+-+ +-+-+-+-+-+-+-+-+-+ \n");
    printf("          |p|r|o|g|r|a|m|m|i|n|g| |l|a|n|g|u|a|g|e|!|  \n");                          
    printf("          +-+-+-+-+-+-+-+-+-+-+-+ +-+-+-+-+-+-+-+-+-+ \n");
    printf("\n");
    printf("\n");
    printf(" +-+-+-+-+-+ +-+-+-+-+ +-+-+ +-+-+-+ +-+-+-+-+ +-+-+-+ +-+-+-+ +-+-+\n");
    printf(" | |W|r|i|t|e| |h|e|l|p| |l|i||s|t| |i|f| |y|o|u| |n|e|e|d| |i|t|. |\n");                          
    printf(" +-+-+-+-+-+ +-+-+-+-+ +-+-+ +-+-+-+ +-+-+-+-+ +-+-+-+ +-+-+-+ +-+-+\n");
    printf("\n");
}



int main()
{
    welcomeMessage();
    printf(">");
    search("ans", NULL);

    return yyparse();

}



/* debugging: dump out an AST */
int debug = 0;




void dumpast(struct ast *a, int level)
{

  printf("%*s", 2*level, "");
  level++;

  if(!a) {
    printf("NULL\n");
    return;
  }

  switch(a->nodetype) {
    /* constant */
  case 'K': printf("number %4.4g\n", ((struct numval *)a)->number); break;

    /* name reference */
  case 'N': printf("ref %s\n", ((struct symref *)a)->s->name); break;

    /* assignment */
  case '=': printf("= %s\n", ((struct symref *)a)->s->name);
    dumpast( ((struct symasgn *)a)->v, level); return;

    /* expressions */
  case '+': case '-': case '*': case '/': case 'L':
  case '1': case '2': case '3':
  case '4': case '5': case '6': 
    printf("binop %c\n", a->nodetype);
    dumpast(a->l, level);
    dumpast(a->r, level);
    return;

  case 'I': case 'W':
    printf("content %c\n", a->nodetype);
    dumpast( ((struct content *)a)->cond, level);
    if( ((struct content *)a)->tl)
      dumpast( ((struct content *)a)->tl, level);
    if( ((struct content *)a)->el)
      dumpast( ((struct content *)a)->el, level);
    return;

  case 'F':
    printf("funzione predefinita %d\n", ((struct funcBuiltIn *)a)->functype);
    dumpast(a->l, level);
    return;

  case 'U': printf("funzione utente %s\n", ((struct userfunc *)a)->s->name);
    dumpast(a->l, level);
    return;

  default: printf("bad %c\n", a->nodetype);
    return;
  }
}
