#include <stdio.h>
#include <stdlib.h>
#include "sensor.h"

int data[];

int saveData(int counter) {
	static const char filename[] = "ECG.txt";
	static int data[10000];
	if (counter == 0) {
		FILE *file = fopen(filename, "r");
		int value;
		for (int i = 0; i < 10000; i++) {
			fscanf(file, "%i", &value);
			data[i] = value;
		}
	}
	return data[counter];
}
int getNextData() {
	static int counter = 0;
	int value = saveData(counter);
	counter++;
	return value;
}
