/*-----------------------------------*/
/*  Brian Arnberg and Stephen Taylor */
/*  ELEC 3040 - Lab 2, main.c        */
/*  Decade up/down counter,          */
/*   count only when S1=1            */
/*   count up when S2=0,             */
/*   count down when S2=1	     */
/*-----------------------------------*/
#include <hidef.h>      /* common defines and macros */
#include <MC9S12C32.h>  /* derivative information */
#pragma LINK_INFO DERIVATIVE "MC9S12C32"

static unsigned char count; //state of the count
char sw1; //state of S1
char sw2; //state of S2
/* function delay, based from lab1 main.c 
 *  i<100 should be about 1 second, so choose
 *  i<50 for 1/2 second	*/
void delay (void) {
	int i,j;
	for (i=0; i<25; i++) { //outer loop, 
		for (j=0; j<20000; j++) { //inner loop 
		}
	}
}

void counting (char sw2) {
	if (sw2 == 0){//count up
		if ( count == 9 ) {//if 9 has been reached, go to 0 
			count = 0; 
		}	
		else { //else increment by 1
			count += 1;
		}

	}
	else if (sw2 == 1){//count down
		if (count == 0 ) { //if 0 has been reached, go to 9
			count = 9 ; 
		}
		else { //else decrement by 1
			count -= 1;
		}
	}
	PTT = count;//output count to DDRT
}

void main (void) {
	DDRA = 0; //set PA0 to read
	DDRB = 0; //set PB4 to read
	DDRT = 0xFF; //set PortT to write
	count = 0; //initialize count to 0
	while (1){
		sw1 = PORTA & 0x01;//read PORTA and isolate bit 0
		delay();//delay for a bit
		if (sw1 == 1) {//while S1 is on, count, up or down
			sw2 = PORTB_BIT4;//read PORTB and isolate bit 4
		  counting(sw2);//call the counting function and tell it how to count
		}
	} /* repeat forever */
}
