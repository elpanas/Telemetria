/***************************************************************/
/* programma per la gestione della telemetria */
/***************************************************************/

/*****************************/
/* inclusione delle librerie */
/*****************************/

#include <stdio.h>
#include <stdlib.h>

/************************************/
/* definizione delle costanti */
/************************************/

#define LUNG_RIGA 70

/************************************/
/* definizione dei tipi */
/************************************/

typedef struct elem_lista
    {
    int tempo,
        giri;
    float velocita,
          accelerazione,
          sterzata;
    struct elem_lista *succ_p, *prec_p;
    } elem_lista_t;

typedef struct elem_ricerca
    {
    int colonna;
    float cercato;
    } elem_ricerca_t;

/***********************************/
/* dichiarazione delle funzioni */
/***********************************/

int acquisisciFile(elem_lista_t**);
int inserisciElemento(elem_lista_t**,
                      int,
                      elem_lista_t*);
float selezionaValori(elem_lista_t*,
                      int);
void visitaLista(elem_lista_t*);
elem_lista_t* ordinaLista(elem_lista_t*,
                          int);
void svuotaLista(elem_lista_t**,
                 int,
                 int);
int rimuoviElemento(elem_lista_t**,
                    int,
                    int);
elem_lista_t* codaLista(elem_lista_t*);
void mostraRisultati(elem_lista_t*,
                     elem_lista_t*,
                     int,
                     int);
float medianoLista(elem_lista_t*,
                   int,
                   int);
float mediaLista(elem_lista_t*,
                 int,
                 int);
void mostraRecord(elem_lista_t*,
                  int,
                  int,
                  float,
                  int);
int menuLista();
int gestioneOrdinamento(elem_lista_t**,
                        elem_lista_t**,
                        int,
                        int);
int datiOrdinamento();
int convertiColonna(char);
void gestioneRicerca(elem_lista_t*);
void datiRicerca(elem_ricerca_t*);
elem_lista_t* cercaElemento(elem_lista_t*,
                            int,
                            float);

/***********************************/
/* definizione delle funzioni */
/***********************************/

/** FUNZIONE MAIN **/
int main(void)
{
    /* dichiarazione variabili */
    elem_lista_t *testa_p = NULL,       /* lavoro: testa della lista */
                 *coda_p = NULL;        /* lavoro: coda della lista */
    int ordine_corr = 1,                /* lavoro: tiene traccia della colonna ordinata */
        lunghezza_lista = 0,            /* lavoro: numero di elementi nella lista */
        colonna,                        /* lavoro: contatore delle colonne */
        scelta;                         /* lavoro: scelta dell'utente */

    /* acquisisce i dati dal file e ne restituisce il numero */
    lunghezza_lista = acquisisciFile(&testa_p);

    if (lunghezza_lista > 0)
        {
         printf("\n*** VISUALIZZA I DATI ***\n\n");

         /* visualizza i dati a schermo */
         visitaLista(testa_p);

         for(colonna = 2; colonna <= 5; colonna++)
             {
               switch(colonna)
                {
                case 2:
                    printf("\n ----- VALORI VELOCITA' ----- \n");
                    break;
                case 3:
                    printf("\n ----- VALORI ACCELERAZIONE ----- \n");
                    break;
                case 4:
                    printf("\n ----- VALORI GIRI ----- \n");
                    break;
                case 5:
                    printf("\n ----- VALORI ANGOLO DI STERZATA ----- \n");
                    break;
                }

               /* ordina la lista in base alla colonna selezionata */
               testa_p = ordinaLista(testa_p,
                                     colonna);

               ordine_corr = colonna;

               /* memorizza la nuova coda */
               coda_p = codaLista(testa_p);

              /* visualizza a schermo tutti i valori calcolati delle diverse colonne */
              mostraRisultati(testa_p,
                              coda_p,
                              colonna,
                              lunghezza_lista);
             }
         do {
            scelta = menuLista();

            switch(scelta)
                 {
                 case 1:
                    printf("\n\n*** ORDINA I DATI ***\n");
                    ordine_corr = gestioneOrdinamento(&testa_p,
                                                      &coda_p,
                                                      lunghezza_lista,
                                                      ordine_corr);
                    break;
                 case 2:
                    printf("\n\n*** CERCA UN VALORE ***\n");
                    gestioneRicerca(testa_p);
                    break;
                 case 3:
                    visitaLista(testa_p);
                    break;
                 case 4:
                    printf("\nBuona giornata\n");
                 }

           }while(scelta != 4);
        }

    return 0;
}

