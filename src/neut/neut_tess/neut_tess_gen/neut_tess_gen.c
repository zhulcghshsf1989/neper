/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_gen_lcl.h"

void
neut_tess_ver_faces (struct TESS Tess, int ver, int** pface, int* pfaceqty)
{
  int i, j, tmpedge;

  (*pfaceqty) = 0;
  for (i = 0; i < Tess.VerEdgeQty[ver]; i++)
  {
    tmpedge = Tess.VerEdgeNb[ver][i];
    (*pface) = ut_realloc_1d_int (*pface, *pfaceqty + Tess.EdgeFaceQty[tmpedge]);
    for (j = 0; j < Tess.EdgeFaceQty[tmpedge]; j++)
      (*pface)[(*pfaceqty)++] = Tess.EdgeFaceNb[tmpedge][j];
  }

  ut_array_1d_int_sort_uniq (*pface, *pfaceqty, pfaceqty);

  return;
}

void
neut_tess_ver_polys (struct TESS Tess, int ver, int** ppoly, int* ppolyqty)
{
  int i, j, faceqty;
  int* face = NULL;

  neut_tess_ver_faces (Tess, ver, &face, &faceqty);

  (*ppolyqty) = 0;
  for (i = 0; i < faceqty; i++)
  {
    (*ppoly) = ut_realloc_1d_int (*ppoly, (*ppolyqty) + 2);
    for (j = 0; j < 2; j++)
      (*ppoly)[(*ppolyqty)++] = Tess.FacePoly[face[i]][j];
  }

  ut_array_1d_int_sort_uniq (*ppoly, *ppolyqty, ppolyqty);

  ut_free_1d_int (face);

  return;
}

void
neut_tess_ver_domfaces (struct TESS Tess, int ver, int** pdface, int* pdfaceqty)
{
  if (Tess.VerDom[ver][0] == -1)
    (*pdfaceqty) = 0;
  else if (Tess.VerDom[ver][0] == 0) // is a dom ver
    neut_tess_domver_domface (Tess, Tess.VerDom[ver][1], pdface, pdfaceqty);
  else if (Tess.VerDom[ver][0] == 1) // is on a dom edge
  {
    (*pdfaceqty) = 2;
    (*pdface) = ut_alloc_1d_int (2);
    ut_array_1d_int_memcpy (*pdface, 2, Tess.DomEdgeFaceNb[Tess.VerDom[ver][1]]);
  }
  else if (Tess.VerDom[ver][0] == 2) // is a dom face
  {
    (*pdfaceqty) = 1;
    (*pdface) = ut_alloc_1d_int (1);
    (*pdface)[0] = Tess.VerDom[ver][1];
  }
  else
    ut_error_reportbug ();

  return;
}


void
neut_tess_edge_polys (struct TESS Tess, int edge, int** ppoly, int* ppolyqty)
{
  int i, j, face, p; 

  // the list of poly of an edge is the union of the poly of its faces. 

  (*ppolyqty) = 0;

  for (i = 0; i < Tess.EdgeFaceQty[edge]; i++)
  {
    face = Tess.EdgeFaceNb[edge][i];

    for (j = 0; j < 2; j++)
    {
      p = Tess.FacePoly[face][j];
      
      if (p > 0 && ut_array_1d_int_eltpos (*ppoly, *ppolyqty, p) == -1)
      {
	(*ppoly) = ut_realloc_1d_int (*ppoly, (*ppolyqty) + 1);
	(*ppoly)[(*ppolyqty)++] = p;
      }
    }
  }

  ut_array_1d_int_sort (*ppoly, *ppolyqty);
  
  return;
}

void
neut_tess_edges_vers (struct TESS Tess, int* edge, int edgeqty, int** pver, int* pverqty)
{
  int i, id;

  (*pverqty) = 0;
  (*pver) = ut_alloc_1d_int (edgeqty * 2);
  for (i = 0; i < edgeqty; i++)
  {
    id = edge[i];
    ut_array_1d_int_memcpy ((*pver) + (*pverqty), (*pverqty), Tess.EdgeVerNb[id] + 1);
    (*pverqty) += 2;
  }

  ut_array_1d_int_sort_uniq (*pver, *pverqty, pverqty);

  return;
}

int
neut_tess_faces_commonedge (struct TESS Tess, int* face, int faceqty, int* pedge)
{
  int i, status, qty;
  int* nb = NULL;

  if (faceqty <= 0)
  {
    (*pedge) = -1;
    return -1;
  }

  nb = ut_alloc_1d_int (Tess.FaceVerQty[face[0]]);
  qty = Tess.FaceVerQty[face[0]];
  ut_array_1d_int_memcpy (nb, Tess.FaceVerQty[face[0]], Tess.FaceEdgeNb[face[0]] + 1);

  for (i = 1; i < faceqty && qty > 0; i++)
    ut_array_1d_int_inter (nb, qty, Tess.FaceEdgeNb[face[i]] + 1,
		           Tess.FaceVerQty[face[i]], nb, &qty);

  if (qty == 1)
  {
    (*pedge) = nb[0];
    status = 0;
  }
  else
  {
    (*pedge) = -1;
    status = -1;
  }

  ut_free_1d_int (nb);

  return status;
}


void
neut_tess_poly_vers (struct TESS Tess, int poly, int** pver, int* pverqty)
{
  int i, j, face;

  (*pverqty) = 0;

  for (i = 1; i <= Tess.PolyFaceQty[poly]; i++)
  {
    face = Tess.PolyFaceNb[poly][i];

    for (j = 1; j <= Tess.FaceVerQty[face]; j++)
      if (ut_array_1d_int_eltpos ((*pver), (*pverqty), Tess.FaceVerNb[face][j]) == -1)
      {
	(*pver) = ut_realloc_1d_int (*pver, (*pverqty) + 1);
	(*pver)[(*pverqty)++] = Tess.FaceVerNb[face][j];
      }
  }

  ut_array_1d_int_sort (*pver, *pverqty);

  return;
}

