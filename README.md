**Proposta di tesina per la Materia**

**Linguaggi e Traduttori**


+--------------------------------------+--------------------------------------+
| **Allievi Ingegneri**                | **Alla Cortese Attenzione**          |
|                                      |                                      |
| Gaetano La Bua                       | Del Prof. Ing. A. Chella             |
|                                      |                                      |
| Mario Caruso                         | Ing. F. Lanza                        |
|                                      |                                      |
| Vincenzo La Mantia                   |                                      |
|                                      |                                      |
+--------------------------------------+--------------------------------------+

**Oggetto**: Proposta di Tesina per la Materia Linguaggi e Traduttori

Oggi, sempre piu' spesso, si sente parlare di **Industria 4.0**.
Questo termine indica una tendenza verso l'[automazione
industriale](https://it.wikipedia.org/wiki/Automazione_industriale) che
integra alcune nuove
[tecnologie](https://it.wikipedia.org/wiki/Tecnologia) produttive per
migliorare le condizioni di
[lavoro](https://it.wikipedia.org/wiki/Lavoro), creare nuovi modelli di
business e aumentare la
[produttività](https://it.wikipedia.org/wiki/Produttivit%C3%A0) e la
qualità produttiva degli impianti.

L’idea del Team è quella di cercare di rendere accessibile la
trasformazione tecnico-organizzativa dell’automazione alla portata di
tutte le piccole e grandi aziende agricole; tale obiettivo sarà
raggiunto sviluppando un linguaggio ad alto livello che consentirà con
una minima formazione di poter gestire gli impianti.

Il linguaggio disporrà di funzionalità per l’automazione di una serra di
piccole-medie dimensioni e inoltre permetterà di creare funzioni da
parte dell’utente, definendo delle funzionalità aggiuntive
personalizzate.

 

*(
dettagli:*[*https://agronotizie.imagelinenetwork.com/vivaismo-e-sementi/2017/12/04/le-tecniche-applicate-alle-serre-e-alle-colture-protette-2-parte/56638*](https://agronotizie.imagelinenetwork.com/vivaismo-e-sementi/2017/12/04/le-tecniche-applicate-alle-serre-e-alle-colture-protette-2-parte/56638)*)*

*Il sistema dovrà essere in grado di gestire seguenti operazioni di
automazione:*

*- condizionamenti*

*- riscaldamento/raffreddamento del terreno/aria*

*- coltivazione*

*- annaffiamento*

*- rasatura del prato (giardino) ecc*

 

 

 

**Tipi:**

·         Tipi base del linguaggio:

o   Stringhe

o   Interi (32 Bit)

o   Reali (64 Bit)

o   Boolean (true, false) (da levare?)

·         Tipi composti:

o   Time (tipo prodotto cartesiano: identifica giorno mese orario nel
formato giorno, mese, ora, anno: 9 Marzo 2012 11:00 : int char int int
int) (da levare?)

o   Mapping (array) / funzioni (condizionamento, annaffiamento,
coltivazione, riscaldamento e altre da pensare): il programmatore deve
essere in grado di creare funzioni autonome per definire il proprio
metodo di irrigazione regolarizzando il riscaldamento del terreno. In
alternativa possono essere definite funzioni più basso livello per
valutare per esempio la temperatura dell’area ecc. in base a ciò che
l’utente vuole fare ecc. (da levare o li metteremo?)

o   Dispositivo/Device:

§  Struttura:

·         Stringa -\> Nome Dispositivo

·         Posizione (opzionale)-\> posizione intesa all’interno di una
rete Lista di puntatori a Periferica -\> Contiene i riferimenti ai tipi
sensore definiti sotto

·         Boolean -\> Dispositivo Acceso o Spento (true, false)

 

o   Periferica:

§  Struttura:

·         Stringa -\> Tipo Sensore o Attuatore

·         Boolean -\> Sensore/periferica (true, false)->status

·         Stringa -\> Codice periferica

 
(periferica e device alla fine dobbiamo lasciare solo device?)
 

**Costrutti:**

if then else, do-while, while,interval between, altri

I costrutti if,else, then, do-while ecc sono ovviamente fondamentali nel linguaggio di programmazione. Tramite questi l'utente potrà avviare operazioni decisionali del tipo:
- se i device sono accesi allora effettua operazioni di potatura
- se il device è raggiungibile (ping) allora attiva il device
E analoghe operazioni.

**Idee di funzionamento:**
Il sistema prevede l'utilizzo di una rete di device. L'utente tramite apposite funzioni previste dal linguaggio di programmazione avrà la possibilità di attivare tali device. Tramite i device una volta attivati l'utente potrà prevedere le operazioni analizzate sopra: giardinaggio, annaffia ecc. Sul device le operazioni previste sono le seguenti:
1. Inserimento di un nuovo device nel sistema di giardinaggio
2. Collegamento e sincronizzazione di un device con una rete di device
3. Connettere il device: effettua una sorta di ping per verificare se effettivamente il device è attivo e puo essere attivato per le operazioni di giardinaggio
4. Una volta inserito il device potrà essere attivato tramite una funzione di accensione
5. O spento tramite una funzione di spegnimento
6. Funzioni che restituiscono lo stato dei device: funzioni generiche che si occupano di restituire le caratteristiche dei device attivi nel sistema di giardinaggio
7. Funzioni generali:
- print: stampa di stringhe, numeri ecc.

**Utilizzatori finali e ambito applicativo****:**
Gli utilizzatori finali saranno utenti di giardinaggio. Il linguaggio deve essere facilemente fruibile anche a gente poco esperte in ambito di computer. Per tutti i sistemi dove sarà necessario avviare un operazione di giardinaggio automatico il sistema dovrà essere accessibile

** **

** **

**Problemi legati alla robotica:**

 

 

 

* * * * *
 



**Manuale di istruzione**

\
 \

Elenco comandi base con esempi: {.western}
-------------------------------

-   Inserimento Stringa:

    \> “Nuova Stringa” [INVIO]

    -   newString return struct ast \* pointerSimbolo

    -   Esempio: “Sono una Stringa”

\
 \

-   Stampa Stringa:

    \> print “Stringa” [ INVIO ]

    -   callbuiltin return char \* pointerSymbol

    -   Esempio: print “Ciao sono una Stringa” [ INVIO ]

    -   N.B. Anche l’inserimento dell’istruzione sopra produce la
        stampa.

-   Inserimento Sempice Nuovo Dispositivo:

    \> newDev “Nome Device” [INVIO]

    -   newDev return struct ast \* pointerDevice

    -   Esempio: newDevice “dev1” [ INVIO ]

-   Inserimento con Collegamenti a Dispositivi di Nuovo Dispositivo:

    \> newDev “Nome Device” → [ “Nome Device 1”, “Nome Device 2”, …] [
    INVIO ]

    -   newDev return struct ast \* pointerDevice

    -   Esempio: newDevice “dev1” → [“dev2”, “dev3”, “dev4”] [ INVIO ]

-   Richiesta Status con Dispositivo:

    \> status “Nome Device” [ INVIO ]

    -   callbuiltin return struct ast \* pointerSymbol

    -   Esempio: status “dev1” [ INVIO ]

        ATTENZIONE: funzione da modificare e completare, bisogna far si
        che restituisca il valore dell’elemento status della struct
        device

-   Richiesta Connessione con Dispositivo:

    \> connect “Nome Device” [ INVIO ]

    -   callbuiltin return struct ast \* pointerSymbol

    -   Esempio: connect “dev1” [ INVIO ]

\
 \

\
 \

-   Richiesta Riconnessione con Dispositivo:

    \> reconnect “Nome Device” [ INVIO ]

    -   callbuiltin return struct ast \* pointerSymbol

    -   Esempio: reconnect “dev1” [ INVIO ]

-   Richiesta Riconnessione con Dispositivo:

    \> reconnect “Nome Device” [ INVIO ]

    -   callbuiltin return struct ast \* pointerSymbol

    -   Esempio: reconnect “dev1” [ INVIO ]

\
 \

\
 \
 - Accensione status dispositivo
 
    \> sintassi: switchOn "StringaNomeDevice"

    -   callbuiltin return struct ast \* pointerSymbol

    -   Esempio: > switchOn "dev1"  [INVIO]
    
           
 - Off status dispositivo
 
    \> sintassi: switchOff "StringaNomeDevice"

    -   callbuiltin return struct ast \* pointerSymbol

    -   Esempio: Esempio: > switchOff "dev1"  [INVIO]

 - Accensione dello status del device per un certo intervallo di tempo:
 
    \> sintassi: switchOn "NomeDevice,Secondi"  ->note: è un unica stringa dove il primo parametro identicia il device da accendere il secondo per quanti         secondi deve stare acceso.E' un unica stringa che prima della virgola ha il nome del device e dopo la virgola un intero: ex: ("pippo,10", "paperino,20")..

    -   callbuiltin return struct ast \* pointerSymbol

    -   Esempio: > interval "pippo,10"  [INVIO]
           
           
"diagnostic"

"archive"



\
 \