/** ACQUISISCI DATI DA FILE **/
/* acquisisce i dati da file,
   li inserisce in una struttura dati
   e infine in una lista ordinata */
int acquisisciFile(elem_lista_t **testa_p)  /* input: puntatore alla testa della lista */
{
    FILE *file_input;           /* lavoro: file con i dati di telemetria */
    elem_lista_t valori_input;  /* lavoro: contiene i dati di una riga */
    int lunghezza = 0;          /* output: totale elementi inseriti nella lista */
    char riga[LUNG_RIGA];       /* lavoro: contiene i caratteri di una riga */

    /* apre il file in lettura e assegna un puntatore */
    file_input = fopen("log.txt",
                       "r");

    /* in caso di errore mostra un messaggio*/
    if (!file_input)
        printf("Errore apertura file\n");
    else
        {/* legge (e salta) la prima riga del file */
        if (fgets(riga,
                  LUNG_RIGA,
                  file_input) != NULL)
            {
             do
                {/* memorizza i dati di una riga del file in "valori_input" */
                if (fscanf(file_input,
                           "%d%f%f%d%f",
                           &valori_input.tempo,
                           &valori_input.velocita,
                           &valori_input.accelerazione,
                           &valori_input.giri,
                           &valori_input.sterzata) != 5)
                     lunghezza = 0;
                 else /* inserisce i dati in un nuovo elemento della lista */
                     if (inserisciElemento(&(*testa_p),
                                           1,
                                           &valori_input)-1 <= 0)
                        lunghezza = 0;
                     else /* se non ci sono stati errori, incrementa il contatore */
                        lunghezza++;

                /* ripete finchè non è stata raggiunta la fine del file o in caso di errore */
                }while (!feof(file_input) && lunghezza != 0);
            }
        /* chiude il file */
        fclose(file_input);
        }

    return(lunghezza);
}

/** INSERISCI ELEMENTO **/
/* inserisce l'elemento nella giusta
   posizione nella lista */
int inserisciElemento(elem_lista_t **testa_p,       /* input: testa della lista */
                      int colonna,                  /* input: valore numerico della colonna */
                      elem_lista_t *input_p)   /* input: struttura dei valori da inserire */
{
    int inserito;          /* output: esito dell'inserimento */
    elem_lista_t *corr_p,  /* lavoro: tiene traccia della posizione durante lo scorrimento */
                 *prec_p,  /* lavoro: elemento precedente */
                 *nuovo_p; /* lavoro: elemento nuovo */

    int n = 0; /* contatore passi base */

    /* scorre la lista finchè l'elemento corrente è nullo
       oppure il valore in lista è minore di quello di
       input (relazione d'ordine "<") */
    for (corr_p = prec_p = *testa_p;
        ((corr_p != NULL) &&
         (selezionaValori(corr_p,colonna) < selezionaValori(input_p,colonna)));
         prec_p = corr_p,corr_p = corr_p->succ_p)
          n = n + 1 + (colonna+3)*2 + 1; /* costo ciclo for n(1+2d+1) */
                                         /* costo funzione (colonna+2) */

    if(n > 0)
        n = n + 1 + 1; /* costo primo e ultimo controllo ciclo for */

    n++; /* condizione if */

    /* se l'elemento è nullo o se esiste già un record con lo stesso valore di tempo */
    if ((corr_p != NULL) && (corr_p->tempo == input_p->tempo))
        {
         inserito = 0;
         n++; /* costo assegnazione */
        }
    else
        {
        inserito = 1;
        nuovo_p = (elem_lista_t *)malloc(sizeof(elem_lista_t));
        nuovo_p->tempo = input_p->tempo;
        nuovo_p->velocita = input_p->velocita;
        nuovo_p->accelerazione = input_p->accelerazione;
        nuovo_p->giri = input_p->giri;
        nuovo_p->sterzata = input_p->sterzata;
        nuovo_p->succ_p = corr_p;

        if (corr_p == *testa_p)
            {
            nuovo_p->prec_p = NULL;
            *testa_p = nuovo_p;
            }
        else
            {
            prec_p->succ_p = nuovo_p;
            nuovo_p->prec_p = prec_p;
            }

        n = n + 11; /* costo 1 istruzione condizionale if + 10 assegnazioni */
        }

    /* printf("Costo Inserimento: %d\n",n); */

    return(inserito+n);
}

