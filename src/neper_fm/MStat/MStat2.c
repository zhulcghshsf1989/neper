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
  double **EltAngle = NULL;
  double* RRatio = NULL;
  double* MeanLength = NULL;
  double vol;
  double* coo = ut_alloc_1d (3);
  int poly_true;
  int dim;
  int* print = ut_alloc_1d_int (6);

  if (! strcmp (In.printstatmesh, "1") || ! strcmp (In.printstatmesh, "all"))
    ut_array_1d_int_set (print, 6, 1);
  else if (ut_string_inlist (In.printstatmesh, ',', "stn"))
    print[0] = 1;
  else if (ut_string_inlist (In.printstatmesh, ',', "stm1"))
    print[1] = 1;
  else if (ut_string_inlist (In.printstatmesh, ',', "stm2"))
    print[2] = 1;
  else if (ut_string_inlist (In.printstatmesh, ',', "stm3"))
    print[3] = 1;
  else if (ut_string_inlist (In.printstatmesh, ',', "stm4"))
    print[4] = 1;
  else if (ut_string_inlist (In.printstatmesh, ',', "stm5"))
    print[5] = 1;

  if (print[0])
  {
    file = ut_file_open (In.stn, "w");
    for (i = 1; i <= Nodes.NodeQty; i++)
    {
      dim = neut_nodes_eltdim (Mesh0D, Mesh1D, Mesh2D, Mesh3D, i);

      fprintf (file, "%d %.12f %.12f %.12f %d\n", i,
	  Nodes.NodeCoo[i][0], Nodes.NodeCoo[i][1], Nodes.NodeCoo[i][2], dim);
    }
    ut_file_close (file, In.stn, "w");
  }

  if (print[1])
  {
    // stm1: element non-quality properties
    file = ut_file_open (In.stm1, "w");
    
    CalcMeanLength (Nodes, Mesh3D, &MeanLength);

    for (i = 1; i <= Mesh3D.EltQty; i++)
    {
      if (pGeo != NULL && (*pGeo).PolyQty > 0)
	poly_true = (*pGeo).PolyTrue[Mesh3D.EltElset[i]];
      else
	poly_true = 0;

      neut_mesh_elt_volume (Nodes, Mesh3D, i, &vol);
      neut_mesh_eltcentre (Mesh3D, Nodes, i, coo);
      fprintf (file, "%d %d %d %g %g %.12f %.12f %.12f\n", \
	  i, Mesh3D.EltElset[i], poly_true, vol, MeanLength[i], coo[0], coo[1], coo[2]);
    }
    ut_file_close (file, In.stm1, "w");
  }

  if (print[2])
  {
    // stm2: element quality properties
    file = ut_file_open (In.stm2, "w");

    CalcAngle (Nodes, Mesh3D, &EltAngle);
    CalcRRatio (Nodes, Mesh3D, &RRatio);

    for (i = 1; i <= Mesh3D.EltQty; i++)
    {
      if (pGeo != NULL && (*pGeo).PolyQty > 0)
	poly_true = (*pGeo).PolyTrue[Mesh3D.EltElset[i]];
      else
	poly_true = 0;

      fprintf (file, "%d %d %d %f %f\n", i, Mesh3D.EltElset[i], poly_true, RRatio[i], EltAngle[i][0]);
    }
    ut_file_close (file, In.stm2, "w");

    ut_free_1d (RRatio);
    ut_free_2d (EltAngle, Mesh3D.EltQty + 1);
  }

  if (print[3])
  {
    CalcAngle (Nodes, Mesh3D, &EltAngle);

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

    ut_free_2d (EltAngle, Mesh3D.EltQty + 1);
  }

  if (print[4])
  {
    // stm4: elset non-quality properties
    file = ut_file_open (In.stm4, "w");
    for (i = 1; i <= Mesh3D.ElsetQty; i++)
    {
      neut_mesh_elset_volume (Nodes, Mesh3D, i, &vol);
      neut_mesh_elset_centre (Nodes, Mesh3D, i, coo);
      fprintf (file, "%d %d %g %f %f %f\n", i, Mesh3D.Elsets[i][0], vol, coo[0], coo[1], coo[2]);
    }
    ut_file_close (file, In.stm4, "w");
  }

  if (print[5])
  {
    // stm5: elset quality properties
    file = ut_file_open (In.stm5, "w");
    double dispfromcl, rrmean, rrmin, rrmax;
    for (i = 1; i <= Mesh3D.ElsetQty; i++)
    {
      neut_mesh_elset_volume (Nodes, Mesh3D, i, &vol);
      neut_mesh_elset_centre (Nodes, Mesh3D, i, coo);
      neut_mesh_elset_dispfromcl (Nodes, Mesh3D, i, GeoPara.cl, &dispfromcl);
      neut_mesh_elset_3d_rr (Nodes, Mesh3D, i, &rrmean, &rrmin, &rrmax);

      int true_val, body_val;
      if (pGeo != NULL && (*pGeo).PolyQty > 0)
      {
	true_val = (*pGeo).PolyTrue[i];
	body_val = (*pGeo).PolyBody[i];
      }
      else
      {
	true_val = 0;
	body_val = 0;
      }

      fprintf (file, "%d %d %d %d %g %g\n", i, true_val, body_val, Mesh3D.Elsets[i][0], rrmin, pow (dispfromcl, 3) );
    }
    ut_file_close (file, In.stm5, "w");
  }

  /* freeing memory */
  ut_free_1d (coo);
  ut_free_1d_int (print);

  return;
}
