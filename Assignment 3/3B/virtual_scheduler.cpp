#include <bits/stdc++.h>
using namespace std;
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <pthread.h>


#define SIZE 3000
#define inf 120

vector<int> STATUS; // -1 <- terminated : 0 <- sleep : 1<- running
int n;
vector<pthread_t> mythread;
queue<int> q;


float generate_random_float()
{
  srand(time(0)^ (pthread_self()<<16));
  float r = ((float)rand() / RAND_MAX) * 10; // to generate no.s from 0 to 10 
  return (r);

}

void sig1_handler(int signo)
{
	//cout<<"sleeping off"<<endl;
  if (signo == SIGUSR1)
    pause();

}

void sig2_handler(int signo)
{						
	//cout<<"Waking up"<<endl;
}


void * WORKER(void *ptr)
{
	int index = *((int*) ptr);
	cout << "Processor index "<<index<<endl;
	signal(SIGUSR1, sig1_handler);
	signal(SIGUSR2, sig2_handler);

//	if (signal(SIGUSR1, sig1_handler) == SIG_ERR)   printf("\ncan't catch SIGUSR1\n");
 //	if (signal(SIGUSR2, sig2_handler) == SIG_ERR)	printf("\ncan't catch SIGUSR2\n");

	float wait_time;
	srand(time(0) + index);	
	std::vector<int> vec;
	for (int i = 0; i < SIZE; ++i)
	{
			vec.push_back((rand() % 1000) + 1);
			//cout << vec[i]<<" ";
	}
	cout << endl;
	wait_time = generate_random_float();
	sort (vec.begin(), vec.end());
	//sleep(1);
	//cout <<"wait time = "<<wait_time<<" i = "<<index<<endl;
	usleep(wait_time*1000000.0);
	STATUS[index] = -1;	
	pthread_exit(0) ;
}



// WORKER 1, 2,3 , 4
void * SCHEDULER(void *ptr)
{

	int x; //index number of the thread
	
	//pthread_kill(q.front(), SIGUSR2); // to wake it up

	while (!q.empty())
	{
		x = q.front();
		pthread_kill(mythread[q.front()], SIGUSR2); // wake up
		sleep(1);
		if(STATUS[x] != -1) 
		{
			q.push(x);
			pthread_kill(mythread[q.front()], SIGUSR1); // sleep

		}

		q.pop();
		//if( !q.empty()) pthread_kill(mythread[q.front()], SIGUSR1);  // to run, will also set the statuss
	}
	pthread_exit(0) ;
}

void *REPORTER(void *ptr)
{
	if(!q.empty())
	{
		int last_proc = q.front();
		while(!q.empty())
		{
			if(q.front() !=last_proc)
			{
				
				if(q.back() == last_proc) cout <<"***contex switch from "<< last_proc << " to "<< q.front() <<"***"<< endl;
				else cout << "***Process "<< last_proc<<" terminated! Now processing "<<q.front()<<"***"<<endl;
				//cout  << "last_proc "<<last_proc<<" q.front "<<q.front()<<endl;
				last_proc = q.front();
			}
		}
		cout << "***Process "<< last_proc<<" terminated!***"<<endl;

	}
	pthread_exit(0) ;
}

int main()
{
	if (signal(SIGUSR1, sig1_handler) == SIG_ERR)   printf("\ncan't catch SIGUSR1\n");
 	if (signal(SIGUSR2, sig2_handler) == SIG_ERR)	printf("\ncan't catch SIGUSR2\n");

	int *ar;
	cin >> n;

	pthread_attr_t attr;
	pthread_attr_init(&attr);

	for (int i = 0; i < n; ++i)
	{
		q.push(i);
		STATUS.push_back(0);
	}


	for (int i = 0; i < n; ++i)
	{
		ar = new int;
		*ar = i;
		mythread.push_back(0);
		pthread_create(&mythread[i], &attr, WORKER, ar);
		pthread_kill(mythread[i], SIGUSR1);
	}

	pthread_t rep, sc;
	pthread_create(&rep, &attr, REPORTER, NULL);
	pthread_create(&sc, &attr, SCHEDULER, NULL);



	for (int i = 0; i < n; ++i)
	{
		pthread_join(mythread[i], NULL);
	}
	pthread_join(rep, NULL);
	pthread_join(sc, NULL);

}