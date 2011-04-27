#ifndef settings_h
#define settings_h

#define DEBUGNONE (0)
#define DEBUGSTATES (1)
#define DEBUGMATCHES (2)
#define DEBUGPARCOUNTS (4)

const int debug_level = DEBUGNONE ; //DEBUGSTATES | DEBUGPARCOUNTS;

typedef struct t_sel_item {
    int nl_before_comma ;
    int nl_after_comma ;
    int space_before_comma;
    int space_after_comma;
} t_sel_item  ;

/*
sel_list_item.nl_before_comma = 1;
sel_list_item.nl_after_comma  = 0;
sel_list_item.nl_before_comma = 0;
sel_list_item.nl_after_comma  = 1;
*/


#endif
