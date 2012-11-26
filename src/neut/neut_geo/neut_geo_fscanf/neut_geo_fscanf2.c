/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_geo_fscanf_lcl.h"

void
neut_geo_fscanf_version (FILE* file, char* version)
{
  int status = 0;
  char* tmp = ut_alloc_1d_char (1000);

  if (fscanf (file, "%s", tmp) != 1)
    status = -1;
  else
  {
    if (! strcmp (tmp, "***tess1.9.2") || ! strcmp (tmp, "***geo1.9"))
    {
      strcpy (version, "1.9.2");
    }
    else if (! strcmp (tmp, "***tess"))
    {
      if (ut_file_string_scanncomp (file, "**format") != 0)
      {
	ut_print_message (2, 0, "Input file is not a valid tessellation file.\n");
	abort ();
      }

      if (fscanf (file, "%s", version) != 1)
	status = -1;
    }
    else
      status = -1;
  }

  if (status != 0)
  {
    ut_print_message (2, 0, "Input file is not a valid tessellation file.\n");
    abort ();
  }

  fseek (file, 0, 0);

  ut_free_1d_char (tmp);

  return;
}

/* Geometry exportation: head */
void
neut_geo_fscanf_head (struct GEO* pGeo, FILE * file)
{
  if (ut_file_string_scanncomp (file, "***tess")  != 0
   || ut_file_string_scanncomp (file, "**format") != 0
   || ut_file_string_scanncomp (file, "1.10")     != 0)
  {
    ut_print_message (2, 0, "Input file is not a valid tessellation file.\n");
    abort ();
  }

  (*pGeo).version = ut_alloc_1d_char (5);
  sprintf ((*pGeo).version, "1.10");

  if (ut_file_string_scanncomp (file, "**general") != 0)
  {
    ut_print_message (2, 0, "Input file is not a valid tessellation file.\n");
    abort ();
  }
  (*pGeo).morpho = ut_alloc_1d_char (100);
  (*pGeo).Type = ut_alloc_1d_char (100);
  
  if (fscanf (file, "%d%d%s%s", &((*pGeo).N), &((*pGeo).Id),
				(*pGeo).Type, (*pGeo).morpho) != 4)
  {
    ut_print_message (2, 0, "Input file is not a valid tessellation file.\n");
    abort ();
  }

  (*pGeo).morpho
    = ut_realloc_1d_char ((*pGeo).morpho, strlen ((*pGeo).morpho) + 1);

  return;
}

/* Geometry exportation: foot */
void
neut_geo_fscanf_foot (FILE * file)
{
  if (ut_file_string_scanncomp (file, "***end") != 0)
    abort ();

  return;
}

/* Geometry exportation: vertex */
void
neut_geo_fscanf_ver (struct GEO* pGeo, FILE * file)
{
  int i, ver, status;

  if (ut_file_string_scanncomp (file, "**vertex") != 0
   || fscanf (file, "%d", &((*pGeo).VerQty)) != 1)
    abort ();


  (*pGeo).VerDom   = ut_alloc_2d_int ((*pGeo).VerQty + 1, 2);
  (*pGeo).VerEdgeQty = ut_alloc_1d_int ((*pGeo).VerQty + 1);
  (*pGeo).VerEdgeNb  = ut_alloc_1d_pint ((*pGeo).VerQty + 1);
  (*pGeo).VerCoo     = ut_alloc_2d     ((*pGeo).VerQty + 1, 3);
  (*pGeo).VerState   = ut_alloc_1d_int ((*pGeo).VerQty + 1);

  for (i = 1; i <= (*pGeo).VerQty; i++)
  {
    status = fscanf (file, " %d", &ver);
    ut_array_1d_int_fscanf (file, (*pGeo).VerDom[ver], 2);
    
    status += fscanf (file, " %d", &((*pGeo).VerEdgeQty[ver]));

    (*pGeo).VerEdgeNb[ver] = ut_alloc_1d_int ((*pGeo).VerEdgeQty[ver]);
    ut_array_1d_int_fscanf (file, (*pGeo).VerEdgeNb[ver], (*pGeo).VerEdgeQty[ver]);

    ut_array_1d_fscanf (file, (*pGeo).VerCoo[ver], 3);
    
    status += fscanf (file, "%d", &((*pGeo).VerState[ver]));

    if (status != 3)
      abort ();
  }

  return;
}

