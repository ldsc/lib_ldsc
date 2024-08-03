/***************************************************************************
 *   Copyright (C) 2004 by Tiago Ribeiro Schaewer - LENEP/UENF             *
 *   Tiago Ribeiro Schaewer: tschaewer@rc01   - 2004                       *
 *   André Duarte Bueno: bueno@lenep.uenf.br  - 2024                       *
 *   This program is free software.                                        *
 ***************************************************************************/
//#include "error.h"       // poucas linhas msg erro
//#include "globais.h"     // definições valores, ex: #define PORE 1
//#include "progressbar.h" // necessário se for querer barra de progressão
#include "CEncrustracao.h"   // arquivo cabeçalho da classe
#include <cmath>

// ---------------------------------------------------------------------------
CEncrustracao::CEncrustracao(TCImagem3D<int>* _pmatrix)
{
  if(_pmatrix == nullptr) {
    cerr <<  "Error. pimagem3D should not have a NULL value.";
  assert(false);
  }
  pimagem3D = _pmatrix;
  // pega dimensoes da matriz 3D
  dim_x = pimagem3D->NX();
  dim_y = pimagem3D->NY();
  dim_z = pimagem3D->NZ();
}

// --------------------------------------------------------------------------
CEncrustracao::~CEncrustracao()
{
}

// -------------------------------------------------------------------------
bool CEncrustracao::scalePercent(int percentPorosityScale) {
  return scaleAgglomeratePercentage(percentPorosityScale, 0, 0);
}

// --------------------------------------------------------------------------
bool CEncrustracao::scaleAgglomeratePercentage(int percentPorosityScale, int agglomerateShare, int microPorosityPrm) {
  double microPorosity = microPorosityPrm / (double)HUNDREDPERCENT;
  cout << microPorosity << endl;
  assert(microPorosity <= 0.5);
  assert((percentPorosityScale/ (double)HUNDREDPERCENT) + microPorosity < 0.9);
  long long totalScaleVoxels = (getNoPorousVoxels(pimagem3D) * percentPorosityScale) / HUNDREDPERCENT;
  long long agglomerateVoxels = (totalScaleVoxels * agglomerateShare) / HUNDREDPERCENT;
  long long normalVoxels = totalScaleVoxels - agglomerateVoxels;

  return scalePorousAgglomerate(normalVoxels, agglomerateVoxels, microPorosity);
}

// -------------------------------------------------------------------------
bool CEncrustracao::scalePorousAgglomerate(long long normalScaleVoxels, long long agglomerateScaleVoxels, float microPorosity) {
  if (scaleAgglomerate(normalScaleVoxels * (1+microPorosity), agglomerateScaleVoxels * (1+microPorosity)) == false) return false;

  // generate microporosity
  // total a ser gerado
  long long microPores = (normalScaleVoxels + agglomerateScaleVoxels) * microPorosity;
  // inicializa com 0
  long long microPoreCount=0;
  int x, y, z;
  std::vector<int>**  ptrDataImagem3D = pimagem3D->Data3D();
  // enquanto não atingiu o total
  while(microPoreCount < microPores) {
    x = rand()%dim_x; // gera numeros randomicos
    y = rand()%dim_y;
    z = rand()%dim_z;
    if(isScale(ptrDataImagem3D, x,y,z)){    // verifica se o poro esta sujeito a entupimento
      ptrDataImagem3D[x][y][z]=PORE; // marca como PORE
      ++microPoreCount;               // incrementa número poros entupidos
    }
  }
  return true;
}

