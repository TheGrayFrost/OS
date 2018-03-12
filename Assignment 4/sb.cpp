#include "structures.h"

sb::sb (int s)
{
	size = s * 1024 * 1024;
	maxin = MAXINODE;
	inused = 0;
	maxdb = size / DBSIZE;
	dbused = 0;
	if (s == 0)
		inbmap = dbbmap = NULL;
	else
	{
		inbmap = new bool [maxin];
		for (int i = 0; i < maxin; ++i)
			inbmap[i] = false;
		dbbmap = new bool [maxdb];			
		for (int i = 0; i < maxdb; ++i)
			dbbmap[i] = false;
	}
}

int sb::getblock()
{
	if (dbused == maxdb)
		return -1;
	for (int i = 0; i < maxdb; ++i)
	{
		if (dbbmap[i] == false)
		{
			dbbmap[i] = true;
			dbused++;
			return i;
		}
	}
	return -1;
}

int sb::getinode()
{
	if (inused == maxin)
		return -1;
	for (int i = 0; i < maxin; ++i)
	{
		if (inbmap[i] == false)
		{
			inbmap[i] = true;
			inused++;
			return i;
		}
	}
	return -1;
}