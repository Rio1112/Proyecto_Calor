set terminal pngcairo size 800,600 enhanced font 'Verdana,10'

# Primer archivo
set output 'heatmap_1.png'
set title "Mapa de Calor - datos_ftcs.txt"
set xlabel "X"
set ylabel "Y"
set pm3d map
set palette defined (0 'black', 1 'red', 2 'yellow', 3 'white')
splot 'datos_ftcs.txt' matrix with image

# Segundo archivo
set output 'heatmap_2.png'
set title "Mapa de Calor - datos_ftcs_2.txt"
set xlabel "X"
set ylabel "Y"
set pm3d map
set palette defined (0 'black', 1 'red', 2 'yellow', 3 'white')
splot 'datos_ftcs_2.txt' matrix with image
