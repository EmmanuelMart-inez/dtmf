#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "wave.h"

#define Pi 3.141592653589

#define SWAP(a,b) tempr=(a); (a)=(b); (b)=tempr

#define M 512

#define FLOAT_TO_INT(x) ((x)>=0?(int)((x)+0.5):(int)((x)-0.5)) //Redondeo


void formatowave (FILE *archivo, wave *w);
void imprimirFormato (wave *wav);
void escribirArchivo(FILE *archivoEscritura, wave *header);
void Multiplicacion (FILE *archivo, wave *in, wave *out);
void Trasformadafourier(FILE *archivo, wave *s);
void CopyShort2Float(float *sample, short *b, int tam);
void Set32768to1(float *sample, int tam);
void Set1to32768(float *sample, int tam);
int RellenaConCeros(float *sample, int tam);
int SetComplexNumber(float *re, float *im, short *new, int tam); //Return new size of tam
/*----------------------------------*/
void CrearWaveFormatMono2Stereo(short *data, wave *old, wave *header, int num_channels, long num_samples, int bits_per_sample, long sample_rate);
void BigEndianLong2LittleEndianChar(unsigned char *a, unsigned long in, int tam);
/*--------------------------------------------*/
void Escalamiento(double *w, wave *entrada);
/*-------------------------------------------*/
void dft(float xr[], float xi[], int m, int inverse);
void bitreversal(float x[], int m);
void fft(float xr[], float xi[], int m, int inverse);

void main(void)
{ int i,n,m, num_samples_rellenado;
  /*Inicialización, valores iniciales para prueba [m,N,f1,f2,xr,xi]
  N=pow(2.0,(float)m);
  N=32;
  printf("N=%d\n",N);
  f1=3; f2=5;
  for (n=0; n<22; n++) { 
    xr[n]=cos(2*Pi*f1*n/N)+cos(2*Pi*f2*n/N); 
    xi[n]=0;
  }
  //  for (n=2; n<5; n++) xr[n]=n; 
  for (n=22; n<N; n++) xr[n]=0;
  */

  /*Prepocesamiento para integración del formato wav*/
  FILE *archivoEntrada1=NULL, *archivoEscritura=NULL;
	wave wav1, wav2;
	wave *conv;	 
	archivoEntrada1=fopen("sen32.wav","rb");
	archivoEscritura=fopen("salida.wav","wb");
	//archivoEntrada1=fopen(argv[1],"rb");
	//archivoEscritura=fopen(argv[2],"wb");	

	if (archivoEntrada1!=NULL && archivoEscritura!=NULL) 
 	{
		formatowave(archivoEntrada1, &wav1);	
		//imprimirFormato(&wav1)

		//Escalamiento y Separacion compleja
		float *xr = (float *)calloc(wav1.num_samples,sizeof(float));	
	 	float *xi = (float *)calloc(wav1.num_samples,sizeof(float));	
		CopyShort2Float(xr, wav1.muestras, wav1.num_samples);
		Set32768to1(xr, wav1.num_samples);
		//cambiar al usar wav2
		printf("\nN:%ld\n", wav1.num_samples);
		int nbase2 = pow(2,ceil(log2(wav1.num_samples)));
		printf("\nNbase2:%d\n", nbase2);
		xr = (float *)realloc(xr, nbase2*sizeof(float));
		xi = (float *)realloc(xi, nbase2*sizeof(float));
		num_samples_rellenado = RellenaConCeros(xr, wav1.num_samples);
		num_samples_rellenado = RellenaConCeros(xi, wav1.num_samples);
		
		//Inicialización de variables
		n = num_samples_rellenado; 
		m = (int)log2(n);
		printf("\n m:%d\n", m);
		//escribirArchivo(archivoEscritura, &wav1);
		//imprimirFormato(&wav1);	
		/*for(i=0; i<wav1.num_samples; i++){
			printf("%lf\n", xr[i]);
		}
*/
  /*Behavior*/

  int j,k, flg;
  float w;

 // n=pow(2.0,(float)m);
  printf("n=%d\n",n);

  printf("\n time domain:\n");
  for (i=0; i<n; i++) {
    printf("(%4.1f %4.1f) ",xr[i], xi[i]);
  }
  printf("\n");

  printf("\nDFT (0) or FFT (1)? ");  scanf("%d",&flg);

  if (flg) fft(xr,xi,m,0);  // forward fft  
  else     dft(xr,xi,m,0);  // forward dft

  printf("\n frequency domain:\n");
  for (i=0; i<n; i++) {
    printf("(%4.1f %4.1f) ",xr[i], xi[i]);
  }
  printf("\n");

  // This is where you can insert your 1D filtering codes. */

  short *data = (short *)malloc(2*num_samples_rellenado*sizeof(short));
  
  SetComplexNumber(xr, xi, data, num_samples_rellenado);
  printf("\n\n");
  for (i=0; i<n; i+=2) {
    printf("{%d}(%d %d) ", i,data[i], data[i+1]);
  }
  printf("\n");
  CrearWaveFormatMono2Stereo(data, &wav1, &wav2, 2, num_samples_rellenado, wav1.bits_per_sample, wav1.sample_rate);
  escribirArchivo(archivoEscritura, &wav2);
  //
  /*f (flg) fft(xr,xi,m,1);   // inverse fft  
  else     dft(xr,xi,m,1);   // inverse dft

  printf("\n back to time domain:\n");
  for (i=0; i<n; i++) {
    printf("(%4.1f %4.1f) ",xr[i], xi[i]);
  }*/
  printf("\n");


		fclose(archivoEntrada1);
		fclose(archivoEscritura);
		free(xr);
		free(xi);
		free(data);
	}else
 	printf("No se pudo generar el archivo\n");
	

}    

