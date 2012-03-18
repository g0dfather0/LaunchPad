/*
 * Main.c
 * Author: g0dfather0
 * Project to explore the programming of the Launchpad
 * CPU Version: MSP430G2553
 */
 
//Include header file for device
#include <msp430g2553.h>
 
//Identify IO pins for LEDs specific to LaunchPad configuration
#define LED_RED BIT0
#define LED_GRN BIT6
 
//Set IO port parameters
#define LED_DIR P1DIR //Assign LEDs to outputs
#define LED_OUT P1OUT //Assign variable for controlling LEDs (P1 output register)
 
//Setting values for timer values for toggling LEDs
#define LED_TOGGLE_RATE 0x0FFF
 
//Declare Functions explicitly
void InitializeLEDs(void);
void InitializeLEDEffect(void);
void InitializeClocks(void);
 
void main(void)
{
	WDTCTL = WDTPW + WDTHOLD; // Disable WatchDog Timer
	__enable_interrupt(); //Enable Interrupts
	//InitializeClocks();
	InitializeLEDs();
	InitializeLEDEffect();
}
 
void InitializeLEDs(void)
{
	LED_DIR |= LED_RED + LED_GRN; //Set IO port direction to output for LED pins
	LED_OUT |= (LED_RED + LED_GRN); //Set both LEDs to 1 (on)
} 

void InitializeLEDEffect(void)
{	
	LED_OUT &= ~LED_GRN; //Set Green LED on and Red LED off
	
	TACCR0 = LED_TOGGLE_RATE; //Set timer value
	
	TACTL = TASSEL_1 + MC_1; //Timer A control = Clock Source Select (Timer A) + Timer A mode Control count up to Capture Control 0 (LED_TOGGLE_RATE)
	
	TACCTL1 = CCIE + OUTMOD_3; //Timer A Capture Compare Control set to enable interrupt + Pulse Width Modulation Set/Reset 
	
	__bis_SR_register(LPM3_bits + GIE); //enable low power with interrupts turned on
}

#pragma vector = TIMER0_A1_VECTOR //set interrupt condition to Timer A expire
__interrupt void ta1_isr(void)
{
	TACCTL1 &= ~CCIFG; //Clear Interrupt Flag for Timer A Capture Compare Control
    LED_OUT ^= (LED_RED + LED_GRN); //Toggle LEDs
}