// --------------------------------------------------------------------------
// Gera números aleatórios, verifica se esta sujeito a incrustração, se sim, encrusta um poro/voxel e retorna.
bool CEncrustracao::scaleNormalVoxel() {
  int x, y, z;
  std::vector<int>**  ptrDataImagem3D = pimagem3D->Data3D();
  bool incrusta = false;
  do {
    // inline void getRandomVoxelCoordinate(int& x, int& y, int& z) { x = rand() % nx; ; y = rand()%ny; z = rand()%nz; }
    //pimagem3D->getRandomVoxelCoordinate(x,y,z);
    getRandomVoxelCoordinate(pimagem3D, x,y,z);
    // is it a porous voxel?
    if (ptrDataImagem3D[x][y][z]==PORE) {
      // does it have at least one rocky neighbour?
      if(numRockNeighbors(pimagem3D,x,y,z)+numScaleNeighbors(x,y,z)>=2) {
        incrusta = true;
      }
    }
  } while(incrusta==false);
  ptrDataImagem3D[x][y][z]=NORMALSCALE;

  return true;
}

// --------------------------------------------------------------------------
// Gera números aleatórios, verifica se esta sujeito a incrustração, se sim, encrusta um poro/voxel e retorna.
bool CEncrustracao::scaleAgglomerateVoxel() {
  int x, y, z;
  std::vector<int>**  ptrDataImagem3D = pimagem3D->Data3D();
  bool incrusta = false;
  do {
    //pimagem3D->getRandomVoxelCoordinate(x,y,z);
    getRandomVoxelCoordinate(pimagem3D, x,y,z);
    // is it a porous voxel?
    if (ptrDataImagem3D[x][y][z]==PORE) {
      // does it have at least two scaled neighbors?
      incrusta = numScaleNeighbors(x,y,z) >= 2;
      if (incrusta == false) {
        // does it have at least one rocky neighbour and one scaled neighbor
        if ((numRockNeighbors(pimagem3D,x,y,z)>0) & hasScaleNeighbor(x,y,z)) {
          incrusta = true;
        }
      }
    }
  } while(incrusta==false);
  ptrDataImagem3D[x][y][z]=AGGLOMERATESCALE;

  return true;
}

// --------------------------------------------------------------------------
// Neste caso a incrustração sera algo como uma cruz, tem uma dimensão dentriteCellSize
int CEncrustracao::scaleDentriticVoxel(int dentriteCellSize, int dendriteSpacing) {
  int x, y, z;
  int count = 0;
  std::vector<int>**  ptrDataImagem3D = pimagem3D->Data3D();
  bool incrusta = false;

  int nx = pimagem3D->NX(); // Dimensões da imagem
  int dim_y = pimagem3D->NY();
  int dim_z = pimagem3D->NZ();

  do {
    x = (rand() % nx) / dentriteCellSize; // Posição de x
    y = (rand() % dim_y) / dentriteCellSize;
    z = (rand() % dim_z) / dentriteCellSize;
    if (  ((x % dendriteSpacing == 0) && (y % dendriteSpacing == 0)) // verificar
          || ((x % dendriteSpacing == 0) && (z % dendriteSpacing == 0))
          || ((y % dendriteSpacing == 0) && (z % dendriteSpacing == 0))) {

      x = x * dentriteCellSize; // parece ser a posição onde irá incrustrar
      y = y * dentriteCellSize;
      z = z * dentriteCellSize;

      for (int i=x; i<x+dentriteCellSize; i++) {
        if (incrusta) break;
        for (int j=y; j<y+dentriteCellSize; j++) {
          for (int l=z; l<z+dentriteCellSize; l++) {
            if(isPorous(ptrDataImagem3D, i,j,l)) {
              // does it have at least one scaled neighbors?
              if (numScaleNeighbors(i,j,l) >= 1) {
                incrusta = true;
              }
            }
          }
        }
      }
    }
  } while(incrusta==false);

  // incrusta todos os voxels poros regiao
  for (int i=x; i<x+dentriteCellSize; i++) {
    for (int j=y; j<y+dentriteCellSize; j++) {
      for (int l=z; l<z+dentriteCellSize; l++) {
        if(isPorous(ptrDataImagem3D, i,j,l)) {
          ptrDataImagem3D[i][j][l]=AGGLOMERATESCALE;
          ++count;
        }
      }
    }
  }

  return count;
}

