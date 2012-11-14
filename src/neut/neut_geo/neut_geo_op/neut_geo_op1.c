/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_geo_op_lcl.h"

void
neut_geo_set_zero (struct GEO* pGeo)
{
  (*pGeo).version = NULL;
  (*pGeo).VerQty = 0;
  (*pGeo).FaceQty = 0;
  (*pGeo).EdgeQty = 0;
  (*pGeo).PolyQty = 0;
  (*pGeo).VerCoo = NULL;
  (*pGeo).VerEdgeQty = NULL;
  (*pGeo).VerEdgeNb = NULL;
  (*pGeo).VerDom = NULL;
  (*pGeo).VerState = NULL;
  (*pGeo).EdgeVerNb = NULL;
  (*pGeo).EdgeFaceQty = NULL;
  (*pGeo).EdgeFaceNb = NULL;
  (*pGeo).EdgeLength = NULL;
  (*pGeo).EdgeState = NULL;
  (*pGeo).EdgeDel = NULL;
  (*pGeo).EdgeDom = NULL;
  (*pGeo).FacePoly = NULL;
  (*pGeo).FaceVerQty = NULL;
  (*pGeo).FaceVerNb = NULL;
  (*pGeo).FaceEdgeNb = NULL;
  (*pGeo).FaceEdgeOri = NULL;
  (*pGeo).FaceEq = NULL;
  (*pGeo).FaceState = NULL;
  (*pGeo).FacePt = NULL;
  (*pGeo).FacePtCoo = NULL;
  (*pGeo).FaceFF = NULL;
  (*pGeo).FaceDom = NULL;
  (*pGeo).PolyId = NULL;
  (*pGeo).PolyFaceQty = NULL;
  (*pGeo).PolyFaceNb = NULL;
  (*pGeo).PolyFaceOri = NULL;
  (*pGeo).PolyTrue = NULL;
  (*pGeo).PolyBody = NULL;
  (*pGeo).CenterCoo = NULL;
  (*pGeo).morpho = NULL;
  (*pGeo).Type = NULL;
  (*pGeo).Id = -1;
  
  (*pGeo).sel = 0;
  (*pGeo).maxff = 0;
  (*pGeo).dbound = NULL;
  (*pGeo).dboundsel = 0;

  (*pGeo).DomType = NULL;

  (*pGeo).DomVerQty = 0;
  (*pGeo).DomVerEdgeQty = NULL;
  (*pGeo).DomVerEdgeNb = NULL;
  (*pGeo).DomTessVerNb = NULL;

  (*pGeo).DomEdgeQty = 0;
  (*pGeo).DomTessEdgeQty = NULL;
  (*pGeo).DomEdgeVerNb = NULL;
  (*pGeo).DomEdgeFaceNb = NULL;
  (*pGeo).DomTessEdgeNb = NULL;

  (*pGeo).DomFaceQty = 0;
  (*pGeo).DomFaceEq = NULL;
  (*pGeo).DomFaceVerQty = NULL;
  (*pGeo).DomFaceVerNb = NULL;
  (*pGeo).DomFaceEdgeNb = NULL;
  (*pGeo).DomTessFaceQty = NULL;
  (*pGeo).DomTessFaceNb = NULL;

  return;
}
  
void
neut_geo_free (struct GEO* pGeo)
{
  ut_free_1d_char((*pGeo).version);
  ut_free_2d     ((*pGeo).VerCoo, (*pGeo).VerQty + 1);
  ut_free_1d_int ((*pGeo).VerEdgeQty);
  ut_free_2d_int ((*pGeo).VerEdgeNb, (*pGeo).VerQty + 1);
  ut_free_2d_int ((*pGeo).VerDom, (*pGeo).VerQty + 1);
  ut_free_1d_int ((*pGeo).VerState);

  ut_free_2d_int ((*pGeo).EdgeVerNb, (*pGeo).EdgeQty + 1);
  ut_free_1d_int ((*pGeo).EdgeFaceQty);
  ut_free_2d_int ((*pGeo).EdgeFaceNb, (*pGeo).EdgeQty + 1);
  ut_free_1d     ((*pGeo).EdgeLength);
  ut_free_1d_int ((*pGeo).EdgeState);
  ut_free_1d_int ((*pGeo).EdgeDel);
  ut_free_2d_int ((*pGeo).EdgeDom, (*pGeo).EdgeQty + 1);

  ut_free_2d_int ((*pGeo).FacePoly, (*pGeo).FaceQty + 1);
  ut_free_1d_int ((*pGeo).FaceVerQty);
  ut_free_2d_int ((*pGeo).FaceVerNb, (*pGeo).FaceQty + 1);
  ut_free_2d_int ((*pGeo).FaceEdgeNb, (*pGeo).FaceQty + 1);
  ut_free_2d_int ((*pGeo).FaceEdgeOri, (*pGeo).FaceQty + 1);
  ut_free_2d     ((*pGeo).FaceEq, (*pGeo).FaceQty + 1);
  ut_free_1d_int ((*pGeo).FaceState);
  ut_free_1d_int ((*pGeo).FacePt);
  ut_free_2d     ((*pGeo).FacePtCoo, (*pGeo).FaceQty + 1);
  ut_free_1d     ((*pGeo).FaceFF);
  ut_free_2d_int ((*pGeo).FaceDom, (*pGeo).FaceQty + 1);

  ut_free_2d     ((*pGeo).CenterCoo, (*pGeo).PolyQty + 1);

  ut_free_1d_int ((*pGeo).PolyId);
  ut_free_1d_int ((*pGeo).PolyFaceQty);
  ut_free_2d_int ((*pGeo).PolyFaceNb, (*pGeo).PolyQty + 1);
  ut_free_2d_int ((*pGeo).PolyFaceOri, (*pGeo).PolyQty + 1);

  ut_free_1d_int ((*pGeo).PolyTrue);
  ut_free_1d_int ((*pGeo).PolyBody);
  
  ut_free_1d_char ((*pGeo).morpho);
  
  ut_free_1d_char ((*pGeo).dbound);

  ut_free_1d_char ((*pGeo).DomType);

  ut_free_1d_int ((*pGeo).DomVerEdgeQty);
  ut_free_2d_int ((*pGeo).DomVerEdgeNb, (*pGeo).DomVerQty + 1);
  ut_free_1d_int ((*pGeo).DomTessVerNb);

  ut_free_2d_int ((*pGeo).DomEdgeVerNb, (*pGeo).DomEdgeQty + 1);
  ut_free_2d_int ((*pGeo).DomEdgeFaceNb, (*pGeo).DomEdgeQty + 1);
  ut_free_1d_int ((*pGeo).DomTessEdgeQty);
  ut_free_2d_int ((*pGeo).DomTessEdgeNb, (*pGeo).DomEdgeQty + 1);

  ut_free_2d ((*pGeo).DomFaceEq, (*pGeo).DomFaceQty + 1);
  ut_free_1d_int ((*pGeo).DomFaceVerQty);
  ut_free_2d_int ((*pGeo).DomFaceVerNb, (*pGeo).DomFaceQty + 1);
  ut_free_2d_int ((*pGeo).DomFaceEdgeNb, (*pGeo).DomFaceQty + 1);
  ut_free_1d_int ((*pGeo).DomTessFaceQty);
  ut_free_2d_int ((*pGeo).DomTessFaceNb, (*pGeo).DomFaceQty + 1);

  ut_free_1d_char ((*pGeo).Type);

  neut_geo_set_zero (pGeo);

  return;
}


