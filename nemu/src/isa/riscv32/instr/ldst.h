def_EHelper(lw) {
  rtl_lm(s, ddest, dsrc1, id_src2->imm, 4);
}

def_EHelper(sw) {
  rtl_sm(s, ddest, dsrc1, id_src2->imm, 4);
}

//下面是I的算术指令
def_EHelper(addi){
	rtl_addi(s,ddest,dsrc1,id_src2->imm);
}
