/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_tess_op_lcl.h"

void
neut_tess_set_zero (struct TESS* pTess)
{
  (*pTess).version = NULL;
  (*pTess).N = 0;
  (*pTess).Id = -1;
  (*pTess).morpho = 0;
  (*pTess).Type = NULL;
  (*pTess).sel = 0;
  (*pTess).maxff = 0;
  (*pTess).dbound = NULL;
  (*pTess).dboundsel = 0;

  (*pTess).VerQty = 0;
  (*pTess).FaceQty = 0;
  (*pTess).EdgeQty = 0;
  (*pTess).PolyQty = 0;
  (*pTess).VerCoo = NULL;
  (*pTess).VerEdgeQty = NULL;
  (*pTess).VerEdgeNb = NULL;
  (*pTess).VerDom = NULL;
  (*pTess).VerState = NULL;
  (*pTess).EdgeVerNb = NULL;
  (*pTess).EdgeFaceQty = NULL;
  (*pTess).EdgeFaceNb = NULL;
  (*pTess).EdgeLength = NULL;
  (*pTess).EdgeState = NULL;
  (*pTess).EdgeDel = NULL;
  (*pTess).EdgeDom = NULL;
  (*pTess).FacePoly = NULL;
  (*pTess).FaceVerQty = NULL;
  (*pTess).FaceVerNb = NULL;
  (*pTess).FaceEdgeNb = NULL;
  (*pTess).FaceEdgeOri = NULL;
  (*pTess).FaceEq = NULL;
  (*pTess).FaceState = NULL;
  (*pTess).FacePt = NULL;
  (*pTess).FacePtCoo = NULL;
  (*pTess).FaceFF = NULL;
  (*pTess).FaceDom = NULL;
  (*pTess).PolyId = NULL;
  (*pTess).PolyFaceQty = NULL;
  (*pTess).PolyFaceNb = NULL;
  (*pTess).PolyFaceOri = NULL;
  (*pTess).PolyTrue = NULL;
  (*pTess).PolyBody = NULL;
  (*pTess).CenterCoo = NULL;

  (*pTess).DomType = NULL;

  (*pTess).DomVerQty = 0;
  (*pTess).DomVerEdgeQty = NULL;
  (*pTess).DomVerEdgeNb = NULL;
  (*pTess).DomTessVerNb = NULL;

  (*pTess).DomEdgeQty = 0;
  (*pTess).DomTessEdgeQty = NULL;
  (*pTess).DomEdgeVerNb = NULL;
  (*pTess).DomEdgeFaceNb = NULL;
  (*pTess).DomTessEdgeNb = NULL;

  (*pTess).DomFaceQty = 0;
  (*pTess).DomFaceLabel = NULL;
  (*pTess).DomFaceEq = NULL;
  (*pTess).DomFaceVerQty = NULL;
  (*pTess).DomFaceVerNb = NULL;
  (*pTess).DomFaceEdgeNb = NULL;
  (*pTess).DomTessFaceQty = NULL;
  (*pTess).DomTessFaceNb = NULL;

  return;
}
  
void
neut_tess_free (struct TESS* pTess)
{
  ut_free_1d_char((*pTess).version);
  ut_free_2d     ((*pTess).VerCoo, (*pTess).VerQty + 1);
  ut_free_1d_int ((*pTess).VerEdgeQty);
  ut_free_2d_int ((*pTess).VerEdgeNb, (*pTess).VerQty + 1);
  ut_free_2d_int ((*pTess).VerDom, (*pTess).VerQty + 1);
  ut_free_1d_int ((*pTess).VerState);

  ut_free_2d_int ((*pTess).EdgeVerNb, (*pTess).EdgeQty + 1);
  ut_free_1d_int ((*pTess).EdgeFaceQty);
  ut_free_2d_int ((*pTess).EdgeFaceNb, (*pTess).EdgeQty + 1);
  ut_free_1d     ((*pTess).EdgeLength);
  ut_free_1d_int ((*pTess).EdgeState);
  ut_free_1d_int ((*pTess).EdgeDel);
  ut_free_2d_int ((*pTess).EdgeDom, (*pTess).EdgeQty + 1);

  ut_free_2d_int ((*pTess).FacePoly, (*pTess).FaceQty + 1);
  ut_free_1d_int ((*pTess).FaceVerQty);
  ut_free_2d_int ((*pTess).FaceVerNb, (*pTess).FaceQty + 1);
  ut_free_2d_int ((*pTess).FaceEdgeNb, (*pTess).FaceQty + 1);
  ut_free_2d_int ((*pTess).FaceEdgeOri, (*pTess).FaceQty + 1);
  ut_free_2d     ((*pTess).FaceEq, (*pTess).FaceQty + 1);
  ut_free_1d_int ((*pTess).FaceState);
  ut_free_1d_int ((*pTess).FacePt);
  ut_free_2d     ((*pTess).FacePtCoo, (*pTess).FaceQty + 1);
  ut_free_1d     ((*pTess).FaceFF);
  ut_free_2d_int ((*pTess).FaceDom, (*pTess).FaceQty + 1);

  ut_free_2d     ((*pTess).CenterCoo, (*pTess).PolyQty + 1);

  ut_free_1d_int ((*pTess).PolyId);
  ut_free_1d_int ((*pTess).PolyFaceQty);
  ut_free_2d_int ((*pTess).PolyFaceNb, (*pTess).PolyQty + 1);
  ut_free_2d_int ((*pTess).PolyFaceOri, (*pTess).PolyQty + 1);

  ut_free_1d_int ((*pTess).PolyTrue);
  ut_free_1d_int ((*pTess).PolyBody);
  
  ut_free_1d_char ((*pTess).morpho);
  
  ut_free_1d_char ((*pTess).dbound);

  ut_free_1d_char ((*pTess).DomType);

  ut_free_1d_int ((*pTess).DomVerEdgeQty);
  ut_free_2d_int ((*pTess).DomVerEdgeNb, (*pTess).DomVerQty + 1);
  ut_free_1d_int ((*pTess).DomTessVerNb);

  ut_free_2d_int ((*pTess).DomEdgeVerNb, (*pTess).DomEdgeQty + 1);
  ut_free_2d_int ((*pTess).DomEdgeFaceNb, (*pTess).DomEdgeQty + 1);
  ut_free_1d_int ((*pTess).DomTessEdgeQty);
  ut_free_2d_int ((*pTess).DomTessEdgeNb, (*pTess).DomEdgeQty + 1);

  ut_free_2d_char ((*pTess).DomFaceLabel, (*pTess).DomFaceQty + 1);
  ut_free_2d ((*pTess).DomFaceEq, (*pTess).DomFaceQty + 1);
  ut_free_1d_int ((*pTess).DomFaceVerQty);
  ut_free_2d_int ((*pTess).DomFaceVerNb, (*pTess).DomFaceQty + 1);
  ut_free_2d_int ((*pTess).DomFaceEdgeNb, (*pTess).DomFaceQty + 1);
  ut_free_1d_int ((*pTess).DomTessFaceQty);
  ut_free_2d_int ((*pTess).DomTessFaceNb, (*pTess).DomFaceQty + 1);

  ut_free_1d_char ((*pTess).Type);

  neut_tess_set_zero (pTess);

  return;
}


