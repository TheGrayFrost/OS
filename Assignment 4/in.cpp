#include "structures.h"

in::in()
{
	ftype = false;
	fsize = spused = 0; // bytes occupied in last block
	lastmod = lastrd = time(NULL);
	accp = md = 0; // mode = 0, 1, 2
	dirbl = new int[INDB];
	for (int i = 0; i < INDB; ++i)
		dirbl[i] = -1;
	sindbl = -1;
	dindbl = -1; // double in indirect blocks
	slbloc = dlbloc = cloc = -1; // current block cloc <- block in which  last data is there
	// slblock - out of 8 locations <- 
	/*
	if mod = 0;
	slblock index of dirbl wil have block number written 
	if mode =1 
	slslblock wil be the index of the sindbl[]
	dlbloc <- 53
	slbloc
	*/
}