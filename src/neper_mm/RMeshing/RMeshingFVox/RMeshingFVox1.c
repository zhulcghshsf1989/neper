/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"RMeshingFVox.h"

void
RMeshingFVox (struct VOX Vox, int cleaning, struct MESH* pMesh)
{
  int i, j, k, elt;
  int rmvoxqty, rmpacketqty;
  struct VOX Vox2;
  double* scale = ut_alloc_1d (3);

  printf ("\n");

  for (i = 0; i < 3; i++)
    scale[i] = (double) (*pMesh).msize[i] / Vox.size[i];

  neut_vox_set_zero (&Vox2);
  neut_vox_memcpy (Vox, &Vox2);
  neut_vox_gridscale (&Vox2, scale[0], scale[1], scale[2]);

  if (cleaning)
  {
    ut_print_message (0, 3, "Cleaning mesh ... ");

    neut_vox_clean (Vox2, 2, &Vox2, &rmvoxqty, &rmpacketqty);

    printf ("%d elt%s removed (%d packet%s).\n",
	    rmvoxqty, (rmvoxqty < 2) ? "" : "s",
	    rmpacketqty, (rmpacketqty < 2) ? "" : "s");
  }

  elt = 0;
  for (k = 1; k <= (*pMesh).msize[2]; k++)
    for (j = 1; j <= (*pMesh).msize[1]; j++)
      for (i = 1; i <= (*pMesh).msize[0]; i++)
	(*pMesh).EltElset[++elt] = Vox2.VoxPoly[i - 1][j - 1][k - 1];

  ut_free_1d (scale);

  return;
}
