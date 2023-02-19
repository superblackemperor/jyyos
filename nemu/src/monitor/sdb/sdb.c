#include <isa.h>
#include <cpu/cpu.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "sdb.h"
#include <stdlib.h>
#include "memory/paddr.h"
#include <malloc.h>
static int is_batch_mode = false;

void init_regex();
void init_wp_pool();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}


static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args);//single step

static int cmd_info(char *args);

static int cmd_x(char *args);

static struct {
  const char *name;
  const char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si", "step a single command with \"si N\"(if there is no N,N=1)",cmd_si },
  { "info","print infomation of register(r) or watchpoint(w) with \"info ...\"\nno argument:print all register and watchpoint\nargument is r:print all register\nregument is w:print all watchpoint",cmd_info},
  { "x","scanf 4*N(32*N bit) from EXPR in memery (x N EXPR)",cmd_x}
  /* TODO: Add more commands */

};

#define NR_CMD ARRLEN(cmd_table)

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

static int cmd_si(char *args){
	//judge if exit argument
	char *arg = strtok(NULL," ");
	uint64_t step_num=1;
	if(arg!=NULL){
	char *end;
	double tmp=strtod(arg,&end);
	if(tmp-(int)tmp!=0){
	printf("the type of argument must be int\n");
	return 0;
	}else if(tmp==0){
		printf("thr argument can not be 0\n");
		return 0;
		}else
		step_num =(int)tmp;	
	}
	printf("step %ld\n",step_num);
	//cpu exe step_numn
	cpu_exec_step(step_num);
	return 0;


}

static int cmd_info(char *args){
	char*arg=strtok(NULL," ");
	//no argument:print all register and watchpoint 
	if(arg==NULL)
	isa_reg_display();
	//argument is r:print all register
	else if(strcmp(arg,"r")==0)
	isa_reg_display();
	else printf("your argument is wrong\n");
	//regument is w:print all watchpoint
	
	return 0;
}

static int cmd_x(char *args){
	char *arg[2];
	arg[0]=strtok(NULL," ");
	if(arg[0]==NULL){
	printf("please offer argument \"N\"\n");
	return 0;
	}
	char *end=NULL;
	double tmpN = strtod(arg[0],&end);
	if(tmpN==0){printf("N can not be 0\n");return 0;}
	 if((tmpN-(int)tmpN)!=0){printf("EXPR can not be X.X\n");return 0;}
	if(strcmp(end,"")==1){printf("please \"N\"\n");return 0;}
	 int N=(int)tmpN;
	arg[1]=strtok(NULL," ");
	 long int EXPR;
	if(arg[1]==NULL){
	//printf("please off argument \"EXPR\"\n");//可以缺省，表示当前指向address
	int step=cpu.pc-2147483648;
	EXPR=0x80000000+step;
	}else{                        
	 end=NULL;
	EXPR=strtol(arg[1],&end,16);
	if(strcmp(end,"")==1){printf("please \"EXPR 后不能有非法字符\"\n");return 0;}	
	}
	int n=0;
	for(;n<N;n++){
	 int i=3;
	 printf("memery 0X%lx~0X%lx : ",EXPR,EXPR+3);
	 for(;i>=0;i--){
	paddr_t pa=*(guest_to_host(EXPR+i));
	if(pa<16)//个位数	
	printf("0%x",pa);
	else
	printf("%x",pa);
	 }
	 EXPR+=4;
	 printf("\n");
 	}	 
	return 0;
	
	//
}
void sdb_set_batch_mode() {
  is_batch_mode = true;
}
void test_expr(){
int right_num=0,all_num=0;
  FILE *fp=fopen("/home/swxgg/ics2021/nemu/src/input.txt","r");
        assert(fp!=NULL);
        char*ans=(char*)malloc(10);
        char*exp=(char*)malloc(320);
        char tmp;
        char t[2]={'\0'};
        bool c;
        while(1){
        c=false;
        memset(ans,0,10);
        memset(exp,0,320);
        while((tmp=fgetc(fp))!=' '){
                t[0]=tmp;
                if(strlen(ans)<9)
                strcat(ans,t);
                else c=true;
        }
        while((tmp=fgetc(fp))!='\n'){
                t[0]=tmp;
                if(strlen(exp)<319)
                strcat(exp,t);
                else c=true;
        }
        if(fgetc(fp)==EOF)break;
        fseek(fp,-1,SEEK_CUR);
        if(c==true)continue;
        printf("%s %s\n",ans,exp);
        uint32_t answer=atoi(ans);
	bool flag=true;
       all_num++;
        uint32_t ret=expr(exp,&flag);
        if(flag==false)
        printf("false\n");
        //else //printf("ret=%d\n",ret);
        if(ret==answer)
                right_num++;
	else {
	printf("ret:%d answer:%d\n",ret,answer);/*getchar();*/}
        }
        printf("(right_num/all_num):%d/%d\njust:%f%%\n",
        right_num,all_num,(float)right_num/(float)all_num*100);
        fclose(fp);
        free(ans);
        free(exp);
}
void sdb_mainloop() {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }
	test_expr();//测试表达式结果
	
  for (char *str; (str = rl_gets()) != NULL; ) {
	  char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef CONFIG_DEVICE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}

void init_sdb() {
  /* Compile the regular expressions. */
  init_regex();

  /* Initialize the watchpoint pool. */
  init_wp_pool();
}