/* Geometry exportation: edge */
void
neut_geo_fscanf_edge (struct GEO* pGeo, FILE * file)
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
  (*pGeo).EdgeDom   = ut_alloc_2d_int ((*pGeo).EdgeQty + 1, 2);

  for (i = 1; i <= (*pGeo).EdgeQty; i++)
  {
    status = fscanf (file, "%d", &edge);
    ut_array_1d_int_fscanf (file, (*pGeo).EdgeDom[edge], 2);

    ut_array_1d_int_fscanf (file, (*pGeo).EdgeVerNb[edge], 2);

    status += fscanf (file, "%d", &((*pGeo).EdgeFaceQty[edge]));

    (*pGeo).EdgeFaceNb[edge] = ut_alloc_1d_int ((*pGeo).EdgeFaceQty[edge]);
    ut_array_1d_int_fscanf (file, (*pGeo).EdgeFaceNb[edge], (*pGeo).EdgeFaceQty[edge]);

    status += fscanf (file, "%d", &((*pGeo).EdgeState[edge]));

    if (status != 3)
      abort ();
  }

  return;
}

/* Geometry exportation: face */
void
neut_geo_fscanf_face (struct GEO* pGeo, FILE * file)
{
  int i, j, face, tmp, status;

  if (ut_file_string_scanncomp (file, "**face") != 0
   || fscanf (file, "%d", &((*pGeo).FaceQty)) != 1)
    abort ();
    
  (*pGeo).FaceDom   = ut_alloc_2d_int ((*pGeo).FaceQty + 1, 2);
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
    status = fscanf (file, "%d", &face);

    ut_array_1d_int_fscanf (file, (*pGeo).FaceDom[face], 2);

    ut_array_1d_int_fscanf (file, (*pGeo).FacePoly[face], 2);

    ut_array_1d_fscanf (file, (*pGeo).FaceEq[face], 4);
    
    status += fscanf (file, "%d", &((*pGeo).FaceVerQty[face]));

    (*pGeo).FaceVerNb[face]   = ut_alloc_1d_int ((*pGeo).FaceVerQty[face] + 1);
    (*pGeo).FaceEdgeNb[face]  = ut_alloc_1d_int ((*pGeo).FaceVerQty[face] + 1);
    (*pGeo).FaceEdgeOri[face] = ut_alloc_1d_int ((*pGeo).FaceVerQty[face] + 1);

    ut_array_1d_int_fscanf (file, (*pGeo).FaceVerNb[face] + 1, (*pGeo).FaceVerQty[face]);

    for (j = 1; j <= (*pGeo).FaceVerQty[face]; j++)
    {
      if (fscanf (file, "%d", &tmp) != 1)
	abort ();
      (*pGeo).FaceEdgeOri[face][j] = ut_num_sgn_int (tmp);
      (*pGeo).FaceEdgeNb[face][j] = tmp * (*pGeo).FaceEdgeOri[face][j];
    }

    status += fscanf (file, "%d", &((*pGeo).FaceState[face]));
    status += fscanf (file, "%d", &((*pGeo).FacePt[face]));
    ut_array_1d_fscanf (file, (*pGeo).FacePtCoo[face], 3);
    status += fscanf (file, "%lf", &((*pGeo).FaceFF[face]));

    if (status != 5)
      abort ();
  }

  return;
}

