#ifndef CGra3Dby2D_M4_h
#define CGra3Dby2D_M4_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:	
            Laboratorio de Desenvolvimento de Software Cientifico 	
            [LDSC].
@author     André Duarte Bueno
@file       CGra3Dby2D_M4.h
@begin      Oct 20 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <vector>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

#ifndef CGra3Dby2D_M3_h
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGra3Dby2D_M3.h>
#endif

// ===============================================================================
// Documentacao Classe: CGra3Dby2D_M4
// ===============================================================================
 /** 
 * @brief Acrescenta a classe base o conceito de correção das distâncias.
  * A informacao dos centros de massa é armazenada na propria
  * classe (this). Esta solucao foi adotada por permitir
  * a destruicao dos vetores cmx e cmy apos o calculo
  * das condutancias.
  * 
  * @author       André Duarte Bueno	
  * @see	     Grafo
 * Superclasse:    CGrafo -> CGra3Dby2D -> CGra3Dby2D_M2 -> CGra3Dby2D_M4
*/
class CGra3Dby2D_M4:public CGra3Dby2D_M3
{
// --------------------------------------------------------------Atributos
public:
  /// Vetor de float para armazenar a informação do cmx de cada sitio do grafo
  std::vector < float >cmx;
  /// Vetor de float para armazenar a informação do cmy de cada sitio do grafo
  std::vector < float >cmy;
  /// Vetor de float para armazenar a informação do cmz de cada sitio do grafo
  std::vector < float >cmz;

public:
// -------------------------------------------------------------Construtor
/// Construtor
CGra3Dby2D_M4 (std::string _nomeArquivo):CGra3Dby2D_M3 (_nomeArquivo)  {
	  tipoGrafo  =  ETipoGrafo::grafo3DBy2D_M4;
  }

// --------------------------------------------------------------Destrutor
/// Destrutor
  virtual ~ CGra3Dby2D_M4 () = default;

  /// Adiciona a saida dos centros de massa
  virtual void Write (std::string nomeArquivo) override;

  // Adiciona o calculo dos centros de massa
  // virtual void  CalcularCentroMassa();

// ----------------------------------------------------------------Métodos

protected:
  /**
   * @brief Calcula a condutancia conforme na classe anterior, 
   * e depois corrige levando em conta o centro de massa dos objetos
   * */
  virtual void CalcularCondutancias (long double _viscosidade,
				    long double _dimensaoPixel,
				    unsigned long int _fatorAmplificacao) override;

  /// Depois de criar os objetos, determina e armazena seus centro de massa
  virtual void AdicionarObjetos
    (CRotulador2DCm * rotulador, unsigned long int ultimoRotuloUtilizado, 
	 CContorno::ETipoContorno tipoContorno) override;

  // A funcao void CGra3Dby2D::EliminarObjetosRedundantes()
  // elimina os ramos mortos(grupos de objetos CSitio),
  // e reordena o vector de objetos.

  /// Vai reorganizar os vetores cmx e cmy,considerando os sitios que foram eliminados
  virtual void ReorganizarCmxCmy () override;

  // Função Go
  // virtual CGrafo* Go( TCMatriz3D<int> * _img3D,unsigned long int _tamanhoMascara = 1);

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
//       friend ostream& operator<< (ostream& os, CGra3Dby2D_M4& obj);
//       friend istream& operator>> (istream& is, CGra3Dby2D_M4& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, CGra3Dby2D_M4& obj);
// istream& operator>> (istream& is, CGra3Dby2D_M4& obj);

#endif
