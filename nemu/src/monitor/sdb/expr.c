#include <isa.h>
#include <string.h>
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#include <macro.h>
#include <malloc.h>
#include <math.h>
void mystrcpy(char*dst,unsigned int len,char *in){
	for(int i=0;i<len;i++)
		dst[i]=in[i];
}
bool isDeRefer();
uint32_t eval(int p,int  q);
bool check_parentheses(int p,int q);
uint32_t mainop_posi(int p,int q);
void strHtoD(char*str);
bool strREGtoD(char*str);
enum {
  TK_NOTYPE = 256,TK_NUM_H,TK_NUM,TK_REG,TK_EQ,TK_NOEQ,TK_AND,TK_DE

  /* TODO: Add more token types */

};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */
	{"\\)",')'},
	{"\\(",'('},
    {"(0x(0|[1-9a-fA-F][0-9a-fA-F]*))",TK_NUM_H},
   {"(0|[1-9][0-9]*)",TK_NUM},
   {"\\$[$]{0,1}[a-zA-Z0-9]+",TK_REG},
  {"\\/",'/'},
  {"\\*",'*'},
  {"\\-",'-'},
  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"==", TK_EQ},        // equal
  {"!=",TK_NOEQ},
  {"&&",TK_AND}
};


#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[320] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;
  memset(tokens,0,sizeof(Token)*320);
  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        switch (rules[i].token_type) {
		case TK_NUM_H:
		case TK_REG:
		case TK_NUM:
			tokens[nr_token].type=rules[i].token_type;
			//寄存器和16进制数先转换为10进制数
			if(substr_len<32)
			{
			mystrcpy(tokens[nr_token].str,substr_len,substr_start);
			
			if(tokens[nr_token].type==TK_NUM_H){
			strHtoD(tokens[nr_token].str);//16进制数先转换为10进制数
			tokens[nr_token].type=TK_NUM;}
			
			if(tokens[nr_token].type==TK_REG){//寄存器
				if(strREGtoD(tokens[nr_token].str)==false)
				{printf("没有找到寄存器\n");return false;}
			tokens[nr_token].type=TK_NUM;}

			//无论是寄存器还是16进制数，都已经在这提前转换为10进制数了

			if(isDeRefer()){//判断一下前面是否有解引用,非解引用什么都不用做
			tokens[nr_token-1].type=TK_NUM;
			strcpy(tokens[nr_token-1].str,"1");//暂时不明白解引用要解什么出来，先形式主义一下用1代替	
			memset(&tokens[nr_token],0,sizeof(Token));
			nr_token--;//与*合并后抛弃本nr_token的tokens	
			}
			}else
			{printf("tokens[%d].str overflow\n",nr_token);
			return false;
			}nr_token++;
			break;
		case TK_NOTYPE:break;
		case '*'://判断是否为解引用
		if(nr_token==0||tokens[nr_token-1].type=='('||(tokens[nr_token-1].type>=42&&tokens[nr_token-1].type<=47))//偷懒了一下，+/-*的ascii码在42到47
		tokens[nr_token].type=TK_DE;
		else//非解引用
		tokens[nr_token].type='*';
		nr_token++;
		break;
		default: 
		tokens[nr_token++].type=rules[i].token_type;
        	break;
	}
        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}


