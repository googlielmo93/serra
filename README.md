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

o   Boolean (true, false)

·         Tipi composti:

o   Time (tipo prodotto cartesiano: identifica giorno mese orario nel
formato giorno, mese, ora, anno: 9 Marzo 2012 11:00 : int char int int
int)

o   Mapping (array) / funzioni (condizionamento, annaffiamento,
coltivazione, riscaldamento e altre da pensare): il programmatore deve
essere in grado di creare funzioni autonome per definire il proprio
metodo di irrigazione regolarizzando il riscaldamento del terreno. In
alternativa possono essere definite funzioni più basso livello per
valutare per esempio la temperatura dell’area ecc. in base a ciò che
l’utente vuole fare ecc.

o   Dispositivo:

§  Struttura:

·         Stringa -\> Nome Dispositivo

·         Posizione (opzionale)-\> posizione intesa all’interno di una
rete Lista di puntatori a Periferica -\> Contiene i riferimenti ai tipi
sensore definiti sotto

·         Boolean -\> Dispositivo Acceso o Spento (true, false)

 

o   Periferica:

§  Struttura:

·         Stringa -\> Tipo Sensore o Attuatore

·         Boolean -\> Sensore/periferica (true, false)

·         Stringa -\> Codice periferica

 

 

**Costrutti:**

if then else, do-while, while,interval between, altri



**Utilizzatori finali e ambito applicativo****:**

** **

** **

**Problemi legati alla robotica:**

 

 

 

* * * * *
 

