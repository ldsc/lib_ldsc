#include  <AnaliseImagem/Reconstrucao/CReconstrucaoEsferas.h>

//Construtor
CReconstrucaoEsferas::CReconstrucaoEsferas ( double _solid, int _dimens, double _fat_sob, double _fat_borda, int _fat_amp, double _tam_pixel )
   : solid(_solid), dimens(_dimens), fat_sob(_fat_sob), fat_borda(_fat_borda), fat_amp(_fat_amp), tam_pixel(_tam_pixel)
{
   //    srand(13);
}

//=================== Executa a recontrução ==============================//
bool CReconstrucaoEsferas::Go ( string nome_arq ) {
   //================ Aloca a memoria ===========================================//
   int i, flag, r, d, ptx, pty, ptz, d_cor, r_aux, rmax, dmax, pos;
   double porosidade, solido, perhist, dif;
   double soma = 0.0;
   int nupontos = dimens*dimens*dimens;
   unsigned char *meio,*inimeio;
   inimeio = (unsigned char*)calloc(nupontos,sizeof(unsigned char));

   //==============  Inicializa o meio ( *meio = 1 ) ============================//
   for ( i = 0; i < nupontos; i++ ) {
      meio = inimeio + i;
      *meio = 1;
   }

   //================ Le os arquivos de entrada =================================//
   ifstream filein( nome_arq.c_str() );

   if ( (! filein.is_open()) || (! filein.good()) ) {
      return false;
   }
   // conta o número de linhas do arquivo
   string lixo;
   /*
    rmax=0;
    while ( (! filein.fail()) && ( ! filein.eof()) ) {
     getline( filein, lixo, '\n' );
     if (lixo.at(0) != '#'){
            rmax++;
      cerr << "aqui!" << rmax << endl;
     }
    }
    rmax--;
    */
   while ( (! filein.fail()) && ( ! filein.eof()) ) { //percorre o arquivo até chegar o fim do arquivo ou atingir o tamanho máximo do vetor correlação
      pos = filein.tellg(); // pega a posição atual de leitura
      getline(filein, lixo, '\n'); // le toda a linha
		if (lixo.substr(0,7) == "# Size:") {
         filein.seekg(pos+8); // reposiciona o ponteiro de leitura para pegar a porosidade
         filein >> rmax; //pega o valor referente ao maior raio
         break;
      }
   }
   filein.clear(); // desliga os bits de estado eofbit, failbit e badbit e liga o bit de estado goodbit.
   filein.seekg (0, ios::beg);	//  vai para posicao 0, a partir do inicio do arquivo
   //filein.seekg (0);	//  vai para posicao 0, a partir do inicio do arquivo

	dmax = rmax * 2;
	cout << "rmax: " << rmax << endl;
	cout << "dmax: " << dmax << endl;
   //teste
   /*
    filein.seekg (0, ios::end);
    cerr << "tamanho do arquivo: " << int(filein.tellg()) << endl;
    filein.clear();
    filein.seekg (0, ios::beg);
    */
   //posiciona o ponteiro de leitura logo após os comentários
   while ( (! filein.fail()) && ( ! filein.eof()) ) {
      pos = filein.tellg();
      getline( filein, lixo );
      if (lixo.at(0) != '#')
         break;
   }
   filein.seekg(pos);

	double phi = 1.0 - solid;
	cout << "Solidos: " << solid << endl;
	cout << "Porosidade: " << phi << endl;

   //----------------------------------------------------------------------------//

   unsigned int hist[dmax+1];

   for ( d = 0; d <= dmax; d++ )
      hist[d] = 0; // Zera o histograma

   for ( r = 1; r < rmax + 1; r++ ) {  // diametro inicia com d = 2
      double vaux;
      filein >> vaux;
      d = r * 2;
      filein >> perhist;
      perhist = perhist * solid; // corrige de acordo com a porosidade
      double volume = (4.0/3.0)*M_PI*(double)(r*r*r);  // volume de uma esfera
      double Ne = perhist*(double)(dimens*dimens*dimens)/volume; // Ne -> número de esferas
      dif = Ne - (int)Ne;
      soma = soma + dif * volume; // Soma para posterior correção
      hist[d] = hist[d] + (int)Ne;
      //----------- Corrige o truncamento dos dados de entrada -----------------//
      int dife = 0;
      while ( soma > 1.0 ) {
         d_cor = (int)(pow((6.*soma)/M_PI,1./3.)); // Calcula um diâmetro d_cor a partir da soma das correções
         if ( d_cor > dmax ) {
            d_cor = dmax;
            soma = soma - (double)(d_cor*d_cor*d_cor)*M_PI/6.0;
            dife = 1;
         }
         if ( d_cor > 0 ) {
            hist[d_cor] = hist[d_cor] + 1;
            if ( dife == 0 )
               soma = 0.0;
            else
               dife = 0;
         }
      }
   }

   filein.close();

   //================ Grava a dist. de saída e a porosidade =====================//
   char hist_img[50];
   sprintf ( hist_img,"Hist_%s_%3d_Fs=%1.1f.dat", nome_arq.c_str(), dimens, fat_sob );

   solido = 0.0;
   ofstream os( hist_img );

   for ( d = 1; d < (dmax+1); d++ ) {
      double volume = (double)(d*d*d)*M_PI/6.0;
      solido = solido + (double)(hist[d])*volume;
      os << d << " - " << (hist[d]) << " - " << (double)(hist[d])*volume << endl;
   }

   os.close();

   porosidade = 1.0 - solido / (double)( dimens*dimens*dimens );
   cout << "\nPorosidade teorica = " << porosidade << endl;

   //===== Verifica qual o maior diâmetro a ser efetivamente gerado =============//
   int dmax_ef = dmax;
   for ( d = dmax; d > 0; d-- ) {
      if ( hist[d] > 0 ) {
         dmax_ef = d;
         break;
      }
   }
   cout << "Diametro maximo gerado = " << dmax_ef << endl;

   //================ Inicio do Looping dos diametros ===========================//
loop:
   int flag_esf = 0;
   int vol_sbr = 0;
   int sum_sbr = 0;
   for ( d = dmax_ef; d > 0; d-- ) {
      r = (int)( (double)d / 2.0 + 0.5 ); // raio com arredondamento
      r_aux = r + 1;
      for ( i = 0; i < hist[d]; i++ ) {
         cout << "Diametro = " << d << "  hist[d] = " << hist[d] << "  esf = " << i << endl;
         //------------ Sorteia um ponto ptx, pty, ptz ------------------------//
         ptx = rand()%dimens;
         pty = rand()%dimens;
         ptz = rand()%dimens;
         //------------ Verifica conexão na borda -----------------------------//
         if ( (double)d <= fat_sob * (double)dmax_ef && flag_esf ) {
            flag = VerBorda ( inimeio, r_aux, ptx, pty, ptz );
            while ( flag == 0 ) {
               ptx = rand()%dimens;
               pty = rand()%dimens;
               ptz = rand()%dimens;
               flag = VerBorda ( inimeio, r_aux, ptx, pty, ptz );
            }
         }
         //-------- Gera a esfera em torno do ponto sorteado ------------------//
         int vol_esf; // Volume da esfera gerada
         vol_sbr = Esfera (inimeio, r, r_aux, ptx, pty, ptz, &vol_esf );
         sum_sbr = sum_sbr + vol_sbr;
         flag_esf = 1;
      }
      //---------------- Corrige o volume sobreposto ---------------------------//
      while ( sum_sbr > 0 ) {
         if ( sum_sbr % 100 == 0 )
            cout << "\rDiametro = " << d << "  hist[d] = " << hist[d] << "  esf = " << i << "  Sobrepos. = " << sum_sbr << "   " ;
         //------------ Sorteia um ponto ptx, pty, ptz ------------------------//
         ptx = rand()%dimens;  // Sorteia um ponto ptx, pty, ptz
         pty = rand()%dimens;
         ptz = rand()%dimens;
         //---------------- Verifica conexão na borda -------------------------//
         if ( (double)d <= fat_sob*(double)dmax  && flag_esf ) {
            flag = VerBorda ( inimeio, r_aux, ptx, pty, ptz );
            while ( flag == 0 )	{
               ptx = rand()%dimens;
               pty = rand()%dimens;
               ptz = rand()%dimens;
               flag = VerBorda ( inimeio, r_aux, ptx, pty, ptz );
            }
         }
         //-------- Gera a esfera em torno do ponto sorteado ------------------//
         int vol_esf; // Volume da esfera gerada
         vol_sbr = Esfera (inimeio, r, r_aux, ptx, pty, ptz, &vol_esf );
         sum_sbr = sum_sbr - vol_esf + vol_sbr;
      }

   } // Fim do looping dos diâmetros

   //=============== Verifica porosidade e corrige fator de borda ==============//
   double phi_ger = Calc_phi ( inimeio );
   cout << "\n\nFator Borda = " << fat_borda << "	Porosidade gerada = " << phi_ger << endl << endl;

   double erro = phi_ger - phi;
   fat_borda = fat_borda + 10.0 * erro;

   if ( fat_borda > 1.0 )
      fat_borda = 1.0;
   if ( fat_borda < 0.0 )
      fat_borda = 0.0;

   if ( erro > 0.005 ) { //--- Reinicializa o meio ( *meio = 1 ) ---//
      for ( i = 0; i < nupontos; i++ ) {
         meio = inimeio + i;
         *meio = 1;
      }
      goto loop;
   }

   //================ Grava os arquivos de saída ================================//
   //stringstream img_dat;
   //img_dat << nome_arq << "_" << dimens << "_FS-" << fat_sob << "_FB-" << fat_borda << ".dat";
   //Grava_dat ( img_dat.str(), inimeio );

   //stringstream img_vtk;
   //img_vtk << nome_arq << "_" << dimens << "_FS-" << fat_sob << "_FB-" << fat_borda << ".vtk";
   //Grava_vtk ( img_vtk.str(), inimeio );

   stringstream name;
   name << nome_arq << "_ES_" << dimens << "_fa-" << fat_amp << "_fb-" << setprecision(2) << fat_borda << "_fs-" << setprecision(2) << fat_sob << ".dbm";

   nomeArquivo = name.str();
   cerr << nomeArquivo.c_str() << endl;
   return Grava_dbm ( nomeArquivo, inimeio );
} // Fim Go()...