// --------------------------------------------------------------------------
bool CEncrustracao::scaleAgglomerate(long long normalScaleVoxels, long long agglomerateScaleVoxels) {
  //Thiago: assertmsg(pimagem3D, "Error. pimagem3D should not have a NULL value.");
  if(pimagem3D == nullptr){
    cerr <<  "Error. pimagem3D should not have a NULL value.";
    assert(false);
  }

  //Thiago: assertmsg(normalScaleVoxels > 0, "Error. normalScaleVoxels must be greater than 0.");
  if(normalScaleVoxels < 0){
    cerr << "Error. normalScaleVoxels must be greater than 0.";
  }

  //initialize progress bar
  ////ProgressBar* progressBar = new ProgressBar(normalScaleVoxels + agglomerateScaleVoxels);
  // show it on the screen, the next update may take too long
  ////progressBar->updateStep(1);

  srand ( time(NULL) );
  long long normalScaleCount = 0;
  long long agglomerateScaleCount = 0;
  int randomNumber;
  int agglomerateChance = (agglomerateScaleVoxels * HUNDREDPERCENT) / (normalScaleVoxels + agglomerateScaleVoxels);
  // start scaling, stop when the desired number of voxels have been scaled
  while((normalScaleCount < normalScaleVoxels) || (agglomerateScaleCount < agglomerateScaleVoxels)) {
    // let's calculate the chance of having a agglomerated voxel
    randomNumber = rand()%HUNDREDPERCENT;
    // let's generate at least 10% of normal voxels before generating any agglomeration
    if (normalScaleCount < (normalScaleVoxels / 10)){
      randomNumber = HUNDREDPERCENT;
    }
    if((randomNumber >= agglomerateChance) && (normalScaleCount < normalScaleVoxels)) {
      // let's generate a normal scale voxel
      scaleNormalVoxel();
      ++normalScaleCount;
    }

    else if (agglomerateScaleCount < agglomerateScaleVoxels){
      // let's generate an agglomerated voxel
      scaleAgglomerateVoxel();
      ++agglomerateScaleCount;
    }// else

    //  // Update the progress bar every 0xFFFF (65535) steps. Updating the scale bar is VERY costly since it requires redrawing it on the screen
    // if (((agglomerateScaleCount+normalScaleCount) & 0xFFF)==0xFFF) {
    //   progressBar->updateStep(agglomerateScaleCount+normalScaleCount);
    //   // let's check if the user has canceled us
    //   if (progressBar->wasCanceled()) {
    //     // close the progress bar
    //     progressBar->close();
    //     return false;
    //   }
    // }
  }// while

  // Close the progress bar
  // progressBar->close();

  return true;
}

// --------------------------------------------------------------------------
bool CEncrustracao::scaleDentriticPercentage(int percentPorosityScale, int dentriticShare) {
  long long totalScaleVoxels = (getNoPorousVoxels(pimagem3D) * percentPorosityScale) / HUNDREDPERCENT;
  long long dentriticVoxels = (totalScaleVoxels * dentriticShare) / HUNDREDPERCENT;
  long long normalVoxels = totalScaleVoxels - dentriticVoxels;

  cout << getNoPorousVoxels(pimagem3D)<<"\n";
  cout << totalScaleVoxels <<"\n";
  cout << dentriticVoxels  <<"\n";
  cout << normalVoxels <<"\n";
  return scaleDentritic(normalVoxels, dentriticVoxels);
}