/* Deformation of the geoellation.
 */
void
neut_tess_scale (struct TESS *pTess, double vx, double vy, double vz)
{
  int i, j = 0, numa, numb;
  double norm;
  double* gsize = ut_alloc_1d (3);
  gsize[0] = vx;
  gsize[1] = vy;
  gsize[2] = vz;

  /* TODO alloc should probably not be done */
  (*pTess).EdgeLength = ut_alloc_1d ((*pTess).EdgeQty + 1);

  /* changing vertex coordinates */
  for (i = 1; i <= (*pTess).VerQty; i++)
    for (j = 0; j < 3; j++)
      (*pTess).VerCoo[i][j] *= gsize[j];

  /* updating edge lengths from the ver positions */
  for (i = 1; i <= (*pTess).EdgeQty; i++)
  {
    numa = (*pTess).EdgeVerNb[i][0];
    numb = (*pTess).EdgeVerNb[i][1];

    (*pTess).EdgeLength[i] = ut_space_dist ((*pTess).VerCoo[numa], (*pTess).VerCoo[numb]);
  }

  /* updating face equations */
  for (i = 1; i <= (*pTess).FaceQty; i++)
  {
    (*pTess).FaceEq[i][1] /= gsize[0];
    (*pTess).FaceEq[i][2] /= gsize[1];
    (*pTess).FaceEq[i][3] /= gsize[2];
    norm = ut_vector_norm ((*pTess).FaceEq[i] + 1);
    for (j = 0; j < 4; j++)
      (*pTess).FaceEq[i][j] /= norm;
  }

  /* changing centre coordinates */
  for (i = 1; i <= (*pTess).PolyQty; i++)
    for (j = 0; j < 3; j++)
      (*pTess).CenterCoo[i][j] *= gsize[j];

  /* changing centre coordinates */
  for (i = 1; i <= (*pTess).FaceQty; i++)
    if ((*pTess).FaceState[i] > 0 && (*pTess).FacePt[i] > 0)
      for (j = 0; j < 3; j++)
	(*pTess).FacePtCoo[i][j] *= gsize[j];

  /* updating domain face equations */
  for (i = 1; i <= (*pTess).DomFaceQty; i++)
  {
    (*pTess).DomFaceEq[i][1] /= gsize[0];
    (*pTess).DomFaceEq[i][2] /= gsize[1];
    (*pTess).DomFaceEq[i][3] /= gsize[2];
    norm = ut_vector_norm ((*pTess).DomFaceEq[i] + 1);
    for (j = 0; j < 4; j++)
      (*pTess).DomFaceEq[i][j] /= norm;
  }

  (*pTess).sel *= ut_vector_norm (gsize);
  (*pTess).dboundsel *= ut_vector_norm (gsize);

  ut_free_1d (gsize);

  return;
}


void
neut_tess_poly_switch (struct TESS* pTess, int p1, int p2)
{
  int i, id;

  if (p1 == p2)
    return;

  // CenterCoo
  ut_array_2d_switchlines ((*pTess).CenterCoo, 3, p1, p2);
  
  // PolyTrue
  if ((*pTess).PolyTrue != NULL)
    ut_array_1d_int_switch ((*pTess).PolyTrue, p1, p2);

  // PolyBody
  if ((*pTess).PolyBody != NULL)
    ut_array_1d_int_switch ((*pTess).PolyBody, p1, p2);
  
  // PolyFace* (keep FaceQty at the end)
  ut_array_2d_int_switchlines_length ((*pTess).PolyFaceNb,
			       p1, (*pTess).PolyFaceQty[p1] + 1,
			       p2, (*pTess).PolyFaceQty[p2] + 1);

  ut_array_2d_int_switchlines_length ((*pTess).PolyFaceOri,
			       p1, (*pTess).PolyFaceQty[p1] + 1,
			       p2, (*pTess).PolyFaceQty[p2] + 1);
  
  ut_array_1d_int_switch ((*pTess).PolyFaceQty, p1, p2);

  // FacePoly | the following is to avoid problems when p1 and p2 share
  // a common face
  for (i = 1; i <= (*pTess).PolyFaceQty[p1]; i++)
  {
    id = (*pTess).PolyFaceNb[p1][i];
    ut_array_1d_int_findnreplace ((*pTess).FacePoly[id], 2, p2, -1000);
  }
  for (i = 1; i <= (*pTess).PolyFaceQty[p2]; i++)
  {
    id = (*pTess).PolyFaceNb[p2][i];
    ut_array_1d_int_findnreplace ((*pTess).FacePoly[id], 2, p1, p2);
  }
  for (i = 1; i <= (*pTess).PolyFaceQty[p1]; i++)
  {
    id = (*pTess).PolyFaceNb[p1][i];
    ut_array_1d_int_findnreplace ((*pTess).FacePoly[id], 2, -1000, p1);
  }

  return;
}

