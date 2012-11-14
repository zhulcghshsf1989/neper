/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"MStat.h"

void
PrintEStat (struct NODES Nodes, struct MESH Mesh0D, struct MESH Mesh1D,
            struct MESH Mesh2D, struct MESH Mesh3D, 
            struct IN In, struct GEOPARA GeoPara, struct GEO* pGeo)
{
  int i, j;
  FILE *file;
  double *EltRho, **EltAngle;
  double* RRatio;
  double* MeanLength;
  double vol;
  double* coo = ut_alloc_1d (3);
  int poly_true;
  int dim;

  file = ut_file_open (In.stn, "w");
  for (i = 1; i <= Nodes.NodeQty; i++)
  {
    dim = neut_nodes_eltdim (Mesh0D, Mesh1D, Mesh2D, Mesh3D, i);

    fprintf (file, "%d %.12f %.12f %.12f %d\n", i,
	Nodes.NodeCoo[i][0], Nodes.NodeCoo[i][1], Nodes.NodeCoo[i][2], dim);
  }
  ut_file_close (file, In.stn, "w");

  /* Calculating element statistics */
  CalcRho (Nodes, Mesh3D, &EltRho);
  CalcAngle (Nodes, Mesh3D, &EltAngle);
  CalcRRatio (Nodes, Mesh3D, &RRatio);
  CalcMeanLength (Nodes, Mesh3D, &MeanLength);

  // stm1: element non-quality properties
  file = ut_file_open (In.stm1, "w");
  for (i = 1; i <= Mesh3D.EltQty; i++)
  {
    if (pGeo != NULL)
      poly_true = (*pGeo).PolyTrue[Mesh3D.EltElset[i]];
    else
      poly_true = 0;

    neut_mesh_elt_volume (Nodes, Mesh3D, i, &vol);
    neut_mesh_eltcentre (Mesh3D, Nodes, i, coo);
    fprintf (file, "%d %d %d %g %g %.12f %.12f %.12f\n", \
	i, Mesh3D.EltElset[i], poly_true, vol, MeanLength[i], coo[0], coo[1], coo[2]);
  }
  ut_file_close (file, In.stm1, "w");

  // stm2: element quality properties
  file = ut_file_open (In.stm2, "w");
  for (i = 1; i <= Mesh3D.EltQty; i++)
  {
    if (pGeo != NULL)
      poly_true = (*pGeo).PolyTrue[Mesh3D.EltElset[i]];
    else
      poly_true = 0;

    fprintf (file, "%d %d %d %f %f\n", i, Mesh3D.EltElset[i], poly_true, RRatio[i], EltAngle[i][0]);
  }
  ut_file_close (file, In.stm2, "w");

  // stm3: element quality properties
  file = ut_file_open (In.stm3, "w");
  for (i = 1; i <= Mesh3D.EltQty; i++)
  {
    fprintf (file, "%d", i);
    for (j = 1; j <= 12; j++)
      fprintf (file, " %.2f", EltAngle[i][j]);
    fprintf (file, "\n");
  }
  ut_file_close (file, In.stm3, "w");

  // stm4: elset non-quality properties
  file = ut_file_open (In.stm4, "w");
  for (i = 1; i <= Mesh3D.ElsetQty; i++)
  {
    neut_mesh_elset_volume (Nodes, Mesh3D, i, &vol);
    neut_mesh_elset_centre (Nodes, Mesh3D, i, coo);
    fprintf (file, "%d %d %g %f %f %f\n", i, Mesh3D.Elsets[i][0], vol, coo[0], coo[1], coo[2]);
  }
  ut_file_close (file, In.stm4, "w");

  // stm5: elset quality properties
  file = ut_file_open (In.stm5, "w");
  double dispfromcl, rrmean, rrmin, rrmax;
  for (i = 1; i <= Mesh3D.ElsetQty; i++)
  {
    neut_mesh_elset_volume (Nodes, Mesh3D, i, &vol);
    neut_mesh_elset_centre (Nodes, Mesh3D, i, coo);
    neut_mesh_elset_dispfromcl (Nodes, Mesh3D, i, GeoPara.cl, &dispfromcl);
    neut_mesh_elset_3d_rr (Nodes, Mesh3D, i, &rrmean, &rrmin, &rrmax);

    fprintf (file, "%d %d %d %d %g %g\n", i, (*pGeo).PolyTrue[i], (*pGeo).PolyBody[i], Mesh3D.Elsets[i][0], rrmin, pow (dispfromcl, 3) );
  }
  ut_file_close (file, In.stm5, "w");

  /* freeing memory */
  ut_free_1d (RRatio);
  ut_free_1d (EltRho);
  ut_free_2d (EltAngle, 13);
  ut_free_1d (coo);

  return;
}
