/***************************************************************************
 *   Copyright (C) 2004 by Tiago Ribeiro Schaewer - LENEP/UENF             *
 *   Tiago Ribeiro Schaewer: tschaewer@rc01   - 2004                       *
 *   André Duarte Bueno: bueno@lenep.uenf.br  - 2024                       *
 *   This program is free software.                                        *
 ***************************************************************************/
#ifndef CENCRUSTRACAO_H
#define CENCRUSTRACAO_H

#ifndef TCImagem3D_h
#include <MetNum/Matriz/TCImagem3D.h>
#endif
#include <cassert>

// --------------------------------------------------------------------------
// Veio do arquivo error.h
// [USAR assert de C++20? ficaria incompatível com versões antigas]
#ifdef NDEBUG
 #define assertmsg(exp, msg)     ((void)0)
#else
 #define assertmsg(exp,msg)  if (exp==false) { cerr << msg << "\n"; assert(false);}
#endif

// ---------------------------------------------------------------------------
// Veio do arquivo globais.h
// Estrutura pair_float
// Dúvida: porque não usou pair<>? -> velocidade? fez testes?
typedef struct pair_float{ float x; float y;} spair_float;

/** @short Returns true if a is the pair with the smallest x value.
    @remarks Used by stl sort algorithm. */
bool operator < (const pair_float &a, const pair_float &b);

/// @short Returns the absolute difference between two floats.
float absdiff(float a, float b);

/// Definições de valores dos píxies da imagem
#define ROCKMATRIX 0
#define PORE 1
#define NORMALSCALE 2
#define AGGLOMERATESCALE 3

#define RED   1
#define GREEN 2
#define BLUE  3

#define INVALIDVALUE -1

/// We will have precision up to 0,01%. 10000 equals 100%, 1 equals 0,01%. Let's avoid floating point numbers.
/// Teremos precisão de até 0,01%. 10.000 é igual a 100%, 1 é igual a 0,01%. Vamos evitar números de ponto flutuante.
#define HUNDREDPERCENT 10000

/// Destruição segura
#define SAFE_DELETE(p)   { if(p) { delete (p); (p)=nullptr; } }

/// @author Tiago Ribeiro Schaewer - LENEP/UENF
class CEncrustracao {
public:
    CEncrustracao(TCImagem3D<int>* _pmatrix);
    ~CEncrustracao();

    /** @short Scale the 3D matrix reducing the porosity by percentPorosityScale.
        @param percentPorosityScale must be a value between 1 and 10000 (100%) */
    bool scalePercent(int percentPorosityScale);

    /** @short Scale the 3D matrix filling a number of voxels.
        @param normalScaleVoxels number of voxels which may be generated next to any non-porous voxel.
	@param agglomeratedScaleVoxels number of voxels which may be generated next to an already scaled voxel.*/
    //bool scaleAgglomerate(long long normalScaleVoxels, long long agglomerateScaleVoxels);
    bool scalePorousAgglomerate(long long normalScaleVoxels, long long agglomerateScaleVoxels, float microPorosity);
    bool scaleAgglomerate(long long normalScaleVoxels, long long agglomerateScaleVoxels);
    bool scaleAgglomeratePercentage(int percentPorosityScale, int agglomerateShare, int microPorosity);

    bool scaleDentriticPercentage(int percentPorosityScale, int dentriticShare);
    bool scaleDentritic(long long normalScaleVoxels, long long dentriticScaleVoxels);

    /** @short Clear all scaled voxels. */
    void clearScale();
    /** @short Converts incrustation to rock matrix.
        @remarks This method was created so that images may be saved in the imago format which does not allow incrustation. */
    void scale2RockMatrix();
private:
  /** @short Pointer to the 3d matrix wich will be scaled. */
  TCImagem3D<int>* pimagem3D;
  // criar ptr para dados onde seta  pimagem3D
  // vector<int>**  ptrDataImagem3D = pimagem3D->Data3D();
  /** @short The dimensions of the matrix. */
  int dim_x, dim_y, dim_z;

  /** @short returns true if any diagonal voxel in the 18-neighborhood is non-porous. */
  bool hasNeighborDiags(int x, int y, int z);
  /** @short returns the number of diagonal voxels in the 18-neighborhood wich are non-porous. */
  inline int  numNeighborDiags(int x, int y, int z);
  /** @short returns the number of scale voxels in the 6-neighborhood. */
  inline int numScaleNeighbors(int x, int y, int z);
  /** @short returns true if any voxel in the 6-neighborhood is a scale. */
  bool hasScaleNeighbor(int x, int y, int z);
  /** @short returns true if any voxel in the 6-neighborhood is not a pore. */
  bool hasNonPorousNeighbor(int x, int y, int z);