void
neut_tess_init_edgelength (struct TESS *pTess)
{
  int i;

  ut_free_1d ((*pTess).EdgeLength);
  (*pTess).EdgeLength = ut_alloc_1d ((*pTess).EdgeQty + 1);

  for (i = 1; i <= (*pTess).EdgeQty; i++)
    neut_tess_init_edgelength_edge (pTess, i);

  return;
}

void
neut_tess_init_edgelength_edge (struct TESS *pTess, int edge)
{
  int ver1, ver2;

  ver1 = (*pTess).EdgeVerNb[edge][0];
  ver2 = (*pTess).EdgeVerNb[edge][1];

  (*pTess).EdgeLength[edge] = ut_space_dist ((*pTess).VerCoo[ver1],
      (*pTess).VerCoo[ver2]);

  return;
}

void
neut_tess_init_polystuff_fromfacepoly (struct TESS* pTess)
{
  int i, j, poly;

  (*pTess).PolyQty = 0;
  for (i = 1; i <= (*pTess).FaceQty; i++)
    for (j = 0; j < 2; j++)
      (*pTess).PolyQty = ut_num_max ((*pTess).PolyQty, (*pTess).FacePoly[i][j]);

  (*pTess).PolyFaceQty = ut_alloc_1d_int ((*pTess).PolyQty + 1);
  (*pTess).PolyFaceNb = ut_alloc_2d_int ((*pTess).PolyQty + 1, 1);

  for (i = 1; i <= (*pTess).FaceQty; i++)
    for (j = 0; j < 2; j++)
      if ((*pTess).FacePoly[i][j] > 0)
      {
	poly = (*pTess).FacePoly[i][j];

	(*pTess).PolyFaceQty[poly]++;
	(*pTess).PolyFaceNb[poly] = ut_realloc_1d_int ((*pTess).PolyFaceNb[poly], (*pTess).PolyFaceQty[poly] + 1);
	(*pTess).PolyFaceNb[poly][(*pTess).PolyFaceQty[poly]] = i;
      }

  return;
}

void
neut_tess_init_edgestuff_fromver (struct TESS* pTess)
{
  int i, j, edge;

  (*pTess).EdgeVerNb = ut_alloc_2d_int ((*pTess).EdgeQty + 1, 2);

  for (i = 1; i <= (*pTess).VerQty; i++)
    for (j = 0; j < (*pTess).VerEdgeQty[i]; j++)
    {
      edge = (*pTess).VerEdgeNb[i][j];
      if ((*pTess).EdgeVerNb[edge][0] == 0)
	(*pTess).EdgeVerNb[edge][0] = i;
      else if ((*pTess).EdgeVerNb[edge][1] == 0)
	(*pTess).EdgeVerNb[edge][1] = i;
      else
	ut_error_reportbug ();
    }

  neut_tess_init_edgelength (pTess);
  
  return;
}

void
neut_tess_init_facestuff_fromedge (struct TESS* pTess)
{
  int i, j, face;

  (*pTess).FaceVerQty = ut_alloc_1d_int ((*pTess).FaceQty + 1);
  (*pTess).FaceEdgeNb = ut_alloc_2d_int ((*pTess).FaceQty + 1, 1);

  for (i = 1; i <= (*pTess).EdgeQty; i++)
    for (j = 0; j < (*pTess).EdgeFaceQty[i]; j++)
    {
      face = (*pTess).EdgeFaceNb[i][j];
      (*pTess).FaceVerQty[face]++;
      (*pTess).FaceEdgeNb[face] = ut_realloc_1d_int ((*pTess).FaceEdgeNb[face], (*pTess).FaceVerQty[face] + 1);
      (*pTess).FaceEdgeNb[face][(*pTess).FaceVerQty[face]] = i;
    }

  // init FaceState
  (*pTess).FaceState = ut_realloc_1d_int ((*pTess).FaceState, (*pTess).FaceQty + 1);

  return;
}

