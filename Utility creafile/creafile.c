/***************************************************************/
/* programma per la creazione di un file dat */
/***************************************************************/

/*****************************/
/* inclusione delle librerie */
/*****************************/

#include <stdio.h>
#include <stdlib.h>

/***********************************/
/* dichiarazione delle funzioni */
/***********************************/

int acquisisciDati();
void creaFile();
float generaCasuale(float a,
                    float b);

/***********************************/
/* definizione delle funzioni */
/***********************************/

/** FUNZIONE MAIN **/
int main(void)
{
    /* dichiarazione variabili */
    int esito_lettura,
        rilevazioni;
    char c;

    /* acquisisce i dati in input da tastiera */
    rilevazioni = acquisisciDati();

    do
        {
         printf("\nLa creazione del file sovrascrivera' eventuali altri\n");
         printf("\nSi desidera continuare? [S/N]\n");
         esito_lettura = scanf("%c",
                               &c);

         if ((esito_lettura != 1) || (c != 'S' && c != 'N'))
             printf("Errore\n");

         while (getchar() != '\n');

        }while ((esito_lettura != 1) || (c != 'S' && c != 'N'));

    if (c == 'S')
        creaFile(rilevazioni);
    else
        printf("\nBuona giornata\n");

    return 0;
}

/** ACQUISISCE I DATI **/
int acquisisciDati()
{
    int esito_lettura,  /* lavoro: variabile per la validazione */
        r;              /* output: numero di rilevazioni */

    do
        {
         printf("\nDa quanti elementi vuoi che sia composto il file di telemetria?\n");
         esito_lettura = scanf("%d",
                               &r);

         if (esito_lettura != 1 || r <= 0)
             printf("Errore\n");

         while (getchar() != '\n');

        }while (esito_lettura != 1 || r <= 0);

    return(r);
}

/** CREA IL FILE **/
void creaFile(int r)    /* input: numero di rilevazioni da inserire nel file */
{
    FILE *file_input;   /* lavoro: puntatore al file */
    int inserito,       /* lavoro: validazione inserimento */
        tempo;          /* lavoro: secondi delle singole rilevazioni */

    /* apre il file in lettura e assegna un puntatore */
    file_input = fopen("log.txt",
                       "w");

    /* in caso di errore, mostra un messaggio*/
    if (!file_input)
        printf("Errore apertura file\n");
    else
        {
         tempo = inserito = 1;
         /* scrive nel file la prima riga d'intestazione*/
         fputs("Tempo\t\tVelocita\tAccelerazione\tGiri\t\tAngolo di sterzata",
               file_input);
         do
            {
             if (fprintf(file_input,
                         "\n%d\t\t%.2f\t\t%.2f\t\t%d\t\t%.2f",
                         tempo++,
                         generaCasuale(25.5,50),
                         generaCasuale(-0.5,0.5),
                         (rand() % 300 + 800),
                         generaCasuale(-360,360)) < 0)
                  inserito = 0;

             /* ripete finch� non � stata raggiunta la fine del file o in caso di errore */
            }while ((tempo <= r) && (inserito != 0));

        /* chiude il file */
        fclose(file_input);

        if (inserito)
            printf("\nFile creato con successo...\n\nBuona giornata\n");
        }
}

/** GENERA UN VALORE CASUALE **/
float generaCasuale(float a,
                    float b)
{
  float r;
  r = ( (float)(rand()) ) / RAND_MAX;
  r *= (b-a);
  r += a;
  return r;
}
