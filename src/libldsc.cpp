/***************************************************************************
 *   Copyright (C) 2007 by Andre Duarte Bueno Fone: 22-27969751,22-99542635*
 *	http://www.lenep.uenf.br/~bueno  bueno@lenep.uenf.br   			*
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <cstdlib>

using namespace std;

int main(int argc, char *argv[])
{
cout 	<< " ===================================================================================\n"
   	<< "                         Sucesso, lib_ldsc compilada !" << endl	
 	<< " ===================================================================================\n"
	<< " Parabéns, a biblioteca foi compilada e os programas demo foram"
	<< " gerados corretamente.\n"
	<< "\n"
	<< " Digite (como root) \"make install\" para instalar a biblioteca.\n" 
	<< " \n"
	<< " Dentro do diretório test estão alguns exemplos de programas montados"
	<< " utilizando a biblioteca.\n"
	<< " \n"
	<< " A documentação da biblioteca é gerada digitando-se, dentro do diretório base,\n"
	<< " o comando \"doxygen Doxfile\". A documentação fica instalada em doc/html. \n"
	<< " ===================================================================================" << endl;


  return EXIT_SUCCESS;
}
