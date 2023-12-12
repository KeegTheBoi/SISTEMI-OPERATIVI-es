/* file:  soccorsi_semplice.c */

#ifndef _THREAD_SAFE
	#define _THREAD_SAFE
#endif
#ifndef _POSIX_C_SOURCE
	#define _POSIX_C_SOURCE 200112L
#endif

#include "printerror.h"

#include <unistd.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <stdint.h>
#include <inttypes.h>
#include <pthread.h> 

#include "DBGpthread.h"

#define NUMOPEPALI 1
#define NUMMARTELLATORI 2

/* variabili globali da proteggere */
pthread_mutex_t  mutex;

pthread_cond_t   condMartella;
pthread_cond_t condAspettaFine;

/* aggiungete le vostre variabili globali */

int numPali = 0;
int martella = 0;
int finePali = 0;

void* TienePalo(void* args) {
    char Plabel[128];
	intptr_t indice;

	indice=(intptr_t)args;
	sprintf(Plabel,"PALO [%" PRIiPTR "]",indice);

    while(1) {
        DBGpthread_mutex_lock(&mutex, Plabel);
        if(numPali == 10) {
            printf("[PALO_REST] %s mi riposo \n", Plabel );
		    fflush(stdout);
            sleep(3);
        }
        numPali+=2;
        printf("[ARRIVED] %s prende due pali nPali [%i] \n", Plabel, numPali);
		fflush(stdout);

        sleep(1);

        printf("[DONE] %s piantati pali, Martellare!! \n", Plabel );
		fflush(stdout);

        martella++;

        DBGpthread_cond_broadcast(&condMartella, Plabel);
        
        printf("[WAIT_END] %s aspetta che entrambi i pali siano piantati \n", Plabel );
        fflush(stdout);
        DBGpthread_cond_wait(&condAspettaFine, &mutex, Plabel);
        martella--;
        DBGpthread_mutex_unlock(&mutex, Plabel);

    }

}

void* Martella(void* args) {
    char Plabel[128];
	intptr_t indice;

	indice=(intptr_t)args;
	sprintf(Plabel,"MARTELLA [%" PRIiPTR "]",indice);

    while(1) {
        DBGpthread_mutex_lock(&mutex, Plabel);
       

        while(martella == 0) {
            printf("[WAIT] %s  attendo che mi si impartisca l'ordine di piantare\n", Plabel );
		    fflush(stdout);
            DBGpthread_cond_wait(&condMartella, &mutex, Plabel);
        }


        if(numPali == 4) {
            printf("[REST] %s mi riposo \n", Plabel );
		    fflush(stdout);
            if(indice == 0) {
                sleep(5);
            }
            else if( indice == 1) {
                sleep(7);
            }
            
        }
        
        printf("[START] %s inizio a martellare un palo {NPALI: %i} \n", Plabel , numPali);
		fflush(stdout);
        finePali++;
        DBGpthread_mutex_unlock(&mutex, Plabel);
        sleep(1);
        DBGpthread_mutex_lock(&mutex, Plabel);
        printf("[END] %s ha finito di martellare un palo \n", Plabel );
		fflush(stdout);
        finePali--;
        DBGpthread_mutex_unlock(&mutex, Plabel);
        if(finePali == 0) {
            printf("[SIGNAL] %s entrambi hanno finito!! \n", Plabel );
		    fflush(stdout);
            DBGpthread_cond_signal(&condAspettaFine, Plabel);
        }
        while(finePali > 0) {
            printf("[WAIT_ENDING] %s aspetta che l'altro finisca!! \n", Plabel );
		    fflush(stdout);
            DBGpthread_cond_wait(&condAspettaFine, &mutex, Plabel);
        }
        DBGpthread_mutex_unlock(&mutex, Plabel); 
    }

}




int main ( int argc, char* argv[] ) 
{ 
	pthread_t    th; 
	int  rc;
	intptr_t i;

	/* inizializzazione variabili globali */
	numPali = 0;
    martella = 0;
	rc = pthread_mutex_init(&mutex, NULL); 
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_init failed");
	rc = pthread_cond_init(&condMartella, NULL);
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");
	rc = pthread_cond_init(&condAspettaFine, NULL);
	if(rc) PrintERROR_andExit(rc,"pthread_create failed");

	


	/* inizializzate le vostre variabili globali */



	for(i=0;i<NUMOPEPALI;i++) {
		rc=pthread_create(&th,NULL,TienePalo,(void*)i); 
		if(rc) PrintERROR_andExit(rc,"pthread_create failed");
	}

    for(i=0;i<NUMMARTELLATORI;i++) {
		rc=pthread_create(&th,NULL, Martella,(void*)i); 
		if(rc) PrintERROR_andExit(rc,"pthread_create failed");
	}

	pthread_exit(NULL);
	return(0); 
} 
  
