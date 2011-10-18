/*-------------------------------------*
 * Brian Arnberg and Stephen Taylor    
 * ELEC 3040 - Lab 9, main.c
 * PWM Output Controller        
 *  Keypad 1-9 - Set PWM speeds 1-9 
 *  Keypad 0 - Stop, Speed to 0
 *  Port AD 7-4 - keypad columns, (output)
 *  Port AD 3-0 - keypad rows, (input)
 *  Port T - PWM ouptut
 *  Port E 1 - IRQ           
 *-------------------------------------*/
#include <hidef.h>      /* common defines and macros */
#include <MC9S12C32.h>  /* derivative information */
#pragma LINK_INFO DERIVATIVE "MC9S12C32"

int PERIOD 250 /* A period of 1ms is 4000 cycles*/
/* Duty Cycles */
unsigned int duty[10]={0,25,50,75,100,125,150,175,200,225};
int i; // for "for" statements
/*the keynumber is in hex, hex 0-D makes sense
 * hex E is * and hex F is # */
static unsigned char keynumber;
/* current command */
static unsigned char current;
int previous;//previous TCNT, used in period
int new;//current TCNT, used in period
int T;//period length in cycles
int overflow;//value of over-flow to be added to new in case of overflow(s) 
//unsigned char T2; //length of off time
/* Define the IRQ service routine */
interrupt void IRQ_ISR(void) {
		PTAD = 0xE0; // PTAD_PTAD4 driven low, C1 low, Celse high
			    for (i=0;i<4;i++) {
		if (PTAD_PTAD0 == 0) {//row 1
		keynumber = 0x01;//key 1
		}
		else if (PTAD_PTAD1 == 0){//row 2
		keynumber = 0x04;//key 4
		}	
		else if (PTAD_PTAD2 == 0){//row 3
		keynumber = 0x07;//key 7
		}
		else if (PTAD_PTAD3 == 0){//row 4
		keynumber = 0x0E;//key *
		} else {
		  	PTAD = 0xD0; // PTAD_PTAD5 driven low, C2 low, Celse high
			    for (i=0;i<4;i++) {
		    if (PTAD_PTAD0 == 0) {//row 1
		    keynumber = 0x02;//key 2
		    }
		    else if (PTAD_PTAD1 == 0){//row 2
		    keynumber = 0x05;//key 5
    	            }	
		    else if (PTAD_PTAD2 == 0){//row 3
		    keynumber = 0x08;//key 8
		    }
		    else if (PTAD_PTAD3 == 0){//row 4
		    keynumber = 0x00;//key 0
		    } else {
		      PTAD = 0xB0; // PTAD_PTAD6 driven low, C3 low, Celse high
			    for (i=0;i<4;i++) {
		      if (PTAD_PTAD0 == 0) {//row 1
		      keynumber = 0x03;//key 3
		      }
		      else if (PTAD_PTAD1 == 0){//row 2
		      keynumber = 0x06;//key 6
		      }	
		      else if (PTAD_PTAD2 == 0){//row 3
		      keynumber = 0x09;//key 9
		      }
		      else if (PTAD_PTAD3 == 0){//row 4
		      keynumber = 0x0F;//key #
		      } else {
		        PTAD = 0x70; // PTAD_PTAD7 driven low, C4 low, Celse high
			    for (i=0;i<4;i++) {
		        if (PTAD_PTAD0 == 0) {//row 1
		        keynumber = 0x0A;//key A
		        }
		        else if (PTAD_PTAD1 == 0){//row 2
		        keynumber = 0x0B;//key B
		        }	
		        else if (PTAD_PTAD2 == 0){//row 3
		        keynumber = 0x0C;//key C
		        }
		        else if (PTAD_PTAD3 == 0){//row 4
		        keynumber = 0x0D;//key D
		        } else { //No key found
		        	PTAD = 0x0F; //reset PTAD columns to 0
			        for (i=0;i<4;i++) {
			        }; //small delay
		          __asm rti //exit the interrupt because nothing happened
		        }
			    }
		      }
			    }
		    }
			    }
		}
			    }
		 

	if ( (keynumber <= 9) && (keynumber >= 1) ) {//Set the current mode
		current = keynumber; //current mode
		PWMDTY5 = duty[current];//set duty cycle
		PWME_PWME5 = 1;// enable chanel 5
		}
	else if (keynumber == 0) {//set current mode to stop
		current = keynumber; //current mode
		PWMDTY5 = 0;//disable
		PWME_PWME5 = 0;
		}

	PTAD = 0x0F; //reset PTAD columns to 0
	for (i=0;i<4;i++) {}; //small delay
	__asm rti //use assembly code to exit the interupt
}