/* Deformation of the geoellation.
 */
void
neut_geo_scale (struct GEO *pGeo, double vx, double vy, double vz)
{
  int i, j = 0, numa, numb;
  double norm;
  double* gsize = ut_alloc_1d (3);
  gsize[0] = vx;
  gsize[1] = vy;
  gsize[2] = vz;

  /* TODO alloc should probably not be done */
  (*pGeo).EdgeLength = ut_alloc_1d ((*pGeo).EdgeQty + 1);

  /* changing vertex coordinates */
  for (i = 1; i <= (*pGeo).VerQty; i++)
    for (j = 0; j < 3; j++)
      (*pGeo).VerCoo[i][j] *= gsize[j];

  /* updating edge lengths from the ver positions */
  for (i = 1; i <= (*pGeo).EdgeQty; i++)
  {
    numa = (*pGeo).EdgeVerNb[i][0];
    numb = (*pGeo).EdgeVerNb[i][1];

    (*pGeo).EdgeLength[i] = ut_space_dist ((*pGeo).VerCoo[numa], (*pGeo).VerCoo[numb]);
  }

  /* updating face equations */
  for (i = 1; i <= (*pGeo).FaceQty; i++)
  {
    (*pGeo).FaceEq[i][1] /= gsize[0];
    (*pGeo).FaceEq[i][2] /= gsize[1];
    (*pGeo).FaceEq[i][3] /= gsize[2];
    norm = ut_vector_norm ((*pGeo).FaceEq[i] + 1);
    for (j = 0; j < 4; j++)
      (*pGeo).FaceEq[i][j] /= norm;
  }

  /* changing centre coordinates */
  for (i = 1; i <= (*pGeo).PolyQty; i++)
    for (j = 0; j < 3; j++)
      (*pGeo).CenterCoo[i][j] *= gsize[j];

  /* changing centre coordinates */
  for (i = 1; i <= (*pGeo).FaceQty; i++)
    if ((*pGeo).FaceState[i] > 0 && (*pGeo).FacePt[i] > 0)
      for (j = 0; j < 3; j++)
	(*pGeo).FacePtCoo[i][j] *= gsize[j];

  /* updating domain face equations */
  for (i = 1; i <= (*pGeo).DomFaceQty; i++)
  {
    (*pGeo).DomFaceEq[i][1] /= gsize[0];
    (*pGeo).DomFaceEq[i][2] /= gsize[1];
    (*pGeo).DomFaceEq[i][3] /= gsize[2];
    norm = ut_vector_norm ((*pGeo).DomFaceEq[i] + 1);
    for (j = 0; j < 4; j++)
      (*pGeo).DomFaceEq[i][j] /= norm;
  }

  (*pGeo).sel *= ut_vector_norm (gsize);
  (*pGeo).dboundsel *= ut_vector_norm (gsize);

  ut_free_1d (gsize);

  return;
}


void
neut_geo_poly_switch (struct GEO* pGeo, int p1, int p2)
{
  int i, id;

  if (p1 == p2)
    return;

  // CenterCoo
  ut_array_2d_switchlines ((*pGeo).CenterCoo, 3, p1, p2);
  
  // PolyTrue
  if ((*pGeo).PolyTrue != NULL)
    ut_array_1d_int_switch ((*pGeo).PolyTrue, p1, p2);

  // PolyBody
  if ((*pGeo).PolyBody != NULL)
    ut_array_1d_int_switch ((*pGeo).PolyBody, p1, p2);
  
  // PolyFace* (keep FaceQty at the end)
  ut_array_2d_int_switchlines_length ((*pGeo).PolyFaceNb,
			       p1, (*pGeo).PolyFaceQty[p1] + 1,
			       p2, (*pGeo).PolyFaceQty[p2] + 1);

  ut_array_2d_int_switchlines_length ((*pGeo).PolyFaceOri,
			       p1, (*pGeo).PolyFaceQty[p1] + 1,
			       p2, (*pGeo).PolyFaceQty[p2] + 1);
  
  ut_array_1d_int_switch ((*pGeo).PolyFaceQty, p1, p2);

  // FacePoly | the following is to avoid problems when p1 and p2 share
  // a common face
  for (i = 1; i <= (*pGeo).PolyFaceQty[p1]; i++)
  {
    id = (*pGeo).PolyFaceNb[p1][i];
    ut_array_1d_int_findnreplace ((*pGeo).FacePoly[id], 2, p2, -1000);
  }
  for (i = 1; i <= (*pGeo).PolyFaceQty[p2]; i++)
  {
    id = (*pGeo).PolyFaceNb[p2][i];
    ut_array_1d_int_findnreplace ((*pGeo).FacePoly[id], 2, p1, p2);
  }
  for (i = 1; i <= (*pGeo).PolyFaceQty[p1]; i++)
  {
    id = (*pGeo).PolyFaceNb[p1][i];
    ut_array_1d_int_findnreplace ((*pGeo).FacePoly[id], 2, -1000, p1);
  }

  return;
}

