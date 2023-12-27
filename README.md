
# Table of Contents

1.  [Título: Biblioteca lib<sub>ldsc</sub>](#orga203df2)
2.  [Versão:](#orgbc15af6)
3.  [Data:](#org08aafc1)
4.  [Autor de Contato:](#org9981ecc)
5.  [O que é a libldsc?](#org580e23d)
    1.  [A libldsc é uma biblioteca de objetos para solução de problemas físicos da área de:](#org471839c)
    2.  [A libldsc é totalmente desenvolvida em ANSI C++](#org3b15c77)
6.  [O que você vai encontrar? (diretórios da biblioteca)](#org09be4b9)
7.  [Vantagens:](#orgf809062)
8.  [Desvantagens:](#org83a6c94)
9.  [Outros arquivos de ajuda (disponibilizados no repositório)](#org8843896)


<a id="orga203df2"></a>

# Título: Biblioteca lib<sub>ldsc</sub>

-   Link no github:
    -   <https://github.com/ldsc/lib_ldsc>


<a id="orgbc15af6"></a>

# Versão:

-   1.0 André Duarte Bueno.
-   2.0 Leandro Puerari.


<a id="org08aafc1"></a>

# Data:

-   ~2013.


<a id="org9981ecc"></a>

# Autor de Contato:

-   André Duarte Bueno
-   [andreduartebueno@gmail.com](mailto:andreduartebueno@gmail.com)
-   [bueno@lenep.uenf.br](mailto:bueno@lenep.uenf.br)
-   Colaboração: Leandro Puerari.


<a id="org580e23d"></a>

# O que é a libldsc?

-   A libldsc tem um conjunto de objetos relacionados a análise de imagens de meios porosos, a solução de problemas de transferência de calor e umidade, a solução de problemas matemáticos, estatísticos e numéricos básicos.
-   O [Software LVP](https://github.com/ldsc/lvp) faz uso extensivo da lib<sub>ldsc</sub>.
-   Manual técnico: [Tese doutorado Prof. André Duarte Bueno](https://github.com/ldsc/lib_ldsc/blob/master/doc/ManualTecnico/Doutorado-AndreDuarteBueno.pdf)
-   Apresentação:  [Apresentação linhas de pesquisa que usam a lib<sub>ldsc</sub>](https://github.com/ldsc/lib_ldsc/blob/master/doc/ManualTecnico/ApresentacaoLinhaPesquisaAnaliseDeImagens-ProcParalelo-Bueno-2023-reduzida.pdf)


<a id="org471839c"></a>

## A libldsc é uma biblioteca de objetos para solução de problemas físicos da área de:

-   Processamento de imagens.
-   Meios porosos (rochas reservatório).
-   Métodos numéricos.
-   Grafos, redes de percolação e redes neurais.


<a id="org3b15c77"></a>

## A libldsc é totalmente desenvolvida em ANSI C++

-   O que significa que pode ser utilizada em qualquer ambiente operacional (DOS, Windows, MAC, UNIX,LINUX, ..) que suporte a linguagem de programação  ANSI C++.
-   A libldsc usa os mais modernos conceitos de programação orientada a objeto  (como polimorfismo, templates e STL).
-   Nota:	Partes da biblioteca usam conceitos antigos, a biblioteca esta sendo atualizada aos poucos.


<a id="org09be4b9"></a>

# O que você vai encontrar? (diretórios da biblioteca)

-   Base
    -   Algumas classes base.
-   Tempo
    -   Tempo de processamento.
-   Material
    -   Materiais como fluido, sólido, meio poroso.
-   Matriz
    -   Vetores e matrizes.
-   Rotulador
    -   Rotulagem 2D e 3D (Hoshen e Kopelman).
-   Mascara
    -   Máscaras de chanfro para processamento de imagens.
-   Filtro
    -   Filtros para processamento de imagens.
-   Filtro/FAmplitude
    -   Filtros de amplitude.
-   Filtro/FEspacial
    -   Filtros espaciais (DeteccaoContorno, MorfologiaMatematica, Esqueleto).
-   Contorno
    -   Identificam o tipo de contorno (condições de contorno).
-   Funcao
    -   Funções matemáticas usuais, e funções necessárias para os métodos utilizados/desenvolvidos.
-   Estatistica
    -   Métodos estatísticos básicos.
-   Integral
    -   Métodos de integração.
-   SFuncao
    -   Solver para funções (zeros).
-   Geometria
    -   Objetos geométricos (bolas discretas).
-   Interpolacao
    -   Métodos de interpolação.
-   FFT
    -   Transformada de FOURIER.
-   Correlacao
    -   Correlação, autocorrelação.
-   SMatriz
    -   Solver para matrizes.
-   Permeabilidade
    -   Cálculo da permeabilidade.
-   Grafo
    -   Grafo de conecção tridimensional.
-   Reconstrucao
    -   Reconstrução 3D.
-   ConEqu
    -   Configurações de equilíbrio.


<a id="orgf809062"></a>

# Vantagens:

-   Ao adotar um padrão ANSI C++, garante-se a portabilidade da libldsc  nas mais diversas
-   Sendo ANSI C++, novos programadores ou antigos programadores de C++ poderão utilizar rapidamente os objetos da libldsc sem nenhum estudo adicional sobre a interface gráfica.
-   Adicionalmente, seu uso fica independente de bibliotecas de interface gráfica (como a COI-LIB, a OWL, VCL, MFC, QT&#x2026;).
-   Caso o programador esteja usando uma destas bibliotecas gráficas, basta adicionar a libldsc  (e os arquivos de cabeçalho \*.h) e utilizar diretamente os objetos da libldsc. Podendo, adicionalmente, criar quadros de diálogo para fácil manipulação dos objetos da libldsc.


<a id="org83a6c94"></a>

# Desvantagens:

-   Caso o programador queira utilizar em um ambiente grafico, devera criar mecanismos (quadros de diálogo, box de entrada de dados, entre outros) para manipulação dos objetos da libldsc.


<a id="org8843896"></a>

# Outros arquivos de ajuda (disponibilizados no repositório)

-   README
    -   Descreve o que é a libldsc e os arquivos de ajuda.
-   INSTALL
    -   Descreve como instalar a libldsc.
-   HOWTO
    -   Descreve como utilizar a libldsc.
-   NEWS
    -   Descreve as novidades das últimas versões.
-   ORGANIZATION
    -   Descreve como modificar a libldsc (informações para desenvolvedores - programadores).
-   TODO
    -   Lista das tarefas a serem realizadas.
-   BUGS
    -   Lista de bugs relatados ou identificados.
-   ChangeLog
    -   Lista de modificações da biblioteca.
-   COPYING
    -   Descreve as condições para cópia e instalação da libldsc e a licença pública da GNU (Versão 2).
-   AUTHORS
    -   Lista dos autores da libldsc.

