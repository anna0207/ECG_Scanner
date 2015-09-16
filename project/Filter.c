
#include <stdio.h>
#include "Filter.h"

void filter(int data, int x[], int y[]){
	int N = 30;
	int lowPass[33];
	int highPass[5];
	int derivative[31];
	int squaring[31];

	static int n = 0;

	x[n%13] = data;

	lowpass(x, lowPass, 13, 33, n);
	//highpass(lowPass, highPass, 5, n);
	//derivative();
	//squaring();
	//mwi();
	n++;

}
void lowpass(int x[], int y[], int inputSize, int outputSize, int n){
	y[n%outputSize] = (2*y[(n-1+outputSize)%outputSize]-y[(n-2+outputSize)%outputSize]+(x[n]-2*x[(n-6+inputSize)%inputSize]+x[(n-12+inputSize)%inputSize])/32);
	printf("y = %d,%d\n", x[n],y[n]);
}

/*highpass(int x[], int y[], int size, int n){
	y[n%size]=y[(n-1+size)%size]-x[n]/32+x[(n-16+size)%size]-x[(n-17+size)%size]+x[(n-32+size)%size]/32;
}

derivative(){
	y3[n]=1/8.0*(2*y2[n]+y2[(n-1+size)%size]-y2[(n-3+size)%size]-2*y2[(n-4+size)%size]);
}

squaring(){
	y4[n] = y3[n]*y3[n];
}

mwi(){
	int ytemp = 0;
	for(int i = 1; i <= N; i++){
		ytemp += (y4[(n-(N-i)+size)%size]);
	}
	//matches x_mwi_div_after file
	y5[n] = ytemp/N;

}
*/


