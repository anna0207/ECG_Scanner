#include <stdio.h>
#include "sensor.h"
#include "filter.h"
#include "peaks.h"

int main(void) {
	int data[13] = { 0 };
	int lowPass[33] = { 0 };
	int highPass[5] = { 0 };
	int deriv[31] = { 0 };
	int squar[31] = { 0 };
	int mwi[3] = { 0 };
	int time[8] = { 0 };
	int rPeaks[8] = { 0 };
	int counter = 0;

	for (int i = 0; i < 10000; i++) {
		data[i % 13] = getNextData();
		lowpass(data, lowPass, 13, 33, i);
		highpass(lowPass, highPass, 33, 5, i);
		derivative(highPass, deriv, 5, 31, i);
		squaring(deriv, squar, 31, 31, i);
		movingWindow(squar, mwi, 31, 3, i);
		counter = findPeaks(mwi, i, time, rPeaks);
		if (counter != -1) {
			int intensity = rPeaks[counter % 8];
			int RRinterval = time[counter % 8] - time[(counter - 1 + 8) % 8];
			int pulse = 60000 / (RRinterval * 1000 / 250);
			printf("R-Peak intensity: %d\nTime-value: %d s (%d samples)\nRR-interval: %d\nPulse: %d beats/minute\n\n",
					intensity, i / 250, i, RRinterval, pulse);
			if (rPeaks[counter % 8] < 2000) {
				printf("WARNING! low R-peak value\n");
			}
		}
	}
	return 0;
}