void
neut_geo_init_edgelength (struct GEO *pGeo)
{
  int i;

  ut_free_1d ((*pGeo).EdgeLength);
  (*pGeo).EdgeLength = ut_alloc_1d ((*pGeo).EdgeQty + 1);

  for (i = 1; i <= (*pGeo).EdgeQty; i++)
    neut_geo_init_edgelength_edge (pGeo, i);

  return;
}

void
neut_geo_init_edgelength_edge (struct GEO *pGeo, int edge)
{
  int ver1, ver2;

  ver1 = (*pGeo).EdgeVerNb[edge][0];
  ver2 = (*pGeo).EdgeVerNb[edge][1];

  (*pGeo).EdgeLength[edge] = ut_space_dist ((*pGeo).VerCoo[ver1],
      (*pGeo).VerCoo[ver2]);

  return;
}

void
neut_geo_init_polystuff_fromfacepoly (struct GEO* pGeo)
{
  int i, j, poly;

  (*pGeo).PolyQty = 0;
  for (i = 1; i <= (*pGeo).FaceQty; i++)
    for (j = 0; j < 2; j++)
      (*pGeo).PolyQty = ut_num_max ((*pGeo).PolyQty, (*pGeo).FacePoly[i][j]);

  (*pGeo).PolyFaceQty = ut_alloc_1d_int ((*pGeo).PolyQty + 1);
  (*pGeo).PolyFaceNb = ut_alloc_2d_int ((*pGeo).PolyQty + 1, 1);

  for (i = 1; i <= (*pGeo).FaceQty; i++)
    for (j = 0; j < 2; j++)
      if ((*pGeo).FacePoly[i][j] > 0)
      {
	poly = (*pGeo).FacePoly[i][j];

	(*pGeo).PolyFaceQty[poly]++;
	(*pGeo).PolyFaceNb[poly] = ut_realloc_1d_int ((*pGeo).PolyFaceNb[poly], (*pGeo).PolyFaceQty[poly] + 1);
	(*pGeo).PolyFaceNb[poly][(*pGeo).PolyFaceQty[poly]] = i;
      }

  return;
}

void
neut_geo_init_edgestuff_fromver (struct GEO* pGeo)
{
  int i, j, edge;

  (*pGeo).EdgeVerNb = ut_alloc_2d_int ((*pGeo).EdgeQty + 1, 2);

  for (i = 1; i <= (*pGeo).VerQty; i++)
    for (j = 0; j < (*pGeo).VerEdgeQty[i]; j++)
    {
      edge = (*pGeo).VerEdgeNb[i][j];
      if ((*pGeo).EdgeVerNb[edge][0] == 0)
	(*pGeo).EdgeVerNb[edge][0] = i;
      else if ((*pGeo).EdgeVerNb[edge][1] == 0)
	(*pGeo).EdgeVerNb[edge][1] = i;
      else
	ut_error_reportbug ();
    }

  neut_geo_init_edgelength (pGeo);
  
  return;
}

void
neut_geo_init_facestuff_fromedge (struct GEO* pGeo)
{
  int i, j, face;

  (*pGeo).FaceVerQty = ut_alloc_1d_int ((*pGeo).FaceQty + 1);
  (*pGeo).FaceEdgeNb = ut_alloc_2d_int ((*pGeo).FaceQty + 1, 1);

  for (i = 1; i <= (*pGeo).EdgeQty; i++)
    for (j = 0; j < (*pGeo).EdgeFaceQty[i]; j++)
    {
      face = (*pGeo).EdgeFaceNb[i][j];
      (*pGeo).FaceVerQty[face]++;
      (*pGeo).FaceEdgeNb[face] = ut_realloc_1d_int ((*pGeo).FaceEdgeNb[face], (*pGeo).FaceVerQty[face] + 1);
      (*pGeo).FaceEdgeNb[face][(*pGeo).FaceVerQty[face]] = i;
    }

  // init FaceState
  (*pGeo).FaceState = ut_realloc_1d_int ((*pGeo).FaceState, (*pGeo).FaceQty + 1);

  return;
}

