#include <stdio.h>
#include "peaks.h"

int spkf = 4000;
int npkf = 2000;
int threshold1 = 2000 + (4000 - 2000) / 4;
int threshold2 = (2000 + (4000 - 2000) / 4) / 2;
int rrAvg1 = 151;
int rrAvg2 = 151;
int rrMiss = 1.66 * 151;
int rrLow = 0.92 * 151;
int rrHigh = 1.16 * 151;
int recentRR_OK[8] = { 0 };
int recentRR[8] = { 0 };
int peakCounter = 0;

int findPeaks(int data[], int sample, int time[], int peaks[], int rPeaks[]) {
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

			int n = peakCounter + 1;
			if (n > 8) {
				n = 8;
			}

			int sumRR_OK = 0;
			for (int i = 0; i < n; i++) {
				sumRR_OK += recentRR_OK[i];
			}
			rrAvg2 = sumRR_OK / n;

			int sumRR = 0;
			for (int j = 0; j < n; j++) {
				sumRR = recentRR[j];
			}
			rrAvg1 = sumRR / n;
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

					int n = peakCounter + 1;
					if (n > 8) {
						n = 8;
					}

					int sumRR = 0;
					for (int j = 0; j < n; j++) {
						sumRR += recentRR[j];
					}
					rrAvg1 = sumRR / n;
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
		return peakCounter;
	} else {
		return -1;
	}
}
