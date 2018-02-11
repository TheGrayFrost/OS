#include <bits/stdc++.h>
using namespace std;
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <pthread.h>


#define SIZE 100
#define inf 120

vector<int> STATUS; // -1 <- terminated : 0 <- sleep : 1<- running
int n;
vector<pthread_t> mythread;
queue<int> q;


int generate_random_float()
{
  srand(time(0)^ (pthread_self()<<16));
  float r = ((float)rand() / RAND_MAX) * 5; 
  return (r);

}

void sig1_handler(int signo)
{
  if (signo == SIGUSR1)
    pause();

}

void sig2_handler(int signo)
{

}


void * WORKER(void *ptr)
{
	int index = *((int*) ptr);
	cout << "Index is "<<index<<endl;

	if (signal(SIGUSR1, sig1_handler) == SIG_ERR)   printf("\ncan't catch SIGUSR1\n");
 	if (signal(SIGUSR2, sig2_handler) == SIG_ERR)	printf("\ncan't catch SIGUSR2\n");

	float wait_time;
	srand(time(0));
	std::vector<int> vec;
	for (int i = 0; i < SIZE; ++i)
	{
			vec.push_back((rand() % 1000) + 1);
			cout << vec[i]<<endl;
	}
	wait_time = generate_random_float();
	sort (vec.begin(), vec.end());
	usleep(wait_time*1000000.0);
	STATUS[index] = -1;	
	pthread_exit(0) ;
}



// WORKER 1, 2,3 , 4
void * SCHEDULER(void *ptr)
{

	int x;
	for (int i = 0; i < n; ++i)
	{
		pthread_kill(mythread[i], SIGUSR1);
		STATUS.push_back(0); // sleep
	}
	
	pthread_kill(q.front(), SIGUSR2); // to wake it up

	while (!q.empty())
	{
		sleep(1);
		x = q.front();
		pthread_kill(q.front(), SIGUSR1); // sleep
		if(STATUS[x] != -1) q.push(x);
		q.pop();
		pthread_kill(q.front(), SIGUSR2);  // to run, will also set the statuss
	}

}

void *REPORTER(void *ptr)
{
	int last_proc = q.front();
	while(q.empty())
	{
		if(q.front() !=last_proc)
		{
			last_proc = q.front();
			if(q.back() == last_proc) cout <<"contex switch from "<< last_proc << " to "<< q.front() << endl;
			else cout << "Process terminated\n";
		}
	}
}

int main()
{
	int *ar;
	cin >> n;
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	for (int i = 0; i < n; ++i)
	{
		q.push(i);
	}


	for (int i = 0; i < n; ++i)
	{
		ar = new int;
		*ar = i;
		mythread.push_back(0);
		pthread_create(&mythread[i], &attr, WORKER, ar);
	}

	pthread_t rep, wor;
	pthread_create(&rep, &attr, REPORTER, NULL);
	pthread_create(&wor, &attr, WORKER, NULL);

	pthread_join(rep, NULL);
	pthread_join(wor, NULL);

	for (int i = 0; i < n; ++i)
	{
		pthread_join(mythread[i], NULL);
	}

}