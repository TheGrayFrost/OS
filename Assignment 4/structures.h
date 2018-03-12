#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <iostream>
#include <time.h>

#define MAXINODE 1024
#define DBSIZE 256
#define INDB 8

using namespace std;

// superblock structure
struct sb
{
	int size;
	int maxin;
	int inused;
	int maxdb;
	int dbused;
	bool * inbmap;
	bool * dbbmap;

	sb(int);
	int getblock();
	int getinode();
};

// inode structure
struct in
{
	bool ftype;
	int fsize, spused;
	time_t lastmod;
	time_t lastrd;
	int accp, md;
	int * dirbl, cloc;
	int sindbl, slbloc;
	int dindbl, dlbloc;

	in();
};

// filesys structure
struct fsys
{
	sb * superbl;
	in * inlist;
	char (* dblist) [DBSIZE];
	unsigned short pwd;

	fsys (int);
	int createdir();
	int appendloc (int&);
	int addentry (char *, bool);
	char * addblock (in&);
} * myfs;

void us2char (char *, unsigned short);
void int2char (char *, int);

#endif