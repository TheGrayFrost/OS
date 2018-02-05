#include <bits/stdc++.h>
int main()
{
	system("echo \"set terminal pdf\n"
	"set output \'Ass3a_schedulers-comp-plot.pdf\'\n"
	"set xlabel \\\"number of processes\\(N\\)\\\"\n"
	"set ylabel \\\"Average Turn around time\\(atn\\)\\\"\n"
	"set title \\\"Comparision of various CPU Schedulers\\\"\n"
	"plot \'fcfs.dat\' title \\\"FCFS\\\" with linespoints ls 1,"
	"\'sjf.dat\' title \\\"PSJF\\\" with linespoints ls 2,"
	"\'rr1.dat\' title \\\"RR\\(tq = 1\\)\\\" with linespoints ls 5,"
	"\'rr2.dat\' title \\\"RR\\(tq = 2\\)\\\" with linespoints ls 7,"
	"\'rr3.dat\' title \\\"RR\\(tq = 5\\)\\\" with linespoints ls 8;\" | gnuplot");
}