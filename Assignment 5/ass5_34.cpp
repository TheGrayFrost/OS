#include <bits/stdc++.h>
using namespace std;

#define VIRTUAL_ADDR_SPACE 64
#define MAX_FRAMES 8
#define FIFO 0
#define LRU 2
#define RANDOM 3
#define NRU 4
#define SEC_CHANCE 5
#define ll long long

int pg_table[VIRTUAL_ADDR_SPACE], wxtr;
queue<int> pages_loaded; 
list<int> lru_list;
list<int> rand_list;
list<int> nru_list;
vector<int> sec_vec;
int ptr = 0; // points to the index in the sec_vec

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

void set_modified(int pg)
{
	 pg_table[pg] = (pg_table[pg] | ( 1 << 1 ));

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
	if (algo == SEC_CHANCE)
		return sec_vec.size();

}

int is_referenced(int pg)
{
	int tmp= (pg_table[pg] & ( 1  )) ;
	return tmp;
}
void set_r_bit(int pg)
{
	pg_table[pg] = pg_table[pg] | 1;
	return;
}
void unset_r_bit(int pg)
{
	pg_table[pg] = pg_table[pg] & ~1;
	return;
}

void print_p_table()
{
	cout <<"<---------------PAGE TABLE---------------------->\n";
	cout <<"PG\tFrame"<<endl;
	for (int i = 0; i < VIRTUAL_ADDR_SPACE; ++i)
	{
		if(is_valid(i))
			cout << i <<"\t"<<frame_no(i)<<endl;
	}

}
int replace_with(int algo, int line_no) // returns the page to be 
{
	int tmp;
	if(algo == FIFO)
	{
		tmp = pages_loaded.front();
		pages_loaded.pop();
		//if(is_modified(tmp))cout << line_no<<": OUT\t "<<tmp<<"\t"<<frame_no(tmp)<<endl;
		//pg_table[tmp]= 0;
		return tmp;
	}
	if(algo == LRU)
	{
		tmp = lru_list.back();
		//if(is_modified(tmp))cout << line_no<<": OUT\t "<<tmp<<"\t"<<frame_no(tmp)<<endl;
		//pg_table[tmp] = 0;
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
				tmp = *it;
				rand_list.erase(it);
				break;
			}
			i++;

    	}
    	//if(is_modified(tmp))cout << line_no<<": OUT\t "<<tmp<<"\t"<<frame_no(tmp)<<endl;
		//pg_table[tmp] = 0;
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
				
				//if(is_modified(tmp))cout << line_no<<": OUT\t "<<tmp<<"\t"<<frame_no(tmp)<<endl;
				//pg_table[tmp] = 0;
				nru_list.erase(it);
				return tmp;
			}


    	}

		tmp = nru_list.front(); // page to be replaced
		//pg_table[tmp] = 0;
		nru_list.erase(nru_list.begin() );
		return tmp;
	}
	if (algo== SEC_CHANCE)
	{
		while(1)
		{
			if(  !is_referenced(sec_vec[ptr] ) )
			{
				tmp = sec_vec[ptr];
				//if(is_modified(tmp))cout << line_no<<": OUT\t "<<tmp<<"\t"<<frame_no(tmp)<<endl;
				sec_vec.erase(sec_vec.begin()+ptr);
				//pg_table[tmp]= 0;
				return tmp;			
			}
			unset_r_bit(sec_vec[ptr]);
			ptr ++;
			ptr = ptr % MAX_FRAMES;

		}
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
	if(algo == SEC_CHANCE)
	{
		sec_vec.push_back(pg);
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
		ll mem_ref = 0, pg_in =0 , pg_out = 0;
		int wr, pg; // =1 , read only
		int rep_algo = RANDOM;

		string word;
		int line_no=0, fr, old_pg;

		std::ifstream input( "final_input" );

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
				mem_ref ++;
			
		if(!is_valid(pg))
		{
			pg_in ++;
			if(rand_list.size() >= MAX_FRAMES)
			{
				old_pg = replace_with(rep_algo, line_no);

				fr = frame_no(old_pg);
				cout <<line_no<< ": UNMAP\t" <<  old_pg<<"\t"<<fr<<endl ;
				if(is_modified(old_pg)){
					pg_out++;
					cout << line_no<<": OUT\t "<<old_pg<<"\t"<<fr<<endl;
				}
				pg_table[old_pg] = 0;

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
		if(wr == 1) set_modified(pg);

	}

		cout << "<--------------------PAGE STATISTICS-------------------->"<<endl;
		cout <<"Memory References :\t"<<mem_ref<<endl;
		cout <<"Page in :\t" <<pg_in<<endl;
		cout <<"Page out :\t"<<pg_out<<endl;

		print_p_table();

}
void lru_case()
{

		int wr, pg; // =1 , read only
		int rep_algo = LRU;
		ll pg_in = 0, pg_out = 0, mem_ref = 0;
		string word;
		int line_no=0, fr, old_pg;

		std::ifstream input( "final_input" );

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
			mem_ref++;

		if(is_valid(pg))
		{
				lru_list.remove(pg);
				lru_list.push_front(pg);
			
		}
		else
		{
			pg_in++;
			if( lru_list.size() >= MAX_FRAMES)
			{
				old_pg = replace_with(rep_algo, line_no);
				fr = frame_no(old_pg);
				cout <<line_no<< ": UNMAP\t" <<  old_pg<<"\t"<<fr<<endl ;
				if(is_modified(old_pg)){
					pg_out++;
					cout << line_no<<": OUT\t "<<old_pg<<"\t"<<fr<<endl;
				}
				pg_table[old_pg] = 0;

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
		if(wr == 1) set_modified(pg);
	}

		cout << "<--------------------PAGE STATISTICS-------------------->"<<endl;
		cout <<"Memory References :\t"<<mem_ref<<endl;
		cout <<"Page in :\t" <<pg_in<<endl;
		cout <<"Page out :\t"<<pg_out<<endl;

		print_p_table();

}
// to be done in all algos  ; set modified bit  = 1 if written to

void fifo_case()
{
		ll pg_in = 0, pg_out = 0, mem_ref = 0;
		int wr, pg; // =1 , read only
		int rep_algo = FIFO;

		string word;
		int line_no=0, fr, old_pg;

		std::ifstream input( "final_input" );

		for( std::string line; getline( input, line ); )
		{
			//cout <<"pages_loaded"<<pages_loaded.size()<<endl;
			line_no++;

			if(is_comment(line)) continue;

			istringstream is(line); // to be done, case of blank lines..

			if (is >> word)
				wr = atoi(word.c_str());
			is >> word;
				pg = atoi(word.c_str());
		
			mem_ref++;
			if(is_valid(pg))
				{if(wr) set_modified(pg);}
			else
			{
				pg_in++;
				if(pages_loaded.size() >= MAX_FRAMES)
				{

					old_pg = replace_with(rep_algo, line_no);
					fr = frame_no(old_pg);
					cout <<line_no<< ": UNMAP\t" <<  old_pg<<"\t"<<fr<<endl ;

					if(is_modified(old_pg)){
						pg_out++;
						cout << line_no<<": OUT\t "<<old_pg<<"\t"<<fr<<endl;
					}

					pg_table[old_pg] = 0;

				}
				else 
				{
					fr = next_frame(rep_algo); 
				}
				cout <<line_no<< ": IN\t"<<pg<<"\t"<<fr<<endl;
				my_map(rep_algo,pg, fr);
				cout <<line_no<<": MAP\t"<< pg <<"\t"<<fr<<endl;
				if(wr) set_modified(pg);
			}

		}

		cout << "<--------------------PAGE STATISTICS-------------------->"<<endl;
		cout <<"Memory References :\t"<<mem_ref<<endl;
		cout <<"Page in :\t" <<pg_in<<endl;
		cout <<"Page out :\t"<<pg_out<<endl;

		print_p_table();
}

void second_chance() // complete tested!
{
		ll mem_ref = 0, pg_in = 0, pg_out = 0;
		int wr, pg; // =1 , read only
		int rep_algo = SEC_CHANCE;

		string word;
		int line_no=0, fr, old_pg;

		std::ifstream input( "final_input" );

		for( std::string line; getline( input, line ); )
		{
			line_no++;

			if(is_comment(line)) continue;

			istringstream is(line); // to be done, case of blank lines..

			if (is >> word)
				wr = atoi(word.c_str());
			is >> word;
				pg = atoi(word.c_str());
			mem_ref++;

			if(is_valid(pg))
			{
				set_r_bit(pg);

			} 	
			else
			{
				pg_in++;
				if(sec_vec.size() >= MAX_FRAMES)
				{
					old_pg = replace_with(rep_algo, line_no);
					fr = frame_no(old_pg);
					cout <<line_no<< ": UNMAP\t" <<  old_pg<<"\t"<<fr<<endl ;
					if(is_modified(old_pg)){
						pg_out++;
						cout << line_no<<": OUT\t "<<old_pg<<"\t"<<fr<<endl;}
					pg_table[old_pg] = 0;
					

				}
				else 
				{
					fr = next_frame(rep_algo); // chnage this
				}
				cout <<line_no<< ": IN\t"<<pg<<"\t"<<fr<<endl;
				my_map(rep_algo,pg, fr);
				cout <<line_no<<": MAP\t"<< pg <<"\t"<<fr<<endl;
				
			}
			if(wr == 1) {
				set_modified(pg);
			}

		}

		cout << "<--------------------PAGE STATISTICS-------------------->"<<endl;
		cout <<"Memory References :\t"<<mem_ref<<endl;
		cout <<"Page in :\t" <<pg_in<<endl;
		cout <<"Page out :\t"<<pg_out<<endl;

		print_p_table();

}


void clear_ref_bits()
{
	for (int i = 0; i < VIRTUAL_ADDR_SPACE; ++i)
	{
		pg_table[i]  &= ~(1UL);
	}
}

void nru_case() // working tested!
{
		ll mem_ref =0, pg_in = 0, pg_out = 0, wr;
		int pg; // =1 , read only
		int rep_algo = NRU;
		int mem_acc = 0;
		string word;
		int line_no=0, fr, old_pg;

		std::ifstream input( "final_input" );

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
			mem_ref++;

		if(is_valid(pg) )
		{
			set_r_bit(pg);
		}
		else
		{
			pg_in++ ;
			if(nru_list.size() >= MAX_FRAMES)
			{
				old_pg = replace_with(rep_algo, line_no);
				fr = frame_no(old_pg);
				cout <<line_no<< ": UNMAP\t" <<  old_pg<<"\t"<<fr<<endl ;
				if(is_modified(old_pg)){
					pg_out++;
					cout << line_no<<": OUT\t "<<old_pg<<"\t"<<fr<<endl;
				}
				pg_table[old_pg] = 0;

			}
			else 
			{
				fr = next_frame(rep_algo); // chnage this
			}
			cout <<line_no<< ": IN\t"<<pg<<"\t"<<fr<<endl;
			my_map(rep_algo,pg, fr);
			cout <<line_no<<": MAP\t"<< pg <<"\t"<<fr<<endl;
			set_r_bit(pg);
		}
		if(wr == 1) set_modified(pg);

	}
		cout << "<--------------------PAGE STATISTICS-------------------->"<<endl;
		cout <<"Memory References :\t"<<mem_ref<<endl;
		cout <<"Page in :\t" <<pg_in<<endl;
		cout <<"Page out :\t"<<pg_out<<endl;

		print_p_table();

}
void initialize_table()
{
	for (int i = 0; i < VIRTUAL_ADDR_SPACE; ++i)
	{
		pg_table[i] = 0;
	}

}
int main()
{
	initialize_table();
	cout <<"\n\nFIFO"<<endl<<endl;
	fifo_case(); // working :)

	initialize_table();
	cout <<"\n\nRANDOM"<<endl<<endl;
	random_case(); // done, and debuuged xD

	initialize_table();
	cout <<"\n\nLRU"<<endl<<endl;
	lru_case(); // done! :D

	initialize_table();
	cout <<"\n\nNRU"<<endl<<endl;
	nru_case(); // working :p

	initialize_table();
	cout <<"\n\nsecond_chance"<<endl<<endl;
	second_chance(); // working :)

	
	
	
}