/** SELEZIONA VALORI **/
/* seleziona il giusto valore estratto
   dalla variabile strutturata in input,
   in base alla colonna indicata */
float selezionaValori(elem_lista_t *elem_p, /* input: puntatore ad una variabile strutturata*/
                      int colonna)          /* input: identificatore numerico della colonna desiderata*/
{
    float valore; /* output: valore della colonna desiderata */
    int n = 0;  /* contatore passi base */

    valore = 0.0;
    n++; /* costo assegnazione */

     switch(colonna)
        {
        case 1:                     /* costo condizione */
            valore = elem_p->tempo; /* costo assegnazione */
            break;                  /* costo istruzione */
        case 2:
            valore = elem_p->velocita;
            break;
        case 3:
            valore = elem_p->accelerazione;
            break;
        case 4:
            valore = elem_p->giri;
            break;
        case 5:
            valore = elem_p->sterzata;
            break;
        }

    /* ogni blocco ha 2 istruzioni, quindi colonna + 2 */
    n = n + colonna + 2; /* costo condizione + assegnazione + break */

    /* printf("Costo selezionaValori(): %d\n",n); */

    return(valore);
}

/** VISITA LISTA **/
/* scorre la lista partendo da un'estremità
   e stampando a video tutti i record*/
void visitaLista(elem_lista_t *testa_p) /* input: puntatore alla testa della lista */
{
    elem_lista_t *elem_p;

    printf("Tempo(A)\tVelocita(B)\tAccel.(C)\tGiri(D)\t\tAngolo di sterzata(E)\n");

    for (elem_p = testa_p;
         elem_p != NULL;
         elem_p = elem_p->succ_p)
         printf("%d\t\t%.2f\t\t%.2f\t\t%d\t\t%.2f\n",
                elem_p->tempo,
                elem_p->velocita,
                elem_p->accelerazione,
                elem_p->giri,
                elem_p->sterzata);
}

/** ORDINA LISTA  **/
/* ordina la lista copiando gli elementi
   ordinatamente dalla lista primaria
   in una di appoggio */
elem_lista_t* ordinaLista(elem_lista_t *testa_p,    /* input: testa della lista */
                          int colonna)              /* input: valore numerico di colonna */
{
    elem_lista_t *elem_p,
                 *lista2;
    int n = 0;

    lista2 = NULL;
    n++; /* costo assegnazione */

    for (elem_p = testa_p;
         elem_p != NULL;
         elem_p = elem_p->succ_p)
         {
          /* inserisce l'elemento nella lista d'appoggio */
          n = n + 1 + inserisciElemento(&lista2,
                                        colonna,
                                        elem_p)-1 + 1; /* costo funzione -1 del flag "inserito" */
                                                       /* costo condizione + funzione + incremento */
          free(elem_p);
          n++;
         }

    if (n > 0)
        n = n + 1 + 1; /* costo primo e ultimo controllo ciclo for */

    printf("Costo Ordinamento: %d\n",n);

    return(lista2);
}

