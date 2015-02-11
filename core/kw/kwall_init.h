#ifndef KWALL_INIT
#define KWALL_INIT


#include "kw.h"


void debug_p(); // TODO : make separate .c and .h files


void set_case(unsigned short int keyword_case);
void set_text_original(unsigned short int ind_original);
void init_all_settings(struct kw_conf * (*kw)(const char *));


#endif
