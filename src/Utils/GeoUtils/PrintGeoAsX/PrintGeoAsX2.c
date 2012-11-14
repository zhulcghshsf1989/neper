/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "PrintGeoAsX.h"

void
PrintGeoFaceToPlot (struct GEO Geo, int *faces, int *FaceList)
{
  int i, j;

  /* recording faces to plot */
  for (i = 1; i <= Geo.FaceQty; i++)
    if (Geo.FaceState[i] >= 0)
      for (j = 0; j < 2; j++)
	if (Geo.FacePoly[i][j] < 0)
	{
	  if (oneDIntEltPos (faces, 1, faces[0],
			     -Geo.FacePoly[i][j], 0) != -1)
	  {
	    FaceList[0]++;
	    FaceList[FaceList[0]] = i;
	  }
	  break;
	}

  return;
}

void
PrintGeoFaceToPlot_poly (struct GEO Geo, int *poly, int *FaceList)
{
  int i, j, polynb;

  /* recording poly to plot */
  FaceList[0] = 0;
  for (i = 1; i <= poly[0]; i++)
  {
    polynb = poly[i];
    
    for (j = 1; j <= Geo.PolyFaceQty[polynb]; j++)
    {
      if (ut_array_1d_int_eltpos (FaceList + 1, FaceList[0], Geo.PolyFaceNb[polynb][j]) == -1)
      FaceList[++FaceList[0]] = Geo.PolyFaceNb[polynb][j];
    }
  }

  return;
}

void
PrintGeoEdgeToPlot (struct GEO Geo, int *FaceList, int *EdgeList)
{
  int i, j, face, edge;

  /* recording vertices and edges to plot */
  for (i = 1; i <= FaceList[0]; i++)
  {
    face = FaceList[i];

    for (j = 1; j <= Geo.FaceVerQty[face]; j++)
    {
      edge = Geo.FaceEdgeNb[face][j];
      if (ut_array_1d_int_eltpos (EdgeList + 1, EdgeList[0], edge) == -1)
      {
	EdgeList[0]++;
	EdgeList[EdgeList[0]] = edge;
      }
    }
  }

  return;
}

void
PrintGeoVerToPlot (struct GEO Geo, int *FaceList, int *VerList)
{
  int i, j, face, ver;

  /* recording vertices and edges to plot */
  for (i = 1; i <= FaceList[0]; i++)
  {
    face = FaceList[i];

    for (j = 1; j <= Geo.FaceVerQty[face]; j++)
    {
      ver = Geo.FaceVerNb[face][j];
      if (ut_array_1d_int_eltpos (VerList + 1, VerList[0], ver) == -1)
      {
	VerList[0]++;
	VerList[VerList[0]] = ver;
      }
    }
  }

  return;
}