/** TROVA CODA DELLA LISTA **/
/*scorre tutti gli elementi della lista
  fino alla fine e memorizza il valore del
  l'ultimo elemento */
elem_lista_t* codaLista(elem_lista_t *testa_p) /* input: puntatore alla testa della lista */
{
    elem_lista_t *elem_p;   /* output: elemento della lista */

    for (elem_p = testa_p;
         elem_p != NULL && elem_p->succ_p != NULL;
         elem_p = elem_p->succ_p);

    return(elem_p);
}

/** MOSTRA RISULTATI **/
/* mostra a video i valori massimi, minimi,
   medi, mediani con i rispettivi record */
void mostraRisultati(elem_lista_t *testa_p, /* input: puntatore alla testa della lista */
                     elem_lista_t *coda_p,  /* input: puntatore alla coda della lista */
                     int colonna,           /* input: valore numerico di colonna */
                     int lunghezza_lista)   /* input: numero di elementi nella lista */
{
  float massimo,    /* lavoro: valore massimo della colonna */
        minimo,     /* lavoro: valore minimo della colonna */
        media,      /* lavoro: valore medio della colonna */
        mediano;    /* lavoro: valore mediano della colonna */
  int n = 0;    /* contatore passi base */

  /* seleziona il valore massimo della colonna indicata */
  massimo = selezionaValori(coda_p,
                            colonna);
  n = 1 + (colonna+3); /* costo assegnazione + funzione */
  printf("Costo Massimo: %d\n",n);

  /* seleziona il valore minimo della colonna indicata */
  minimo = selezionaValori(testa_p,
                           colonna);
  n = 1 + (colonna+3); /* costo assegnazione + funzione */
  printf("Costo Minimo: %d\n",n);

  /* restituisce la media dei valori della colonna indicata */
  media = mediaLista(testa_p,
                     lunghezza_lista,
                     colonna);

  /* restituisce la mediana dei valori della colonna indicata */
  mediano = medianoLista(testa_p,
                         lunghezza_lista,
                         colonna);

  /* ----- MEDIANO E MASSIMO ----- */
  if (colonna != 4)
      {
       printf("\nMediano: %.2f\n",mediano);
       printf("\nMassimo: %.2f\n",massimo);
      }
  else
      {
       printf("\nMediano: %d\n",(int)mediano);
       printf("\nMassimo: %d\n",(int)massimo);
      }

  /* restituisce tutti i record in cui è presente il valore massimo */
  mostraRecord(coda_p,
               colonna,
               1,
               massimo,
               1);

  /* ----- MEDIO E MINIMO ----- */
  if (colonna != 4)
      {
       printf("\nMedio: %.2f\n",media);
       printf("\nMinimo: %.2f\n",minimo);
      }
  else
      {
       printf("\nMedio: %d\n",(int)media);
       printf("\nMinimo: %d\n",(int)minimo);
      }

  /* restituisce tutti i record in cui è presente il valore minimo */
  mostraRecord(testa_p,
               colonna,
               1,
               minimo,
               0);
}

/** CALCOLA MEDIA **/
float mediaLista(elem_lista_t *testa_p, /* input: puntatore alla testa della lista */
                 int lunghezza_lista,   /* input: numero di elementi attualmente in lista */
                 int colonna)           /* input: valore numerico della colonna */
{
    elem_lista_t *elem_p;   /* lavoro: puntatore all'elemento di appoggio */
    float media;            /* output: memorizza il risultato del calcolo*/
    int n = 0;  /* contatore passi base */

    media = 0.0;
    n++; /* costo assegnazione */

    for (elem_p = testa_p;
         elem_p != NULL;
         elem_p = elem_p->succ_p)
         {
           media += selezionaValori(elem_p,colonna);
           n = n + 1 + 1 + 1 + (colonna+3); /* costo assegnazione + funzione + incremento */
         }

    media = media / ((float)lunghezza_lista);
    n++; /* costo assegnazione */

    if (n > 0)
        n = n + 1 + 1; /* costo primo e ultimo controllo ciclo for */

    printf("Costo Media: %d\n",n);

    return(media);
}

