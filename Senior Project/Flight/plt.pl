#! usr/bin/perl

open (GNUPLOT,"| gnuplot -persist");

print GNUPLOT <<gnuplot_commands;
  plot "response.txt" using 1:1 title 'time' with lines, "response.txt" using 1:4 title 'throttle' with lines
gnuplot_commands
close(GNUPLOT);


open (GNUPLOT,"| gnuplot -persist");

print GNUPLOT <<gnuplot_commands;
  plot [4800:5600] "response.txt" using 1:1 title 'time' with lines, "response.txt" using 1:2 title 'ax' with lines, "response.txt" using 1:3 title 'ay' with lines, "response.txt" using 1:4 title 'throttle' with lines
gnuplot_commands
close(GNUPLOT);
