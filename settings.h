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

    char * text;
    
    int (*function_before)();
    int (*function_after)();
} t_spacing_settings;

t_spacing_settings new_settings
    (int nl_before, int tab_before, int space_before, int nl_after, int tab_after, int space_after, char * text )
{
    t_spacing_settings new_settings;

    new_settings.nl_before    = nl_before;
    new_settings.tab_before   = tab_before;
    new_settings.space_before = space_before;

    new_settings.nl_after     = nl_after;
    new_settings.tab_after    = tab_after;
    new_settings.space_after  = space_after;
    new_settings.text         = text;

    new_settings.function_before = NULL;
    new_settings.function_after  = NULL;
    return new_settings;
}

t_spacing_settings new_settings_w_funct
    (int nl_before, int tab_before, int space_before, int nl_after, int tab_after, int space_after, char * text , int (*f1)(), int (*f2)() )
{
    t_spacing_settings new_settings;

    new_settings.nl_before    = nl_before;
    new_settings.tab_before   = tab_before;
    new_settings.space_before = space_before;

    new_settings.nl_after     = nl_after;
    new_settings.tab_after    = tab_after;
    new_settings.space_after  = space_after;
    new_settings.text         = text;

    new_settings.function_before = f1;
    new_settings.function_after  = f2;
    return new_settings;
}

void debug_spacing_settings(t_spacing_settings s){
    printf("\nspace_before %d , tab_before %d , nl_before %d , space_after %d , tab_after %d , nl_after %d\n , text %s "
           ,s.space_before,s.tab_before,s.nl_before,s.space_after,s.tab_after,s.nl_after, s.text);
}





t_spacing_settings sp_comma, kw_select, kw_inner_join, kw_left_join, kw_right_join, kw_full_join, kw_cross_join, kw_from, kw_on, kw_where, kw_and, kw_exists, kw_in, kw_from_2, kw_as;

t_spacing_settings kw_left_p, kw_right_p;

int testfunct(){printf("|xYx|");}

int debug_p();

void init_all_settings(){
    sp_comma      = new_settings(1,0,0,0,0,0,",");
    kw_select     = new_settings(1,0,0,0,0,1,"SelecT");
    kw_inner_join = new_settings(1,0,0,0,0,1,"JoiN");
    kw_left_join  = new_settings(1,0,0,0,0,1,"LefT JoiN");
    kw_right_join = new_settings(1,0,0,0,0,1,"RighT JoiN");
    kw_full_join  = new_settings(1,0,0,0,0,1,"FulL JoiN");
    kw_cross_join = new_settings(1,0,0,0,0,1,"CrosS JoiN");
    kw_from       = new_settings(1,0,0,0,0,1,"FroM");
    kw_on         = new_settings(1,0,0,0,0,1,"oN");
    kw_where      = new_settings(1,0,0,0,0,1,"WherE");
    kw_and        = new_settings(1,0,0,0,0,1,"AnD");
    kw_exists     = new_settings(0,0,0,0,0,1,"exists");
    kw_in         = new_settings(0,0,0,0,0,1,"in");
    kw_from_2     = new_settings(0,0,1,0,0,1,"from");
    kw_as         = new_settings(0,0,1,0,0,1,"as");
    kw_left_p   = new_settings_w_funct(0,0,0,0,0,0,"(",&debug_p,NULL);
    kw_right_p  = new_settings_w_funct(0,0,0,0,0,0,")",&debug_p,NULL);
}

#endif
