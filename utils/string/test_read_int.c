#include <assert.h>	// assert
#include "read_int.h"


int main()
{
	const char *txt = "12 345  6789";
	int r;

	int cnt = 1;

	cnt = read_int(txt, 6, &r);
	assert(cnt == 2);
	assert(r == 12);
	txt += cnt;

	cnt = read_int(txt, 6, &r);
	assert(cnt == 4); // 1 space + 3 digits
	assert(r == 345);
	txt += cnt;

	cnt = read_int(txt, 6, &r);
	assert(cnt == 6); // 2 space + 4 digits
	assert(r == 6789);
}
