/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_tess_op_lcl.h"

void
neut_tess_init_domain_memcpy (struct TESS* pTess, struct POLY Domain, char* type)
{
  int i;
  struct TESS DomTess;

  neut_tess_set_zero (&DomTess);
  neut_poly_tess (Domain, &DomTess);

  // General
  (*pTess).DomType = ut_alloc_1d_char (strlen (type) + 1);
  strcpy ((*pTess).DomType, type);

  // Faces

  (*pTess).DomFaceQty = DomTess.FaceQty;
  (*pTess).DomFaceEq = ut_alloc_2d ((*pTess).DomFaceQty + 1, 4);
  (*pTess).DomFaceVerQty = ut_alloc_1d_int  ((*pTess).DomFaceQty + 1);
  (*pTess).DomFaceVerNb  = ut_alloc_1d_pint ((*pTess).DomFaceQty + 1);
  (*pTess).DomFaceEdgeNb = ut_alloc_1d_pint ((*pTess).DomFaceQty + 1);
    
  for (i = 1; i <= (*pTess).DomFaceQty; i++)
    ut_array_1d_memcpy ((*pTess).DomFaceEq[i], 4, DomTess.FaceEq[i]);

  ut_array_1d_int_memcpy ((*pTess).DomFaceVerQty, (*pTess).DomFaceQty + 1, DomTess.FaceVerQty);

  for (i = 1; i <= (*pTess).DomFaceQty; i++)
  {
    (*pTess).DomFaceVerNb[i]  = ut_alloc_1d_int ((*pTess).DomFaceVerQty[i]  + 1);
    (*pTess).DomFaceEdgeNb[i] = ut_alloc_1d_int ((*pTess).DomFaceVerQty[i] + 1);
    
    ut_array_1d_int_memcpy ((*pTess).DomFaceVerNb[i] + 1, (*pTess).DomFaceVerQty[i],
	                        DomTess.FaceVerNb[i] + 1);
    
    ut_array_1d_int_memcpy ((*pTess).DomFaceEdgeNb[i] + 1, (*pTess).DomFaceVerQty[i],
	                        DomTess.FaceEdgeNb[i] + 1);
  }

  neut_tess_init_domain_facelabel (pTess);

  // Edges
  (*pTess).DomEdgeQty = DomTess.EdgeQty;
  (*pTess).DomEdgeVerNb = ut_alloc_2d_int (DomTess.EdgeQty + 1, 2);
  (*pTess).DomEdgeFaceNb = ut_alloc_2d_int (DomTess.EdgeQty + 1, 2);

  for (i = 1; i <= (*pTess).DomEdgeQty; i++)
    ut_array_1d_int_memcpy ((*pTess).DomEdgeVerNb[i], 2, DomTess.EdgeVerNb[i]);

  for (i = 1; i <= (*pTess).DomEdgeQty; i++)
  {
    ut_array_1d_int_memcpy ((*pTess).DomEdgeFaceNb[i], 2, DomTess.EdgeFaceNb[i]);
    if (DomTess.EdgeFaceQty[i] != 2)
      ut_error_reportbug ();
  }

  // Vertices
  (*pTess).DomVerQty = DomTess.VerQty;
  (*pTess).DomVerEdgeQty = ut_alloc_1d_int (DomTess.VerQty + 1);
  (*pTess).DomVerEdgeNb  = ut_alloc_1d_pint (DomTess.VerQty + 1);

  ut_array_1d_int_memcpy ((*pTess).DomVerEdgeQty + 1, (*pTess).DomVerQty, DomTess.VerEdgeQty + 1);
  for (i = 1; i <= (*pTess).DomVerQty; i++)
  {
    (*pTess).DomVerEdgeNb[i] = ut_alloc_1d_int ((*pTess).DomVerEdgeQty[i]);
    ut_array_1d_int_memcpy ((*pTess).DomVerEdgeNb[i], (*pTess).DomVerEdgeQty[i],
	                    DomTess.VerEdgeNb[i]);
  }

  neut_tess_free (&DomTess);

  return;
}


void
neut_tess_init_domain_facelabel (struct TESS* pTess)
{
  int i, j, id;
  double* dist = NULL;

  if (! strcmp ((*pTess).DomType, "cube"))
  {
    (*pTess).DomFaceLabel = ut_alloc_2d_char (7, 3);
    strcpy ((*pTess).DomFaceLabel[1], "x0");
    strcpy ((*pTess).DomFaceLabel[2], "x1");
    strcpy ((*pTess).DomFaceLabel[3], "y0");
    strcpy ((*pTess).DomFaceLabel[4], "y1");
    strcpy ((*pTess).DomFaceLabel[5], "z0");
    strcpy ((*pTess).DomFaceLabel[6], "z1");
  }

  else if (! strcmp ((*pTess).DomType, "cylinder"))
  {
    (*pTess).DomFaceLabel = ut_alloc_2d_char ((*pTess).DomFaceQty + 1, 10);
    strcpy ((*pTess).DomFaceLabel[1], "z0");
    strcpy ((*pTess).DomFaceLabel[2], "z1");
    for (i = 3; i <= (*pTess).DomFaceQty; i++)
      sprintf ((*pTess).DomFaceLabel[i], "f%d\n", i - 2);
  }

  else // unknown domain type
  {
    if ((*pTess).DomVerQty == 8 && (*pTess).DomEdgeQty == 12
     && (*pTess).DomFaceQty == 6)
    {
      strcpy ((*pTess).DomType, "cube");
      (*pTess).DomFaceLabel = ut_alloc_2d_char (7, 3);

      double** n = ut_alloc_2d (6, 3);
      n[0][0] = -1;
      n[1][0] =  1;
      n[2][1] = -1;
      n[3][1] =  1;
      n[4][2] = -1;
      n[5][2] =  1;
      char** label = ut_alloc_2d_char (6, 3);
      strcpy (label[0], "x0");
      strcpy (label[1], "x1");
      strcpy (label[2], "y0");
      strcpy (label[3], "y1");
      strcpy (label[4], "z0");
      strcpy (label[5], "z1");

      dist = ut_alloc_1d (6);

      for (i = 1; i <= 6; i++)
      {
	for (j = 0; j < 6; j++)
	  dist[j] = ut_vector_angle (n[j], (*pTess).DomFaceEq[i] + 1);

	id = ut_array_1d_min_index (dist, 6);
	strcpy ((*pTess).DomFaceLabel[i], label[id]);
      }

      ut_free_1d (dist);
      ut_free_2d (n, 6);
      ut_free_2d_char (label, 6);
    }

    else
    {
      (*pTess).DomFaceLabel = ut_alloc_2d_char ((*pTess).DomFaceQty + 1, 10);
      for (i = 1; i <= (*pTess).DomFaceQty; i++)
	sprintf ((*pTess).DomFaceLabel[i], "f%d\n", i);
    }
  }

  return;
}
