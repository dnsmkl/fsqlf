#ifndef CLI_H
#define CLI_H



#define FAIL_WITH_ERROR( CODE , MESSAGE , ... ) \
    {                                           \
        fprintf(stderr, "\n" MESSAGE "\n", __VA_ARGS__ );\
        exit( CODE );                           \
    } 


#define PRINT_OPTION_INFO( OPTION_TEXT , OPTION_DESCRIPTION ) \
    fprintf(stderr,"\t"   OPTION_TEXT   "\n\t    "   OPTION_DESCRIPTION   "\n");


#define ARGV_MATCH( INDEX , TEXT ) (strcmp(argv[ INDEX ] , TEXT ) == 0)


void usage_info(int argc, char **argv)
{
    fprintf(stderr,"usage: %s [<input_file>] [<output_file>] [options]\n", argv[0] );
    fprintf(stderr,"\t", argv[0] );
    fprintf(stderr,"If <output_file> or <input_file> is missing, then corresponding standart IO is used\n");
    fprintf(stderr,"options:\n");
    PRINT_OPTION_INFO( "--select-comma-newline (after|before|none)" , "New lines for each item in SELECT clause");
    PRINT_OPTION_INFO( "--select-newline-after <num>"               , "Put <num> new lines right after SELECT keyword");
    PRINT_OPTION_INFO( "--debug (none|state|match|paranthesis)"     , "Print info for debuging.  To have different kinds of debug output, use more then once");
}

void read_cli_options(int argc, char **argv)
{
    int i;
    for(i=1;i<argc;i++)
    {
        if( argv[i][0] != '-')
        {
            if(yyin == stdin){   //try to openinig INPUT file
                if(  !(yyin = fopen(argv[1],"r"))  ) FAIL_WITH_ERROR(1,"Error opening input file: %s", argv[i]);
            }
            else if(yyout == stdout){   //try to openinig OUTPUT file (only if INPUT file is set)
                if(  !(yyout=fopen(argv[2],"w+"))  ) FAIL_WITH_ERROR(1,"Error opening output file: %s", argv[i]);
            }
        } else if( ARGV_MATCH(i,"--select-comma-newline") )
        {
            if( ++i >= argc) FAIL_WITH_ERROR(1,"Missing value for option : %s", argv[i-1]);
            if(strcmp(argv[i],"after") == 0) {
                kw_comma.nl_before = 0;
                kw_comma.nl_after  = 1;
            } else if(strcmp(argv[i],"before") == 0) {
                kw_comma.nl_before = 1;
                kw_comma.nl_after  = 0;
            } else if(strcmp(argv[i],"none") == 0) {
                kw_comma.nl_before = 0;
                kw_comma.nl_after  = 0;
            }
        } else if( ARGV_MATCH(i,"--select-newline-after") )
        {
            if( ++i >= argc || !isdigit(argv[i][0]) ) FAIL_WITH_ERROR(1,"Missing or invalid value for option : %s", argv[i-1]);
            kw_select.nl_after = atoi(argv[i]);
        } else if( ARGV_MATCH(i,"--debug") )
        {
            if( ++i >= argc ) FAIL_WITH_ERROR(1,"Missing or invalid value for option : %s", argv[i-1]);
            if     ( ARGV_MATCH(i,"none")         ) debug_level |= DEBUGNONE;
            else if( ARGV_MATCH(i,"state")        ) debug_level |= DEBUGSTATES;
            else if( ARGV_MATCH(i,"match")        ) debug_level |= DEBUGMATCHES;
            else if( ARGV_MATCH(i,"paranthesis")  ) debug_level |= DEBUGPARCOUNTS;
            else FAIL_WITH_ERROR(1,"Missing or invalid value for option : %s", argv[i-1]);

        } else if( strcmp(argv[i],"--help") == 0 || strcmp(argv[i],"-h") == 0)
        {
            usage_info(argc, argv);
            exit(0);
        } else FAIL_WITH_ERROR(1,"Try `%s --help' for more information\n", argv[0]);
    }
}



#endif