/** CALCOLA MEDIANO **/
float medianoLista(elem_lista_t *testa_p,   /* input: puntatore alla testa della lista */
                   int lunghezza_lista,     /* input: numero di elementi della lista */
                   int colonna)             /* input: valore numerico della colonna */
{
    elem_lista_t *elem_p;   /* lavoro: puntatore all'elemento di appoggio */
    int contatore = 0,      /* lavoro: conta gli elementi */
        termina = 0;        /* lavoro: segnala che la mediana è stata trovata */
    float valore1 = 0.0,    /* lavoro: 1° valore in caso di lista pari */
          valore2 = 0.0,    /* lavoro: 2° valore in caso di lista pari */
          mediana = 0.0;    /* output: memorizza il risultato del calcolo */

    int n = 0; /* contatore passi base */

    n = 5; /* costo assegnazioni */

        for (elem_p = testa_p;
             elem_p != NULL && termina == 0;
             elem_p = elem_p->succ_p)
             {
              contatore++;
              n = n + 1 + 1 + 1 + 1; /* costo condizione for + incremento + contatore + condizione if*/
              if ((lunghezza_lista % 2) != 0)
                  {/* In caso la lista contenga un numero dispari di elementi */
                   n++; /* costo condizione if */
                   if (contatore == (lunghezza_lista + 1) / 2)
                       {
                        mediana = selezionaValori(elem_p,
                                                  colonna);
                        termina = 1;
                        n = n + 1 + (colonna+3); /* costo assegnazione + funzione */
                       }
                  }
              else
                  {/* In caso la lista contenga un numero pari di elementi */
                   n++; /* costo condizione if */
                   if (contatore == lunghezza_lista / 2)
                       {
                        valore1 = selezionaValori(elem_p,
                                                  colonna);

                        valore2 = selezionaValori(elem_p->succ_p,
                                                 colonna);

                        mediana = (valore1 + valore2) / 2;

                        termina = 1;

                        n = n + 4 + (colonna+3)*2; /* costo 4 assegnazioni + 2 funzioni */
                       }
                   }
               }

    if (n > 5)
        n = n + 1 + 1; /* costo primo e ultimo controllo ciclo for */

    printf("Costo Mediana: %d\n",n);

    return(mediana);
}

/** MOSTRA RECORD **/
/* cerca e restituisce tutti i record
   in cui si trova l'elemento */
void mostraRecord(elem_lista_t *estremo,    /* input: puntatore ad una delle estremità */
                  int colonna,              /* input: valore numerico di colonna */
                  int maxmin,               /* input: indica se si stanno mostrando solo i valori max (min) */
                  float cercato,            /* input: valore cercato */
                  int inverti)              /* input: flag per capire in quale direzione scorrere */
{
    elem_lista_t *elem_p = estremo; /* lavoro: puntatore all'elemento di appoggio */
    int contatore = 0,              /* lavoro: conta i record trovati */
        termina = 0;                /* lavoro: flag per capire se uscire dal ciclo */

    while(elem_p != NULL && termina == 0)
        {
        if (selezionaValori(elem_p,colonna) == cercato)
            {
             contatore++;
             /* stampa a video i record dove è presente il valore cercato */
             printf("%d(%d,%.2f,%.2f,%d,%.2f)\n",
                    contatore,
                    elem_p->tempo,
                    elem_p->velocita,
                    elem_p->accelerazione,
                    elem_p->giri,
                    elem_p->sterzata);
            }
        else if (maxmin)
                 termina = 1;

        elem_p = (inverti) ? elem_p->prec_p : elem_p->succ_p;
        }
}