// --------------------------------------------------------------------------
bool CEncrustracao::scaleDentritic(long long normalScaleVoxels, long long dentriticScaleVoxels) {
  //Thiago: assertmsg(pimagem3D, "Error. pimagem3D should not have a NULL value.");
  // Testei no construtor
  // Thiago: assertmsg(normalScaleVoxels > 0, "Error. normalScaleVoxels must be greater than 0.");
  if(normalScaleVoxels < 0) {
    cerr << "Error. normalScaleVoxels must be greater than 0.";
  return 0;
  }
  const int dendriteDiameter = 5;  //microns
  float voxelSizeMicrons = pimagem3D->DimensaoPixel() * 1000000 * pimagem3D->FatorAmplificacao();
  int dentriteCellSize = round(dendriteDiameter / voxelSizeMicrons);
  if (dentriteCellSize < 1) return false;

  //float dendriteSizeMicrons = 35;  //e=7
  float dendriteSizeMicrons = 25;    //e=5
  int dendriteSpacing = round(dendriteSizeMicrons / voxelSizeMicrons );

  //initialize progress bar
  ////ProgressBar* progressBar = new ProgressBar(normalScaleVoxels + dentriticScaleVoxels);
  // show it on the screen, the next update may take too long
  ////progressBar->updateStep(1);

  srand ( time(NULL) );
  long long normalScaleCount = 0;
  long long dentriticScaleCount = 0;
  int randomNumber;
  int dentriticChance = (dentriticScaleVoxels * HUNDREDPERCENT) / (normalScaleVoxels + dentriticScaleVoxels);
  // start scaling, stop when the desired number of voxels have been scaled
  while((normalScaleCount < normalScaleVoxels) || (dentriticScaleCount < dentriticScaleVoxels)) {
    // let's calculate the chance of having a dentriticd voxel
    randomNumber = rand()%HUNDREDPERCENT;
    // let's generate at least 10% of normal voxels before generating any agglomeration
    if (normalScaleCount < (normalScaleVoxels / 10)){
      randomNumber = HUNDREDPERCENT;
    }
    if((randomNumber >= dentriticChance) && (normalScaleCount < normalScaleVoxels)) {
      // let's generate a normal scale voxel
      scaleNormalVoxel();
      ++normalScaleCount;
    }

    else if (dentriticScaleCount < dentriticScaleVoxels){
      // let's generate an dentriticd voxel
      dentriticScaleCount = dentriticScaleCount + scaleDentriticVoxel(dentriteCellSize, dendriteSpacing);
      //  cout << dentriticScaleCount << "\n";
    }// else

    //  // Update the progress bar every 0xFFFF (65535) steps. Updating the scale bar is VERY costly since it requires redrawing it on the screen
    // if (((dentriticScaleCount+normalScaleCount) & 0x1FF)==0x1FF) {
    //   progressBar->updateStep(dentriticScaleCount+normalScaleCount);
    //   // let's check if the user has canceled us
    //   if (progressBar->wasCanceled()) {
    //     // close the progress bar
    //     progressBar->close();
    //     return false;
    //   }
    // }
  }// while

  // close the progress bar
  //progressBar->close();

  return true;
}

// -------------------------------------------------------------------------
void CEncrustracao::clearScale() {
  //Thiago: assertmsg(pimagem3D, "Error. pimagem3D should not have a NULL value.");
  // Coloquei no construtor
  std::vector<int>**  ptrDataImagem3D = pimagem3D->Data3D();
  for(int x=0; x < dim_x; x++) {
    for(int y=0; y < dim_y; y++) {
      for(int z=0; z < dim_z; z++) {
        if (isScale(ptrDataImagem3D, x,y,z)) {
          ptrDataImagem3D[x][y][z]=PORE;
        }
      }
    }
  }
}

// -------------------------------------------------------------------------
void CEncrustracao::scale2RockMatrix() {
  //Thiago: assertmsg(pimagem3D, "Error. pimagem3D should not have a NULL value.");
  // Coloquei no construtor
  std::vector<int>**  ptrDataImagem3D = pimagem3D->Data3D();
  for(int x=0; x < dim_x; x++) {
    for(int y=0; y < dim_y; y++) {
      for(int z=0; z < dim_z; z++) {
        if (isScale(ptrDataImagem3D, x,y,z)) {
          ptrDataImagem3D[x][y][z]=ROCKMATRIX;
        }
      }
    }
  }
}

