#include <cmath>
#include <AnaliseImagem/Filtro/Mascara/CMSobelHorizontal.h>
#include <AnaliseImagem/Filtro/Mascara/CMSobelVertical.h>
#include <AnaliseImagem/Filtro/FEspacial/CFESobel.h>

/*
----------------------------------------------------------------------------
PROJETO:    Anaimp.
      Analise de Imagens de Meios Porosos.
----------------------------------------------------------------------------

Desenvolvido por: Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:    Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:    Todos os direitos reservados.
Nome deste arquivo: TFGaussiano.cpp
Nome da classe:      TFGaussiano
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/*
----------------------------------------------------------------------------
      BIBLIOTECAS
----------------------------------------------------------------------------
*/


TCMatriz2D<int>* CFESobel::Go (TCMatriz2D<int>* &matriz, unsigned int _tamanhoMascara) {
 tamanhoMascara = _tamanhoMascara;
  CriaMascara (tamanhoMascara); // Cria a mascara adequada

  // a matriz deve ser uma imagem tons de cinza
  this->pm = matriz;
  TCMatriz2D<int> *cImg = new TCMatriz2D<int> (*this->pm); //matriz copia da original

  int i, j, k, l;     // variáveis auxiliares

  int soma;
  int sum_y, sum_x;

  // Percorre a matriz imagem
  for (i = 0; i < this->pm->NX(); i++) {      //linha
    for (j = 0; j < this->pm->NY(); j++) {    //coluna
      sum_x = 0;
      sum_y = 0;

      //ignora as bordas
      if (i == 0 || i == this->pm->NX() - 1)
        soma = 0;
      else if (j == 0 || j == this->pm->NY() - 1)
        soma = 0;
      else {
        for (k = -1; k <= 1; k++) {
          for (l = -1; l <= 1; l++) {
            int piX = l + i;
            int piY = k + j;

            sum_x = sum_x + cImg->data2D[piX][piY] * mask->data2D[l + 1][k + 1]; 
            sum_y = sum_y + cImg->data2D[piX][piY] * mask_2->data2D[l + 1][k + 1]; 
          }
        }

        soma = abs(sum_x) + abs(sum_y);
      }

      if (soma > 255)
        soma = 255;
      if (soma < 0)
        soma = 0;

      int novo_pixel = (255 - (unsigned char)(soma));
      this->pm->data2D[i][j] = novo_pixel;
    }
  }

  delete cImg;
  return this->pm;
}

void CFESobel::CriaMascara (unsigned int _tamanhoMascara) {
  tamanhoMascara = _tamanhoMascara;
  if (mask)
    delete mask;
  if (mask_2)
    delete mask_2;
  mask = new CMSobelHorizontal (_tamanhoMascara);  // Cria a mascara
  mask_2 = new CMSobelVertical (_tamanhoMascara);  // Cria a segunda mascara
}
