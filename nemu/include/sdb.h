#ifndef __SDB_H__
#define __SDB_H__

#include <common.h>


#define NR_WP 32

word_t expr(char *e, bool *success);
bool init_wp(char*arg);
void scan_wp();
bool del_wp(int num);
void show_wp();
#endif