void
neut_tess_init_facestuff_fromver (struct TESS* pTess)
{
  int i, j, k, edge, ver, pos;
  int* perm = NULL;

  // permuting FaceEdgeNb and init FaceEdgeOri & FaceVerNb
  (*pTess).FaceEdgeOri = ut_alloc_1d_pint ((*pTess).FaceQty + 1);
  (*pTess).FaceVerNb  = ut_alloc_1d_pint ((*pTess).FaceQty + 1);
  for (i = 1; i <= (*pTess).FaceQty; i++)
  {
    perm = ut_alloc_1d_int ((*pTess).FaceVerQty[i] + 1);
    perm[1] = 1;
    edge = (*pTess).FaceEdgeNb[i][1];
    ver = (*pTess).EdgeVerNb[edge][1];
    // looking for the 2nd, 3rd, ... edges
    for (j = 2; j <= (*pTess).FaceVerQty[i]; j++)
    {
      // checking every edges (we know it's not the 1st)
      for (k = 2; k <= (*pTess).FaceVerQty[i]; k++)
	if (k != perm[j - 1])
	{
	  edge = (*pTess).FaceEdgeNb[i][k];
	  pos = ut_array_1d_int_eltpos ((*pTess).EdgeVerNb[edge], 2, ver);
	  if (pos != -1)
	  {
	    perm[j] = k;
	    ver = (pos == 0) ? (*pTess).EdgeVerNb[edge][1]
	                     : (*pTess).EdgeVerNb[edge][0];
	    break;
	  }
	}

      if (perm[j] == 0)
	ut_error_reportbug ();
    }

    ut_array_1d_permutation_int ((*pTess).FaceEdgeNb[i], (*pTess).FaceVerQty[i] + 1, perm);

    ut_free_1d_int (perm);

    // init FaceEdgeOri and FaceVerNb
    (*pTess).FaceEdgeOri[i] = ut_alloc_1d_int ((*pTess).FaceVerQty[i] + 1);
    (*pTess).FaceVerNb[i]   = ut_alloc_1d_int ((*pTess).FaceVerQty[i] + 1);

    edge = (*pTess).FaceEdgeNb[i][1];
    ver = (*pTess).EdgeVerNb[edge][1];
    (*pTess).FaceEdgeOri[i][1] = 1;
    (*pTess).FaceVerNb[i][1] = (*pTess).EdgeVerNb[edge][0];
    (*pTess).FaceVerNb[i][2] = (*pTess).EdgeVerNb[edge][1];

    for (j = 2; j <= (*pTess).FaceVerQty[i]; j++)
    {
      edge = (*pTess).FaceEdgeNb[i][j];
      if ((*pTess).EdgeVerNb[edge][0] == ver)
      {
	(*pTess).FaceEdgeOri[i][j] = 1;
	if (j < (*pTess).FaceVerQty[i])
	  (*pTess).FaceVerNb[i][j + 1] = (*pTess).EdgeVerNb[edge][1];
	ver = (*pTess).EdgeVerNb[edge][1];
      }
      else if ((*pTess).EdgeVerNb[edge][1] == ver)
      {
	(*pTess).FaceEdgeOri[i][j] = -1;
	if (j < (*pTess).FaceVerQty[i])
	  (*pTess).FaceVerNb[i][j + 1] = (*pTess).EdgeVerNb[edge][0];
	ver = (*pTess).EdgeVerNb[edge][0];
      }
      else
	ut_error_reportbug ();
    }
  }

  // reverse FaceVerNb and FaceEdgeNb and modify FaceEdgeOri
  for (i = 1; i <= (*pTess).FaceQty; i++)
    if (neut_tess_test_face_normal (*pTess, i, 0) != 0)
    {
      ut_array_1d_int_reverseelts ((*pTess).FaceEdgeNb[i] + 2,
	                           (*pTess).FaceVerQty[i] - 1);
      ut_array_1d_int_reverseelts ((*pTess).FaceVerNb[i] + 2,
	                           (*pTess).FaceVerQty[i] - 1);
      ut_array_1d_int_reverseelts ((*pTess).FaceEdgeOri[i] + 2,
	                           (*pTess).FaceVerQty[i] - 1);
      ut_array_1d_int_scale ((*pTess).FaceEdgeOri[i] + 1,
			     (*pTess).FaceVerQty[i] , -1);
    }

  // init FacePt, FacePtCoo, FaceFF
  (*pTess).FacePt = ut_alloc_1d_int ((*pTess).FaceQty + 1);
  ut_array_1d_int_set ((*pTess).FacePt + 1, (*pTess).FaceQty, -1);

  (*pTess).FacePtCoo = ut_alloc_2d ((*pTess).FaceQty + 1, 3);
  (*pTess).FaceFF = ut_alloc_1d ((*pTess).FaceQty + 1);

  return;
}

double
neut_tess_init_faceff_face (struct TESS *pTess, int face)
{
  int i, j, k;
  int ver1, ver2, ver3;
  double ff = 180;
  double minff = 180;
  double thisff = 180;
  double** eq   = ut_alloc_2d ((*pTess).FaceVerQty[face] + 1, 4);
  double* angle = ut_alloc_1d ((*pTess).FaceVerQty[face] + 1);

  /* for face point on each ver of the face, the fff is calculated:
   * fff = maximum angle between 2 triangular parts of the face (see
   * the method of interpolation of the face).
   */
  for (i = 1; i <= (*pTess).FaceVerQty[face]; i++)
  {
    thisff = 0;

    /* ver i is the face point. */
    ver1 = (*pTess).FaceVerNb[face][i];

    for (j = 1; j <= (*pTess).FaceVerQty[face] - 2; j++)
    {
      ver2 =
	(*pTess).FaceVerNb[face][ut_num_rotpos
				(1, (*pTess).FaceVerQty[face], i, j)];
      ver3 =
	(*pTess).FaceVerNb[face][ut_num_rotpos
				(1, (*pTess).FaceVerQty[face], i, j + 1)];

      ut_space_points_plane ((*pTess).VerCoo[ver1], (*pTess).VerCoo[ver2],
		 (*pTess).VerCoo[ver3], eq[j]);
    }
    for (j = 1; j <= (*pTess).FaceVerQty[face] - 2; j++)
      for (k = j + 1; k <= (*pTess).FaceVerQty[face] - 2; k++)
      {
	ff = ut_vector_angle (eq[j] + 1, eq[k] + 1);
	thisff = ut_num_max (thisff, ff);
      }

    if (thisff < minff)
    {
      minff = thisff;
      (*pTess).FacePt[face] = i;
    }
  }

  // case of the barycenter 
  
  // calculating coo
  ut_array_1d_zero ((*pTess).FacePtCoo[face], 3);
  for (i = 1; i <= (*pTess).FaceVerQty[face]; i++)
  {
    ver1 = (*pTess).FaceVerNb[face][i];
    ut_array_1d_add ((*pTess).FacePtCoo[face], (*pTess).VerCoo[ver1], 3, (*pTess).FacePtCoo[face]);
  }
  ut_array_1d_scale ((*pTess).FacePtCoo[face], 3, 1./(*pTess).FaceVerQty[face]);

  thisff = 0;
  for (j = 1; j <= (*pTess).FaceVerQty[face]; j++)
  {
    ver2 =
      (*pTess).FaceVerNb[face][ut_num_rotpos (1, (*pTess).FaceVerQty[face], i, j)];
    ver3 =
      (*pTess).FaceVerNb[face][ut_num_rotpos
			      (1, (*pTess).FaceVerQty[face], i, j + 1)];

    ut_space_points_plane ((*pTess).FacePtCoo[face], (*pTess).VerCoo[ver2],
	       (*pTess).VerCoo[ver3], eq[j]);
  }
  for (j = 1; j <= (*pTess).FaceVerQty[face]; j++)
    for (k = j + 1; k <= (*pTess).FaceVerQty[face] - 2; k++)
    {
      ff = ut_vector_angle (eq[j] + 1, eq[k] + 1);
      thisff = ut_num_max (thisff, ff);
    }

  if (thisff < minff)
  {
    (*pTess).FacePt[face] = 0;
    minff = thisff;
  }
  else
  {
    ver1 = (*pTess).FaceVerNb[face][(*pTess).FacePt[face]];

    ut_array_1d_memcpy ((*pTess).FacePtCoo[face], 3, (*pTess).VerCoo[ver1]);
  }

  (*pTess).FaceFF[face] = minff;

  ut_free_2d (eq, (*pTess).FaceVerQty[face] + 1);
  ut_free_1d (angle);

  return (*pTess).FaceFF[face];
}

