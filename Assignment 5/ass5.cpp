#include <bits/stdc++.h>
using namespace std;

#define VIRTUAL_ADDR_SPACE 64
#define MAX_FRAMES 3
#define FIFO 0
#define LRU 2
int pg_table[VIRTUAL_ADDR_SPACE];
queue<int> pages_loaded; 
list<int> lru_list;
bool is_comment(string line)
{
	string word;
	istringstream is(line);
	if (is >> word)
		if(word[0] != '#' ) return false;
	return true;
}
int frame_no(int pg)
{
	int tmp = pg_table[pg];
	tmp = tmp >>3;
	return tmp;
}

bool is_modified(int pg)
{
	int tmp= (pg_table[pg] & ( 1 << 1 )) >> 1;
	return tmp;

}
bool is_valid(int pg)
{
	int tmp= (pg_table[pg] & ( 1 << 2 )) >> 2;
	if(tmp == 1) return true;
	return false;

}
int next_frame(int algo)
{
	if(algo == FIFO)
		return pages_loaded.size();
	if (algo == LRU)
		return lru_list.size();
}

int replace_with(int algo) // returns the page to be 
{
	int tmp;
	if(algo == FIFO)
	{
		tmp = pages_loaded.front();
		pages_loaded.pop();
		return tmp;
	}
	if(algo == LRU)
	{
		tmp = lru_list.back();
		lru_list.pop_back();
		return tmp;
	}
}
void my_map(int algo, int pg, int fr)
{
	if(algo == FIFO)
	{
		pages_loaded.push(pg);
		pg_table[pg] = fr;
		pg_table[pg] = pg_table[pg] << 3;
		pg_table[pg] += 1<<2;
		return;
	}
	if(algo == LRU)
	{
		lru_list.push_front(pg);
		pg_table[pg] = fr;
		pg_table[pg] = pg_table[pg] << 3;
		pg_table[pg] += 1<<2;
		return;
	}
	// to be done, what about referenced bit ?
}
int main()
{
	int wr, pg; // =1 , read only
	int rep_algo = LRU;
	/*
	0 - First-in First-out (FIFO)
	1 Random
	2 Least Recently Used (LRU)
	3 Not Recently Used (NRU)
	4 Second chance (derivative of FIFO)
	*/
	
	string word;
	int line_no=0, fr, old_pg;

	std::ifstream input( "input2.txt" );

	for( std::string line; getline( input, line ); )
	{
		line_no++;

		if(is_comment(line)) continue;

		istringstream is(line);

		if (is >> word)
			wr = atoi(word.c_str());
		is >> word;
			pg = atoi(word.c_str());
		//cout <<pg<<endl;

		if(is_valid(pg))
		{
			if(rep_algo == LRU)
			{
				lru_list.remove(pg);
				lru_list.push_front(pg);
			}
		}
		else
		{
			if(pages_loaded.size() >= MAX_FRAMES || lru_list.size() >= MAX_FRAMES)
			{
				old_pg = replace_with(rep_algo);
				fr = frame_no(old_pg);
				cout <<line_no<< ": UNMAP\t" <<  old_pg<<"\t"<<fr<<endl ;
				if(is_modified(old_pg))cout << line_no<<": OUT\t "<<old_pg<<"\t"<<fr<<endl;

			}
			else 
			{
				fr = next_frame(rep_algo); // chnage this
				//pages_loaded.push(pg);
				//cout <<"fr = "<<fr<<endl;
			}
			cout <<line_no<< ": IN\t"<<pg<<"\t"<<fr<<endl;
			my_map(rep_algo,pg, fr);
			cout <<line_no<<": MAP\t"<< pg <<"\t"<<fr<<endl;
		}

	}

}