/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "Res_mm.h"

/* Searching the EoD nodes from the FoDNodes. 12 edges, numbering: */
void
FoD2EoDNodes (int **FoDNodes, int ***pEoDNodes)
{
  int qty;
  int edge, f1, f2;
  int **EdgeFace = ut_alloc_2d_int (13, 2);

/*  1: y=0 z=0    2: y=1 z=0    3: y=0 z=1    4: y=1 z=1 */
/*  5: z=0 x=0    6: z=1 x=0    7: z=0 x=1    8: z=1 x=1 */
/*  9: x=0 y=0   10: x=1 y=0   11: x=0 y=1   12: x=1 y=1 */
  EdgeFace[1][0] = 3;
  EdgeFace[1][1] = 5;
  EdgeFace[2][0] = 4;
  EdgeFace[2][1] = 5;
  EdgeFace[3][0] = 3;
  EdgeFace[3][1] = 6;
  EdgeFace[4][0] = 4;
  EdgeFace[4][1] = 6;
  EdgeFace[5][0] = 5;
  EdgeFace[5][1] = 1;
  EdgeFace[6][0] = 6;
  EdgeFace[6][1] = 1;
  EdgeFace[7][0] = 5;
  EdgeFace[7][1] = 2;
  EdgeFace[8][0] = 6;
  EdgeFace[8][1] = 2;
  EdgeFace[9][0] = 1;
  EdgeFace[9][1] = 3;
  EdgeFace[10][0] = 2;
  EdgeFace[10][1] = 3;
  EdgeFace[11][0] = 1;
  EdgeFace[11][1] = 4;
  EdgeFace[12][0] = 2;
  EdgeFace[12][1] = 4;

  (*pEoDNodes) = ut_alloc_1d_pint (13);

  for (edge = 1; edge <= 12; edge++)
  {
    f1 = EdgeFace[edge][0];
    f2 = EdgeFace[edge][1];

    qty = oneDIntCommonElts (FoDNodes[f1], 1, FoDNodes[f1][0],
			     FoDNodes[f2], 1, FoDNodes[f2][0],
			     &((*pEoDNodes)[edge]), 1);
    (*pEoDNodes)[edge][0] = qty;
  }

  ut_free_2d_int (EdgeFace, 13);

  return;
}


void
EoD2CoDNodes (int **EoDNodes, int ***pCoDNodes)
{
  int corner, e1, e2;
  int qty;
  int **CornerEdge = ut_alloc_2d_int (9, 2);
  int **tmp;

  (*pCoDNodes) = ut_alloc_2d_int (9, 2);

  CornerEdge[1][0] = 1;
  CornerEdge[1][1] = 9;
  CornerEdge[2][0] = 1;
  CornerEdge[2][1] = 10;
  CornerEdge[3][0] = 2;
  CornerEdge[3][1] = 11;
  CornerEdge[4][0] = 2;
  CornerEdge[4][1] = 12;
  CornerEdge[5][0] = 3;
  CornerEdge[5][1] = 9;
  CornerEdge[6][0] = 3;
  CornerEdge[6][1] = 10;
  CornerEdge[7][0] = 4;
  CornerEdge[7][1] = 11;
  CornerEdge[8][0] = 4;
  CornerEdge[8][1] = 12;

  tmp = ut_alloc_1d_pint (9);

  for (corner = 1; corner <= 8; corner++)
  {
    e1 = CornerEdge[corner][0];
    e2 = CornerEdge[corner][1];

    qty = oneDIntCommonElts (EoDNodes[e1], 1, EoDNodes[e1][0],
			     EoDNodes[e2], 1, EoDNodes[e2][0],
			     &(tmp[corner]), 1);

    if (qty != 1)
      abort ();

    (*pCoDNodes)[corner][0] = 1;
    (*pCoDNodes)[corner][1] = tmp[corner][1];
  }

  /* free the non-square tmp array */


  ut_free_2d_int (CornerEdge, 9);

  return;
}


