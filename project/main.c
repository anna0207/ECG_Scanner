
#include <stdio.h>
#include "sensor.h"
#include "Filter.h"


int main(void) {
	int data[13]={0};
	int lowPass[33] = {0};
	int highPass[5] = {0};
	int deriv[31] = {0};
	int squar[31] = {0};
	int mwi[50]={0};

	for(int i = 0; i<55; i++){
		data[i%13] = getNextData();
		lowpass(data, lowPass, 13, 33, i);
		highpass(lowPass, highPass, 33, 5, i);
		derivative(highPass, deriv, 5, 31, i);
		squaring(deriv, squar, 31, 31, i);
		movingWindow(squar, mwi, 31, 50, i);
		printf("n = %d, x = %d, y = %d\n", i+1, squar[i%31], mwi[i%50]);
	}
	return 0;
}
