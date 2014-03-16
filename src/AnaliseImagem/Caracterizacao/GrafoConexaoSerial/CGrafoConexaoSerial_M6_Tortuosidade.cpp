/**
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
@Desenvolvido_por:
				Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author:        André Duarte Bueno
@file:          CGrafoConexaoSerial_M6_Tortuosidade.cpp
@begin:         Sat Sep 16 2000
@copyright:     (C) 2000 by André Duarte Bueno
@email:         andreduartebueno@gmail.com
*/
// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <cassert>
#include <cmath>
#include <sstream>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafoConexaoSerial_M6_Tortuosidade.h>

#ifndef CObjetoEsqueleto_Sitio_h
#include <EstruturaDados/ObjetoEsqueleto/CObjetoEsqueleto_Sitio.h>
#endif

#ifndef CObjetoEsqueleto_Sitio_WEST_h
#include <EstruturaDados/ObjetoEsqueleto/CObjetoEsqueleto_Sitio_WEST.h>
#endif

#ifndef CObjetoEsqueleto_Sitio_EST_h
#include <EstruturaDados/ObjetoEsqueleto/CObjetoEsqueleto_Sitio_EST.h>
#endif

#ifndef CObjetoEsqueleto_Final_h
#include <EstruturaDados/ObjetoEsqueleto/CObjetoEsqueleto_Final.h>
#endif

#ifndef CObjetoEsqueleto_Tipo_h
#include <EstruturaDados/ObjetoEsqueleto/CObjetoEsqueleto_Tipo.h>
#endif

#ifndef CRede_h
#include <EstruturaDados/CRede.h>
#endif

#ifndef CMath_h
#include <Base/CMath.h>
#endif
using namespace std;

// -------------------------------------------------------------------------
// Função:       CriarObjeto
// -------------------------------------------------------------------------
/**
@short  : Cria objeto herdeiro de CObjetoRede, de acordo com o tipo solicitado (CObjetoRede_CC_Sitio...).
@author : André Duarte Bueno
@see    : grafos
@param  : CContorno::ETipoContorno tipoContorno
@return : Retorna um ponteiro para um sítio novo
*/
CObjetoRede*
CGrafoConexaoSerial_M6_Tortuosidade::CriarObjeto ( ETipoObjetoGrafo tipoObjeto  )
{
// cerr<<"\nCriar Objeto->Tipo objeto=" << static_cast<uint16_t> (tipoObjeto) << endl;
// #ifdef OTIMIZAR_VELOCIDADE_PROCESSAMENTO
//          using value_type_objeto_esqueleto = CObjetoEsqueleto_Final;
// #else
//          using value_type_objeto_esqueleto = CObjetoEsqueleto;
// #endif

   CObjetoRede* data;

   switch ( tipoObjeto ) {
	   case ETipoObjetoGrafo::ObjetoEsqueleto_Sitio :
         data = new CObjetoEsqueleto_Sitio ();
         break;

      case ETipoObjetoGrafo::ObjetoEsqueleto_Sitio_WEST :
         data = new CObjetoEsqueleto_Sitio_WEST ();
         break;

      case ETipoObjetoGrafo::ObjetoEsqueleto_Sitio_EST :
         data = new CObjetoEsqueleto_Sitio_EST ();
         break;

      case ETipoObjetoGrafo::ObjetoEsqueleto_Final :
         data = new CObjetoRede_Final ( tipoObjeto );
         break;

      case ETipoObjetoGrafo::ObjetoEsqueleto_Tipo :
         data = reinterpret_cast<CObjetoRede*> ( new CObjetoEsqueleto_Tipo ( tipoObjeto ) );
         break;

	  default:
         data = new CObjetoEsqueleto_Sitio ();
         break;
      }
   assert ( data );
   return data;
}