//=================== Verificação da borda ===================================//

int CReconstrucaoEsferas::VerBorda ( unsigned char *inimeio, int raio, int ptx, int pty ,int ptz ) {
   unsigned char *meio;
   int x, y, z;
   int xx, yy, zz;
   int flag = 0;
   double delta = M_PI/(double)( 4*raio );

   for ( double phi = 0.0; phi <= M_PI; phi = phi + delta ) {
      for ( double theta = 0.0; theta <= 2*M_PI; theta = theta + delta ) {
         x = ptx + (int)((double)raio*cos(theta)*sin(phi)+0.5);
         y = pty + (int)((double)raio*sin(theta)*sin(phi)+0.5);
         z = ptz + (int)((double)raio*cos(phi)+0.5);

         xx = ( x + dimens )%dimens;
         yy = ( y + dimens )%dimens;
         zz = ( z + dimens )%dimens;

         meio = inimeio + xx + yy*dimens + zz*dimens*dimens;

         if ( *meio == 0 ) flag = 1; // Há conexão //
      }
   }
   return flag;
}

//=================== Gera esfera e retorna vol. sobreposto ==================//

int CReconstrucaoEsferas::Esfera ( unsigned char* inimeio, int r, int r_aux, int ptx, int pty, int ptz, int* vol_ger ) {
   unsigned char *meio;
   int vol_sobr = 0;
   int x, y, z;
   int xx, yy, zz;
   int sum_vol = 0;
   for ( x = (ptx-r_aux); x < (ptx+r_aux); x++ ) {
      for ( y = (pty-r_aux); y < (pty+r_aux);  y++ ) {
         for ( z = (ptz-r_aux); z < (ptz+r_aux); z++ ) {
            if ( (double)(rand()%101 ) < fat_borda * 100.0 ) {
               if ( (x-ptx)*(x-ptx)+(y-pty)*(y-pty)+(z-ptz)*(z-ptz) <= r*r ) {
                  xx = ( x + dimens )%dimens;
                  yy = ( y + dimens )%dimens;
                  zz = ( z + dimens )%dimens;
                  meio = inimeio + xx + yy*dimens + zz*dimens*dimens;
                  if ( *meio == 0 ) vol_sobr++;// Soma vol. sobreposto
                  *meio = 0;
                  sum_vol++;
               }
            } else {
               if ( (x-ptx)*(x-ptx)+(y-pty)*(y-pty)+(z-ptz)*(z-ptz) < r*r ) {
                  xx = ( x + dimens )%dimens;
                  yy = ( y + dimens )%dimens;
                  zz = ( z + dimens )%dimens;
                  meio = inimeio + xx + yy*dimens + zz*dimens*dimens;
                  if ( *meio == 0 ) vol_sobr++;// Soma vol. sobreposto
                  *meio = 0;
                  sum_vol++;
               }
            }
         }
      }
   }
   *vol_ger = sum_vol;
   return vol_sobr;
}