void bitreversal(float x[], int m)   /*  bit reverses a given vector x[] */
{ int i,j,k, N=pow(2.0,(float)m);
  float tempr;
  for (i=0; i<N; ++i) {       /* bit reversal */
    j=0;
    for (k=0; k<m; ++k)
      j=(j << 1) | (1 & (i >> k));
    if (j < i) {
      SWAP(x[i],x[j]);
    }
  }
}

void fft(float xr[], float xi[], int m, int inverse)
{ int mmax,step,i,j,k,j1,n,nu,N;
  float arg,s,c,tempr,tempi;

  N=pow(2.0,(float)m);
  if (inverse) printf("IFFT of size %d \n",N);
  else printf("FFT of size %d \n",N);
/*
  printf("\n before reversal...\n");
  for (i=0; i<N; i++)  printf("%3.0f ",xr[i]);
  printf("\n");
*/

  bitreversal(xr,m); bitreversal(xi,m);   /* bit reversal of input data */

/*
  printf("\n after reversal...\n");
  for (i=0; i<N; i++)  printf("%3.0f ",xr[i]);
  printf("\n");
  pause();
*/

  for (i=0; i<m; i++) {         /* for m=log N stages */
    n=pow(2.0,(float)i);                 /* length of a section */
    k=0;
    while (k<N-1) {             /* for N components in each stage */
      for (j=0; j<n; j++) {     /* for each section */
	arg=-j*6.28318530717959/n/2;
	if (inverse) arg=-arg;
	c=cos(arg);
	s=sin(arg);
	j1=k+j;
	tempr=xr[j1+n]*c-xi[j1+n]*s;
	tempi=xi[j1+n]*c+xr[j1+n]*s;
	xr[j1+n]=xr[j1]-tempr;
	xi[j1+n]=xi[j1]-tempi;
	xr[j1]=xr[j1]+tempr;
	xi[j1]=xi[j1]+tempi;
      }
      k+=2*n;
    }
  }
  if (inverse) {        /* only for inverse transform */
    for (i=0; i<N; i++) {
      xr[i]=xr[i]/N;
      xi[i]=xi[i]/N;
    }
  }
}

