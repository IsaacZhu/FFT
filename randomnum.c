#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
	int i;
	FILE *fp;
	fp=fopen("input.txt","a+");
	srand(time(0));
	printf("Making some random numbers...\n");
	for (i=1;i<=128;++i){
		fprintf(fp,"%d",(int)rand()%65535+1);
		fprintf(fp,"\n");		
	}
	printf("Success!\n");
	fclose(fp);
}