// -------------------------------------------------------------------------
// Função     AdicionarObjetos
// -------------------------------------------------------------------------
/**
@short  :	Função que adiciona a lista de objetos do grafo, os objetos identificados em rotulador.
        Redefinida para inclusão das informações de cx,cy,cz
@author :	André Duarte Bueno
@see    :
@param  : Recebe a imagem rotulada com os objetos a serem incluídos (ra ou rp),
    o número do ultimo rótulo utilizado e o
    tipo de contorno (identifica o objeto a ser criado:
    CObjetoRede_Sitio_EST = 0, CSitioCentro = 1,  CObjetoRede_Sitio_EST = 2)
@return : void
*/
void
CGrafoConexaoSerial_M6_Tortuosidade::AdicionarObjetos
( CRotulador2DCm* rotulador, unsigned long int ultimoRotuloUtilizado,
  ETipoObjetoGrafo tipoObjeto )
{
   // Na classe CGrafoConexaoSerial_M3 a função Go()
   // seta o tipoObjeto = ETipoObjetoGrafo::ObjetoRede_Sitio_CENTER;
   // aqui quero criar objetos do tipo Esqueleto, então
   // tenho de mudar o tipo antes de chamar CriarObjeto(tipo).
   if ( tipoObjeto == ETipoObjetoGrafo::ObjetoRede_Sitio )
      tipoObjeto = ETipoObjetoGrafo::ObjetoEsqueleto_Sitio;
   else if ( tipoObjeto == ETipoObjetoGrafo::ObjetoRede_Sitio_WEST )
      tipoObjeto = ETipoObjetoGrafo::ObjetoEsqueleto_Sitio_WEST;
   else if ( tipoObjeto == ETipoObjetoGrafo::ObjetoRede_Sitio_EST )
      tipoObjeto = ETipoObjetoGrafo::ObjetoEsqueleto_Sitio_EST;
   else if ( tipoObjeto == ETipoObjetoGrafo::ObjetoRede_Final )
      tipoObjeto = ETipoObjetoGrafo::ObjetoEsqueleto_Final;
   else if ( tipoObjeto == ETipoObjetoGrafo::ObjetoRede_Tipo )
      tipoObjeto = ETipoObjetoGrafo::ObjetoEsqueleto_Tipo;

// cerr<<"\nAdicionar Objeto->Tipo objeto=" << static_cast<uint16_t> (tipoObjeto) << endl;

   // Calcula o centro de massa dos objetos da imagem rotulada (NOVO)
   rotulador->CentroMassaObjetos ();
// cerr<<"\nCalculou rotulador->CentroMassaObjetos ();" << endl; 

   // Não deve considerar o objeto 0 que é o fundo.
   // inclue o rotulo final, o objeto final
   for ( int rotulo = 1; rotulo <= rotulador->RotuloFinal (); rotulo++ ) {
         // Código antigo
         //// Ponteiro para objeto a ser criado
         //CObjetoRede*      data;
         //// Obtem um sítio novo passando o tipo
         //data = CriarObjeto ( tipoContornoObjeto );
         //assert ( data );

         // Código novo
         // Dentro de CRede, value_type_objeto esta setada para criar objeto do tipo CObjetoRede
         // ou CObjetoRede_Tipo(tipo); depende da variável de pré-processamento
         // OTIMIZAR_VELOCIDADE_PROCESSAMENTO;
         // mas aqui quero criar CObjetoEsqueleto_Sitio ou ObjetoEsqueleto_Final, então uso
         // #idef OTIMIZAR_VELOCIDADE_PROCESSAMENTO para setar corretamente o tipo de ponteiro.
         // Lembrando que ObjetoEsqueleto_Final é +rápida pois não tem polimorfismo.
//          value_type_objeto_esqueleto * data = nullptr;
//          data = static_cast< value_type_objeto_esqueleto * > ( CriarObjeto ( tipoObjeto ) );
//         data = dynamic_cast< value_type_objeto_esqueleto * > ( CriarObjeto ( tipoObjeto ) );
#ifdef OTIMIZAR_VELOCIDADE_PROCESSAMENTO
         using value_type_objeto_esqueleto = CObjetoEsqueleto_Final;
// cerr << "debug1: using value_type_objeto_esqueleto = CObjetoEsqueleto_Final;\n";
#else
         using value_type_objeto_esqueleto = CObjetoEsqueleto;
// cerr << "debug2: using value_type_objeto_esqueleto = CObjetoEsqueleto;\n";
#endif
//          value_type_objeto_esqueleto * data = nullptr;
// cerr<<"\nAntes dynamic_cast< value_type_objeto_esqueleto * > ( CriarObjeto ( tipoObjeto ) );" << endl; 
// 		 data = dynamic_cast< value_type_objeto_esqueleto * > ( CriarObjeto ( tipoObjeto ) );
 value_type_objeto_esqueleto * data = nullptr;
 data = static_cast<value_type_objeto_esqueleto *> ( CriarObjeto ( tipoObjeto ) );
	   // Obtem um novo objeto
         assert ( data );

         // No rotulador o objeto 0 é o fundo
         // como rotulo esta iniciando em 1, faço -1
         // para que o primeiro sitio tenha rotulo 0.
         data->rotulo = rotulo - 1 + ultimoRotuloUtilizado;

         // Propriedade raio hidraulico
         data->propriedade = rotulador->RaioHidraulicoObjetos ( rotulo );

         // Passa para x o plano atual (sera usado na estimacao de x)
         data->x = this->plano;

		// Adiciona a posição do centro de massa, seta cx,cy,cz de cada sítio
// cerr<<"\nAntes data->cx = rotulador->CMXObjeto ( rotulo );" << endl; 
         data->cx = rotulador->CMXObjeto ( rotulo );
         data->cy = rotulador->CMYObjeto ( rotulo );
         data->cz = plano;

         // Insere o objeto criado a lista de objetos do grafo
         objeto.push_back ( data );
      }
}

