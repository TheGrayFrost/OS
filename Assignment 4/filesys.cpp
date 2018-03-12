#include "filesys.h"

int create_myfs (int size)
{
	try
	{
		myfs = new fsys(size);
		return 0;
	}

	catch (int e)
	{
		cout << e << ": An exception occured in filesys creation.\n";
		return -1;
	}
}

int copy_pc2myfs(char * src, char * dest)
{
	FILE * fp = fopen (src, "rb");
	if (fp == NULL)
	{
		cout << src << ": File does not exist.\n";
		return -1;
	}



	int d = fclose (fp);
	if (d == EOF)
	{
		cout << src << ": Error closing file.\n";
		return -1;
	}
	return 0;
}

void test (char * &u)
{
	u = (char *)98;
	return;
}


int main()
{
	int s = 9;
	char * h = (char *) &s;
	test(h);
	cout << (long)h << "\n";
	unsigned short r = 20;
	unsigned short inno = 0;
	unsigned short dbno = 0;
	char (*dblist) [256] = new char[2][256];
	int loc = NAMELEN;
	snprintf(dblist[dbno], NAMELEN + 1, "%s", ".");
	us2char(&dblist[dbno][loc], inno);
	loc += INLEN;
	snprintf(&dblist[dbno][loc], NAMELEN + 1, "%s", "..");
	loc += NAMELEN;
	us2char(&dblist[dbno][loc], inno);
	direntry g(dblist[dbno]);
	g.print();
	direntry u(dblist[dbno] + TOTLEN);
	u.print();

	return 0;
}


