#include <stdio.h>
#include "peaks.h"

int findPeaks(int data[], int sample, int time[], int peaks[], int rPeaks[]) {
	static int peakCounter = 0;
	int counter = -1;
	int newData = sample % 3;
	int mid = (newData - 1 + 3) % 3;
	int first = (newData - 2 + 3) % 3;
	if (data[mid] > data[first] && data[mid] > data[newData]) {
		time[peakCounter % 8] = sample - 1;
		peaks[peakCounter % 8] = data[mid];
		counter = threshold(time, peaks, rPeaks, peakCounter);
		if (counter != -1) {
			peakCounter++;
		}
	}
	return counter;
}

int threshold(int time[], int peaks[], int rPeaks[], int peakCounter) {
	static int spkf = 4500;
	static int npkf = 200;
	static int threshold1 = 200 + (4500-200)/4;
	static int threshold2 = (200 + (4500-200)/4)/2;
	static int rrAvg1 = 162;
	static int rrAvg2 = 162;
	static int rrMiss = 162 * 1.66;
	static int rrLow = 162 * 0.92;
	static int rrHigh = 162 * 1.16;
	static int recentRR_OK[8] = { 0 };
	static int recentRR[8] = { 0 };
	int found = 0;
	int warningCounter = 0;

	if (peaks[peakCounter % 8] > threshold1) {
		int rr = time[peakCounter % 8] - time[(peakCounter - 1 + 8) % 8];
		if (rr > rrLow && rr < rrHigh) {
			rPeaks[peakCounter % 8] = peaks[peakCounter % 8];
			found = 1;

			spkf = 0.125 * peaks[peakCounter % 8] + 0.875 * spkf;
			recentRR_OK[peakCounter % 8] = rr;
			recentRR[peakCounter % 8] = rr;

			int sumRR_OK = 0;
			for (int i = 0; i < 8; i++) {
				sumRR_OK += recentRR_OK[i];
			}
			rrAvg2 = sumRR_OK / 8;

			int sumRR = 0;
			for (int j = 0; j < 8; j++) {
				sumRR += recentRR[j];
			}
			rrAvg1 = sumRR / 8;
			printf("Av2: %d\n", rrAvg2);
			rrLow = 0.92 * rrAvg2;
			rrHigh = 1.16 * rrAvg2;
			rrMiss = 1.66 * rrAvg2;
			threshold1 = npkf + 0.25 * (spkf - npkf);
			threshold2 = 0.5 * threshold1;
			warningCounter = 0;
		} else {
			warningCounter++;
			if (warningCounter >= 5) {
				printf("WARNING! Unstable heartbeat");
			}
			if (rr > rrMiss) {
				printf("MISS");
				//Search backwards
				int i = 0;
				while (peaks[7 - i] <= threshold2 && i < 8) {
					i++;
				}
				if (i != 8) {
					int rPeak = peaks[7 - i];
					rPeaks[peakCounter % 8] = rPeak;
					found = 1;

					spkf = 0.25 * rPeak + 0.75 * spkf;
					recentRR[peakCounter % 8] = rr;

					int sumRR = 0;
					for (int j = 0; j < 8; j++) {
						sumRR += recentRR[j];
					}
					printf("Av1: %d\n", rrAvg1);
					rrAvg1 = sumRR / 8;
					rrLow = rrAvg1 * 0.92;
					rrHigh = rrAvg1 * 1.16;
					rrMiss = rrAvg1 * 1.66;
					threshold1 = npkf + 0.25 * (spkf - npkf);
					threshold2 = 0.5 * threshold1;
				}
				peakCounter++;
			}
		}
	} else {
		npkf = 0.125 * peaks[peakCounter % 8] + 0.875 * npkf;
		threshold1 = npkf + 0.25 * (spkf - npkf);
		threshold2 = threshold1 * 0.5;
	}

	if (found == 1) {
		return peakCounter - 1;
	} else {
		return -1;
	}
}
