/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"TestEdge.h"

int
TestEdgeLength (struct GEO Geo, int i)
{
  double eps = 1e-15;

  if (Geo.EdgeLength[i] < eps)
  {
    /*printf("edge %d has a zero-length.\n",i); */
    return 2;
  }

  return 0;
}

int
TestEdgeReciprocityVer (struct GEO Geo, int i)
{
  int j;
  int ver;

  for (j = 0; j <= 1; j++)
  {
    ver = Geo.EdgeVerNb[i][j];
    if (ut_array_1d_int_eltpos (Geo.VerEdgeNb[ver], Geo.VerEdgeQty[ver], i)
	== -1)
    {
      /*printf("ver %d of edge %d = ver %d has not edge %d\n",j,i,ver,i); */
      return 2;
    }
  }

  return 0;
}

int
TestEdgeReciprocityFace (struct GEO Geo, int i)
{
  int j;
  int face;

  /* this test could be more accurate (considering edge ver bounds) */
  if (Geo.EdgeFaceQty[i] < 2)
  {
    /*printf("edge %d has less than 2 = %d faces\n",i,Geo.EdgeFaceQty[i]); */
    return 2;
  }

  for (j = 0; j <= Geo.EdgeFaceQty[i] - 1; j++)
  {
    face = Geo.EdgeFaceNb[i][j];
    /*printf("\n");
       for(k=0;k<=Geo.FaceVerQty[face]-1;k++)
       printf("%d ",Geo.FaceEdgeNb[face][k]);
       printf("\n"); */

    if (ut_array_1d_int_eltpos (Geo.FaceEdgeNb[face] + 1, Geo.FaceVerQty[face], i)
	== -1)
    {
      /*printf("face %d of edge %d = face %d has not edge %d\n",j,i,face,i); */
      return 2;
    }
  }

  return 0;
}
