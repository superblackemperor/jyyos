#include <stdarg.h>
#include <stdint.h>
//#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "mystring.h"
#include "mystdio.h"
int main(){	
	char buf[128]={'\0'};
	mysprintf(buf, "%s", "Hello world!\n");
	printf("%s\n",buf); 
	printf("%d\n",strcmp(buf, "Hello world!\n") == 0);	

	mysprintf(buf, "%d + %d = %d\n", 1, 1, 2); 
	printf("%s\n",buf);
	printf("%d\n",strcmp(buf, "1 + 1 = 2\n") == 0);
}
