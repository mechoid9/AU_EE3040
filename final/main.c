/*-------------------------------------*
 * Brian Arnberg and Stephen Taylor    
 * ELEC 3040 - Lab 11, main.c
 * PWM Output Controller        
 *  Keypad 1-9 - Set PWM speeds 1-9 
 *  Keypad 0 - Stop, Speed to 0
 *  Port T 7-4 - keypad columns, (output)
 *  Port T 3-0 - keypad rows, (input)
 *  PP5 - PWM ouptut
 *  PPAD bit 0 - ADC capture
 *  Port E 1 - IRQ           
 *-------------------------------------*/
#include <hidef.h>      /* common defines and macros */
#include <MC9S12C32.h>  /* derivative information */
#pragma LINK_INFO DERIVATIVE "MC9S12C32"
#define N 500 /* define N samples to be 250 */
unsigned long PERIOD = 250; //for 1ms
/* Duty Cycles */
int duty[10];
int i; // for "for" statements
/* the keynumber is in hex, hex 0-D makes sense
 * hex E is * and hex F is # */
static unsigned char keynumber;
/* current command */
static unsigned char current;
/* previous command */
static unsigned char previous;
unsigned int sample[N];//past 250 samples
unsigned int j;//variable used for counting samples

/* Define the IRQ service routine */
interrupt void IRQ_ISR(void) {
		PTT = 0xE0; // PTT_PTT4 driven low, C1 low, Celse high
			    for (i=0;i<4;i++) {
		if (PTT_PTT0 == 0) {//row 1
		keynumber = 0x01;//key 1
		}
		else if (PTT_PTT1 == 0){//row 2
		keynumber = 0x04;//key 4
		}	
		else if (PTT_PTT2 == 0){//row 3
		keynumber = 0x07;//key 7
		}
		else if (PTT_PTT3 == 0){//row 4
		keynumber = 0x0E;//key *
		} else {
		  	PTT = 0xD0; // PTT_PTT5 driven low, C2 low, Celse high
			    for (i=0;i<4;i++) {
		    if (PTT_PTT0 == 0) {//row 1
		    keynumber = 0x02;//key 2
		    }
		    else if (PTT_PTT1 == 0){//row 2
		    keynumber = 0x05;//key 5
    	            }	
		    else if (PTT_PTT2 == 0){//row 3
		    keynumber = 0x08;//key 8
		    }
		    else if (PTT_PTT3 == 0){//row 4
		    keynumber = 0x00;//key 0
		    } else {
		      PTT = 0xB0; // PTT_PTT6 driven low, C3 low, Celse high
			    for (i=0;i<4;i++) {
		      if (PTT_PTT0 == 0) {//row 1
		      keynumber = 0x03;//key 3
		      }
		      else if (PTT_PTT1 == 0){//row 2
		      keynumber = 0x06;//key 6
		      }	
		      else if (PTT_PTT2 == 0){//row 3
		      keynumber = 0x09;//key 9
		      }
		      else if (PTT_PTT3 == 0){//row 4
		      keynumber = 0x0F;//key #
		      } else {
		        PTT = 0x70; // PTT_PTT7 driven low, C4 low, Celse high
			    for (i=0;i<4;i++) {
		        if (PTT_PTT0 == 0) {//row 1
		        keynumber = 0x0A;//key A
		        }
		        else if (PTT_PTT1 == 0){//row 2
		        keynumber = 0x0B;//key B
		        }	
		        else if (PTT_PTT2 == 0){//row 3
		        keynumber = 0x0C;//key C
		        }
		        else if (PTT_PTT3 == 0){//row 4
		        keynumber = 0x0D;//key D
		        } else { //No key found
		        	PTT = 0x0F; //reset PTT columns to 0
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

	PTT = 0x0F; //reset PTT columns to 0
	for (i=0;i<4;i++) {}; //small delay
	__asm rti //use assembly code to exit the interupt
}



/* TIMER_CHANNEL_0 - accumulates voltage information
 * from A/D conversions
 * if the previous command is different from the current,
 * the the routine assumes that the system experienced a step 
 * change and begins to sample voltage data for 
 * N samples
 */
interrupt void TIMER_CHANNEL_0 (void) {	
  if ( previous != current ) {//if previous not current
    if ( j != N ) {
     while (ATDSTAT0_SCF == 0); // wait for conversion sequence complete
     sample[j] = ATDDR0H;//insert value from left side
     j++;//increment j
   } else {
    previous = current;//set the previous command to be the current to quit sampling
    j = 0;//reset j
   }
  }
TFLG1_C0F = 1;//reset timer interrupt
TC0+=625;//check again in 10ms
TIE_C0I = 1;//timer interupt enable channel 0
 
}


void main (void) {
	DDRE = 0; //set Port E to read
	DDRT = 0xF0; //set PortT  bits 7-4 output, 3-0 input 
	DDRAD = 0x00; //set PortAD input 
	ATDDIEN = 0x00; //disable digital input buffer
	
	j = previous = current = 0;//initialize variables 
 	//set the duty periods based on PERIOD in cycles
	for (i=0;i<10;i++) { 
 	  duty[i] = PERIOD * i / 10;
 	}
	
	PERT = 1; //Enable Port T's pull device
	PPST = 0xF0; // Port T: 7-4 pull low; 3-0 pull up
	PTT = 0x0F; //initialize Port T 
	
	PWMDTY4 =0;//period on channel 4 to 0
	PWMPER4 = 0;//duty on channel 4 to 0
	PWMCTL_CON45 = 0;//channels 4 and 5 (0 = separate, 1 = together)
	PWME_PWME5 = 1;//signal on PWM
	PWMPOL_PPOL5 = 1;//signal high at beginning of period, Channel 5
	PWMCAE_CAE5 = 0;//channel 5 operates in left-aligned mode
	PWMPRCLK_PCKA = 4;//divide bus clock frequency by 16
	PWMCLK_PCLK5 = 0;//select clock A
	PWMPER5 = PERIOD;//period length for 1ms
	PWMDTY5 = 0;//set a zero length duty cycle 

	ATDCTL2_ADPU = 1;//power up
	ATDCTL2_AFFC = 0;//reset conversion flag by reading status
	ATDCTL2_AWAI = 0;//continue to run in wait mode
	ATDCTL2_ETRIGE = 0;//disable external trigger
	ATDCTL2_ASCIE = 0;//ASCIF != SCF flag

	for (i=0;i<20;i++);//delay for about 20us

	ATDCTL3_S8C = 0;
	ATDCTL3_S4C = 0;
	ATDCTL3_S2C = 0;
	ATDCTL3_S1C = 1;//one conversion/sequence
	ATDCTL3_FIFO = 0;//each sequence to consecutive registers
	ATDCTL3_FRZ = 0;//continue during freeze mode
	
	ATDCTL4_SRES8 = 1;//8-bit conversion sequence
	ATDCTL4_SMP = 3;//16 clocks ; sample time select
	ATDCTL4_PRS = 1;//divide clock by 2 (for 2MHz)

	ATDCTL5_SCAN = 1;//continuous conversion sequences
//	TSCR1_TEN = 1;//enable timer
//	TSCR2_PR = 7;/*prescale factor (binary 111, or factor of 128 */
//	TIOS_IOS0 = 0;/*Set to input  compare*/
//	TCTL4_EDG0B = 1;//input capture on PT0 rising edge
//	TCTL4_EDG0A = 0;//input capture on PT0 rising edge		
//	TFLG1_C0F = 1; //reset timer interupt
//	TIE_C0I = 1;//Timer Interupt Enable 0
//	TSCR2_TOI = 1;//enable timer overflow interrupt
//	TFLG2_TOF = 1;//reset timer overflow flag

	TSCR1_TEN = 1;//enable timer
	TSCR2_PR = 6;//prescale timer by 64 
	TIOS_IOS0 = 1;//set to output compare on timer channel 0

	INTCR_IRQEN = 1; /*enable IRQ# interrupts */
	INTCR_IRQE = 1; /*IRQ# interrupts edge-triggered */
	EnableInterrupts; /*clear I mask to enable interrupts */
	TFLG1_C0F = 1;//set interrupt flag
	TC0 = TCNT + 625;//timer interupt in 10 ms
	TIE_C0I = 1;//enable timer interrupt
	while (1){
	__asm wai //Wait for interupt	
	} /* repeat forever */
}
