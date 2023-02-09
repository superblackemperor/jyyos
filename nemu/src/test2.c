#include <stdio.h>

int main ()
{
   FILE *fp;
   int c;
  
   fp = fopen("a.txt","r");
   if(fp == NULL) 
   {
      perror("打开文件时发生错误");
      return(-1);
   }
   while(!feof(fp))
   {
	 char t=fgetc(fp);
   printf("%c",t);
   if(t=='\n')printf("+\n");
   if(t==EOF)printf("-\n");
   if(fgetc(fp)==EOF)break;
   fseek(fp,-1,SEEK_CUR);
   }
   fclose(fp);
   return(0);
}