void
neut_geo_init_facestuff_fromver (struct GEO* pGeo)
{
  int i, j, k, edge, ver, pos;
  int* perm = NULL;

  // permuting FaceEdgeNb and init FaceEdgeOri & FaceVerNb
  (*pGeo).FaceEdgeOri = ut_alloc_1d_pint ((*pGeo).FaceQty + 1);
  (*pGeo).FaceVerNb  = ut_alloc_1d_pint ((*pGeo).FaceQty + 1);
  for (i = 1; i <= (*pGeo).FaceQty; i++)
  {
    perm = ut_alloc_1d_int ((*pGeo).FaceVerQty[i] + 1);
    perm[1] = 1;
    edge = (*pGeo).FaceEdgeNb[i][1];
    ver = (*pGeo).EdgeVerNb[edge][1];
    // looking for the 2nd, 3rd, ... edges
    for (j = 2; j <= (*pGeo).FaceVerQty[i]; j++)
    {
      // checking every edges (we know it's not the 1st)
      for (k = 2; k <= (*pGeo).FaceVerQty[i]; k++)
	if (k != perm[j - 1])
	{
	  edge = (*pGeo).FaceEdgeNb[i][k];
	  pos = ut_array_1d_int_eltpos ((*pGeo).EdgeVerNb[edge], 2, ver);
	  if (pos != -1)
	  {
	    perm[j] = k;
	    ver = (pos == 0) ? (*pGeo).EdgeVerNb[edge][1]
	                     : (*pGeo).EdgeVerNb[edge][0];
	    break;
	  }
	}

      if (perm[j] == 0)
	ut_error_reportbug ();
    }

    ut_array_1d_permutation_int ((*pGeo).FaceEdgeNb[i], (*pGeo).FaceVerQty[i] + 1, perm);

    ut_free_1d_int (perm);

    // init FaceEdgeOri and FaceVerNb
    (*pGeo).FaceEdgeOri[i] = ut_alloc_1d_int ((*pGeo).FaceVerQty[i] + 1);
    (*pGeo).FaceVerNb[i]   = ut_alloc_1d_int ((*pGeo).FaceVerQty[i] + 1);

    edge = (*pGeo).FaceEdgeNb[i][1];
    ver = (*pGeo).EdgeVerNb[edge][1];
    (*pGeo).FaceEdgeOri[i][1] = 1;
    (*pGeo).FaceVerNb[i][1] = (*pGeo).EdgeVerNb[edge][0];
    (*pGeo).FaceVerNb[i][2] = (*pGeo).EdgeVerNb[edge][1];

    for (j = 2; j <= (*pGeo).FaceVerQty[i]; j++)
    {
      edge = (*pGeo).FaceEdgeNb[i][j];
      if ((*pGeo).EdgeVerNb[edge][0] == ver)
      {
	(*pGeo).FaceEdgeOri[i][j] = 1;
	if (j < (*pGeo).FaceVerQty[i])
	  (*pGeo).FaceVerNb[i][j + 1] = (*pGeo).EdgeVerNb[edge][1];
	ver = (*pGeo).EdgeVerNb[edge][1];
      }
      else if ((*pGeo).EdgeVerNb[edge][1] == ver)
      {
	(*pGeo).FaceEdgeOri[i][j] = -1;
	if (j < (*pGeo).FaceVerQty[i])
	  (*pGeo).FaceVerNb[i][j + 1] = (*pGeo).EdgeVerNb[edge][0];
	ver = (*pGeo).EdgeVerNb[edge][0];
      }
      else
	ut_error_reportbug ();
    }
  }

  // reverse FaceVerNb and FaceEdgeNb and modify FaceEdgeOri
  for (i = 1; i <= (*pGeo).FaceQty; i++)
    if (neut_geo_test_face_normal (*pGeo, i, 0) != 0)
    {
      ut_array_1d_int_reverseelts ((*pGeo).FaceEdgeNb[i] + 2,
	                           (*pGeo).FaceVerQty[i] - 1);
      ut_array_1d_int_reverseelts ((*pGeo).FaceVerNb[i] + 2,
	                           (*pGeo).FaceVerQty[i] - 1);
      ut_array_1d_int_reverseelts ((*pGeo).FaceEdgeOri[i] + 2,
	                           (*pGeo).FaceVerQty[i] - 1);
      ut_array_1d_int_scale ((*pGeo).FaceEdgeOri[i] + 1,
			     (*pGeo).FaceVerQty[i] , -1);
    }

  // init FacePt, FacePtCoo, FaceFF
  (*pGeo).FacePt = ut_alloc_1d_int ((*pGeo).FaceQty + 1);
  ut_array_1d_int_set ((*pGeo).FacePt + 1, (*pGeo).FaceQty, -1);

  (*pGeo).FacePtCoo = ut_alloc_2d ((*pGeo).FaceQty + 1, 3);
  (*pGeo).FaceFF = ut_alloc_1d ((*pGeo).FaceQty + 1);

  return;
}

double
neut_geo_init_faceff_face (struct GEO *pGeo, int face)
{
  int i, j, k;
  int ver1, ver2, ver3;
  double ff = 180;
  double minff = 180;
  double thisff = 180;
  double** eq   = ut_alloc_2d ((*pGeo).FaceVerQty[face] + 1, 4);
  double* angle = ut_alloc_1d ((*pGeo).FaceVerQty[face] + 1);

  /* for face point on each ver of the face, the fff is calculated:
   * fff = maximum angle between 2 triangular parts of the face (see
   * the method of interpolation of the face).
   */
  for (i = 1; i <= (*pGeo).FaceVerQty[face]; i++)
  {
    thisff = 0;

    /* ver i is the face point. */
    ver1 = (*pGeo).FaceVerNb[face][i];

    for (j = 1; j <= (*pGeo).FaceVerQty[face] - 2; j++)
    {
      ver2 =
	(*pGeo).FaceVerNb[face][ut_num_rotpos
				(1, (*pGeo).FaceVerQty[face], i, j)];
      ver3 =
	(*pGeo).FaceVerNb[face][ut_num_rotpos
				(1, (*pGeo).FaceVerQty[face], i, j + 1)];

      ut_space_points_plane ((*pGeo).VerCoo[ver1], (*pGeo).VerCoo[ver2],
		 (*pGeo).VerCoo[ver3], eq[j]);
    }
    for (j = 1; j <= (*pGeo).FaceVerQty[face] - 2; j++)
      for (k = j + 1; k <= (*pGeo).FaceVerQty[face] - 2; k++)
      {
	ff = ut_vector_angle (eq[j] + 1, eq[k] + 1);
	thisff = ut_num_max (thisff, ff);
      }

    if (thisff < minff)
    {
      minff = thisff;
      (*pGeo).FacePt[face] = i;
    }
  }

  // case of the barycenter 
  
  // calculating coo
  ut_array_1d_zero ((*pGeo).FacePtCoo[face], 3);
  for (i = 1; i <= (*pGeo).FaceVerQty[face]; i++)
  {
    ver1 = (*pGeo).FaceVerNb[face][i];
    ut_array_1d_add ((*pGeo).FacePtCoo[face], (*pGeo).VerCoo[ver1], 3, (*pGeo).FacePtCoo[face]);
  }
  ut_array_1d_scale ((*pGeo).FacePtCoo[face], 3, 1./(*pGeo).FaceVerQty[face]);

  thisff = 0;
  for (j = 1; j <= (*pGeo).FaceVerQty[face]; j++)
  {
    ver2 =
      (*pGeo).FaceVerNb[face][ut_num_rotpos (1, (*pGeo).FaceVerQty[face], i, j)];
    ver3 =
      (*pGeo).FaceVerNb[face][ut_num_rotpos
			      (1, (*pGeo).FaceVerQty[face], i, j + 1)];

    ut_space_points_plane ((*pGeo).FacePtCoo[face], (*pGeo).VerCoo[ver2],
	       (*pGeo).VerCoo[ver3], eq[j]);
  }
  for (j = 1; j <= (*pGeo).FaceVerQty[face]; j++)
    for (k = j + 1; k <= (*pGeo).FaceVerQty[face] - 2; k++)
    {
      ff = ut_vector_angle (eq[j] + 1, eq[k] + 1);
      thisff = ut_num_max (thisff, ff);
    }

  if (thisff < minff)
  {
    (*pGeo).FacePt[face] = 0;
    minff = thisff;
  }
  else
  {
    ver1 = (*pGeo).FaceVerNb[face][(*pGeo).FacePt[face]];

    ut_array_1d_memcpy ((*pGeo).FacePtCoo[face], 3, (*pGeo).VerCoo[ver1]);
  }

  (*pGeo).FaceFF[face] = minff;

  ut_free_2d (eq, (*pGeo).FaceVerQty[face] + 1);
  ut_free_1d (angle);

  return (*pGeo).FaceFF[face];
}

