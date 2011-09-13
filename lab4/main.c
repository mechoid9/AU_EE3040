/*-------------------------------------*
 * Brian Arnberg and Stephen Taylor    *
 * ELEC 3040 - Lab 4, main.c           *
 * Two decade counters,                *
 *  PortT (count1) always up           *
 *  PortAD (count2) variable direction *
 *   up - XIRQ activated 	       *
 *   down - IRQ activated              *
 *-------------------------------------*/
#include <hidef.h>      /* common defines and macros */
#include <MC9S12C32.h>  /* derivative information */
#pragma LINK_INFO DERIVATIVE "MC9S12C32"

static unsigned char count1; //Count 1 (Port T)
static unsigned char count2; //Count 2 (Port AD)
char sw1; //state of S1
char sw2; //state of S2
char dir; //count direction (1 is down, 0 is up)

/* Define the XIRQ service routine */
interrupt void XIRQ_ISR(void) {
 	dir = 1;//dir = 1 counts down
}

/* Define the IRQ service routine */
interrupt void IRQ_ISR(void) {
 	dir = 0;//dir = 0 counts down
}

/* Delay function - In lab tests show i<17 ~ .5s */ 
void delay (void) {
	int i,j;
	for (i=0; i<17; i++) { //outer loop, "i<50" seems too slow 
		for (j=0; j<20000; j++) { //inner loop 
		}
	}
}

/* Function for the first count */
void count_1 (void) {
	if ( count1 == 9 ) { count1 = 0; }
	else { count1 +=1; }
	PTT = count1; // output count1 to Port T
}

/* Function for the second count.  */
void count_2 (char dir) {
	if (dir == 0){//count up
		if ( count2 == 9 ) {//if 9 has been reached, go to 0 
			count2 = 0; 
		}	
		else { //else increment by 1
			count2 += 1;
		}

	}
	else if (dir == 1){//count down
		if (count2 == 0 ) { //if 0 has been reached, go to 9
			count2 = 9 ; 
		}
		else { //else decrement by 1
			count2 -= 1;
		}
	}
	PORTAD = count2;//output count2 to Port AD
}

void main (void) {
/*	DDRA = 0; //set PA0 to 0
	DDRB = 0; //set PB4 to 0 */
	DDRE = 0; //set Port E to read
	DDRT = 0xFF; //set PortT to output
	DDRAD = 0xFF; //set PortAD to output
	count1 = 0; //initialize count1 to 0
	count2 = 0; //initialize count2 to 0
	dir = 0; //initial direction is up (1 is down)
	INTCR_IRQEN = 1; /*enable IRQ# interrupts */
	INTCR_IRQE = 1; /*IRQ# interrupts edge-triggered */
	EnableInterrupts; /*clear I mask to enable interrupts */
	__asm ANDCC #0xBF /*clear X mast to enable XIRQ# */
	while (1){
		delay();//delay for 0.5s
		count_1();//First Count	
		delay();//delay for 0.5s
		count_1();//First Count
		count_2(dir);//Second Count
	} /* repeat forever */
}
