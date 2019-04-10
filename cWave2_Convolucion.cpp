#include <string.h>
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "wave.h"

#define M 100

int main(void)
{
 FILE *archivo;
 FILE *archivoEntrada;
 unsigned char buffer4[4];
 unsigned char buffer2[2];
 struct HEADER header;
 
 /*Convoluci�n */
 //Orden del sistema FIR de orden, M=100.
short x[M]; //funcion muestra
double h[M]; //funci�n respuesta al impulso del circuito RC

 char ih;
  //double alpha =(2*3.141592653589793*2000)/44100;
  for (ih=0; ih<100; ih++)
	{
			h[ih] = exp(-ih*(2*3.141592653589793*2000)/44100); 
	}
	
	archivo=fopen("probebash.wav","wb");	
    archivoEntrada=fopen("probe1.wav","rb");
 if (archivo!=NULL && archivoEntrada!=NULL) 
 {
    fread(header.riff, sizeof(header.riff), 1, archivoEntrada);	fwrite(header.riff, sizeof(header.riff),1,archivo);
    fread(buffer4, sizeof(buffer4), 1, archivoEntrada);	fwrite(buffer4, sizeof(buffer4),1,archivo);
    header.overall_size  = buffer4[0] | 
						(buffer4[1]<<8) | 
						(buffer4[2]<<16) | 
						(buffer4[3]<<24);
   fread(header.wave, sizeof(header.wave), 1, archivoEntrada);	fwrite(header.wave, sizeof(header.wave),1,archivo);
   fread(header.fmt_chunk_marker, sizeof(header.fmt_chunk_marker), 1, archivoEntrada);	fwrite(header.fmt_chunk_marker, sizeof(header.fmt_chunk_marker),1,archivo); 
   fread(buffer4, sizeof(buffer4), 1, archivoEntrada);	fwrite(buffer4, sizeof(buffer4),1,archivo);
   header.length_of_fmt = buffer4[0] |
							(buffer4[1] << 8) |
							(buffer4[2] << 16) |
							(buffer4[3] << 24);  
   fread(buffer2, sizeof(buffer2), 1, archivoEntrada);	fwrite(buffer2, sizeof(buffer2),1,archivo);
	header.format_type = buffer2[0] | (buffer2[1] << 8);
   fread(buffer2, sizeof(buffer2), 1, archivoEntrada);	fwrite(buffer2, sizeof(buffer2),1,archivo);
   header.channels = buffer2[0] | (buffer2[1] << 8);
   printf("(23-24) Channels: %u \n", header.channels);
   fread(buffer4, sizeof(buffer4), 1, archivoEntrada);	fwrite(buffer4, sizeof(buffer4),1,archivo); 
   header.sample_rate = buffer4[0] |
						(buffer4[1] << 8) |
						(buffer4[2] << 16) |
						(buffer4[3] << 24);
   fread( buffer4, sizeof( buffer4), 1, archivoEntrada);	fwrite(buffer4, sizeof(buffer4),1,archivo); 
    header.byterate  = buffer4[0] |
						(buffer4[1] << 8) |
						(buffer4[2] << 16) |
						(buffer4[3] << 24);
   fread(buffer2, sizeof(buffer2), 1, archivoEntrada);	fwrite(buffer2, sizeof(buffer2),1,archivo); 
   header.block_align = buffer2[0] |
					(buffer2[1] << 8);
   fread(buffer2, sizeof(buffer2), 1, archivoEntrada);	fwrite(buffer2, sizeof(buffer2),1,archivo);
   header.bits_per_sample = buffer2[0] |
					(buffer2[1] << 8); 
					
   fread(header.data_chunk_header, sizeof(header.data_chunk_header), 1, archivoEntrada);	fwrite(header.data_chunk_header, sizeof(header.data_chunk_header),1,archivo); 
   fread(buffer4, sizeof(buffer4), 1, archivoEntrada);	 
	header.data_size = buffer4[0] |
				(buffer4[1] << 8) |
				(buffer4[2] << 16) | 
				(buffer4[3] << 24 );
	fwrite(buffer4, sizeof(buffer4),1,archivo);  
	 printf("(41-44) Size of data chunk: %u \n", header.data_size);
	 // calculate no.of samples
	 long num_samples = (8 * header.data_size) / (header.channels * header.bits_per_sample);
	 printf("Number of samples:%lu \n", num_samples);
	
	 long size_of_each_sample = (header.channels * header.bits_per_sample) / 8;
	 printf("Size of each sample:%ld bytes\n", size_of_each_sample);  
	  
  unsigned char data_buffer[sizeof(size_of_each_sample)];
  
  //Convolucionar cada una de las muestras
  int i,n,k;
  /*Imprimir respuesta al impulso del circuito RC
  for(i=0; i<99; i++){
  	printf("//%1.15f\n", h[i]);
  }*/
  short data_in_channel = 0;
  double sum=0;
  double* conv = (double *) malloc(num_samples*sizeof(double));
  for (n=0; n<num_samples+37; n++)
  //for (n=0; n<30; n++)
  {	//r = 0;
	   if(n<num_samples){
	   		fread(&data_in_channel, 2, 1, archivoEntrada);		
			for(k=M-1; k>=0; k--){
				sum += data_in_channel*x[k-n];
			}
			sum = sum/32768;
			if(sum > 32767){
				sum = 32767;
			}else if(sum < -32767){
				sum = -32767;
			}		
			//printf("(%lf)\n",sum);
			conv[n] = sum;
			data_in_channel = conv[n];
			fwrite(&data_in_channel, 2,1,archivo);
			sum = 0;
		}else
		{
			fread(data_buffer, sizeof(data_buffer), 1, archivoEntrada);	
			fwrite(data_buffer, sizeof(data_buffer),1,archivo);  
		}
  }
  fclose(archivo);
  fclose(archivoEntrada);

 }else
  printf("No se pudo generar el archivo\n");

  return 0;
}