void
FoD2FoDBNodes (int **FoDNodes, int **EoDNodes, int ***pFoDBNodes)
{
  int i, j, k, test;
  int qty;
  /* int edge,f1,f2; */
  int **FaceEdge = ut_alloc_2d_int (7, 4);

  (*pFoDBNodes) = ut_alloc_1d_pint (7);

  /* for the numbering, see the definition of the edges. */
  FaceEdge[1][0] = 9;
  FaceEdge[1][1] = 11;
  FaceEdge[1][2] = 5;
  FaceEdge[1][3] = 6;
  FaceEdge[2][0] = 10;
  FaceEdge[2][1] = 12;
  FaceEdge[2][2] = 7;
  FaceEdge[2][3] = 8;
  FaceEdge[3][0] = 9;
  FaceEdge[3][1] = 10;
  FaceEdge[3][2] = 1;
  FaceEdge[3][3] = 3;
  FaceEdge[4][0] = 11;
  FaceEdge[4][1] = 12;
  FaceEdge[4][2] = 2;
  FaceEdge[4][3] = 4;
  FaceEdge[5][0] = 5;
  FaceEdge[5][1] = 7;
  FaceEdge[5][2] = 1;
  FaceEdge[5][3] = 2;
  FaceEdge[6][0] = 6;
  FaceEdge[6][1] = 8;
  FaceEdge[6][2] = 3;
  FaceEdge[6][3] = 4;

  for (i = 1; i <= 6; i++)
  {
    qty = FoDNodes[i][0] + 4;
    for (j = 0; j <= 3; j++)
      qty -= EoDNodes[FaceEdge[i][j]][0];

    (*pFoDBNodes)[i] = ut_alloc_1d_int (qty + 1);

    (*pFoDBNodes)[i][0] = 0;
    for (j = 1; j <= FoDNodes[i][0]; j++)
    {
      test = 0;
      for (k = 0; k <= 3; k++)
	if (oneDIntEltPos
	    (EoDNodes[FaceEdge[i][k]], 1, EoDNodes[FaceEdge[i][k]][0],
	     FoDNodes[i][j], 0) != -1)
	  test = 1;

      if (test == 1)
	continue;

      (*pFoDBNodes)[i][0]++;	/* recording the node */
      (*pFoDBNodes)[i][(*pFoDBNodes)[i][0]] = FoDNodes[i][j];
    }

    if ((*pFoDBNodes)[i][0] != qty)
    {
      printf ("Bad FoDB node qty!\n");
      printf ("debug!\n");
      abort ();
    }
  }

  ut_free_2d_int (FaceEdge, 7);

  return;
}


void
EoD2EoDBNodes (int **EoDNodes, int **CoDNodes, int ***pEoDBNodes)
{
  int i, j;
  int **EdgeCorner = ut_alloc_2d_int (13, 2);

  (*pEoDBNodes) = ut_alloc_1d_pint (13);

  /* for the numbering, see the definition of the edges. */
  EdgeCorner[1][0] = CoDNodes[1][1];
  EdgeCorner[1][1] = CoDNodes[2][1];
  EdgeCorner[2][0] = CoDNodes[3][1];
  EdgeCorner[2][1] = CoDNodes[4][1];
  EdgeCorner[3][0] = CoDNodes[5][1];
  EdgeCorner[3][1] = CoDNodes[6][1];
  EdgeCorner[4][0] = CoDNodes[7][1];
  EdgeCorner[4][1] = CoDNodes[8][1];
  EdgeCorner[5][0] = CoDNodes[1][1];
  EdgeCorner[5][1] = CoDNodes[3][1];
  EdgeCorner[6][0] = CoDNodes[5][1];
  EdgeCorner[6][1] = CoDNodes[7][1];
  EdgeCorner[7][0] = CoDNodes[2][1];
  EdgeCorner[7][1] = CoDNodes[4][1];
  EdgeCorner[8][0] = CoDNodes[6][1];
  EdgeCorner[8][1] = CoDNodes[8][1];
  EdgeCorner[9][0] = CoDNodes[1][1];
  EdgeCorner[9][1] = CoDNodes[5][1];
  EdgeCorner[10][0] = CoDNodes[2][1];
  EdgeCorner[10][1] = CoDNodes[6][1];
  EdgeCorner[11][0] = CoDNodes[3][1];
  EdgeCorner[11][1] = CoDNodes[7][1];
  EdgeCorner[12][0] = CoDNodes[4][1];
  EdgeCorner[12][1] = CoDNodes[8][1];

  for (i = 1; i <= 12; i++)
  {
    (*pEoDBNodes)[i] = ut_alloc_1d_int (EoDNodes[i][0] - 1);	/* +1-2 */

    (*pEoDBNodes)[i][0] = 0;
    for (j = 1; j <= EoDNodes[i][0]; j++)
      if (EoDNodes[i][j] != EdgeCorner[i][0]
	  && EoDNodes[i][j] != EdgeCorner[i][1])
      {
	(*pEoDBNodes)[i][0]++;	/* recording the node */
	(*pEoDBNodes)[i][(*pEoDBNodes)[i][0]] = EoDNodes[i][j];
      }
  }

  ut_free_2d_int (EdgeCorner, 13);

  return;
}

void
WriteNSet (char *name, int *NodeSet, char *format, FILE * file)
{
  int i, col;

  if (strcmp (format, "geof") == 0)
  {
    fprintf (file, "\n**nset %s\n", name);

    col = 0;
    for (i = 1; i <= NodeSet[0]; i++)
      ut_print_wnc_int (file, NodeSet[i], &col, 72);

    fprintf (file, "\n");
  }
  else if (strcmp (format, "abq") == 0 || strcmp (format, "inp") == 0)
  {
    fprintf (file, "*Nset, nset=%s\n", name);

    int call = 0;
    col = 0;
    for (i = 1; i <= NodeSet[0]; i++)
      ut_print_wnc_wncall (file, &col, 72, &call, 16, (i < NodeSet[0]) ? "%d," : "%d", NodeSet[i]);

    if (call == 0)
      fprintf (file, "\n");
    else
      fprintf (file, "\n\n");
  }
  else if (strcmp (format, "fev") == 0)
  {
    for (i = 1; i <= NodeSet[0]; i++)
      fprintf (file, "%d %s\n", NodeSet[i], name);
  }

  return;
}
