/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"Flatness.h"

/*
int* ReadMod(char* filename)
{
    int i,qty;
    int* data;
    FILE* mod=OpenFile(filename,"r");

    fscanf(mod,"%d",&qty);
    
    data=ut_alloc_1d_int(qty+1);

    data[0]=qty;
    for(i=1;i<=data[0];i++)
	fscanf(mod,"%d",&data[i]);

    CloseFile(mod);

    return data;
}
*/

void
Flatness (struct NODES Nodes, struct MESH Mesh2D,
	  int *ModifiedFaces, double **FaceMisN)
{
  int i, face;

  for (i = 1; i <= ModifiedFaces[0]; i++)
  {
    face = ModifiedFaces[i];
    FaceMisN[i][0] = ThisFaceFF (Nodes, Mesh2D, face, 0);
    FaceMisN[i][1] = ThisFaceFF (Nodes, Mesh2D, face, 1);
  }

  return;
}

void
WriteFaceFF (double **FaceMisN, int *ModifiedFaces, FILE * mff)
{
  int i, face;

  for (i = 1; i <= ModifiedFaces[0]; i++)
  {
    face = ModifiedFaces[i];
    fprintf (mff, "%d\t%5.2f\t%5.2f\n", face, FaceMisN[i][0], FaceMisN[i][1]);
  }

  return;
}
