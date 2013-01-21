/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nem_reconstruct_vox.h"

void
nem_reconstruct_vox (struct VOX Vox, struct TESS* pTess, struct NODES* pNodes,
		   struct MESH *pMesh1D, struct MESH *pMesh2D,
		   struct NSET* pNSet2D)
{
  int i, j, k, elt;
  double* dsize = ut_alloc_1d (3);
  struct MESH Mesh0D, Mesh2D, Mesh3D;

  neut_mesh_set_zero (&Mesh0D);
  neut_mesh_set_zero (&Mesh2D);
  neut_mesh_set_zero (&Mesh3D);

  // Considering that the domain is a filled cube to set VoxPoly at the
  // boundary.  This is for marching cube.

  // x = 0 and x = 1
  for (k = 1; k <= Vox.size[2]; k++)
    for (j = 1; j <= Vox.size[1]; j++)
    {
      Vox.VoxPoly[0][j][k] = -1;
      Vox.VoxPoly[Vox.size[0] + 1][j][k] = -2;
    }
  
  // y = 0 and y = 1
  for (i = 1; i <= Vox.size[0]; i++)
    for (k = 1; k <= Vox.size[2]; k++)
    {
      Vox.VoxPoly[i][0][k] = -3;
      Vox.VoxPoly[i][Vox.size[1] + 1][k] = -4;
    }

  // z = 0 and z = 1
  for (i = 1; i <= Vox.size[0]; i++)
    for (j = 1; j <= Vox.size[1]; j++)
    {
      Vox.VoxPoly[i][j][0] = -5;
      Vox.VoxPoly[i][j][Vox.size[2] + 1] = -6;
    }

  for (i = 0; i < 3; i++)
    dsize[i] = Vox.size[i] * Vox.vsize[i];

  neut_mesh_quad (Vox.size, 1, pNodes, &Mesh3D, pNSet2D);
  neut_nodes_scale (pNodes, dsize[0], dsize[1], dsize[2]);

  elt = 0;
  for (k = 1; k <= Vox.size[2]; k++)
    for (j = 1; j <= Vox.size[1]; j++)
      for (i = 1; i <= Vox.size[0]; i++)
	Mesh3D.EltElset[++elt] = Vox.VoxPoly[i][j][k];

  neut_mesh_init_elsets (&Mesh3D);

  neut_tess_set_zero (pTess);
  nem_reconstruct_mesh ("0,1,2,3", pNodes, &Mesh0D, pMesh1D, &Mesh2D,
			    &Mesh3D, pTess);

  ut_free_2d_int (Mesh3D.NodeElts, (*pNodes).NodeQty + 1);
  neut_mesh_init_nodeelts (&Mesh3D, (*pNodes).NodeQty);

  neut_mesh_quad_tri (*pNodes, Mesh2D, pMesh2D);

  neut_mesh_free (&Mesh0D);
  neut_mesh_free (&Mesh2D);
  neut_mesh_free (&Mesh3D);

  ut_free_1d (dsize);

  return;
}
