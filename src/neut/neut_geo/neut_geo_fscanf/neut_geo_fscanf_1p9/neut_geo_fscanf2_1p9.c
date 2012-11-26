/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_geo_fscanf_1p9.h"

/* Geometry exportation: head */
void
neut_geo_fscanf_head_1p9 (struct GEO* pGeo, FILE * file)
{
  int status;
  char* tmp = ut_alloc_1d_char (100);

  (*pGeo).PolyQty = (*pGeo).PolyQty;
  status = fscanf (file, "***geo%s", tmp);
  if (status != 1)
  {
    rewind (file);
    status = fscanf (file, "***tess%s", tmp);
  }

  if (status != 1)
    ut_print_message (2, 0, "Input file is not a valid geometry file.\n");
  else if (strncmp (tmp, "1.9", 3) != 0)
  {
    ut_print_message (2, 0, "Input file is not a valid geometry file.\n");
    printf ("Unknown / unsupported version %s\n", tmp);
    abort ();
  }

  if (strcmp (tmp, "1.9.2") == 0)
  {
    if (fscanf (file, "%s", tmp) != 1 || strcmp (tmp, "**general") != 0)
    {
      ut_print_message (2, 0, "Input file is not a valid geometry file.\n");
      abort ();
    }
    (*pGeo).morpho = ut_alloc_1d_char (100);
    (*pGeo).Type = ut_alloc_1d_char (100);
    if (fscanf (file, "%s%s%d", (*pGeo).morpho, (*pGeo).Type, &((*pGeo).Id)) != 3)
      abort ();

    if (! strcmp ((*pGeo).Type, "0"))
      strcpy ((*pGeo).Type, "standard");
    else
      ut_error_reportbug ();
  }

  ut_free_1d_char (tmp);

  return;
}

/* Geometry exportation: foot */
void
neut_geo_fscanf_foot_1p9 (FILE * file)
{
  if (ut_file_string_scanncomp (file, "***end") != 0)
    abort ();

  return;
}

/* Geometry exportation: vertex */
void
neut_geo_fscanf_ver_1p9 (struct GEO* pGeo, FILE * file)
{
  int i, ver;

  if (ut_file_string_scanncomp (file, "**vertex") != 0
   || fscanf (file, "%d", &((*pGeo).VerQty)) != 1)
    abort ();

  // (*pGeo).VerDom   = ut_alloc_2d_int ((*pGeo).VerQty + 1, 4);
  (*pGeo).VerEdgeQty = ut_alloc_1d_int ((*pGeo).VerQty + 1);
  (*pGeo).VerEdgeNb  = ut_alloc_1d_pint ((*pGeo).VerQty + 1);
  (*pGeo).VerCoo     = ut_alloc_2d     ((*pGeo).VerQty + 1, 3);
  (*pGeo).VerState   = ut_alloc_1d_int ((*pGeo).VerQty + 1);

  for (i = 1; i <= (*pGeo).VerQty; i++)
  {
    if (fscanf (file, "%d", &ver) != 1)
      abort ();
    ut_file_skip (file, 4);
    
    if (fscanf (file, "%d", &((*pGeo).VerEdgeQty[ver])) != 1)
      abort ();

    (*pGeo).VerEdgeNb[ver] = ut_alloc_1d_int ((*pGeo).VerEdgeQty[ver]);
    ut_array_1d_int_fscanf (file, (*pGeo).VerEdgeNb[ver],
	(*pGeo).VerEdgeQty[ver]);

    ut_array_1d_fscanf (file, (*pGeo).VerCoo[ver], 3);
    
    if (fscanf (file, "%d", &((*pGeo).VerState[ver])) != 1)
      abort ();
  }

  return;
}

/* Geometry exportation: edge */
void
neut_geo_fscanf_edge_1p9 (struct GEO* pGeo, FILE * file)
{
  int i, edge, status;

  if (ut_file_string_scanncomp (file, "**edge") != 0
   || fscanf (file, "%d", &((*pGeo).EdgeQty)) != 1)
    abort ();

  (*pGeo).EdgeFaceQty = ut_alloc_1d_int ((*pGeo).EdgeQty + 1);
  (*pGeo).EdgeVerNb   = ut_alloc_2d_int ((*pGeo).EdgeQty + 1, 2);
  (*pGeo).EdgeFaceNb  = ut_alloc_1d_pint ((*pGeo).EdgeQty + 1);
  (*pGeo).EdgeState   = ut_alloc_1d_int ((*pGeo).EdgeQty + 1);
  (*pGeo).EdgeDel     = ut_alloc_1d_int ((*pGeo).EdgeQty + 1);

  for (i = 1; i <= (*pGeo).EdgeQty; i++)
  {
    status = fscanf (file, "%d", &edge);
    ut_array_1d_int_fscanf (file, (*pGeo).EdgeVerNb[edge], 2);
    status += fscanf (file, "%d", &((*pGeo).EdgeFaceQty[edge]));

    (*pGeo).EdgeFaceNb[i] = ut_alloc_1d_int ((*pGeo).EdgeFaceQty[edge]);
    ut_array_1d_int_fscanf (file, (*pGeo).EdgeFaceNb[edge], (*pGeo).EdgeFaceQty[edge]);

    status += fscanf (file, "%d", &((*pGeo).EdgeState[edge]));

    if (status != 3)
      abort ();
  }

  return;
}

