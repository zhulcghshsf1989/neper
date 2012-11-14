/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"MStat.h"

/* CalcVol calculates the volume of the elements. */
void
CalcVol (struct NODES Nodes, struct MESH Mesh3D, double **pEltVol)
{
  int i, n1, n2, n3, n4;

  (*pEltVol) = ut_alloc_1d (Mesh3D.EltQty + 1);

  /* for every element */
  for (i = 1; i <= Mesh3D.EltQty; i++)
  {
    n1= Mesh3D.EltNodes[i][0];
    n2= Mesh3D.EltNodes[i][1];
    n3= Mesh3D.EltNodes[i][2];
    n4= Mesh3D.EltNodes[i][3];

    (*pEltVol)[i] = ut_space_tet_volume (Nodes.NodeCoo[n1],
				         Nodes.NodeCoo[n2],
				         Nodes.NodeCoo[n3],
				         Nodes.NodeCoo[n4]);
  }

  return;
}

/* CalcRho calculates, records and prints the rho-factor of the elements.
 * the supplied results are the same than those of gmsh.
 */
void
CalcRho (struct NODES Nodes, struct MESH Mesh3D, double **pEltRho)
{
  int i;			/* mute variable */
  int n1, n2, n3, n4;	/* mute variables */
  double *EdgeLength = ut_alloc_1d (6);	/* mute          */

  (*pEltRho) = ut_alloc_1d (Mesh3D.EltQty + 1);

  /* for every element */
  for (i = 1; i <= Mesh3D.EltQty; i++)
  {
    /* its nodes are recorded: we record their positions. */
    n1 = Mesh3D.EltNodes[i][0];
    n2 = Mesh3D.EltNodes[i][1];
    n3 = Mesh3D.EltNodes[i][2];
    n4 = Mesh3D.EltNodes[i][3];

    /* the various edge lengths are calculated. */
    EdgeLength[1] = ut_space_dist (Nodes.NodeCoo[n1],
				      Nodes.NodeCoo[n3]);
    EdgeLength[0] = ut_space_dist (Nodes.NodeCoo[n1],
				      Nodes.NodeCoo[n2]);
    EdgeLength[2] = ut_space_dist (Nodes.NodeCoo[n1],
				      Nodes.NodeCoo[n4]);
    EdgeLength[3] = ut_space_dist (Nodes.NodeCoo[n2],
				      Nodes.NodeCoo[n3]);
    EdgeLength[4] = ut_space_dist (Nodes.NodeCoo[n2],
				      Nodes.NodeCoo[n4]);
    EdgeLength[5] = ut_space_dist (Nodes.NodeCoo[n3],
				      Nodes.NodeCoo[n4]);

    /* rho is calculated */
    (*pEltRho)[i] =
      ut_array_1d_min (EdgeLength, 6) / ut_array_1d_max (EdgeLength, 6);
  }

  ut_free_1d (EdgeLength);

  return;
}

void
CalcAngle (struct NODES Nodes, struct MESH Mesh3D, double ***pEltAngle)
{
  int i, j;			/* mute variable */
  double *v1 = ut_alloc_1d (3);
  double *v2 = ut_alloc_1d (3);
  int angleid;
  int face, n1, n2, n3, n1b, n2b, n3b;

  (*pEltAngle) = ut_alloc_2d (Mesh3D.EltQty + 1, 13);

  /* for every element */
  for (i = 1; i <= Mesh3D.EltQty; i++)
  {
    angleid = 0;
    for (face = 1; face <= 4; face++)	/* for every face */
      for (n1 = 1; n1 <= 4; n1++)	/* for every node */
      {
	/* skipping n1=face */
	if (n1 == face)
	  continue;

	/* searching n2 & n3 */
	for (n2 = 1; n2 <= 4; n2++)
	  if (n2 != face && n2 != n1)
	    break;
	for (n3 = 1; n3 <= 4; n3++)
	  if (n3 != face && n3 != n1 && n3 != n2)
	    break;

	angleid++;

	n1b = Mesh3D.EltNodes[i][n1 - 1];
	n2b = Mesh3D.EltNodes[i][n2 - 1];
	n3b = Mesh3D.EltNodes[i][n3 - 1];

	for (j = 0; j < 3; j++)
	{
	  v1[j] = Nodes.NodeCoo[n2b][j] - Nodes.NodeCoo[n1b][j];
	  v2[j] = Nodes.NodeCoo[n3b][j] - Nodes.NodeCoo[n1b][j];
	}
	(*pEltAngle)[i][angleid] = ut_vector_angle (v1, v2);
      }

    (*pEltAngle)[i][0] = ut_array_1d_min ((*pEltAngle)[i] + 1, 12);	/* min angle */
  }

  ut_free_1d (v1);
  ut_free_1d (v2);

  return;
}

void
CalcRRatio (struct NODES Nodes, struct MESH Mesh3D, double **pRRatio)
{
  int i, j;
  int* node = ut_alloc_1d_int (4);
  double** p = ut_alloc_2d (4, 3);

  (*pRRatio) = ut_alloc_1d (Mesh3D.EltQty + 1);

  /* for every element */
  for (i = 1; i <= Mesh3D.EltQty; i++)
  {
    if (! strcmp (Mesh3D.EltType, "tri"))
    {
      /* its nodes are recorded: we record their positions. */
      for (j = 0; j < 4; j++)
	node[j] = Mesh3D.EltNodes[i][j];

      for (j = 0; j < 4; j++)
	ut_array_1d_memcpy (p[j], 3, Nodes.NodeCoo[node[j]]);

      (*pRRatio)[i] = neut_elt_3d_radiusratio (p[0], p[1], p[2], p[3]);
    }
    else
      (*pRRatio)[i] = -1;
  }

  ut_free_1d_int (node);
  ut_free_2d (p, 4);

  return;
}

void
CalcMeanLength (struct NODES Nodes, struct MESH Mesh3D, double **pMeanLength)
{
  int i;

  (*pMeanLength) = ut_alloc_1d (Mesh3D.EltQty + 1);

  /* for every element */
  for (i = 1; i <= Mesh3D.EltQty; i++)
    neut_mesh_elt_lengths (Mesh3D, Nodes, i, &((*pMeanLength)[i]), NULL, NULL);

  return;
}
