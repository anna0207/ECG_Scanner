
#include <stdio.h>
#include "sensor.h"
#include "Filter.h"


int main(void) {
	//const int size = 50;
	int data[13]={0};
	int lowPass[33]={0};
	int movingWindow[50]={0};
	int n = 0;

	int j = 0;
	for(int i = 0; i<55; i++){
		filter(getNextData(), data, lowPass);
		if(i == 50){
			j=0;
		}
		//printf("%d, ", j);
		//printf("%d, ", x[j]);
		//printf("%d\n", y[j]);
		j++;
	}
	return 0;
}
