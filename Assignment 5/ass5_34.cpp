#include <bits/stdc++.h>
using namespace std;

#define VIRTUAL_ADDR_SPACE 64
#define MAX_FRAMES 3
#define FIFO 0
#define LRU 2
#define RANDOM 3
#define NRU 4

int pg_table[VIRTUAL_ADDR_SPACE];
queue<int> pages_loaded; 
list<int> lru_list;
list<int> rand_list;
list<int> nru_list;

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
	if (algo == RANDOM)
		return rand_list.size();
	if (algo == NRU)
		return nru_list.size();
}

int is_referenced(int pg)
{
	int tmp= (pg_table[pg] & ( 1  )) ;
	return tmp;
}
int replace_with(int algo) // returns the page to be 
{
	int tmp;
	if(algo == FIFO)
	{
		tmp = pages_loaded.front();
		pages_loaded.pop();
		pg_table[tmp]= 0;
		return tmp;
	}
	if(algo == LRU)
	{
		tmp = lru_list.back();
		pg_table[tmp] = 0;
		lru_list.pop_back();
		return tmp;
	}
	if(algo == RANDOM)
	{
		int tmp_index = rand() % rand_list.size() ; 
		//tmp = rand_list[tmp_index];
		list<int>::iterator it;
		int i = 0;
		for (it=rand_list.begin(); it!=rand_list.end(); ++it)
    	{
    		if(i == tmp_index)
    		{
    			rand_list.erase(it);
				tmp = *it;
				rand_list.erase(it);
				break;
			}
			i++;

    	}

		pg_table[tmp] = 0;
		return tmp;
	}
	if(algo == NRU)
	{
		list<int>::iterator it;
		for (it=nru_list.begin(); it!=nru_list.end(); ++it)
    	{
    		if(!is_referenced(*it))
			{
				tmp = *it;
				pg_table[tmp] = 0;
				nru_list.erase(it);
				return tmp;
			}


    	}

		tmp = nru_list.front(); // page to be replaced
		pg_table[tmp] = 0;
		nru_list.erase(nru_list.begin() );
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
	if(algo == RANDOM)
	{
		rand_list.push_front(pg);
		pg_table[pg] = fr;
		pg_table[pg] = pg_table[pg] << 3;
		pg_table[pg] += 1<<2;
		return;
	}
	if(algo == NRU)
	{
		nru_list.push_back(pg);
		pg_table[pg] = fr;
		pg_table[pg] = pg_table[pg] << 3;
		pg_table[pg] += 1<<2;
		pg_table[pg] += 1;
		return;
	}
	// to be done, what about referenced bit ?
}

void random_case()
{

		int wr, pg; // =1 , read only
		int rep_algo = RANDOM;

		string word;
		int line_no=0, fr, old_pg;

		std::ifstream input( "ip.txt" );

		for( std::string line; getline( input, line ); )
		{
			line_no++;

			if(is_comment(line)) continue;

			istringstream is(line); // to be done, case of blank lines..

			if (is >> word)
				wr = atoi(word.c_str());
			is >> word;
				pg = atoi(word.c_str());
/*			cout <<pg<<endl;
*/
		
			if(line_no == 22)
			{
				cout << pages_loaded.front()<< " " <<pages_loaded.back()<<endl;
				cout << pg_table[0]<<endl;

			}
		if(!is_valid(pg))
		{

			cout <<"check";
			if(pages_loaded.size() >= MAX_FRAMES)
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
void lru_case()
{
		int wr, pg; // =1 , read only
		int rep_algo = LRU;

		string word;
		int line_no=0, fr, old_pg;

		std::ifstream input( "input2.txt" );

		for( std::string line; getline( input, line ); )
		{
			line_no++;

			if(is_comment(line)) continue;

			istringstream is(line); // to be done, case of blank lines..

			if (is >> word)
				wr = atoi(word.c_str());
			is >> word;
				pg = atoi(word.c_str());
			//cout <<pg<<endl;

		if(is_valid(pg))
		{
				lru_list.remove(pg);
				lru_list.push_front(pg);
			
		}
		else
		{
			if( lru_list.size() >= MAX_FRAMES)
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
// to be done in all algos  ; set modified bit  = 1 if written to

void fifo_case()
{
		int wr, pg; // =1 , read only
		int rep_algo = FIFO;

		string word;
		int line_no=0, fr, old_pg;

		std::ifstream input( "ip.txt" );

		for( std::string line; getline( input, line ); )
		{
			line_no++;

			if(is_comment(line)) continue;

			istringstream is(line); // to be done, case of blank lines..

			if (is >> word)
				wr = atoi(word.c_str());
			is >> word;
				pg = atoi(word.c_str());
/*			cout <<pg<<endl;
*/
		
			if(line_no == 22)
			{
				cout<<"sj";
				cout << pages_loaded.front()<< " " <<pages_loaded.back()<<endl;
				cout << pg_table[0]<<endl;

			}
		if(!is_valid(pg))
		{

			cout <<"check";
			if(pages_loaded.size() >= MAX_FRAMES)
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

void second_chance()
{
		int wr, pg; // =1 , read only
		int rep_algo = FIFO;

		string word;
		int line_no=0, fr, old_pg;

		std::ifstream input( "ip.txt" );

		for( std::string line; getline( input, line ); )
		{
			line_no++;

			if(is_comment(line)) continue;

			istringstream is(line); // to be done, case of blank lines..

			if (is >> word)
				wr = atoi(word.c_str());
			is >> word;
				pg = atoi(word.c_str());
/*			cout <<pg<<endl;
*/
		
			if(line_no == 22)
			{
				cout<<"sj";
				cout << pages_loaded.front()<< " " <<pages_loaded.back()<<endl;
				cout << pg_table[0]<<endl;

			}
		if(!is_valid(pg))
		{

			cout <<"check";
			if(pages_loaded.size() >= MAX_FRAMES)
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


void clear_ref_bits()
{
	for (int i = 0; i < VIRTUAL_ADDR_SPACE; ++i)
	{
		pg_table[i]  &= ~(1UL);
		/* code */
	}
}
void set_r_bit(int pg)
{
	pg_table[pg] |= 1UL ;
}
void nru_case()
{

		int wr, pg; // =1 , read only
		int rep_algo = NRU;
		int mem_acc = 0;
		string word;
		int line_no=0, fr, old_pg;

		std::ifstream input( "ip.txt" );

		for( std::string line; getline( input, line ); )
		{
			line_no++;

			if(is_comment(line)) continue;
			mem_acc ++;

			if(mem_acc == 1000) clear_ref_bits();

			istringstream is(line); // to be done, case of blank lines..

			if (is >> word)
				wr = atoi(word.c_str());
			is >> word;
				pg = atoi(word.c_str());
/*			cout <<pg<<endl;
*/

		if(is_valid(pg) )
		{
			set_r_bit(pg);
		}
		else
		{
			if(nru_list.size() >= MAX_FRAMES)
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
void initialize_table()
{
	for (int i = 0; i < VIRTUAL_ADDR_SPACE; ++i)
	{
		pg_table[i] = 0;
		/* code */
	}

}
int main()
{
	

	//fifo_case();
	lru_case();
	/*
	0 - First-in First-out (FIFO)
	1 Random
	2 Least Recently Used (LRU)
	3 Not Recently Used (NRU)
	4 Second chance (derivative of FIFO)
	*/
	
	

}