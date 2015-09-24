#include <stdio.h>
#include <stdlib.h>
#include "sensor.h"

int counter = 0;

int getNextData(){
	static const char filename[] = "ECG.txt";
	FILE *file = fopen(filename,"r");
	int value;
	if(fscanf(file, "%i", &value)!=EOF){
		for(int j = 0; j<counter; j++){
			fscanf(file, "%i", &value);
		}
	}
	counter++;
	return value;
}
