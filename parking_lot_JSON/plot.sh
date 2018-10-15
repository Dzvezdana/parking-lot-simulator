#!/usr/bin/gnuplot -persist
stats "datafile.dat" u 2 name 'Y_'
set xdata time
set grid

set style line 1 \
    linecolor rgb '#0060ad' \
    linetype 1 linewidth 2 \
    pointtype 7 pointsize 1.5

set timefmt "%Y-%m-%d"

set terminal png size 950, 600

set format x "%d/%m/%Y"
set format y "%.0f"

set output "chart_1.png"
set title "Date and maximum number of cars"
set xlabel "Date" 
set ylabel "Max number of cars"
plot [:][:Y_max+3] 'datafile.dat' using 1:2 title "max cars" with linespoints linestyle 1
exit