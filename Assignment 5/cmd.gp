set terminal pdf
set output 'LOR.pdf'
set xlabel 'Time'
set ylabel 'Page Accessed'
set title 'Locality of Reference'
set key off
plot 'input_file.txt' using ($0):2 with linespoints
exit