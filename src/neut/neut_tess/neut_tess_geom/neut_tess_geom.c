/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_geom_lcl.h"

int
neut_tess_edge_selratio (struct TESS Tess, struct REG Reg, int edge,
                  double* pratio)
{
  double sel;
  
  if (! Reg.dbound)
    sel = Reg.sel;
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

    status = ut_math_eval (Reg.dbound, var_qty, vars, vals, &dbound);

    if (status == -1)
      abort ();

    if (dbound == 1)
      sel = Reg.dboundsel;           // means on bound
    else
      sel = Reg.sel;

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
  if (elttype == NULL || ! strcmp (elttype, "tri"))
    (*pcl) = rcl * 0.5 * pow (vol / PolyQty, 0.3333333333333333333333333333);
  else if (! strcmp (elttype, "quad"))
  {
    (*pcl) = rcl * 0.5 * pow (vol / PolyQty, 0.3333333333333333333333333333);
    (*pcl) *= .43084877208099956915;
  }
  
  return;
}
