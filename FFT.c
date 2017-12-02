//FFT 
//zjr 11.26

#include <stdio.h>
#include <stdlib.h>
#include "timecount.h"

double A[64],B[64];

void FFT(int N){
}

void Multiply(){
}

void Reverse_FFT(int N){
}

int main(){
	int i;
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
	
	printf("**************CALCULATION START...****************\n");
	for (i=0;i<4;++i){

		timestart();
		FFT(scale[i]);
		Multiply();
		Reverse_FFT(scale[i]);
		timeend();

		printf("\nN is %d:\n",scale[i]);
		outputtime();
	}
	printf("\n**************CALCULATION END**********************\n");	
}
