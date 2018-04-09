#include <bits/stdc++.h>
using namespace std;

#define VIRTUAL_ADDR_SPACE 64
#define MAX_FRAMES 8

int main()
{
	int n_ref = 1000;// 1 million
	float prob = 0.99;

	int l_page = 0;

	ofstream output;
  	output.open ("input_file.txt",std::ofstream::out | std::ofstream::app);

	output << "1\t"<< l_page<<endl;
	for (int i = 0; i < n_ref; ++i)
	{
		if( !((float)rand()/(float)RAND_MAX < prob) )
			l_page = rand()%VIRTUAL_ADDR_SPACE;

		output << rand()%2 <<"\t"<<l_page<<endl;
		/* code */
	}
}