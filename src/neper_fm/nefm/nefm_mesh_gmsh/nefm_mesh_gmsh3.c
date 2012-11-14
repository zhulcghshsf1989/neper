/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nefm_mesh_gmsh.h"


void
nefm_mesh_2d_gmsh_writeface (struct GEO Geo, struct MESH Mesh1D, int
			     face, double cl, FILE * file)
{
  int i, j, EdgeQty, edge, ori;

  EdgeQty = Geo.FaceVerQty[face];
  fprintf (file, "Line Loop(%d) = {", face);

  for (i = 1; i <= EdgeQty; i++)
  {
    ori = Geo.FaceEdgeOri[face][i];
    edge = Geo.FaceEdgeNb[face][i];

    if (ori == 1)
      for (j = 1; j <= Mesh1D.Elsets[edge][0]; j++)
      {
	if (i != EdgeQty || j != Mesh1D.Elsets[edge][0])
	  fprintf (file, "%d,", Mesh1D.Elsets[edge][j]);
	else
	  fprintf (file, "%d};\n", Mesh1D.Elsets[edge][j]);
      }
    else
      for (j = Mesh1D.Elsets[edge][0]; j >= 1; j--)
      {
	if (i != EdgeQty || j != 1)
	  fprintf (file, "-%d,", Mesh1D.Elsets[edge][j]);
	else
	  fprintf (file, "-%d};\n", Mesh1D.Elsets[edge][j]);
      }
  }

  fprintf (file, "Plane Surface(%d) = {%d}; ", face, face);

  fprintf (file, "Physical Surface(%d) = {%d};\n", face, face);

  // fprintf (file, "Mesh.CharacteristicLengthMax = %f;\n", cl);
  // above does not work on all architecture.
  
  cl = cl;

  return;
}

void
nefm_mesh_2d_gmsh_writeface_b (double cl, FILE * file)
{
  fprintf (file, "Merge \"tmp-surf.msh\";\n");
  fprintf (file, "Line Loop (1) = {1};\n");
  fprintf (file, "Surface (1) = {1};\n");

  if (cl > 0)
    fprintf (file, "Mesh.CharacteristicLengthMax = %.12f;\n", cl);

  return;
}

void
nefm_mesh_3d_gmsh_writepoly (double cl, FILE * file)
{
  fprintf (file, "Merge \"tmp-surf.msh\";\n");
  fprintf (file, "Surface Loop (1) = {1};\n");
  fprintf (file, "Volume (1) = {1};\n");

  if (cl > 0)
    fprintf (file, "Mesh.CharacteristicLengthMax = %.12f;\n", cl);

  return;
}
