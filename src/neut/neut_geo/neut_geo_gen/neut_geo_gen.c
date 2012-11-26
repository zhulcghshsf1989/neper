/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_geo_gen_lcl.h"

void
neut_geo_ver_faces (struct GEO Geo, int ver, int** pface, int* pfaceqty)
{
  int i, j, tmpedge;

  (*pfaceqty) = 0;
  for (i = 0; i < Geo.VerEdgeQty[ver]; i++)
  {
    tmpedge = Geo.VerEdgeNb[ver][i];
    (*pface) = ut_realloc_1d_int (*pface, *pfaceqty + Geo.EdgeFaceQty[tmpedge]);
    for (j = 0; j < Geo.EdgeFaceQty[tmpedge]; j++)
      (*pface)[(*pfaceqty)++] = Geo.EdgeFaceNb[tmpedge][j];
  }

  ut_array_1d_int_sort_uniq (*pface, *pfaceqty, pfaceqty);

  return;
}

void
neut_geo_ver_polys (struct GEO Geo, int ver, int** ppoly, int* ppolyqty)
{
  int i, j, faceqty;
  int* face = NULL;

  neut_geo_ver_faces (Geo, ver, &face, &faceqty);

  (*ppolyqty) = 0;
  for (i = 0; i < faceqty; i++)
  {
    (*ppoly) = ut_realloc_1d_int (*ppoly, (*ppolyqty) + 2);
    for (j = 0; j < 2; j++)
      (*ppoly)[(*ppolyqty)++] = Geo.FacePoly[face[i]][j];
  }

  ut_array_1d_int_sort_uniq (*ppoly, *ppolyqty, ppolyqty);

  ut_free_1d_int (face);

  return;
}

void
neut_geo_ver_domfaces (struct GEO Geo, int ver, int** pdface, int* pdfaceqty)
{
  if (Geo.VerDom[ver][0] == -1)
    (*pdfaceqty) = 0;
  else if (Geo.VerDom[ver][0] == 0) // is a dom ver
    neut_geo_domver_domface (Geo, Geo.VerDom[ver][1], pdface, pdfaceqty);
  else if (Geo.VerDom[ver][0] == 1) // is on a dom edge
  {
    (*pdfaceqty) = 2;
    (*pdface) = ut_alloc_1d_int (2);
    ut_array_1d_int_memcpy (*pdface, 2, Geo.DomEdgeFaceNb[Geo.VerDom[ver][1]]);
  }
  else if (Geo.VerDom[ver][0] == 2) // is a dom face
  {
    (*pdfaceqty) = 1;
    (*pdface) = ut_alloc_1d_int (1);
    (*pdface)[0] = Geo.VerDom[ver][1];
  }
  else
    ut_error_reportbug ();

  return;
}


