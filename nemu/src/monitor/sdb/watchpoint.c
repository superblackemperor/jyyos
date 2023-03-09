#include "sdb.h"
#include "utils.h"

typedef struct watchpoint {
  int NO;
  char EXPR[320];
  uint32_t old_value,new_value;  
  struct watchpoint *next;

  /* TODO: Add more members if necessary */

} WP;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL,*free_ = NULL,*end=NULL;

WP*new_wp();
void free_wp(WP*p);

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    memset(wp_pool[i].EXPR,0,320);
    wp_pool[i].old_value=-1;
    wp_pool[i].new_value=-1;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }

  head = NULL;
  free_ = wp_pool;
}

bool init_wp(char*arg){
	WP*p=new_wp();
	bool flag=true;
	uint32_t EXPR=expr(arg,&flag);
	if(flag==false){printf("EXPR wrong\n");return false;}//先检查是否合法
	
	strcpy(p->EXPR,arg);
	p->new_value=EXPR;
	return true;

}

void scan_wp(){
	WP*bp[NR_WP];int i=0;
	for(WP*it=head;it!=NULL;it=it->next)
	{
	bool flag=true;
	uint32_t EXPR=expr(it->EXPR,&flag);
	if(flag==false){printf("EXPR wrong\n");return;}
	if(EXPR!=it->new_value){
		assert(i<32);
		if(nemu_state.state!=NEMU_END&&nemu_state.state!=NEMU_ABORT)
		nemu_state.state=NEMU_STOP;
	
		bp[i++]=it;
		it->old_value=it->new_value;
		it->new_value=EXPR;
		}
	}
	//bp里都是触发的断点
	for(int n=0;n<i;n++)
	printf("watchpoint %d:%s <old_value=%x> <new_value=%x>\n",
	bp[n]->NO,bp[n]->EXPR,bp[n]->old_value,bp[n]->new_value);
	

		
}

bool del_wp(int num){
	bool ret=false;
	for(WP*it=head;it!=NULL;it=it->next)
		if(it->NO==num)
		{free_wp(it);ret=true;}
	return ret;
	
}

//new可以让free_一直往后new，free可以让head表任意释放监视点
WP* new_wp(){
	assert(free_!=NULL);//无监视点可用
	WP*tmp=free_;
	free_=free_->next;
	if(head==NULL)
	{head=tmp;end=head;tmp->next=NULL;}
	else{
	tmp->next=NULL;
	end->next=tmp;
	end=end->next;
	}
	return end;
}

void free_wp(WP*p){//归还wp给free_，然后把head表连回去
	assert(p!=NULL);
	if(free_==NULL)
	free_=p;
	else{
	WP*free_end=free_;
	for(;free_end->next!=NULL;free_end=free_end->next);
	free_end->next=p;
	free_end=free_end->next;
	}
	//处理p表
	if(head==p){
	head=p->next;
	if(end==p)//只有一个监视点
	end=head;
	}
	else{
	WP*it=head;
	for(;it->next!=p;it=it->next);//只考虑有p的情况
	if(end==p)end=it;
	it->next=p->next;}
	p->next=NULL;
	
	//最后参数p指向了free_表的最后一个元素
}

void show_wp(){
	for(WP*it=head;it!=NULL;it=it->next)
	printf("watchpoint %d:%s    <current value=%xH>\n",
	it->NO,it->EXPR,it->new_value);
}
