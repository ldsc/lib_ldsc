/**
  ===============================================================================
  PROJETO:          Biblioteca LIB_LDSC
  Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
  ===============================================================================
  Desenvolvido por:
                    Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
  @author:          André Duarte Bueno
  @file:            CBaseGrafoRedeEsqueleto.cpp
  @begin:           Sat Sep 16 2000
  @copyright:       (C) 2000 by André Duarte Bueno
  @email:           andreduartebueno@gmail.com
*/
// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <EstruturaDados/CBaseGrafoRedeEsqueleto.h>

std::ostream & operator<< ( std::ostream & out, const CBaseGrafoRedeEsqueleto & grafo )
{
   grafo.Write( out );
   return out;
}
