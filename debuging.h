#ifndef debuging_h
#define debuging_h

#include "settings.h"

int debug_p(){
    if(debug_level & DEBUGPARCOUNTS)
        printf("\n\t***  (=%d; )=%d; subselect_level=%d)  ***\n", left_p , right_p, subselect_level);
}

int debug_match(char * debugstring){
    if(debug_level & DEBUGMATCHES)
        printf("\n\t**   %s   **\n", debugstring);
}

char* state_to_char(int state){
    char* sate_str;
    switch(state)
    {
        case INITIAL:  sate_str="INITIAL"   ; break;
        case stSELECT: sate_str="stSELECT"  ; break;
        case stFROM:   sate_str="stFROM"    ; break;
        case stWHERE:  sate_str="stWHERE"   ; break;
        case stON:     sate_str="stON"      ; break;
        case stEXISTS: sate_str="stEXISTS"  ; break;
        case stLEFTP:  sate_str="stLEFTP"   ; break;
        case stJOIN:   sate_str="stJOIN"    ; break;
        case stCOMMA:  sate_str="stCOMMA"   ; break;
        default:       sate_str="STATE NOT AVAILABLE";
    }
    return sate_str;
}

int debug_stchange(char* newstate)
{
    char * currentstate;
    
    currentstate=state_to_char(YY_START);
    
    if(debug_level & DEBUGSTATES)printf("\n\t*   %s->%s   *\n", currentstate,newstate);
}

int debug_stchange2(int newstate_int)
{
    char* currentstate;
    char* newstate;
    
    currentstate=state_to_char(YY_START);
    newstate=state_to_char(newstate_int);
    
    if(debug_level & DEBUGSTATES)printf("\n\t*   %s->%s   *\n", currentstate,newstate);
}

#endif
