#include <bits/stdc++.h>
int main()
{
	system("echo \"set terminal pdf\n"
	"set output \'../Outputs/Scheduler\\ Comparison.pdf\'\n"
	"set xlabel \\\"number of processes\\(N\\)\\\"\n"
	"set ylabel \\\"Average Turn around time\\(atn\\)\\\"\n"
	"set title \\\"Comparision of various CPU Schedulers\\\"\n"
	"plot \'../Outputs/FCFS.dat\' title \\\"FCFS\\\" with linespoints ls 1,"
	"\'../Outputs/P-SJF.dat\' title \\\"PSJF\\\" with linespoints ls 2,"
	"\'../Outputs/RR1.dat\' title \\\"RR\\(tq = 1\\)\\\" with linespoints ls 5,"
	"\'../Outputs/RR2.dat\' title \\\"RR\\(tq = 2\\)\\\" with linespoints ls 7,"
	"\'../Outputs/RR3.dat\' title \\\"RR\\(tq = 5\\)\\\" with linespoints ls 8;\" | gnuplot");
	return 0;
}