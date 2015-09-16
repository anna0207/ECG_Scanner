
#include <stdio.h>

void searchPeaks(int data, int filterData[], int peaks[]) {

	static int sample = 0;
	static int peakCounter = 0;

	filterData[sample] = data;

	if (filterData[sample] > filterData[sample+1] && filterData[sample] > filterData[sample-1]) {
		peaks[peakCounter] = sample+1;
		peakCounter++;
	}

	sample++;
}

//få parametre til at passe med main.c
//Både peak og peaks[]?
void threshold1(int peak, int peaks[], int rPeaks[]) {
	static int spkf, npkf, threshold1, threshold2, count, n;
	static int rrMiss, rrAvg1, rrAvg2;
	static int rrLow = 0;
	static int rrHigh = 1000000;

	//Sæt counter til en værdi så vi kan holde øje med hvor der er en ledig plads i arrays.
	count = ...;
	//Sæt n til den plads i peaks hvor peak er.
	n = ...;

	if (peak > threshold1) {
		int rr = peaks-peaks[n-1];

		if (rr > rrLow && rr < rrHigh) {
			rPeaks[count] = rr;
			spkf = 0.125*peak + 0.875*spkf;
			//Opret arrays tidligere til en bestemt længde?
			recentRR_OK[count] = rr;
			recentRR[count] = rr;

			int sumRR_OK = 0;
			for (int i = 0; i < recentRR_OK.size(); i++) {
				sumRR_OK += recentRR_OK[i];
			}
			rrAvg2 = sumRR_OK/recentRR_OK.size();

			int sumRR = 0;
			for (int j = 0; j < recentRR.size(); j++) {
				sumRR += recentRR[j];
			}
			rrAvg1 = sumRR/recentRR.size();
			rrLow = 0.92*rrAvg2;
			rrHigh = 1.16*rrAvg2;
			rrMiss = 1.66*rrAvg2;
			threshold1 = npkf + 0.25*(spkf-npkf);
			threshold2 = 0.5*threshold1;

			count++;
		} else if (rr > rrMiss) {
			int i = 1;
			while(peaks[n-i] <= threshold2) {
				i++;
			}
			rPeaks[count] = rr;
			spkf = 0.25*peak + 0.75*spkf;
			recentRR[count] = rr;

			int sumRR = 0;
			for (int j = 0; j < recentRR.size(); j++) {
				sumRR += recentRR(j);
			}
			rrAvg1 = sumRR/recentRR.size();
			rrLow = rrAvg1*0.92;
			rrHigh = rrAvg1*1.16;
			rrMiss = rrAvg1*1.66;
			threshold1 = npkf + 0.25*(spkf-npkf);
			threshold2 = 0.5*threshold1;
		}

	} else {
		npkf = 0.125*peak + 0.875*npkf;
		threshold1 = npkf + 0.25*(spkf-npkf);
		threshold2 = threshold1*0.5;
	}




}

