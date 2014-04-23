#include <stdio.h>

typedef int bool;
enum { false, true };

//46,000 vertices
//146500 - 46000 faces

void putBinary(FILE *f,double n) {
	n *= 256 * 256;
	int small = (int)(n) % 256;
	int large = (int)(n) / 256;
	fputc(small,f);
	fputc(large,f);
}

void putInt(FILE *f,int n) {
	int small = (int)(n) % 256;
	int large = (int)(n) / 256;
	fputc(small,f);
	fputc(large,f);
}


double doubleFromStream(FILE *f) {
	double number = 0;
	double size = 1.0;
	double adjust = 0.1;
	int i = 0;
	bool foundDot = false;
	while (true) {
		char c = fgetc(f);
		if (c == '-') {
			adjust *= -1;
			continue;
		}
		if (c == ' ' || c == '\n') {
			return number * adjust;
		}
		if (c == '.') {
			foundDot = true;
			continue;
		}
		if (!foundDot) {
			adjust *= 10.0;
		}
		number += size * ((int)(c - '0'));
		size *= 0.1;
	}
	return -1;
}

int main(int argc, char **argv) {

	int vertex = 0;

	int thru = 0;

	for (thru = 0; thru < 2; thru++) {
		FILE *mesh;
		FILE *tiny;
		mesh = fopen(argv[1],"r");
		tiny = fopen("small.boj","wb");

		bool newline = true;

		double numberSize = 1;
		double number = 0;
		char c;
		int i;
		for (i = 0; i < 256; i++) {
			fputc(i,tiny);
		}

		putInt(tiny,vertex);
		printf("Put %i vertecies\n",vertex);

		while (true) {
			c = fgetc(mesh);
			if (c == EOF) {
				break;
			}
			if (c == 'v') {
				//v double double double\n
				fgetc(mesh); //The first space.
				double one = doubleFromStream(mesh);
				double two = doubleFromStream(mesh);
				double three = doubleFromStream(mesh);
				//[0, -1.456875, -330.59314]
				//[1066.43811, 1235.279785, 361.556854]
				//  0    -- 1100  ONE
				// -100  -- 1300  TWO
				// -400  -- 400   THREE
				one += 0.0;
				one /= (1100.0 + 0.0);
				two += 100.0;
				two /= (1300.0 + 100.0);
				three += 400.0;
				three /= (400.0 + 400.0);
				putBinary(tiny,one);
				putBinary(tiny,two);
				putBinary(tiny,three);
				vertex++;
			} else {
				//f int int int\n
				int one;
				int two;
				int three;
				fscanf(mesh," %i %i %i\n",&one,&two,&three);
				putInt(tiny,one);
				putInt(tiny,two);
				putInt(tiny,three);
			}
		}
		fclose(mesh);
		fclose(tiny);
		//Now we have vertex to print to the top of it.
	}
}