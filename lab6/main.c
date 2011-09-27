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
/*the keynumber is in hex, hex 0-D makes sense
 * hex E is * and hex F is # */
static unsigned char keynumber;
/* Define the IRQ service routine */

interrupt void IRQ_ISR(void) {
		PTT = 0xF0; 
		if (PTT_PTT7 == 0) { } 
		else {
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
		        }
		      }
		    }
		}
		PTT = 0x0F; //reset PTT columns to 0
		__asm rti //use assembly code to exit the interupt
		
		}
}


/* count_1 - increments the count
 * - called by other functions
 * - outputs to Port AD */
void count_1 (void) {
	if ( count2 == 9 ) { 
		count2 = 0; 
		if ( count1 == 9 ) {
			count1 =  0;
			count2 = 0;
		} else { count1 += 1; }
	}
	else { count2 += 1; }
	PTAD = count2; // output count1 to Port AD
}

void main (void) {
	DDRE = 0; //set Port E to read
	DDRT = 0xF0; //set PortT bits 7-4 output, 3-0 input
	DDRAD = 0xFF; //set PortAD to output
	count1 = 0; //initialize count1 to 0 to start count up
	count2 = 0; //initialize count2 to 0 to prevent countdown
	PERT = 1; //Enable Port T's pull device
	PPST = 0xF0; // Port T: 7-4 pull low; 3-0 pull up
	PTT = 0x0F;
	INTCR_IRQEN = 1; /*enable IRQ# interrupts */
	INTCR_IRQE = 1; /*IRQ# interrupts edge-triggered */
	EnableInterrupts; /*clear I mask to enable interrupts */
	//__asm ANDCC #0xBF /*clear X mast to enable XIRQ# */
	while (1){
		delay();//delay for 0.5s
		delay();//delay for 0.5s
		count_1();//increment count 1	
	} /* repeat forever */
}