// -------------------------------------------------------------------------
// Função:   CalcularCondutancias
// -------------------------------------------------------------------------
/**
@short  : Redefinida, em relação a CGrafo.
        Adiciona o cálculo das condutâncias das ligações
@author :	André Duarte Bueno
@see    : Condutância
@param  : nada
@return : void
*/
void CGrafoConexaoSerial_M6_Tortuosidade::CalcularCondutancias ( long double _viscosidade, long double _dimensaoPixel, unsigned long int _fatorAmplificacao )
{
// ***********NOVO CALCULO TORTUOSIDADE*******
   tortuosidade = 0.0;
   numeroDerivacoesUsadasCalculoTortuosidade = 0;
// FIM NOVO*********

   ofstream saida ( ( NomeGrafo() + ".fatorCorrecaoDistancias" ).c_str() );
   saida << "fatorCorrecaoDistancias(desviosLaterais) e fatorCorrecaoDistanciasLimitado a 2\n";

   // Chama função da classe base que calcula as condutâncias
   CGrafoConexaoSerial_M3::CalcularCondutancias ( _viscosidade, _dimensaoPixel, _fatorAmplificacao );

   // Inicio do cálculo da correção das condutâncias
   // Ponteiro para sitio derivado

   // Distancia dx entre os dois sítios
   double     dx = 0.0;
   double     dy = 0.0;

   // Fator de correção da condutancia (distancia total entre os dois sitios)
   double   fatorCorrecaoDistancias = 0.0;

   // Percorre  todos os objetos do  grafo
   for ( unsigned long int k = 0; k < objeto.size (); k++ ) {
         // Converte o ponteiro ObjetoGrafo para CObjetoEsqueleto, para ter acesso ao vetor condutancia[link] e cx,cy,cz
#ifdef OTIMIZAR_VELOCIDADE_PROCESSAMENTO
         using value_type_objeto_esqueleto = CObjetoEsqueleto_Final;
cerr << "debug1: using value_type_objeto_esqueleto = CObjetoEsqueleto_Final;\n";
#else
         using value_type_objeto_esqueleto = CObjetoEsqueleto;
cerr << "debug2: using value_type_objeto_esqueleto = CObjetoEsqueleto;\n";
#endif		
        value_type_objeto_esqueleto * sitio = nullptr;
		sitio = dynamic_cast< value_type_objeto_esqueleto * > ( objeto[k] );
        assert ( sitio );

         // Obtêm a informação do cmx e cmy do sitio atual (k)
         double cmxSitio = sitio->cx;
         double cmySitio = sitio->cy;

         // Percorre todas as conexões do sitio atual
//          CObjetoEsqueleto*  sitioConexo = nullptr;

         for ( unsigned int link = 0; link < sitio->conexao.size (); link++ ) {
			   value_type_objeto_esqueleto * sitioConexo = nullptr;
			   sitioConexo = dynamic_cast< value_type_objeto_esqueleto * > ( objeto[k] );
               assert ( sitioConexo );

               // Recupera a informação  do centro de massa na direção x, do sitio conexo
               double  cmxSitioConexo = sitioConexo->cx;
               double  cmySitioConexo = sitioConexo->cy;

               // Determina a distância dx e dy entre o sítio e o sitio conexo
               // Correção, como faz dx*dx, não precisa achar módulo!
               dx = cmxSitio - cmxSitioConexo ;
               dy = cmySitio - cmySitioConexo ;

               // Calcula o fator de correção da condutancia em função  da distância entre os sitios
               fatorCorrecaoDistancias = sqrt ( 1.0 + dx * dx + dy * dy );
               saida << setw ( 10 ) << fatorCorrecaoDistancias << ' ';
               // Corrige a condutancia, considerando o fator de correção

               // NOVO NO MODELO 6 EVITA EXCESSO DE CORRECAO
               // Este fator precisa ser melhor definido, pois é arbitrário; em alguns casos piora valor condutancia.
               if ( fatorCorrecaoDistancias < 2.0 ) {
                     sitio->condutancia[link] /= fatorCorrecaoDistancias;
                  }
               else {
                     sitio->condutancia[link] /= 2.0;
                  }

               saida << setw ( 10 ) << fatorCorrecaoDistancias << '\n';

               // ***********NOVO CALCULO TORTUOSIDADE*******
               // Para cálculo da tortuosidade vai acumular as distâncias laterais e o número de derivações
               // (número de conexões); depois vai dividir pelo numeroDerivacoesUsadasCalculoTortuosidade.
               tortuosidade += fatorCorrecaoDistancias;    // acumula as distâncias
               numeroDerivacoesUsadasCalculoTortuosidade++;// acumula número conexões
               // FIM NOVO*********
            }			// for link
      }				// for k

   // ***********NOVO CALCULO TORTUOSIDADE*******
   tortuosidade /= numeroDerivacoesUsadasCalculoTortuosidade;// corrige a tortuosidade

   ofstream arq_tortuosidade ( ( NomeGrafo() + ".tortuosidade" ).c_str() );
   arq_tortuosidade << "A tortuosidade da imagem " << nomeArquivo << " é : " << tortuosidade << "\n";
   arq_tortuosidade.close();

   cout << "\nA tortuosidade da imagem " << nomeArquivo << " é : " << tortuosidade << "\n";
   // FIM NOVO*********

   saida.close ();
   return;
}

