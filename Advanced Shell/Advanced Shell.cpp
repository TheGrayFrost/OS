#include <bits/stdc++.h>
#include <fstream>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <csignal>

using namespace std;

char menu()
{
	char u; 
	cout << "\n\t\tVishesh and Shikha's Shell\n\n";
	cout << "A. Run an internal command\n";
	cout << "B. Run an external command\n";
	cout << "C. Run an external command by redirecting standard input from a file\n";
	cout << "D. Run an external command by redirecting standard output to a file\n";
	cout << "E. Run an external command in the background\n";
	cout << "F. Run several external commands in the pipe mode\n";
	cout << "G. Quit the shell\n";
	cout << "\nPlease enter your choice: ";
	cin >> u;
	while ((u < 'A') || (u > 'G'))
	{
		cout << "\nInvalid choice.\nPlease enter again: ";
		cin >> u;
	}
	return u;
}

string wd;

bool internal (const char * cmd, const char * arg = NULL)
{
	if (strcmp(cmd, "echo") == 0)
	{
		if (arg[0] == '$')
			cout << getenv(&arg[1]) << "\n";
		else if (strcmp(arg, "") == 0)
			cout << "\n";
		else
			cout << arg << "\n";
	}

	else if (strcmp(cmd, "cd") == 0)
	{
		chdir(arg);
		char cmd[1024];
		getcwd(cmd, 1024);
		setenv("PWD", cmd, 1);
		cout << "\nDirectory changed to: " << getenv("PWD") << "\n";
	}

	else if (strcmp(cmd, "mkdir") == 0)
	{
		mkdir(arg, 0755);
		cout << "\nNew directory created: " << arg << "\n";
	}

	else if (strcmp(cmd, "rmdir") == 0)
	{
		rmdir(arg);
		cout << "\nDirectory removed: " << arg << "\n";
	}

	else if (strcmp(cmd, "pwd") == 0)
	{
		cout << "\nPWD: " << getenv("PWD") << "\n";
	}

	else if (strcmp(cmd, "history") == 0)
	{
		if (strcmp(arg, "") == 0)
		{
			string cm;
			cout << "\nHistory:\n\n";
			ifstream fi;
			fi.open(wd.c_str(), ios::in);
			int i = 1;
			while (getline (fi, cm))
				cout << i++ << "\t" << cm << "\n";
			fi.close();	
		}
		else if (strcmp(arg, "-c") == 0)
			{
				ofstream fu;
				fu.open(wd.c_str(), ios::out);
				fu.close();
				cout << "\nHistory cleared.\n";
			}
		else
			cout << "\nAbusive use of history command.\nChoose option (A) and type help history for more info.\n";
	}

	else if (strcmp(cmd, "kill") == 0)
	{
		if (strcmp(arg, "") == 0)
			cout << "\nNo arguments provided to kill.\nChoose option (A) and type help kill for more info.\n";
		else
		{
			if (isalpha(arg[0]) || atoi(arg) < 0)
				cout << "\nInvald PID entered. Please enter a positive numeric number.\nChoose option (A) and type help kill for more info.\n";
			else
			{
				int pid = atoi(arg);
				kill (pid, SIGTERM);
				cout << "\nKilled PID: " << pid << "\n";
			}
		}
	}

	else if (strcmp(cmd, "help") == 0)
	{
		if (strcmp(arg, "cd") == 0)
		{
			cout << "\n\tcd: cd [dir]\n";
			cout << "\nChange the current directory to DIR.\nThe variable $HOME is the default DIR.";
			cout << "\nA null directory name is the same as the current directory, i.e. `.'.\n\n";
		}
		else if (strcmp(arg, "echo") == 0)
		{
			cout << "\n\techo: echo [arg]\n";
			cout << "Output the ARG.\n\n";
		}
		else if (strcmp(arg, "mkdir") == 0)
		{
			cout << "\n\tmkdir: mkdir [dir]\n";
			cout << "\nCreate a new directory named DIR with permissions 755.\n\n";
		}
		else if (strcmp(arg, "rmdir") == 0)
		{
			cout << "\n\trmdir: rmdir [dir]\n";
			cout << "\nRemove the directory named DIR.\n\n";
		}
		else if (strcmp(arg, "kill") == 0)
		{
			cout << "\n\tkill: kill pid | jobspec\n";
			cout << "\nSend the processes named by PID (or JOBSPEC) the signal SIGTERM.";
			cout << "\nKill is a shell builtin for two reasons:\n1. it allows job IDs to be used instead of process IDs, and,";
			cout << "\n2. if you have reached the limit on processes that you can create, you don't have to start a process to kill another one.\n\n";
		}
		else if (strcmp(arg, "help") == 0)
		{
			cout << "\n\thelp: help [pattern]\n";
			cout << "\nDisplay helpful information about builtin commands.\nIf PATTERN is specified, gives detailed help on all commands matching PATTERN,\n";
			cout << "otherwise a list of the builtins is printed.\n\n";
		}
		else if (strcmp(arg, "history") == 0)
		{
			cout << "\n\thistory: history [-c]\n";
			cout << "\nDisplay the history list with line numbers.\nThe `-c' option causes the history list to be cleared by deleting all of the entries.\n\n";
		}
		else if (strcmp(arg, "pwd") == 0)
		{
			cout << "\n\tpwd: pwd\n";
			cout << "\nPrint the current working directory.\n\n";
		}
		else
		{
			cout << "\n\t\tVishesh and Shikha's shell, version 1.0.0-release (x86_64-pc-linux-gnu)\n\nThese shell commands are defined internally.";
			cout << "\nType `help' to see this list.\nType `help name' to find out more about the function `name'.";
			cout << "\nUse `info bash' to find out more about the shell in general.";
			cout << "\nChoose option (B) in the menu and use `man -k' or `info' to find out more about commands not in this list.";
			cout << "\n\ncd\necho\nmkdir\nrmdir\npwd\nhistory\nkill\nhelp\n\n";
		}
	}

	else
		return false;

	return true;
}

