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

vector <int> prime_numbers;

void siever_of_eratosthenes()
{
  int i, j;
  bool prime[10000];

  for ( i = 0; i < 10000; ++i)
    prime[i] =true;

  for ( i = 2; i*i < 10000; ++i)
    if (prime[i]) 
      for(j = 2*i; j < 10000; j += i)
        prime[j] = false;

  for (i = 2; i < 10000; ++i)
    if (prime[i])
      prime_numbers.push_back(i);
}

int generate_random_prime_no()
{
  srand(time(NULL)^(getpid()<<16));
  int n = prime_numbers.size();
  int i = rand() % n;
  return prime_numbers[i];
}

shared_struct * attach_memory (int key)
{
  int shmid = shmget (key, SHM_SIZE, 0666);

  if (shmid < 0)
  {
    cout << "Couldn't get the shared mem " << shmid << endl;
    return NULL;
  }

  shared_struct * myseg = (shared_struct *) shmat(shmid, NULL, 0);
  if (myseg == (shared_struct *)-1)
  {
    cout << "Attachment failed" << endl;
    return NULL;
  }

  return myseg;
}

int generate_random_float()
{
  srand (time(NULL)^(getpid()<<16));
  float r = ((float)rand() / RAND_MAX) * 5; 
  return r;
}

void producer (shared_struct * myseg, int serial_no)
{
  int shmid, pr_no;
  float wait_time;
  time_t mytime;

  pr_no = generate_random_prime_no();
  wait_time = generate_random_float();

  usleep (wait_time*1000000);
  while ((myseg -> in+1) % BUF_SIZE == myseg -> out);

  myseg -> buffer [myseg -> in] = pr_no;
  myseg -> in = (myseg -> in + 1) % BUF_SIZE;  
  mytime = time (NULL);

  cout << "Producer " << serial_no << ":" << pr_no << ", time: " << ctime(&mytime) << endl;

  if (shmdt(myseg) == -1)
    cout << "Error detaching memory\n";

  return;
}

void consumer (shared_struct * myseg, int serial_no)
{
  int pr_no;
  float wait_time;
  time_t mytime;
  wait_time = generate_random_float();

  usleep (wait_time*1000000);

  while (myseg -> in == myseg -> out);
  pr_no = myseg -> buffer [myseg -> out];
  myseg -> out = (myseg -> out + 1) % BUF_SIZE;
  mytime = time(NULL);
  cout << "Consumer " << serial_no << ":" << pr_no << ", time: " << ctime(&mytime) << endl;

  if (shmdt(myseg) == -1)
    cout<<"Error detaching memory\n";

  return;
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

  shared_struct * myseg = attach_memory (key);

  if (myseg == NULL)
    exit(0);

  // initialize the in and out
  myseg -> in = 0;
  myseg -> out = 0;

  // creating producers
  for (i = 0; i < np; ++i)
  {
    prod[i] = fork();

    if(prod[i]  == 0)
      producer(myseg, i);
  }

  // creating consumers
  for ( i = 0; i < nc; ++i)
  {
    cons[i] = fork();

    if(cons[i] == 0)
      consumer(myseg, i);
  }

  sleep(WAIT_BEFORE_KILL);

  if (shmdt(myseg) == -1)
    cout << "Error detaching memory\n";

  // killing producers
  for (i = 0; i < np; ++i)
    kill(prod[i], SIGKILL);

  // killing condumers
  for (i = 0; i < nc; ++i)
    kill(cons[i], SIGKILL);

}