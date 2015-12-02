#ifndef debuging_h
#define debuging_h


#define DEBUGNONE (0)
#define DEBUGSTATES (1)
#define DEBUGMATCHES (2)
#define DEBUGPARCOUNTS (4)


extern int debug_level;


void debug_p();

void debug_match(char * debugstring);

char *state_to_char(int state);

void debug_stchange(int newstate_int);


#endif
