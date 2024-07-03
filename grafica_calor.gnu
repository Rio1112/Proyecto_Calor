# Configuración inicial
reset
set terminal pngcairo enhanced font 'Verdana,12'
set output 'mapa_calor.png'

# Títulos y etiquetas
set title 'Mapa de Calor - Transferencia de Calor'
set xlabel 'Eje Y'
set ylabel 'Eje X'

# Rangos y escalas
set xrange [0:40]  # Ajusta según el tamaño de tu matriz NxM
set yrange [0:50]

# Colormap y configuración de la paleta de colores
set palette rgbformula 33,13,10  # Colormap

# Estilo de la gráfica
set view map
unset key  # Sin leyenda

# Plot del mapa de calor
plot 'resultados.txt' matrix with image notitle
