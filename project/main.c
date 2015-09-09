
#include <stdio.h>
#include "sensor.h"

#define size 50
#define N 30
int x[size];
int y[size];
int y2[size];
int y3[size];
int y4[size];
int y5[size];
int n = 0;


int main(void) {
	for(int i = 0; i<55; i++){
		filter(getNextData());
	}
	for(int i = 0; i<55; i++){
		printf("%d, ", x[i]);
		printf("%d\n", y[i]);
	}

	getNextData();

	return 0;
}

filter(int data){
	x[n] = data;

	lowpass();
	highpass();
	derivative();
	squaring();
	mwi();

	n++;
	if(n >= size){
		n = 0;
	}
}
lowpass(){
	y[n] = (2*y[(n-1+size)%size]-y[(n-2+size)%size]+(x[n]-2*x[(n-6+size)%size]+x[(n-12+size)%size])/32);
}

highpass(){
	y2[n]=y2[(n-1+size)%size]-y[n]/32.0+y[(n-16+size)%size]-y[(n-17+size)%size]+y[(n-32+size)%size]/32.0;
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
	y5[n] = ytemp/N;
}
