/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2011, Romain Quey. */
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
    fscanf (file, "%s", tmp);
    if (strcmp (tmp, "**general") != 0)
    {
      ut_print_message (2, 0, "Input file is not a valid geometry file.\n");
      abort ();
    }
    (*pGeo).morpho = ut_alloc_1d_char (100);
    (*pGeo).Type = ut_alloc_1d_char (100);
    fscanf (file, "%s%s%d", (*pGeo).morpho, (*pGeo).Type, &((*pGeo).Id));
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
  int i, j, ver;

  if (ut_file_string_scanncomp (file, "**vertex") != 0)
    abort ();

  fscanf (file, " %d", &((*pGeo).VerQty));

  // (*pGeo).VerDom   = ut_alloc_2d_int ((*pGeo).VerQty + 1, 4);
  (*pGeo).VerEdgeQty = ut_alloc_1d_int ((*pGeo).VerQty + 1);
  (*pGeo).VerEdgeNb  = ut_alloc_1d_pint ((*pGeo).VerQty + 1);
  (*pGeo).VerCoo     = ut_alloc_2d     ((*pGeo).VerQty + 1, 3);
  (*pGeo).VerState   = ut_alloc_1d_int ((*pGeo).VerQty + 1);

  for (i = 1; i <= (*pGeo).VerQty; i++)
  {
    fscanf (file, " %d", &ver);
    // for (j = 0; j < 4; j++)
    //  fscanf (file, "%d", &((*pGeo).VerDom[ver][j]));
    ut_file_skip (file, 4);
    
    fscanf (file, " %d", &((*pGeo).VerEdgeQty[ver]));
    (*pGeo).VerEdgeNb[ver] = ut_alloc_1d_int ((*pGeo).VerEdgeQty[ver]);
    for (j = 0; j < (*pGeo).VerEdgeQty[i]; j++)
      fscanf (file, " %d", &((*pGeo).VerEdgeNb[ver][j]));

    for (j = 0; j < 3; j++)
      fscanf (file, " %lf", &((*pGeo).VerCoo[ver][j]));
    
    fscanf (file, "%d", &((*pGeo).VerState[ver]));
  }

  return;
}

/* Geometry exportation: edge */
void
neut_geo_fscanf_edge_1p9 (struct GEO* pGeo, FILE * file)
{
  int i, j, edge;

  if (ut_file_string_scanncomp (file, "**edge") != 0)
    abort ();
  
  fscanf (file, "%d", &((*pGeo).EdgeQty));

  (*pGeo).EdgeFaceQty = ut_alloc_1d_int ((*pGeo).EdgeQty + 1);
  (*pGeo).EdgeVerNb   = ut_alloc_2d_int ((*pGeo).EdgeQty + 1, 2);
  (*pGeo).EdgeFaceNb  = ut_alloc_1d_pint ((*pGeo).EdgeQty + 1);
  (*pGeo).EdgeState   = ut_alloc_1d_int ((*pGeo).EdgeQty + 1);
  (*pGeo).EdgeDel     = ut_alloc_1d_int ((*pGeo).EdgeQty + 1);

  for (i = 1; i <= (*pGeo).EdgeQty; i++)
  {
    fscanf (file, "%d", &edge);
    for (j = 0; j < 2; j++)
      fscanf (file, "%d", &((*pGeo).EdgeVerNb[edge][j]));

    fscanf (file, "%d", &((*pGeo).EdgeFaceQty[edge]));

    (*pGeo).EdgeFaceNb[i] = ut_alloc_1d_int ((*pGeo).EdgeFaceQty[edge]);
    ut_array_1d_int_fscanf (file, (*pGeo).EdgeFaceNb[edge], (*pGeo).EdgeFaceQty[edge]);

    fscanf (file, "%d", &((*pGeo).EdgeState[edge]));
  }

  return;
}

