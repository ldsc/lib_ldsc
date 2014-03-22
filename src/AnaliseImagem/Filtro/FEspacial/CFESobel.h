
#ifndef CFESobel_h
#define CFESobel_h

#include <MetNum/Matriz/TCMatriz2D.h>

#ifndef TCFEspacial_h
#include <AnaliseImagem/Filtro/FEspacial/TCFEspacial.h>
#endif

/**
 * @brief Implementa o filtro de Sobel.
 * Herdeiro de TCFEspacial.
*/
class CFESobel : public TCFEspacial<int>
{
public:
  /// Construtor
  CFESobel (TCMatriz2D<int> * matriz) : TCFEspacial<int> (matriz, 3)
  {
  }

  /// Processa o filtro.
  virtual TCMatriz2D<int> *Go (TCMatriz2D<int>* &matriz, unsigned int _tamanhoMascara = 3);
  virtual void CriaMascara (unsigned int _tamanhoMascara);
};

#endif