// -------------------------------------------------------------------------
bool CEncrustracao::hasNeighborDiags(int x, int y, int z){
  std::vector<int>**  ptrDataImagem3D = pimagem3D->Data3D();
  // lower region
  if ((z!=dim_z-1) && (x!=dim_x-1))
    if((ptrDataImagem3D[x+1][y][z+1]!=PORE)) return true;
  if ((z!=dim_z-1) && ((x!=0)))
    if((ptrDataImagem3D[x-1][y][z+1]!=PORE)) return true;
  if ((z!=dim_z-1) && (y!=dim_y-1))
    if((ptrDataImagem3D[x][y+1][z+1]!=PORE)) return true;
  if ((z!=dim_z-1) && (y!=0))
    if((ptrDataImagem3D[x][y-1][z+1]!=PORE)) return true;
  // upper region
  if ((z!=0) && (x!=dim_x-1))
    if((ptrDataImagem3D[x+1][y][z-1]!=PORE)) return true;
  if ((z!=0) && ((x!=0)))
    if((ptrDataImagem3D[x-1][y][z-1]!=PORE)) return true;
  if ((z!=0) && (y!=dim_y-1))
    if((ptrDataImagem3D[x][y+1][z-1]!=PORE)) return true;
  if ((z!=0) && (y!=0))
    if((ptrDataImagem3D[x][y-1][z-1]!=PORE)) return true;
  // middle region
  if ((x!=dim_x-1) && (y!=0))
    if((ptrDataImagem3D[x+1][y-1][z]!=PORE)) return true;
  if ((x!=dim_x-1) && (y!=dim_y-1))
    if((ptrDataImagem3D[x+1][y+1][z]!=PORE)) return true;
  if ((x!=0) && (y!=0))
    if((ptrDataImagem3D[x-1][y-1][z]!=PORE)) return true;
  if ((x!=0) && (y!=dim_y-1))
    if((ptrDataImagem3D[x-1][y+1][z]!=PORE)) return true;

  return false;
}

// -------------------------------------------------------------------------
int CEncrustracao::numNeighborDiags(int x, int y, int z){
  std::vector<int>**  ptrDataImagem3D = pimagem3D->Data3D();
  int numNDiags = 0;
  // lower region
  if ((z!=dim_z-1) && (x!=dim_x-1))
    if((ptrDataImagem3D[x+1][y][z+1]!=PORE)) ++numNDiags;
  if ((z!=dim_z-1) && ((x!=0)))
    if((ptrDataImagem3D[x-1][y][z+1]!=PORE)) ++numNDiags;
  if ((z!=dim_z-1) && (y!=dim_y-1))
    if((ptrDataImagem3D[x][y+1][z+1]!=PORE)) ++numNDiags;
  if ((z!=dim_z-1) && (y!=0))
    if((ptrDataImagem3D[x][y-1][z+1]!=PORE)) ++numNDiags;
  // upper region
  if ((z!=0) && (x!=dim_x-1))
    if((ptrDataImagem3D[x+1][y][z-1]!=PORE)) ++numNDiags;
  if ((z!=0) && ((x!=0)))
    if((ptrDataImagem3D[x-1][y][z-1]!=PORE)) ++numNDiags;
  if ((z!=0) && (y!=dim_y-1))
    if((ptrDataImagem3D[x][y+1][z-1]!=PORE)) ++numNDiags;
  if ((z!=0) && (y!=0))
    if((ptrDataImagem3D[x][y-1][z-1]!=PORE)) ++numNDiags;
  // middle region
  if ((x!=dim_x-1) && (y!=0))
    if((ptrDataImagem3D[x+1][y-1][z]!=PORE)) ++numNDiags;
  if ((x!=dim_x-1) && (y!=dim_y-1))
    if((ptrDataImagem3D[x+1][y+1][z]!=PORE)) ++numNDiags;
  if ((x!=0) && (y!=0))
    if((ptrDataImagem3D[x-1][y-1][z]!=PORE)) ++numNDiags;
  if ((x!=0) && (y!=dim_y-1))
    if((ptrDataImagem3D[x-1][y+1][z]!=PORE)) ++numNDiags;
  return numNDiags;
}

