#include "structures.h"

#define NAMELEN 30
#define INLEN 2
#define TOTLEN (NAMELEN + INLEN)

void us2char (char * d, unsigned short r)
{
	for (int i = 0; i < 2; ++i)
		*(d + i) = ((char *) &r)[i];
	return;
}

void int2char (char * d, int r)
{
	for (int i = 0; i < 4; ++i)
		*(d + i) = ((char *) &r)[i];
	return;
}


fsys::fsys(int s)
{
	superbl = new sb(s);
	inlist = new in[MAXINODE];
	dblist = new char [superbl->maxdb][DBSIZE];
	pwd = 0;
	createdir();
}

int fsys::createdir()
{
	unsigned short inno = superbl->getinode();
	int dbno = superbl->getblock();
	int loc = 0;
	
	snprintf(&dblist[dbno][loc], NAMELEN + 1, "%s", ".");
	loc += NAMELEN;
	us2char(&dblist[dbno][loc], inno);
	loc += INLEN;
	snprintf(&dblist[dbno][loc], NAMELEN + 1, "%s", "..");
	loc += NAMELEN;
	us2char(&dblist[dbno][loc], pwd);
	loc += INLEN;

	inlist[inno].ftype = true;
	inlist[inno].fsize = loc;
	inlist[inno].spused = loc;
	inlist[inno].lastmod = inlist[inno].lastrd = time(NULL);
	inlist[inno].accp = 0777;
	inlist[inno].dirbl[0] = dbno;

	inlist[inno].md = inlist[inno].cloc = inlist[inno].slbloc = 0;

	return inno;
}

char * fsys::addblock (in& ap)
{
	int rx = DBSIZE / sizeof(int);
	int dbno = superbl->getblock();
	ap.cloc = dbno;
	ap.spused = 0;
	ap.slbloc++;
	switch (ap.md)
	{
		case 0:
				if (ap.slbloc < INDB)
					ap.dirbl[ap.slbloc] = dbno;
				else
				{
					ap.md++;
					ap.sindbl = superbl->getblock();
					int2char(dblist[ap.sindbl], dbno);
					ap.slbloc = 0;
				}
				break;
		case 1:
				if (ap.slbloc < rx)
					int2char((char *)&((int *)dblist[ap.sindbl])[ap.slbloc], dbno);
				else
				{
					ap.md++;
					ap.dindbl = superbl->getblock();
					int h = superbl->getblock();
					ap.dlbloc = ap.slbloc = 0;
					int2char (dblist[ap.dindbl], h);
					int2char (dblist[h], dbno);
				}
				break;
		case 2:
				if (ap.slbloc < rx)
				{
					int h = ((int *)dblist[ap.dindbl])[ap.dlbloc];
					int2char((char *)&((int *)dblist[h])[ap.slbloc], dbno);				
				}
				else
				{
					int h = superbl->getblock();
					ap.dlbloc++;
					int2char((char *)&((int *)dblist[ap.dindbl])[ap.dlbloc], h);
					int2char (dblist[h], dbno);		
					ap.slbloc = 0;			
				}
	}
	return dblist[dbno];
}

int fsys::addentry (char * nm, bool ty)
{
	char * u = NULL;
	in * r = &inlist[pwd];
	if (r->spused == DBSIZE)
		u = addblock(inlist[pwd]);
	else
		u = &dblist[r->cloc][r->spused];

	return 0;
}

