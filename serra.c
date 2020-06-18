#  include <stdio.h>
#  include <stdlib.h>
#  include <stdarg.h>
#  include <string.h>
#  include <math.h>
#  include <ctype.h>
#  include "serra.h"

/* Tabella dei Simboli -> Hashing dei Simboli*/
static unsigned symhash(char *sym) {

  unsigned int hash = 0;
  unsigned c;

  while(c = *sym++)    hash = hash*9 ^ c;

  return hash;
}


struct symbol *search(char* sym){

 /* puntatore alla cella corrispondente al simbolo cercato della tabella dei simboli dichiarata nell'header serra.h */
  struct symbol *symptr = &symtab[ symhash(sym) % DIMHASH ];
  
  int symcount = DIMHASH;      /* viene passata la dimensione della tabella per cercare in tutte le celle di questa il simbolo cercato */

  while(--symcount >= 0) {

    if(symptr->name && !strcmp(symptr->name, sym)) {         /* se trova il simbolo cercato ritorna il puntatore alla cella contenente il simbolo cercato */
        return symptr; 
    }

    if(!symptr->name) {             /* NUOVO SIMBOLO */
      symptr->name = strdup(sym);
      symptr->value = 0;
      symptr->func = NULL;
      symptr->syms = NULL;
      return symptr;
    }

    if(++symptr >= symtab + DIMHASH) {      /* Ricomincia da capo se la cella corrente è l'ultima */
        symptr = symtab;                       
    }
  }
  
  yyerror("Errore: Tabella piena\n");
  abort(); 

}




struct symbol *searchDevice (char* sym){

 /* puntatore alla cella corrispondente al simbolo cercato della tabella dei simboli dichiarata nell'header serra.h */
  struct symbol *symptr = &symtab[ symhash(sym) % DIMHASH ];
  
  int symcount = DIMHASH;      /* viene passata la dimensione della tabella per cercare in tutte le celle di questa il simbolo cercato */

  while(--symcount >= 0) {

    if(symptr->name && !strcmp(symptr->name, sym)) {         /* se trova il simbolo cercato ritorna il puntatore alla cella contenente il simbolo cercato */
        return symptr; 
    }

    if(!symptr->name) {
       return NULL;
    }

    if(++symptr >= symtab + DIMHASH) {      /* Ricomincia da capo se la cella corrente è l'ultima */
        symptr = symtab;                       
    }
  }

  yyerror("Errore: Dispositivo cercato non disponibile...\n");
  abort(); 

}



struct ast *newast(int nodetype, struct ast *l, struct ast *r)
{
  struct ast *a = malloc(sizeof(struct ast));
  
  if(!a) {
    yyerror("Spazio di memoria insufficiente");
    exit(0);
  }
  
  a->nodetype = nodetype;
  a->l = l;
  a->r = r;
  return a;
}



struct ast *newDevice(struct symbol *s)
{
  struct device *a = malloc(sizeof(struct device));
  
  if(!a) {
    yyerror("Spazio di memoria insufficiente");
    exit(0);
  }
    a->nodetype = 'D';
    a->s = s;

  return (struct ast *)a;
}



struct ast *newnum(double d)
{
  struct numval *a = malloc(sizeof(struct numval));
  
  if(!a) {
    yyerror("Spazio di memoria insufficiente");
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
    yyerror("Spazio di memoria insufficiente");
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
    yyerror("Spazio di memoria insufficiente");
    exit(0);
  }
  a->nodetype = '0' + cmptype;
  a->l = l;
  a->r = r;
  return a;
}


struct ast *newfunc(int functype, struct ast *l)
{
  struct funcBuiltIn *a = malloc(sizeof(struct funcBuiltIn));
  
  if(!a) {
    yyerror("Spazio di memoria insufficiente");
    exit(0);
  }
  a->nodetype = 'F';
  a->l = l;
  a->functype = functype;
  //printf("%d", functype);       //DEBUG
  return (struct ast *)a;
}


