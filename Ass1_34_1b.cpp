#include <bits/stdc++.h>
using namespace std;
#include  <sys/types.h>
#include <sys/wait.h>

#include<unistd.h>
#define MAX_SIZE 100

int main()
{
	int i, x =1, ret, status;
	char * arg[MAX_SIZE] ;
	string str;
	std::vector<string> vec;
	pid_t pid;

	while(1)
	{
		cout <<">";
		getline(cin, str);
		if(str == "quit") break;
		
		x = fork();

		if(x == 0)
		{
			std::istringstream is( str );

			std::string word;
			
			i = 0;

			while ( is >> word ) 
				{
					vec.push_back( word );
				}
				for (i = 0; i < vec.size(); ++i)
				{
					arg[i] = (char*)vec[i].c_str();
				}
				arg[i] = NULL;
				vec.clear();
				ret = execvp(*arg, arg);
				if(ret == -1)
					{
						cout <<"Incorrect command"<<endl;
						return 0;
					}
		}
		else
		{
			 pid = wait(&status);
		}

	}
}