void
neut_geo_init_polytrue (struct GEO* pGeo)
{
  int i, j, all, face, p;
  int tru, status;
  
  if ((*pGeo).PolyTrue == NULL)
    (*pGeo).PolyTrue = ut_alloc_1d_int ((*pGeo).PolyQty + 1);

  for (i = 1; i <= (*pGeo).PolyQty; i++)
    (*pGeo).PolyTrue[i] = neut_geo_poly_true ((*pGeo), i);

  status = 1;
  for (tru = 2; status == 1; tru++)
  {
    status = 0;
    for (i = 1; i <= (*pGeo).PolyQty; i++)
      if ((*pGeo).PolyTrue[i] == tru - 1)
      {
	all = 1;
	for (j = 1; j <= (*pGeo).PolyFaceQty[i]; j++)
	{
	  face = (*pGeo).PolyFaceNb[i][j];
	  p = ((*pGeo).FacePoly[face][0] != i)? 
		(*pGeo).FacePoly[face][0] : (*pGeo).FacePoly[face][1];

	  if ((*pGeo).PolyTrue[p] < tru - 1)
	  {
	    all = 0;
	    break;
	  }
	}
	if (all == 1)
	{
	  (*pGeo).PolyTrue[i] = tru;
	  status = 1;
	}
      }
  }

  return;
}

void
neut_geo_init_polybody (struct GEO* pGeo)
{
  int i, j, all, face, p;
  int body, status;

  if ((*pGeo).PolyBody == NULL)
    (*pGeo).PolyBody = ut_alloc_1d_int ((*pGeo).PolyQty + 1);

  for (i = 1; i <= (*pGeo).PolyQty; i++)
    (*pGeo).PolyBody[i] = neut_geo_poly_body ((*pGeo), i);

  status = 1;
  for (body = 2; status == 1; body++)
  {
    status = 0;
    for (i = 1; i <= (*pGeo).PolyQty; i++)
    {
      if ((*pGeo).PolyBody[i] == body - 1)
      {
	all = 1;
	for (j = 1; j <= (*pGeo).PolyFaceQty[i]; j++)
	{
	  face = (*pGeo).PolyFaceNb[i][j];
	  p = ((*pGeo).FacePoly[face][0] != i)? 
		(*pGeo).FacePoly[face][0] : (*pGeo).FacePoly[face][1];

	  if ((*pGeo).PolyBody[p] < body - 1)
	  {
	    all = 0;
	    break;
	  }
	}
	if (all == 1)
	{
	  (*pGeo).PolyBody[i] = body;
	  status = 1;
	}
      }
    }
  }

  return;
}

