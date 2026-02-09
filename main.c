#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "prodcons.h"

void Produttore(MonitorVettoreCircolare *p, MonitorBufferSingolo *b);
void Consumatore(MonitorVettoreCircolare *p, MonitorBufferSingolo *b);

int main() {


	key_t id_circolare = shmget(IPC_PRIVATE, sizeof(MonitorVettoreCircolare), IPC_CREAT | 0664);

    if(id_circolare < 0){

        perror("errore shmget coda circolare");
        exit(1);
    }

    MonitorVettoreCircolare * p = (MonitorVettoreCircolare *) shmat(id_circolare,0,0);

    if(p == (void*)-1){

        perror("errore shmat monitor coda circolare");
        exit(1);
    }

	/* TBD: Allocare l'oggetto MonitorVettoreCircolare in memoria condivisa */

    init_monitor_circolare(p);



 	key_t id_singolo=shmget(IPC_PRIVATE, sizeof(MonitorBufferSingolo), IPC_CREAT|0664);

    if(id_singolo < 0){

        perror("errore shmget buffer singolo");
        exit(1);
    }

    MonitorBufferSingolo * b = (MonitorBufferSingolo *) shmat(id_singolo, 0, 0);
    
    if(b == (void*)-1){

        perror("errore shmat monitor buffer singolo");
        exit(1);
    }

	/* TBD: Allocare l'oggetto MonitorBufferSingolo in memoria condivisa */;

    init_monitor_buffer_singolo(b);
	

    /* TBD: Creare un processo figlio, che esegua la funzione "Produttore()" */
    
    pid_t pid;
	
	pid = fork();
    if (pid == 0) {
        printf("[PROCESSO PRODUTTORE] Avviato (PID: %d)\n", getpid());
        Produttore(p, b);
        exit(0);
    }else if (pid < 0) {
        perror("Errore fork produttore");
        exit(1);
    }
	
    /* TBD: Creare un processo figlio, che esegua la funzione "Consumatore()" */

    pid = fork();
    if (pid == 0) {
        // Codice del figlio Consumatore
        printf("[PROCESSO CONSUMATORE] Avviato (PID: %d)\n", getpid());
        Consumatore(p, b);
        exit(0); // IMPORTANTE: Termina il processo figlio
    } else if (pid < 0) {
        perror("Errore fork consumatore");
        exit(1);
    }

    /* TBD: Attenere la terminazione dei processi figli */

    int status;
    for (int i = 0; i < 2; i++) {
        pid = wait(&status);
        if (pid == -1) {
            perror("Errore nella wait");
        } else {
            printf("[MAIN] Il processo figlio con PID %d è terminato.\n", pid);
        }
    }

	
	/* TBD: Deallocare gli oggetti monitor */

     remove_monitor_circolare(p);
    remove_monitor_buffer_singolo(b);

    shmctl(id_circolare, IPC_RMID,0);
    shmctl(id_singolo, IPC_RMID, 0);
    return(0);
}



void Produttore(MonitorVettoreCircolare *p, MonitorBufferSingolo *b) {

    // Genero numero casuale (meglio evitare 0, facciamo da 1 a 10)
    int numero_elementi = rand() % 10 + 1;

    printf("[PRODUTTORE] Totale elementi da produrre: %d\n", numero_elementi);
    
    // [RISOLTO TBD] Produzione sul buffer singolo
    // Corretto l'errore di battitura "numer_elementi" -> "numero_elementi"
    produzione_buffer_singolo(b, numero_elementi);

    for(int i=0; i<numero_elementi; i++) {
        
        // Genero il valore
        int val = rand() % 10;
        printf("[PRODUTTORE] Prodotto: %d\n", val);

        // [RISOLTO TBD] Inserimento nel buffer circolare
        produzione_circolare(p, val);

        sleep(1);
    }
}

void Consumatore(MonitorVettoreCircolare *p, MonitorBufferSingolo *b) {
    
    // [RISOLTO TBD] Prelievo dal buffer singolo
    // Questa riga fa tutto: chiama la funzione e salva il risultato
    int numero_elementi = consumazione_buffer_singolo(b); 

    printf("[CONSUMATORE] Totale elementi da consumare: %d\n", numero_elementi);

    for(int i=0; i<numero_elementi; i++) {
        
        // [RISOLTO TBD] Prelievo dal buffer circolare
        // MANCAVA QUESTA RIGA: Dichiariamo 'val' e preleviamo il dato
        int val = consumazione_circolare(p); 

        printf("[CONSUMATORE] Consumato: %d\n", val);
        
        // Opzionale: aggiungo una sleep per vedere meglio l'alternanza
        sleep(1); 
    }
}
