/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_tesl.h"

/* neut_tesl_set_zero: set a tesl to 0 / NULL */
void
neut_tesl_set_zero (struct TESL* pTess)
{
  (*pTess).Type = NULL;
  (*pTess).Id   = 0;
  (*pTess).VerQty = 0;
  (*pTess).FaceQty = 0;
  (*pTess).EdgeQty = 0;
  (*pTess).PolyQty = 0;
  
  (*pTess).morpho = NULL;
  (*pTess).VerGerm = NULL;
  (*pTess).VerEdge = NULL;
  (*pTess).VerCoo = NULL;
  (*pTess).VerOfDomain = NULL;
  (*pTess).EdgeVerNb = NULL;
  (*pTess).EdgeLength = NULL;
  (*pTess).FacePoly = NULL;
  (*pTess).FaceEq = NULL;
  (*pTess).FaceVerQty = NULL;
  (*pTess).FaceVerNb = NULL;
  (*pTess).FaceEdgeNb = NULL;
  (*pTess).FaceEdgeOri = NULL;
  (*pTess).CenterCoo = NULL;
  (*pTess).PolyVerQty = NULL;
  (*pTess).PolyVerNb = NULL;
  (*pTess).PolyEdgeQty = NULL;
  (*pTess).PolyEdgeNb = NULL;
  (*pTess).PolyFaceQty = NULL;
  (*pTess).PolyFaceNb = NULL;
  (*pTess).PolyFaceOri = NULL;
  (*pTess).PolyTrue = NULL;
  (*pTess).PolyBody = NULL;

  return;
}

void
neut_tesl_free (struct TESL* pTess)
{
  ut_free_1d_char ((*pTess).Type);
  ut_free_1d_char ((*pTess).morpho);
  ut_free_2d_int ((*pTess).VerGerm, (*pTess).VerQty + 1);
  ut_free_2d_int ((*pTess).VerEdge, (*pTess).VerQty + 1);
  ut_free_2d     ((*pTess).VerCoo, (*pTess).VerQty + 1);
  ut_free_1d_int ((*pTess).VerOfDomain);

  ut_free_2d_int ((*pTess).EdgeVerNb, (*pTess).EdgeQty + 1);
  ut_free_1d     ((*pTess).EdgeLength);
  ut_free_2d_int ((*pTess).FacePoly, (*pTess).FaceQty + 1);
  ut_free_2d     ((*pTess).FaceEq, (*pTess).FaceQty + 1);
  ut_free_1d_int ((*pTess).FaceVerQty);
  ut_free_2d_int ((*pTess).FaceVerNb, (*pTess).FaceQty + 1);
  ut_free_2d_int ((*pTess).FaceEdgeNb, (*pTess).FaceQty + 1);
  ut_free_2d_int ((*pTess).FaceEdgeOri, (*pTess).FaceQty + 1);
  ut_free_2d     ((*pTess).CenterCoo, (*pTess).PolyQty + 1);
  ut_free_1d_int ((*pTess).PolyVerQty);
  ut_free_2d_int ((*pTess).PolyVerNb, (*pTess).PolyQty + 1);
  ut_free_1d_int ((*pTess).PolyEdgeQty);
  ut_free_2d_int ((*pTess).PolyEdgeNb, (*pTess).PolyQty + 1);
  ut_free_1d_int ((*pTess).PolyFaceQty);
  ut_free_2d_int ((*pTess).PolyFaceNb, (*pTess).PolyQty + 1);
  ut_free_2d_int ((*pTess).PolyFaceOri, (*pTess).PolyQty + 1);
  ut_free_1d_int ((*pTess).PolyTrue);
  ut_free_1d_int ((*pTess).PolyBody);

  neut_tesl_set_zero (pTess);

  return;
}

void
neut_tesl_bbox (struct TESL Tess, double** size)
{
  int i, j, ver;

  /* Searching the tessellation bounding box */
  size[0][0] = size[1][0] = size[2][0] = DBL_MAX;
  size[0][1] = size[1][1] = size[2][1] = -DBL_MAX;

  for (i = 1; i <= Tess.VerOfDomain[0]; i++)
  {
    ver = Tess.VerOfDomain[i];
    for (j = 0; j < 3; j++)
    {
      size[j][0] = ut_num_min (size[j][0], Tess.VerCoo[ver][j]);
      size[j][1] = ut_num_max (size[j][1], Tess.VerCoo[ver][j]);
    }
  }

  return;
}

void
neut_tesl_domainface (struct TESL Tess, int **Face)
{
  int i, j;
  int face;

  for (face = 1; face <= 6; face++)
    Face[face][0] = 0;

  for (i = 1; i <= Tess.FaceQty; i++)
  {
    for (j = 0; j <= 1; j++)
      if (Tess.FacePoly[i][j] < 0)
      {
	face = -Tess.FacePoly[i][j];

	Face[face][0]++;
	Face[face][Face[face][0]] = i;

	continue;
      }
  }

  return;
}
  
