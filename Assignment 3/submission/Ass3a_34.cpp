#include <bits/stdc++.h>

#define MEAN 0.5	// mean of exponential series

using namespace std;

struct node
{
	int in;
	float rem;

	node(int in_, float rem_)
	{
		in = in_;
		rem = rem_;
	}
};

class mycomparator
{
public:
	int operator() (const node& n1, const node& n2)
    {
        return n1.rem > n2.rem;
    }
};
double preemp_sjf(vector <int> art, vector <int> bt)
{
	priority_queue<node, std::vector<node> ,mycomparator> queue;
	node curr = node(0, bt[0]);
	int ct = 0, tnt = 0, n = art.size();

	queue.push(node(0,bt[0]));

	for (int i = 1; i < n; ++i)
	{
		//cout <<"iteration for i = "<<i<<endl;
		//cout << "current queue contains "<<endl;

		while(!queue.empty() )
		{
			curr = queue.top();
			//cout << "popped "<<curr.in<< " "<<curr.rem<<endl;
			queue.pop();
			if(ct + curr.rem > art[i])
			{
				queue.push(node(curr.in, curr.rem - art[i] + ct));
				//cout<<"pushing "<<curr.in<< " "<< curr.rem - art[i] + ct << endl;
				ct = art[i];
				break;
			}
			else
			{
				ct += curr.rem;
				tnt += ct - art[curr.in];
			}
			
			//cout<<"process ending at "<<ct<<" "<<curr.in<<endl;
		}

		ct = art[i];

		queue.push(node(i, bt[i]));
		//cout<<"pushing "<<i<<" "<< bt[i]<<endl;
	}
	//cout <<"\n all process are done\n";

	while(!queue.empty() )
	{
		curr = queue.top();
		queue.pop();

		ct += curr.rem;
		tnt += ct - art[curr.in];

	}

	return (1.0 * tnt)/(1.0*n);
}

double fcfs (vector <int> art, vector <int> bt)
{
	int n = bt.size(), ct = 0, tnt = 0;
	for (int i = 0; i < n; ++i)
	{
		if (ct < art[i])
			ct = art[i];
		ct += bt[i];
		tnt += (ct - art[i]);
	}
	//cout << tnt << " ";
	return ((1.0 * tnt) / n);
}

double roundrobin (vector <int> art, vector <int> bt, int del)
{
	int n = bt.size(), h = n, i = 0, ct = 0, tnt = 0;
	//cout << n;
	while (h)
	{
		i %= h;
		//cout << "oi\n";
		if (ct < art[i])
			ct = art[i];
		if (bt[i] <= del)
		{
			ct += bt[i];
			tnt += (ct - art[i]);
			//cout << "arr\n";
			art.erase(art.begin() + i);
			bt.erase(bt.begin() + i);
			//cout << "barr\n";
			--h;
			//if (h == 0)
			//	break;
		}
		else
		{
			ct += del;
			bt[i] -= del;
			i += 1;
		}
		//cout << ct << " " << tnt << "\n";
	}
	return ((1.0 * tnt) / n);
}

int main()
{
	srand(time(NULL));

	int n;
	vector <int> art, bt;
	double t;

    ofstream fcf_file;
  	fcf_file.open ("fcfs.dat",std::ofstream::out | std::ofstream::app);
  	ofstream sjf_file;
  	sjf_file.open ("sjf.dat",std::ofstream::out | std::ofstream::app);
  	ofstream rr1_file;
  	rr1_file.open ("rr1.dat",std::ofstream::out | std::ofstream::app);
  	ofstream rr2_file;
  	rr2_file.open ("rr2.dat",std::ofstream::out | std::ofstream::app);
  	ofstream rr3_file;
  	rr3_file.open ("rr3.dat",std::ofstream::out | std::ofstream::app);


	cout << "Please enter number of processes: ";
	float avg_fcfs = 0, avg_sjf = 0,avg_rr1 = 0,avg_rr2= 0, avg_rr3 = 0;
	cin >> n;
	

	for (int j = 0; j < 10; ++j)
	{
		art.clear();
		bt.clear();
		art.push_back(0);
		for (int i = 0; i < n; ++i)
		{
			// generating exponential inter-arrival times
			t = (1.0 * rand())/RAND_MAX;
			art.push_back(-(log(t)/MEAN)); 
			if (art[i+1] > 10) //makes it not random
				art[i+1] = 10;

			// calculating actual arrival times
			art[i+1] += art[i];

			// generating uniform burst times
			bt.push_back((rand() % 20) + 1);
			//cout << i + 1 << ". " << art[i] << " " << bt[i] << "\n";
			//cout << roundrobin(art, bt, 5) << "\n";
			//cout << fcfs(art, bt) << "\n";
		}
		art.pop_back();

	
		avg_fcfs += fcfs(art, bt);
		avg_sjf += preemp_sjf(art,bt);
		avg_rr1 += roundrobin(art, bt, 1);
		avg_rr2 +=roundrobin(art, bt, 2);
		avg_rr3 +=roundrobin(art, bt, 5);
		/* code */
	}
	fcf_file << n <<"\t"<<avg_fcfs/10<<endl;
	sjf_file << n <<"\t"<<avg_sjf/10<<endl;
	rr1_file << n <<"\t"<<avg_rr1/10<<endl;
	rr2_file << n <<"\t"<<avg_rr2/10<<endl;
	rr3_file << n <<"\t"<<avg_rr3/10<<endl;
	fcf_file.close();
	sjf_file.close();
	rr1_file.close();
	rr2_file.close();
	rr3_file.close();
	//cout << preemp_sjf(art, bt);
	/*
	int h[] = {0, 7, 10, 12};
	int w[] = {4, 11, 2, 5};
	art.assign(h, h + 4);
	bt.assign(w, w + 4);
	cout << fcfs(art, bt) << "\n";*/

	return 0;
}