void dft(float xr[], float xi[], int m, int inverse)
{
  int i,j,k,N,len;
  float w;
  N=pow(2.0,(float)m);
  float *yr = (float *)calloc(N,sizeof(float)); 
  float *yi = (float *)calloc(N,sizeof(float)); 
  float **wr = (float **)malloc(N * sizeof(float *)); 
    for (i=0; i<N; i++) 
         wr[i] = (float *)malloc(N * sizeof(int));
  float **wi = (float **)malloc(N * sizeof(float *)); 
    for (i=0; i<N; i++) 
         wi[i] = (float *)malloc(N * sizeof(int));

  for (i=0; i<N; i++)
    { yr[i]=xr[i], yi[i]=xi[i]; }

  printf("\nThe transform matrix:\n");
  for (i=0; i<N; i++) {
    for (j=0; j<N; j++) {
      w=6.28318530717959*i*j/N;
      wr[i][j]=cos(w);
      wi[i][j]=-sin(w);
      if (inverse) wi[i][j]=-wi[i][j];
      printf("(%5.2f %5.2f) ",wr[i][j],wi[i][j]);
    }
    printf("\n");
  }

  for (i=0; i<N; i++) {
    xr[i]=xi[i]=0;
    for (j=0; j<N; j++) {
      xr[i]+=wr[i][j]*yr[j]-wi[i][j]*yi[j];
      xi[i]+=wr[i][j]*yi[j]+wi[i][j]*yr[j];
    }
    if (!inverse) 
      { xr[i]/=N; xi[i]/=N; }
    //    printf("(%.2f %.2f) ",xr[i],xi[i]);
  }
  free(yr); free(yi);
}


void Trasformadafourier(FILE *archivo, wave *s){
	int k,n,N,i;
	N = s->num_samples;
	double *real = malloc(sizeof(double)*s->num_samples);
	double *imag = malloc(sizeof(double)*s->num_samples);
	short *data = malloc(sizeof(short)*(2*s->num_samples));
 	i=0;
	for(k=0; k < s->num_samples; k++){
		real[k]=0;
		imag[k]=0;
		for(n=0; n < s->num_samples; n++){
			real[k] += s->muestras[n]*cos(2*Pi*k*n/N);
			imag[k] += s->muestras[n]*sin(2*Pi*k*n/N);
			//printf("k[%d]n[%d]Cos:%lf\n",k, n, s->muestras[n]*cos(2*PI*k*n/N));
			//printf("Sen:%lf\n", sin(2*PI*k*n/N));
		}
		//real[k] = real[k]/N;
		//imag[k] = -imag[k]/N;
		printf("real[%lf]imaginaria[%lf]\n", real[k], imag[k]);
		data[i] = real[k]/N;
		i++;
		data[i] = -imag[k]/N;
		i++;
	}
	CrearWaveFormatMono2Stereo(data, s, s, 2, s->num_samples, s->bits_per_sample, s->sample_rate);  	
}

void escribirArchivo(FILE *archivoEscritura, wave *header){
	fwrite(header->riff, sizeof(header->riff),1,archivoEscritura);
	fwrite(header->overall_sizeb, sizeof(header->overall_sizeb),1,archivoEscritura);
	fwrite(header->wave, sizeof(header->wave),1,archivoEscritura);
	fwrite(header->fmt_chunk_marker, sizeof(header->fmt_chunk_marker),1,archivoEscritura);
	fwrite(header->length_of_fmtb, sizeof(header->length_of_fmtb),1,archivoEscritura);
	fwrite(header->format_typeb, sizeof(header->format_typeb),1,archivoEscritura);
	fwrite(header->channelsb, sizeof(header->channelsb),1,archivoEscritura);
	fwrite(header->sample_rateb, sizeof(header->sample_rateb),1,archivoEscritura);
	fwrite(header->byterateb, sizeof(header->byterateb),1,archivoEscritura);
	fwrite(header->block_alignb, sizeof(header->block_alignb),1,archivoEscritura);
	fwrite(header->bits_per_sampleb, sizeof(header->bits_per_sampleb),1,archivoEscritura);
	fwrite(header->data_chunk_header, sizeof(header->data_chunk_header),1,archivoEscritura);
	fwrite(header->data_sizeb, sizeof(header->data_sizeb),1,archivoEscritura);
	fwrite(header->muestras, sizeof(short)*2*header->num_samples,1,archivoEscritura);
	//fwrite(header->muestras, sizeof(header->muestras),1,archivoEscritura);
	fwrite(header->end, sizeof(header->end),1,archivoEscritura);
}

