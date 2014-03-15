#ifndef CEstatistica_h
#define CEstatistica_h

/**
 * @brief Classe base de estatistica, cálculo de atributos básicos como média, desvio padrão e variância.
 * 
 * Deve:
 * Receber um vetor de dados, que pode ser um CVetor, um vetor de funções,
 * ou seja um grupo de objetos.
*/

// Esta classe é herdeira da CEstatistica e vai usar objetos
// de CEstatistica, precisa ser declarada como friend
class CETesteMediaDuasAmostras;	

/**
 * @brief Classe estatística básica, determina informações como média, desvio padrão, variância, covariancia.
 * A classe CEstatistica deve ter objetos agregados para a realização de cálculos estatisticos.
 * */
class CEstatistica
{
  friend class CETesteMediaDuasAmostras;

// Atributos
protected:
  double media;         	///< Valor da média
  double desvioPadrao;  	///< Valor do desvio padrão
  double variancia;		///< Valor da variância =desvioPadrao*desvioPadrao;
  double covariancia;	        ///< Valor da covariância 
  // Verificar pois covariancia relaciona duas variáveis.

protected:
  int hasMissingData;		///< Dados inválidos
  double missingCode;		///< Tipo de erro nos dados

  int numeroDados;		///< Tamanho do vetor de dados
  double sum;			///< soma
  double sumX; 			///< Potenciais das somas
  double sumX2;			///< Potenciais das somas
  double sumX3; 		///< Potenciais das somas
  double sumX4;			///< Potenciais das somas

// Métodos
public:
  /// Construtor
  CEstatistica ()
  {
    Inicializa (false, 0);
  }

  /// Construtor
  CEstatistica (int HasMissingData, double MissingCode)
  {
    Inicializa (HasMissingData, MissingCode);
  }

  /// Destrutor
  virtual ~ CEstatistica ()
  {
  }

  /// Definir valores padrões
  void Inicializa (int HasMissingData, double MissingCode);	
 
  /// Calcula e retorna média
  double Media (double *Re_data, int NX, int NY, int NZ);

  /// Calcula e retorna o desvio padrão.
  double DesvioPadrao (double *x, int numData);

  /// Calcula e retorna variância
  double Variancia (double *Re_data, int NX, int NY, int NZ);
  
  /// Calcula covariância de um vetor Re_data
  double Covariancia (double *Re_data, int NX, int NY, int NZ);
  
  /// Retorna numero dados
  int NumeroDados ()
  {
    return numeroDados;
  }

  /// Retorna soma
  double Soma ()
  {
    return sum;
  }

  /// Retorna media
  double Media ()
  {
    return media;
  }

  /// Retorna desvio padrao
  double DesvioPadrao ()
  {
	  return desvioPadrao;
  }

  /// Retorna variancia
  double Variancia ()
  {
	  return variancia;
  }

  /// Retorna covariancia
  double Covariancia ()
  {
	  return covariancia;
  }
};

// Quando deve-se comparar dois vetores de dados
/// Falta implementar.
class CEstatistica2D
{
public:
  double covariancia;
};

#endif
