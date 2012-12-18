/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_stat.h"

void
nem_stat_nodes (FILE* file, char* stn, struct NODES Nodes, struct MESH Mesh0D,
                struct MESH Mesh1D, struct MESH Mesh2D, struct MESH Mesh3D)
{
  int i, j, k, dim;
  double* coo = ut_alloc_1d (3);
  char** val = NULL;;
  int valqty;

  ut_string_separate (stn, ',', &val, &valqty);

  for (i = 1; i <= Nodes.NodeQty; i++)
    for (j = 0; j < valqty; j++)
    {
      ut_array_1d_memcpy (coo, 3, Nodes.NodeCoo[i]);
      for (k = 0; k < 3; k++)
	if (fabs (coo[k]) < 1e-12)
	  coo[k] = 0;

      if (! strcmp (val[j], "id"))
	fprintf (file, "%d", i);
      else if (! strcmp (val[j], "x"))
	fprintf (file, "%.12f", coo[0]);
      else if (! strcmp (val[j], "y"))
	fprintf (file, "%.12f", coo[1]);
      else if (! strcmp (val[j], "z"))
	fprintf (file, "%.12f", coo[2]);
      else if (! strcmp (val[j], "p"))
	fprintf (file, "%.12f %.12f %.12f", coo[0], coo[1], coo[2]);
      else if (! strcmp (val[j], "dim"))
      {
	dim = neut_mesh_node_dim (Mesh0D, Mesh1D, Mesh2D, Mesh3D, i);
	fprintf (file, "%d", dim);
      }

      if (j < valqty - 1)
	fprintf (file, " ");
      else
	fprintf (file, "\n");
    }

  ut_free_1d (coo);

  return;
}

void
nem_stat_elts (FILE* file, char* ste, struct NODES Nodes, struct MESH Mesh3D, 
	       struct TESS Tess)
{
  int i, j, t, b;
  double vol, length, rr;
  double* coo = ut_alloc_1d (3);
  char** val = NULL;
  int valqty;

  ut_string_separate (ste, ',', &val, &valqty);

  for (i = 1; i <= Mesh3D.EltQty; i++)
    for (j = 0; j < valqty; j++)
    {
      neut_mesh_elt_centre (Mesh3D, Nodes, i, coo);

      if (! strcmp (val[j], "id"))
	fprintf (file, "%d", i);
      else if (! strcmp (val[j], "x"))
	fprintf (file, "%.12f", coo[0]);
      else if (! strcmp (val[j], "y"))
	fprintf (file, "%.12f", coo[1]);
      else if (! strcmp (val[j], "z"))
	fprintf (file, "%.12f", coo[2]);
      else if (! strcmp (val[j], "p"))
	fprintf (file, "%.12f %.12f %.12f", coo[0], coo[1], coo[2]);
      else if (! strcmp (val[j], "true"))
      {
	if (Tess.PolyQty > 0)
	  t = Tess.PolyTrue[Mesh3D.EltElset[i]];
	else
	  t = 0;

	fprintf (file, "%d", t);
      }
      else if (! strcmp (val[j], "body"))
      {
	if (Tess.PolyQty > 0)
	  b = Tess.PolyBody[Mesh3D.EltElset[i]];
	else
	  b = 0;

	fprintf (file, "%d", b);
      }
      else if (! strcmp (val[j], "elset"))
	fprintf (file, "%d", Mesh3D.EltElset[i]);
      else if (! strcmp (val[j], "vol"))
      {
	neut_mesh_elt_volume (Nodes, Mesh3D, i, &vol);
	fprintf (file, "%.12g", vol);
      }
      else if (! strcmp (val[j], "length"))
      {
	neut_mesh_elt_lengths (Mesh3D, Nodes, i, &length, NULL, NULL);
	fprintf (file, "%.12g", length);
      }
      else if (! strcmp (val[j], "rr"))
      {
	neut_mesh_elt_rr (Nodes, Mesh3D, i, &rr);
	fprintf (file, "%.12f", rr);
      }

      if (j < valqty - 1)
	fprintf (file, " ");
      else
	fprintf (file, "\n");
    }

  ut_free_1d (coo);

  return;
}

void
nem_stat_elsets (FILE* file, char* ste, struct NODES Nodes, struct MESH Mesh3D, 
	         struct TESSPARA TessPara, struct TESS Tess)
{
  int i, j, t, b;
  double vol, rrmean, rrmin, rrmax, Osize;
  double* coo = ut_alloc_1d (3);
  char** val = NULL;
  int valqty;

  ut_string_separate (ste, ',', &val, &valqty);

  for (i = 1; i <= Mesh3D.ElsetQty; i++)
    for (j = 0; j < valqty; j++)
    {
      neut_mesh_elset_centre (Nodes, Mesh3D, i, coo);
      neut_mesh_elset_rr (Nodes, Mesh3D, i, &rrmean, &rrmin, &rrmax);

      if (Tess.PolyQty > 0)
      {
	t = Tess.PolyTrue[i];
	b = Tess.PolyBody[i];
      }
      else
      {
	t = 0;
	b = 0;
      }

      if (! strcmp (val[j], "id"))
	fprintf (file, "%d", i);
      else if (! strcmp (val[j], "eltnb"))
	fprintf (file, "%d", Mesh3D.Elsets[i][0]);
      else if (! strcmp (val[j], "x"))
	fprintf (file, "%.12f", coo[0]);
      else if (! strcmp (val[j], "y"))
	fprintf (file, "%.12f", coo[1]);
      else if (! strcmp (val[j], "z"))
	fprintf (file, "%.12f", coo[2]);
      else if (! strcmp (val[j], "p"))
	fprintf (file, "%.12f %.12f %.12f", coo[0], coo[1], coo[2]);
      else if (! strcmp (val[j], "vol"))
      {
	neut_mesh_elset_volume (Nodes, Mesh3D, i, &vol);
	fprintf (file, "%.12g", vol);
      }
      else if (! strcmp (val[j], "Osize"))
      {
	if (TessPara.cl > 0)
	{
	  neut_mesh_elset_Osize (Nodes, Mesh3D, i, TessPara.cl, &Osize);
	  fprintf (file, "%.12f", Osize);
	}
	else
	  fprintf (file, "-1");
      }
      else if (! strcmp (val[j], "rrav"))
	fprintf (file, "%.12f", rrmean);
      else if (! strcmp (val[j], "rrmin"))
	fprintf (file, "%.12f", rrmin);
      else if (! strcmp (val[j], "rrmax"))
	fprintf (file, "%.12f", rrmax);
      else if (! strcmp (val[j], "true"))
	fprintf (file, "%d", t);
      else if (! strcmp (val[j], "body"))
	fprintf (file, "%d", b);

      if (j < valqty - 1)
	fprintf (file, " ");
      else
	fprintf (file, "\n");
    }

  /* freeing memory */
  ut_free_1d (coo);
  ut_free_2d_char (val, valqty);

  return;
}
