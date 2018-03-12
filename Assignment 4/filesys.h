#ifndef FILESYS_H
#define FILESYS_H

#include "structures.h"
#include <stdio.h>

#define NAMELEN 30
#define INLEN 2
#define TOTLEN (NAMELEN + INLEN)

int create_myfs (int);
int copy_pc2myfs(char *, char *);

struct direntry
{
	char name[NAMELEN + 1];
	short inno;

	void print()
	{
		cout << inno << " " << name << "\n";
		return;
	}

	direntry (char * s)
	{
		snprintf (name, NAMELEN + 1, "%s", s);
		inno = *(s + NAMELEN);
	}
};

#endif