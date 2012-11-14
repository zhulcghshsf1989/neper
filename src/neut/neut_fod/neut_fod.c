/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_fod.h"

void
neut_fod_fodnodes (int **FoD, struct MESH Mesh2D, int **NSets)
{
  int i, j, nbnodes;
  int **FaceNodes;

  for (i = 1; i <= 6; i++)
  {
    FaceNodes = ut_alloc_1d_pint (FoD[i][0] + 1);

    nbnodes = 0;
    for (j = 1; j <= FoD[i][0]; j++)
    {
      /* neut_mesh_elset_nodes should be equivalent */
      ThisFaceNodes (Mesh2D, FoD[i][j], &(FaceNodes[j]));
      nbnodes += FaceNodes[j][0];
    }

    NSets[i] = ut_alloc_1d_int (nbnodes + 1);

    for (j = 1; j <= FoD[i][0]; j++)
    {
      oneDIntCpy (FaceNodes[j], 1, FaceNodes[j][0], NSets[i],
		  NSets[i][0] + 1, 0);
      NSets[i][0] += FaceNodes[j][0];
    }

    oneDIntSort (NSets[i], 1, NSets[i][0]);
    NSets[i][0] =
      oneDIntCompress (NSets[i], 1, NSets[i][0]);

    twoDIntFree (FaceNodes, FoD[i][0] + 1);
  }

  return;
}

/*
void
neut_fod_mesh_nsets (int **FoD, struct MESH Mesh2D, int ***pNSets)
{
  int i, j, nbnodes;
  int **FaceNodes;

  for (i = 1; i <= 6; i++)
  {
    FaceNodes = ut_alloc_1d_pint (FoD[i][0] + 1);

    nbnodes = 0;
    for (j = 1; j <= FoD[i][0]; j++)
    {
      ThisFaceNodes (Mesh2D, FoD[i][j], &(FaceNodes[j]));
      nbnodes += FaceNodes[j][0];
    }

    (*pNSets[i] = ut_alloc_1d_int (nbnodes + 1);

    for (j = 1; j <= FoD[i][0]; j++)
    {
      oneDIntCpy (FaceNodes[j], 1, FaceNodes[j][0], NSets[i],
		  NSets[i][0] + 1, 0);
      NSets[i][0] += FaceNodes[j][0];
    }

    oneDIntSort (NSets[i], 1, NSets[i][0]);
    NSets[i][0] =
      oneDIntCompress (NSets[i], 1, NSets[i][0]);

    twoDIntFree (FaceNodes, FoD[i][0] + 1);
  }

  return;
}
*/