void
neut_tess_init_polytrue (struct TESS* pTess)
{
  int i, j, all, face, p;
  int tru, status;
  
  if ((*pTess).PolyTrue == NULL)
    (*pTess).PolyTrue = ut_alloc_1d_int ((*pTess).PolyQty + 1);

  for (i = 1; i <= (*pTess).PolyQty; i++)
    (*pTess).PolyTrue[i] = neut_tess_poly_true ((*pTess), i);

  status = 1;
  for (tru = 2; status == 1; tru++)
  {
    status = 0;
    for (i = 1; i <= (*pTess).PolyQty; i++)
      if ((*pTess).PolyTrue[i] == tru - 1)
      {
	all = 1;
	for (j = 1; j <= (*pTess).PolyFaceQty[i]; j++)
	{
	  face = (*pTess).PolyFaceNb[i][j];
	  p = ((*pTess).FacePoly[face][0] != i)? 
		(*pTess).FacePoly[face][0] : (*pTess).FacePoly[face][1];

	  if ((*pTess).PolyTrue[p] < tru - 1)
	  {
	    all = 0;
	    break;
	  }
	}
	if (all == 1)
	{
	  (*pTess).PolyTrue[i] = tru;
	  status = 1;
	}
      }
  }

  return;
}

void
neut_tess_init_polybody (struct TESS* pTess)
{
  int i, j, all, face, p;
  int body, status;

  if ((*pTess).PolyBody == NULL)
    (*pTess).PolyBody = ut_alloc_1d_int ((*pTess).PolyQty + 1);

  for (i = 1; i <= (*pTess).PolyQty; i++)
    (*pTess).PolyBody[i] = neut_tess_poly_body ((*pTess), i);

  status = 1;
  for (body = 2; status == 1; body++)
  {
    status = 0;
    for (i = 1; i <= (*pTess).PolyQty; i++)
    {
      if ((*pTess).PolyBody[i] == body - 1)
      {
	all = 1;
	for (j = 1; j <= (*pTess).PolyFaceQty[i]; j++)
	{
	  face = (*pTess).PolyFaceNb[i][j];
	  p = ((*pTess).FacePoly[face][0] != i)? 
		(*pTess).FacePoly[face][0] : (*pTess).FacePoly[face][1];

	  if ((*pTess).PolyBody[p] < body - 1)
	  {
	    all = 0;
	    break;
	  }
	}
	if (all == 1)
	{
	  (*pTess).PolyBody[i] = body;
	  status = 1;
	}
      }
    }
  }

  return;
}

