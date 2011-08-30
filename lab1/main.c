/*--------------------------------------*/
/* Victor P. Nelson			*/
/* ELEC 3040/3050 - Lab1, Program 1	*/
/* Toggle LED1 while SW1 pressed,	*/
/*   with short delay inserted		*/
/*--------------------------------------*/

#include <hidef.h> 	/* common defines and macros */
#include <MC9S12C32.h> 	/*derivative information */
/* Defines PORTA,DDRA: LED1 connected to PA0 */
/* Defines PORTE,DDRE: SW1 connected to PE0 */

/* Delay function - do nothing for about 1 second */
voide delay () {
	  int i,j;
	  for (i=0; i<100; i++) { //outer loop
		  for (j=0; j<20000; j++) { //inner loop
		  }
	  }
}

void main(void) {
	char sw1;		//state of SW1
	char led1;		//state of LED1

	/* Set port directins */
	DDRA = 1;		//PORTA PA0 = output to LED1
	DDRE = 0;		//PORTE PE0 = input from SW1

	/* Initial LED state */
	led1 = 0;		//state of LED1

	/* Endless loop */
	while (1) { 		//Can also use: for(;;) {
		PORTA = led1;	//Display LED1 state via PA0
		sw1 = PORTE & 0x01;	//Read PORTE and isolate bit 0

		/* Wait until SW1 pressed */
		while (sw1 == 0) {	//Wait for SW1 = 1 on PE0
			sw1 = PORTE & 0x01;  //Read PORTE and isolate bit 0
		}
		delay();	//Time delay for button release
		led1 = ~led1;	//Complement LED1 state
	} /* repeat forever */
	}

