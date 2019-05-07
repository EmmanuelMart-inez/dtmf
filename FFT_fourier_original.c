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
   

#include <math.h>
#define SWAP(a,b) tempr=(a); (a)=(b); (b)=tempr

void pause()
{
  int k;
  printf("paused, enter 1 to continue...\n");
  scanf("%d",&k);
}

void bitreversal(x,m)   /*  bit reverses a given vector x[] */
float x[];
int m;
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

void fft(xr,xi,m,inverse)
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

void dft(xr,xi,m,inverse)
     float xr[], xi[];
     int m,inverse;
{
  int i,j,k,N;
  float w,yr[8],yi[8],wr[8][8],wi[8][8];

  N=pow(2.0,(float)m);

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
    if (inverse) 
      { xr[i]/=N; xi[i]/=N; }
    //    printf("(%.2f %.2f) ",xr[i],xi[i]);
  }

}

main()
{
  int i,j,k, m=6, n, flg;
  float w,xr[64],xi[64];

  n=pow(2.0,(float)m);
  printf("n=%d\n",n);
  for (i=0; i<n; i++) { xr[i]=1; xi[i]=0;}
  //for (i=2; i<5; i++) xr[i]=i; 

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

  if (flg) fft(xr,xi,m,1);   // inverse fft  
  else     dft(xr,xi,m,1);   // inverse dft

  printf("\n back to time domain:\n");
  for (i=0; i<n; i++) {
    printf("(%4.1f %4.1f) ",xr[i], xi[i]);
  }
  printf("\n");
}    