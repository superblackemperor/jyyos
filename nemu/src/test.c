#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
int main ()
{
   FILE *fp;
  
   fp = fopen("a.txt","r");
   if(fp == NULL) 
   {
      perror("打开文件时发生错误");
      return(-1);
   }
	
	char*ans=(char*)malloc(10);
        char*exp=(char*)malloc(400);
	char tmp;  
	char t[2]={'\0'};
	while(!feof(fp)){
	memset(ans,0,10);
        memset(exp,0,400);
       // printf("%s %s\n",ans,exp);
	while((tmp=getc(fp))!=' '){
              	t[0]=tmp;  
	//strcat(ans,t);

        }
	tmp=fgetc(fp);
        while(tmp!='\n'&&tmp!=EOF){
                t[0]=tmp;
	//	strcat(exp,t);
        	tmp=fgetc(fp);
	}
	if(tmp==EOF)break;
        printf("%s %s\n",ans,exp); 
       	}
	printf("end\n");
   free(ans);
   fclose(fp);
   free(exp);
   return 0;
}
