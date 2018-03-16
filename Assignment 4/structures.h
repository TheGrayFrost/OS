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
	bool ftype; 		//filetype
	int fsize, spused;	//filesize, spaceused in last block of the file
	time_t lastmod;		//lastmodified
	time_t lastrd;		//lst read
	int accp, md;		//access permission 0777 0770
	int * dirbl, cloc;	//array of direct block, direct block
	int sindbl, slbloc;	// single indirect block, 
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
/*	int appendloc (int&);
*/	int addentry (char *, bool);
	char * addblock (in&);
} * myfs;

void us2char (char *, unsigned short);
void int2char (char *, int);

#endif