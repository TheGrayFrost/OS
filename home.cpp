#include <bits/stdc++.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

#define MAX_SIZE 100

int main()
{
	int i, x, l;
	char ** arg;
	string str, word;
	vector <string> vec;

	while (1)
	{
		cout << "> ";
		getline (cin, str);

		if (str == "quit")
			break;
		
		x = fork();

		if (x == 0)
		{
			istringstream is(str);

			vec.clear();
			while (is >> word) 
				vec.push_back(word);

			l = vec.size();
			arg = new char * [l + 1];
			for (i = 0; i < l; ++i)
				arg[i] = (char *) vec[i].c_str();
			arg[i] = NULL;
			execvp (*arg, arg);

			cout << "Incorrect command\n";
			return 0;
		}
		
		wait(NULL);
	}
}