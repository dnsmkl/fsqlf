#ifndef REGISTERED_TESTCASES_H
#define REGISTERED_TESTCASES_H


enum tc_position
{
    TC_INPUT = 0,
    TC_ACTUAL = 1,
    TC_EXPECTED = 2
};


#define TC_COUNT (9)
const char *tcs[TC_COUNT][3] = {
    {
        "cases/bigquery_input.sql",
        "cases/bigquery_actual.sql",
        "cases/bigquery_expected.sql"
    },
    {
        "cases/case_input.sql",
        "cases/case_actual.sql",
        "cases/case_expected.sql"
    },
    {
        "cases/create_table_input.sql",
        "cases/create_table_actual.sql",
        "cases/create_table_expected.sql"
    },
    {
        "cases/delete_input.sql",
        "cases/delete_actual.sql",
        "cases/delete_expected.sql"
    },
    {
        "cases/group_order_input.sql",
        "cases/group_order_actual.sql",
        "cases/group_order_expected.sql"
    },
    {
        "cases/insert_input.sql",
        "cases/insert_actual.sql",
        "cases/insert_expected.sql"
    },
    {
        "cases/many_semicolons_input.sql",
        "cases/many_semicolons_actual.sql",
        "cases/many_semicolons_expected.sql"
    },
    {
        "cases/update_input.sql",
        "cases/update_actual.sql",
        "cases/update_expected.sql"
    },
    {
        "cases/using_input.sql",
        "cases/using_actual.sql",
        "cases/using_expected.sql"
    }
};


#endif
