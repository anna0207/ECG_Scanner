
#include <stdio.h>

void searchPeaks(int filterData[], int time, int **peaks) {
	static int peakCounter = 0;

	if (filterData[1] > filterData[0] && filterData[1] > filterData[2]) {
		peaks[0][peakCounter] = time-1;
		peaks[1][peakCounter] = filterData[1];
		peakCounter++;
	}
}

void threshold(int **peaks, int peakCounter) {
	static int spkf, npkf, threshold1, threshold2, n;
	static int rrMiss, rrAvg1, rrAvg2;
	static int rrLow = 0;
	static int rrHigh = 1000000;

	int rPeaks[8] = {0};
	int recentRR_OK[8] = {0};
	int recentRR[8] = {0};
	int rCounter = 0;

	if (peaks[1][peakCounter] > threshold1) {
		int rr = peaks[0][peakCounter]-peaks[0][peakCounter-1];
		if (rr > rrLow && rr < rrHigh) {
			rPeaks[rCounter] = rr;

			spkf = 0.125*peaks[1][peakCounter] + 0.875*spkf;
			recentRR_OK[rCounter] = rr;
			recentRR[rCounter] = rr;

			int sumRR_OK = 0;
			for (int i = 0; i < 8; i++) {
				sumRR_OK += recentRR_OK[i];
			}
			rrAvg2 = sumRR_OK/8;

			int sumRR = 0;
			for (int j = 0; j < 8; j++) {
				sumRR += recentRR[j];
			}
			rrAvg1 = sumRR/8;
			rrLow = 0.92*rrAvg2;
			rrHigh = 1.16*rrAvg2;
			rrMiss = 1.66*rrAvg2;
			threshold1 = npkf + 0.25*(spkf-npkf);
			threshold2 = 0.5*threshold1;

			rCounter++;

		} else if (rr > rrMiss) {
			int i = 1;
			while(peaks[n-i] <= threshold2 && i < 8) {
				i++;
			}
			rPeaks[rCounter] = rr;
			spkf = 0.25*peaks[1][peakCounter] + 0.75*spkf;
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
		npkf = 0.125*peaks[1][peakCounter] + 0.875*npkf;
		threshold1 = npkf + 0.25*(spkf-npkf);
		threshold2 = threshold1*0.5;
	}
}
