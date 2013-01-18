/* This file is part of the 'neper' program. */
/* Copyright (C) 5003-5012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_vox.h"

void
net_tess_vox (struct IN In, struct TESS Tess, VOX* pVox)
{
  int i, j, k, qty;
  int pid, prevpid;
  char* progress = ut_alloc_1d_char (100);
  double* coo = ut_alloc_1d (3);
  double** bbox = ut_alloc_2d (3, 2);

  neut_tess_bbox (Tess, bbox);

  (*pVox).PolyQty = Tess.PolyQty;

  (*pVox).size = ut_alloc_1d_int (3);
  net_vox_init_size (In.voxsizetype, In.voxsize, In.voxsize3, bbox, pVox);

  (*pVox).VoxPoly = ut_alloc_3d_int ((*pVox).size[0], (*pVox).size[1], (*pVox).size[2]);

  ut_print_progress (stdout, 0, (*pVox).size[0] * (*pVox).size[1] * (*pVox).size[2],
                     "%3.0f%%", progress);
  prevpid = 1;
  qty = 0;
  for (k = 0; k < (*pVox).size[2]; k++)
  {
    coo[2] = (*pVox).vsize[2] * ((double) k + 0.5);

    for (j = 0; j < (*pVox).size[1]; j++)
    {
      coo[1] = (*pVox).vsize[1] * ((double) j + 0.5);

      for (i = 0; i < (*pVox).size[0]; i++)
      {
	qty++;

	coo[0] = (*pVox).vsize[0] * ((double) i + 0.5);

	if (neut_tess_point_inpoly (Tess, coo, prevpid) == 1)
	  (*pVox).VoxPoly[i][j][k] = prevpid;
	else
	{
	  for (pid = 1; pid <= Tess.PolyQty; pid++)
	  {
	    if (pid == prevpid)
	      continue;

	    if (neut_tess_point_inpoly (Tess, coo, pid) == 1)
	    {
	      (*pVox).VoxPoly[i][j][k] = pid;
	      prevpid = pid;
	      break;
	    }
	  }
	}

	ut_print_progress (stdout, qty,
	    (*pVox).size[0] * (*pVox).size[1] * (*pVox).size[2],
	    "%3.0f%%", progress);
      }
    }
  }

  ut_free_1d_char (progress);
  ut_free_1d (coo);
  ut_free_2d (bbox, 3);

  return;
}

void
net_vox (struct IN In, struct TESS Domain, struct GERMSET GermSet, struct VOX* pVox)
{
  int i, j, k, l;
  double* coo = ut_alloc_1d (3);
  double* GermDist = ut_alloc_1d (GermSet.N + GermSet.nN + 1);
  double* bboxv = ut_alloc_1d (7);
  double** bbox = ut_alloc_2d (3, 2);
  int Germ;
  struct POLY DomainPoly;

  neut_poly_set_zero (&DomainPoly);
  net_tess_poly (Domain, 1, &DomainPoly);

  if (! strcmp (In.domain, "cube"))
  {
    for (i = 0; i < 3; i++)
      bbox[i][1] = In.domainparms[i];
  }
  else if (! strcmp (In.domain, "cylinder"))
  {
    bbox[0][1] = In.domainparms[1];
    bbox[1][1] = In.domainparms[1];
    bbox[2][1] = In.domainparms[0];
  }
  else
  {
    neut_poly_bbox (DomainPoly, bboxv);
    bbox[0][1] = bboxv[2];
    bbox[1][1] = bboxv[4];
    bbox[2][1] = bboxv[6];
  }

  (*pVox).PolyQty = GermSet.N;

  (*pVox).size = ut_alloc_1d_int (3);
  net_vox_init_size (In.voxsizetype, In.voxsize, In.voxsize3, bbox, pVox);

  (*pVox).VoxPoly = ut_alloc_3d_int ((*pVox).size[0], (*pVox).size[1], (*pVox).size[2]);

  for (k = 0; k < (*pVox).size[2]; k++)
  {
    coo[2] = (*pVox).vsize[2] * ((double) k + 0.5);

    for (j = 0; j < (*pVox).size[1]; j++)
    {
      coo[1] = (*pVox).vsize[1] * ((double) j + 0.5);

      for (i = 0; i < (*pVox).size[0]; i++)
      {
	coo[0] = (*pVox).vsize[0] * ((double) i + 0.5);

	if (neut_poly_point_in (DomainPoly, coo) == 0)
	  (*pVox).VoxPoly[i][j][k] = 0;
	else
	{
	  for (l = 1; l <= GermSet.N; l++)
	    GermDist[l] = ut_space_dist (coo, GermSet.GermsCoo[l] + 1);

	  for (l = 1; l <= GermSet.nN; l++)
	    GermDist[GermSet.N + l] = ut_space_dist (coo, GermSet.nGermsCoo[l] + 1);

	  (*pVox).VoxPoly[i][j][k] =
	    1 + ut_array_1d_min_index (GermDist + 1, GermSet.N + GermSet.nN);
	}
      }
    }
  }

  if (! strcmp (In.ttype, "periodic"))
    for (k = 0; k < (*pVox).size[2]; k++)
      for (j = 0; j < (*pVox).size[1]; j++)
	for (i = 0; i < (*pVox).size[0]; i++)
	{
	  Germ = (*pVox).VoxPoly[i][j][k];

	  if (Germ >= GermSet.N + 1) // modifying Germ 
	  {
	    Germ = GermSet.GermToGerm[Germ - GermSet.N];
	    (*pVox).VoxPoly[i][j][k] = Germ;
	  }
	}

  ut_free_1d (coo);
  ut_free_1d (bboxv);
  ut_free_2d (bbox, 3);
  ut_free_1d (GermDist);
  neut_poly_free (&DomainPoly);

  return;
}