void
neut_tesl_domainfacever (struct TESL Tess, int **Ver)
{
  int i, j, k;
  int face;
  int **DFace = ut_alloc_2d_int (7, Tess.FaceQty + 1);
  int *FaceVer;

  neut_tesl_domainface (Tess, DFace);

  for (i = 1; i <= 6; i++)
  {
    FaceVer = ut_alloc_1d_int (DFace[i][0] * 50);

    for (j = 1; j <= DFace[i][0]; j++)
    {
      face = DFace[i][j];
      for (k = 1; k <= Tess.FaceVerQty[face]; k++)
      {
	FaceVer[0]++;
	FaceVer[FaceVer[0]] = Tess.FaceVerNb[face][k];
      }
    }

    ut_array_1d_int_sort_uniq (FaceVer + 1, FaceVer[0], &(FaceVer[0]));

    ut_array_1d_int_memcpy (Ver[i], FaceVer[0] + 1, FaceVer);

    ut_free_1d_int (FaceVer);
  }

  ut_free_2d_int (DFace, 7);

  return;
}

int
neut_tesl_poly_true (struct TESL Tess, int nb)
{
  int i, j, k, res;
  int ver;
  double ctov;
  double *coo = ut_alloc_1d (3);
  double** size = ut_alloc_2d (3, 2);

  neut_tesl_bbox (Tess, size);

  res = 1;
  for (i = 1; i <= Tess.PolyVerQty[nb]; i++)
  {
    ver = Tess.PolyVerNb[nb][i];
    ctov = ut_space_dist (Tess.CenterCoo[nb], Tess.VerCoo[ver]);

    for (j = 0; j < 3; j++)
      for (k = 0; k < 2; k++)
	if (fabs (Tess.VerCoo[ver][j] - size[j][k]) < 0.999 * ctov)
	  {
	    res = 0;
	    break;
	  }
  }

  ut_free_1d (coo);
  ut_free_2d (size, 3);

  return res;
}

/* returns 0 if not true, 1 if true & 2 if all parent poly are true */
int
neut_tesl_ver_true (struct TESL Tess, int *TruePoly, int nb)
{
  int j;
  int poly, res, all;

  res = 0;
  all = 1;
  for (j = 0; j < 4; j++)
  {
    poly = Tess.VerGerm[nb][j];
    if (poly > 0)
    {
      if (TruePoly[poly] == 1)
	res = 1;
      else
	all = 0;
    }
  }

  if (res == 1 && all == 1)
    res = 2;

  return res;
}

/* returns 0 if not true, 1 if true & 2 if all parent poly are true */
int
neut_tesl_edge_true (struct TESL Tess, int *TruePoly, int nb)
{
  int j;
  int res, all;
  int* poly = ut_alloc_1d_int (5);

  neut_tesl_edge_poly (Tess, nb, poly);

  res = 0;
  all = 1;
  for (j = 1; j <= poly[0]; j++)
  {
    if (poly[j] > 0)
    {
      if (TruePoly[poly[j]] == 1)
	res = 1;
      else
	all = 0;
    }
  }

  if (res == 1 && all == 1)
    res = 2;

  ut_free_1d_int (poly);

  return res;
}

void
neut_tesl_edge_poly (struct TESL Tess, int edge, int* poly)
{
  int i, ver1, ver2;

  ver1 = Tess.EdgeVerNb[edge][0];
  ver2 = Tess.EdgeVerNb[edge][1];

  ut_array_1d_int_inter (Tess.VerGerm[ver1], 4, Tess.VerGerm[ver2], 4, \
                     poly + 1, &(poly[0]));
  
  ut_array_1d_int_sort (poly + 1, poly[0]);
  
  for (i = -1; i >= -6; i--)
    poly[0] -= ut_array_1d_int_deletencompress (poly + 1, poly[0], i, 1);
  
  return;
}

/* returns 0 if not true, 1 if true & 2 if all parent poly are true */
int
neut_tesl_face_true (struct TESL Tess, int *TruePoly, int nb)
{
  int j;
  int res, all;

  res = 0;
  all = 1;
  for (j = 0; j < 2; j++)
  {
    if (Tess.FacePoly[nb][j] > 0)
    {
      if (TruePoly[Tess.FacePoly[nb][j]] == 1)
	res = 1;
      else
	all = 0;
    }
    else
    {
      res = 0;
      break;
    }
  }

  if (res == 1 && all == 1)
    res = 2;

  return res;
}

