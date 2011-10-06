/*-------------------------------------*
 * Brian Arnberg and Stephen Taylor    
 * ELEC 3040 - Lab 7a, main.c
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
#define PERIOD 4000 /* A period of 1ms is 4000 cycles*/

static unsigned char count1; //Count 1 - Left of the decimal      
static unsigned char count2; //Count 2 - Right of the decimal             
static unsigned char started; // 1 or 0; 1 = started, 0 = stopped
static unsigned char high; //1 or 0; 1 = voltage high, 0 = low
/* Duty Cycles */
unsigned int duty[10]={0,400,800,1200,1600,2000,2400,2800,3200,3600};
int i; // for "for" statements
/*the keynumber is in hex, hex 0-D makes sense
 * hex E is * and hex F is # */
static unsigned char keynumber;
/* current command */
static unsigned char current;
unsigned char T2; //length of off time
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
		if (!started) {//if not started, then initialize
		TSCR1_TEN = 1;//enable timer
		TIOS_IOS0 = 1;/*Set to output compare, Bit 0*/
		TCTL1_OM5 = 0; /*Output Compair toggles PTT_PTT5 */
		TCTL1_OL5 = 1;
		TFLG1_C0F = 1; //reset timer interupt
		TC0 = TCNT + PERIOD;//set new compare time
		TIE_C0I = 1;//Timer Interupt Enable 0
		started=~started;//toggle started
		}
		current = keynumber; //current mode
		T2 = PERIOD - duty[current];//time off (not duty)
		PTAD = 0x0F; //reset PTAD columns to 0
		for (i=0;i<4;i++) {}; //small delay
		  PTT = current;
		__asm rti //use assembly code to exit the interupt
	}
	else if (keynumber == 0) {//set current mode to stop
		current = keynumber; //current mode
		TCTL1_OM5 = 1; /*Force PT5 to 0 */
		TCTL1_OL5 = 0;
		started=0;//stopped
		high=0;//low
		PTAD = 0x0F; //reset PTAD columns to 0
		for (i=0;i<4;i++) {}; //small delay
		  PTT = current;
		__asm rti //use assembly code to exit the interupt
	}
}
		    




/* TIMER_CHANNEL_0 - toggles PWM    
 * - executed by timer interupt
 * - outputs PWM          */
interrupt void TIMER_CHANNEL_0 (void) {
	if (!high) { //turn on
	high = ~high;//toggle high state
	PORTA_BIT0=1;//set to on
	TFLG1_C0F = 1; //reset timer interupt
	//T2 = PERIOD - duty[current];//the "off" time is period minus duty
	TC0 += duty[current];//set new compare time(at the end of duty cycle)
	TIE_C0I = 1;//Timer Interupt Enable 0
	} else { //turn off
  if (keynumber == 0) {
	 PORTA_BIT0=0;//set to off
	 //TFLG1_C0F = 1; //reset timer interupt
	 //TC0 += T2;//next compare time is T2 cycles away
	 //TIE_C0I = 1;//Timer Interupt Enable 0
   } else {
  	high = ~high;//toggle high state
	  PORTA_BIT0=0;//set to off
	  TFLG1_C0F = 1; //reset timer interupt
       TC0 += T2;//next compare time is T2 cycles away
	  TIE_C0I = 1;//Timer Interupt Enable 0
   }
	}
}
			  


void main (void) {
  DDRA = 1;//output to A
	DDRE = 0; //set Port E to read
	DDRT = 0xFF; //set PortT to output 
	DDRAD = 0xF0; //set PortAD bits 7-4 output, 3-0 input
	ATDDIEN = 0x0F; //Enable digital input buffer
	
	count1 = 0; //initialize count1 to 0 to start count up
	count2 = 0; //initialize count2 to 0 to prevent countdown
	started = 0; //initialize in the "unstarted" state
	high = 0; //initialize high to 0
	current = 0;//current mode is 0 

	PERAD = 1; //Enable Port AD's pull device
	PPSAD = 0xF0; // Port AD: 7-4 pull low; 3-0 pull up
	PTAD = 0x0F; //initialize Port AD 
	
/*	TSCR1_TEN = 1;//enable timer
	//TSCR2_PR = 7; //prescale factor (binary 111, or factor of 128
	TIOS_IOS0 = 1;//Set to output compare, Bit 0
	TCTL1_OM5 = 0; //Output Compair toggles PTT_PTT5
	TCTL1_OL5 = 1;
	TC0 = TCNT;//Initialize TC0
*/	
	INTCR_IRQEN = 1; /*enable IRQ# interrupts */
	INTCR_IRQE = 1; /*IRQ# interrupts edge-triggered */
	EnableInterrupts; /*clear I mask to enable interrupts */
	//__asm ANDCC #0xBF /*clear X mast to enable XIRQ# */
	while (1){
  
	__asm wai //Wait for interupt	
	} /* repeat forever */
}
