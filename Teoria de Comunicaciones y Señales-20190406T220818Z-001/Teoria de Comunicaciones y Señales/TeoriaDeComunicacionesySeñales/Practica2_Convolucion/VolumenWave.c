#include <string.h>
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "wave.h"

void main(void)
{
 FILE *archivo;
 
 FILE *archivoEntrada;
 unsigned char buffer4[4];
 unsigned char buffer2[2];
 struct HEADER header;
 
 archivo=fopen("seno403yeah.wav","wb");
 archivoEntrada=fopen("entrada.wav","rb");
 if (archivo!=NULL && archivoEntrada!=NULL) 
 {
    fread(header.riff, sizeof(header.riff), 1, archivoEntrada);	fwrite(header.riff, sizeof(header.riff),1,archivo);
    //strcpy(cadena,"RIFF"); fwrite(cadena,4,1,archivo);
    fread(buffer4, sizeof(buffer4), 1, archivoEntrada);	fwrite(buffer4, sizeof(buffer4),1,archivo);
    header.overall_size  = buffer4[0] | 
						(buffer4[1]<<8) | 
						(buffer4[2]<<16) | 
						(buffer4[3]<<24);
  //nn=160036;             fwrite(&nn,4,1,archivo);
   fread(header.wave, sizeof(header.wave), 1, archivoEntrada);	fwrite(header.wave, sizeof(header.wave),1,archivo);
  //strcpy(cadena,"WAVE"); fwrite(cadena,4,1,archivo);
   fread(header.fmt_chunk_marker, sizeof(header.fmt_chunk_marker), 1, archivoEntrada);	fwrite(header.fmt_chunk_marker, sizeof(header.fmt_chunk_marker),1,archivo); 
  //strcpy(cadena,"fmt "); fwrite(cadena,4,1,archivo);
   fread(buffer4, sizeof(buffer4), 1, archivoEntrada);	fwrite(buffer4, sizeof(buffer4),1,archivo);
   header.length_of_fmt = buffer4[0] |
							(buffer4[1] << 8) |
							(buffer4[2] << 16) |
							(buffer4[3] << 24);  
  //nn=16;                 fwrite(&nn,4,1,archivo);
   fread(buffer2, sizeof(buffer2), 1, archivoEntrada);	fwrite(buffer2, sizeof(buffer2),1,archivo);
  //n=1;                   fwrite(&n,2,1,archivo);
	header.format_type = buffer2[0] | (buffer2[1] << 8);
   fread(buffer2, sizeof(buffer2), 1, archivoEntrada);	fwrite(buffer2, sizeof(buffer2),1,archivo);
   header.channels = buffer2[0] | (buffer2[1] << 8);
   printf("(23-24) Channels: %u \n", header.channels);
  //n=1;                   fwrite(&n,2,1,archivo);
   fread(buffer4, sizeof(buffer4), 1, archivoEntrada);	fwrite(buffer4, sizeof(buffer4),1,archivo); 
   header.sample_rate = buffer4[0] |
						(buffer4[1] << 8) |
						(buffer4[2] << 16) |
						(buffer4[3] << 24);
  //nn=8000;               fwrite(&nn,4,1,archivo);
   fread( buffer4, sizeof( buffer4), 1, archivoEntrada);	fwrite(buffer4, sizeof(buffer4),1,archivo); 
    header.byterate  = buffer4[0] |
						(buffer4[1] << 8) |
						(buffer4[2] << 16) |
						(buffer4[3] << 24);
  //nn=16000;              fwrite(&nn,4,1,archivo);
   fread(buffer2, sizeof(buffer2), 1, archivoEntrada);	fwrite(buffer2, sizeof(buffer2),1,archivo); 
   header.block_align = buffer2[0] |
					(buffer2[1] << 8);
  //n=2;                   fwrite(&n,2,1,archivo);
   fread(buffer2, sizeof(buffer2), 1, archivoEntrada);	fwrite(buffer2, sizeof(buffer2),1,archivo);
   header.bits_per_sample = buffer2[0] |
					(buffer2[1] << 8); 
					
  //n=16;                  fwrite(&n,2,1,archivo);
   fread(header.data_chunk_header, sizeof(header.data_chunk_header), 1, archivoEntrada);	fwrite(header.data_chunk_header, sizeof(header.data_chunk_header),1,archivo); 
  //strcpy(cadena,"data"); fwrite(cadena,4,1,archivo);
   fread(buffer4, sizeof(buffer4), 1, archivoEntrada);	 
	header.data_size = buffer4[0] |
				(buffer4[1] << 8) |
				(buffer4[2] << 16) | 
				(buffer4[3] << 24 );
	fwrite(buffer4, sizeof(buffer4),1,archivo);  
	 printf("(41-44) Size of data chunk: %u \n", header.data_size);
 // nn=160000;             fwrite(&nn,4,1,archivo);
 // calculate no.of samples
 long num_samples = (8 * header.data_size) / (header.channels * header.bits_per_sample);
 printf("Number of samples:%lu \n", num_samples);

 long size_of_each_sample = (header.channels * header.bits_per_sample) / 8;
 printf("Size of each sample:%ld bytes\n", size_of_each_sample);  
  
 unsigned char data_buffer[size_of_each_sample];
  
  int i;
  short data_in_channel;
  for (i=0; i<2*num_samples; i++)
	  {
	   if(i<num_samples){
	   	    fread(&data_in_channel, 2, 1, archivoEntrada);	
			//BigEndian conversion
			//printf("[%u,%u]\n", data_buffer[0],data_buffer[1]);
			//data_in_channel = data_buffer[0] |
							 //(data_buffer[1] << 8);
			printf("1--(%u)(%u)=>%hi\n", data_buffer[0], data_buffer[1],data_in_channel);
			//sample = sample >> 1;
			//sample = data_in_channel/2;
			//Littlendian conversion again
			//	data_buffer[1] = sample >> 8;
			
			/*Wc = 2*pi*Fc/Fs=2*pi*2000/44100
				h(n)={Wc/pi, n=0
					 {sin(Wc*n)/n*pi, n!=0
				y(n) = h(n)(*)x(n)*/
			
			data_in_channel = data_in_channel/2;
			printf("1--(%u)(%u)=>%hi\n", data_buffer[0], data_buffer[1],data_in_channel);
			//data_buffer[1] = data_buffer[1] >>1;
			//data_buffer[0] = data_buffer[0] >1;
			
			//printf("-2-(%u)(%u)==>%d\n", data_buffer[0], data_buffer[1], data_buffer);
			fwrite(&data_in_channel, 2,1,archivo);   
		}else
		{
			fread(data_buffer, sizeof(data_buffer), 1, archivoEntrada);	
			fwrite(data_buffer, sizeof(data_buffer),1,archivo);  
		}
  }
  fclose(archivo);
  fclose(archivoEntrada);
 }
 else
  printf("No se pudo generar el archivo\n");
}