void imprimirFormato (wave *wav){
	int i;
	printf("(%c)(%c)(%c)(%c):\n", wav->riff[0], wav->riff[1], wav->riff[2], wav->riff[3]);
	printf("(%lu):\n", wav->overall_size);
	printf("(%c)(%c)(%c)(%c):\n", wav->wave[0], wav->wave[1], wav->wave[2], wav->wave[3]);
	printf("(%c)(%c)(%c):\n", wav->fmt_chunk_marker[0], wav->fmt_chunk_marker[1], wav->fmt_chunk_marker[2]);
	printf("length_of_fmt(%lu):\n", wav->length_of_fmt);
	printf("format_type(%u):\n", wav->format_type);
	printf("channels(%u):\n", wav->channels);
	printf("sample rate:(%u):\n", wav->sample_rate);
	printf("byterate(%u):\n", wav->byterate);
	printf("block_align(%u):\n", wav->block_align);
	printf("bits_per_sample(%u):\n", wav->bits_per_sample);
	printf("(%c)(%c)(%c)(%c):\n", wav->data_chunk_header[0], wav->data_chunk_header[1], wav->data_chunk_header[2], wav->data_chunk_header[3]);
	printf("data_size(%ld bytes):\n", wav->data_size);
	printf("num_samples(%lu):\n", wav->num_samples);
	printf("size(%ld):\n", wav->size_of_each_sample);
	for(i=0; i<wav->num_samples; i++){
		printf("(%d:%04x)\n",i, wav->muestras[i]);
		}
	for(i=0; i<74; i++){
		//printf("|%d:%02x", i, wav->end[i]);
	}//printf("|\n");
}

void formatowave (FILE *archivoEntrada, wave *header){
	unsigned char buffer4[4];
 	unsigned char buffer2[2];
 	int i;
	if (archivoEntrada!=NULL) 
 	{
 		fread(header->riff, sizeof(header->riff), 1, archivoEntrada);
 		fread(header->overall_sizeb, sizeof(header->overall_sizeb), 1, archivoEntrada);
 		 header->overall_size  = header->overall_sizeb[0] | 
						(header->overall_sizeb[1]<<8) | 
						(header->overall_sizeb[2]<<16) | 
						(header->overall_sizeb[3]<<24);
 		fread(header->wave, sizeof(header->wave), 1, archivoEntrada);
 		fread(header->fmt_chunk_marker, sizeof(header->fmt_chunk_marker), 1, archivoEntrada);
 		fread(header->length_of_fmtb, sizeof(header->length_of_fmtb), 1, archivoEntrada);
 		 header->length_of_fmt = header->length_of_fmtb[0] |
							(header->length_of_fmtb[1] << 8) |
							(header->length_of_fmtb[2] << 16) |
							(header->length_of_fmtb[3] << 24);
 		fread(header->format_typeb, sizeof(header->format_typeb), 1, archivoEntrada);
 		 header->format_type = header->format_typeb[0] | (header->format_typeb[1] << 8);
 		fread(header->channelsb, sizeof(header->channelsb), 1, archivoEntrada);
 		 header->channels = header->channelsb[0] | (header->channelsb[1] << 8);
 		fread(header->sample_rateb, sizeof(header->sample_rateb), 1, archivoEntrada);
 		 header->sample_rate = header->sample_rateb[0] |
						(header->sample_rateb[1] << 8) |
						(header->sample_rateb[2] << 16) |
						(header->sample_rateb[3] << 24);
 		fread(header->byterateb, sizeof(header->byterateb), 1, archivoEntrada); 
 		 header->byterate  = header->byterateb[0] |
						(header->byterateb[1] << 8) |
						(header->byterateb[2] << 16) |
						(header->byterateb[3] << 24);
 		fread(header->block_alignb, sizeof(header->block_alignb), 1, archivoEntrada);
 		 header->block_align = header->block_alignb[0] |
					(header->block_alignb[1] << 8);
 		fread(header->bits_per_sampleb, sizeof(header->bits_per_sampleb), 1, archivoEntrada);
 		 header->bits_per_sample = header->bits_per_sampleb[0] |
					(header->bits_per_sampleb[1] << 8); 
 		fread(header->data_chunk_header, sizeof(header->data_chunk_header), 1, archivoEntrada);
 		fread(header->data_sizeb, sizeof(header->data_sizeb), 1, archivoEntrada);
		  header->data_size = header->data_sizeb[0] |
				(header->data_sizeb[1] << 8) |
				(header->data_sizeb[2] << 16) | 
				(header->data_sizeb[3] << 24 );
 		header->num_samples = (8 * header->data_size) / (header->channels * header->bits_per_sample);
 		header->size_of_each_sample = (header->channels * header->bits_per_sample) / 8;
 		header->muestras = malloc (sizeof(short)*header->num_samples);
 		for(i=0; i<header->num_samples; i++){
	 		fread(&header->muestras[i], sizeof(short), 1, archivoEntrada);
			//printf("(%x):\n", header->muestras[i]);
		}
		for(i=0; i<74; i++){
			fread(&header->end[i], sizeof(char), 1, archivoEntrada);
			//printf("%x\n", header->end[i]);
		}
	}else{
		printf("No se pudo abrir el archivo\n");
	}
}


