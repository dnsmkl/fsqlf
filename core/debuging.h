#ifndef debuging_h
#define debuging_h

#include "settings.h"

#define DEBUGNONE (0)
#define DEBUGSTATES (1)
#define DEBUGMATCHES (2)
#define DEBUGPARCOUNTS (4)


int debug_level = DEBUGNONE ;//| DEBUGSTATES | DEBUGMATCHES;


void debug_p()
{
    if(debug_level & DEBUGPARCOUNTS)
        printf("\n\t***  '('=%d; ')'=%d; sub_openings.length=%d)  ***\n", left_p, right_p, sub_openings.length);
}


void debug_match(char * debugstring)
{
    if(debug_level & DEBUGMATCHES)
        printf("\n\t**   %s   **\n", debugstring);
}


char* state_to_char(int state)
{
    char* state_str;
    switch(state)
    {
        case INITIAL:   state_str="INITIAL"   ; break;
        case stSELECT:  state_str="stSELECT"  ; break;
        case stFROM:    state_str="stFROM"    ; break;
        case stWHERE:   state_str="stWHERE"   ; break;
        case stON:      state_str="stON"      ; break;
        case stEXISTS:  state_str="stEXISTS"  ; break;
        case stLEFTP:   state_str="stLEFTP"   ; break;
        case stJOIN:    state_str="stJOIN"    ; break;
        case stCOMMA:   state_str="stCOMMA"   ; break;
        case stIN:      state_str="stIN"      ; break;
        case stINLIST:  state_str="stINLIST"  ; break;
        case stP_SUB:  state_str="stP_SUB"    ; break;
        case stCOMMENTML:  state_str="stCOMMENTML"  ; break;
        case stSTRING:     state_str="stSTRING"     ; break;
        case stFROM_LEFTP: state_str="stFROM_LEFTP" ; break;
        default:        state_str="STATE NOT AVAILABLE";
    }
    return state_str;
}


void debug_stchange(int newstate_int)
{
    char* currentstate;
    char* newstate;

    currentstate=state_to_char(YY_START);
    newstate=state_to_char(newstate_int);

    if(debug_level & DEBUGSTATES) printf("\n\t*   %s->%s   *\n", currentstate,newstate);
}


#endif
