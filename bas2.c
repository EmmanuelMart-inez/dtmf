#include <stdio.h>
#define FLOAT_TO_INT(x) ((x)>=0?(int)((x)+0.5):(int)((x)-0.5))
void main(){
	float a = 1.3;
	float b = 1.6;

	int c = FLOAT_TO_INT(a);
	int v = FLOAT_TO_INT(b);

	printf("%d", c);

	printf("%d", v);
}