void
neut_tess_poly_edges (struct TESS Tess, int poly, int** pedge, int* pedgeqty)
{
  int i, j, face;

  (*pedgeqty) = 0;

  for (i = 1; i <= Tess.PolyFaceQty[poly]; i++)
  {
    face = Tess.PolyFaceNb[poly][i];
    for (j = 1; j <= Tess.FaceVerQty[face]; j++)
      if (ut_array_1d_int_eltpos (*pedge, *pedgeqty, Tess.FaceEdgeNb[face][j]) == -1)
      {
	(*pedge) = ut_realloc_1d_int ((*pedge), (*pedgeqty) + 1);
	(*pedge)[(*pedgeqty)++] = Tess.FaceEdgeNb[face][j];
      }
  }
  
  ut_array_1d_int_sort (*pedge, *pedgeqty);

  return;
}


void
neut_tess_poly_edge_faces (struct TESS Tess, int poly, int edge, int** pface)
{
  int i, f, qty = 0;

  (*pface) = ut_alloc_1d_int (2);

  for (i = 0; i < Tess.EdgeFaceQty[edge]; i++)
  {
    f = Tess.EdgeFaceNb[edge][i];
    if (ut_array_1d_int_eltpos (Tess.FacePoly[f], 2, poly) != -1)
      (*pface)[qty++] = f;
  }

  if (qty != 2)
    ut_error_reportbug ();

  return;
}

void
neut_tess_face_edge_ori (struct TESS Tess, int face, int edge, int* pedgeori)
{
  int pos;

  pos = 1 + ut_array_1d_int_eltpos (Tess.FaceEdgeNb[face] + 1,
				    Tess.FaceVerQty[face], edge);
  if (pos == 0)
    (*pedgeori) = -1;
  else
    (*pedgeori) = Tess.FaceEdgeOri[face][pos];

  return;
}

void
neut_tess_poly_face_ori (struct TESS Tess, int poly, int face, int* pfaceori)
{
  int pos;

  pos = 1 + ut_array_1d_int_eltpos (Tess.PolyFaceNb[poly] + 1,
				    Tess.PolyFaceQty[poly], face);
  if (pos == 0)
    (*pfaceori) = 0;
  else
    (*pfaceori) = Tess.PolyFaceOri[poly][pos];

  return;
}

int
neut_tess_poly_true (struct TESS Tess, int nb)
{
  int i, j, res, verid;
  double ctov, dist;
  double *coo = ut_alloc_1d (3);
  int verqty;
  int* ver = NULL;
  double* proj = ut_alloc_1d (3);

  if (Tess.DomFaceQty == 0)
    ut_error_reportbug ();

  neut_tess_poly_vers (Tess, nb, &ver, &verqty);

  res = 1;
  for (i = 0; i < verqty; i++)
  {
    verid = ver[i];
    ctov = ut_space_dist (Tess.CenterCoo[nb], Tess.VerCoo[verid]);

    for (j = 1; j <= Tess.DomFaceQty; j++)
    {
      ut_array_1d_memcpy (proj, 3, Tess.VerCoo[verid]);
      ut_space_projpoint_alongonto (proj, Tess.DomFaceEq[j] + 1, Tess.DomFaceEq[j]);
      dist = ut_space_dist (Tess.VerCoo[verid], proj);

      if (dist < 1.001 * ctov)
      {
	res = 0;
	break;
      }
    }
  }

  ut_free_1d (coo);
  ut_free_1d_int (ver);
  ut_free_1d (proj);

  return res;
}

int
neut_tess_poly_body (struct TESS Tess, int nb)
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