/* Geometry exportation: poly */
void
neut_geo_fscanf_poly (struct GEO* pGeo, FILE * file)
{
  int i, poly, status;

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
    status = fscanf (file, "%d", &poly);

    ut_array_1d_fscanf (file, (*pGeo).CenterCoo[poly], 3);

    status += fscanf (file, "%d", &((*pGeo).PolyTrue[poly]));
    status += fscanf (file, "%d", &((*pGeo).PolyBody[poly]));
    status += fscanf (file, "%d", &((*pGeo).PolyFaceQty[poly]));

    if (status != 4)
      abort ();

    (*pGeo).PolyFaceNb[poly]  = ut_alloc_1d_int ((*pGeo).PolyFaceQty[poly] + 1);
    (*pGeo).PolyFaceOri[poly] = ut_alloc_1d_int ((*pGeo).PolyFaceQty[poly] + 1);

    ut_array_1d_int_fscanf (file, (*pGeo).PolyFaceNb[poly] + 1, (*pGeo).PolyFaceQty[poly]);
    ut_array_1d_int_sgn ((*pGeo).PolyFaceNb[poly] + 1,
			 (*pGeo).PolyFaceQty[poly],
			 (*pGeo).PolyFaceOri[poly] + 1);
    ut_array_1d_int_abs ((*pGeo).PolyFaceNb[poly] + 1, (*pGeo).PolyFaceQty[poly]);
  }
    
  return;
}