void
neut_tess_init_domain (struct TESS* pTess)
{
  int i, j, k, ver, domver, domedge, face, poly, faceori;
  int*  domface_edgeqty = NULL;
  int** domface_edges   = NULL;
  int*  domedge_verqty = NULL;
  int** domedge_vers   = NULL;
  int interqty;
  int* inter = NULL;

  // Init FaceDom, DomFaceQty, DomFaceEq, DomTessFaceQty and DomTessFaceNb
  (*pTess).DomFaceQty = neut_tess_init_facedom (pTess);

  neut_tess_init_domtessface (pTess);

  (*pTess).DomFaceEq = ut_alloc_2d ((*pTess).DomFaceQty + 1, 4);
  for (i = 1; i <= (*pTess).DomFaceQty; i++)
  {
    face = (*pTess).DomTessFaceNb[i][1];
    poly = ut_array_1d_int_max ((*pTess).FacePoly[face], 2);
    neut_tess_poly_face_ori (*pTess, poly, face, &faceori);
    ut_array_1d_memcpy ((*pTess).DomFaceEq[i], 4, (*pTess).FaceEq[face]);
    ut_array_1d_scale ((*pTess).DomFaceEq[i], 4, faceori);
  }

  // Init DomEdgeQty, DomEdgeFaceNb, DomTessEdgeQty, DomTessEdgeNb.
  domface_edges   = ut_alloc_1d_pint ((*pTess).DomFaceQty + 1);
  domface_edgeqty = ut_alloc_1d_int ((*pTess).DomFaceQty + 1);

  for (i = 1; i <= (*pTess).DomFaceQty; i++)
    neut_tess_domface_edges ((*pTess), i, &(domface_edges[i]), &(domface_edgeqty[i]));

  (*pTess).DomEdgeQty = 0;
  (*pTess).DomEdgeFaceNb = ut_alloc_1d_pint (1);
  (*pTess).DomTessEdgeNb = ut_alloc_1d_pint (1);
  for (i = 1; i <= (*pTess).DomFaceQty; i++)
    for (j = i + 1; j <= (*pTess).DomFaceQty; j++)
    {
      inter = ut_alloc_1d_int (domface_edgeqty[i]);

      ut_array_1d_int_inter (domface_edges[i], domface_edgeqty[i],
                             domface_edges[j], domface_edgeqty[j],
			     inter,            &interqty         );

      if (interqty > 0) // means that the domain faces share a domain edge
      {
	(*pTess).DomEdgeQty++;
	domedge = (*pTess).DomEdgeQty;

	(*pTess).DomEdgeFaceNb = ut_realloc_2d_int_addline ((*pTess).DomEdgeFaceNb,
							   (*pTess).DomEdgeQty + 1, 2);
	(*pTess).DomEdgeFaceNb[domedge][0] = i;
	(*pTess).DomEdgeFaceNb[domedge][1] = j;

	(*pTess).DomTessEdgeQty = ut_realloc_1d_int ((*pTess).DomTessEdgeQty,
	                                            (*pTess).DomEdgeQty + 1);
	(*pTess).DomTessEdgeNb = ut_realloc_2d_int_addline ((*pTess).DomTessEdgeNb,
	                                            (*pTess).DomEdgeQty + 1, interqty + 1);

	(*pTess).DomTessEdgeQty[domedge] = interqty;
	ut_array_1d_int_memcpy ((*pTess).DomTessEdgeNb[domedge] + 1, interqty, inter);
      }

      ut_free_1d_int (inter);
    }

  // Init DomVerQty, DomVerEdgeQty, DomVerEdgeNb and DomTessVerNb
  domedge_vers   = ut_alloc_1d_pint ((*pTess).DomEdgeQty + 1);
  domedge_verqty = ut_alloc_1d_int ((*pTess).DomEdgeQty + 1);

  for (i = 1; i <= (*pTess).DomEdgeQty; i++)
    neut_tess_domedge_ver ((*pTess), i, &(domedge_vers[i]), &(domedge_verqty[i]));

  (*pTess).DomVerQty = 0;
  (*pTess).DomVerEdgeNb = ut_alloc_1d_pint (1);
  (*pTess).DomTessVerNb = ut_alloc_1d_int (1);
  for (i = 1; i <= (*pTess).DomEdgeQty; i++)
    for (j = i + 1; j <= (*pTess).DomEdgeQty; j++)
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
	domver = ut_array_1d_int_eltpos ((*pTess).DomTessVerNb, (*pTess).DomVerQty + 1, ver);

	if (domver == -1)
	{
	  domver = ++(*pTess).DomVerQty;

	  (*pTess).DomVerEdgeQty = ut_realloc_1d_int ((*pTess).DomVerEdgeQty, (*pTess).DomVerQty + 1);
	  (*pTess).DomVerEdgeQty[domver] = 2;

	  (*pTess).DomVerEdgeNb = ut_realloc_2d_int_addline ((*pTess).DomVerEdgeNb,
							     (*pTess).DomVerQty + 1, 2);
	  (*pTess).DomVerEdgeNb[domver][0] = i;
	  (*pTess).DomVerEdgeNb[domver][1] = j;

	  (*pTess).DomTessVerNb = ut_realloc_1d_int ((*pTess).DomTessVerNb,
						    (*pTess).DomVerQty + 1);

	  (*pTess).DomTessVerNb[domver] = ver;
	}
	else
	{
	  int* tmpedge = ut_alloc_1d_int (2);
	  tmpedge[0] = i;
	  tmpedge[1] = j;

	  for (k = 0; k < 2; k++)
	    if (ut_array_1d_int_eltpos ((*pTess).DomVerEdgeNb[domver],
					(*pTess).DomVerEdgeQty[domver], tmpedge[k]) == -1)
	    {
	      (*pTess).DomVerEdgeQty[domver]++;

	      (*pTess).DomVerEdgeNb[domver]
		= ut_realloc_1d_int ((*pTess).DomVerEdgeNb[domver],
				     (*pTess).DomVerEdgeQty[domver]);
		  
	      (*pTess).DomVerEdgeNb[domver][(*pTess).DomVerEdgeQty[domver] - 1] = tmpedge[k];
	    }

	  ut_free_1d_int (tmpedge);
	}
      }

      ut_free_1d_int (inter);
    }
  
  // Init DomEdgeVerNb
  (*pTess).DomEdgeVerNb  = ut_alloc_2d_int ((*pTess).DomEdgeQty + 1, 3);

  for (i = 1; i <= (*pTess).DomVerQty; i++)
    for (j = 0; j < (*pTess).DomVerEdgeQty[i]; j++)
    {
      domedge = (*pTess).DomVerEdgeNb[i][j];
      if (ut_array_1d_int_eltpos ((*pTess).DomEdgeVerNb[domedge], 2, i) == -1)
      {
	if ((*pTess).DomEdgeVerNb[domedge][0] == 0)
	  (*pTess).DomEdgeVerNb[domedge][0] = i;
	else if ((*pTess).DomEdgeVerNb[domedge][1] == 0)
	  (*pTess).DomEdgeVerNb[domedge][1] = i;
	else
	  ut_error_reportbug ();
      }
    }

  int x, y;
  if (ut_array_2d_int_eltpos ((*pTess).DomEdgeVerNb + 1, (*pTess).DomEdgeQty, 2, 0, &x, &y) != -1)
    ut_error_reportbug ();

  // Init DomFaceVerQty, DomFaceEdgeNb and DomFaceVerNb
  (*pTess).DomFaceVerQty = ut_alloc_1d_int  ((*pTess).DomFaceQty + 1);
  (*pTess).DomFaceEdgeNb = ut_alloc_1d_pint ((*pTess).DomFaceQty + 1);
  for (i = 1; i <= (*pTess).DomEdgeQty; i++)
    for (j = 0; j < 2; j++)
    {
      face = (*pTess).DomEdgeFaceNb[i][j];

      (*pTess).DomFaceVerQty[face]++; 
      (*pTess).DomFaceEdgeNb[face]
	= ut_realloc_1d_int ((*pTess).DomFaceEdgeNb[face], (*pTess).DomFaceVerQty[face] + 1);
      (*pTess).DomFaceEdgeNb[face][(*pTess).DomFaceVerQty[face]] = i;
    }

  (*pTess).DomFaceVerNb = ut_alloc_1d_pint ((*pTess).DomFaceQty + 1);
  for (i = 1; i <= (*pTess).DomFaceQty; i++)
  {
    int qty;
    int* id = NULL;
    neut_tess_domedges_domvers (*pTess, (*pTess).DomFaceEdgeNb[i] + 1, (*pTess).DomFaceVerQty[i], 
	                              &id, &qty);

    if (qty != (*pTess).DomFaceVerQty[i])
      ut_error_reportbug ();

    (*pTess).DomFaceVerNb[i] = ut_alloc_1d_int ((*pTess).DomFaceVerQty[i] + 1);
    ut_array_1d_int_memcpy ((*pTess).DomFaceVerNb[i] + 1, qty, id);

    ut_free_1d_int (id);
  }

  neut_tess_init_edgedom (pTess);
  neut_tess_init_verdom (pTess);

  // Settings face labels
  neut_tess_init_domain_facelabel (pTess);

  return;
}