/* returns 0 if not true, 1 if true & 2 if all parent poly are true */
int
neut_tess_edge_true (struct TESS Tess, int *TruePoly, int nb)
{
  int j;
  int res, all;
  int polyqty;
  int* poly = NULL;

  neut_tess_edge_polys (Tess, nb, &poly, &polyqty);

  res = 0;
  all = 1;
  for (j = 0; j < polyqty; j++)
  {
    if (poly[j] > 0)
    {
      if (TruePoly[poly[j]] > 0)
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

/* returns 0 if not body, 1 if body & 2 if all parent poly are body */
int
neut_tess_edge_body (struct TESS Tess, int *BodyPoly, int nb)
{
  int j;
  int res, all;
  int polyqty;
  int* poly = NULL;

  neut_tess_edge_polys (Tess, nb, &poly, &polyqty);

  res = 0;
  all = 1;
  for (j = 0; j < polyqty; j++)
  {
    if (poly[j] > 0)
    {
      if (BodyPoly[poly[j]] > 0)
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


/* returns 0 if not true, 1 if true & 2 if all parent poly are true */
int
neut_tess_face_true (struct TESS Tess, int *TruePoly, int nb)
{
  int j;
  int res, all;

  res = 0;
  all = 1;
  for (j = 0; j < 2; j++)
  {
    if (Tess.FacePoly[nb][j] > 0)
    {
      if (TruePoly[Tess.FacePoly[nb][j]] > 0)
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

/* returns 0 if not true, 1 if true & 2 if all parent poly are true */
int
neut_tess_ver_true (struct TESS Tess, int *TruePoly, int nb)
{
  int j;
  int res, all;
  int polyqty;
  int* poly = NULL;

  neut_tess_ver_polys (Tess, nb, &poly, &polyqty);

  res = 0;
  all = 1;
  for (j = 0; j < polyqty; j++)
  {
    if (poly[j] > 0)
    {
      if (TruePoly[poly[j]] > 0)
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

int
neut_tess_edge_polytruelevelmax (struct TESS Tess, int *TruePoly, int nb)
{
  int j, res;
  int polyqty;
  int* poly = NULL;

  neut_tess_edge_polys (Tess, nb, &poly, &polyqty);

  res = 0;
  for (j = 0; j < polyqty; j++)
    if (poly[j] > 0)
      res = ut_num_max_int (res, TruePoly[poly[j]]);

  ut_free_1d_int (poly);

  return res;
}

int
neut_tess_edge_polybodylevelmax (struct TESS Tess, int *BodyPoly, int nb)
{
  int j, res;
  int polyqty;
  int* poly = NULL;

  neut_tess_edge_polys (Tess, nb, &poly, &polyqty);

  res = 0;
  for (j = 0; j < polyqty; j++)
    if (poly[j] > 0)
      res = ut_num_max_int (res, BodyPoly[poly[j]]);

  ut_free_1d_int (poly);

  return res;
}

int
neut_tess_face_polybodylevelmax (struct TESS Tess, int nb)
{
  int res = 0;

  if (Tess.FacePoly[nb][0] > 0)
    res = Tess.PolyBody[Tess.FacePoly[nb][0]];
  if (Tess.FacePoly[nb][1] > 0)
    res = ut_num_max_int (res, Tess.PolyBody[Tess.FacePoly[nb][1]]);

  return res;
}

int
neut_tess_face_polytruelevelmax (struct TESS Tess, int nb)
{
  int res = 0;

  if (Tess.FacePoly[nb][0] > 0)
    res = Tess.PolyTrue[Tess.FacePoly[nb][0]];
  if (Tess.FacePoly[nb][1] > 0)
    res = ut_num_max_int (res, Tess.PolyTrue[Tess.FacePoly[nb][1]]);

  return res;
}

int
neut_tess_ver_polytruelevelmax (struct TESS Tess, int *TruePoly, int nb)
{
  int j, res;
  int polyqty;
  int* poly = NULL;

  neut_tess_ver_polys (Tess, nb, &poly, &polyqty);

  res = 0;
  for (j = 0; j < polyqty; j++)
    if (poly[j] > 0)
      res = ut_num_max_int (res, TruePoly[poly[j]]);

  ut_free_1d_int (poly);

  return res;
}

int
neut_tess_ver_polybodylevelmax (struct TESS Tess, int *BodyPoly, int nb)
{
  int j, res;
  int polyqty;
  int* poly = NULL;

  neut_tess_ver_polys (Tess, nb, &poly, &polyqty);

  res = 0;
  for (j = 0; j < polyqty; j++)
    if (poly[j] > 0)
      res = ut_num_max_int (res, BodyPoly[poly[j]]);

  ut_free_1d_int (poly);

  return res;
}

void
neut_tess_domface_edges (struct TESS Tess, int dface, int **pedge, int* pedgeqty)
{
  neut_tess_faces_edges (Tess, Tess.DomTessFaceNb[dface] + 1,
                        Tess.DomTessFaceQty[dface],
                        pedge, pedgeqty);

  return;
}

int
neut_tess_edge_selratio (struct TESS Tess, struct TESSPARA TessPara, int edge,
                  double* pratio)
{
  double sel;
  
  if (! TessPara.dbound)
    sel = TessPara.sel;
  else 
  {
    int var_qty = 2;
    char** vars  = ut_alloc_2d_char (var_qty, 15);
    double* vals = ut_alloc_1d      (var_qty);
    int status = -1;
    double dbound;

    sprintf (vars[0], "body");
    sprintf (vars[1], "true");
    vals[0] = neut_tess_edge_polybodylevelmax (Tess, Tess.PolyBody, edge);
    vals[1] = neut_tess_edge_polytruelevelmax (Tess, Tess.PolyTrue, edge);

    status = ut_math_eval (TessPara.dbound, var_qty, vars, vals, &dbound);

    if (status == -1)
      abort ();

    if (dbound == 1)
      sel = TessPara.dboundsel;           // means on bound
    else
      sel = TessPara.sel;

    ut_free_2d_char (vars, var_qty);
    ut_free_1d (vals);
  }

  if (Tess.EdgeState[edge] == 0)
  {
    (*pratio) = Tess.EdgeLength[edge] / sel;
    return 0;
  }
  else
    return 1;
}

int
neut_tess_face_area (struct TESS Tess, int face, double* parea)
{
  int i, ver;
  double* p0 = NULL;
  double* p1 = NULL;
  double* p2 = NULL;

  p0 = ut_alloc_1d (3);

  if (face < 1 || face > Tess.FaceQty || Tess.FaceState[face] == -1)
  {
    (*parea) = 0;
    return -1;
  }

  if (Tess.FaceState[face] > 0)
  {
    if (Tess.FacePt[face] == 0)
      ut_array_1d_memcpy (p0, 3, Tess.FacePtCoo[face]);
    else if (Tess.FacePt[face] > 0)
    {
      ver = Tess.FaceVerNb[face][Tess.FacePt[face]];
      ut_array_1d_memcpy (p0, 3, Tess.VerCoo[ver]);
    }
    else 
      neut_tess_face_centre (Tess, face, p0);
  }
  else if (Tess.FaceState[face] == 0)
  {
    ver = Tess.FaceVerNb[face][1];
    ut_array_1d_memcpy (p0, 3, Tess.VerCoo[ver]);
  }

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
neut_tess_volume (struct TESS Tess, double* pvol)
{
  int i;
  double tmp;

  (*pvol) = 0;
  for (i = 1; i <= Tess.PolyQty; i++)
  {
    neut_tess_poly_volume (Tess, i, &tmp);
    (*pvol) += tmp;
  }

  return 0;
}

int
neut_tess_poly_volume (struct TESS Tess, int poly, double* pvol)
{
  int i, ver, f, face;
  double area, h;
  double* p0 = ut_alloc_1d (3);
  double* p1 = NULL;
  double* p2 = NULL;
  double* proj = ut_alloc_1d (3);
  double* eq = ut_alloc_1d (4);

  // Principle: sum the volumes of the pyramids based on the faces and
  // the poly centre. For the non-plane faces, we consider the
  // individual triangular parts (no approximation)

  if (poly < 1 || poly > Tess.PolyQty)
  {
    (*pvol) = 0;
    return -1;
  }

  (*pvol) = 0;
  for (f = 1; f <= Tess.PolyFaceQty[poly]; f++)
  {
    face = Tess.PolyFaceNb[poly][f];

    if (Tess.FaceState[face] > 0)
    {
      if (Tess.FacePt[face] == 0)
	ut_array_1d_memcpy (p0, 3, Tess.FacePtCoo[face]);
      else if (Tess.FacePt[face] == -1)
      {
	ver = Tess.FaceVerNb[face][1];
	neut_tess_face_centre (Tess, face, p0);
      }
      else
      {
	ver = Tess.FaceVerNb[face][Tess.FacePt[face]];
	ut_array_1d_memcpy (p0, 3, Tess.VerCoo[ver]);
      }
    }
    else
    {
      ver = Tess.FaceVerNb[face][1];
      ut_array_1d_memcpy (p0, 3, Tess.VerCoo[ver]);
    }

    for (i = 1; i < Tess.FaceVerQty[face]; i++)
    {
      p1 = Tess.VerCoo[Tess.FaceVerNb[face][i]];
      p2 = Tess.VerCoo[Tess.FaceVerNb[face][i + 1]];
      area = ut_space_triangle_area (p0, p1, p2);
      if (area > 1e-20)
      {
	ut_array_1d_memcpy (proj, 3, Tess.CenterCoo[poly]);
	ut_space_points_plane (p0, p1, p2, eq);
	ut_space_projpoint_alongonto (proj, eq + 1, eq);
	h = ut_space_dist (Tess.CenterCoo[poly], proj);
	(*pvol) += area * h * 0.3333333333333333333333;
      }
    }

    p1 = Tess.VerCoo[Tess.FaceVerNb[face][Tess.FaceVerQty[face]]];
    p2 = Tess.VerCoo[Tess.FaceVerNb[face][1]];
    area = ut_space_triangle_area (p0, p1, p2);
    if (area > 1e-20)
    {
      ut_array_1d_memcpy (proj, 3, Tess.CenterCoo[poly]);
      ut_space_points_plane (p0, p1, p2, eq);
      ut_space_projpoint_alongonto (proj, eq + 1, eq);
      h = ut_space_dist (Tess.CenterCoo[poly], proj);
      (*pvol) += area * h * 0.3333333333333333333333;
    }
  }

  // don't free p1, p2 (shortcuts)
  ut_free_1d (proj);
  ut_free_1d (eq);
  ut_free_1d (p0);

  return 0;
}

int
neut_tess_poly_centroid (struct TESS Tess, int poly, double* coo)
{
  int i, ver, f, face;
  double area;
  double* p0 = ut_alloc_1d (3);
  double* p1 = NULL;
  double* p2 = NULL;
  double vol, totvol;
  double* tmp = ut_alloc_1d (3);

  // Principle: sum the volumes of the pyramids based on the faces and
  // the poly centre. For the non-plane faces, we consider the
  // individual triangular parts (no approximation)

  if (poly < 1 || poly > Tess.PolyQty)
    return -1;

  totvol = 0;
  ut_array_1d_set (coo, 3, 0);

  for (f = 1; f <= Tess.PolyFaceQty[poly]; f++)
  {
    face = Tess.PolyFaceNb[poly][f];

    if (Tess.FaceState[face] > 0)
    {
      if (Tess.FacePt[face] == 0)
	ut_array_1d_memcpy (p0, 3, Tess.FacePtCoo[face]);
      else if (Tess.FacePt[face] == -1)
      {
	ver = Tess.FaceVerNb[face][1];
	neut_tess_face_centre (Tess, face, p0);
      }
      else
      {
	ver = Tess.FaceVerNb[face][Tess.FacePt[face]];
	ut_array_1d_memcpy (p0, 3, Tess.VerCoo[ver]);
      }
    }
    else
    {
      ver = Tess.FaceVerNb[face][1];
      ut_array_1d_memcpy (p0, 3, Tess.VerCoo[ver]);
    }

    for (i = 1; i <= Tess.FaceVerQty[face]; i++)
    {
      p1 = Tess.VerCoo[Tess.FaceVerNb[face][i]];
      p2 = Tess.VerCoo[Tess.FaceVerNb[face]
	             [ut_num_rotpos (1, Tess.FaceVerQty[face], i, 1)]];

      area = ut_space_triangle_area (p0, p1, p2);

      if (area > 1e-20)
      {
	ut_space_tet_centre (Tess.CenterCoo[poly], p0, p1, p2, tmp);
	vol = ut_space_tet_volume (Tess.CenterCoo[poly], p0, p1, p2);
	ut_array_1d_scale (tmp, 3, vol);
	ut_array_1d_add (coo, tmp, 3, coo);
	totvol += vol;
      }
    }
  }

  ut_array_1d_scale (coo, 3, 1./totvol);

  // don't free p1, p2 (shortcuts)
  ut_free_1d (p0);
  ut_free_1d (tmp);

  return 0;
}

void
neut_tess_polys_skinfaces (struct TESS Tess, int* poly, int polyqty,
                          int** pface, int** pfacepoly, int* pfaceqty)
{
  int i, j, k, pos, p, f;

  // allocation
  (*pfaceqty) = 0;
  for (i = 0; i < polyqty; i++)
  {
    p = poly[i];
    (*pfaceqty) += Tess.PolyFaceQty[p];
  }
  (*pface) = ut_alloc_1d_int ((*pfaceqty));
  (*pfacepoly) = ut_alloc_1d_int ((*pfaceqty));

  (*pfaceqty) = 0;
  for (i = 0; i < polyqty; i++)
  {
    p = poly[i];
    for (j = 1; j <= Tess.PolyFaceQty[p]; j++)
    {
      f = Tess.PolyFaceNb[p][j];
      pos = ut_array_1d_int_eltpos (*pface, *pfaceqty, f);
      if (pos == -1)
      {
	(*pface)[(*pfaceqty)] = f;
	(*pfacepoly)[(*pfaceqty)] = p;
	(*pfaceqty)++;
      }
      else
      {
	for (k = pos; k < (*pfaceqty) - 1; k++)
	{
	  (*pface)[k] = (*pface)[k + 1];
	  (*pfacepoly)[k] = (*pfacepoly)[k + 1];
	}
	(*pfaceqty)--;
      }
    }
  }
  (*pface) = ut_realloc_1d_int ((*pface), (*pfaceqty));

  return;
}

void
neut_tess_bbox (struct TESS Tess, double** size)
{
  int i, j;

  /* Searching the tessellation bounding box */
  size[0][0] = size[1][0] = size[2][0] = DBL_MAX;
  size[0][1] = size[1][1] = size[2][1] = -DBL_MAX;

  for (i = 1; i <= Tess.VerQty; i++)
    for (j = 0; j < 3; j++)
    {
      size[j][0] = ut_num_min (size[j][0], Tess.VerCoo[i][j]);
      size[j][1] = ut_num_max (size[j][1], Tess.VerCoo[i][j]);
    }

  return;
}

void
neut_tess_face_centre (struct TESS Tess, int face, double* centre)
{
  int i, ver;

  ut_array_1d_set (centre, 3, 0);
  for (i = 1; i <= Tess.FaceVerQty[face]; i++)
  {
    ver = Tess.FaceVerNb[face][i];
    ut_array_1d_add (centre, Tess.VerCoo[ver], 3, centre);
  }
  ut_array_1d_scale (centre, 3, 1./Tess.FaceVerQty[face]);

  return;
}

int
neut_tess_edge_centre (struct TESS Tess, int edge, double* pos)
{
  int i;

  for (i = 0; i < 3; i++)
    pos[i] = (Tess.VerCoo[Tess.EdgeVerNb[edge][0]][i]
	    + Tess.VerCoo[Tess.EdgeVerNb[edge][1]][i]) / 2.;
  
  return 0;
}

void
neut_tess_centre (struct TESS Tess, double* centre)
{
  int i;
  double totvol, vol;
  double* coo = ut_alloc_1d (3);

  ut_array_1d_zero (centre, 3);
  totvol = 0;
  for (i = 1; i <= Tess.PolyQty; i++)
  {
    neut_tess_poly_centroid (Tess, i, coo);
    neut_tess_poly_volume   (Tess, i, &vol);
    ut_array_1d_scale (coo, 3, vol);
    ut_array_1d_add (centre, coo, 3, centre);
    totvol += vol;
  }
  ut_array_1d_scale (centre, 3, 1. / totvol);

  ut_free_1d (coo);

  return;
}

void
neut_tess_poly_neighpoly (struct TESS Tess, int poly, int** pnpoly, int* pnpolyqty)
{
  int i, j, face;

  (*pnpolyqty) = 0;
  for (i = 1; i <= Tess.PolyFaceQty[poly]; i++)
  {
    face = Tess.PolyFaceNb[poly][i];

    for (j = 0; j < 2; j++)
      if (Tess.FacePoly[face][j] > 0 && Tess.FacePoly[face][j] != poly)
      {
	(*pnpoly) = ut_realloc_1d_int ((*pnpoly), (*pnpolyqty) + 1);
	(*pnpoly)[(*pnpolyqty)++] = Tess.FacePoly[face][j];
      }
  }

  return;
}
int
neut_tess_expr_polytab (struct TESS Tess, char* expr, int* poly)
{
  int i, id;
  FILE* file;

  if (strcmp (expr, "all") == 0)
    ut_array_1d_int_set (poly + 1, Tess.PolyQty, 1);

  else if (expr[0] == '@')
  {
    file = ut_file_open (expr + 1, "r");
    while (fscanf (file, "%d", &id) != EOF)
      poly[id] = 1;
    ut_file_close (file, expr + 1, "r");
  }
  else
  {
    int var_qty = 7;
    char **vars = ut_alloc_2d_char (var_qty, 15);
    double *vals = ut_alloc_1d (var_qty);
    int status;
    double res;

    sprintf (vars[0], "cenx");
    sprintf (vars[1], "ceny");
    sprintf (vars[2], "cenz");
    sprintf (vars[3], "volume");
    sprintf (vars[4], "true");
    sprintf (vars[5], "body");
    sprintf (vars[6], "id");

    for (i = 1; i <= Tess.PolyQty; i++)
    {
      ut_array_1d_memcpy (vals, 3, Tess.CenterCoo[i]);
      neut_tess_poly_volume (Tess, i, &(vals[3]));
      vals[4] = Tess.PolyTrue[i];
      vals[5] = Tess.PolyBody[i];
      vals[6] = i;
#ifdef HAVE_LIBMATHEVAL
      status = ut_math_eval (expr, var_qty, vars, vals, &res);
#else
ut_print_messagewnc (2, 72, "This capability is not available because this version of Neper has not been compiled with libmatheval.");
abort ();
#endif
      if (status == -1)
	abort ();
      poly[i] = ut_num_equal (res, 1, 1e-6);
    }

    ut_free_2d_char (vars, var_qty);
    ut_free_1d (vals);
  }

  return 0;
}

int
neut_tess_expr_facetab (struct TESS Tess, char* expr, int* face)
{
  int i, id;
  FILE* file;

  if (strcmp (expr, "all") == 0)
    ut_array_1d_int_set (face + 1, Tess.FaceQty, 1);

  else if (expr[0] == '@')
  {
    file = ut_file_open (expr + 1, "r");
    while (fscanf (file, "%d", &id) != EOF)
      face[id] = 1;
    ut_file_close (file, expr + 1, "r");
  }
  else
  {
    int var_qty = 7;
    char **vars = ut_alloc_2d_char (var_qty, 15);
    double *vals = ut_alloc_1d (var_qty);
    int status;
    double res;

    sprintf (vars[0], "cenx");
    sprintf (vars[1], "ceny");
    sprintf (vars[2], "cenz");
    sprintf (vars[3], "volume");
    sprintf (vars[4], "true");
    sprintf (vars[5], "body");
    sprintf (vars[6], "id");

    for (i = 1; i <= Tess.FaceQty; i++)
    {
      neut_tess_face_centre (Tess, i, vals);
      neut_tess_face_area (Tess, i, &(vals[3]));
      vals[4] = neut_tess_face_true (Tess, Tess.PolyTrue, i);
      vals[5] = 0;
      vals[6] = i;

#ifdef HAVE_LIBMATHEVAL
      status = ut_math_eval (expr, var_qty, vars, vals, &res);
#else
ut_print_messagewnc (2, 72, "This capability is not available because this version of Neper has not been compiled with libmatheval.");
abort ();
#endif
      if (status == -1)
	abort ();
      face[i] = ut_num_equal (res, 1, 1e-6);
    }

    ut_free_2d_char (vars, var_qty);
    ut_free_1d (vals);
  }

  return 0;
}

int
neut_tess_poly_skinmesh (struct TESS Tess, int nb,
		        struct NODES* pNodes, struct MESH* pMesh)
{
  int i, j, face;
  int* ver = NULL;
  int* verinv = NULL;
  int verinvqty;
  int node0;
  int verqty;
  int* eltnodes = ut_alloc_1d_int (3);
  double* centre = ut_alloc_1d (3);

  neut_mesh_set_zero  (pMesh );
  neut_nodes_set_zero (pNodes);
  neut_mesh_addelset (pMesh, NULL, 0);
  (*pMesh).EltOrder = 1;
  (*pMesh).Dimension = 2;
  (*pMesh).EltType = ut_alloc_1d_char (4);
  sprintf ((*pMesh).EltType, "tri");

  // adding poly vertices and calculating the inverse array
  neut_tess_poly_vers (Tess, nb, &ver, &verqty);
  ut_array_1d_int_inv (ver, verqty, &verinv, &verinvqty);
  ut_array_1d_int_addval (verinv, verinvqty, 1, verinv);

  for (i = 0; i < verqty; i++)
    neut_nodes_addnode (pNodes, Tess.VerCoo[ver[i]], 1);

  // for each face, recording a new node if necessary and recording the
  // elements
  for (i = 1; i <= Tess.PolyFaceQty[nb]; i++)
  {
    face = Tess.PolyFaceNb[nb][i];

    verqty = Tess.FaceVerQty[face];

    // adding node if defined as the barycenter
    // FacePt == -1 stuff added but not tested.
    if (Tess.FaceState[face] == 0)
    {
      neut_tess_face_centre (Tess, face, Tess.FacePtCoo[face]);
      neut_nodes_addnode (pNodes, Tess.FacePtCoo[face], 1);
    }
    else if (Tess.FacePt[face] == 0)
    {
      neut_nodes_addnode (pNodes, Tess.FacePtCoo[face], 1);
    }
    else if (Tess.FacePt[face] == -1)
    {
      neut_tess_face_centre (Tess, face, centre);
      neut_nodes_addnode (pNodes, centre, 1);
    }

    // looking for the master node (as recorded in Nodes)
    node0 = (Tess.FacePt[face] > 0) ?
            verinv[Tess.FaceVerNb[face][Tess.FacePt[face]]]
	  : (*pNodes).NodeQty;

    for (j = 1; j <= verqty; j++)
    {
      // if p0 is a face vertex: 
      // if the ver is p0 or the previous vertex, it is skipped
      if (j == Tess.FacePt[face]
	  || ut_num_rotpos (1, verqty, j, 1) == Tess.FacePt[face])
	continue;

      // recording coo of the 1st and 2nd pts
      eltnodes[0] = node0;
      eltnodes[1] = verinv[Tess.FaceVerNb[face][j]];
      eltnodes[2] = verinv[Tess.FaceVerNb[face][ut_num_rotpos (1, verqty, j, 1)]];

      neut_mesh_addelt (pMesh, eltnodes);
      neut_mesh_elset_addelt (pMesh, 1, (*pMesh).EltQty);

      /* Probably this kind of test would be useful here
      // if the 3 points are aligned (on an edge of the domain), skipping.
      // use an angle-criterion instead.
      // 01/10: sounds that this never happens...
      if (ut_space_pointaligned (p1, p2, p0, 1e-6))
	continue;
      */
    }
  }

  neut_mesh_init_eltelset (pMesh, NULL);

  ut_free_1d_int (ver);
  ut_free_1d_int (verinv);
  ut_free_1d_int (eltnodes);
  ut_free_1d (centre);

  return 0;
}

int
neut_tess_poly_mesh (struct TESS Tess, int nb,
		    struct NODES* pNodes, struct MESH* pMesh)
{
  int i;
  struct MESH Mesh2D;
  int* eltnodes = ut_alloc_1d_int (4);

  neut_tess_poly_skinmesh (Tess, nb, pNodes, &Mesh2D);
  neut_nodes_addnode (pNodes, Tess.CenterCoo[nb], 1);

  neut_mesh_set_zero (pMesh);
  neut_mesh_addelset (pMesh, NULL, 0);
  (*pMesh).EltOrder = 1;
  (*pMesh).Dimension = 3;
  (*pMesh).EltType = ut_alloc_1d_char (4);

  sprintf ((*pMesh).EltType, "tri");
  for (i = 1; i <= Mesh2D.EltQty; i++)
  {
    ut_array_1d_int_memcpy (eltnodes, 3, Mesh2D.EltNodes[i]);
    eltnodes[3] = (*pNodes).NodeQty;

    neut_mesh_addelt (pMesh, eltnodes);
    neut_mesh_elset_addelt (pMesh, 1, (*pMesh).EltQty);
  }

  neut_mesh_init_eltelset (pMesh, NULL);

  ut_free_1d_int (eltnodes);
  neut_mesh_free (&Mesh2D);

  return 0;
}

int
neut_tess_poly_reg (struct TESS Tess, int nb)
{
  int i;

  for (i = 1; i <= Tess.PolyFaceQty[nb]; i++)
    if (Tess.FaceState[Tess.PolyFaceNb[nb][i]] != 0)
      return 1;

  return 0;
}

int
neut_tess_poly_std (struct TESS Tess, int nb)
{
  return (neut_tess_poly_reg (Tess, nb) == 0) ? 1 : 0;
}

int
neut_tess_point_inpoly_std (struct TESS Tess, double* coo, int nb)
{
  int i, j, res;
  double* faceeq = ut_alloc_1d (4);

  res = 1;
  for (i = 1; i <= Tess.PolyFaceQty[nb]; i++)
  {
    for (j = 0; j < 4; j++)
      faceeq[j] =
	(double) Tess.PolyFaceOri[nb][i] *
	Tess.FaceEq[Tess.PolyFaceNb[nb][i]][j];

    if (ut_space_planeside_tol (faceeq, coo - 1, 1e-12) > 0)
    {
      res = 0;
      break;
    }
  }

  ut_free_1d (faceeq);

  return res;
}

int
neut_tess_point_inpoly_reg (struct TESS Tess, double* coo, int nb)
{
  int elt, status;
  struct MESH Mesh;
  struct NODES Nodes;

  neut_mesh_set_zero  (&Mesh);
  neut_nodes_set_zero (&Nodes);

  neut_tess_poly_mesh (Tess, nb, &Nodes, &Mesh);

  status = neut_mesh_elset_point_elt (Mesh, Nodes, 1, coo, &elt);

  neut_mesh_free  (&Mesh);
  neut_nodes_free (&Nodes);

  if (status == 0)
    return 1;
  else
    return 0;
}

int
neut_tess_point_inpoly (struct TESS Tess, double* coo, int nb)
{
  if (neut_tess_poly_std (Tess, nb) == 1)
    return neut_tess_point_inpoly_std (Tess, coo, nb);
  else
    return neut_tess_point_inpoly_reg (Tess, coo, nb);
}

// Returns the interpolation (triangles) of a (regularized or not) face
// under the form of a mesh.
int
neut_tess_face_interpolmesh (struct TESS Tess, int face, struct NODES* pN,
    struct MESH* pM)
{
  int i, tqty, ver, ver0, verqty;
  double *p0 = NULL, *p1 = NULL, *p2 = NULL;
  int* nodes = ut_alloc_1d_int (3);
  int* elts = NULL;

  p0 = ut_alloc_1d (3);

  neut_nodes_set_zero (pN);
  neut_mesh_set_zero (pM);
  (*pM).Dimension = 2;
  (*pM).EltOrder = 1;

  verqty = Tess.FaceVerQty[face];
  for (i = 1; i <= verqty; i++)
  {
    ver = Tess.FaceVerNb[face][i];
    neut_nodes_addnode (pN, Tess.VerCoo[ver], 1);
  }

  if (Tess.FacePt[face] == 0)
  {
    neut_nodes_addnode (pN, Tess.FacePtCoo[face], 1);
    ver0 = verqty + 1;
  }
  else if  (Tess.FacePt[face] > 0)
    ver0 = Tess.FacePt[face];
  else 
  {
    neut_tess_face_centre (Tess, face, p0);
    neut_nodes_addnode (pN, p0, 1);
    ver0 = verqty + 1;
  }

  ut_array_1d_memcpy (p0, 3, Tess.VerCoo[ver0]);

  tqty = 0;
  for (i = 1; i <= verqty; i++)
  {
    // if p0 is a face vertex: 
    // if the ver is p0 or the previous vertex, it is skipped
    if (i == Tess.FacePt[face]
	|| ut_num_rotpos (1, verqty, i, 1) == Tess.FacePt[face])
      continue;

    // recording coo of the 1st and 2nd pts
    nodes[0] = ver0;
    nodes[1] = i;
    nodes[2] = ut_num_rotpos (1, verqty, i, 1);
    p1 = Tess.VerCoo[nodes[1]];
    p2 = Tess.VerCoo[nodes[2]];

    neut_mesh_addelt (pM, nodes);

    // if the 3 points are aligned (on an edge of the domain), skipping.
    // use an angle-criterion instead.
    // 01/10: sounds that this never happens...
    if (ut_space_pointaligned (p1, p2, p0, 1e-6))
      continue;

    tqty++;
  }
  elts = ut_alloc_1d_int ((*pM).EltQty);
  for (i = 0; i < (*pM).EltQty; i++)
    elts[i] = i + 1;
  neut_mesh_addelset (pM, elts, (*pM).EltQty);
  
  ut_free_1d_int (elts);
  ut_free_1d_int (nodes);
  ut_free_1d (p0);

  return tqty;
}

int
neut_tess_domfaces_commondomver (struct TESS Tess, int* face, int faceqty, int* pver)
{
  int i, status, qty;
  int* nb = NULL;

  if (faceqty <= 0)
  {
    (*pver) = -1;
    return -1;
  }

  nb = ut_alloc_1d_int (Tess.DomFaceVerQty[face[0]]);
  qty = Tess.DomFaceVerQty[face[0]];
  ut_array_1d_int_memcpy (nb, Tess.DomFaceVerQty[face[0]], Tess.DomFaceVerNb[face[0]] + 1);

  for (i = 1; i < faceqty && qty > 0; i++)
    ut_array_1d_int_inter (nb, qty, Tess.DomFaceVerNb[face[i]] + 1,
		           Tess.DomFaceVerQty[face[i]], nb, &qty);

  if (qty == 1)
  {
    (*pver) = nb[0];
    status = 0;
  }
  else
  {
    (*pver) = -1;
    status = -1;
  }

  ut_free_1d_int (nb);

  return status;
}

int
neut_tess_domfaces_commondomedge (struct TESS Tess, int* dface, int dfaceqty, int* pdedge)
{
  int i, status, qty;
  int* nb = NULL;

  if (dfaceqty <= 0)
  {
    (*pdedge) = -1;
    return -1;
  }

  nb = ut_alloc_1d_int (Tess.DomFaceVerQty[dface[0]]);
  qty = Tess.DomFaceVerQty[dface[0]];
  ut_array_1d_int_memcpy (nb, Tess.DomFaceVerQty[dface[0]], Tess.DomFaceEdgeNb[dface[0]] + 1);

  for (i = 1; i < dfaceqty && qty > 0; i++)
    ut_array_1d_int_inter (nb, qty, Tess.DomFaceEdgeNb[dface[i]] + 1,
		           Tess.DomFaceVerQty[dface[i]], nb, &qty);

  if (qty == 1)
  {
    (*pdedge) = nb[0];
    status = 0;
  }
  else
  {
    (*pdedge) = -1;
    status = -1;
  }

  ut_free_1d_int (nb);

  return status;
}


int
neut_tess_domvers_commondomedge (struct TESS Tess, int* dver, int dverqty, int* pdedge)
{
  int status, i, qty;
  int* id = NULL;
  
  // no vers: return
  if (dverqty <= 0)
  {
    (*pdedge) = -1;
    return -1;
  }

  qty = Tess.DomVerEdgeQty[dver[0]];

  id = ut_alloc_1d_int (qty);
  ut_array_1d_int_memcpy (id, qty, Tess.DomVerEdgeNb[dver[0]]);

  for (i = 1; i < dverqty; i++)
    ut_array_1d_int_inter (id, qty,
	                   Tess.DomVerEdgeNb[dver[i]], Tess.DomVerEdgeQty[dver[i]], 
	                   id, &qty);

  if (qty == 1)
  {
    (*pdedge) = id[0];
    status = 0;
  }
  else
  {
    (*pdedge) = -1;
    status = -1;
  }

  ut_free_1d_int (id);

  return status;
}

int
neut_tess_domedges_commondomver (struct TESS Tess, int* dedge, int dedgeqty, int* pdver)
{
  int status, i, qty;
  int* id = NULL;
  
  // no edges: return
  if (dedgeqty <= 0)
  {
    (*pdver) = -1;
    return -1;
  }

  qty = 2;

  id = ut_alloc_1d_int (qty);
  ut_array_1d_int_memcpy (id, qty, Tess.DomEdgeVerNb[dedge[0]]);

  for (i = 1; i < dedgeqty; i++)
    ut_array_1d_int_inter (id, qty, Tess.DomEdgeVerNb[dedge[i]], 2, id, &qty);

  if (qty == 1)
  {
    (*pdver) = id[0];
    status = 0;
  }
  else
  {
    (*pdver) = -1;
    status = -1;
  }

  ut_free_1d_int (id);

  return status;
}

int
neut_tess_domedges_domvers (struct TESS Tess, int* dedge, int dedgeqty, int** pdver, int* pdverqty)
{
  int i, id;

  // no edges: return
  if (dedgeqty <= 0)
  {
    (*pdver) = NULL;
    (*pdverqty) = 0;
    return -1;
  }

  (*pdver) = ut_alloc_1d_int (dedgeqty * 2);
  (*pdverqty) = 0;
  for (i = 0; i < dedgeqty; i++)
  {
    id = dedge[i];
    ut_array_1d_int_memcpy (*pdver + *pdverqty, 2, Tess.DomEdgeVerNb[id]);
    (*pdverqty) += 2;
  }

  ut_array_1d_int_sort_uniq (*pdver, *pdverqty, pdverqty);

  return 0;
}

int
neut_tess_domedges_commondomface (struct TESS Tess, int* dedge, int dedgeqty, int* pdface)
{
  int status, i, qty;
  int* id = NULL;
  
  // no edges: return
  if (dedgeqty <= 0)
  {
    (*pdface) = -1;
    return -1;
  }

  qty = 2;

  id = ut_alloc_1d_int (qty);
  ut_array_1d_int_memcpy (id, qty, Tess.DomEdgeFaceNb[dedge[0]]);

  for (i = 1; i < dedgeqty; i++)
    ut_array_1d_int_inter (id, qty, Tess.DomEdgeFaceNb[dedge[i]], 2, id, &qty);

  if (qty == 1)
  {
    (*pdface) = id[0];
    status = 0;
  }
  else
  {
    (*pdface) = -1;
    status = -1;
  }

  ut_free_1d_int (id);

  return status;
}

void
neut_tess_domver_domface (struct TESS Tess, int dver, int** pdface, int* pdfaceqty)
{
  int i, j, tmpedge;

  (*pdfaceqty) = 0;
  for (i = 0; i < Tess.DomVerEdgeQty[dver]; i++)
  {
    tmpedge = Tess.DomVerEdgeNb[dver][i];
    (*pdface) = ut_realloc_1d_int ((*pdface), (*pdfaceqty) + 2);
    for (j = 0; j < 2; j++)
      (*pdface)[(*pdfaceqty)++] = Tess.DomEdgeFaceNb[tmpedge][j];
  }

  ut_array_1d_int_sort_uniq (*pdface, *pdfaceqty, pdfaceqty);

  return;
}

void
neut_tess_domedge_ver (struct TESS Tess, int dedge, int **pver, int* pverqty)
{
  int i, edge;

  (*pver) = ut_alloc_1d_int (Tess.DomTessEdgeQty[dedge] * 2);

  (*pverqty) = 0;
  for (i = 1; i <= Tess.DomTessEdgeQty[dedge]; i++)
  {
    edge = Tess.DomTessEdgeNb[dedge][i];

    ut_array_1d_int_memcpy ((*pver) + (*pverqty), 2, Tess.EdgeVerNb[edge]);

    (*pverqty) += 2;
  }

  ut_array_1d_int_sort_uniq ((*pver), (*pverqty), pverqty);

  return;
}

void
neut_tess_domface_ver (struct TESS Tess, int dface, int **pver, int* pverqty)
{
  int i, j, face;

  (*pverqty) = 0;
  for (i = 1; i <= Tess.DomTessFaceQty[dface]; i++)
  {
    face = Tess.DomTessFaceNb[dface][i];
    
    (*pver) = ut_realloc_1d_int ((*pver), (*pverqty) + Tess.FaceVerQty[face]);

    for (j = 1; j <= Tess.FaceVerQty[face]; j++)
      (*pver)[(*pverqty)++] = Tess.FaceVerNb[face][j];
  }

  ut_array_1d_int_sort_uniq (*pver, *pverqty, pverqty);

  return;
}

void
neut_tess_faces_edges (struct TESS Tess, int* face, int faceqty, int** pedge, int* pedgeqty)
{
  int i, id;

  if (ut_array_1d_int_max (face, faceqty) > Tess.FaceQty)
    ut_error_reportbug ();

  (*pedgeqty) = 0;
  ut_free_1d_int (*pedge);

  for (i = 0; i < faceqty; i++)
  {
    id = face[i];
    (*pedge) = ut_realloc_1d_int ((*pedge), (*pedgeqty) + Tess.FaceVerQty[id]);
    ut_array_1d_int_memcpy ((*pedge) + (*pedgeqty), Tess.FaceVerQty[id], Tess.FaceEdgeNb[id] + 1);

    (*pedgeqty) += Tess.FaceVerQty[id];
  }

  ut_array_1d_int_sort_uniq (*pedge, *pedgeqty, pedgeqty);

  return;
}

int
neut_tess_edge_fake (struct TESS Tess, int edge)
{
  int domedge, face, domface1, domface2, poly1, poly2, j, status;

  status = 0;

  if (! strcmp (Tess.DomType, "cylinder"))
  {
    if (Tess.EdgeDom[edge][0] == 1)
    {
      domedge = Tess.EdgeDom[edge][1];
      domface1 = Tess.DomEdgeFaceNb[domedge][0];
      domface2 = Tess.DomEdgeFaceNb[domedge][1];

      poly1 = -1;
      poly2 = -1;
      for (j = 0; j < Tess.EdgeFaceQty[edge]; j++)
      {
	face = Tess.EdgeFaceNb[edge][j];
	if (Tess.FaceDom[face][0] == 2)
	{
	  if (Tess.FaceDom[face][1] == domface1)
	    poly1 = Tess.FacePoly[face][0];
	  if (Tess.FaceDom[face][1] == domface2)
	    poly2 = Tess.FacePoly[face][0];
	}
      }

      if (poly1 < 0 || poly2 < 0)
	ut_error_reportbug ();

      if (domface1 > 2 && domface2 > 2 && poly1 == poly2)
	status = 1;
    }
  }

  return status;
}

/* rsel2sel sets sel from rsel */
void
rsel2sel (double rsel, double vol, int PolyQty, double* psel)
{
  (*psel) = 0.5 * rsel * 0.5 / pow (PolyQty / vol, 0.3333333333333333333333333333);
  
  return;
}

/* rcl2cl sets cl from rcl */
void
rcl2cl (double rcl, double vol, int PolyQty, char* elttype, double* pcl)
{
  if (elttype == NULL || ! strcmp (elttype, "tet") || ! strcmp (elttype, "tri"))
    (*pcl) = rcl * 0.5 * pow (vol / PolyQty, 0.3333333333333333333333333333);
  else if (! strcmp (elttype, "hex") || ! strcmp (elttype, "quad"))
  {
    (*pcl) = rcl * 0.5 * pow (vol / PolyQty, 0.3333333333333333333333333333);
    (*pcl) *= .43084877208099956915;
  }
  
  return;
}
