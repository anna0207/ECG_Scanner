#include <stdio.h>
#include <stdlib.h>
#include "sensor.h"

int getNextData(){
	static const char filename[] = "ECG.txt";
	FILE *file = fopen(filename,"r");
	int value;

	fscanf(file, "%i", &value);
	printf("%d\n", value);

	return 0;
}
