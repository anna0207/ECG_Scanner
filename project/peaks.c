#include <stdio.h>
#include "peaks.h"

int findPeaks(int data[], int sample, int time[], int rPeaks[]) {
	static int rPeakCounter = 0;
	static int peakCounter = 0;
	static int peaks[8] = { 0 };
	int foundRPeak = -1;
	int newData = sample % 3;
	int mid = (newData - 1 + 3) % 3;
	int first = (newData - 2 + 3) % 3;
	if (data[mid] > data[first] && data[mid] > data[newData]) {
		time[rPeakCounter % 8] = sample - 2;
		peaks[peakCounter % 8] = data[mid];
		foundRPeak = findRPeaks(time, rPeaks, rPeakCounter, peakCounter, peaks);
		if (foundRPeak != -1) {
			rPeakCounter++;
		}
		peakCounter++;
	}
	return foundRPeak;
}

int findRPeaks(int time[], int rPeaks[], int rPeakCounter, int peakCounter, int peaks[]) {
	static int spkf = 4000;
	static int npkf = 1500;
	static int threshold1 = 1500 + (4000 - 1500) / 4;
	static int threshold2 = (1500 + (4000 - 1500) / 4) / 2;
	static int rrAvg1 = 151;
	static int rrAvg2 = 151;
	static int rrMiss = 1.66 * 151;
	static int rrLow = 0.92 * 151;
	static int rrHigh = 1.16 * 151;
	static int recentRR_OK[8] = { 0 };
	static int recentRR[8] = { 0 };
	static int warningCounter = 0;
	static recentRRCounter = 0;
	static recentRROKCounter = 0;
	int found = 0;

	if (peaks[peakCounter % 8] > threshold1) {
		int rr = time[rPeakCounter % 8] - time[(rPeakCounter - 1 + 8) % 8];
		if (rr > rrLow && rr < rrHigh) {
			rPeaks[rPeakCounter % 8] = peaks[peakCounter % 8];
			found = 1;

			spkf = 0.125 * peaks[peakCounter % 8] + 0.875 * spkf;
			recentRR_OK[rPeakCounter % 8] = rr;
			recentRR[rPeakCounter % 8] = rr;
			recentRRCounter++;
			recentRROKCounter++;

			int n = recentRROKCounter;
			if (n > 8) {
				n = 8;
			}

			int sumRR_OK = 0;
			for (int i = 0; i < n; i++) {
				sumRR_OK += recentRR_OK[i];
			}
			rrAvg2 = sumRR_OK / n;

			n = recentRRCounter;
			if (n > 8) {
				n = 8;
			}
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
				printf("WARNING! Unstable heartbeat\n\n");
			}
			if (rr > rrMiss) {
				//Search backwards
				int i = 0;
				while (peaks[(peakCounter - i + 8) % 8] <= threshold2 && i < 8) {
					i++;
				}
				if (i != 8) {
					int rPeak = peaks[(peakCounter - i + 8) % 8];
					rPeaks[rPeakCounter % 8] = rPeak;
					found = 1;

					spkf = 0.25 * rPeak + 0.75 * spkf;
					recentRR[rPeakCounter % 8] = rr;
					recentRRCounter++;

					int n = recentRRCounter;
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
			}
		}
	} else {
		npkf = 0.125 * peaks[peakCounter % 8] + 0.875 * npkf;
		threshold1 = npkf + 0.25 * (spkf - npkf);
		threshold2 = threshold1 * 0.5;
	}

	if (found == 1) {
		return rPeakCounter;
	} else {
		return -1;
	}
}
