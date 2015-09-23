#include <stdio.h>
#include "sensor.h"
#include "filter.h"
#include "peaks.h"


int main(void) {
	int data[13]={0};
	int lowPass[33] = {0};
	int highPass[5] = {0};
	int deriv[31] = {0};
	int squar[31] = {0};
	int mwi[3] = {0};
	int time[8] = {0};
	int peaks[8] = {0};
	int rPeaks[8] = {0};
	int counter = 0;

	for(int i = 0; i<2000; i++){
		data[i%13] = getNextData();
		lowpass(data, lowPass, 13, 33, i);
		highpass(lowPass, highPass, 33, 5, i);
		derivative(highPass, deriv, 5, 31, i);
		squaring(deriv, squar, 31, 31, i);
		movingWindow(squar, mwi, 31, 3, i);
		//printf("n = %d\n", i);
		counter = findPeaks(mwi, i, time, peaks, rPeaks);
		//if(counter != -1){
		//	printf("n = %d, %d, %d, %d, %d, %d, %d, %d, %d\n", i+1, rPeaks[0], rPeaks[1], rPeaks[2], rPeaks[3], rPeaks[4], rPeaks[5], rPeaks[6], rPeaks[7] );
		//}
		if(counter != -1){
			printf("R peak-value: %d\nTime value: %d\nPulse: %d\n", rPeaks[counter], i/250, 60*(time[counter%8]-time[(counter-1+8)%8])/250);
			printf("time[counter%8] = %d, time[(counter-1+8)%8] = %d \n\n", time[counter%8], time[(counter-1+8)%8]);
		}

	}
	return 0;
}
