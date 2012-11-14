/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "RMeshing.h"

void
nemm_in_msize (struct IN In, double** domainsize, int* msize)
{
  int i;
  double* dsize = ut_alloc_1d (3);
  double avdsize;

  if (In.msizetype == 1)
  {
    for (i = 0; i < 3; i++)
      dsize[i] = (domainsize[i][1] - domainsize[i][0]);
    avdsize = pow (dsize[0] * dsize[1] * dsize[2], 0.3333333333);

    for (i = 0; i < 3; i++)
    {
      msize[i] = ut_num_d2ri (In.msize * dsize[i] / avdsize);
      if (msize[i] == 0)
	msize[i] = 1;
    }
  }
  else
    ut_array_1d_int_memcpy (msize, 3, In.msize3);

  ut_free_1d (dsize);

  return;
}


void
RMeshing_meshpoly (char* meshpoly, struct VOX Vox, struct MESH* pMesh, struct NODES* pNodes,
                   int*** pFoDNodes)
{
  int i, polyqty;
  int* poly = NULL;

  neut_vox_expr_polys (Vox, meshpoly, &poly, &polyqty);

  for (i = 1; i <= Vox.PolyQty; i++)
    if (ut_array_1d_int_eltpos (poly, polyqty, i) == -1)
      neut_mesh_rmelset (pMesh, *pNodes, i);

  neut_nodes_rmorphans (pNodes, pMesh, pFoDNodes);

  ut_free_1d_int (poly);

  return;
}


void
RMeshing_singnodedup (struct MESH* pMesh, struct NODES* pNodes, int*** pFoDNodes)
{
  int i, j, x, y, z, l, m, n, l2, m2, n2, o, elt, ininodeqty, status;
  int*** array = ut_alloc_3d_int (2, 2, 2);
  int*** arrayid = ut_alloc_3d_int (2, 2, 2);
  int eltnodeqty = neut_elt_nodeqty ((*pMesh).EltType, (*pMesh).Dimension, (*pMesh).EltOrder);

  ininodeqty = (*pNodes).NodeQty;

  char* message = ut_alloc_1d_char (1000);
  ut_print_progress (stdout, 0, ininodeqty, "%3.0f%%", message);

  for (i = 1; i <= ininodeqty; i++)
  {
    // printf ("(*pMesh).NodeElts[%d][0] = %d\n", i, (*pMesh).NodeElts[i][0]);
    if ((*pMesh).NodeElts[i][0] >= 2 && (*pMesh).NodeElts[i][0] <= 4)
    {
      // printf ("node = %d\n", i);
      // printf ("(*pMesh).NodeElts[i][0] = %d\n", (*pMesh).NodeElts[i][0]);

      status = 0;
      ut_array_3d_int_zero (array, 2, 2, 2);
      ut_array_3d_int_zero (arrayid, 2, 2, 2);

      for (j = 1; j <= (*pMesh).NodeElts[i][0]; j++)
      {
	elt = (*pMesh).NodeElts[i][j];
	/*
	printf ("(*pNodes).NodeCoo[i] = %f %f %f\n", (*pNodes).NodeCoo[i][0],
	    (*pNodes).NodeCoo[i][1], (*pNodes).NodeCoo[i][2]);
	printf ("(*pMesh).EltCoo[%d] = %f %f %f\n", elt, (*pMesh).EltCoo[elt][0],
	    (*pMesh).EltCoo[elt][1], (*pMesh).EltCoo[elt][2]);
	    */
	x = ((*pMesh).EltCoo[elt][0] < (*pNodes).NodeCoo[i][0]) ? 0 : 1;
	y = ((*pMesh).EltCoo[elt][1] < (*pNodes).NodeCoo[i][1]) ? 0 : 1;
	z = ((*pMesh).EltCoo[elt][2] < (*pNodes).NodeCoo[i][2]) ? 0 : 1;
	// printf ("x = %d y = %d z = %d\n", x, y, z);
	array[x][y][z] = 1;
	arrayid[x][y][z] = elt;
      }

      /*
      for (l = 0; l < 2; l++)
	for (m = 0; m < 2; m++)
	  for (n = 0; n < 2; n++)
	    printf ("array[%d][%d][%d] = %d\n", l, m, n, array[l][m][n]);
	    */

      for (l = 0; l < 2; l++)
	for (m = 0; m < 2; m++)
	  for (n = 0; n < 2; n++)
	    if (array[l][m][n] == 1)
	    {
	      // status = 0 means first time we check the node.
	      // The element should keep it anyway.
	      if (status == 0)
		status = 1;
	      else
	      {
		// printf ("testing elt %d\n", arrayid[l][m][n]);
		if (status == 0)
		  status = 1;
	      
		l2 = (l == 0) ? 1 : 0;
		m2 = (m == 0) ? 1 : 0;
		n2 = (n == 0) ? 1 : 0;

		elt = arrayid[l][m][n];
		
		// printf ("l = %d m = %d n = %d\n", l, m, n);
		// printf ("l2 = %d m2 = %d n2 = %d\n", l2, m2, n2);
		// printf ("array[l2][m][n] = %d\n", array[l2][m][n]);
		// printf ("array[l][m2][n] = %d\n", array[l][m2][n]);
		// printf ("array[l][m][n2] = %d\n", array[l][m][n2]);

		// should the elt have his own node?
		// is its sharing a face with another elt?
		if (array[l2][m][n] == 0 && array[l][m2][n] == 0 && array[l][m][n2] == 0)
		{
		  // printf ("elt = %d should have his own node\n", elt);
		  
		  neut_nodes_addnode (pNodes, (*pNodes).NodeCoo[i], 0);
		  ut_array_1d_int_findnreplace ((*pMesh).EltNodes[elt],
						eltnodeqty, i, (*pNodes).NodeQty);
		  if (pFoDNodes != NULL)
		    for (o = 1; o <= 6; o++)
		    {
		      if (ut_array_1d_int_eltpos ((*pFoDNodes)[o] + 1,
						  (*pFoDNodes)[o][0], i) != -1)
		      {
			(*pFoDNodes)[o][0]++;
			(*pFoDNodes)[o]
			  = ut_realloc_1d_int ((*pFoDNodes)[o], (*pFoDNodes)[o][0] + 1);
			(*pFoDNodes)[o][(*pFoDNodes)[o][0]] = (*pNodes).NodeQty;
		      }
		    }
		}
	      }
	    }
    }
    ut_print_progress (stdout, i, ininodeqty, "%3.0f%%", message);
  }

  ut_free_1d_char (message);

  ut_print_message (0, 3, "%d node(s) added.\n", (*pNodes).NodeQty - ininodeqty);

  ut_free_2d_int ((*pMesh).NodeElts, ininodeqty);
  (*pMesh).NodeElts = NULL;
  neut_mesh_init_nodeelts (pMesh, (*pNodes).NodeQty);
  
  ut_free_3d_int (array, 2, 2);
  ut_free_3d_int (arrayid, 2, 2);
  
  return;
}
