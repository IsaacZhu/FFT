#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timecount.h"
double A[64],B[64],C[128];

int main(){
	int i,j,k;
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
	memset(C,0,128*sizeof(double));
	fp=fopen("result.txt","w+");
	
	//calculation start
	for (i=0;i<4;++i){
		timestart();
		for (j=0;j<scale[i];++j){
			for (k=0;k<scale[i];++k){
				C[j+k]+=A[j]*B[k];
			}
		}
		timeend();
		printf("\nN is %d:\n",scale[i]);
		outputtime();
		fprintf(fp,"N is %d;result's coef is:\n",scale[i]);
		for (j=0;j<2*scale[i];++j){
			fprintf(fp,"%lf ",C[j]);
		}
		fprintf(fp,"\n\n");
		memset(C,0,128*sizeof(double));
	}//i for
	//end
	fclose(fp);
}