/** MENU **/
int menuLista()
{
    int scelta,         /* output: scelta dell'utente */
        esito_lettura;  /* lavoro: validazione input */

    printf("\n\n\t***MENU OPERAZIONI***\n");

    do {
        esito_lettura = scelta = 0;

        printf("\nInserire il numero dell'operazione desiderata:\n");
        printf("\t1 - Ordinare la lista\n");
        printf("\t2 - Cercare un elemento\n");
        printf("\t3 - Visualizzare la lista\n");
        printf("\t4 - Terminare l'esecuzione\n");
        esito_lettura = scanf("%d",
                              &scelta);

        while (getchar() != '\n');

       }while(esito_lettura != 1 && (scelta < 1 || scelta > 4));

    return(scelta);
}

/** GESTISCE GLI ALGORITMI PER L'ORDINAMENTO **/
int gestioneOrdinamento(elem_lista_t **testa_p, /* input: testa della lista */
                        elem_lista_t **coda_p,  /* input: coda della lista */
                        int lunghezza_lista,    /* input: lunghezza della lista */
                        int ordine_corr)        /* input/output: ordinamento corrente */
{
    int esito_lettura,              /* lavoro: validazione */
        colonna;                    /* lavoro: valore numerico della colonna da ordinare */
    char scelta;                    /* lavoro: scelta dell'utente */

    /* acquisisce i parametri dall'utente */
     do {
         colonna = datiOrdinamento();

         if (colonna != ordine_corr && lunghezza_lista > 1)
             {
               /* ordina la lista in base alla colonna desiderata */
               *testa_p = ordinaLista(*testa_p,
                                    colonna);

               ordine_corr = colonna;

               /* memorizza la nuova coda */
               *coda_p = codaLista(*testa_p);
             }

        /* visualizza i dati appena ordinati */
        visitaLista(*testa_p);

        do {
            while (getchar() != '\n');
            printf("\nVuoi effettuare un altro ordinamento? [S/n]\n");
            esito_lettura = scanf("%c",
                                  &scelta);

           } while(esito_lettura != 1);

       while (getchar() != '\n');

     } while(scelta == 'S' || scelta == 's');

    return(ordine_corr);
}

/** ACQUISIZIONE PARAMETRI DI ORDINAMENTO **/
/* richiede, memorizza e restituisce
   i dati di input da tastiera */
int datiOrdinamento()
{
    char lettera_colonna;   /* lavoro: valore letterale di colonna */
    int esito_lettura,      /* lavoro: variabile per la validazione  */
        colonna;            /* output: valore numerico di colonna */

    do
        {
         printf("\nInserire la colonna in base alla quale effettuare l'ordinamento: B, C, D, E\n");
         esito_lettura = scanf("%1c",
                               &lettera_colonna);

         /* converte il valore letterale della colonna in numerico */
         colonna = convertiColonna(lettera_colonna);

         if (esito_lettura != 1 || colonna == 0)
            {
             printf("Errore: input non accetabile\n");
             printf("\n\nPremere Invio per continuare...\n");
             while (getchar() != '\n');
            }

        }while (esito_lettura != 1 || colonna == 0);

    return(colonna);
}

/** CONVERTI COLONNA **/
/* converte l'identificatore della colonna
   da lettera a numero */
int convertiColonna(char lettera_colonna)   /* input: identificatore letterale della colonna */
{
    int colonna;    /* output: identificatore numerico della colonna */

     switch(lettera_colonna)
        {
        case 'B':
        case 'b':
            colonna = 2;
            break;
        case 'C':
        case 'c':
            colonna = 3;
            break;
        case 'D':
        case 'd':
            colonna = 4;
            break;
        case 'E':
        case 'e':
            colonna = 5;
            break;
        default:
            colonna = 0;
            break;
        }

    return(colonna);
}