/* TIMER_OVERFLOW - keep up with overflows
 * - triggers on overflow
 * - each overflow adds to the total value
 * - resets overflow flag
 */
interrupt void TIMER_OVERFLOW (void) {	
	overflow += 65536;//add to the value of the overflow
	//reset the timer overflow flag
	TFLG2_TOF = 1;//reset timer overflow flag
}


/* TIMER_CHANNEL_0 - calculates the period
 * - uses the difference between two rising
 *   edges to determine period length 
 * - T is in cycles
 * - period in time is T*32uS
 */
interrupt void TIMER_CHANNEL_0 (void) {	
	new = TCNT + overflow;//set the value of new to be the TCNT
 	T = new - previous;//find the value of the period
	previous = new;/* store the current value of TCNT to previous for use in
		  next calculation */
	overflow = 0; //reset overflow to 0		
	TFLG1_C0F = 1; //reset timer interupt
	TIE_C0I = 1;//Timer Interupt Enable 0
}


void main (void) {
	DDRE = 0; //set Port E to read
	DDRT = 0xFF; //set PortT to output 
	DDRAD = 0xF0; //set PortAD bits 7-4 output, 3-0 input
	ATDDIEN = 0x0F; //Enable digital input buffer
	
	current = 0;//current mode is 0
 	for (i=0;i<10;i++) { //set the duty periods based on PERIOD in cycles
	  duty[i] = PERIOD * i * 0.10;
	}
	previous = new = overflow = 0;//initialize variables to 0

	PERAD = 1; //Enable Port AD's pull device
	PPSAD = 0xF0; // Port AD: 7-4 pull low; 3-0 pull up
	PTAD = 0x0F; //initialize Port AD 
	
	PWMDTY4 =0;//period on channel 4 to 0
	PWMPER4 = 0;//duty on channel 4 to 0
	PWMCTL_CON45 = 0;//channels 4 and 5 are separate
	PWME_PWME5 = 1;//signal on PWM
	PWMPOL_PPOL5 = 1;//signal high at beginning of period, Channel 5
	PWMCAE_CAE5 = 0;//channel 5 operates in left-aligned mode
	PWMPRCLK_PCKA = 4;//divide bus clock frequency by 16
	PWMCLK_PCLK5 = 0;//select clock A
	PWMPER5 = PERIOD;//period length for 1ms
	PWMDTY5 = 0;//set a zero length duty cycle 

	TSCR1_TEN = 1;//enable timer
	TSCR2_PR = 7;/*prescale factor (binary 111, or factor of 128 */
	TIOS_IOS0 = 0;/*Set to input  compare*/
	TCTL4_EDG0B = 0;//input capture on PT0 rising edge
	TCTL4_EDG0A = 1;//input capture on PT0 rising edge		
	TFLG1_C0F = 1; //reset timer interupt
	TIE_C0I = 1;//Timer Interupt Enable 0
	TSCR2_TOI = 1;//enable timer overflow interrupt
	TFLG2_TOF = 1;//reset timer overflow flag

	INTCR_IRQEN = 1; /*enable IRQ# interrupts */
	INTCR_IRQE = 1; /*IRQ# interrupts edge-triggered */
	EnableInterrupts; /*clear I mask to enable interrupts */
	//__asm ANDCC #0xBF /*clear X mast to enable XIRQ# */
	while (1){
	__asm wai //Wait for interupt	
	} /* repeat forever */
}