/* Geometry exportation: face */
void
neut_geo_fscanf_face_1p9 (struct GEO* pGeo, FILE * file)
{
  int i, j, face, tmp;

  if (ut_file_string_scanncomp (file, "**face") != 0)
    abort ();
  
  fscanf (file, "%d", &((*pGeo).FaceQty));
    
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
    fscanf (file, "%d", &face);
    // fscanf (file, "%d", &((*pGeo).FaceDom[face]));
    ut_file_skip (file, 1);

    for (j = 0; j < 2; j++)
      fscanf (file, " %d", &((*pGeo).FacePoly[face][j]));
    for (j = 0; j < 4; j++)
      fscanf (file, " %lf", &((*pGeo).FaceEq[face][j]));

    fscanf (file, "%d", &((*pGeo).FaceVerQty[face]));

    (*pGeo).FaceVerNb[face]   = ut_alloc_1d_int ((*pGeo).FaceVerQty[face] + 1);
    (*pGeo).FaceEdgeNb[face]  = ut_alloc_1d_int ((*pGeo).FaceVerQty[face] + 1);
    (*pGeo).FaceEdgeOri[face] = ut_alloc_1d_int ((*pGeo).FaceVerQty[face] + 1);

    for (j = 1; j <= (*pGeo).FaceVerQty[face]; j++)
      fscanf (file, "%d", &((*pGeo).FaceVerNb[face][j]));

    for (j = 1; j <= (*pGeo).FaceVerQty[face]; j++)
    {
      fscanf (file, "%d", &tmp);
      (*pGeo).FaceEdgeOri[face][j] = ut_num_sgn_int (tmp);
      (*pGeo).FaceEdgeNb[face][j] = tmp * (*pGeo).FaceEdgeOri[face][j];
    }

    fscanf (file, "%d", &((*pGeo).FaceState[face]));
    fscanf (file, "%d", &((*pGeo).FacePt[face]));
    ut_array_1d_fscanf (file, (*pGeo).FacePtCoo[face], 3);
    fscanf (file, "%lf", &((*pGeo).FaceFF[face]));
  }

  return;
}

/* Geometry exportation: poly */
void
neut_geo_fscanf_poly_1p9 (struct GEO* pGeo, FILE * file)
{
  int i, j, poly, tmp;

  if (ut_file_string_scanncomp (file, "**polyhedron") != 0)
    abort ();

  fscanf (file, "%d", &((*pGeo).PolyQty));
  
  (*pGeo).CenterCoo   = ut_alloc_2d      ((*pGeo).PolyQty + 1, 3);
  (*pGeo).PolyTrue    = ut_alloc_1d_int  ((*pGeo).PolyQty + 1);
  (*pGeo).PolyBody    = ut_alloc_1d_int  ((*pGeo).PolyQty + 1);
  (*pGeo).PolyFaceQty = ut_alloc_1d_int  ((*pGeo).PolyQty + 1);
  (*pGeo).PolyFaceNb  = ut_alloc_1d_pint ((*pGeo).PolyQty + 1);
  (*pGeo).PolyFaceOri = ut_alloc_1d_pint ((*pGeo).PolyQty + 1);

  for (i = 1; i <= (*pGeo).PolyQty; i++)
  {
    fscanf (file, "%d", &poly);

    for (j = 0; j < 3; j++)
      fscanf (file, " %lf", &((*pGeo).CenterCoo[poly][j]));
    fscanf (file, " %d", &((*pGeo).PolyTrue[poly]));
    fscanf (file, " %d", &((*pGeo).PolyBody[poly]));
    fscanf (file, "\n    ");

    fscanf (file, " %d", &((*pGeo).PolyFaceQty[poly]));
    (*pGeo).PolyFaceNb[poly]  = ut_alloc_1d_int ((*pGeo).PolyFaceQty[poly] + 1);
    (*pGeo).PolyFaceOri[poly] = ut_alloc_1d_int ((*pGeo).PolyFaceQty[poly] + 1);

    for (j = 1; j <= (*pGeo).PolyFaceQty[poly]; j++)
    {
      fscanf (file, "%d", &tmp);
      (*pGeo).PolyFaceOri[poly][j] = ut_num_sgn_int (tmp);
      (*pGeo).PolyFaceNb[poly][j] = tmp * (*pGeo).PolyFaceOri[poly][j];
    }
  }

  return;
}
