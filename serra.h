/* Header per Progetto Serra - Gruppo La Bua, Caruso, La Mantia */



/* Tabella dei Simboli */
struct symbol {
  char *name;
  char *value;
  struct ast *func;         /* stmt per le funzioni */
  struct ast *dev;         /* stmt per le funzioni */
  struct argsList *syms;     /* Lista dei i */
};


/* Tabella di dimensione fissa */
#define DIMHASH 10000
struct symbol symtab[DIMHASH];


/* Dichiarazione Funzione Search() */
struct symbol *search(char*, char*);


struct numval {
  int nodetype;         /* tipo nodo K -> valore costante*/
  double number;
};



struct stringVal {
  int nodetype;         /* tipo nodo C -> valore stringa costante*/
  struct symbol *s;
};



struct device {
  int nodetype;         /* tipo nodo D -> dispositivo inserito nella rete */
  int status;          /* definisce lo stato, acceso 1, spento 0 */
  struct symbol *s;     
  struct ast *l;        /* Lista dei device collegati */
};



/* Struttura di collegamento tra un simbolo della tabella e una lista di simboli */
struct argsList {
  struct symbol *sym;
  struct argsList *next;
};


/* Dichiarazione Funzione argsList() */
struct argsList *newargsList(struct symbol *sym, struct argsList *next);


/* Dichiarazione Funzione argsListfree() */
void argsListfree(struct argsList *sl);


/* Tipi di Nodi
 *  + - 
 *  0-7 Tipo di simbolo di comparazione
 *  L statement list
 *  I IF statement
 *  W WHILE statement
 *  N riferimento ad un simbolo
 *  K valore costante
 *  = assegnamento
 *  C Stringa
 *  S Lista di Simboli
 *  F Funzioni Predefinite
 *  O other function di sistema
 *  U Funzioni Utente
 *  D Dispositivo
 */ 


/* Funzioni Predefinite */
enum builtFunc {              
  B_print = 1, 
  B_connect = 2, 
  B_reconnect = 3,
  B_status = 4,
  B_switchOn = 5, 
  B_switchOff = 6,
  B_diagnostic = 7, 
  B_archive = 8,
  B_interval = 9,
  B_insertDevice = 10,
  B_readFile= 11
};


enum builtFuncSystem {            
    B_clear = 1
};



/* Struttura Nodo dell'albero di Sintassi Astratto */
struct ast {
  int nodetype;
  struct ast *l;
  struct ast *r;
};


/* Struttura Nodo per l'AST, per le funzioni predefinite */
struct funcBuiltIn {    
  int nodetype;         /* tipo nodo F -> Funzioni Built-In*/
  struct ast *l;
  struct ast *r;
  enum builtFunc functype;
};




/* Struttura Nodo per l'AST, per le funzioni predefinite */
struct funcBuiltInSystem {    
  int nodetype;         /* tipo nodo O -> Funzioni Built-In System o senza argomenti */
  enum builtFuncSystem functype;
};


/* Struttura Nodo per l'AST, per le funzioni utente */
struct userfunc {      
  int nodetype;         /* tipo nodo U -> Funzioni Utente*/
  struct ast *l;        /* Lista degli argomenti della funzione utente */
  struct symbol *s;
};


/* Struttura Nodo per i costrutti IF e WHILE */
struct content {
  int nodetype;         /* tipo nodo I o W */
  struct ast *cond;     /* condizione */
  struct ast *tl;       /* costrutti then o do listStmt */
  struct ast *el;       /* opzionale else listStmt */
};


/* Struttura Nodo per un riferimento ad un simbolo della tabella, per le funzioni utente */
/* Anche usato per accedere alle variabili */
struct symref {
  int nodetype;         /* tipo nodo N -> riferimento ad un simbolo*/
  struct symbol *s;
};


/* Struttura Nodo per il simbolo di assegnamento = */
struct symasgn {
  int nodetype;         /* tipo nodo = -> assegnamento */
  struct symbol *s;
  struct ast *v;        /* valore */
};


/* FUNZIONI PER LA GENERAZIONE DI UN AST */
struct ast *newast(int nodetype, struct ast *l, struct ast *r);
struct ast *newcmp(int cmptype, struct ast *l, struct ast *r);
struct ast *newfunc(int functype, struct ast *l, struct ast *r);
struct ast *newfuncSystem(int functype);
struct ast *newcall(struct symbol *s, struct ast *l);
struct ast *newref(struct symbol *s);
struct ast *newasgn(struct symbol *s, struct ast *v);
struct ast *newnum(double d);
struct ast *newString(struct symbol *s);
struct ast *newDev(struct symbol *ps, struct argsList *l);
struct ast *newContent(int nodetype, struct ast *cond, struct ast *tl, struct ast *tr);


/* Per la definizione di una nuova funzione*/
void defSymRef(struct symbol *name, struct argsList *syms, struct ast *stmts);


/* evaluate an AST */
char * eval(struct ast *);


/* delete and free an AST */
void treefree(struct ast *);


/* Interfaccia con il Lexer */
extern int yylineno; /* Variabile del Lexer */
void yyerror(char *s, ...);


extern int debug;
void dumpast(struct ast *a, int level);

/* Read_file*/
static int index_file=0;

extern FILE *yyin;