struct ast *newfuncSystem(int functype)
{
  struct funcBuiltInSystem *a = malloc(sizeof(struct funcBuiltInSystem));
  
  if(!a) {
    yyerror("Spazio di memoria insufficiente");
    exit(0);
  }
  a->nodetype = 'O';
  a->functype = functype;
  //printf("%d", functype);       //DEBUG
  return (struct ast *)a;
}


struct ast *newcall(struct symbol *s, struct ast *l)
{
  struct userfunc *a = malloc(sizeof(struct userfunc));
  
  if(!a) {
    yyerror("Spazio di memoria insufficiente");
    exit(0);
  }
  a->nodetype = 'C';
  a->l = l;
  a->s = s;
  return (struct ast *)a;
}


struct ast *newref(struct symbol *s)
{
  struct symref *a = malloc(sizeof(struct symref));
  
  if(!a) {
    yyerror("Spazio di memoria insufficiente");
    exit(0);
  }
  a->nodetype = 'N';
  a->s = s;
  return (struct ast *)a;
}


struct ast *newasgn(struct symbol *s, struct ast *v)
{
  struct symasgn *a = malloc(sizeof(struct symasgn));
  
  if(!a) {
    yyerror("Spazio di memoria insufficiente");
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
    yyerror("Spazio di memoria insufficiente");
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
    yyerror("Spazio di memoria insufficiente");
    exit(0);
  }
  sl->sym = sym;
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


/* define a function */
void dodef(struct symbol *name, struct argsList *syms, struct ast *func)
{
  if(name->syms) argsListfree(name->syms);
  if(name->func) treefree(name->func);
  name->syms = syms;
  name->func = func;
}



struct ast * callbuiltin(struct funcBuiltIn *);
struct ast * callbuiltinSystem(struct funcBuiltInSystem *);
static char* calluser(struct userfunc *);



char* eval(struct ast *a)
{
  char * v;

  if(!a) {
    yyerror("internal error, null eval");
    return 0;
  }

  switch(a->nodetype) {
    /* Number constant */
  case 'K': 
            v= malloc(sizeof((((struct numval *)a)->number)));
            sprintf(v, "%4.4g", ((struct numval *)a)->number);
            break;
  
  /* String constant */
  case 'C': v= strdup(((struct stringVal *)a)->s->name); break;

    /* name reference */
  case 'N': v = strdup((((struct symref *)a)->s->value)); break;

    /* assignment */
  case '=': v = strdup((((struct symasgn *)a)->s->value = eval(((struct symasgn *)a)->v))); break;



    /* comparisons *
  case '1': v = strdup((eval(a->l) > eval(a->r))? 1 : 0); break;
  case '2': v = strdup((eval(a->l) < eval(a->r))? 1 : 0); break;
  case '3': v = strdup((eval(a->l) != eval(a->r))? 1 : 0); break;
  case '4': v = strdup((eval(a->l) == eval(a->r))? 1 : 0); break;
  case '5': v = strdup((eval(a->l) >= eval(a->r))? 1 : 0); break;
  case '6': v = strdup((eval(a->l) <= eval(a->r))? 1 : 0); break;
  */


  /* control content */
  /* null if/else/do expressions allowed in the grammar, so check for them */
  case 'I': 
    if( eval( ((struct content *)a)->cond) != 0) {
      if( ((struct content *)a)->tl) {
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

  case 'F': v = callbuiltin((struct funcBuiltIn *)a); break;
  
  case 'O': v = callbuiltinSystem((struct funcBuiltInSystem *)a); break;

  case 'U': v = calluser((struct userfunc *)a); break;
  
  case 'D':  break;

  default: printf("internal error: bad node %c\n", a->nodetype);
  }
  return v;
}


struct ast * callbuiltin(struct funcBuiltIn *f)
{

  enum builtFunc functype = f->functype;
  char *v = strdup(eval(f->l));
  struct symbol *symDev;

 switch(functype) {
   case B_print:
     printf("display = ");
     return (struct ast *) v;
     break;
   case B_connect:
     printf("Connessione in corso con il device %s...\nConnesso\n", v);
     return (struct ast *) v;
     break;
   case B_reconnect:
     printf("Riconnessione in corso con il device %s...\nConnesso\n", v);
     return (struct ast *) v;
     break;
   case B_status:
     printf("Richiesta status in corso per il device %s...\n", v);
     symDev= searchDevice(v);

     if(!symDev){
        printf("Dispositivo %s: Trovato-> %s\n", v, symDev-> value);     //INSERIRE ENUM CON CODICE DISPOSITIVO COME HTTP 200 AD ESEMPIO
     }else{
        printf("Dispositivo %s: Non trovato\n", v);  
        return NULL;
     }
     //statusDevice(symDev);
     return (struct ast *) symDev;
     break;
   case B_insertDevice:
     printf("Device inserito con successo...\n");
     return (struct ast *) v;
     break;
   default:
     yyerror("Funzione built-in sconosciuta %d", functype);
     return 0;
 }
}


struct ast * callbuiltinSystem(struct funcBuiltInSystem *f)
{
  enum builtFuncSystem functype = f->functype;

 switch(functype) {
   case B_clear:
     system("clear");
     return 0;
     break;
   default:
     yyerror("Unknown built-in function %d", functype);
 }
}



static char * calluser(struct userfunc *f)
{
  struct symbol *fn = f->s;	/* function name */
  struct argsList *sl;		/* dummy arguments */
  struct ast *args = f->l;	/* actual arguments */
  char *oldval, *newval;	/* saved arg values */
  char *v;
  int nargs;
  int i;

  if(!fn->func) {
    yyerror("call to undefined function", fn->name);
    return 0;
  }

  /* count the arguments */
  sl = fn->syms;
  for(nargs = 0; sl; sl = sl->next)
    nargs++;

  /* prepare to save them */
  oldval = (char *)malloc(nargs * sizeof(char));
  newval = (char *)malloc(nargs * sizeof(char));
  if(!oldval || !newval) {
    yyerror("Out of space in %s", fn->name); return 0;
  }
  
  /* evaluate the arguments */
  for(i = 0; i < nargs; i++) {
    if(!args) {
      yyerror("too few args in call to %s", fn->name);
      free(oldval); free(newval);
      return 0;
    }

    if(args->nodetype == 'L') {        /* if this is a list node */
      sprintf(newval[i], "%d", eval(args->l) );
      args = args->r;
    } else {                           /* if it's the end of the list */
      sprintf(newval[i], "%d", eval(args) );
      args = NULL;
    }
  }
		     
  /* save old values of dummies, assign new ones */
  sl = fn->syms;
  for(i = 0; i < nargs; i++) {
    struct symbol *s = sl->sym;

    sprintf(oldval[i], "%d", s->value );
    sprintf(newval[i], "%d", s->value );

    sl = sl->next;
  }

  free(newval);

  /* evaluate the function */
  v= malloc(sizeof( eval(fn->func)));
  sprintf(v, "%s", eval(fn->func) );
  
  

  /* put the dummies back */
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
  case 'K': case 'N': case 'C':case 'F':case 'O':case 'U':
    break;

  case '=':
    free( ((struct symasgn *)a)->v);
    break;

  case 'I': case 'W': 
    free( ((struct content *)a)->cond);
    if( ((struct content *)a)->tl) free( ((struct content *)a)->tl);
    if( ((struct content *)a)->el) free( ((struct content *)a)->el);
    break;

  default: printf("internal error: free bad node %c\n", a->nodetype);
  
  }
  
  free(a); /* always free the node itself */

}

void yyerror(char *s, ...)
{
  va_list ap;
  va_start(ap, s);

  fprintf(stderr, "%d: error: ", yylineno);
  vfprintf(stderr, s, ap);
  fprintf(stderr, "\n");
}



int main()
{
  printf("> "); 
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


