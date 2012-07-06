/*This head file is used for allocating meomery in integer 3-d matrix.*/

#include </usr/lpp/xlC/include/new.h>

#ifndef __Matrix3di_h
#define __Matrix3di_h

#define TIPO  int

TIPO ***
Matrix3di (int NX, int NY, int NZ)
{


  int ***x;

  x = new TIPO **[NX];

  for (int i = 0; i < NX; i++)
    {
      x[i] = new TIPO *[NY];

      for (int j = 0; j < NY; j++)
	x[i][j] = new int[NZ];
    }

  return (x);
}

void
DeleteMatrix3di (TIPO *** x)
{

  delete[]x[0][0];
  delete[]x[0];
  delete[]x;
}


#endif
