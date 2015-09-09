
#include <stdio.h>

int main(void) {
	static const char filename[] = "ECG.txt";
	FILE *file = fopen(filename,"r");
	int value;

	fscanf(file, "%i", &value);
	printf("%d\n", value);

}
