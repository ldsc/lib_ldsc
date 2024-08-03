# Configurações iniciais
set terminal pngcairo size 800,600
set output 'grafico.png'
set title "Dados em Tempo Real"
set xlabel "Tempo"
set ylabel "Valor"

# Loop principal
do for [i=1:100] {
  # Abre o arquivo (ajuste o nome do arquivo e o modo de abertura)
  set datafile separator ","
  #plot '$1' using 1:2 with lines
  plot "Berea500-gt2-n1-np60-N300-lp_Encrustrada_MScale_Permeabilidades.dat" using 1:2 w lp

  # Limpa a tela e salva o gráfico
  #set output
  pause 10
}
#plot "Berea500-gt2-n1-np60-N300-lp_Encrustrada_MScale_Permeabilidades.dat" using 1:2 w lp