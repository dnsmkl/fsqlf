#include <assert.h>	// assert
#include "read_int.h"


int main()
{
	int r;
	int cnt = 1;

	// Invalid number: starting with leading space.
	r = 7789;
	cnt = read_int(" z15", 6, &r);
	assert(cnt == 0);
	assert(r == 7789); // unchanged

	// Invalid number: empty string
	r = 7789;
	cnt = read_int("", 6, &r);
	assert(cnt == 0);
	assert(r == 7789); // unchanged

	// Invalid number: only spaces
	r = 7789;
	cnt = read_int("        ", 6, &r);
	assert(cnt == 0);
	assert(r == 7789); // unchanged

	// Valid number: limted by @lim
	r = 7789;
	cnt = read_int("1234", 2, &r);
	assert(cnt == 2);
	assert(r == 12);

	// Valid number: limted by @lim
	r = 7789;
	cnt = read_int(" 1234", 2, &r);
	assert(cnt == 2);
	assert(r == 1);

	// Number too big to fit into int
	r = 7789;
	cnt = read_int("9234567890123456789012345678901234567890", 1000, &r);
	assert(cnt == 40);
	assert(r == INT_MAX);


	// Demo: incrementaly read numbers from string.
	const char *txt = "12 345  6789";

	cnt = read_int(txt, 6, &r);
	assert(cnt == 2);
	assert(r == 12);
	txt += cnt;

	cnt = read_int(txt, 6, &r); // txt = " 345  6789"
	assert(cnt == 4); // 1 space + 3 digits
	assert(r == 345);
	txt += cnt;

	cnt = read_int(txt, 6, &r); // txt = "  6789"
	assert(cnt == 6); // 2 space + 4 digits
	assert(r == 6789);
	txt += cnt;

	cnt = read_int(txt, 6, &r); // txt = ""
	assert(cnt == 0); // 2 space + 4 digits
	assert(r == 6789);
}