  bool scaleAgglomerateVoxel();
  bool scaleNormalVoxel();
  int scaleDentriticVoxel(int dentriteCellSize, int dendriteSpacing);

  // ----------------------------
  // Códigos que vieram da classe Matriz3D do Thiago mas que trouxe para cá porque se referem a dano de formação
private:
  // Na Matriz3D Thiago:
  // inline bool isScale(int x, int y, int z) { return ((data3D[x][y][z]==NORMALSCALE) ||(data3D[x][y][z]==AGGLOMERATESCALE)); };
 inline bool isScale(vector<int>**  data3D,int x, int y, int z) {  return ((data3D[x][y][z]==NORMALSCALE) ||(data3D[x][y][z]==AGGLOMERATESCALE));  }
  // Na Matriz3D Thiago:
  //  inline bool isPorous(int x, int y, int z) { return (data3D[x][y][z]==PORE); };
  inline bool isPorous(vector<int>**  data3D, int x, int y, int z) { return (data3D[x][y][z]==PORE); };
  // Na Matriz3D Thiago:
  // inline bool isRock(int x, int y, int z) { return (data3D[x][y][z]==ROCKMATRIX); };
  inline bool isRock(vector<int>**  data3D, int x, int y, int z) { return (data3D[x][y][z]==ROCKMATRIX); };

  // Na matriz: long long getNoPorousVoxels(); pimagem3D->getNoPorousVoxels();
  long long getNoPorousVoxels(TCMatriz3D<int>* pimagem3D) {
    std::vector<int>**  ptrDataImagem3D = pimagem3D->Data3D();
    long long porousCount = 0;
    int nx = pimagem3D->NX();
    int ny = pimagem3D->NY();
    int nz = pimagem3D->NZ();
    for(int x=0; x < nx; x++)
      for(int y=0; y < ny; y++)
        for(int z=0; z < nz; z++)
          //if (isPorous(ptrDataImagem3D,x,y,z))
          if(ptrDataImagem3D[x][y][z]==PORE)
            ++porousCount;
    return porousCount;
  }

  inline void getRandomVoxelCoordinate(TCMatriz3D<int>* pimagem3D, int& x, int& y, int& z) {
    x = rand() % pimagem3D->NX();
    y = rand() % pimagem3D->NY();
    z = rand() % pimagem3D->NZ();
  }

  int numRockNeighbors(TCImagem3D<int>* pimagem3D, int x, int y, int z){
  int nx = pimagem3D->NX();
  int ny = pimagem3D->NY();
  int nz = pimagem3D->NZ();
  int numRockVoxels = 0;
  std::vector<int>**  ptrDataImagem3D = pimagem3D->Data3D();
  if (z!=0)    if(isRock(ptrDataImagem3D,x,y,z-1)) ++numRockVoxels;
  if (z!=nz-1) if(isRock(ptrDataImagem3D,x,y,z+1)) ++numRockVoxels;
  if (y!=0)    if(isRock(ptrDataImagem3D,x,y-1,z)) ++numRockVoxels;
  if (y!=ny-1) if(isRock(ptrDataImagem3D,x,y+1,z)) ++numRockVoxels;
  if (x!=0)    if(isRock(ptrDataImagem3D,x-1,y,z)) ++numRockVoxels;
  if (x!=nx-1) if(isRock(ptrDataImagem3D,x+1,y,z)) ++numRockVoxels;
  return numRockVoxels;
}
  // não esta sendo usada
  int numNonPorousNeighbors(TCImagem3D<int>* pimagem3D, int x, int y, int z){
  int numNonPorousVoxels = 0;
  int nx = pimagem3D->NX();
  int ny = pimagem3D->NY();
  int nz = pimagem3D->NZ();
  std::vector<int>**  ptrDataImagem3D = pimagem3D->Data3D();
  if (z!=0)    if(!isPorous(ptrDataImagem3D,x,y,z-1)) ++numNonPorousVoxels;
  if (z!=nz-1) if(!isPorous(ptrDataImagem3D,x,y,z+1)) ++numNonPorousVoxels;
  if (y!=0)    if(!isPorous(ptrDataImagem3D,x,y-1,z)) ++numNonPorousVoxels;
  if (y!=ny-1) if(!isPorous(ptrDataImagem3D,x,y+1,z)) ++numNonPorousVoxels;
  if (x!=0)    if(!isPorous(ptrDataImagem3D,x-1,y,z)) ++numNonPorousVoxels;
  if (x!=nx-1) if(!isPorous(ptrDataImagem3D,x+1,y,z)) ++numNonPorousVoxels;
  return numNonPorousVoxels;
}

  // Na matriz: long long getNoConnectedVoxels(TRotulador3D* rot);
  // long long getNoConnectedVoxels( TRotulador3D* rot); codigo?
  // não usa aqui; mas tem de copiar este código para TCImagem3D

};
#endif
