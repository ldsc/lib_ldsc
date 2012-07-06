#ifndef CGra3Dby2D_h
#define CGra3Dby2D_h

/*
  ===============================================================================
  PROJETO:    Biblioteca LIB_LDSC
  Assunto/Ramo: CGra3Dby2D...
  ===============================================================================
  Desenvolvido por:	 Laboratorio de Desenvolvimento de Software Cientifico 	 [LDSC].
  @author     André Duarte Bueno
  @file       CGra3Dby2D.h
  @begin      Sat Sep 16 2000
  @copyright  (C) 2000 by André Duarte Bueno
  @email      andre@lmpt.ufsc.br
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <fstream>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

#ifndef CGrafoContorno_h
#include <Grafo/CGrafoContorno.h>
#endif

#ifndef CRotulador2DCm_h
#include <Rotulador/CRotulador2DCm.h>
#endif

// ===============================================================================
// Documentacao da  @class CGra3Dby2D
// ===============================================================================
/**
 * @brief Determina o grafo a partir de imagens tridimensionais.
 *
 * Gera o grafo a partir de imagens tridimensionais.
 * Gera a estrutura de sítios baseado na avaliação
 * de cada plano da imagem tridimensional (planos bidimensionais).
 * Vai ter mais de uma herdeira.
 * Observe que da forma como esta, sempre usa sítios (CSitio e herdeiros)
 * e nunca COGLigacao ou COGComponenteGrafo.
 *
 * Lista dos herdeiros:
 * -CGra3Dby2_M1	Modelo_1
 * Para cada pixel uma ligação
 * Condutancia do pixel
 *
 * -CGra3Dby2_M2	Modelo_2
 * Para cada objeto uma ligação
 * Condutancia média entre os dois objetos
 *
 * -CGra3Dby2_M3	Modelo_3
 * Para cada objeto uma ligação
 * Condutancia calculada sobre a area da intersecção
 *
 * -CGra3Dby2_M4	Modelo_4
 * Para cada objeto uma ligação.
 * Condutancia calculada sobre a área da intersecção
 * Adicionalmente calcula as distâncias entre
 * os objetos, para correção das condutâncias.
 *
 * -CGra3Dby2_M5	Modelo_5
 * -# sub ítem do modelo 5
 * -# sub ítem do modelo 5
 *
 * -CGra3Dby2_M6	Modelo_6
 * Para cada objeto uma ligação.
 * A condutância é corrigida levando em conta as distâncias
 * dos centros de massa, que aqui são armazenados no próprio sítio.
 *
 * @author	André Duarte Bueno
 * @see		Grafo
*/
class CGra3Dby2D : public CGrafoContorno
{
  // --------------------------------------------------------------Atributos
 private:
  int eliminaRamosMortos;	///< Elimina ramos mortos
  int eliminaConeccoesRepetidas;	///< Elimina conecções repetidas

 protected:
  /// Dimensoes da imagem tridimensional
  int nx; ///< Dimensão nx
  int ny; ///< Dimensão ny
  int nz; ///< Dimensão nz

  /// Rotulador bidimensional para imagem anterior
  CRotulador2DCm *ra;

  /// Rotulador bidimensional para imagem posterior
  CRotulador2DCm *rp;

  /// Imagem usada internamente para copiar planos ra, rp e plano intermediário
  CMatriz2D *img2D;

  /**
   * @brief Informa o plano que esta sendo avaliado.
   * É passado para o objeto, fazendo=se objeto->x=plano
   * de forma que a previsão inicial do valor de x poderá considerar o plano
  */
  unsigned long int plano;

  /// Por default o objeto esta no centro do grafo e não em seus contornos
  CContorno::ETipoContorno tipoContornoObjeto;

  /*Usado para definir o maior rótulo do plano anterior a ra.
     Ex: Se ra é o plano 12, rp é o plano 13,
     maiorRotuloUtilizado =  soma de todos os rótulos dos planos 0->11.*/

  /**
   * @brief Valor do maior rotulo já utilizado.
   * Em cada plano, os rótulos iniciam em 0 (o fundo), 1 o primeiro objeto,...n
   * A variável maior RotuloUtilizado vai acumulando o número de objetos do grafo.
  */
  unsigned long int maiorRotuloUtilizado;

 public:

  // -------------------------------------------------------------Construtor
/// Construtor, recebe um nome de arquivo
  CGra3Dby2D (std::string _fileName) : CGrafoContorno (_fileName), eliminaRamosMortos (2), eliminaConeccoesRepetidas (1)
  {
  }

    // --------------------------------------------------------------Destrutor
    /// Destrutor, deleta objetos internos
    virtual ~ CGra3Dby2D ()
    {
		if (ra) {
			delete ra; 				// deleta objeto
		}
		if (rp) {
			delete rp; 			// deleta objeto
		}

		if (img2D) {
			delete img2D; 			// deleta objeto
		}
     }

