#include <stdio.h>
#include <stdlib.h>

void imprime(long);
unsigned char * BigEndianLong2LittleEndianChar(unsigned long in);

void main(){
	unsigned int a=30000;
	unsigned long b=40000000;
char *g;
	imprime(a);
	imprime(b);
	g = BigEndianLong2LittleEndianChar(a);
	g = BigEndianLong2LittleEndianChar(b);
}

void imprime(long c){
	printf("Es:%ld\n", sizeof(c));
}

unsigned char * BigEndianLong2LittleEndianChar(unsigned long in){
	if(in <= 32767){
		printf("%lX\n", in);
		unsigned char *buffer2 = malloc(sizeof(char)*2);
		buffer2[0] = (char)(in>>8);
		buffer2[1] = (char)(in); ///checar si se puede como abajo, en caso de ser valido
		//printf("%x %x\n", buffer2[1], buffer2[0]); 
		return  buffer2;
	}else if(in > 32767){
		printf("%lX\n", in);
		unsigned char *buffer4 = malloc(sizeof(char)*4);
		buffer4[0] = ((in>>24));  // move byte 3 to byte 0
        buffer4[1] = ((in<<8)>>24);// move byte 1 to byte 2
        buffer4[2] = ((in<<16)>>24); // move byte 2 to byte 1
        buffer4[3] = ((in<<24)>>24); // byte 0 to byte 3
		//printf("%x %x %x %x\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
		return buffer4;
	} 
}