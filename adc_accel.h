// Deanna Buttaro dlb3un, Lina He lh3su

#ifndef ADC_ACCEL_H_
#define ADC_ACCEL_H_

#define NUM_SAMPLES 8	// The number of samples
#define BIT_SHIFT 3	// The log base 2 of the number of samples; used for taking the average

void ConfigureADC();
void Filter();
void ComputeAverage();
void ReadADC();


#endif
