set palette rgbformulae 33,13,10
set border 15 front linetype -1 linewidth 1.000
set logscale z 10
set view map
set isosamples 60, 60
unset surface
set contour base
set title "Himmelblau"
set xlabel "x"
set xrange [*:*] noreverse nowriteback
set ylabel "y"
set zlabel ""
set yrange [*:*] noreverse nowriteback
set zrange [*:*] noreverse nowriteback
set cntrparam levels 30
set surface
splot [-10.5:10.5] [-10.5:10.5] \
      (x**2 + y - 11)**2 + (x + y**2 - 7)**2 with lines palette notitle nosurface, \
      '-' with lines lc "#000000" notitle
2.0 5.0 1
9.0 2.0 1
-3.0 -8.0 1
2.0 5.0 1
e
