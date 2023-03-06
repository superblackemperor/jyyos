def_EHelper(lw) {
  rtl_lm(s, ddest, dsrc1, id_src2->imm, 4);
}

def_EHelper(sw) {
  rtl_sm(s, ddest, dsrc1, id_src2->imm, 4);
}

//下面是I的算术指令
def_EHelper(addi){
	rtl_addi(s,ddest,dsrc1,id_src2->imm);
//无符号数加有符号数结果是无符号数，如果立即数是负数，那会不会造成错误？？不过我认为编译器应该会把加上一个负数转化为减去一个正数，所以不用担心会加上一个负数
}
def_EHelper(sltiu){
	rtl_setrelopi(s,RELOP_LTU,ddest,dsrc1,id_src2->imm);
}
//下面是reg的算术指令
def_EHelper(add){
	rtl_add(s,ddest,dsrc1,dsrc2);
}
def_EHelper(sub){
	rtl_sub(s,ddest,dsrc1,dsrc2);
}
