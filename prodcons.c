#include <stdio.h>

#include "prodcons.h"


void init_monitor_circolare(MonitorVettoreCircolare * p) {
	init_monitor(&p->m,2);
	p->testa=0;
	p->coda=0;
	p->count=0;

    
}

void produzione_circolare(MonitorVettoreCircolare * p, int val) {
	
    enter_monitor(&p->m);
		
    if(p->count==DIM){
			wait_condition(&p->m,VARCOND_PROD_CIRCOLARE);
		}
	

    /* TBD: Completare la produzione nel buffer circolare con sincronizzazione */

    p->buffer[p->testa] = val;
    p->testa=(p->testa + 1) % DIM;
    p->count ++;
    
    signal_condition(&p->m, VARCOND_CONS_CIRCOLARE);
    leave_monitor(&p->m);
    
}
    


int consumazione_circolare(MonitorVettoreCircolare * p) {

 	enter_monitor(&p->m);
	    /* TBD: Completare la consumazione dal buffer circolare con sincronizzazione */

    int val;

    if(p->count == 0){

        wait_condition(&p->m, VARCOND_CONS_CIRCOLARE);

    }

    val = p->buffer[p->coda];
    p->coda = (p->coda + 1) % DIM;
    p->count--;
    
    signal_condition(&p->m, VARCOND_PROD_CIRCOLARE);
    
    leave_monitor(&p->m);

    return val;
}

void remove_monitor_circolare(MonitorVettoreCircolare * p) {

	remove_monitor(&p->m);

    /* TBD: Disattivare l'oggetto monitor */
}



void init_monitor_buffer_singolo(MonitorBufferSingolo * b) {

    /* TBD: Inizializzare il monitor */

	init_monitor(&b->m, 2);

    b->buffer_pieno=0;
    b->buffer = 0;
}

void produzione_buffer_singolo(MonitorBufferSingolo * b, int val) {

	enter_monitor(&b->m);
		if(b->buffer_pieno == 1){
				wait_condition(&b->m, VARCOND_PROD_SINGOLO);
				
		}

    /* TBD: Completare la produzione sul buffer singolo con sincronizzazione */
    b->buffer = val;
    b->buffer_pieno = 1;
    
    signal_condition(&b->m, VARCOND_CONS_SINGOLO);
    leave_monitor(&b->m);

}

int consumazione_buffer_singolo(MonitorBufferSingolo * b) {
	
    enter_monitor(&b->m);
	int val;

		if(b->buffer_pieno == 0){
				wait_condition(&b->m, VARCOND_CONS_SINGOLO);
				
		}		
    /* TBD: Completare la consumazione dal buffer singolo con sincronizzazione */
    val = b->buffer;

    b->buffer_pieno=0;

    signal_condition(&b->m, VARCOND_PROD_SINGOLO);
    leave_monitor(&b->m);

    return val;
}

void remove_monitor_buffer_singolo(MonitorBufferSingolo * b) {

	remove_monitor(&b->m);

    /* TBD: Disattivare l'oggetto monitor */
}
