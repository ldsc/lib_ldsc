/**
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
				  Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author:          Andre Duarte Bueno
@file:            CGrafoConexaoSerial_M5.cpp
@begin:           Sat Sep 16 2000
copyright:        (C) 2000 by Andre Duarte Bueno
@email:           andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafoConexaoSerial_M5.h>

static unsigned long int totalPixeisDeletados = 0;

// -------------------------------------------------------------------------
// Funcao     Go
// -------------------------------------------------------------------------
CRede*
CGrafoConexaoSerial_M5::Go ( TCMatriz3D<int>* _img3D, unsigned long int funcao )
{
   // no modelo 5 usa informacoes do plano anterior e posterior,
   // obtem dados da imagem tridimensional
   totalPixeisDeletados = 0;
   img3D = _img3D;
   return CGrafoConexaoSerial_M4::Go ( _img3D, funcao );
}

// -------------------------------------------------------------------------
// Funcao     AdicionarObjetos (redefinida)
// -------------------------------------------------------------------------
/** @short  Funcao que adiciona ao grafo os objetos deste plano. Redefinida.
 * Novo no Mod5
 * Elimina os sítios isolados. Sítios que estao no plano atual rp
 * e nao tem conexão com o plano anterior (ra) e posterior (img3D[i][j][ plano + 1 ]).
 * De forma que o cálculo das distâncias reais entre os sítios
 * vai ser mais correto no que se refere ao escoamento.
 * Note que não recalcula área, perímetro nem raio RaioHidraulicoObjetos.
 * img3D -> ponteiro para imagem 3D,
 * plano -> valor de z,
 * rotulador, é o rotulador passado, vai ser sempre rp
 * a nao ser no caso dos planos z=0 e z=nz.
 * @author :	Andre Duarte Bueno
 * @see    :
 * @param  : Recebe a imagem rotulada com os objetos a serem incluídos,
 *	o número do ultimo rótulo utilizado e o	tipo  objeto.
 * @return : void
*/
void CGrafoConexaoSerial_M5::AdicionarObjetos ( CRotulador2DCm* rotulador,
  unsigned long int ultimoRotuloUtilizado,  ETipoObjetoGrafo tipoObjeto )
{
   // Chama função da classe base que adiciona os objetos;
   // considera area/perimetro como no CGrafoConexaoSerial.
   CGrafoConexaoSerial::AdicionarObjetos ( rotulador, ultimoRotuloUtilizado, tipoObjeto );

   // Se nao for o primeiro plano da imagem 3D nem o último, entra
   if ( plano > 0 && plano < ( img3D->NZ () - 1 ) ) {
         // cout<<" entrou "<<endl;         
	   arqpixeisDeletados << "\n";

         // Precisa percorrer o plano bidimensional
         // ra rotulador para plano anterior
         // rp rotulador para plano atual
         // img3D->data3D[ii][jj][plano + 1] Acesso ao plano posterior
         for ( unsigned long int ii = 0; ii < rp->NX (); ii++ ) {
               // cout << " ii=" << ii<<endl;
               for ( unsigned long int jj = 0; jj < rp->NY (); jj++ ) {
                     // cout << " jj=" << jj <<endl;
                     if (
                        // Se esta presente na posição ii,jj,plano
                        ( rp->data2D[ii][jj] == 1 ) &&
                        // e tem intersecção nula com o plano anterior
                        ( ra->data2D[ii][jj] == 0 ) &&
                        // e tem interseção nula com plano posterior
                        ( img3D->data3D[ii][jj][plano + 1] == 0 ) )
                        // Apaga o píxel atual
                        {
                           rp->data2D[ii][jj] = 0;
                           totalPixeisDeletados++;
                           arqpixeisDeletados << "\nPíxel Deletado (" << jj << "," << ii << "," << plano << ")";
                        }

                     //cout << "\nPíxel Deletado (" << ii << "," << jj << "," << plano << ")";
                     arqpixeisDeletados << rp->data2D[jj][ii] << " ";
                  }
               arqpixeisDeletados << "\n";
            }
      }

   cout            << "\nTotal de pixeis deletados=" << totalPixeisDeletados << endl;
   arqpixeisDeletados << "\nTotal de pixeis deletados=" << totalPixeisDeletados << endl;

   cout            << "\nPorosidade eliminada de " << ( totalPixeisDeletados
                   / ( img3D->NX() *img3D->NY() *img3D->NZ() ) )  << endl;
   arqpixeisDeletados << "\nPorosidade eliminada de " << ( totalPixeisDeletados
                   / ( img3D->NX() *img3D->NY() *img3D->NZ() ) )  << endl;
   // --------------------------------------------------------------
   // Calcula o centro de massa dos objetos da imagem rotulada
  rotulador->CalculaAreaObjetos () ; //atualiza vetor áreas
  rotulador->CalculaPerimetroObjetos (); //atualiza perímetros
  rotulador->CentroMassaObjetos ();

   // Percorre todos os objetos novos deste plano (rotulador)
   for ( unsigned long int cont = 1; cont <= rotulador->RotuloFinal ();
         cont++ ) {
         // Adiciona ao vetor cmx a informação do cmx do objeto (long double)
         cmx.push_back ( rotulador->CMXObjeto ( cont ) );
         cmy.push_back ( rotulador->CMYObjeto ( cont ) );
         cmz.push_back ( plano );
      }
}
