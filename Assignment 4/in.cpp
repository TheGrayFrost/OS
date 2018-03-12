#include "structures.h"

in::in()
{
	ftype = false;
	fsize = spused = 0;
	lastmod = lastrd = time(NULL);
	accp = md = 0;
	dirbl = new int[INDB];
	for (int i = 0; i < INDB; ++i)
		dirbl[i] = -1;
	sindbl = -1;
	dindbl = -1;
	slbloc = dlbloc = cloc = -1;
}