void
neut_tesl_init_polytrue (struct TESL* pTess)
{
  int i, j, all, face, p;
  int tru, status;
  
  if ((*pTess).PolyTrue == NULL)
    (*pTess).PolyTrue = ut_alloc_1d_int ((*pTess).PolyQty + 1);

  for (i = 1; i <= (*pTess).PolyQty; i++)
    (*pTess).PolyTrue[i] = neut_tesl_poly_true ((*pTess), i);

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

int
neut_tesl_face_area (struct TESL Tess, int face, double* parea)
{
  int i, j, ver;
  double* p0 = ut_alloc_1d (3);
  double* p1 = NULL;
  double* p2 = NULL;

  if (face < 1 || face > Tess.FaceQty)
  {
    (*parea) = 0;
    return -1;
  }

  // taking barycenter
  ut_array_1d_zero (p0, 3);
  for (i = 1; i <= Tess.FaceVerQty[face]; i++)
  {
    ver = Tess.FaceVerNb[face][i];

    for (j = 0; j < 3; j++)
      p0[j] += Tess.VerCoo[ver][j];
  }
  ut_array_1d_scale (p0, 3, 1./Tess.FaceVerQty[face]);

  (*parea) = 0;
  for (i = 1; i < Tess.FaceVerQty[face]; i++)
  {
    p1 = Tess.VerCoo[Tess.FaceVerNb[face][i]];
    p2 = Tess.VerCoo[Tess.FaceVerNb[face][i + 1]];
    (*parea) += ut_space_triangle_area (p0, p1, p2);
  }

  p1 = Tess.VerCoo[Tess.FaceVerNb[face][Tess.FaceVerQty[face]]];
  p2 = Tess.VerCoo[Tess.FaceVerNb[face][1]];
  (*parea) += ut_space_triangle_area (p0, p1, p2);

  ut_free_1d (p0);

  return 0;
}

int
neut_tesl_poly_volume (struct TESL Tess, int poly, double* pvol)
{
  int f, face;
  double area, h;
  double* proj = ut_alloc_1d (3);

  // Principle: sum the volumes of the pyramids based on the faces and
  // the poly centre.

  if (poly < 1 || poly > Tess.PolyQty)
  {
    (*pvol) = 0;
    return -1;
  }

  (*pvol) = 0;
  for (f = 1; f <= Tess.PolyFaceQty[poly]; f++)
  {
    face = Tess.PolyFaceNb[poly][f];
    
    neut_tesl_face_area (Tess, face, &area);

    ut_array_1d_memcpy (proj, 3, Tess.CenterCoo[poly]);
    ut_space_projpoint_alongonto (proj, Tess.FaceEq[face] + 1, Tess.FaceEq[face]);
    h = ut_space_dist (Tess.CenterCoo[poly], proj);
    (*pvol) += area * h * 0.3333333333333333333333;
  }

  ut_free_1d (proj);

  return 0;
}

void
neut_tesl_volume (struct TESL Tess, double* pvol)
{
  int i;
  double poly_vol;

  (*pvol) = 0;
  for (i = 1; i <= Tess.PolyQty; i++)
  {
    neut_tesl_poly_volume (Tess, i, &poly_vol);
    (*pvol) += poly_vol;
  }

  return;
}

int
neut_tesl_poly_body (struct TESL Tess, int nb)
{
  int i, face, res;

  res = 1;
  for (i = 1; i <= Tess.PolyFaceQty[nb]; i++)
  {
    face = Tess.PolyFaceNb[nb][i];

    if (Tess.FacePoly[face][0] < 0 || Tess.FacePoly[face][1] < 0)
    {
      res = 0;
      break;
    }
  }

  return res;
}

void
neut_tesl_init_polybody (struct TESL* pTess)
{
  int i, j, all, face, p;
  int body, status;
  
  if ((*pTess).PolyBody == NULL)
    (*pTess).PolyBody = ut_alloc_1d_int ((*pTess).PolyQty + 1);

  for (i = 1; i <= (*pTess).PolyQty; i++)
    (*pTess).PolyBody[i] = neut_tesl_poly_body ((*pTess), i);

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
neut_tesl_poly_switch (struct TESL* pTess, int p1, int p2)
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

  // PolyEdge* (keep EdgeQty at the end)
  ut_array_2d_int_switchlines_length ((*pTess).PolyEdgeNb,
			       p1, (*pTess).PolyEdgeQty[p1] + 1,
			       p2, (*pTess).PolyEdgeQty[p2] + 1);
  
  ut_array_1d_int_switch ((*pTess).PolyEdgeQty, p1, p2);
  
  // PolyVer* (keep VerQty at the end)
  ut_array_2d_int_switchlines_length ((*pTess).PolyVerNb,
			       p1, (*pTess).PolyVerQty[p1] + 1,
			       p2, (*pTess).PolyVerQty[p2] + 1);
  
  ut_array_1d_int_switch ((*pTess).PolyVerQty, p1, p2);
  
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

  // VerGerm | the following is to avoid problems when p1 and p2 share
  // a common ver
  for (i = 1; i <= (*pTess).PolyVerQty[p1]; i++)
  {
    id = (*pTess).PolyVerNb[p1][i];
    ut_array_1d_int_findnreplace ((*pTess).VerGerm[p1], 4, p2, -1000);
  }
  for (i = 1; i <= (*pTess).PolyVerQty[p2]; i++)
  {
    id = (*pTess).PolyVerNb[p1][i];
    ut_array_1d_int_findnreplace ((*pTess).VerGerm[p2], 2, p1, p2);
  }
  for (i = 1; i <= (*pTess).PolyVerQty[p1]; i++)
  {
    id = (*pTess).PolyVerNb[p1][i];
    ut_array_1d_int_findnreplace ((*pTess).VerGerm[p1], 4, -1000, p1);
  }

  return;
}
