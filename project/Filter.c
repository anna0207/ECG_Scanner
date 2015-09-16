#include <stdio.h>
#include "Filter.h"

void lowpass(int x[], int *y, int inputSize, int outputSize, int n){
	y[n%outputSize] = (2*y[(n-1+outputSize)%outputSize]-y[(n-2+outputSize)%outputSize]+(x[n%inputSize]-2*x[(n-6+inputSize)%inputSize]+x[(n-12+inputSize)%inputSize])/32);
}

void highpass(int x[], int y[], int inputSize, int outputSize, int n){
	y[n%outputSize]=y[(n-1+outputSize)%outputSize]-x[n%inputSize]/32+x[(n-16+inputSize)%inputSize]-x[(n-17+inputSize)%inputSize]+x[(n-32+inputSize)%inputSize]/32;
}

derivative(int x[], int y[], int inputSize, int outputSize, int n){
	y[n%outputSize]=(2*x[n%inputSize]+x[(n-1+inputSize)%inputSize]-x[(n-3+inputSize)%inputSize]-2*x[(n-4+inputSize)%inputSize])/8;
}


squaring(int x[], int y[], int inputSize, int outputSize, int n){
	y[n%outputSize] = x[n%inputSize]*x[n%inputSize];
}

movingWindow(int x[], int y[], int inputSize, int outputSize, int n){
	int ytemp = 0;
	int N = 30;
	for(int i = 1; i <= N; i++){
		ytemp += x[(n-(N-i)+inputSize)%inputSize];
	}
	y[n%outputSize] = ytemp/N;

}