word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
	//提前检测表达式的括号正确性
	int flag=0;
	for(int i=0;i<nr_token;i++){
		if(tokens[i].type=='(')
		flag++;
		 if(tokens[i].type==')')
		flag--;
		if(flag<0)
		assert(0);
	}if(flag!=0)assert(0);
  /* TODO: Insert codes to evaluate the expression. */
	return eval(0,nr_token-1);
}
uint32_t eval(int p,int  q) {
  if (p > q) {
    /* Bad expression */
	return -1;
  }
  else if (p == q) {
    /* Single token.
     * For now this token should be a number.
     * Return the value of the number.
     */char *end;
	double tmp=strtod(tokens[p].str,&end);
	return (uint)tmp;
  }
  else if (check_parentheses(p, q) == true) {
    /* The expression is surrounded by a matched pair of parentheses.
     * If that is the case, just throw away the parentheses.
     */
    return eval(p + 1, q - 1);
  }
  else {
    //the position of 主运算符 in the token expression;
    int op=mainop_posi(p,q);
	int val1 = eval(p, op - 1);
    int val2 = eval(op + 1, q);

    switch (tokens[op].type) {
      case '+': return val1 + val2;
      case '-': return val1-val2;
      case '*': return val1*val2;
      case '/': 
		if(val2==0)return (uint32_t)-1;
		return (uint32_t)(val1/val2);
      case TK_EQ:return val1==val2;
	case TK_NOEQ:return val1!=val2;
	case TK_AND:return val1&&val2;
      default: assert(0);
    }
  }
}
bool check_parentheses(int p,int q){
	if(tokens[p].type=='('&&tokens[q].type==')')
	{
	int flag=0;
        for(int i=p+1;i<q;i++){
                if(tokens[i].type=='(')
                flag++;
                 if(tokens[i].type==')')
                flag--;
                if(flag<0)
                return false;
	}return true;
	}else return false;
} 
uint32_t mainop_posi(int p,int q){
	int ret=p;
	int i=p;
	for(;i<=q;){
	if(tokens[i].type>=TK_EQ&&tokens[i].type<=TK_AND)
	{ret=i++;
	}
	if(tokens[i].type=='+'||tokens[i].type=='-')
	{
	if(tokens[ret].type<TK_EQ||tokens[ret].type>TK_AND)
	ret=i++;
	}
	if(tokens[i].type=='*'||tokens[i].type=='/')
	{
	if(tokens[ret].type!='+'&&tokens[ret].type!='-'&&(tokens[ret].type<TK_EQ||tokens[ret].type>TK_AND))
	ret=i++;
	}
	if(tokens[i].type=='('){//要找到括号对
	int flag=1;
	while(flag!=0)//策略是有多少左括号就有多少右括号
	{
	i++;
	if(tokens[i].type=='(')flag++;
	if(tokens[i].type==')')flag--;
	if(i>q)assert(0);
	}i++;
	}else i++;
  }
	return ret;
}

/*int pow(int di,int mi){
	if(mi==0)return 1;
	int ret=di;
	for(int i=1;i<mi;i++)
	ret*=di;
	return ret;
}*/
int strHtoint(char*str){
	int ret=0;
	int len=strlen(str);
	for(int i=len-1;i>=2;i--){
	if(str[i]>=48&&str[i]<=57)
	ret+=pow(16,(len-1-i))*(str[i]-48);//0~9
	if(str[i]>=65&&str[i]<=70)
	ret+=pow(16,(len-1-i))*(str[i]-55);//A~F
	if(str[i]>=97&&str[i]<=102)
	ret+=pow(16,(len-1-i))*(str[i]-87);//a~f
	}
	return ret;
}
void int2strD(char*str,uint32_t num){
	if(num==0)strcpy(str,"0");
	char*tmp=(char*)malloc(50);//字符stack
	memset(tmp,0,50);
	char cat[2]={'\0'};
	while(num!=0){
	uint32_t mod=num%10;
	cat[0]=(char)(mod+48);
	strcat(tmp,cat);
	num-=mod;
	num/=10;
	}
	//双指针字符串反转
	int len=strlen(tmp);
	for(int i=0,j=len-1;i<j;i++,j--)
	{
		char x=tmp[i];
		tmp[i]=tmp[j];
		tmp[j]=x;
	}	
	strcpy(str,tmp);
	free(tmp);
}
void strHtoD(char*str){
	int num=strHtoint(str);
	int2strD(str,num);
}
bool strREGtoD(char*str){
	extern const char *regs[];
	bool flag=false;
	//先判断是否为PC
	if(strcmp("PC",str+1)==0){
	int num=cpu.pc;
	int2strD(str,num);
	return true;
	}
	int i=0;
	for(;i<32;i++)
	if(0==strcmp(regs[i],str+1))
	{flag=true;break;}
	if(flag==false)return flag;
	int num=cpu.gpr[i]._32;
	int2strD(str,num);
	return flag;
}
bool isDeRefer(){
	if(nr_token==0)//数字放在首位，必没有解引用
	return false;
	if(tokens[nr_token-1].type==TK_DE)
		return true;
	else return false;
}