/* Geometry exportation: face */
void
neut_geo_fscanf_face_1p9 (struct GEO* pGeo, FILE * file)
{
  int i, face;

  if (ut_file_string_scanncomp (file, "**face") != 0
   || fscanf (file, "%d", &((*pGeo).FaceQty)) != 1)
    abort ();
    
  // (*pGeo).FaceDom   = ut_alloc_1d_int ((*pGeo).FaceQty + 1);
  (*pGeo).FacePoly    = ut_alloc_2d_int ((*pGeo).FaceQty + 1, 2);
  (*pGeo).FaceEq      = ut_alloc_2d     ((*pGeo).FaceQty + 1, 4);
  (*pGeo).FaceVerQty  = ut_alloc_1d_int ((*pGeo).FaceQty + 1);
  (*pGeo).FaceVerNb   = ut_alloc_1d_pint ((*pGeo).FaceQty + 1);
  (*pGeo).FaceEdgeNb  = ut_alloc_1d_pint ((*pGeo).FaceQty + 1);
  (*pGeo).FaceEdgeOri = ut_alloc_1d_pint ((*pGeo).FaceQty + 1);
  (*pGeo).FaceState   = ut_alloc_1d_int ((*pGeo).FaceQty + 1);
  (*pGeo).FacePt      = ut_alloc_1d_int ((*pGeo).FaceQty + 1);
  (*pGeo).FacePtCoo   = ut_alloc_2d     ((*pGeo).FaceQty + 1, 3);
  (*pGeo).FaceFF      = ut_alloc_1d     ((*pGeo).FaceQty + 1);

  for (i = 1; i <= (*pGeo).FaceQty; i++)
  {
    if (fscanf (file, "%d", &face) != 1)
      abort ();
    ut_file_skip (file, 1);

    ut_array_1d_int_fscanf (file, (*pGeo).FacePoly[face], 2);
    ut_array_1d_fscanf     (file, (*pGeo).FaceEq[face], 4);

    if (fscanf (file, "%d", &((*pGeo).FaceVerQty[face])) != 1)
      abort ();

    (*pGeo).FaceVerNb[face]   = ut_alloc_1d_int ((*pGeo).FaceVerQty[face] + 1);
    (*pGeo).FaceEdgeNb[face]  = ut_alloc_1d_int ((*pGeo).FaceVerQty[face] + 1);
    (*pGeo).FaceEdgeOri[face] = ut_alloc_1d_int ((*pGeo).FaceVerQty[face] + 1);

    ut_array_1d_int_fscanf (file, (*pGeo).FaceVerNb[face] + 1, 
			          (*pGeo).FaceVerQty[face]);

    ut_array_1d_int_fscanf (file, (*pGeo).FaceEdgeNb[face] + 1,
	                          (*pGeo).FaceVerQty[face]);
    ut_array_1d_int_sgn ((*pGeo).FaceEdgeNb[face] + 1,
			 (*pGeo).FaceVerQty[face],
			 (*pGeo).FaceEdgeOri[face] + 1);
    ut_array_1d_int_abs ((*pGeo).FaceEdgeNb[face] + 1,
			 (*pGeo).FaceVerQty[face]);

    if (fscanf (file, "%d%d", &((*pGeo).FaceState[face]),
			      &((*pGeo).FacePt[face])) != 2)
      abort ();

    ut_array_1d_fscanf (file, (*pGeo).FacePtCoo[face], 3);

    if (fscanf (file, "%lf", &((*pGeo).FaceFF[face])) != 1)
      abort ();
  }

  return;
}

/* Geometry exportation: poly */
void
neut_geo_fscanf_poly_1p9 (struct GEO* pGeo, FILE * file)
{
  int i, poly;

  if (ut_file_string_scanncomp (file, "**polyhedron") != 0
   || fscanf (file, "%d", &((*pGeo).PolyQty)) != 1)
    abort ();
  
  (*pGeo).CenterCoo   = ut_alloc_2d      ((*pGeo).PolyQty + 1, 3);
  (*pGeo).PolyTrue    = ut_alloc_1d_int  ((*pGeo).PolyQty + 1);
  (*pGeo).PolyBody    = ut_alloc_1d_int  ((*pGeo).PolyQty + 1);
  (*pGeo).PolyFaceQty = ut_alloc_1d_int  ((*pGeo).PolyQty + 1);
  (*pGeo).PolyFaceNb  = ut_alloc_1d_pint ((*pGeo).PolyQty + 1);
  (*pGeo).PolyFaceOri = ut_alloc_1d_pint ((*pGeo).PolyQty + 1);

  for (i = 1; i <= (*pGeo).PolyQty; i++)
  {
    if (fscanf (file, "%d", &poly) != 1)
      abort ();

    ut_array_1d_fscanf (file, (*pGeo).CenterCoo[poly], 3);

    if (fscanf (file, "%d%d%d", &((*pGeo).PolyTrue[poly]),
				&((*pGeo).PolyBody[poly]),
				&((*pGeo).PolyFaceQty[poly])) != 3)
      abort ();

    (*pGeo).PolyFaceNb[poly]  = ut_alloc_1d_int ((*pGeo).PolyFaceQty[poly] + 1);
    (*pGeo).PolyFaceOri[poly] = ut_alloc_1d_int ((*pGeo).PolyFaceQty[poly] + 1);

    ut_array_1d_int_fscanf (file, (*pGeo).PolyFaceNb[poly] + 1, 
	                          (*pGeo).PolyFaceQty[poly]);
    ut_array_1d_int_sgn ((*pGeo).PolyFaceNb[poly] + 1,
	                 (*pGeo).PolyFaceQty[poly],
			 (*pGeo).PolyFaceOri[poly] + 1);
    ut_array_1d_int_abs ((*pGeo).PolyFaceNb[poly] + 1, 
	                 (*pGeo).PolyFaceQty[poly]);
  }

  return;
}