void
neut_geo_edge_polys (struct GEO Geo, int edge, int** ppoly, int* ppolyqty)
{
  int i, j, face, p; 

  // the list of poly of an edge is the union of the poly of its faces. 

  (*ppolyqty) = 0;

  for (i = 0; i < Geo.EdgeFaceQty[edge]; i++)
  {
    face = Geo.EdgeFaceNb[edge][i];

    for (j = 0; j < 2; j++)
    {
      p = Geo.FacePoly[face][j];
      
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
neut_geo_edges_vers (struct GEO Geo, int* edge, int edgeqty, int** pver, int* pverqty)
{
  int i, id;

  (*pverqty) = 0;
  (*pver) = ut_alloc_1d_int (edgeqty * 2);
  for (i = 0; i < edgeqty; i++)
  {
    id = edge[i];
    ut_array_1d_int_memcpy ((*pver) + (*pverqty), (*pverqty), Geo.EdgeVerNb[id] + 1);
    (*pverqty) += 2;
  }

  ut_array_1d_int_sort_uniq (*pver, *pverqty, pverqty);

  return;
}

int
neut_geo_faces_commonedge (struct GEO Geo, int* face, int faceqty, int* pedge)
{
  int i, status, qty;
  int* nb = NULL;

  if (faceqty <= 0)
  {
    (*pedge) = -1;
    return -1;
  }

  nb = ut_alloc_1d_int (Geo.FaceVerQty[face[0]]);
  qty = Geo.FaceVerQty[face[0]];
  ut_array_1d_int_memcpy (nb, Geo.FaceVerQty[face[0]], Geo.FaceEdgeNb[face[0]] + 1);

  for (i = 1; i < faceqty && qty > 0; i++)
    ut_array_1d_int_inter (nb, qty, Geo.FaceEdgeNb[face[i]] + 1,
		           Geo.FaceVerQty[face[i]], nb, &qty);

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
neut_geo_poly_vers (struct GEO Geo, int poly, int** pver, int* pverqty)
{
  int i, j, face;

  (*pverqty) = 0;

  for (i = 1; i <= Geo.PolyFaceQty[poly]; i++)
  {
    face = Geo.PolyFaceNb[poly][i];

    for (j = 1; j <= Geo.FaceVerQty[face]; j++)
      if (ut_array_1d_int_eltpos ((*pver), (*pverqty), Geo.FaceVerNb[face][j]) == -1)
      {
	(*pver) = ut_realloc_1d_int (*pver, (*pverqty) + 1);
	(*pver)[(*pverqty)++] = Geo.FaceVerNb[face][j];
      }
  }

  ut_array_1d_int_sort (*pver, *pverqty);

  return;
}

void
neut_geo_poly_edges (struct GEO Geo, int poly, int** pedge, int* pedgeqty)
{
  int i, j, face;

  (*pedgeqty) = 0;

  for (i = 1; i <= Geo.PolyFaceQty[poly]; i++)
  {
    face = Geo.PolyFaceNb[poly][i];
    for (j = 1; j <= Geo.FaceVerQty[face]; j++)
      if (ut_array_1d_int_eltpos (*pedge, *pedgeqty, Geo.FaceEdgeNb[face][j]) == -1)
      {
	(*pedge) = ut_realloc_1d_int ((*pedge), (*pedgeqty) + 1);
	(*pedge)[(*pedgeqty)++] = Geo.FaceEdgeNb[face][j];
      }
  }
  
  ut_array_1d_int_sort (*pedge, *pedgeqty);

  return;
}


void
neut_geo_poly_edge_faces (struct GEO Geo, int poly, int edge, int** pface)
{
  int i, f, qty = 0;

  (*pface) = ut_alloc_1d_int (2);

  for (i = 0; i < Geo.EdgeFaceQty[edge]; i++)
  {
    f = Geo.EdgeFaceNb[edge][i];
    if (ut_array_1d_int_eltpos (Geo.FacePoly[f], 2, poly) != -1)
      (*pface)[qty++] = f;
  }

  if (qty != 2)
    ut_error_reportbug ();

  return;
}

void
neut_geo_face_edge_ori (struct GEO Geo, int face, int edge, int* pedgeori)
{
  int pos;

  pos = 1 + ut_array_1d_int_eltpos (Geo.FaceEdgeNb[face] + 1,
				    Geo.FaceVerQty[face], edge);
  if (pos == 0)
    (*pedgeori) = -1;
  else
    (*pedgeori) = Geo.FaceEdgeOri[face][pos];

  return;
}

void
neut_geo_poly_face_ori (struct GEO Geo, int poly, int face, int* pfaceori)
{
  int pos;

  pos = 1 + ut_array_1d_int_eltpos (Geo.PolyFaceNb[poly] + 1,
				    Geo.PolyFaceQty[poly], face);
  if (pos == 0)
    (*pfaceori) = -1;
  else
    (*pfaceori) = Geo.PolyFaceOri[poly][pos];

  return;
}

int
neut_geo_poly_true (struct GEO Geo, int nb)
{
  int i, j, res, verid;
  double ctov, dist;
  double *coo = ut_alloc_1d (3);
  int verqty;
  int* ver = NULL;
  double* proj = ut_alloc_1d (3);

  if (Geo.DomFaceQty == 0)
    ut_error_reportbug ();

  neut_geo_poly_vers (Geo, nb, &ver, &verqty);

  res = 1;
  for (i = 0; i < verqty; i++)
  {
    verid = ver[i];
    ctov = ut_space_dist (Geo.CenterCoo[nb], Geo.VerCoo[verid]);

    for (j = 1; j <= Geo.DomFaceQty; j++)
    {
      ut_array_1d_memcpy (proj, 3, Geo.VerCoo[verid]);
      ut_space_projpoint_alongonto (proj, Geo.DomFaceEq[j] + 1, Geo.DomFaceEq[j]);
      dist = ut_space_dist (Geo.VerCoo[verid], proj);

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
neut_geo_poly_body (struct GEO Geo, int nb)
{
  int i, face, res;

  res = 1;
  for (i = 1; i <= Geo.PolyFaceQty[nb]; i++)
  {
    face = Geo.PolyFaceNb[nb][i]; 

    if (Geo.FacePoly[face][0] < 0 || Geo.FacePoly[face][1] < 0)
    {
      res = 0;
      break;
    }
  }

  return res;
}

/* returns 0 if not true, 1 if true & 2 if all parent poly are true */
int
neut_geo_edge_true (struct GEO Geo, int *TruePoly, int nb)
{
  int j;
  int res, all;
  int polyqty;
  int* poly = NULL;

  neut_geo_edge_polys (Geo, nb, &poly, &polyqty);

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
neut_geo_edge_body (struct GEO Geo, int *BodyPoly, int nb)
{
  int j;
  int res, all;
  int polyqty;
  int* poly = NULL;

  neut_geo_edge_polys (Geo, nb, &poly, &polyqty);

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
neut_geo_face_true (struct GEO Geo, int *TruePoly, int nb)
{
  int j;
  int res, all;

  res = 0;
  all = 1;
  for (j = 0; j < 2; j++)
  {
    if (Geo.FacePoly[nb][j] > 0)
    {
      if (TruePoly[Geo.FacePoly[nb][j]] > 0)
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
neut_geo_ver_true (struct GEO Geo, int *TruePoly, int nb)
{
  int j;
  int res, all;
  int polyqty;
  int* poly = NULL;

  neut_geo_ver_polys (Geo, nb, &poly, &polyqty);

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
neut_geo_edge_polytruelevelmax (struct GEO Geo, int *TruePoly, int nb)
{
  int j, res;
  int polyqty;
  int* poly = NULL;

  neut_geo_edge_polys (Geo, nb, &poly, &polyqty);

  res = 0;
  for (j = 0; j < polyqty; j++)
    if (poly[j] > 0)
      res = ut_num_max_int (res, TruePoly[poly[j]]);

  ut_free_1d_int (poly);

  return res;
}

int
neut_geo_edge_polybodylevelmax (struct GEO Geo, int *BodyPoly, int nb)
{
  int j, res;
  int polyqty;
  int* poly = NULL;

  neut_geo_edge_polys (Geo, nb, &poly, &polyqty);

  res = 0;
  for (j = 0; j < polyqty; j++)
    if (poly[j] > 0)
      res = ut_num_max_int (res, BodyPoly[poly[j]]);

  ut_free_1d_int (poly);

  return res;
}

int
neut_geo_face_polybodylevelmax (struct GEO Geo, int nb)
{
  int res = 0;

  if (Geo.FacePoly[nb][0] > 0)
    res = Geo.PolyBody[Geo.FacePoly[nb][0]];
  if (Geo.FacePoly[nb][1] > 0)
    res = ut_num_max_int (res, Geo.PolyBody[Geo.FacePoly[nb][1]]);

  return res;
}

int
neut_geo_face_polytruelevelmax (struct GEO Geo, int nb)
{
  int res = 0;

  if (Geo.FacePoly[nb][0] > 0)
    res = Geo.PolyTrue[Geo.FacePoly[nb][0]];
  if (Geo.FacePoly[nb][1] > 0)
    res = ut_num_max_int (res, Geo.PolyTrue[Geo.FacePoly[nb][1]]);

  return res;
}

int
neut_geo_ver_polytruelevelmax (struct GEO Geo, int *TruePoly, int nb)
{
  int j, res;
  int polyqty;
  int* poly = NULL;

  neut_geo_ver_polys (Geo, nb, &poly, &polyqty);

  res = 0;
  for (j = 0; j < polyqty; j++)
    if (poly[j] > 0)
      res = ut_num_max_int (res, TruePoly[poly[j]]);

  ut_free_1d_int (poly);

  return res;
}

int
neut_geo_ver_polybodylevelmax (struct GEO Geo, int *BodyPoly, int nb)
{
  int j, res;
  int polyqty;
  int* poly = NULL;

  neut_geo_ver_polys (Geo, nb, &poly, &polyqty);

  res = 0;
  for (j = 0; j < polyqty; j++)
    if (poly[j] > 0)
      res = ut_num_max_int (res, BodyPoly[poly[j]]);

  ut_free_1d_int (poly);

  return res;
}

void
neut_geo_domface_edges (struct GEO Geo, int dface, int **pedge, int* pedgeqty)
{
  neut_geo_faces_edges (Geo, Geo.DomTessFaceNb[dface] + 1,
                        Geo.DomTessFaceQty[dface],
                        pedge, pedgeqty);

  return;
}

int
neut_geo_edge_selratio (struct GEO Geo, struct GEOPARA GeoPara, int edge,
                  double* pratio)
{
  double sel;
  
  if (GeoPara.dboundcl < 0)
    sel = GeoPara.sel;
  else 
  {
    int var_qty = 2;
    char** vars  = ut_alloc_2d_char (var_qty, 15);
    double* vals = ut_alloc_1d      (var_qty);
    int status = -1;
    double dbound;

    sprintf (vars[0], "body");
    sprintf (vars[1], "true");
    vals[0] = neut_geo_edge_polybodylevelmax (Geo, Geo.PolyBody, edge);
    vals[1] = neut_geo_edge_polytruelevelmax (Geo, Geo.PolyTrue, edge);

    status = ut_math_eval (GeoPara.dbound, var_qty, vars, vals, &dbound);

    if (status == -1)
      abort ();

    if (dbound == 1)
      sel = GeoPara.dboundsel;           // means on bound
    else
      sel = GeoPara.sel;

    ut_free_2d_char (vars, var_qty);
    ut_free_1d (vals);
  }

  if (Geo.EdgeState[edge] == 0)
  {
    (*pratio) = Geo.EdgeLength[edge] / sel;
    return 0;
  }
  else
    return 1;
}

int
neut_geo_face_area (struct GEO Geo, int face, double* parea)
{
  int i, ver;
  double* p0 = NULL;
  double* p1 = NULL;
  double* p2 = NULL;

  p0 = ut_alloc_1d (3);

  if (face < 1 || face > Geo.FaceQty || Geo.FaceState[face] == -1)
  {
    (*parea) = 0;
    return -1;
  }

  if (Geo.FaceState[face] > 0)
  {
    if (Geo.FacePt[face] == 0)
      ut_array_1d_memcpy (p0, 3, Geo.FacePtCoo[face]);
    else if (Geo.FacePt[face] > 0)
    {
      ver = Geo.FaceVerNb[face][Geo.FacePt[face]];
      ut_array_1d_memcpy (p0, 3, Geo.VerCoo[ver]);
    }
    else 
      neut_geo_face_centre (Geo, face, p0);
  }
  else if (Geo.FaceState[face] == 0)
  {
    ver = Geo.FaceVerNb[face][1];
    ut_array_1d_memcpy (p0, 3, Geo.VerCoo[ver]);
  }

  (*parea) = 0;
  for (i = 1; i < Geo.FaceVerQty[face]; i++)
  {
    p1 = Geo.VerCoo[Geo.FaceVerNb[face][i]];
    p2 = Geo.VerCoo[Geo.FaceVerNb[face][i + 1]];
    (*parea) += ut_space_triangle_area (p0, p1, p2);
  }

  p1 = Geo.VerCoo[Geo.FaceVerNb[face][Geo.FaceVerQty[face]]];
  p2 = Geo.VerCoo[Geo.FaceVerNb[face][1]];
  (*parea) += ut_space_triangle_area (p0, p1, p2);

  ut_free_1d (p0);

  return 0;
}

int
neut_geo_volume (struct GEO Geo, double* pvol)
{
  int i;
  double tmp;

  (*pvol) = 0;
  for (i = 1; i <= Geo.PolyQty; i++)
  {
    neut_geo_poly_volume (Geo, i, &tmp);
    (*pvol) += tmp;
  }

  return 0;
}

int
neut_geo_poly_volume (struct GEO Geo, int poly, double* pvol)
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

  if (poly < 1 || poly > Geo.PolyQty)
  {
    (*pvol) = 0;
    return -1;
  }

  (*pvol) = 0;
  for (f = 1; f <= Geo.PolyFaceQty[poly]; f++)
  {
    face = Geo.PolyFaceNb[poly][f];

    if (Geo.FaceState[face] > 0)
    {
      if (Geo.FacePt[face] == 0)
	ut_array_1d_memcpy (p0, 3, Geo.FacePtCoo[face]);
      else if (Geo.FacePt[face] == -1)
      {
	ver = Geo.FaceVerNb[face][1];
	neut_geo_face_centre (Geo, face, p0);
      }
      else
      {
	ver = Geo.FaceVerNb[face][Geo.FacePt[face]];
	ut_array_1d_memcpy (p0, 3, Geo.VerCoo[ver]);
      }
    }
    else
    {
      ver = Geo.FaceVerNb[face][1];
      ut_array_1d_memcpy (p0, 3, Geo.VerCoo[ver]);
    }

    for (i = 1; i < Geo.FaceVerQty[face]; i++)
    {
      p1 = Geo.VerCoo[Geo.FaceVerNb[face][i]];
      p2 = Geo.VerCoo[Geo.FaceVerNb[face][i + 1]];
      area = ut_space_triangle_area (p0, p1, p2);
      if (area > 1e-20)
      {
	ut_array_1d_memcpy (proj, 3, Geo.CenterCoo[poly]);
	ut_space_points_plane (p0, p1, p2, eq);
	ut_space_projpoint_alongonto (proj, eq + 1, eq);
	h = ut_space_dist (Geo.CenterCoo[poly], proj);
	(*pvol) += area * h * 0.3333333333333333333333;
      }
    }

    p1 = Geo.VerCoo[Geo.FaceVerNb[face][Geo.FaceVerQty[face]]];
    p2 = Geo.VerCoo[Geo.FaceVerNb[face][1]];
    area = ut_space_triangle_area (p0, p1, p2);
    if (area > 1e-20)
    {
      ut_array_1d_memcpy (proj, 3, Geo.CenterCoo[poly]);
      ut_space_points_plane (p0, p1, p2, eq);
      ut_space_projpoint_alongonto (proj, eq + 1, eq);
      h = ut_space_dist (Geo.CenterCoo[poly], proj);
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
neut_geo_poly_centroid (struct GEO Geo, int poly, double* coo)
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

  if (poly < 1 || poly > Geo.PolyQty)
    return -1;

  totvol = 0;
  ut_array_1d_set (coo, 3, 0);

  for (f = 1; f <= Geo.PolyFaceQty[poly]; f++)
  {
    face = Geo.PolyFaceNb[poly][f];

    if (Geo.FaceState[face] > 0)
    {
      if (Geo.FacePt[face] == 0)
	ut_array_1d_memcpy (p0, 3, Geo.FacePtCoo[face]);
      else if (Geo.FacePt[face] == -1)
      {
	ver = Geo.FaceVerNb[face][1];
	neut_geo_face_centre (Geo, face, p0);
      }
      else
      {
	ver = Geo.FaceVerNb[face][Geo.FacePt[face]];
	ut_array_1d_memcpy (p0, 3, Geo.VerCoo[ver]);
      }
    }
    else
    {
      ver = Geo.FaceVerNb[face][1];
      ut_array_1d_memcpy (p0, 3, Geo.VerCoo[ver]);
    }

    for (i = 1; i <= Geo.FaceVerQty[face]; i++)
    {
      p1 = Geo.VerCoo[Geo.FaceVerNb[face][i]];
      p2 = Geo.VerCoo[Geo.FaceVerNb[face]
	             [ut_num_rotpos (1, Geo.FaceVerQty[face], i, 1)]];

      area = ut_space_triangle_area (p0, p1, p2);

      if (area > 1e-20)
      {
	ut_space_tet_centre (Geo.CenterCoo[poly], p0, p1, p2, tmp);
	vol = ut_space_tet_volume (Geo.CenterCoo[poly], p0, p1, p2);
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
neut_geo_polys_skinfaces (struct GEO Geo, int* poly, int polyqty,
                          int** pface, int** pfacepoly, int* pfaceqty)
{
  int i, j, k, pos, p, f;

  // allocation
  (*pfaceqty) = 0;
  for (i = 0; i < polyqty; i++)
  {
    p = poly[i];
    (*pfaceqty) += Geo.PolyFaceQty[p];
  }
  (*pface) = ut_alloc_1d_int ((*pfaceqty));
  (*pfacepoly) = ut_alloc_1d_int ((*pfaceqty));

  (*pfaceqty) = 0;
  for (i = 0; i < polyqty; i++)
  {
    p = poly[i];
    for (j = 1; j <= Geo.PolyFaceQty[p]; j++)
    {
      f = Geo.PolyFaceNb[p][j];
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
neut_geo_boundingbox (struct GEO Geo, double** size)
{
  int i, j;

  /* Searching the tessellation bounding box */
  size[0][0] = size[1][0] = size[2][0] = DBL_MAX;
  size[0][1] = size[1][1] = size[2][1] = -DBL_MAX;

  for (i = 1; i <= Geo.VerQty; i++)
    for (j = 0; j < 3; j++)
    {
      size[j][0] = ut_num_min (size[j][0], Geo.VerCoo[i][j]);
      size[j][1] = ut_num_max (size[j][1], Geo.VerCoo[i][j]);
    }

  return;
}

void
neut_geo_face_centre (struct GEO Geo, int face, double* centre)
{
  int i, ver;

  ut_array_1d_set (centre, 3, 0);
  for (i = 1; i <= Geo.FaceVerQty[face]; i++)
  {
    ver = Geo.FaceVerNb[face][i];
    ut_array_1d_add (centre, Geo.VerCoo[ver], 3, centre);
  }
  ut_array_1d_scale (centre, 3, 1./Geo.FaceVerQty[face]);

  return;
}

int
neut_geo_edge_centre (struct GEO Geo, int edge, double* pos)
{
  int i;

  for (i = 0; i < 3; i++)
    pos[i] = (Geo.VerCoo[Geo.EdgeVerNb[edge][0]][i]
	    + Geo.VerCoo[Geo.EdgeVerNb[edge][1]][i]) / 2.;
  
  return 0;
}

void
neut_geo_centre (struct GEO Geo, double* centre)
{
  int i;
  double totvol, vol;
  double* coo = ut_alloc_1d (3);

  ut_array_1d_zero (centre, 3);
  totvol = 0;
  for (i = 1; i <= Geo.PolyQty; i++)
  {
    neut_geo_poly_centroid (Geo, i, coo);
    neut_geo_poly_volume   (Geo, i, &vol);
    ut_array_1d_scale (coo, 3, vol);
    ut_array_1d_add (centre, coo, 3, centre);
    totvol += vol;
  }
  ut_array_1d_scale (centre, 3, 1. / totvol);

  ut_free_1d (coo);

  return;
}

void
neut_geo_poly_neighpoly (struct GEO Geo, int poly, int** pnpoly, int* pnpolyqty)
{
  int i, j, face;

  (*pnpolyqty) = 0;
  for (i = 1; i <= Geo.PolyFaceQty[poly]; i++)
  {
    face = Geo.PolyFaceNb[poly][i];

    for (j = 0; j < 2; j++)
      if (Geo.FacePoly[face][j] > 0 && Geo.FacePoly[face][j] != poly)
      {
	(*pnpoly) = ut_realloc_1d_int ((*pnpoly), (*pnpolyqty) + 1);
	(*pnpoly)[(*pnpolyqty)++] = Geo.FacePoly[face][j];
      }
  }

  return;
}
int
neut_geo_expr_polytab (struct GEO Geo, char* expr, int* poly)
{
  int i, id;
  FILE* file;

  if (strcmp (expr, "all") == 0)
    ut_array_1d_int_set (poly + 1, Geo.PolyQty, 1);

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

    for (i = 1; i <= Geo.PolyQty; i++)
    {
      ut_array_1d_memcpy (vals, 3, Geo.CenterCoo[i]);
      neut_geo_poly_volume (Geo, i, &(vals[3]));
      vals[4] = Geo.PolyTrue[i];
      vals[5] = Geo.PolyBody[i];
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
neut_geo_expr_facetab (struct GEO Geo, char* expr, int* face)
{
  int i, id;
  FILE* file;

  if (strcmp (expr, "all") == 0)
    ut_array_1d_int_set (face + 1, Geo.FaceQty, 1);

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

    for (i = 1; i <= Geo.FaceQty; i++)
    {
      neut_geo_face_centre (Geo, i, vals);
      neut_geo_face_area (Geo, i, &(vals[3]));
      vals[4] = neut_geo_face_true (Geo, Geo.PolyTrue, i);
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
neut_geo_poly_skinmesh (struct GEO Geo, int nb,
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
  neut_geo_poly_vers (Geo, nb, &ver, &verqty);
  ut_array_1d_int_inv (ver, verqty, &verinv, &verinvqty);
  ut_array_1d_int_addval (verinv, verinvqty, 1, verinv);

  for (i = 0; i < verqty; i++)
    neut_nodes_addnode (pNodes, Geo.VerCoo[ver[i]], 1);

  // for each face, recording a new node if necessary and recording the
  // elements
  for (i = 1; i <= Geo.PolyFaceQty[nb]; i++)
  {
    face = Geo.PolyFaceNb[nb][i];

    verqty = Geo.FaceVerQty[face];

    // adding node if defined as the barycenter
    // FacePt == -1 stuff added but not tested.
    if (Geo.FaceState[face] == 0)
    {
      neut_geo_face_centre (Geo, face, Geo.FacePtCoo[face]);
      neut_nodes_addnode (pNodes, Geo.FacePtCoo[face], 1);
    }
    else if (Geo.FacePt[face] == 0)
    {
      neut_nodes_addnode (pNodes, Geo.FacePtCoo[face], 1);
    }
    else if (Geo.FacePt[face] == -1)
    {
      neut_geo_face_centre (Geo, face, centre);
      neut_nodes_addnode (pNodes, centre, 1);
    }

    // looking for the master node (as recorded in Nodes)
    node0 = (Geo.FacePt[face] > 0) ?
            verinv[Geo.FaceVerNb[face][Geo.FacePt[face]]]
	  : (*pNodes).NodeQty;

    for (j = 1; j <= verqty; j++)
    {
      // if p0 is a face vertex: 
      // if the ver is p0 or the previous vertex, it is skipped
      if (j == Geo.FacePt[face]
	  || ut_num_rotpos (1, verqty, j, 1) == Geo.FacePt[face])
	continue;

      // recording coo of the 1st and 2nd pts
      eltnodes[0] = node0;
      eltnodes[1] = verinv[Geo.FaceVerNb[face][j]];
      eltnodes[2] = verinv[Geo.FaceVerNb[face][ut_num_rotpos (1, verqty, j, 1)]];

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
neut_geo_poly_mesh (struct GEO Geo, int nb,
		    struct NODES* pNodes, struct MESH* pMesh)
{
  int i;
  struct MESH Mesh2D;
  int* eltnodes = ut_alloc_1d_int (4);

  neut_geo_poly_skinmesh (Geo, nb, pNodes, &Mesh2D);
  neut_nodes_addnode (pNodes, Geo.CenterCoo[nb], 1);

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
neut_geo_poly_reg (struct GEO Geo, int nb)
{
  int i;

  for (i = 1; i <= Geo.PolyFaceQty[nb]; i++)
    if (Geo.FaceState[Geo.PolyFaceNb[nb][i]] != 0)
      return 1;

  return 0;
}

int
neut_geo_poly_std (struct GEO Geo, int nb)
{
  return (neut_geo_poly_reg (Geo, nb) == 0) ? 1 : 0;
}

int
neut_geo_point_inpoly_std (struct GEO Geo, double* coo, int nb)
{
  int i, j, res;
  double* faceeq = ut_alloc_1d (4);

  res = 1;
  for (i = 1; i <= Geo.PolyFaceQty[nb]; i++)
  {
    for (j = 0; j < 4; j++)
      faceeq[j] =
	(double) Geo.PolyFaceOri[nb][i] *
	Geo.FaceEq[Geo.PolyFaceNb[nb][i]][j];

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
neut_geo_point_inpoly_reg (struct GEO Geo, double* coo, int nb)
{
  int elt, status;
  struct MESH Mesh;
  struct NODES Nodes;

  neut_mesh_set_zero  (&Mesh);
  neut_nodes_set_zero (&Nodes);

  neut_geo_poly_mesh (Geo, nb, &Nodes, &Mesh);

  status = neut_mesh_elset_point_elt (Mesh, Nodes, 1, coo, &elt);

  neut_mesh_free  (&Mesh);
  neut_nodes_free (&Nodes);

  if (status == 0)
    return 1;
  else
    return 0;
}

int
neut_geo_point_inpoly (struct GEO Geo, double* coo, int nb)
{
  if (neut_geo_poly_std (Geo, nb) == 1)
    return neut_geo_point_inpoly_std (Geo, coo, nb);
  else
    return neut_geo_point_inpoly_reg (Geo, coo, nb);
}

// Returns the interpolation (triangles) of a (regularized or not) face
// under the form of a mesh.
int
neut_geo_face_interpolmesh (struct GEO Geo, int face, struct NODES* pN,
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

  verqty = Geo.FaceVerQty[face];
  for (i = 1; i <= verqty; i++)
  {
    ver = Geo.FaceVerNb[face][i];
    neut_nodes_addnode (pN, Geo.VerCoo[ver], 1);
  }

  if (Geo.FacePt[face] == 0)
  {
    neut_nodes_addnode (pN, Geo.FacePtCoo[face], 1);
    ver0 = verqty + 1;
  }
  else if  (Geo.FacePt[face] > 0)
    ver0 = Geo.FacePt[face];
  else 
  {
    neut_geo_face_centre (Geo, face, p0);
    neut_nodes_addnode (pN, p0, 1);
    ver0 = verqty + 1;
  }

  ut_array_1d_memcpy (p0, 3, Geo.VerCoo[ver0]);

  tqty = 0;
  for (i = 1; i <= verqty; i++)
  {
    // if p0 is a face vertex: 
    // if the ver is p0 or the previous vertex, it is skipped
    if (i == Geo.FacePt[face]
	|| ut_num_rotpos (1, verqty, i, 1) == Geo.FacePt[face])
      continue;

    // recording coo of the 1st and 2nd pts
    nodes[0] = ver0;
    nodes[1] = i;
    nodes[2] = ut_num_rotpos (1, verqty, i, 1);
    p1 = Geo.VerCoo[nodes[1]];
    p2 = Geo.VerCoo[nodes[2]];

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
neut_geo_domfaces_commondomver (struct GEO Geo, int* face, int faceqty, int* pver)
{
  int i, status, qty;
  int* nb = NULL;

  if (faceqty <= 0)
  {
    (*pver) = -1;
    return -1;
  }

  nb = ut_alloc_1d_int (Geo.DomFaceVerQty[face[0]]);
  qty = Geo.DomFaceVerQty[face[0]];
  ut_array_1d_int_memcpy (nb, Geo.DomFaceVerQty[face[0]], Geo.DomFaceVerNb[face[0]] + 1);

  for (i = 1; i < faceqty && qty > 0; i++)
    ut_array_1d_int_inter (nb, qty, Geo.DomFaceVerNb[face[i]] + 1,
		           Geo.DomFaceVerQty[face[i]], nb, &qty);

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
neut_geo_domfaces_commondomedge (struct GEO Geo, int* dface, int dfaceqty, int* pdedge)
{
  int i, status, qty;
  int* nb = NULL;

  if (dfaceqty <= 0)
  {
    (*pdedge) = -1;
    return -1;
  }

  nb = ut_alloc_1d_int (Geo.DomFaceVerQty[dface[0]]);
  qty = Geo.DomFaceVerQty[dface[0]];
  ut_array_1d_int_memcpy (nb, Geo.DomFaceVerQty[dface[0]], Geo.DomFaceEdgeNb[dface[0]] + 1);

  for (i = 1; i < dfaceqty && qty > 0; i++)
    ut_array_1d_int_inter (nb, qty, Geo.DomFaceEdgeNb[dface[i]] + 1,
		           Geo.DomFaceVerQty[dface[i]], nb, &qty);

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
neut_geo_domvers_commondomedge (struct GEO Geo, int* dver, int dverqty, int* pdedge)
{
  int status, i, qty;
  int* id = NULL;
  
  // no vers: return
  if (dverqty <= 0)
  {
    (*pdedge) = -1;
    return -1;
  }

  qty = Geo.DomVerEdgeQty[dver[0]];

  id = ut_alloc_1d_int (qty);
  ut_array_1d_int_memcpy (id, qty, Geo.DomVerEdgeNb[dver[0]]);

  for (i = 1; i < dverqty; i++)
    ut_array_1d_int_inter (id, qty,
	                   Geo.DomVerEdgeNb[dver[i]], Geo.DomVerEdgeQty[dver[i]], 
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
neut_geo_domedges_commondomver (struct GEO Geo, int* dedge, int dedgeqty, int* pdver)
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
  ut_array_1d_int_memcpy (id, qty, Geo.DomEdgeVerNb[dedge[0]]);

  for (i = 1; i < dedgeqty; i++)
    ut_array_1d_int_inter (id, qty, Geo.DomEdgeVerNb[dedge[i]], 2, id, &qty);

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
neut_geo_domedges_domvers (struct GEO Geo, int* dedge, int dedgeqty, int** pdver, int* pdverqty)
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
    ut_array_1d_int_memcpy (*pdver + *pdverqty, 2, Geo.DomEdgeVerNb[id]);
    (*pdverqty) += 2;
  }

  ut_array_1d_int_sort_uniq (*pdver, *pdverqty, pdverqty);

  return 0;
}

int
neut_geo_domedges_commondomface (struct GEO Geo, int* dedge, int dedgeqty, int* pdface)
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
  ut_array_1d_int_memcpy (id, qty, Geo.DomEdgeFaceNb[dedge[0]]);

  for (i = 1; i < dedgeqty; i++)
    ut_array_1d_int_inter (id, qty, Geo.DomEdgeFaceNb[dedge[i]], 2, id, &qty);

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
neut_geo_domver_domface (struct GEO Geo, int dver, int** pdface, int* pdfaceqty)
{
  int i, j, tmpedge;

  (*pdfaceqty) = 0;
  for (i = 0; i < Geo.DomVerEdgeQty[dver]; i++)
  {
    tmpedge = Geo.DomVerEdgeNb[dver][i];
    (*pdface) = ut_realloc_1d_int ((*pdface), (*pdfaceqty) + 2);
    for (j = 0; j < 2; j++)
      (*pdface)[(*pdfaceqty)++] = Geo.DomEdgeFaceNb[tmpedge][j];
  }

  ut_array_1d_int_sort_uniq (*pdface, *pdfaceqty, pdfaceqty);

  return;
}

void
neut_geo_domedge_ver (struct GEO Geo, int dedge, int **pver, int* pverqty)
{
  int i, edge;

  (*pver) = ut_alloc_1d_int (Geo.DomTessEdgeQty[dedge] * 2);

  (*pverqty) = 0;
  for (i = 1; i <= Geo.DomTessEdgeQty[dedge]; i++)
  {
    edge = Geo.DomTessEdgeNb[dedge][i];

    ut_array_1d_int_memcpy ((*pver) + (*pverqty), 2, Geo.EdgeVerNb[edge]);

    (*pverqty) += 2;
  }

  ut_array_1d_int_sort_uniq ((*pver), (*pverqty), pverqty);

  return;
}

void
neut_geo_domface_ver (struct GEO Geo, int dface, int **pver, int* pverqty)
{
  int i, j, face;

  (*pverqty) = 0;
  for (i = 1; i <= Geo.DomTessFaceQty[dface]; i++)
  {
    face = Geo.DomTessFaceNb[dface][i];
    
    (*pver) = ut_realloc_1d_int ((*pver), (*pverqty) + Geo.FaceVerQty[face]);

    for (j = 1; j <= Geo.FaceVerQty[face]; j++)
      (*pver)[(*pverqty)++] = Geo.FaceVerNb[face][j];
  }

  ut_array_1d_int_sort_uniq (*pver, *pverqty, pverqty);

  return;
}

void
neut_geo_faces_edges (struct GEO Geo, int* face, int faceqty, int** pedge, int* pedgeqty)
{
  int i, id;

  if (ut_array_1d_int_max (face, faceqty) > Geo.FaceQty)
    ut_error_reportbug ();

  (*pedgeqty) = 0;
  ut_free_1d_int (*pedge);

  for (i = 0; i < faceqty; i++)
  {
    id = face[i];
    (*pedge) = ut_realloc_1d_int ((*pedge), (*pedgeqty) + Geo.FaceVerQty[id]);
    ut_array_1d_int_memcpy ((*pedge) + (*pedgeqty), Geo.FaceVerQty[id], Geo.FaceEdgeNb[id] + 1);

    (*pedgeqty) += Geo.FaceVerQty[id];
  }

  ut_array_1d_int_sort_uniq (*pedge, *pedgeqty, pedgeqty);

  return;
}

int
neut_geo_edge_fake (struct GEO Geo, int edge)
{
  int domedge, face, domface1, domface2, poly1, poly2, j, status;

  status = 0;

  if (! strcmp (Geo.DomType, "cylinder"))
  {
    if (Geo.EdgeDom[edge][0] == 1)
    {
      domedge = Geo.EdgeDom[edge][1];
      domface1 = Geo.DomEdgeFaceNb[domedge][0];
      domface2 = Geo.DomEdgeFaceNb[domedge][1];

      poly1 = -1;
      poly2 = -1;
      for (j = 0; j < Geo.EdgeFaceQty[edge]; j++)
      {
	face = Geo.EdgeFaceNb[edge][j];
	if (Geo.FaceDom[face][0] == 2)
	{
	  if (Geo.FaceDom[face][1] == domface1)
	    poly1 = Geo.FacePoly[face][0];
	  if (Geo.FaceDom[face][1] == domface2)
	    poly2 = Geo.FacePoly[face][0];
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
