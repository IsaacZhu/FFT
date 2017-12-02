//FFT 
//zjr 11.26

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "timecount.h"

typedef struct complex{	//real+im*i
	double real;
	double im;
}complex;


double A[64],B[64],C[128];
complex ya[128],yb[128],yc[128];  
complex aa[128],ab[128],ac[128];

void complexadd(complex *result,complex a,complex b){
	(*result).real=a.real+b.real;
	(*result).im=a.im+b.im;
}

void complexsub(complex *result,complex a,complex b){
	(*result).real=a.real-b.real;
	(*result).im=a.im-b.im;
}

void complexmul(complex *result,complex a,complex b){
	(*result).real=a.real*b.real-a.im*b.im;
	(*result).im=a.real*b.im+b.real*a.im;
}

void FFT(int N,complex a[],complex y[]){
	if (N==1) {
		y[0].real=a[0].real;
		y[0].im=a[0].im;
		return;
	}
	//initialize
	int i;
	complex w,wn,tmp;
	w.real=1;
	w.im=0;
	wn.real=cos(2*M_PI/N);
	wn.im=sin(2*M_PI/N);
	
	complex a0[N/2];
	complex a1[N/2];
	for (i=0;i<N/2;++i){
		memcpy(&a0[i],&a[2*i],sizeof(complex));
		memcpy(&a1[i],&a[2*i+1],sizeof(complex));
	}

	complex y0[N/2];
	complex y1[N/2];
	FFT(N/2,a0,y0);
	FFT(N/2,a1,y1);	
	
	for (i=0;i<=N/2-1;++i){
		complexmul(&tmp,w,y1[i]);		//tmp=w*y1[i]		
		complexadd(&y[i],y0[i],tmp);		//y[i]=y0[i]+w*y1[i]

		complexmul(&tmp,w,y1[i]);		//tmp=w*y1[i]
		complexsub(&y[i+N/2],y0[i],tmp);	//y[i+N/2]=y0[i]-w*y1[i];		

		complexmul(&w,w,wn);
	}
}

void Multiply(int N){
	int i;
	for (i=0;i<N;++i){
		complexmul(&yc[i],ya[i],yb[i]);
	}
}

void Reverse_FFT(int N,complex y[],complex a[]){
	if (N==1) {
		a[0].real=y[0].real;
		a[0].im=y[0].im;
		return;
	}
	//initialize
	int i;
	complex w,wn,tmp;
	w.real=1;
	w.im=0;
	wn.real=cos(-2*M_PI/N);
	wn.im=sin(-2*M_PI/N);
	complex y0[N/2];
	complex y1[N/2];
	for (i=0;i<N/2;++i){
		memcpy(&y0[i],&y[2*i],sizeof(complex));
		memcpy(&y1[i],&y[2*i+1],sizeof(complex));
	}
	complex a0[N/2];
	complex a1[N/2];
	Reverse_FFT(N/2,y0,a0);
	Reverse_FFT(N/2,y1,a1);		
	for (i=0;i<=N/2-1;++i){
		complexmul(&tmp,w,a1[i]);
		complexadd(&a[i],a0[i],tmp);
		complexmul(&tmp,w,a1[i]);
		complexsub(&a[i+N/2],a0[i],tmp);		
		complexmul(&w,w,wn);
	}
}

int main(){
	int i,j,N,places,x,flag;
	int scale[4]={4,16,32,60};
	FILE *fp;
	fp=fopen("input.txt","r+");
	for (i=0;i<64;++i){		//read A
		fscanf(fp,"%lf",&A[i]);
	}
	for (i=0;i<64;++i){		//read B
		fscanf(fp,"%lf",&B[i]);
	}	
	fclose(fp);
	

	fp=fopen("result.txt","w+");
	fprintf(fp,"CALCULATION RESULT:\n");
	printf("**************CALCULATION START...****************\n");
	for (i=0;i<4;++i){

		timestart();
		x=scale[i];
		places=0;
		flag=0;
		while(x>=2){
			if (!flag) flag=x&1;
			x=x>>1;
			places++;
		}
		if (flag){			//not 2'power
			N=1<<(places+1);	//(int)lg(scale[i])+1
			for (j=0;j<scale[i];++j){
				aa[j].real=A[j];
				aa[j].im=0;
				ab[j].real=B[j];
				ab[j].im=0;
			}
			for (j=scale[i];j<2*N;++j){	//let A[scale[i]...2N] be 0  ;B[scale[i]...2N] be 0
				aa[j].real=0;
				aa[j].im=0;
				ab[j].real=0;
				ab[j].im=0;
			}
			FFT(N,aa,ya);			//DFT(A)
			FFT(N,ab,yb);			//DFT(B)
			Multiply(2*N);			//DFT(A)*DFT(B)
			Reverse_FFT(2*N,yc,ac);		//C=REVERSE_DFT(DFT(A)*DFT(B))
			for (j=0;j<2*N;++j) C[j]=ac[j].real/(2*N);
		}
		else{
			N=scale[i];
			for (j=0;j<scale[i];++j){
				aa[j].real=A[j];
				aa[j].im=0;
				ab[j].real=B[j];
				ab[j].im=0;
			}
			for (j=scale[i];j<2*N;++j){	//let A[scale[i]...2N] be 0  ;B[scale[i]...2N] be 0
				aa[j].real=0;
				aa[j].im=0;
				ab[j].real=0;
				ab[j].im=0;
			}

			FFT(2*N,aa,ya);			//DFT(A)
			FFT(2*N,ab,yb);			//DFT(B)
			Multiply(2*N);			//DFT(A)*DFT(B)
			Reverse_FFT(2*N,yc,ac);		//C=REVERSE_DFT(DFT(A)*DFT(B))
		}
		for (j=0;j<2*N;++j) C[j]=ac[j].real/(2*N);
		timeend();
	
		//print result to file
		fprintf(fp,"N is %d;result's coef is:\n",N);
		for (j=0;j<2*N;++j){
			fprintf(fp,"%lf ",C[j]);
		}
		fprintf(fp,"\n");
		memset(C,0,128*sizeof(double));

		printf("\nN is %d:\n",scale[i]);
		outputtime();
	}
	printf("\n**************CALCULATION END**********************\n");
	fclose(fp);	
}
