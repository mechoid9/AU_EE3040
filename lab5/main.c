/*-------------------------------------*
 * Brian Arnberg and Stephen Taylor    
 * ELEC 3040 - Lab 5, main.c
 * Keypad programming
 *  Always count up
 *  Output Port AD
 *  Port T 7-4 - keypad columns, (output)
 *  Port T 3-0 - keypad rows, (input)
 *  Port E 1 - IRQ           
 *-------------------------------------*/
#include <hidef.h>      /* common defines and macros */
#include <MC9S12C32.h>  /* derivative information */
#pragma LINK_INFO DERIVATIVE "MC9S12C32"

static unsigned char count1; //Count 1 - Constant up, output to AD
static unsigned char count2; //Count 2 - used to display keynumber for 10s
/*the keynumber is in hex, hex 0-D makes sense
 * hex E is * and hex F is # */
static unsigned char keynumber;
/* Define the IRQ service routine */
interrupt void IRQ_ISR(void) {
	/*This is really ugly, but it should work*/
	keynumber = PTAD;
	PTT = 0xF0; 
		if (PTT_PTT7 == 0) {
	
		} 
		else {
		  PTT = 0xE0; // PTT_PTT4 driven low, C1 low, Celse high
		  PTT = 0xE0; // PTT_PTT4 driven low, C1 low, Celse high
		  PTT = 0xE0; // PTT_PTT4 driven low, C1 low, Celse high
		  PTT = 0xE0; // PTT_PTT4 driven low, C1 low, Celse high
		if (PTT_PTT0 == 0) {//row 1
		PTAD = 0x01;//key 1
	  count2 = 10;
		PTT = 0x0F; //reset PTT columns to 0
		__asm rti //use assembly code to exit the interupt
		}
		else if (PTT_PTT1 == 0){//row 2
		PTAD = 0x04;//key 4
		count2 = 10;
		PTT = 0x0F; //reset PTT columns to 0
		__asm rti //use assembly code to exit the interupt
		}	
		else if (PTT_PTT2 == 0){//row 3
		PTAD = 0x07;//key 7
		count2 = 10;
		PTT = 0x0F; //reset PTT columns to 0
		__asm rti //use assembly code to exit the interupt
		}
		else if (PTT_PTT3 == 0){//row 4
		PTAD = 0x0E;//key *
		count2 = 10;
		PTT = 0x0F; //reset PTT columns to 0
		__asm rti //use assembly code to exit the interupt
		} else {
		  	PTT = 0xD0; // PTT_PTT5 driven low, C2 low, Celse high
		  	PTT = 0xD0; // PTT_PTT5 driven low, C2 low, Celse high
		  	PTT = 0xD0; // PTT_PTT5 driven low, C2 low, Celse high
		  	PTT = 0xD0; // PTT_PTT5 driven low, C2 low, Celse high
		    if (PTT_PTT0 == 0) {//row 1
		    PTAD = 0x02;//key 2
		    count2 = 10;
		    PTT = 0x0F; //reset PTT columns to 0
		    __asm rti //use assembly code to exit the interupt
		    }
		    else if (PTT_PTT1 == 0){//row 2
		    PTAD = 0x05;//key 5
		    count2 = 10;  
    		PTT = 0x0F; //reset PTT columns to 0  
		    __asm rti //use assembly code to exit the interupt
    		}	
		    else if (PTT_PTT2 == 0){//row 3
		    PTAD = 0x08;//key 8
		    count2 = 10;
		    PTT = 0x0F; //reset PTT columns to 0
		    __asm rti //use assembly code to exit the interupt
		    }
		    else if (PTT_PTT3 == 0){//row 4
		    PTAD = 0x00;//key 0
		    count2 = 10;
		    PTT = 0x0F; //reset PTT columns to 0
		    __asm rti //use assembly code to exit the interupt
		    } else {
		      PTT = 0xB0; // PTT_PTT6 driven low, C3 low, Celse high
		      PTT = 0xB0; // PTT_PTT6 driven low, C3 low, Celse high
		      PTT = 0xB0; // PTT_PTT6 driven low, C3 low, Celse high
		      PTT = 0xB0; // PTT_PTT6 driven low, C3 low, Celse high
		      if (PTT_PTT0 == 0) {//row 1
		      PTAD = 0x03;//key 3
		      count2 = 10;
		      PTT = 0x0F; //reset PTT columns to 0
		      __asm rti //use assembly code to exit the interupt
		      }
		      else if (PTT_PTT1 == 0){//row 2
		      PTAD = 0x06;//key 6
		      count2 = 10;
		      PTT = 0x0F; //reset PTT columns to 0
		      __asm rti //use assembly code to exit the interupt
		      }	
		      else if (PTT_PTT2 == 0){//row 3
		      PTAD = 0x09;//key 9
		      count2 = 10;
		      PTT = 0x0F; //reset PTT columns to 0
		      __asm rti //use assembly code to exit the interupt
		      }
		      else if (PTT_PTT3 == 0){//row 4
		      PTAD = 0x0F;//key #
		      count2 = 10;
		      PTT = 0x0F; //reset PTT columns to 0
		      __asm rti //use assembly code to exit the interupt
		      } else {
		        PTT = 0x70; // PTT_PTT7 driven low, C4 low, Celse high
		        PTT = 0x70; // PTT_PTT7 driven low, C4 low, Celse high
		        PTT = 0x70; // PTT_PTT7 driven low, C4 low, Celse high
		        PTT = 0x70; // PTT_PTT7 driven low, C4 low, Celse high
		        if (PTT_PTT0 == 0) {//row 1
		        PTAD = 0x0A;//key A
		        count2 = 10;
		        PTT = 0x0F; //reset PTT columns to 0
		        __asm rti //use assembly code to exit the interupt
		        }
		        else if (PTT_PTT1 == 0){//row 2
		        PTAD = 0x0B;//key B
		        count2 = 10;
		        PTT = 0x0F; //reset PTT columns to 0
		        __asm rti //use assembly code to exit the interupt
		        }	
		        else if (PTT_PTT2 == 0){//row 3
		        PTAD = 0x0C;//key C
		        count2 = 10;
		        PTT = 0x0F; //reset PTT columns to 0
		        __asm rti //use assembly code to exit the interupt
		        }
		        else if (PTT_PTT3 == 0){//row 4
		        PTAD = 0x0D;//key D
		        count2 = 10;
		        PTT = 0x0F; //reset PTT columns to 0
		        __asm rti //use assembly code to exit the interupt
		        }
		      }
		    }
		}
		}
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
	//PTAD = count1; // output count1 to Port AD
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
		/* if the countdown is greater than 0, decrement by one 
		 * and do nothing;
		 *  else output  the "count up" to AD */
		if ( count2 > 0 ) {
			count2 -= 1; //decrement countdown 
	    PTT = 0x0F;
		}
		else {
			PTAD = count1; //display the current count to AD
		  PTT = 0x0F;
		}
	} /* repeat forever */
}
