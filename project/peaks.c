#include <stdio.h>
#include "peaks.h"

void findPeaks(int data[], int sample, int time[], int peaks[], int rPeaks[]) {
	static int peakCounter = 0;
	int newData = sample%3;
	int mid = (newData-1+3)%3;
	int first = (newData-2+3)%3;
	if (data[mid] > data[first] && data[mid] > data[newData]) {
		time[peakCounter] = sample-1;
		peaks[peakCounter] = data[mid];
		threshold(time, peaks, rPeaks, peakCounter);
		peakCounter++;
	}
	if (peakCounter >= 8){
		peakCounter = 0;
	}
}

void threshold(int time[], int peaks[], int rPeaks[], int peakCounter) {
	static int spkf, npkf, threshold1, threshold2;
	static int rrMiss, rrAvg1, rrAvg2;
	static int rrLow = 0;
	static int rrHigh = 0;
	static int rCounter = 0;
	static int recentRR_OK[8] = {0};
	static int recentRR[8] = {0};

	if (peaks[peakCounter] > threshold1) {
		int rr = time[peakCounter]-time[peakCounter-1];
		if (rr > rrLow && rr < rrHigh) {
			rPeaks[rCounter] = rr;

			spkf = 0.125*peaks[peakCounter] + 0.875*spkf;
			recentRR_OK[rCounter] = rr;
			recentRR[rCounter] = rr;

			//Get number of rPeaks saved
			int numberRPeaks = 8;
			if(rCounter < 8){
				numberRPeaks = rCounter+1;
			}

			int sumRR_OK = 0;
			for (int i = 0; i < 8; i++) {
				sumRR_OK += recentRR_OK[i];
			}
			rrAvg2 = sumRR_OK/numberRPeaks;


			int sumRR = 0;
			for (int j = 0; j < 8; j++) {
				sumRR += recentRR[j];
			}
			rrAvg1 = sumRR/numberRPeaks;

			rrLow = 0.92*rrAvg2;
			rrHigh = 1.16*rrAvg2;
			rrMiss = 1.66*rrAvg2;
			threshold1 = npkf + 0.25*(spkf-npkf);
			threshold2 = 0.5*threshold1;

			rCounter++;
		} else if (rr > rrMiss) {
			//Search backwards
			int i = 0;
			while(peaks[7-i] <= threshold2 && i < 8) {
				i++;
			}
			int rPeak = peaks[7-i];
			rPeaks[rCounter] = rPeak;

			spkf = 0.25*peaks[peakCounter] + 0.75*spkf;
			recentRR[rCounter] = rr;
			int sumRR = 0;
			for (int j = 0; j < 8; j++) {
				sumRR += recentRR[j];
			}
			rrAvg1 = sumRR/8;
			rrLow = rrAvg1*0.92;
			rrHigh = rrAvg1*1.16;
			rrMiss = rrAvg1*1.66;
			threshold1 = npkf + 0.25*(spkf-npkf);
			threshold2 = 0.5*threshold1;
		}

	} else {
		npkf = 0.125*peaks[peakCounter] + 0.875*npkf;
		threshold1 = npkf + 0.25*(spkf-npkf);
		threshold2 = threshold1*0.5;
	}
}
