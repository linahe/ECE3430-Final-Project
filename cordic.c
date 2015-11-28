/*
 * // Deanna Buttaro dlb3un, Lina He lh3su
 * LEVEL assignment
 * 10/29/15
*/
#include <msp430.h>
#include "cordic.h"

const int table[CORDIC_NTAB] = {11520, 6801, 3593, 1824, 916, 458, 229, 115, 57, 29, 14, 7, 4, 2, 1, 0};


void Cordic(calculations *calcs,char return_type)
{
	if (return_type == '0')			//if the return_type is a '0', calculate the Sine and Cosine of the given angle
		calculateSinCosXY(calcs);
	else if (return_type == '1')	//if the return_type is a '1', calculate the Arctangent and Hypotenuse
		calculateArcHypXY(calcs);
	else
		return;
}

void calculateArcHypXY(calculations *calcs)
{
	long newX, newY, angleTheta;

	if (calcs->x < 0)		//if x is negative, set the initial angleTheta to 180 and flip sign of x and y
	{
		angleTheta = 180;
		calcs->x *= -1;
		calcs->y *= -1;
	}
	else if (calcs->y < 0)	//if x is positive AND y is negative, account for angleTheta being in the fourth quadrant
		angleTheta = 360;
	else
		angleTheta = 0;

	calcs->x *= MUL;
	calcs->y *= MUL;
	angleTheta *= MUL;
	newX = calcs->x;
	newY = calcs->y;

	int n;
	for (n = 0; n < CORDIC_NTAB; n++)
	{
		if (newY > 0)
		{									//rotate clockwise if y is positive
			newX = newX + (calcs->y >> n);
			newY = newY - (calcs->x >> n);
			angleTheta += table[n];				//increase angleTheta by rotation amount
		}
		else								//otherwise rotate counter-clockwise
		{
			newX = newX - (calcs->y >> n);
			newY = newY + (calcs->x >> n);	//decrease angleTheta by rotation amount
			angleTheta -= table[n];
		}
		calcs->x = newX;
		calcs->y = newY;
	}
	calcs->angleTheta = angleTheta;					//store the total rotated angleTheta in calcs
	calcs->x = CORDIC_GAIN * newX;			//store the hypotenuse in x
}

void calculateSinCosXY(calculations *calcs)
{
	long newX, newY, newAngle;
	calcs->x = CORDIC_1K;
	calcs->y = 0;

	newAngle = 0;

	if (calcs->angleTheta > 90)		//account for angle being in quadrants other than 1
		newAngle = 180;			//check at the end to adjust signs for sine and cosine
	if (calcs->angleTheta > 270)
		newAngle = 360;

	newAngle *= MUL;
	calcs->angleTheta *= MUL;
	newX = calcs->x;
	newY = calcs->y;

	int n;
	for (n = 0; n < CORDIC_NTAB; n++)
	{
	    if (calcs->angleTheta > newAngle)		//rotate counter clockwise if the angle is still less than the desired angle
	    {
	    	newAngle = newAngle + table[n];
	    	newX = newX - (calcs->y >> n);
	    	newY = newY + (calcs->x >> n);
	    }
	    else								//otherwise rotate clockwise
	    {
	    	newAngle = newAngle - table[n];
	    	newX = newX + (calcs->y >> n);
	    	newY = newY - (calcs->x >> n);
	    }
	    calcs->x = newX;
	    calcs->y = newY;
	}

	calcs->angleTheta /= MUL;
	if (calcs->angleTheta > 90 && calcs->angleTheta <= 270)		//if angle is in 2nd or 3rd quadrante, flip signs
	{
		calcs->x = -newX;
		calcs->y = -newY;
	}

}

void calculateArcHypZ(calculations *calcs){
	calculateArcHypXY(calcs);	//stores a value for angleTheta and the hypotenuse of XY
	long newX, newZ, anglePhi;	//Calculates ArcHyp between HypXY and z.
								//HypXY is stored under calcs->x in calculateArcHypXY

		if (calcs->x < 0)		//if x is negative, set the initial angleTheta to 180 and flip sign of x and z
		{
			anglePhi = 180;
			calcs->x *= -1;
			calcs->z *= -1;
		}
		else if (calcs->z < 0)	//if x is positive AND z is negative, account for angleTheta being in the fourth quadrant
			anglePhi = 360;
		else
			anglePhi = 0;

		//calcs->x *= MUL;
		calcs->z *= MUL;
		anglePhi *= MUL;
		newX = calcs->x;
		newZ = calcs->z;

		int n;
		for (n = 0; n < CORDIC_NTAB; n++)
		{
			if (newZ > 0)
			{									//rotate clockwise if z is positive
				newX = newX + (calcs->z >> n);
				newZ = newZ - (calcs->x >> n);
				anglePhi += table[n];				//increase anglePhi by rotation amount
			}
			else								//otherwise rotate counter-clockwise
			{
				newX = newX - (calcs->z >> n);
				newZ = newZ + (calcs->x >> n);	//decrease anglePhi by rotation amount
				anglePhi -= table[n];
			}
			calcs->x = newX;
			calcs->z = newZ;
		}
		calcs->anglePhi = anglePhi;					//store the total rotated anglePhi in calcs
		calcs->x = CORDIC_GAIN * newX;			//store the hypotenuse in x

}

