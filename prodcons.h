#ifndef _PRODCONS_
#define _PRODCONS_

#include "monitor_hoare.h"
#define VARCOND_PROD_CIRCOLARE 0
#define VARCOND_CONS_CIRCOLARE 1
#define VARCOND_PROD_SINGOLO 2
#define VARCOND_CONS_SINGOLO 3 


#define DIM 5

typedef struct {

    int buffer[DIM];
    int testa;
    int coda;
    int count;
    Monitor m;

    /* TBD: Introdurre ulteriori variabili per la gestione del vettore 
            e per la sincronizzazione */

} MonitorVettoreCircolare;

void init_monitor_circolare(MonitorVettoreCircolare * p);
void produzione_circolare(MonitorVettoreCircolare * p, int val);
int consumazione_circolare(MonitorVettoreCircolare * p);
void remove_monitor_circolare(MonitorVettoreCircolare * p);

typedef struct {

    int buffer;
    int buffer_pieno;
    Monitor m;
    
    /* TBD: Introdurre ulteriori variabili per la gestione del buffer
            e per la sincronizzazione */

} MonitorBufferSingolo;

void init_monitor_buffer_singolo(MonitorBufferSingolo * b);
void produzione_buffer_singolo(MonitorBufferSingolo * b, int val);
int consumazione_buffer_singolo(MonitorBufferSingolo * b);
void remove_monitor_buffer_singolo(MonitorBufferSingolo * b);

#endif