string parser (string str, vector<string>& vec, char r)
{
	string sen, word = "";
	istringstream is;
	int i, l;

	l = str.size();
	for (i = 0; i < l; ++i)
	{
		if (str[i] == r)
		{
			is.str(word);
			while (is >> word)
				vec.push_back(word);
			word = "";
		}
		else
			word += str[i];
	}
	word += '\0';

	sen = "";
	i = 0;
	while ((word[i] == ' ') || (word[i] == '\t'))
		i++;
	while (word[i] != '\0')
		sen += word[i++];

	return sen;
}

int main()
{
	int i, x, l, r, j, p[2], si, so, no, ni;
	char ** arg, h, *sr, *lr;
	bool s;
	string str, word, sen;
	vector <string> vec, vec2;
	ofstream fp;

	wd = getenv ("PWD");
	wd = wd + "/.shell_history";

	while (1)
	{
		h = menu();
		scanf("%*c");

		if (h == 'G')
		{
			cout << "\nExiting shell.\n\n";
			return 0;
		}

		else
		{
			cout << "\n> ";
			getline (cin, str);
			fp.open(wd.c_str(), ios::app);
			fp << str << "\n";
			fp.close();
			vec.clear();
			vec2.clear();
			istringstream is(str);

			switch(h)
			{
				case 'A':	is >> word;
							vec.push_back(word);
							sen = "";
							if (is >> word)
								sen = word;
							while (is >> word)
								sen = sen + " " + word;
							vec.push_back(sen);
							s = internal (vec[0].c_str(), vec[1].c_str());
							if (!s)
								cout << "\nCommand not found.\nChoose option (A) and type 'help' to get a list of our shell buit-ins.\n";
							break;

				case 'B':	while (is >> word)
								vec.push_back(word);

							x = fork();

							if (x == 0)
							{
								l = vec.size();
								arg = new char * [l + 1];
								for (i = 0; i < l; ++i)
									arg[i] = (char *) vec[i].c_str();
								arg[i] = NULL;
								execvp (*arg, arg);

								cout << "\nIncorrect command\n";

								return 0;
							}

							waitpid(-1, NULL, WNOHANG);
							wait(NULL);

							break;

				case 'C':	sen = parser(str, vec, '<');

							x = fork();
							if (x == 0)
							{
								int f = open(sen.c_str(), O_RDONLY);
								if (f < 0)
								{
									cout << "\nError opening the file\n";
									return 0;
								}

								close(STDIN_FILENO);
								dup(f);

								l = vec.size();
								arg = new char * [l + 1];
								for (i = 0; i < l; ++i)
									arg[i] = (char *) vec[i].c_str();
								arg[i] = NULL;
								execvp (*arg, arg);

								cout << "Incorrect command\n";

								return 0;
							}

							waitpid(-1, NULL, WNOHANG);
							wait(NULL);
							break;
					
				case 'D':	sen = parser(str, vec, '>');
				
							x = fork();
							if (x == 0)
							{
								int f = open(sen.c_str(), O_CREAT|O_RDWR, 0666);
								if (f < 0)
								{
									cout << "\nError opening the file\n";
									return 0;
								}

								close(STDOUT_FILENO);
								dup(f);

								l = vec.size();
								arg = new char * [l + 1];
								for (i = 0; i < l; ++i)
									arg[i] = (char *) vec[i].c_str();
								arg[i] = NULL;
								execvp (*arg, arg);

								cout << "Incorrect command\n";

								return 0;
							}

							waitpid(-1, NULL, WNOHANG);
							wait(NULL);
							break;

				case 'E':	while (is >> word)
								vec.push_back(word);
							vec.pop_back();

							x = fork();

							if (x == 0)
							{
								l = vec.size();
								arg = new char * [l + 1];
								for (i = 0; i < l; ++i)
									arg[i] = (char *) vec[i].c_str();
								arg[i] = NULL;
								execvp (*arg, arg);

								cout << "Incorrect command\n";

								return 0;
							}

							break;

				case 'F': 	sr = strdup (str.c_str());
							lr = strtok (sr, "|");
							while (lr != NULL)
							{
								vec2.push_back(lr);
								lr = strtok (NULL, "|");								
							}
							r = vec2.size();

							j = 0;
							i = 0;
							sr = strdup(vec2[j].c_str());
							while(sr[i] != '\0')
							{
								word = "";
								while (((sr[i] == ' ') || (sr[i] == '\t')) && (sr[i] != '\0')) i++;
								while ((sr[i] != ' ') && (sr[i] != '\t') && (sr[i] != '\0'))
									word = word + sr[i++];
								if (strcmp(word.c_str(), "") != 0)
									vec.push_back(word);

							}

							l = vec.size();

							arg = new char * [l + 1];
							for (i = 0; i < l; ++i)
								arg[i] = (char *) vec[i].c_str();
							arg[i] = NULL;

							pipe(p);
							si = dup(0);
							so = dup(1);
							x = fork();

							if (x == 0)							
							{	
								dup2 (p[1], 1);
								close (p[1]);
								execvp (*arg, arg);

								cout << "Incorrect command\n";

								return 0;
							}
							
							waitpid(-1, NULL, WNOHANG);
							wait(NULL);	
							close(p[1]);
							ni = p[0];
							j++;

							while (j < r-1)
							{
								vec.clear();

								i = 0;
								sr = strdup(vec2[j].c_str());
								while(sr[i] != '\0')
								{
									word = "";
									while (((sr[i] == ' ') || (sr[i] == '\t')) && (sr[i] != '\0')) i++;
									while ((sr[i] != ' ') && (sr[i] != '\t') && (sr[i] != '\0'))
										word = word + sr[i++];
									if (strcmp(word.c_str(), "") != 0)
										vec.push_back(word);
								}
								l = vec.size();

								arg = new char * [l + 1];
								for (i = 0; i < l; ++i)
									arg[i] = (char *) vec[i].c_str();
								arg[i] = NULL;

								pipe(p);
								x = fork();

								if (x == 0)							
								{									
									dup2(ni, 0);
									close(ni);
									dup2(p[1], 1);
									close(p[1]);
									execvp (*arg, arg);

									cout << "Incorrect command\n";

									return 0;
								}

								waitpid(-1, NULL, WNOHANG);
								wait(NULL);

								ni = p[0];
								close(p[1]);
								j++;
							}
						
							vec.clear();

							i = 0;
							sr = strdup(vec2[j].c_str());
							while(sr[i] != '\0')
							{
								word = "";
								while (((sr[i] == ' ') || (sr[i] == '\t')) && (sr[i] != '\0')) i++;
								while ((sr[i] != ' ') && (sr[i] != '\t') && (sr[i] != '\0'))
									word = word + sr[i++];
								if (strcmp(word.c_str(), "") != 0)
									vec.push_back(word);							}

							l = vec.size();

							arg = new char * [l + 1];
							for (i = 0; i < l; ++i)
								arg[i] = (char *) vec[i].c_str();
							arg[i] = NULL;

							x = fork();

							if (x == 0)							
							{	
								dup2 (ni, 0);
								close (ni);
								execvp (*arg, arg);

								cout << "Incorrect command\n";

								return 0;
							}

							waitpid(-1, NULL, WNOHANG);
							wait(NULL);
							dup2 (si, 0);
							dup2 (so, 1);
							break;

				default:	cout << "\nWork under progress\n\n";
							break;
			}
		}
	}
}