//=================== Grava os resultados .dat ===============================//

void CReconstrucaoEsferas::Grava_dat ( string nomeimg, unsigned char *inimeio ) {
   unsigned char *meio;
   ofstream fmeio ( nomeimg.c_str() );
   if ( fmeio.good() && fmeio.is_open()) {
      fmeio << tam_pixel << "e-6" << endl;
      fmeio << dimens << " " << dimens << " " << dimens << endl;
      int poros = 0;
      for ( int z = 0; z < dimens; z++ ) {
         for ( int y = 0; y < dimens; y++ ) {
            for ( int x = 0; x < dimens; x++ ) {
               meio = inimeio + x + y*dimens + z*dimens*dimens;
               if ( *meio ) {
                  fmeio << 1 << " " ;
                  poros = poros + 1;
               } else {
                  fmeio << 0 << " " ;
               }
            }
            fmeio << endl;
         }
         fmeio << endl;
      }
      fmeio.close();
      double phi_ger = (double)poros/(double)( dimens *dimens *dimens );
      cout << "\nArquivo gerado em: " << nomeimg.c_str() << endl;
      cout << "\nPorosidade gerada = " << phi_ger << endl;
   } else {
      cerr << "Não foi possível criar o arquivo em CReconstrucaoEsferas::Grava_dat" << endl;
   }
}

