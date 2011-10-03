/*-------------------------------------*
 * Brian Arnberg and Stephen Taylor    
 * ELEC 3040 - Lab 6, main.c
 * Stop Watch - Decade 0.0 - 9.9 sec       
 *  always count up
 *  Keypad 1 - Start/Stop
 *  Keypad 0 - Reset
 *  Output Port AD
 *  Port T 7-4 - keypad columns, (output)
 *  Port T 3-0 - keypad rows, (input)
 *  Port E 1 - IRQ           
 *-------------------------------------*/
#include <hidef.h>      /* common defines and macros */
#include <MC9S12C32.h>  /* derivative information */
#pragma LINK_INFO DERIVATIVE "MC9S12C32"

static unsigned char count1; //Count 1 - Left of the decimal      
static unsigned char count2; //Count 2 - Right of the decimal             
static unsigned char started; // 1 or 0; 1 = started, 0 = stopped
int i; // for "for" statements
/*the keynumber is in hex, hex 0-D makes sense
 * hex E is * and hex F is # */
static unsigned char keynumber;
/* Define the IRQ service routine */

interrupt void IRQ_ISR(void) {
	/*	PTT = 0xF0;
		for (i=0;i<4;i++) {
		}; //small delay 
		if (PTT_PTT7 == 0) { } 
		else {*/
		PTT = 0xE0; // PTT_PTT4 driven low, C1 low, Celse high
		PTT = 0xE0; // PTT_PTT4 driven low, C1 low, Celse high
		PTT = 0xE0; // PTT_PTT4 driven low, C1 low, Celse high
		PTT = 0xE0; // PTT_PTT4 driven low, C1 low, Celse high
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
		  	PTT = 0xD0; // PTT_PTT5 driven low, C2 low, Celse high
		  	PTT = 0xD0; // PTT_PTT5 driven low, C2 low, Celse high
		  	PTT = 0xD0; // PTT_PTT5 driven low, C2 low, Celse high
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
		      PTT = 0xB0; // PTT_PTT6 driven low, C3 low, Celse high
		      PTT = 0xB0; // PTT_PTT6 driven low, C3 low, Celse high
		      PTT = 0xB0; // PTT_PTT6 driven low, C3 low, Celse high
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
		        PTT = 0x70; // PTT_PTT7 driven low, C4 low, Celse high
		        PTT = 0x70; // PTT_PTT7 driven low, C4 low, Celse high
		        PTT = 0x70; // PTT_PTT7 driven low, C4 low, Celse high
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
		 
		/* Test the key press, 
		 * start or stop if the keypress is 0 */
		if ( (keynumber == 0) && (started == 0) ) {//start
			started = ~started; //toggle the "started" state
			/* start the clock */
			
			TFLG1_C0F = 1; //reset timer interupt
			TC0 = TCNT + 3125;//set new compare time
			TIE_C0I = 1;//Timer Interupt Enable 0
			PTT = 0x0F; //reset PTT columns to 0
			for (i=0;i<4;i++) {
			}; //small delay
			__asm rti //use assembly code to exit the interupt
		}
		else if ( (keynumber == 0) && (started) ){//stop
			started = ~started; //toggle the "started" state
			/* stop the clock and stuff */
			
			TIE_C0I = 0; //disable timer interupts
			PTT = 0x0F; //reset PTT columns to 0
			for (i=0;i<4;i++) {
			}; //small delay
			__asm rti //use assembly code to exit the interupt
	       	}
		/* Else if the keypressed is 1 and the code is in 
		 * the stopped state, then clear the display */
		else if ( (keynumber == 1) && (started == 0) ) {
			count1 = count2 = 0; //clear the counts to 0
			PTAD = 0; //clear the LEDs
			PTT = 0x0F; //reset PTT columns to 0
			for (i=0;i<4;i++) {
			}; //small delay
			__asm rti //use assembly code to exit the interupt
		}
}


/* redelay - increments the count
 * - executed by timer interupt
 * - outputs to Port AD */
interrupt void TIMER_CHANNEL_0 (void) {
	if ( count2 == 9 ) { 
		count2 = 0; 
		if ( count1 == 9 ) {
			count1 = 0;
			count2 = 0;
		} else { count1 += 1; }
	}
	else { count2 += 1; }
	TFLG1_C0F = 1; //reset timer interupt
	TC0 += 3125;//set new compare time
	TIE_C0I = 1;//Timer Interupt Enable 0
	PTAD = count1*16 + count2; // output BCD to Port AD
}

void main (void) {
	DDRE = 0; //set Port E to read
	DDRT = 0xF0; //set PortT bits 7-4 output, 3-0 input
	DDRAD = 0xFF; //set PortAD to output
	
	count1 = 0; //initialize count1 to 0 to start count up
	count2 = 0; //initialize count2 to 0 to prevent countdown
	started = 0; //initialize in the "unstarted" state
	
	PERT = 1; //Enable Port T's pull device
	PPST = 0xF0; // Port T: 7-4 pull low; 3-0 pull up
	PTT = 0x0F; //initialize Port T
	
	TSCR1_TEN = 1;//enable timer
	TSCR2_PR = 7;/*prescale factor (binary 111, or factor of 128 */
	TIOS_IOS0 = 1;/*Set to output compare*/
	TC0 = TCNT;//Initialize TC0
	
	INTCR_IRQEN = 1; /*enable IRQ# interrupts */
	INTCR_IRQE = 1; /*IRQ# interrupts edge-triggered */
	EnableInterrupts; /*clear I mask to enable interrupts */
	//__asm ANDCC #0xBF /*clear X mast to enable XIRQ# */
	while (1){
		
	} /* repeat forever */
}
