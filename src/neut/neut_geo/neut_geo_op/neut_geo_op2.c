/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_geo_op_lcl.h"

void
neut_geo_init_domain_memcpy (struct GEO* pGeo, struct POLY Domain, char* type)
{
  int i;
  struct GEO DomGeo;

  neut_geo_set_zero (&DomGeo);
  neut_poly_geo (Domain, &DomGeo);

  // General
  (*pGeo).DomType = ut_alloc_1d_char (strlen (type) + 1);
  strcpy ((*pGeo).DomType, type);

  // Faces

  (*pGeo).DomFaceQty = DomGeo.FaceQty;
  (*pGeo).DomFaceEq = ut_alloc_2d ((*pGeo).DomFaceQty + 1, 4);
  (*pGeo).DomFaceVerQty = ut_alloc_1d_int  ((*pGeo).DomFaceQty + 1);
  (*pGeo).DomFaceVerNb  = ut_alloc_1d_pint ((*pGeo).DomFaceQty + 1);
  (*pGeo).DomFaceEdgeNb = ut_alloc_1d_pint ((*pGeo).DomFaceQty + 1);
    
  for (i = 1; i <= (*pGeo).DomFaceQty; i++)
    ut_array_1d_memcpy ((*pGeo).DomFaceEq[i], 4, DomGeo.FaceEq[i]);

  ut_array_1d_int_memcpy ((*pGeo).DomFaceVerQty, (*pGeo).DomFaceQty + 1, DomGeo.FaceVerQty);

  for (i = 1; i <= (*pGeo).DomFaceQty; i++)
  {
    (*pGeo).DomFaceVerNb[i]  = ut_alloc_1d_int ((*pGeo).DomFaceVerQty[i]  + 1);
    (*pGeo).DomFaceEdgeNb[i] = ut_alloc_1d_int ((*pGeo).DomFaceVerQty[i] + 1);
    
    ut_array_1d_int_memcpy ((*pGeo).DomFaceVerNb[i] + 1, (*pGeo).DomFaceVerQty[i],
	                        DomGeo.FaceVerNb[i] + 1);
    
    ut_array_1d_int_memcpy ((*pGeo).DomFaceEdgeNb[i] + 1, (*pGeo).DomFaceVerQty[i],
	                        DomGeo.FaceEdgeNb[i] + 1);
  }

  neut_geo_init_domain_facelabel (pGeo);

  // Edges
  (*pGeo).DomEdgeQty = DomGeo.EdgeQty;
  (*pGeo).DomEdgeVerNb = ut_alloc_2d_int (DomGeo.EdgeQty + 1, 2);
  (*pGeo).DomEdgeFaceNb = ut_alloc_2d_int (DomGeo.EdgeQty + 1, 2);

  for (i = 1; i <= (*pGeo).DomEdgeQty; i++)
    ut_array_1d_int_memcpy ((*pGeo).DomEdgeVerNb[i], 2, DomGeo.EdgeVerNb[i]);

  for (i = 1; i <= (*pGeo).DomEdgeQty; i++)
  {
    ut_array_1d_int_memcpy ((*pGeo).DomEdgeFaceNb[i], 2, DomGeo.EdgeFaceNb[i]);
    if (DomGeo.EdgeFaceQty[i] != 2)
      ut_error_reportbug ();
  }

  // Vertices
  (*pGeo).DomVerQty = DomGeo.VerQty;
  (*pGeo).DomVerEdgeQty = ut_alloc_1d_int (DomGeo.VerQty + 1);
  (*pGeo).DomVerEdgeNb  = ut_alloc_1d_pint (DomGeo.VerQty + 1);

  ut_array_1d_int_memcpy ((*pGeo).DomVerEdgeQty + 1, (*pGeo).DomVerQty, DomGeo.VerEdgeQty + 1);
  for (i = 1; i <= (*pGeo).DomVerQty; i++)
  {
    (*pGeo).DomVerEdgeNb[i] = ut_alloc_1d_int ((*pGeo).DomVerEdgeQty[i]);
    ut_array_1d_int_memcpy ((*pGeo).DomVerEdgeNb[i], (*pGeo).DomVerEdgeQty[i],
	                    DomGeo.VerEdgeNb[i]);
  }

  neut_geo_free (&DomGeo);

  return;
}


void
neut_geo_init_domain_facelabel (struct GEO* pGeo)
{
  int i, j, id;
  double* dist = NULL;

  if (! strcmp ((*pGeo).DomType, "cube"))
  {
    (*pGeo).DomFaceLabel = ut_alloc_2d_char (7, 3);
    strcpy ((*pGeo).DomFaceLabel[1], "x0");
    strcpy ((*pGeo).DomFaceLabel[2], "x1");
    strcpy ((*pGeo).DomFaceLabel[3], "y0");
    strcpy ((*pGeo).DomFaceLabel[4], "y1");
    strcpy ((*pGeo).DomFaceLabel[5], "z0");
    strcpy ((*pGeo).DomFaceLabel[6], "z1");
  }

  else if (! strcmp ((*pGeo).DomType, "cylinder"))
  {
    (*pGeo).DomFaceLabel = ut_alloc_2d_char ((*pGeo).DomFaceQty + 1, 10);
    strcpy ((*pGeo).DomFaceLabel[1], "z0");
    strcpy ((*pGeo).DomFaceLabel[2], "z1");
    for (i = 3; i <= (*pGeo).DomFaceQty; i++)
      sprintf ((*pGeo).DomFaceLabel[i], "f%d\n", i - 2);
  }

  else // unknown domain type
  {
    if ((*pGeo).DomVerQty == 8 && (*pGeo).DomEdgeQty == 12
     && (*pGeo).DomFaceQty == 6)
    {
      strcpy ((*pGeo).DomType, "cube");
      (*pGeo).DomFaceLabel = ut_alloc_2d_char (7, 3);

      double** n = ut_alloc_2d (6, 3);
      n[0][0] =  1;
      n[1][0] = -1;
      n[2][1] =  1;
      n[3][1] = -1;
      n[4][2] =  1;
      n[5][2] = -1;
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
	  dist[j] = ut_space_dist (n[j], (*pGeo).DomFaceEq[i] + 1);

	id = ut_array_1d_min_index (dist, 6);
	strcpy ((*pGeo).DomFaceLabel[i], label[id]);
      }

      ut_free_1d (dist);
      ut_free_2d (n, 6);
      ut_free_2d_char (label, 6);
    }

    else
    {


    }
  }

  return;
}