void
neut_tess_init_domain_poly (struct TESS* pTess, struct POLY Domain, char* type)
{
  // Copying domain to the TESS
  neut_tess_init_domain_memcpy (pTess, Domain, type);

  // We are left with linking the Tess to the Domain
  
  neut_tess_init_facedom (pTess);
  neut_tess_init_edgedom (pTess);
  neut_tess_init_verdom  (pTess);

  neut_tess_init_domtessface (pTess);
  neut_tess_init_domtessedge (pTess);
  neut_tess_init_domtessver  (pTess);

  return;
}

void
neut_tess_init_domtessface (struct TESS* pTess)
{
  int i, id;

  ut_free_1d_int ((*pTess).DomTessFaceQty);
  ut_free_2d_int ((*pTess).DomTessFaceNb, (*pTess).DomFaceQty + 1);

  (*pTess).DomTessFaceQty = ut_alloc_1d_int ((*pTess).DomFaceQty + 1);
  (*pTess).DomTessFaceNb  = ut_alloc_1d_pint ((*pTess).DomFaceQty + 1);

  for (i = 1; i <= (*pTess).FaceQty; i++)
    if ((*pTess).FaceDom[i][0] == 2)
    {
      id = (*pTess).FaceDom[i][1];
      (*pTess).DomTessFaceQty[id]++;
      (*pTess).DomTessFaceNb[id] = ut_realloc_1d_int ((*pTess).DomTessFaceNb[id],
	                                             (*pTess).DomTessFaceQty[id] + 1);
      (*pTess).DomTessFaceNb[id][(*pTess).DomTessFaceQty[id]] = i;
    }

  return;
}

void
neut_tess_init_domtessedge (struct TESS* pTess)
{
  int i, j, k, edge, dface1, dface2, status;
  int* nb = NULL;
    
  nb = ut_alloc_1d_int (2);

  ut_free_1d_int ((*pTess).DomTessEdgeQty);
  ut_free_2d_int ((*pTess).DomTessEdgeNb, (*pTess).DomEdgeQty + 1);

  (*pTess).DomTessEdgeQty = ut_alloc_1d_int ((*pTess).DomEdgeQty + 1);
  (*pTess).DomTessEdgeNb = ut_alloc_1d_pint ((*pTess).DomEdgeQty + 1);

  for (i = 1; i <= (*pTess).DomEdgeQty; i++)
  {
    dface1 = (*pTess).DomEdgeFaceNb[i][0];
    dface2 = (*pTess).DomEdgeFaceNb[i][1];

    for (j = 1; j <= (*pTess).DomTessFaceQty[dface1]; j++)
    {
      nb[0] = (*pTess).DomTessFaceNb[dface1][j];

      for (k = 1; k <= (*pTess).DomTessFaceQty[dface2]; k++)
      {
	nb[1] = (*pTess).DomTessFaceNb[dface2][k];

	status = neut_tess_faces_commonedge (*pTess, nb, 2, &edge);

	if (status == 0)
	{
	  ((*pTess).DomTessEdgeQty[i])++;
	  (*pTess).DomTessEdgeNb[i] = ut_realloc_1d_int ((*pTess).DomTessEdgeNb[i],
	                                                (*pTess).DomTessEdgeQty[i] + 1);

	  (*pTess).DomTessEdgeNb[i][(*pTess).DomTessEdgeQty[i]] = edge;
	  break;
	}
      }
    }
  }
  
  ut_free_1d_int (nb);

  return;
}

void
neut_tess_init_domtessver (struct TESS* pTess)
{
  int i;
  int* nb = NULL;

  ut_free_1d_int ((*pTess).DomTessVerNb);

  (*pTess).DomTessVerNb = ut_alloc_1d_int ((*pTess).DomVerQty + 1);

  for (i = 1; i <= (*pTess).VerQty; i++)
    if ((*pTess).VerDom[i][0] == 0)
      (*pTess).DomTessVerNb[(*pTess).VerDom[i][1]] = i;

  ut_free_1d_int (nb);

  return;
}

