#include <bits/stdc++.h>
using namespace std;
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <time.h>

#define WAIT_BEFORE_KILL 30
#define BUF_SIZE 5

struct shared_struct
{
  int buffer[BUF_SIZE];
  int in;
  int out;
};

int SHM_SIZE;

vector<int> prime_numbers;

void siever_of_eratosthenes()
{
  int i, j;
  bool prime[10000];
  for ( i = 0; i < 10000; ++i)
  {
    prime[i] =true;
    /* code */
  }
  for ( i = 2; i*i < 10000; ++i)
  {
    if (prime[i]) 
    {
      for(j = 2*i; j < 10000; j = j + i)
        {
          prime[j] = false;
        }
      /* code */
    }
    /* code */
  }

  for (i = 2; i < 10000; ++i)
  {
    if (prime[i])
    {
      prime_numbers.push_back(i);
    }
  }
}


int generate_random_prime_no()
{
  srand(time(0)^ (getpid()<<16));
  int n = prime_numbers.size();
  int i = ( rand()% n );
  return prime_numbers[i];
}

shared_struct *attach_memory(int key)
{
  int shmid = shmget(key, SHM_SIZE, 0666);

  if (shmid < 0)
  {
    cout <<"couldn't get the shared mem "<<shmid<<endl;
    exit(0);
  }

  shared_struct *myseg = (shared_struct *)shmat(shmid, NULL, 0);

  return myseg;
}
int generate_random_float()
{
  srand(time(0)^ (getpid()<<16));
  float r = ((float)rand() / RAND_MAX) * 5; 
  return (r);

}
void producer(key_t key, int serial_no)
{
  int shmid, pr_no;
  float wait_time;
  time_t mytime;

  shared_struct *myseg =attach_memory(key);
  pr_no = generate_random_prime_no();
  wait_time = generate_random_float();
  usleep(wait_time*1000000.0);
  while((myseg -> in+1) % BUF_SIZE == myseg -> out );

  myseg -> buffer[myseg -> in] = pr_no;
  myseg -> in = (myseg -> in +1) % BUF_SIZE;  
  mytime = time(NULL);

  cout <<"Producer "<< serial_no<<":" <<pr_no<<", time: "<<ctime(&mytime)<<endl;

  if (shmdt(myseg) == -1)
  {
    cout<<"error detaching memory";
  } 

  exit(0);

}

void consumer(key_t key, int serial_no)
{
  int pr_no;
  float wait_time;
  time_t mytime;
  wait_time = generate_random_float();

  shared_struct *myseg = attach_memory(key);

  usleep(wait_time*1000000.0);

  while(myseg -> in == myseg -> out);
  pr_no = myseg -> buffer[myseg -> out];
  myseg -> out = (myseg -> out+1)% BUF_SIZE;
  mytime = time(NULL);
  cout <<"Consumer "<< serial_no<<":" <<pr_no<<", time: "<<ctime(&mytime)<<endl;

  if (shmdt(myseg) == -1)
  {
    cout<<"error detaching memory";
  } 

  exit(0);
}
int main()
{
  siever_of_eratosthenes();
  int np, nc, i, shmid;
  cin >> np >> nc;
  pid_t prod[np], cons[nc];


  key_t key = 111;
  SHM_SIZE = sizeof(shared_struct);
  shmid = shmget(key, SHM_SIZE, IPC_CREAT|0666);

  if( shmid< 0 )
  {
    cout << "error getting shared memory"<<endl;
    return 0;
  }

  shared_struct *myseg = (shared_struct *)shmat(shmid, NULL, 0);

  if(myseg == (shared_struct *)-1)
  {
    cout << "attachment failed"<<endl;
  }

  //initialize the in and out
  myseg -> in = 0;
  myseg -> out = 0;


  for (i = 0; i < np; ++i)
  {
    prod[i] = fork();

    if(prod[i]  == 0)
    {
      producer(key, i);
    }

  }

  for ( i = 0; i < nc; ++i)
  {
    cons[i] = fork();

    if(cons[i] == 0)
    {
      consumer(key, i);
    }

  }
  sleep(WAIT_BEFORE_KILL );

  if(shmdt(myseg) == -1)
  {
    cout<<"error detaching memory";
  }
  for (i = 0; i < np; ++i)
  {
    kill(prod[i], SIGKILL);
  }

  for (i = 0; i < nc; ++i)
  {
    kill(cons[i], SIGKILL);
  }

}