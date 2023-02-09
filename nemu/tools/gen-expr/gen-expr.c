#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
// this should be enough
static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s;"
"  printf(\"%%u\", result); "
"  return 0; "
"}";

int gen_expr_num=0;

uint32_t choose(uint32_t n){
	return rand()%n;
}

static void gen_num(){
	char tmp[2]={};
	int end=0;
	switch(choose(10)){
	case 0:
		 end=strlen(buf)-1;
		while(buf[end]=='(')
			end--;
		if(buf[end]=='/')
		*tmp='1';
		else
		*tmp='0';
	       break;
	case 1:*tmp='1';break;
	case 2:*tmp='2';break;
	case 3:*tmp='3';break;
	case 4:*tmp='4';break;
	case 5:*tmp='5';break;
	case 6:*tmp='6';break;
 	case 7:*tmp='7';break;
	case 8:*tmp='8';break;
	default:*tmp='9'; break;
}
	strcat(buf,tmp);
}
static void gen(char *p){
	strcat(buf, p);
}
static void gen_rand_op(){
	char tmp[2]={};
	switch(choose(4)){
	case 0:*tmp='+';break;
	case 1:*tmp='-';break;
	case 2:*tmp='*';break;
	default:*tmp='/';break;
	}
	strcat(buf,tmp);
}
static void gen_notype(bool n){
	if(n==false)return ;
	strcat(buf,"   ");

}
static void gen_rand_expr() {
	gen_notype(choose(1));
	int s=choose(3);
	gen_expr_num++;
	if(gen_expr_num>1000)
		s=0;
	switch (s) {
    case 0: gen_num(); break;
    case 1: gen("("); gen_rand_expr(); gen(")"); break;
    default: gen_rand_expr(); gen_rand_op(); gen_rand_expr(); break;
  }
	gen_notype(choose(1));
}

int main(int argc, char *argv[]) {
	int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
	int i;
  for (i = 0; i < loop; i ++) {
        buf[0]='\0';
	gen_expr_num=0;	
      	gen_rand_expr();
	
    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr 2> /tmp/judge.txt");
    //if (ret != 0) continue;
	printf("%d\n",ret);
    fp =fopen("/tmp/judge.txt","r");
	assert(fp!=NULL);
	if(fgetc(fp)!=EOF){
		printf("loop%d\n",i+1);
		fclose(fp);
		continue;}
	fclose(fp);
    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    int num=fscanf(fp, "%d", &result);
	pclose(fp);
	num++;
	if(result<0||(uint32_t)result==(uint32_t)(~0))//保证无符号和unsiged int 运算
	continue;
	fp=fopen("input.txt","a");
	assert(fp!=NULL);
    fprintf(fp,"%u %s\n", result, buf);
    fclose(fp);
  }
  return 0;
}
