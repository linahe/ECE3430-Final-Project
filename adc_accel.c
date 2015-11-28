// Deanna Buttaro dlb3un, Lina He lh3su


#include <msp430.h>
#include "adc_accel.h"

unsigned int index;			// The index of the oldest value in samples

volatile unsigned int adc[3] = {0};	// This will hold the x,y and z axis values
volatile unsigned int X_Axis = 0;
volatile unsigned int Y_Axis = 0;
volatile unsigned int Z_Axis = 0;

int XBuffer[NUM_SAMPLES];
int YBuffer[NUM_SAMPLES];
int ZBuffer[NUM_SAMPLES];

int XSum, YSum, ZSum;
extern int XAvg, YAvg, ZAvg;

void Filter()
{
	XSum += X_Axis - XBuffer[index];		// Subtract out the oldest value in samples from sums
	YSum += Y_Axis - YBuffer[index];		// and add in the new value to sum
	ZSum += Z_Axis - ZBuffer[index];

	XBuffer[index] = X_Axis;	// Set oldest value to the new current reading
	YBuffer[index] = Y_Axis;
	ZBuffer[index] = Z_Axis;

	index++;						// Increment the positition of the oldest value in samples
	if (index == NUM_SAMPLES)		// Reset the index to 0 if it equals NUM_SAMPLES
		index = 0;

	ComputeAverage();				// Update averages
}

void ComputeAverage()
{
	XAvg = XSum >> BIT_SHIFT;		// the average (the sum shifted by the log (base 2) of NUM_SAMPLES)
	YAvg = YSum >> BIT_SHIFT;
	ZAvg = ZSum >> BIT_SHIFT;
}

void ConfigureADC()
{
	ADC10CTL1 = INCH_2 + CONSEQ_1;// + ADC10DIV_3;	//A2, A1, A0; read single sequense, ADC10DIV_3 = ADC10CLK divider, divide by 4
	ADC10CTL0 = REF2_5V + ADC10SHT_2 + REFON +  MSC + ADC10ON;// + ADC10IE;

	__delay_cycles(1000);                     // Wait for ADC Ref to settle

	ADC10DTC1 = 0x03;                         // 3 conversions
	ADC10AE0 |= 0x03;                         // Disable digital I/O on P1.0 to P1.2

}

void ReadADC()
{
	ADC10CTL0 &= ~ENC;
	while (ADC10CTL1 & BUSY)               // Wait if ADC10 core is active
	{
		__no_operation();
	}
	ADC10SA = (unsigned int)adc;			// Copies data in ADC10SA to unsigned int adc array
	ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start

	Z_Axis = adc[0];						// adc array 0 copied to the variable X_Axis
	Y_Axis = adc[1];						// adc array 1 copied to the variable Y_Axis
	X_Axis = adc[2];						// adc array 2 copied to the variable Z_Axis
	__no_operation();
	//__bis_SR_register(CPUOFF + GIE);        // LPM0, ADC10_ISR will force exit
}

