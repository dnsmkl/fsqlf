#ifndef settings_h
#define settings_h

#define DEBUGNONE (0)
#define DEBUGSTATES (1)
#define DEBUGMATCHES (2)
#define DEBUGPARCOUNTS (4)

const int debug_level = DEBUGNONE ;//| DEBUGSTATES | DEBUGMATCHES;


// YY_USER_INIT is lex macro executed before initialising parser
#define YY_USER_INIT init_all_settings();



// struture which to store spacing (space/tab/newline) before/after some keyword
typedef struct t_spacing_settings {
    int nl_before;
    int tab_before;
    int space_before;

    int nl_after;
    int tab_after;
    int space_after;
} t_spacing_settings;

t_spacing_settings new_spacing_settings
    ( nl_before, tab_before, space_before, nl_after, tab_after, space_after )
{
    t_spacing_settings new_settings;

    new_settings.nl_before    = nl_before;
    new_settings.tab_before   = tab_before;
    new_settings.space_before = space_before;

    new_settings.nl_after     = nl_after;
    new_settings.tab_after    = tab_after;
    new_settings.space_after  = space_after;

    return new_settings;
}

void debug_spacing_settings(t_spacing_settings s){
    printf("\nspace_before %d , tab_before %d , nl_before %d , space_after , tab_after %d , nl_after %d\n"
           ,s.space_before,s.tab_before,s.nl_before,s.space_after,s.tab_after,s.nl_after);
}





t_spacing_settings sp_comma, sp_select;




void init_all_settings(){
    sp_comma = new_spacing_settings(1,0,0,0,0,0);
}

#endif
