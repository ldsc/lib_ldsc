/*
 * @file:   cutil.cpp
 * Author: rafael
 *
 * Created on 20 de Julho de 2009, 15:24
 */

#include "CUtil.h"
#include <fstream>
#include <iostream>

using namespace std;

CUtil::CUtil() {
}

CUtil::CUtil(const CUtil& orig) {}

void CUtil::ArrumarImagem(std::string nomeImg)
{
    ifstream arq;
    std::string formato;
    arq.open( nomeImg.c_str() );
    arq >> formato;
    arq.close();

    // linha =  arq.getline();
    cout << "linha: " << formato;
}

CUtil::~CUtil() {
}