/** GESTISCE GLI ALGORITMI DI RICERCA **/
void gestioneRicerca(elem_lista_t *testa_p) /* input: testa della lista */
{
    elem_ricerca_t param_ricerca;   /* lavoro: parametri di ricerca */
    int esito_lettura;              /* lavoro: validazione */
    char scelta;                    /* lavoro: scelta dell'utente */

    do {
         esito_lettura = 0;
         /* acquisisce i parametri dall'utente */
         datiRicerca(&param_ricerca);

         /* restituisce l'elemento cercato, se presente */
         if (cercaElemento(testa_p,
                           param_ricerca.colonna,
                           param_ricerca.cercato) != NULL)
             /* mostra tutti i record in cui appare l'elemento cercato */
             mostraRecord(testa_p,
                          param_ricerca.colonna,
                          0,                        /* flag maxmin */
                          param_ricerca.cercato,
                          0);                       /* flag inverti */
         else
            printf("Elemento non trovato\n");

         do {
             printf("\nSi desidera effettuare un'altra ricerca? [S/n]\n");
             esito_lettura = scanf("%c",
                                   &scelta);

            } while(esito_lettura != 1);

            while (getchar() != '\n');

        } while (scelta == 'S' || scelta == 's');
}

/** ACQUISIZIONE PARAMETRI DI RICERCA **/
/* richiede, memorizza e restituisce
   i dati di input da tastiera */
void datiRicerca(elem_ricerca_t *param_ricerca) /* input/output: contiene i dati per effettuare la ricerca */
{
    int esito_lettura;      /* lavoro: variabile per la validazione */
    char lettera_colonna;   /* lavoro: valore letterale della colonna */

    do
        {
         printf("\nInserire la colonna nella quale cercare il valore: B, C, D, E\n");
         esito_lettura = scanf("%1s",
                               &lettera_colonna);

         /* converte il valore letterale della colonna in numerico */
         param_ricerca->colonna = convertiColonna(lettera_colonna);

         if (esito_lettura < 1 || param_ricerca->colonna == 0)
             {
              printf("Errore: input non accettabile\n");
              printf("\nPremere Invio per continuare...\n");
              while (getchar() != '\n');
             }

         while (getchar() != '\n');

        }while (esito_lettura < 1 || param_ricerca->colonna == 0);

    do
        {
         printf("\nInserire il valore da cercare: \n");
         esito_lettura = scanf("%f",
                               &param_ricerca->cercato);

         if (esito_lettura < 1)
             {
              printf("Errore: input non accetabile\n");
              printf("\nPremere Invio per continuare...\n");
              while (getchar() != '\n');
             }

         while (getchar() != '\n');

        }while (esito_lettura < 1);
}

/** CERCA ELEMENTO **/
/* cerca e restituisce tutti i record
   in cui si trova l'elemento */
elem_lista_t* cercaElemento(elem_lista_t *testa_p,    /* input: puntatore ad una delle estremità */
                            int colonna,              /* input: valore numerico di colonna */
                            float cercato)            /* input: valore cercato */
{
    elem_lista_t *elem_p;   /* output: puntatore all'elemento di appoggio */
    int n = 0;  /* contatore passi base */

    for(elem_p = testa_p;
        elem_p != NULL && selezionaValori(elem_p,colonna) != cercato;
        elem_p = elem_p->succ_p)
        n = n + 1 + (colonna+3) + 1; /* costo ciclo for n(1+d+1) */

    if(n > 0)
        {
         n = n + 1 + 1; /* costo primo e ultimo controllo ciclo for */
         if(elem_p->succ_p == NULL)
            n = n + 2 + (colonna+3); /* costo primo controllo ciclo for + condizione + funzione */
        }
    else
        n = 2 + (colonna+3); /* costo primo controllo ciclo for + condizione + funzione */

    printf("Costo Ricerca: %d\n",n);

    return(elem_p);
}
