#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
int main(){
	int i
	FILE* fp = fopen("a.txt","r");
	assert(fp!=NULL);
	getc(fp);
	if(feof(fp))
	printf("end\n");
	else
	printf("noend\n");
	fclose(fp);
	return 0;
}
