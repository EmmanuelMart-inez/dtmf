/* This is a sample program for 1D fast Fourier transform (FFT). 
   Compile it with "cc myfft.c -lm -o myfft" 
   The code includes a function fft(xr,xi,m,inverse) which can do both
   forward (inverse=0) and inverse (inverse=1) Fourier transform. xr and xi
   are the real and imaginary parts of the complex data and m is the number
   of complex values in the 1D array.
   Run it to see the data in the frequency domain. Since the input data is 
   real, the spectrum is symmetric (even for real part, odd for imaginary
   art). The program then does the inverse transform to get back to time 
   domain. See that the data is recovered in time domain. 
   You can use the fft function in this program for your project. I wrote
   this fft function following closely what I showed you in class (and also
   the handout). Do not treat it as a black box. Read it to understand it.
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define Pi 3.141592653589
#define SWAP(a,b) tempr=(a); (a)=(b); (b)=tempr

#define M 512

void pause()
{
  int k;
  printf("paused, enter 1 to continue...\n");
  scanf("%d",&k);
}

void bitreversal(x,N)   /*  bit reverses a given vector x[] */
     float x[];
     int N;
{ int i,j,k, m=(int)log2f((float)N);
  float tempr;
  for (i=0; i<N; ++i) {       /* bit reversal */
    j=0;
    for (k=0; k<m; ++k)
      j=(j << 1) | (1 & (i >> k));
    if (j < i) 
      SWAP(x[i],x[j]);
  }
}

void fft(xr,xi,N,inverse)
     float *xr,*xi;
     int N,inverse;
{ int mmax,step,i,j,k,j1,n,nu,m;
  float arg,s,c,tempr,tempi,v;

  //  N=pow(2.0,(float)m);
  m=(int)log2f((float)N);
  printf("fft: m=%d\n",m);

  bitreversal(xr,N); bitreversal(xi,N);   /* bit reversal of input data */

  for (i=0; i<m; i++) {         /* for m=log N stages */
    n=pow(2.0,(float)i);                 /* length of a section */
    k=0;
    while (k<N-1) {             /* for N components in each stage */
      for (j=0; j<n; j++) {     /* for each section */
    	arg=-j*Pi/n;
    	if (inverse) arg=-arg;
    	c=cos(arg);
    	s=sin(arg);
    	j1=k+j;
    	tempr=xr[j1+n]*c-xi[j1+n]*s;
    	//tempi=xi[j1+n]*c+xr[j1+n]*s;
    	xr[j1+n]=xr[j1]-tempr;
    	//xi[j1+n]=xi[j1]-tempi;
    	xr[j1]=xr[j1]+tempr;
    	//xi[j1]=xi[j1]+tempi;
      }
      k+=2*n;
    }
  }
  if (inverse) 
    for (n=0; n<N; n++) { xr[n]/=N; xi[n]/=N; }
}

void fftold(xr,xi,m,inverse)
     float xr[],xi[];
     int m,inverse;
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
	arg=-j*Pi/n;
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
  if (inverse) 
    for (n=0; n<N; n++) { xr[n]/=N; xi[n]/=N; }
    //    printf("(%.2f %.2f) ",xr[i],xi[i]);
}

void dft(xr,xi,N,inverse)
     float xr[], xi[];
     int N,inverse;
{
  int i,j,k,n;
  float w,yr[M],yi[M],wr[M][M],wi[M][M];

  for (i=0; i<N; i++)
    { yr[i]=xr[i], yi[i]=xi[i]; }

  //  printf("\nThe transform matrix:\n");
  for (i=0; i<N; i++) {
    for (j=0; j<N; j++) {
      w=2*Pi*i*j/N;
      wr[i][j]=cos(w);
      wi[i][j]=-sin(w);
      if (inverse) wi[i][j]=-wi[i][j];
      //      printf("(%5.2f %5.2f) ",wr[i][j],wi[i][j]);
    }
    //    printf("\n");
  }
  for (i=0; i<N; i++) {
    xr[i]=xi[i]=0;
    for (j=0; j<N; j++) {
      xr[i]+=wr[i][j]*yr[j]-wi[i][j]*yi[j];
      xi[i]+=wr[i][j]*yi[j]+wi[i][j]*yr[j];
    }
    if (inverse) { xr[i]/=N; xi[i]/=N; }
    //    printf("(%.2f %.2f) ",xr[i],xi[i]);
  }
}


main()
{
  int i,j,k, m=3, n, N, f1, f2, flg;
  float w,xr[M],xi[M];

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

  printf("\n time domain:\n");
  for (i=0; i<N; i++) {
    printf("(%4.1f %4.1f) ",xr[i], xi[i]);
  }
  printf("\n");

  printf("\nDFT (0) or FFT (1)? ");  scanf("%d",&flg);

  if (flg) fft(xr,xi,N,0);  // forward fft  
  else     dft(xr,xi,N,0);  // forward dft

  printf("\n frequency domain:\n");
  for (i=0; i<N; i++) {
    printf("(%4.1f %4.1f) ",xr[i], xi[i]);
  }
  printf("\n");

  // This is where you can insert your 1D filtering codes. */

  if (flg) fft(xr,xi,N,1);   // inverse fft  
  else     dft(xr,xi,N,1);   // inverse dft

  printf("\n back to time domain:\n");
  for (i=0; i<N; i++) {
    printf("(%4.1f %4.1f) ",xr[i], xi[i]);
  }
  printf("\n");
}    