void
neut_geo_init_domain (struct GEO* pGeo)
{
  int i, j, k, ver, domver, domedge, face;
  int*  domface_edgeqty = NULL;
  int** domface_edges   = NULL;
  int*  domedge_verqty = NULL;
  int** domedge_vers   = NULL;
  int interqty;
  int* inter = NULL;

  // Init FaceDom, DomFaceQty, DomFaceEq, DomTessFaceQty and DomTessFaceNb
  (*pGeo).DomFaceQty = neut_geo_init_facedom (pGeo);

  neut_geo_init_domtessface (pGeo);

  (*pGeo).DomFaceEq = ut_alloc_2d ((*pGeo).DomFaceQty + 1, 4);
  for (i = 1; i <= (*pGeo).DomFaceQty; i++)
  {
    face = (*pGeo).DomTessFaceNb[i][1];
    ut_array_1d_memcpy ((*pGeo).DomFaceEq[i], 4, (*pGeo).FaceEq[face]);
  }

  // Init DomEdgeQty, DomEdgeFaceNb, DomTessEdgeQty, DomTessEdgeNb.
  domface_edges   = ut_alloc_1d_pint ((*pGeo).DomFaceQty + 1);
  domface_edgeqty = ut_alloc_1d_int ((*pGeo).DomFaceQty + 1);

  for (i = 1; i <= (*pGeo).DomFaceQty; i++)
    neut_geo_domface_edges ((*pGeo), i, &(domface_edges[i]), &(domface_edgeqty[i]));

  (*pGeo).DomEdgeQty = 0;
  (*pGeo).DomEdgeFaceNb = ut_alloc_1d_pint (1);
  (*pGeo).DomTessEdgeNb = ut_alloc_1d_pint (1);
  for (i = 1; i <= (*pGeo).DomFaceQty; i++)
    for (j = i + 1; j <= (*pGeo).DomFaceQty; j++)
    {
      inter = ut_alloc_1d_int (domface_edgeqty[i]);

      ut_array_1d_int_inter (domface_edges[i], domface_edgeqty[i],
                             domface_edges[j], domface_edgeqty[j],
			     inter,            &interqty         );

      if (interqty > 0) // means that the domain faces share a domain edge
      {
	(*pGeo).DomEdgeQty++;
	domedge = (*pGeo).DomEdgeQty;

	(*pGeo).DomEdgeFaceNb = ut_realloc_2d_int_addline ((*pGeo).DomEdgeFaceNb,
							   (*pGeo).DomEdgeQty + 1, 2);
	(*pGeo).DomEdgeFaceNb[domedge][0] = i;
	(*pGeo).DomEdgeFaceNb[domedge][1] = j;

	(*pGeo).DomTessEdgeQty = ut_realloc_1d_int ((*pGeo).DomTessEdgeQty,
	                                            (*pGeo).DomEdgeQty + 1);
	(*pGeo).DomTessEdgeNb = ut_realloc_2d_int_addline ((*pGeo).DomTessEdgeNb,
	                                            (*pGeo).DomEdgeQty + 1, interqty + 1);

	(*pGeo).DomTessEdgeQty[domedge] = interqty;
	ut_array_1d_int_memcpy ((*pGeo).DomTessEdgeNb[domedge] + 1, interqty, inter);
      }

      ut_free_1d_int (inter);
    }

  // Init DomVerQty, DomVerEdgeQty, DomVerEdgeNb and DomTessVerNb
  domedge_vers   = ut_alloc_1d_pint ((*pGeo).DomEdgeQty + 1);
  domedge_verqty = ut_alloc_1d_int ((*pGeo).DomEdgeQty + 1);

  for (i = 1; i <= (*pGeo).DomEdgeQty; i++)
    neut_geo_domedge_ver ((*pGeo), i, &(domedge_vers[i]), &(domedge_verqty[i]));

  (*pGeo).DomVerQty = 0;
  (*pGeo).DomVerEdgeNb = ut_alloc_1d_pint (1);
  (*pGeo).DomTessVerNb = ut_alloc_1d_int (1);
  for (i = 1; i <= (*pGeo).DomEdgeQty; i++)
    for (j = i + 1; j <= (*pGeo).DomEdgeQty; j++)
    {
      inter = ut_alloc_1d_int (domedge_verqty[i]);

      ut_array_1d_int_inter (domedge_vers[i], domedge_verqty[i],
                             domedge_vers[j], domedge_verqty[j],
			     inter,            &interqty         );

      if (interqty < 0 || interqty > 2)
	ut_error_reportbug ();

      if (interqty == 1) // means that the domain edges share a domain ver
      {
	ver = inter[0];
	domver = ut_array_1d_int_eltpos ((*pGeo).DomTessVerNb, (*pGeo).DomVerQty + 1, ver);

	if (domver == -1)
	{
	  domver = ++(*pGeo).DomVerQty;

	  (*pGeo).DomVerEdgeQty = ut_realloc_1d_int ((*pGeo).DomVerEdgeQty, (*pGeo).DomVerQty + 1);
	  (*pGeo).DomVerEdgeQty[domver] = 2;

	  (*pGeo).DomVerEdgeNb = ut_realloc_2d_int_addline ((*pGeo).DomVerEdgeNb,
							     (*pGeo).DomVerQty + 1, 2);
	  (*pGeo).DomVerEdgeNb[domver][0] = i;
	  (*pGeo).DomVerEdgeNb[domver][1] = j;

	  (*pGeo).DomTessVerNb = ut_realloc_1d_int ((*pGeo).DomTessVerNb,
						    (*pGeo).DomVerQty + 1);

	  (*pGeo).DomTessVerNb[domver] = ver;
	}
	else
	{
	  int* tmpedge = ut_alloc_1d_int (2);
	  tmpedge[0] = i;
	  tmpedge[1] = j;

	  for (k = 0; k < 2; k++)
	    if (ut_array_1d_int_eltpos ((*pGeo).DomVerEdgeNb[domver],
					(*pGeo).DomVerEdgeQty[domver], tmpedge[k]) == -1)
	    {
	      (*pGeo).DomVerEdgeQty[domver]++;

	      (*pGeo).DomVerEdgeNb[domver]
		= ut_realloc_1d_int ((*pGeo).DomVerEdgeNb[domver],
				     (*pGeo).DomVerEdgeQty[domver]);
		  
	      (*pGeo).DomVerEdgeNb[domver][(*pGeo).DomVerEdgeQty[domver] - 1] = tmpedge[k];
	    }

	  ut_free_1d_int (tmpedge);
	}
      }

      ut_free_1d_int (inter);
    }
  
  // Init DomEdgeVerNb
  (*pGeo).DomEdgeVerNb  = ut_alloc_2d_int ((*pGeo).DomEdgeQty + 1, 3);

  for (i = 1; i <= (*pGeo).DomVerQty; i++)
    for (j = 0; j < (*pGeo).DomVerEdgeQty[i]; j++)
    {
      domedge = (*pGeo).DomVerEdgeNb[i][j];
      if (ut_array_1d_int_eltpos ((*pGeo).DomEdgeVerNb[domedge], 2, i) == -1)
      {
	if ((*pGeo).DomEdgeVerNb[domedge][0] == 0)
	  (*pGeo).DomEdgeVerNb[domedge][0] = i;
	else if ((*pGeo).DomEdgeVerNb[domedge][1] == 0)
	  (*pGeo).DomEdgeVerNb[domedge][1] = i;
	else
	  ut_error_reportbug ();
      }
    }

  int x, y;
  if (ut_array_2d_int_eltpos ((*pGeo).DomEdgeVerNb + 1, (*pGeo).DomEdgeQty, 2, 0, &x, &y) != -1)
    ut_error_reportbug ();

  // Init DomFaceVerQty, DomFaceEdgeNb and DomFaceVerNb
  (*pGeo).DomFaceVerQty = ut_alloc_1d_int  ((*pGeo).DomFaceQty + 1);
  (*pGeo).DomFaceEdgeNb = ut_alloc_1d_pint ((*pGeo).DomFaceQty + 1);
  for (i = 1; i <= (*pGeo).DomEdgeQty; i++)
    for (j = 0; j < 2; j++)
    {
      face = (*pGeo).DomEdgeFaceNb[i][j];

      (*pGeo).DomFaceVerQty[face]++; 
      (*pGeo).DomFaceEdgeNb[face]
	= ut_realloc_1d_int ((*pGeo).DomFaceEdgeNb[face], (*pGeo).DomFaceVerQty[face] + 1);
      (*pGeo).DomFaceEdgeNb[face][(*pGeo).DomFaceVerQty[face]] = i;
    }

  (*pGeo).DomFaceVerNb = ut_alloc_1d_pint ((*pGeo).DomFaceQty + 1);
  for (i = 1; i <= (*pGeo).DomFaceQty; i++)
  {
    int qty;
    int* id = NULL;
    neut_geo_domedges_domvers (*pGeo, (*pGeo).DomFaceEdgeNb[i] + 1, (*pGeo).DomFaceVerQty[i], 
	                              &id, &qty);

    if (qty != (*pGeo).DomFaceVerQty[i])
      ut_error_reportbug ();

    (*pGeo).DomFaceVerNb[i] = ut_alloc_1d_int ((*pGeo).DomFaceVerQty[i] + 1);
    ut_array_1d_int_memcpy ((*pGeo).DomFaceVerNb[i] + 1, qty, id);

    ut_free_1d_int (id);
  }

  neut_geo_init_edgedom (pGeo);
  neut_geo_init_verdom (pGeo);

  return;
}