void Multiplicacion (FILE *archivo, wave *in1, wave *in2){
	int tam,i;
	wave *mayor, *menor;

	if(in1->num_samples >= in2->num_samples){
		mayor = in1;
		menor = in2;
	}
	else{
		mayor = in2;
		menor = in1;
	}

	double *aux = malloc(sizeof(double)*mayor->num_samples);
	for(i=0; i < mayor->num_samples; i++){
		aux[i] = mayor->muestras[i] * menor->muestras[i];
	}

  	Escalamiento(aux, mayor);
  	escribirArchivo(archivo, mayor);
}


double BuscaMuestraMayor(double *w, wave *signa){
	int i;
	double max = 1;
	for(i=0; i < signa->num_samples; i++)
		if(fabs(w[i]) > max)	
			max = w[i];
	return max/32767;
}

void Escalamiento(double *w, wave *entrada){
	double max = BuscaMuestraMayor(w, entrada);
	printf("\n\n\n MAx: %lf\n\n\n", max);
   	int i;
   	//printf("Samples: %lu\n",entrada->num_samples);
   	for(i=0; i < entrada->num_samples; i++){
  		entrada->muestras[i] = w[i]/max;
  		//printf("%u\n", sample[i]);
  		//if(entrada->muestras[i] > 32767)
  		//	entrada->muestras[i] = 32767;
  		//else if(entrada->muestras[i] < -32767)
  		//	entrada->muestras[i] = -32767;
  		//printf("%04x\n", entrada->muestras[i]);
   	}
}


void CrearWaveFormatMono2Stereo(short *data, wave *old, wave *header, int num_channels, long num_samples, int bits_per_sample, long sample_rate){
 	header->riff[0] = old->riff[0];
 	header->riff[1] = old->riff[1];
 	header->riff[2] = old->riff[2];
 	header->riff[3] = old->riff[3];
 	header->wave[0] = old->wave[0];
 	header->wave[1] = old->wave[1];
 	header->wave[2] = old->wave[2];
 	header->wave[3] = old->wave[3];
 	header->fmt_chunk_marker[0] = old->fmt_chunk_marker[0];
 	header->fmt_chunk_marker[1] = old->fmt_chunk_marker[1];
 	header->fmt_chunk_marker[2] = old->fmt_chunk_marker[2];
 	header->fmt_chunk_marker[3] = old->fmt_chunk_marker[3];
 	header->data_chunk_header[0] = old->data_chunk_header[0];
 	header->data_chunk_header[1] = old->data_chunk_header[1];
 	header->data_chunk_header[2] = old->data_chunk_header[2];
 	header->data_chunk_header[3] = old->data_chunk_header[3];

 	header->length_of_fmt = old->length_of_fmt;
 	//	BigEndianLong2LittleEndianChar(header->length_of_fmtb, header->length_of_fmt);
 	header->length_of_fmtb[0] = old->length_of_fmtb[0];
 	header->length_of_fmtb[1] = old->length_of_fmtb[1];
 	header->length_of_fmtb[2] = old->length_of_fmtb[2];
 	header->length_of_fmtb[3] = old->length_of_fmtb[3];
 	header->format_type = old->format_type;
 	BigEndianLong2LittleEndianChar(header->format_typeb, header->format_type, 2); 
 	header->bits_per_sample = old->bits_per_sample;
 	BigEndianLong2LittleEndianChar(header->bits_per_sampleb, header->bits_per_sample, 2);
 	//->bits_per_sampleb[0]=3;
 	//header->bits_per_sampleb[1]=4;
 	header->size_of_each_sample = 2*old->size_of_each_sample;
 	///
 	header->channels = 2;
 	BigEndianLong2LittleEndianChar(header->channelsb, header->channels, 2);
 	header->sample_rate = sample_rate;
	BigEndianLong2LittleEndianChar(header->sample_rateb, sample_rate, 4);
 	header->byterate = sample_rate * num_channels * bits_per_sample/8;
 	BigEndianLong2LittleEndianChar(header->byterateb, header->byterate, 4);
 	header->block_align = num_channels * bits_per_sample/8;
 	BigEndianLong2LittleEndianChar(header->block_alignb, header->block_align, 2);
 	header->data_size = num_samples * num_channels * bits_per_sample/8;
 	BigEndianLong2LittleEndianChar(header->data_sizeb, header->data_size, 4);
 	header->overall_size = 4+(8+16)+(8+header->data_size)+74; //74=endfile*.. 
 	printf("\n\n%ld\n", header->overall_size);
 	BigEndianLong2LittleEndianChar(header->overall_sizeb, header->overall_size, 4);
 	header->muestras = data;
 	//header->num_samples = sizeof(data)/sizeof(data[0]);
 	header->num_samples = num_samples; //si}?
 	int i; 
 	for (i=0; i<74; i++)
 		header->end[i] = old->end[i];
}