int
neut_tess_init_facedom (struct TESS* pTess)
{
  int i, id, max = 0;

  // Init face bound
  (*pTess).FaceDom = ut_alloc_2d_int ((*pTess).FaceQty + 1, 2);

  for (i = 1; i <= (*pTess).FaceQty; i++)
    (*pTess).FaceDom[i][0] = -1;

  for (i = 1; i <= (*pTess).FaceQty; i++)
  {
    id = -ut_array_1d_int_min ((*pTess).FacePoly[i], 2);

    if (id > 0)
    {
      (*pTess).FaceDom[i][0] = 2;
      (*pTess).FaceDom[i][1] = id;
      max = ut_num_max (max, id);
    }
  }

  return max;
}

void
neut_tess_init_edgedom (struct TESS* pTess)
{
  int qty, i, j, face, status;
  int* nb = NULL;

  (*pTess).EdgeDom = ut_alloc_2d_int ((*pTess).EdgeQty + 1, 2);

  for (i = 1; i <= (*pTess).EdgeQty; i++)
  {
    nb = ut_alloc_1d_int ((*pTess).EdgeFaceQty[i]);
    
    qty = 0;
    for (j = 0; j < (*pTess).EdgeFaceQty[i]; j++)
    {
      face = (*pTess).EdgeFaceNb[i][j];

      if ((*pTess).FaceDom[face][0] == 2)
	nb[qty++] = (*pTess).FaceDom[face][1];
    }

    ut_array_1d_int_sort_uniq (nb, qty, &qty);

    if (qty == 0) // the edge is not on the domain
      (*pTess).EdgeDom[i][0] = -1;
    else if (qty == 1) // the edge is on a domain face
    {
      (*pTess).EdgeDom[i][0] = 2;
      (*pTess).EdgeDom[i][1] = nb[1];
    }
    else if (qty == 2) // the edge is on a domain edge
    {
      (*pTess).EdgeDom[i][0] = 1;
      status = neut_tess_domfaces_commondomedge (*pTess, nb, 2, &((*pTess).EdgeDom[i][1]));
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
neut_tess_init_verdom (struct TESS* pTess)
{
  int i, j, verbosity = 0;
  int faceqty, domfaceqty;
  int* face = NULL;
  int* domface = NULL;

  (*pTess).VerDom = ut_alloc_2d_int ((*pTess).VerQty + 1, 2);

  for (i = 1; i <= (*pTess).VerQty; i++)
  {
    if (verbosity)
    {
      printf ("i = %d\n", i);
      printf ("coo = ");
      ut_array_1d_fprintf (stdout, (*pTess).VerCoo[i], 3, "%f");
    }

    neut_tess_ver_faces (*pTess, i, &face, &faceqty);

    if (verbosity)
    {
      printf ("ver_face = ");
      ut_array_1d_int_fprintf (stdout, face, faceqty, "%d");
    }
    
    domface = ut_alloc_1d_int (faceqty + 1);

    domfaceqty = 0;
    for (j = 0; j < faceqty; j++)
      if ((*pTess).FaceDom[face[j]][0] == 2)
	domface[domfaceqty++] = (*pTess).FaceDom[face[j]][1];

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
      (*pTess).VerDom[i][0] = -1;
    else if (domfaceqty == 1) // the ver is on a domain face
    {
      (*pTess).VerDom[i][0] = 2;
      (*pTess).VerDom[i][1] = domface[0];
    }
    else if (domfaceqty == 2) // the ver is on a domain edge
    {
      (*pTess).VerDom[i][0] = 1;
      if (neut_tess_domfaces_commondomedge (*pTess, domface, domfaceqty, &((*pTess).VerDom[i][1])) != 0)
	ut_error_reportbug ();
    }
    else if (domfaceqty >= 3) // the ver is on a domain ver
    {
      (*pTess).VerDom[i][0] = 0;
      if (neut_tess_domfaces_commondomver (*pTess, domface, domfaceqty, &((*pTess).VerDom[i][1])) != 0)
	ut_error_reportbug ();
    }

    ut_free_1d_int (domface);
    ut_free_1d_int (face);
    face = NULL;
  }

  return;
}

void
neut_tess_init_facedom_face_v (struct TESS* pTess, int face)
{
  int i, ver, qty;
  int domfaceqty1, domfaceqty2;
  int* domface1 = NULL;
  int* domface2 = NULL;

  // default value
  (*pTess).FaceDom[face][0] = -1;

  // if a ver is not on the domain, the face is not on the domain
  for (i = 1; i <= (*pTess).FaceVerQty[face]; i++)
  {
    ver = (*pTess).FaceVerNb[face][i];

    if ((*pTess).VerDom[ver][0] == -1)
    {
      (*pTess).FaceDom[face][0] = -1;
      return;
    }
  }

  // for all of the face vertices
  domfaceqty1 = 0;
  for (i = 1; i <= (*pTess).FaceVerQty[face]; i++)
  {
    ver = (*pTess).FaceVerNb[face][i];

    int* tmp = NULL;
    if ((*pTess).VerDom[ver][0] == 0)
      neut_tess_domver_domface (*pTess, (*pTess).VerDom[ver][1], &tmp, &qty);
    else if ((*pTess).VerDom[ver][0] == 1)
    {
      qty = 2;
      tmp = ut_alloc_1d_int (qty);
      ut_array_1d_int_memcpy (tmp, 2, (*pTess).DomEdgeFaceNb[(*pTess).VerDom[ver][1]]);
    }
    else if ((*pTess).VerDom[ver][0] == 2)
    {
      qty = 1;
      tmp = ut_alloc_1d_int (qty);
      tmp[0] = (*pTess).VerDom[ver][1];
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
    if (qty == (*pTess).FaceVerQty[face])
    {
      (*pTess).FaceDom[face][0] = 2;
      (*pTess).FaceDom[face][1] = domface2[i];
      break;
    }
  }

  ut_free_1d_int (domface1);
  ut_free_1d_int (domface2);

  return;
}
