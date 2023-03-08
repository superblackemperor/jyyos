//load
def_EHelper(lw) {
  rtl_lm(s, ddest, dsrc1, id_src2->imm, 4);
}
def_EHelper(lbu){
  rtl_lm(s,ddest,dsrc1,id_src2->imm,1);
}
//store
def_EHelper(sw) {
  rtl_sm(s, ddest, dsrc1, id_src2->imm, 4);
}

def_EHelper(sh){
	rtl_sm(s,ddest,dsrc1,id_src2->imm,2);
}

def_EHelper(sb){
	rtl_sm(s,ddest,dsrc1,id_src2->imm,1);
}
//下面是I的算术指令
def_EHelper(addi){
	rtl_addi(s,ddest,dsrc1,id_src2->imm);
//无符号数加有符号数结果是无符号数，如果立即数是负数，那会不会造成错误？？不过我认为编译器应该会把加上一个负数转化为减去一个正数，所以不用担心会加上一个负数
}
def_EHelper(sltiu){
	rtl_setrelopi(s,RELOP_LTU,ddest,dsrc1,id_src2->imm);
}
def_EHelper(srai){
	rtl_srai(s,ddest,dsrc1,id_src2->imm);
}
def_EHelper(srli){
	rtl_srli(s,ddest,dsrc1,id_src2->imm);
}
def_EHelper(ani){
	rtl_andi(s,ddest,dsrc1,id_src2->imm);
}
def_EHelper(xori){
	rtl_xori(s,ddest,dsrc1,id_src2->imm);
}
//下面是reg的算术指令
def_EHelper(add){
	rtl_add(s,ddest,dsrc1,dsrc2);
}
def_EHelper(sub){
	rtl_sub(s,ddest,dsrc1,dsrc2);
}
def_EHelper(sltu){
	rtl_setrelop(s,RELOP_LTU,ddest,dsrc1,dsrc2);	
}
def_EHelper(xor){
	rtl_xor(s,ddest,dsrc1,dsrc2);
}
def_EHelper(or){
	rtl_or(s,ddest,dsrc1,dsrc2);
}
def_EHelper(sll){
	int imm=0x0000001f&*dsrc2;
	rtl_slli(s,ddest,dsrc1,imm);
}
def_EHelper(and){
	rtl_and(s,ddest,dsrc1,dsrc2);
}
def_EHelper(mul){
	rtl_mulu_lo(s,ddest,dsrc1,dsrc2);//无符号的,不用扩展
}
def_EHelper(div){
	rtl_divs_q(s,ddest,dsrc1,dsrc2);//需要符号扩展，但是同为32，就处理不处理都一样
}
