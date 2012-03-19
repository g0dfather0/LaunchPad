/*
 * Main.c
 * Author: g0dfather0
 * Project to explore the programming of the Launchpad
 * CPU Version: MSP430G2553
 * 
 * License:
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * g0dfather0 from GitHub wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */
 
//#define MSP430g2452 1
#define MSP430g2553 1

//Include header file for device
//LaunchPad came with these two devices to sample
#ifdef MSP430g2452	
	#include <msp430g2452.h>
#elif MSP430g2553
	#include <msp430g2553.h>
#endif
 
//Identify IO pins for LEDs specific to LaunchPad configuration
#define LED_RED BIT0
#define LED_GRN BIT6
 
//Set IO port parameters
#define LED_DIR P1DIR //Assign LEDs to outputs
#define LED_OUT P1OUT //Assign variable for controlling LEDs (P1 output register)
 
//Setting values for timer values for toggling LEDs
#define LED_TOGGLE_RATE 0x1F40 //
 
//Declare Functions explicitly
void InitializeLEDs(void);
void InitializeLEDEffect(void);
void InitializeClocks(void);
 
void main(void)
{
	WDTCTL = WDTPW + WDTHOLD; // Disable WatchDog Timer
	//__enable_interrupt(); //Enable Interrupts
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
	LED_OUT &= ~LED_RED; //Set Green LED on and Red LED off
	
	//Lower clock speed if needed. This app uses full Timer A clock speed so this setting is not needed
	//BCSCTL1 |= DIVA_0; //Basic clock system control 1 |= Divide Clock by 1 (Full clock speed, 32kHz) 
	//Same functionality uncommented, divide by 1 does nothing
	
	//Setting the clock source is necessary for running this app without the debugger natively on the CPU       
  	BCSCTL3 |= LFXT1S_2; //Basic clock system control 3 ACLK = VLO (Very Low power/frequency Oscillator => 32kHz)
  	
	TACCR0 = LED_TOGGLE_RATE; //Set timer value
	TACTL = TASSEL_1 + MC_1; //Timer A control = Clock Source Select (Timer A) + Timer A mode Control count up to Capture Control 0 (LED_TOGGLE_RATE)
	TACCTL1 = CCIE + OUTMOD_0; //Timer A Capture Compare Control set to enable interrupt + Output Only (PWM) 
	__bis_SR_register(LPM3_bits + GIE); //enable low power with interrupts turned on
}

#pragma vector = TIMER0_A1_VECTOR //set interrupt condition to Timer A expire
__interrupt void ta1_isr(void)
{
	TACCTL1 &= ~CCIFG; //Clear Interrupt Flag for Timer A Capture Compare Control
	LED_OUT ^= (LED_RED + LED_GRN); //Toggle LEDs
}
