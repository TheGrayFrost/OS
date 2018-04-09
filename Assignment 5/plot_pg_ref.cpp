#include <bits/stdc++.h>
int main()
/*{
	system("echo \"set terminal pdf\n"
	"set output \'../Outputs/Scheduler\\ Comparison.pdf\'\n"
	"set xlabel \\\"number of processes\\(N\\)\\\"\n"
	"set ylabel \\\"Average Turn around time\\(atn\\)\\\"\n"
	"set title \\\"Comparision of various CPU Schedulers\\\"\n"
	"plot \'../Outputs/RR3.dat\' title \\\"RR\\(tq = 5\\)\\\" with linespoints ls 8;\" | gnuplot");
	return 0;
}*/
{
	system("echo \"set terminal pdf\n"
	"set output \'../Outputs/Scheduler\\ Comparison.pdf\'\n"
	"set xlabel \\\"Line\\(N\\)\\\"\n"
	"set ylabel \\\"Page Number\\(\\)\\\"\n"
	"set title \\\"Comparision of various CPU Schedulers\\\"\n"
	"plot \'input\' using 0 : 3 title \\\"page_number\\(tq = 5\\)\\\" with linespoints ls 8;\" | gnuplot");
	return 0;
}