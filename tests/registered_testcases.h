#ifndef REGISTERED_TESTCASES_H
#define REGISTERED_TESTCASES_H


#ifndef PATH_TC_STATICSQL
#define PATH_TC_STATICSQL "cases/"
#endif


#ifndef PATH_TC_GENERATED
#define PATH_TC_GENERATED "cases/"
#endif


enum tc_position
{
    TC_INPUT = 0,
    TC_ACTUAL = 1,
    TC_EXPECTED = 2
};


#define TC_COUNT (10)
const char *tcs[TC_COUNT][3] = {
    {
        PATH_TC_STATICSQL "bigquery_input.sql",
        PATH_TC_GENERATED "bigquery_actual.sql",
        PATH_TC_STATICSQL "bigquery_expected.sql"
    },
    {
        PATH_TC_STATICSQL "case_input.sql",
        PATH_TC_GENERATED "case_actual.sql",
        PATH_TC_STATICSQL "case_expected.sql"
    },
    {
        PATH_TC_STATICSQL "create_table_input.sql",
        PATH_TC_GENERATED "create_table_actual.sql",
        PATH_TC_STATICSQL "create_table_expected.sql"
    },
    {
        PATH_TC_STATICSQL "comment_marker_input.sql",
        PATH_TC_GENERATED "comment_marker_actual.sql",
        PATH_TC_STATICSQL "comment_marker_expected.sql"
    },
    {
        PATH_TC_STATICSQL "delete_input.sql",
        PATH_TC_GENERATED "delete_actual.sql",
        PATH_TC_STATICSQL "delete_expected.sql"
    },
    {
        PATH_TC_STATICSQL "group_order_input.sql",
        PATH_TC_GENERATED "group_order_actual.sql",
        PATH_TC_STATICSQL "group_order_expected.sql"
    },
    {
        PATH_TC_STATICSQL "insert_input.sql",
        PATH_TC_GENERATED "insert_actual.sql",
        PATH_TC_STATICSQL "insert_expected.sql"
    },
    {
        PATH_TC_STATICSQL "many_semicolons_input.sql",
        PATH_TC_GENERATED "many_semicolons_actual.sql",
        PATH_TC_STATICSQL "many_semicolons_expected.sql"
    },
    {
        PATH_TC_STATICSQL "update_input.sql",
        PATH_TC_GENERATED "update_actual.sql",
        PATH_TC_STATICSQL "update_expected.sql"
    },
    {
        PATH_TC_STATICSQL "using_input.sql",
        PATH_TC_GENERATED "using_actual.sql",
        PATH_TC_STATICSQL "using_expected.sql"
    }
};


#endif
