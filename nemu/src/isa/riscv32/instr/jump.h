def_EHelper(jal){
	if(id_src1->imm>=0)
	cpu.pc+=id_src1->imm;
	else
	cpu.pc-=id_src1->imm;
  	rtl_li(s,ddest,cpu.pc+4);
}
