//FFT 
//zjr 11.26

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timecount.h"

typedef struct complex{	//val*exp{2*pi*i*power/n}
	double val;
	int power;
}complex;

double A[64],B[64],C[128];
complex ya[128],yb[128],yc[128];  
complex aa[128],ab[128];


complex complexadd(complex a,complex b){
	int i;
	complex result;
	for (i=0;i<64;++i){
		//a.val[i]*b.val[i]
	}
	return result;
}

complex complexmul(complex a,complex b){
	complex result;
	return result;
}

void FFT(int N,complex a[],complex y[]){
	if (N==1) return;
	//initialize
	int i;
	complex w;
	w.val=1;
	w.power=0;
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
	for (i=0;i<N/2-1;++i){
		y[k].val=y0[k].val+w.val*y1[k].val;
		y[k].power=y0[k].power
		
		w.power+=2;
		if (w.power==N){
			w.val*=-1;
			w.power=0;
		}
	}
}

void Multiply(int N){
	int i;
	for (i=0;i<N;++i){
		yc[i]=complexmul(ya[i],yb[i]);
	}
}

void Reverse_FFT(int N,complex y[],complex a[]){
	if (N==1) return;
	//initialize
	int i;
	complex w;
	w.val=1;
	w.power=0;
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
	for (i=0;i<N/2-1;++i){
		a[k].val=a0[k].val+w.val*a1[k].val;
		a[k].power=a0[k].power
		
		w.power+=2;
		if (w.power==N){
			w.val*=-1;
			w.power=0;
		}
	}
}

int main(){
	int i,j,N,places,x,flag;
	complex ya[64],yb[64];
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
	fprintf("CALCULATION RESULT:\n");
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
				aa[j].val=A[j];
				aa[j].power=0;
				ab[j].val=B[j];
				ab[j].power=0;
			}
			for (j=scale[i];j<2N;++j){	//let A[scale[i]...2N] be 0  ;B[scale[i]...2N] be 0
				aa[j].val=0;
				aa[j].power=0;
				ab[j].val=0;
				ab[j].power=0;
			}
			FFT(N,aa,ya);			//DFT(A)
			FFT(N,ab,yb);			//DFT(B)
			Multiply(2*N);			//DFT(A)*DFT(B)
			Reverse_FFT(2*N,yc);		//C=REVERSE_DFT(DFT(A)*DFT(B))
		}
		else{
			for (j=0;j<scale[i];++j){
				ya[j].val=A[j];
				ya[j].power=0;
				yb[j].val=B[j];
				yb[j].power=0;
			}
			for (j=scale[i];j<2*N;++j){	//let A[scale[i]...2N] be 0  ;B[scale[i]...2N] be 0
				ya[j].val=0;
				ya[j].power=0;
				yb[j].val=0;
				yb[j].power=0;
			}
			N=scale[i];
			FFT(2*N,ya);
			FFT(2*N,yb);
			Multiply(2*N);
			Reverse_FFT(2*N,yc);
		}
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
