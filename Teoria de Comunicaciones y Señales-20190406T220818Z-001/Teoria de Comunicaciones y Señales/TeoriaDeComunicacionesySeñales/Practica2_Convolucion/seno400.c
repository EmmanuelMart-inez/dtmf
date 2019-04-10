#include <string.h>
#include <stdio.h>
#include <math.h>

void main(void)
{
 char cadena[5];
 unsigned int i, nn;
 unsigned short int n;
 short int s;
 double t;
 FILE *archivo;
 
 archivo=fopen("seno400.wav","wb");
 if (archivo!=NULL)
 {
  strcpy(cadena,"RIFF"); fwrite(cadena,4,1,archivo);
  nn=160036;             fwrite(&nn,4,1,archivo);
  strcpy(cadena,"WAVE"); fwrite(cadena,4,1,archivo);
  strcpy(cadena,"fmt "); fwrite(cadena,4,1,archivo);
  nn=16;                 fwrite(&nn,4,1,archivo);
  n=1;                   fwrite(&n,2,1,archivo);
  n=1;                   fwrite(&n,2,1,archivo);
  nn=8000;               fwrite(&nn,4,1,archivo);
  nn=16000;              fwrite(&nn,4,1,archivo);
  n=2;                   fwrite(&n,2,1,archivo);
  n=16;                  fwrite(&n,2,1,archivo);
  strcpy(cadena,"data"); fwrite(cadena,4,1,archivo);
  nn=160000;             fwrite(&nn,4,1,archivo);
  for (i=0; i<80000; i++)
  {
   t=1.0/8000.0*i;
   s=(short int)(sin(2.0*M_PI*400.0*t)*32767.0);
   fwrite(&s,2,1,archivo);
  }
  fclose(archivo);
 }
 else
  printf("No se pudo generar el archivo\n");
}