// -------------------------------------------------------------------------
int CEncrustracao::numScaleNeighbors(int x, int y, int z){
  std::vector<int>**  ptrDataImagem3D = pimagem3D->Data3D();
  int numScaleNeighbors = 0;
  if (z!=0)
    if((ptrDataImagem3D[x][y][z-1]==NORMALSCALE)||(ptrDataImagem3D[x][y][z-1]==AGGLOMERATESCALE)) ++numScaleNeighbors;
  if (z!=dim_z-1)
    if((ptrDataImagem3D[x][y][z+1]==NORMALSCALE)||(ptrDataImagem3D[x][y][z+1]==AGGLOMERATESCALE)) ++numScaleNeighbors;
  if (y!=0)
    if((ptrDataImagem3D[x][y-1][z]==NORMALSCALE)||(ptrDataImagem3D[x][y-1][z]==AGGLOMERATESCALE)) ++numScaleNeighbors;
  if (y!=dim_y-1)
    if((ptrDataImagem3D[x][y+1][z]==NORMALSCALE)||(ptrDataImagem3D[x][y+1][z]==AGGLOMERATESCALE)) ++numScaleNeighbors;
  if (x!=0)
    if((ptrDataImagem3D[x-1][y][z]==NORMALSCALE)||(ptrDataImagem3D[x-1][y][z]==AGGLOMERATESCALE)) ++numScaleNeighbors;
  if (x!=dim_x-1)
    if((ptrDataImagem3D[x+1][y][z]==NORMALSCALE)||(ptrDataImagem3D[x+1][y][z]==AGGLOMERATESCALE)) ++numScaleNeighbors;

  return numScaleNeighbors;
}

// -------------------------------------------------------------------------
bool CEncrustracao::hasScaleNeighbor(int x, int y, int z){
  std::vector<int>**  ptrDataImagem3D = pimagem3D->Data3D();
  if (z!=0)
    if((ptrDataImagem3D[x][y][z-1]==NORMALSCALE)||(ptrDataImagem3D[x][y][z-1]==AGGLOMERATESCALE)) return true;
  if (z!=dim_z-1)
    if((ptrDataImagem3D[x][y][z+1]==NORMALSCALE)||(ptrDataImagem3D[x][y][z+1]==AGGLOMERATESCALE)) return true;
  if (y!=0)
    if((ptrDataImagem3D[x][y-1][z]==NORMALSCALE)||(ptrDataImagem3D[x][y-1][z]==AGGLOMERATESCALE)) return true;
  if (y!=dim_y-1)
    if((ptrDataImagem3D[x][y+1][z]==NORMALSCALE)||(ptrDataImagem3D[x][y+1][z]==AGGLOMERATESCALE)) return true;
  if (x!=0)
    if((ptrDataImagem3D[x-1][y][z]==NORMALSCALE)||(ptrDataImagem3D[x-1][y][z]==AGGLOMERATESCALE)) return true;
  if (x!=dim_x-1)
    if((ptrDataImagem3D[x+1][y][z]==NORMALSCALE)||(ptrDataImagem3D[x+1][y][z]==AGGLOMERATESCALE)) return true;

  return false;
}

// -------------------------------------------------------------------------
bool CEncrustracao::hasNonPorousNeighbor(int x, int y, int z){
  std::vector<int>**  ptrDataImagem3D = pimagem3D->Data3D();
  if (z!=0)
    if(ptrDataImagem3D[x][y][z-1]!=PORE) return true;
  if (z!=dim_z-1)
    if(ptrDataImagem3D[x][y][z+1]!=PORE) return true;
  if (y!=0)
    if(ptrDataImagem3D[x][y-1][z]!=PORE) return true;
  if (y!=dim_y-1)
    if(ptrDataImagem3D[x][y+1][z]!=PORE) return true;
  if (x!=0)
    if(ptrDataImagem3D[x-1][y][z]!=PORE) return true;
  if (x!=dim_x-1)
    if(ptrDataImagem3D[x+1][y][z]!=PORE) return true;

  return false;
}

// --------------------------------------------------------------------------
