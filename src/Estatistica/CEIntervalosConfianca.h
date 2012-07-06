#ifndef CEIntervalosConfianca_h
#define CEIntervalosConfianca_h

#include <Estatistica/CEstatistica.h>

/**
 * @brief Determina o intervalo de confiança para média e desvio padrão.
 * Herdeira de CEstatistica.
*/
class CEIntervalosConfianca : public CEstatistica
{
protected:
  double deslocamento; ///< Valor do deslocamento
  double minimo;       ///< Valor mínimo
  double maximo;       ///< Valor máximo

public:
  /// Construtor
  CEIntervalosConfianca(): deslocamento(0),minimo(0),maximo(0) {};
	
  /// Destrutor
  virtual ~ CEIntervalosConfianca() {};

  /// Dada a probabilidade calcula o deslocamento em relação a média, retorna 0/1.
  bool DeslocamentoMedia (double probabilidade);

  /// Dada a probabilidade calcula o deslocamento em relação ao desvio padrão, retorna 0/1.
  bool DeslocamentoDesvioPadrao (double probabilidade);

  /// Retorna o deslocamento calculado.
  double Deslocamento()		{ return deslocamento; }

  /// Retorna valor mínimo calculado.
  double Minimo()		{ return minimo; }

  /// Retorna valor máximo calculado.
  double Maximo()		{ return maximo; }

};

#endif
