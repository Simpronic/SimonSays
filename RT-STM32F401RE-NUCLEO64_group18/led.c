#include <stdlib.h> //libreria per funzione rand()

//DEFINIZIONE LED
#define RED_LED_LINE    PAL_LINE( GPIOC, 10U ) // Onboard RED LED
#define YELLOW_LED_LINE    PAL_LINE( GPIOC, 11U ) // Onboard YELLOW LED
#define BLUE_LED_LINE    PAL_LINE( GPIOC, 12U ) // Onboard BLUE LED
#define GREEN_LED_LINE    PAL_LINE( GPIOC, 13U ) // Onboard GREEN LED

int end_game; //Variabile per terminare la partita quando viene sbagliata la sequenza da parte del giocatore (1 = Partita finita)

void gestione_led()
{

    palSetLineMode( RED_LED_LINE, PAL_MODE_OUTPUT_PUSHPULL ); //Modalità di output per il pin PC10
    palSetLineMode( YELLOW_LED_LINE, PAL_MODE_OUTPUT_PUSHPULL ); //Modalità di output per il pin PC12
    palSetLineMode( BLUE_LED_LINE, PAL_MODE_OUTPUT_PUSHPULL ); //Modalità di output per il pin PC14
    palSetLineMode( GREEN_LED_LINE, PAL_MODE_OUTPUT_PUSHPULL ); //Modalità di output per il pin PC15

    palClearLine(RED_LED_LINE);
    palClearLine(YELLOW_LED_LINE);
    palClearLine(BLUE_LED_LINE);
    palClearLine(GREEN_LED_LINE);


  srand(NULL); //Set seed funzione rand()

  int sequenza[20]; //Array per salvare la sequenza di colori
  int contatore = 0; //Numero di led/colori da attivare

  end_game = 0; //Imposto la variabile a 0, quindi comincia la partita

  /*
   Genero la sequenza di colori prima di cominciare la partita
   La sequenza viene passata al ciclo for che scorre il vettore contenente i led da accendere ad ogni round del gioco
   */
  for(int i=0; i<20; i++)
  {
    sequenza[i]=rand()%4;
  }

  while(end_game == 0 && contatore < 20) //Fin quando la partita non termina, genero una nuova sequenza di colori
  {

      for(int j=0; j < contatore+1; j++)
      {
        switch(sequenza[j])
              {
                case 0:
                  palSetLine( RED_LED_LINE ); //Accendo LED Rosso
                  chThdSleepMilliseconds(500);
                  palClearLine( RED_LED_LINE );
                  chThdSleepMilliseconds(500);
                  break;

                case 1:
                  palSetLine( YELLOW_LED_LINE ); //Accendo LED Giallo
                  chThdSleepMilliseconds(500);
                  palClearLine( YELLOW_LED_LINE );
                  chThdSleepMilliseconds(500);
                  break;

                case 2:
                  palSetLine( BLUE_LED_LINE ); //Accendo LED Blu
                  chThdSleepMilliseconds(500);
                  palClearLine( BLUE_LED_LINE );
                  chThdSleepMilliseconds(500);
                  break;

                case 3:
                  palSetLine( GREEN_LED_LINE ); //Accendo LED Verde
                  chThdSleepMilliseconds(500);
                  palClearLine( GREEN_LED_LINE );
                  chThdSleepMilliseconds(500);
                  break;
              }
      }
      contatore++;
      chThdSleepMilliseconds(5000);
  }

}