/* Geometry import: domain */
void
neut_geo_fscanf_domain (struct GEO* pGeo, FILE * file)
{
  int i, id;

  if (! strncmp ((*pGeo).version, "1.10", 4))
  {
    (*pGeo).DomType = ut_alloc_1d_char (10);

    if (ut_file_string_scanncomp (file, "**domain") != 0
     || ut_file_string_scanncomp (file, "*general") != 0
     || fscanf (file, "%s", (*pGeo).DomType)        != 1
     || ut_file_string_scanncomp (file, "*vertex")  != 0
     || fscanf (file, "%d", &((*pGeo).DomVerQty))   != 1)
      abort ();

    (*pGeo).DomTessVerNb = ut_alloc_1d_int ((*pGeo).DomVerQty + 1);
    (*pGeo).DomVerEdgeQty = ut_alloc_1d_int ((*pGeo).DomVerQty + 1);
    (*pGeo).DomVerEdgeNb = ut_alloc_1d_pint ((*pGeo).DomVerQty + 1);

    for (i = 1; i <= (*pGeo).DomVerQty; i++)
    {
      if (fscanf (file, "%d", &id) != 1 || id != i)
	abort ();

      if (fscanf (file, "%d%d", &((*pGeo).DomTessVerNb[i]),
				&((*pGeo).DomVerEdgeQty[i])) != 2)
	abort ();

      (*pGeo).DomVerEdgeNb[i] = ut_alloc_1d_int ((*pGeo).DomVerEdgeQty[i]);
      ut_array_1d_int_fscanf (file, (*pGeo).DomVerEdgeNb[i], (*pGeo).DomVerEdgeQty[i]);
    }

    // Reading edges
    
    if (ut_file_string_scanncomp (file, "*edge")   != 0
     || fscanf (file, "%d", &((*pGeo).DomEdgeQty)) != 1)
      abort ();

    (*pGeo).DomTessEdgeQty = ut_alloc_1d_int ((*pGeo).DomEdgeQty + 1);
    (*pGeo).DomTessEdgeNb = ut_alloc_1d_pint ((*pGeo).DomEdgeQty + 1);
    (*pGeo).DomEdgeVerNb = ut_alloc_2d_int ((*pGeo).DomEdgeQty + 1, 2);
    (*pGeo).DomEdgeFaceNb = ut_alloc_2d_int ((*pGeo).DomEdgeQty + 1, 2);

    for (i = 1; i <= (*pGeo).DomEdgeQty; i++)
    {
      if (fscanf (file, "%d", &id) != 1 || id != i)
	abort ();

      if (fscanf (file, "%d", &((*pGeo).DomTessEdgeQty[i])) != 1)
	abort ();

      (*pGeo).DomTessEdgeNb[i] = ut_alloc_1d_int ((*pGeo).DomTessEdgeQty[i] + 1);
      ut_array_1d_int_fscanf (file, (*pGeo).DomTessEdgeNb[i] + 1, (*pGeo).DomTessEdgeQty[i]);
      ut_array_1d_int_fscanf (file, (*pGeo).DomEdgeVerNb[i], 2);
      ut_array_1d_int_fscanf (file, (*pGeo).DomEdgeFaceNb[i], 2);
    }
    
    // Reading faces
    if (ut_file_string_scanncomp (file, "*face") != 0
     || fscanf (file, "%d", &((*pGeo).DomFaceQty)) != 1)
      abort ();

    (*pGeo).DomFaceEq = ut_alloc_2d ((*pGeo).DomFaceQty + 1, 4);
    (*pGeo).DomTessFaceQty = ut_alloc_1d_int ((*pGeo).DomFaceQty + 1);
    (*pGeo).DomTessFaceNb  = ut_alloc_1d_pint ((*pGeo).DomFaceQty + 1);
    (*pGeo).DomFaceVerQty  = ut_alloc_1d_int ((*pGeo).DomFaceQty + 1);
    (*pGeo).DomFaceVerNb   = ut_alloc_1d_pint ((*pGeo).DomFaceQty + 1);
    (*pGeo).DomFaceEdgeNb  = ut_alloc_1d_pint ((*pGeo).DomFaceQty + 1);

    for (i = 1; i <= (*pGeo).DomFaceQty; i++)
    {
      if (fscanf (file, "%d", &id) != 1 || id != i)
      {
	printf ("id = %d != %d\n", id, i);
	abort ();
      }
      
      if (fscanf (file, "%d", &((*pGeo).DomTessFaceQty[i])) != 1)
	abort ();

      (*pGeo).DomTessFaceNb[i] = ut_alloc_1d_int ((*pGeo).DomTessFaceQty[i] + 1);

      ut_array_1d_int_fscanf (file, (*pGeo).DomTessFaceNb[i] + 1, (*pGeo).DomTessFaceQty[i]);
      ut_array_1d_fscanf (file, (*pGeo).DomFaceEq[i] + 1, 3);

      if (fscanf (file, "%lf%d", &((*pGeo).DomFaceEq[i][0]),
				 &((*pGeo).DomFaceVerQty[i])) != 2)
	abort ();

      (*pGeo).DomFaceVerNb[i]  = ut_alloc_1d_int ((*pGeo).DomFaceVerQty[i] + 1);
      (*pGeo).DomFaceEdgeNb[i] = ut_alloc_1d_int ((*pGeo).DomFaceVerQty[i] + 1);

      ut_array_1d_int_fscanf (file, (*pGeo).DomFaceVerNb[i] + 1, (*pGeo).DomFaceVerQty[i]);
      ut_array_1d_int_fscanf (file, (*pGeo).DomFaceEdgeNb[i] + 1, (*pGeo).DomFaceVerQty[i]);
    }
  }
  else if (! strcmp ((*pGeo).version, "1.9.2"))
  {
    double** bbox = ut_alloc_2d (3, 2);

    neut_geo_boundingbox (*pGeo, bbox);

    (*pGeo).DomType = ut_alloc_1d_char (5);
    sprintf ((*pGeo).DomType, "cube");

    (*pGeo).DomFaceQty = 6;
    (*pGeo).DomFaceEq = ut_alloc_2d ((*pGeo).DomFaceQty + 1, 4);
    
    (*pGeo).DomFaceEq[1][1] = -1;
    (*pGeo).DomFaceEq[1][0] =  bbox[0][0];
    (*pGeo).DomFaceEq[2][1] =  1;
    (*pGeo).DomFaceEq[2][0] =  bbox[0][1];
    (*pGeo).DomFaceEq[3][2] = -1;
    (*pGeo).DomFaceEq[3][0] =  bbox[1][0];
    (*pGeo).DomFaceEq[4][2] =  1;
    (*pGeo).DomFaceEq[4][0] =  bbox[1][1];
    (*pGeo).DomFaceEq[5][3] = -1;
    (*pGeo).DomFaceEq[5][0] =  bbox[2][0];
    (*pGeo).DomFaceEq[6][3] =  1;
    (*pGeo).DomFaceEq[6][0] =  bbox[2][1];

    neut_geo_init_domtessface (pGeo);
    // this would be needed for backward compatibility...
    // neut_geo_init_domtessedge (pGeo);
    // neut_geo_init_domtessver  (pGeo);

    ut_free_2d (bbox, 3);
  }

  return;
}