//=================== Grava os resultados .vtk ===============================//

void CReconstrucaoEsferas::Grava_vtk ( string nomeimg, unsigned char *inimeio ) {
   unsigned char *meio;
   ofstream fmeio_out( nomeimg.c_str() );
   if ( fmeio_out.good() && fmeio_out.is_open()) {
      fmeio_out << "# vtk DataFile Version 2.0" << endl;
      fmeio_out << "Geometria" << endl;
      fmeio_out << "ASCII" << endl;
      fmeio_out << "DATASET STRUCTURED_POINTS" << endl;
      fmeio_out << "DIMENSIONS " << dimens << " " << dimens << " " << dimens << endl;
      fmeio_out << "ASPECT_RATIO 1 1 1" << endl;
      fmeio_out << "ORIGIN 0 0 0" << endl;
      fmeio_out << "POINT_DATA " << dimens*dimens*dimens << endl;
      fmeio_out << "SCALARS geometria int" << endl;
      fmeio_out << "LOOKUP_TABLE default" << endl;
      for ( int z = 0; z < dimens; z++ ) {
         for ( int y = 0; y < dimens; y++ ) {
            for ( int x = 0; x < dimens; x++ ) {
               meio = inimeio + x + y*dimens + z*dimens*dimens;
               if ( *meio )
                  fmeio_out << 1 << " ";
               else
                  fmeio_out << 0 << " ";
            }
            fmeio_out << endl;
         }
      }
      fmeio_out.close();
      cout << "\nArquivo gerado em: " << nomeimg.c_str() << endl;
   } else {
      cerr << "Não foi possível criar o arquivo em CReconstrucaoEsferas::Grava_vtk" << endl;
   }
}

//=================== Grava os resultados .dbm ===============================//

bool CReconstrucaoEsferas::Grava_dbm ( string nomeimg, unsigned char *inimeio ) {
   unsigned char *meio;
   ofstream fout( nomeimg.c_str() );
   if ( fout.good() && fout.is_open()) {
      fout << setw (0) << "D1\n" << dimens << " " << dimens << " " << dimens << "\n";
      fout << setw (0) << "# fatorAmplificacao: " << fat_amp << "\n";
      fout << setw (0) << "# dimensaoPixel: " 	     << tam_pixel << "\n";
      fout << setw (0) << "# numeroPixelsBorda: " << 0 << "\n";
      for ( int z = 0; z < dimens; z++ ) {
         for ( int y = 0; y < dimens; y++ ) {
            for ( int x = 0; x < dimens; x++ ) {
               meio = inimeio + x + y*dimens + z*dimens*dimens;
               if ( *meio )
                  fout << 1 << " ";
               else
                  fout << 0 << " ";
            }
            fout << "\n";
         }
      }
      fout.close();
      cout << "\nArquivo gerado em: " << nomeimg.c_str() << endl;
   } else {
      cerr << "Não foi possível criar o arquivo em CReconstrucaoEsferas::Grava_dbm" << endl;
      return false;
   }
   return true;
}

//=================== Calcula a porosidade da imagem =========================//

double CReconstrucaoEsferas::Calc_phi ( unsigned char* inimeio) {
   unsigned char *meio;
   int poros = 0;
   for ( int z = 0; z < dimens; z++ ) {
      for ( int y = 0; y < dimens; y++ ) {
         for ( int x = 0; x < dimens; x++ ) {
            meio = inimeio + x + y*dimens + z*dimens*dimens;
            if ( *meio )
               poros++;
         }
      }
   }
   double phi_ger = (double)poros/(double)( dimens *dimens *dimens );
   return phi_ger;
}
