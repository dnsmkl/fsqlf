#include <assert.h> // assert
#include <limits.h> // INT_MAX
#include "read_int.h"


int main()
{
    int r;
    int cnt = 1;

    // Invalid number: starting with leading space.
    r = 7789;
    cnt = FSQLF_read_int(" z15", 6, &r);
    assert(cnt == 0);
    assert(r == 7789); // unchanged

    // Invalid number: empty string
    r = 7789;
    cnt = FSQLF_read_int("", 6, &r);
    assert(cnt == 0);
    assert(r == 7789); // unchanged

    // Invalid number: only spaces
    r = 7789;
    cnt = FSQLF_read_int("        ", 6, &r);
    assert(cnt == 0);
    assert(r == 7789); // unchanged

    // Valid number: limted by @lim
    r = 7789;
    cnt = FSQLF_read_int("1234", 2, &r);
    assert(cnt == 2);
    assert(r == 12);

    // Valid number: limted by @lim
    r = 7789;
    cnt = FSQLF_read_int(" 1234", 2, &r);
    assert(cnt == 2);
    assert(r == 1);

    // Number too big to fit into int
    r = 7789;
    cnt = FSQLF_read_int("9234567890123456789012345678901234567890 1", 1000, &r);
    assert(cnt == 40);
    assert(r == INT_MAX);


    // Demo: incrementaly read numbers from string.
    const char *txt = "12 345  6789";

    cnt = FSQLF_read_int(txt, 6, &r);
    assert(cnt == 2);
    assert(r == 12);
    txt += cnt; // txt = " 345  6789"

    cnt = FSQLF_read_int(txt, 6, &r);
    assert(cnt == 4); // 1 space + 3 digits
    assert(r == 345);
    txt += cnt; // txt = "  6789"

    cnt = FSQLF_read_int(txt, 6, &r);
    assert(cnt == 6); // 2 space + 4 digits
    assert(r == 6789);
    txt += cnt; // txt = ""

    cnt = FSQLF_read_int(txt, 6, &r);
    assert(cnt == 0); // 2 space + 4 digits
    assert(r == 6789);


    // Tests for FSQLF_read_int_array
    int rs[10];

    // Valid input
    cnt = FSQLF_read_int_array("1 2 3", 5, 3, rs);
    assert(cnt == 5);
    assert(rs[0] == 1);
    assert(rs[1] == 2);
    assert(rs[2] == 3);

    // One of numbers is invalid
    cnt = FSQLF_read_int_array("1 a 3", 5, 3, rs);
    assert(cnt == 0);

    // Less values exist in string then requested
    cnt = FSQLF_read_int_array("1 2  ", 5, 3, rs);
    assert(cnt == 0);

    // String contains more value then requested
    cnt = FSQLF_read_int_array("1 2 3 4", 20, 3, rs);
    assert(cnt == 5);

    // String contains enough values, but limit is set too low
    cnt = FSQLF_read_int_array("1 2 3 4", 3, 3, rs);
    assert(cnt == 0);
}

// gcc read_int.c -std=c99 -c -o read_int.o && gcc test_read_int.c -std=c99 -c -o test_read_int.o && gcc *.o && ./a.out
