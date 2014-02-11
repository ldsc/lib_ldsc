#ifndef CReconstrucao_h
#define CReconstrucao_h

// -----------------------------------------------------------------------------
//                                        BIBLIOTECAS
// -----------------------------------------------------------------------------
// Bibliotecas do lmpt
// #include    <stdio.h>
// #include    <fstream.h>
// #include <Matriz/TCMatriz3D.h>
#include <string>

/**
 * @brief Classe base para reconstrucao de imagens 3D.
*/
class  CReconstrucao
{
// Atributos
// Metodos
public:
/// Construtor
  CReconstrucao ()
  {
  }

  /// Destrutor
  virtual ~ CReconstrucao ()
  {
  }

  /// Executa a reconstrucao
  virtual int Go ()
  {
    return 1;
  }

  //  Incluida para atender CReconstrucaoBueno
  virtual void Calculo_Ry1D (){}
  virtual bool SalvarImagemEmDisco (std::string) { return false; }
};
#endif