void BigEndianLong2LittleEndianChar(unsigned char *a, unsigned long in, int tam){
	if(tam == 2){
		//printf("%lX\n", in);
		a[1] = (char)(in>>8);
		a[0] = (char)(in); ///checar si se puede como abajo, en caso de ser valido
		//printf("\n\n%d %d\n", sizeof(a), sizeof(a[0])); 
	}else if(tam == 4){
		//printf("%lX\n", in);
		a[3] = ((in>>24));  // move byte 3 to byte 0
        a[2] = ((in<<8)>>24);// move byte 1 to byte 2
        a[1] = ((in<<16)>>24); // move byte 2 to byte 1
        a[0] = ((in<<24)>>24); // byte 0 to byte 3
		//printf("%x %x %x %x\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
	}else 

	printf("no concuerda el tamaño con el formato a convertir: %s\n", a);
}


//PASAR ENTRADA A LAS FUNCIONES ES SOLO PARA CONOCER NUM_SAMPLES

void CopyShort2Float(float *sample, short *b, int tam){
	int i;
	for (int i = 0; i < tam; ++i)
	{
		sample[i] = (float)b[i];
		//printf("%f\n", a[i]);
	}
}

void Set32768to1(float *sample, int tam){
	int i;
	for (int i = 0; i < tam; ++i)
	{
		sample[i] = sample[i]/32768;
		//printf("%f\n", a[i]);
	}
}

int RellenaConCeros(float *sample, int tam){
	int i, nbase2, n;
	//La función ceil devuelve el menor valor entero no inferior a x. 
	//Redondeo hacia arriba
	nbase2 = ceil(log2(tam));
	n = pow(2, nbase2);
	for (int i = 0; i < n; ++i)
	{
		if(i>=tam)
			sample[i]=0;
	}

	return n;
}


void Set1to32768(float *sample, int tam){
	int i;
	int aux;
	for (int i = 0; i < tam; ++i)
	{	
		aux = sample[i]*32768;
		if(abs(aux)<32768)
			sample[i] = sample[i]*32768;
		else
			if(sample[i]>32768)
				sample[i]=32768;
			else
				sample[i]=-32768;
		//printf("%f\n", a[i]);
	}
}

int SetComplexNumber(float *re, float *im, short *new, int tam){
	int i,j; 
	Set1to32768(re, tam);
	Set1to32768(im, tam);
	
	for (int i = 0, j = 0; i < tam; i++,j+=2)
	{
		new[j] = re[i];
		new[j+1] = im[i];
		printf("(%d %d) ",new[j], new[j+1]);
	}
}