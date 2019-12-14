# Telemetria
## I - Specifica del problema
Si supponga di dover progettare un programma per la gestione di un sistema di telemetria di una
automobile. Il sistema rileva una serie di parametri di funzionamento che vengono misurati una
volta al secondo. I dati raccolti periodicamente vengono organizzati in un file di log (in formato testo)
nel quale ad ogni riga sono associati i seguenti valori (si assumano campi separati da tabulazione o
spazio):
* Tempo: un numero intero che rappresenta il tempo a cui e stata effettuata la misurazione,
espresso in secondi trascorsi a partire da un istante zero in cui viene fatta iniziare la
rilevazione.
* Velocita: un numero reale che rappresenta la velocita del veicolo (Km/h).
* Accelerazione: un numero reale che rappresenta l’accelerazione del veicolo (m/s 2 ).
* Giri motore: un numero intero che rappresenta il numero di giri del motore.
* Angolo di sterzata: un numero intero (compreso tra -360 e +360) che rappresenta l’angolo
dello sterzo.

Si scriva un programma ANSI C che esegue le seguenti elaborazioni:

1. Acquisisce il file e memorizza le relative informazioni in una opportuna struttura dati.
2. Calcola per ognuno dei parametri (escluso il tempo) il valore massimo, minimo, medio e
mediano.
3. Ordina il file sulla base di uno dei parametri (escluso il tempo) indicato dall’utente.
4. Ricerca e restituisce il record (ovvero le informazioni contenute in una o più righe del file) in
corrispondenza del quale uno dei parametri scelto dall’utente (escluso il tempo) ha avuto un
determinato valore. Ad esempio: se l’utente chiede in quali istanti la velocità e uguale a 32.5
Km/h, il programma deve restituire le informazioni contenute nella riga 2 (2, 32.5, 0.32, 950,
0.00).

Per quanto riguarda l’analisi teorica si deve fornire la complessità corrispondente ad ognuna delle
seguenti operazioni: calcolo delle statistiche (massimo, minimo, media, mediana); ordinamento;
ricerca. Oltre all’analisi teorica della complessità si deve effettuare uno studio sperimentale della
stessa per le operazioni specificate al punto precedente. Come suggerimento si può operare
generando un numero N di rilevazioni casuali. L’analisi sperimentale deve quindi valutare la
complessità al variare di N.

E' possibile trovare le seguenti informazioni all'interno della [relazione](https://github.com/elpanas/Telemetria/blob/master/relazione.pdf):

**II - Analisi del problema**

**III - Progettazione dell’algoritmo**

**IV - Implementazione dell’algoritmo**

**V - Testing del programma**

**VI - Valutazione della complessità** (Teorica e sperimentale)
