/*
 * File:   cutil.h
 * Author: rafael
 *
 * Created on 20 de Julho de 2009, 15:24
 */

#ifndef _CUTIL_H
#define	_CUTIL_H
#include <iostream>

/// Classe com métodos utilitários.
class CUtil
{
public:
    CUtil();
    CUtil(const CUtil& orig);
    virtual ~CUtil();

    /// Arruma a imagem.
    void ArrumarImagem( std::string nomeImg );

};

#endif	/* _CUTIL_H */