// -------------------------------------------------------------------------
// Função:   CalcularTortuosidade
// -------------------------------------------------------------------------
/**
@short  : Nova, adiciona CalcularTortuosidade sem necessidade calccular condutâncias.
@author :	André Duarte Bueno
@see    : Condutância
@param  : nada
@return : void
*/
long double CGrafoConexaoSerial_M6_Tortuosidade::CalcularTortuosidade ()
{
   tortuosidade = 0.0;
   numeroDerivacoesUsadasCalculoTortuosidade = 0;

   ofstream arq_tortuosidade ( ( NomeGrafo() + ".tortuosidade" ).c_str() );
   arq_tortuosidade << "fatorCorrecaoDistancias(desvios laterais entre centros de massa dos objetos conectados)\n";

   // Inicio do cálculo da correção das condutâncias
   // Ponteiro para sitio derivado
   CObjetoEsqueleto*     sitio = nullptr;

   // Distancia dx entre os dois sítios
   double     dx = 0.0;
   double     dy = 0.0;

   // Fator de correção da condutancia (distancia total entre os dois sitios)
   double   fatorCorrecaoDistancias = 0.0;

   if ( objeto.size () == 0 ) {
         cerr << "\n Chamou CalcularCondutancias antes de executar Go, ou seja, "
              << "ainda não montou o grafo; valor de tortuosidade indefinido!\n";
         arq_tortuosidade << "\n Chamou CalcularCondutancias antes de executar Go, ou seja, "
                          << "ainda não montou o grafo; valor de tortuosidade indefinido!\n";
		arq_tortuosidade.close();
         return 0.0;
      }

   // Percorre  todos os objetos do  grafo
   for ( unsigned long int k = 0; k < objeto.size (); k++ ) {
         // Converte o ponteiro ObjetoGrafo para CObjetoEsqueleto, para ter acesso ao vetor condutancia[link] e cx,cy,cz
         sitio = dynamic_cast < CObjetoEsqueleto* > ( objeto[k] );
         assert ( sitio );

         // Obtêm a informação do cmx e cmy do sitio atual (k)
         double cmxSitio = sitio->cx;
         double cmySitio = sitio->cy;

         // Percorre todas as conexões do sitio atual
         CObjetoEsqueleto*  sitioConexo = nullptr;

         for ( unsigned int link = 0; link < sitio->conexao.size (); link++ ) {
               sitioConexo = dynamic_cast < CObjetoEsqueleto* > ( sitio->conexao[link] );
               assert ( sitioConexo );

               // Recupera a informação  do centro de massa na direção x, do sitio conexo
               double  cmxSitioConexo = sitioConexo->cx;
               double  cmySitioConexo = sitioConexo->cy;

               // Determina a distância dx e dy entre o sítio e o sitio conexo
               // Correção, como faz dx*dx, não precisa achar módulo!
               dx = cmxSitio - cmxSitioConexo ;
               dy = cmySitio - cmySitioConexo ;

               // Calcula o fator de correção da condutancia em função  da distância entre os sitios
               fatorCorrecaoDistancias = sqrt ( 1.0 + dx * dx + dy * dy );
               arq_tortuosidade << setw ( 10 ) << fatorCorrecaoDistancias << '\n';
               // Corrige a condutancia, considerando o fator de correção

               // Para cálculo da tortuosidade vai acumular as distâncias laterais e o número de derivações
               // (número de conexões); depois vai dividir pelo numeroDerivacoesUsadasCalculoTortuosidade.
               tortuosidade += fatorCorrecaoDistancias;    // acumula as distâncias
               numeroDerivacoesUsadasCalculoTortuosidade++;// acumula número conexões
               // FIM NOVO*********
            }			// for link
      }				// for k

   tortuosidade /= numeroDerivacoesUsadasCalculoTortuosidade;// corrige a tortuosidade

   arq_tortuosidade << "\nA tortuosidade da imagem " << nomeArquivo << " é : " << tortuosidade << "\n";
   arq_tortuosidade.close();

   cout << "\nA tortuosidade da imagem " << nomeArquivo << " é : " << tortuosidade << "\n";

   return tortuosidade;
}