void
neut_geo_init_domain_poly (struct GEO* pGeo, struct POLY Domain, char* type)
{
  // Copying domain to the GEO
  neut_geo_init_domain_memcpy (pGeo, Domain, type);

  // We are left with linking the Geo to the Domain
  
  neut_geo_init_facedom (pGeo);
  neut_geo_init_edgedom (pGeo);
  neut_geo_init_verdom  (pGeo);

  neut_geo_init_domtessface (pGeo);
  neut_geo_init_domtessedge (pGeo);
  neut_geo_init_domtessver  (pGeo);

  return;
}

void
neut_geo_init_domtessface (struct GEO* pGeo)
{
  int i, id;

  ut_free_1d_int ((*pGeo).DomTessFaceQty);
  ut_free_2d_int ((*pGeo).DomTessFaceNb, (*pGeo).DomFaceQty + 1);

  (*pGeo).DomTessFaceQty = ut_alloc_1d_int ((*pGeo).DomFaceQty + 1);
  (*pGeo).DomTessFaceNb  = ut_alloc_1d_pint ((*pGeo).DomFaceQty + 1);

  for (i = 1; i <= (*pGeo).FaceQty; i++)
    if ((*pGeo).FaceDom[i][0] == 2)
    {
      id = (*pGeo).FaceDom[i][1];
      (*pGeo).DomTessFaceQty[id]++;
      (*pGeo).DomTessFaceNb[id] = ut_realloc_1d_int ((*pGeo).DomTessFaceNb[id],
	                                             (*pGeo).DomTessFaceQty[id] + 1);
      (*pGeo).DomTessFaceNb[id][(*pGeo).DomTessFaceQty[id]] = i;
    }

  return;
}

void
neut_geo_init_domtessedge (struct GEO* pGeo)
{
  int i, j, k, edge, dface1, dface2, status;
  int* nb = NULL;
    
  nb = ut_alloc_1d_int (2);

  ut_free_1d_int ((*pGeo).DomTessEdgeQty);
  ut_free_2d_int ((*pGeo).DomTessEdgeNb, (*pGeo).DomEdgeQty + 1);

  (*pGeo).DomTessEdgeQty = ut_alloc_1d_int ((*pGeo).DomEdgeQty + 1);
  (*pGeo).DomTessEdgeNb = ut_alloc_1d_pint ((*pGeo).DomEdgeQty + 1);

  for (i = 1; i <= (*pGeo).DomEdgeQty; i++)
  {
    dface1 = (*pGeo).DomEdgeFaceNb[i][0];
    dface2 = (*pGeo).DomEdgeFaceNb[i][1];

    for (j = 1; j <= (*pGeo).DomTessFaceQty[dface1]; j++)
    {
      nb[0] = (*pGeo).DomTessFaceNb[dface1][j];

      for (k = 1; k <= (*pGeo).DomTessFaceQty[dface2]; k++)
      {
	nb[1] = (*pGeo).DomTessFaceNb[dface2][k];

	status = neut_geo_faces_commonedge (*pGeo, nb, 2, &edge);

	if (status == 0)
	{
	  ((*pGeo).DomTessEdgeQty[i])++;
	  (*pGeo).DomTessEdgeNb[i] = ut_realloc_1d_int ((*pGeo).DomTessEdgeNb[i],
	                                                (*pGeo).DomTessEdgeQty[i] + 1);

	  (*pGeo).DomTessEdgeNb[i][(*pGeo).DomTessEdgeQty[i]] = edge;
	  break;
	}
      }
    }
  }
  
  ut_free_1d_int (nb);

  return;
}

void
neut_geo_init_domtessver (struct GEO* pGeo)
{
  int i;
  int* nb = NULL;

  ut_free_1d_int ((*pGeo).DomTessVerNb);

  (*pGeo).DomTessVerNb = ut_alloc_1d_int ((*pGeo).DomVerQty + 1);

  for (i = 1; i <= (*pGeo).VerQty; i++)
    if ((*pGeo).VerDom[i][0] == 0)
      (*pGeo).DomTessVerNb[(*pGeo).VerDom[i][1]] = i;

  ut_free_1d_int (nb);

  return;
}

int
neut_geo_init_facedom (struct GEO* pGeo)
{
  int i, id, max = 0;

  // Init face bound
  (*pGeo).FaceDom = ut_alloc_2d_int ((*pGeo).FaceQty + 1, 2);

  for (i = 1; i <= (*pGeo).FaceQty; i++)
    (*pGeo).FaceDom[i][0] = -1;

  for (i = 1; i <= (*pGeo).FaceQty; i++)
  {
    id = -ut_array_1d_int_min ((*pGeo).FacePoly[i], 2);

    if (id > 0)
    {
      (*pGeo).FaceDom[i][0] = 2;
      (*pGeo).FaceDom[i][1] = id;
      max = ut_num_max (max, id);
    }
  }

  return max;
}