    // ----------------------------------------------------------------Métodos
    /**
     * @brief 	Função Go, realiza a determinação de todo o grafo
     * @param 	Recebe uma imagem tridimensional (ou seja toda a imagem 3D)
     * @return Retorna um ponteiro para this
    */
    virtual CGrafo *Go (CMatriz3D * _img3D, unsigned long int _tamanhoMascara = 1);

    /**
     * @brief Função Go, realiza a determinação de todo o grafo
     * @param Recebe o nome de um arquivo de disco (vai ler plano a plano)
     * @return Retorna um ponteiro para this
    */
    virtual CGrafo *Go (std::string fileName, unsigned long int _tamanhoMascara = 0);

    /**
     * @brief VAZIA: implementada no modelo Adv3, elimina os links repetidos
    */
    virtual void EliminarCondutanciasRepetidas ()
    {
    }

 protected:

    // FUNÇÕES AUXILIARES
    /**
    * @brief Função que cria os objetos e os adiciona a lista de objetos.
     * Recebe como parâmetros um objeto rotulador (com os objetos a
     * serem adicionados) o valor do maior rotulo utilizado
     * o tipo de objeto a ser criado*/
    virtual void AdicionarObjetos (CRotulador2DCm * rotulador, unsigned long int rotuloAtual, CContorno::ETipoContorno tipo);

    /// Função que conecta objetos em planos adjacentes
    virtual void DeterminarConeccoesObjetos (unsigned long int maiorRotuloUtilizado) = 0;

    /// Função Usada para calcular a condutancia das ligações
    // virtual void CalcularPropriedadesConeccoes() {};

    /// Função que elimina sítios com 0 links (??)
    void EliminarObjetosRedundantes ()
    {
      switch (eliminaRamosMortos)
	{
	case 0:			// não elimina os ramos mortos.
	  break;
	case 1:
	  EliminarObjetosRedundantes_1 ();
	  break;
	case 2:
	  EliminarObjetosRedundantes_2 ();
	  break;
	default:
	  EliminarObjetosRedundantes_2 ();
	  break;
	}
    }

    /// Elimina objetos redundantes, versão 1
    void EliminarObjetosRedundantes_1 ();

        /// Elimina objetos redundantes, versão 2
    void EliminarObjetosRedundantes_2 ();

    /**
     * @brief Reorganiza os links para cmx e cmy.
     * Funcao nova, como alguns ramos estão sendo deletados
     * eu chamo esta função para reorganizar os centros de massa
     * respeitando o novo estado do grafo
     * Necessaria para o modelo 4. Por isto é definida aqui como sendo vazia
    */
    virtual void ReorganizarCmxCmy ()
    {
    }

    /*
     * @brief Função que calcula o centro de massa dos objetos
     * vai ser redefinida na Adv4 e Adv5
     * Não implementada, vai deletar os centros de massa
     * ao deletar os objetos
    */
    // virtual void  CalcularCentroMassa(){};

    /*Função que deleta um objeto do grafo(não usada)*/
    // virtual bool DeletarObjeto(int ri) {return 0;};

    /**
     * @brief Função que marca um objeto do grafo para deleção é deletado
     * efeticamente por EliminarObjetosRedundantes_2
    */
    virtual bool MarcarParaDelecaoObjeto (int i);

    // virtual bool DeletarObjeto(CObjetoGrafo* sitio) ;

    // --------------------------------------------------------------------Get
 public:
	 /// Retorna nx
    int Getnx ()    { return nx;  }

	 /// Retorna ny
    int Getny ()    { return ny;  }

    	 /// Retorna nz
    int Getnz ()    { return nz;  }

    	 /// Elimina ramos mortos
    int GeteliminaRamosMortos() { return eliminaRamosMortos;    }

    /// Elimina conecções repetidas
    int GeteliminaConeccoesRepetidas() {  return eliminaConeccoesRepetidas;}

    // --------------------------------------------------------------------Set
    /// Seta elimina ramos mortos
    void SeteliminaRamosMortos (int _e) { eliminaRamosMortos = _e; }

    /// Seta conecções repetidas
    void SeteliminaConeccoesRepetidas (int _r) { eliminaConeccoesRepetidas = _r;}

    // -----------------------------------------------------------------Friend
    //       friend ostream& operator<< (ostream& os, CGra3Dby2D& obj);
    //       friend istream& operator>> (istream& is, CGra3Dby2D& obj); //  Public attributes
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, CGra3Dby2D& obj);
// istream& operator>> (istream& is, CGra3Dby2D& obj);

#endif