void
neut_geo_init_edgedom (struct GEO* pGeo)
{
  int qty, i, j, face, status;
  int* nb = NULL;

  (*pGeo).EdgeDom = ut_alloc_2d_int ((*pGeo).EdgeQty + 1, 2);

  for (i = 1; i <= (*pGeo).EdgeQty; i++)
  {
    nb = ut_alloc_1d_int ((*pGeo).EdgeFaceQty[i]);
    
    qty = 0;
    for (j = 0; j < (*pGeo).EdgeFaceQty[i]; j++)
    {
      face = (*pGeo).EdgeFaceNb[i][j];

      if ((*pGeo).FaceDom[face][0] == 2)
	nb[qty++] = (*pGeo).FaceDom[face][1];
    }

    ut_array_1d_int_sort_uniq (nb, qty, &qty);

    if (qty == 0) // the edge is not on the domain
      (*pGeo).EdgeDom[i][0] = -1;
    else if (qty == 1) // the edge is on a domain face
    {
      (*pGeo).EdgeDom[i][0] = 2;
      (*pGeo).EdgeDom[i][1] = nb[1];
    }
    else if (qty == 2) // the edge is on a domain edge
    {
      (*pGeo).EdgeDom[i][0] = 1;
      status = neut_geo_domfaces_commondomedge (*pGeo, nb, 2, &((*pGeo).EdgeDom[i][1]));
      if (status == -1)
	ut_error_reportbug ();
    }
    else
      ut_error_reportbug ();

    ut_free_1d_int (nb);
  }

  return;
}

void
neut_geo_init_verdom (struct GEO* pGeo)
{
  int i, j, verbosity = 0;
  int faceqty, domfaceqty;
  int* face = NULL;
  int* domface = NULL;

  (*pGeo).VerDom = ut_alloc_2d_int ((*pGeo).VerQty + 1, 2);

  for (i = 1; i <= (*pGeo).VerQty; i++)
  {
    if (verbosity)
    {
      printf ("i = %d\n", i);
      printf ("coo = ");
      ut_array_1d_fprintf (stdout, (*pGeo).VerCoo[i], 3, "%f");
    }

    neut_geo_ver_faces (*pGeo, i, &face, &faceqty);

    if (verbosity)
    {
      printf ("ver_face = ");
      ut_array_1d_int_fprintf (stdout, face, faceqty, "%d");
    }
    
    domface = ut_alloc_1d_int (faceqty + 1);

    domfaceqty = 0;
    for (j = 0; j < faceqty; j++)
      if ((*pGeo).FaceDom[face[j]][0] == 2)
	domface[domfaceqty++] = (*pGeo).FaceDom[face[j]][1];

    if (verbosity)
    {
      printf ("ver_domface = ");
      ut_array_1d_int_fprintf (stdout, domface, domfaceqty, "%d");
    }

    ut_array_1d_int_sort_uniq (domface, domfaceqty, &domfaceqty);

    if (verbosity)
    {
      printf ("ver_domface = ");
      ut_array_1d_int_fprintf (stdout, domface, faceqty, "%d");
    }

    if (domfaceqty == 0) // the ver is not on the domain
      (*pGeo).VerDom[i][0] = -1;
    else if (domfaceqty == 1) // the ver is on a domain face
    {
      (*pGeo).VerDom[i][0] = 2;
      (*pGeo).VerDom[i][1] = domface[0];
    }
    else if (domfaceqty == 2) // the ver is on a domain edge
    {
      (*pGeo).VerDom[i][0] = 1;
      if (neut_geo_domfaces_commondomedge (*pGeo, domface, domfaceqty, &((*pGeo).VerDom[i][1])) != 0)
	ut_error_reportbug ();
    }
    else if (domfaceqty >= 3) // the ver is on a domain ver
    {
      (*pGeo).VerDom[i][0] = 0;
      if (neut_geo_domfaces_commondomver (*pGeo, domface, domfaceqty, &((*pGeo).VerDom[i][1])) != 0)
	ut_error_reportbug ();
    }

    ut_free_1d_int (domface);
    ut_free_1d_int (face);
    face = NULL;
  }

  return;
}

void
neut_geo_init_facedom_face_v (struct GEO* pGeo, int face)
{
  int i, ver, qty;
  int domfaceqty1, domfaceqty2;
  int* domface1 = NULL;
  int* domface2 = NULL;

  // default value
  (*pGeo).FaceDom[face][0] = -1;

  // if a ver is not on the domain, the face is not on the domain
  for (i = 1; i <= (*pGeo).FaceVerQty[face]; i++)
  {
    ver = (*pGeo).FaceVerNb[face][i];

    if ((*pGeo).VerDom[ver][0] == -1)
    {
      (*pGeo).FaceDom[face][0] = -1;
      return;
    }
  }

  // for all of the face vertices
  domfaceqty1 = 0;
  for (i = 1; i <= (*pGeo).FaceVerQty[face]; i++)
  {
    ver = (*pGeo).FaceVerNb[face][i];

    int* tmp = NULL;
    if ((*pGeo).VerDom[ver][0] == 0)
      neut_geo_domver_domface (*pGeo, (*pGeo).VerDom[ver][1], &tmp, &qty);
    else if ((*pGeo).VerDom[ver][0] == 1)
    {
      qty = 2;
      tmp = ut_alloc_1d_int (qty);
      ut_array_1d_int_memcpy (tmp, 2, (*pGeo).DomEdgeFaceNb[(*pGeo).VerDom[ver][1]]);
    }
    else if ((*pGeo).VerDom[ver][0] == 2)
    {
      qty = 1;
      tmp = ut_alloc_1d_int (qty);
      tmp[0] = (*pGeo).VerDom[ver][1];
    }

    domface1 = ut_realloc_1d_int (domface1, domfaceqty1 + qty);
    ut_array_1d_int_memcpy (domface1 + domfaceqty1, qty, tmp);
    domfaceqty1 += qty;

    ut_free_1d_int (tmp);
  }

  domface2 = ut_alloc_1d_int (domfaceqty1);
  ut_array_1d_int_memcpy (domface2, domfaceqty1, domface1);
  ut_array_1d_int_sort_uniq (domface2, domfaceqty1, &domfaceqty2);
  domface2 = ut_realloc_1d_int (domface2, domfaceqty2);

  for (i = 0; i < domfaceqty2; i++)
  {
    qty = ut_array_1d_int_nbofthisval (domface1, domfaceqty1, domface2[i]);
    if (qty == (*pGeo).FaceVerQty[face])
    {
      (*pGeo).FaceDom[face][0] = 2;
      (*pGeo).FaceDom[face][1] = domface2[i];
      break;
    }
  }

  ut_free_1d_int (domface1);
  ut_free_1d_int (domface2);

  return;
}
