/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_elt.h"

void
neut_elt_name_prop (char *format, char *name, char *type, int *pDimension,
		    int *pOrder)
{
  if (strcmp (format, "geof") == 0)
  {
    if (strcmp (name, "c3d4") == 0)
    {
      sprintf (type, "tri");
      (*pDimension) = 3;
      (*pOrder) = 1;
    }
    else if (strcmp (name, "c3d10") == 0)
    {
      sprintf (type, "tri");
      (*pDimension) = 3;
      (*pOrder) = 2;
    }
    else
    {
      ut_print_message (2, 0, "Elt name unknown.\n");
      abort ();
    }
  }
  else
  {
    ut_print_message (2, 0, "Format unknown.\n");
    abort ();
  }

  return;
}

int
neut_elt_nodeqty (char *type, int dim, int order)
{
  int res;
  int **TriNodesPerElt = ut_alloc_2d_int (4, 3);

  TriNodesPerElt[0][1] = 1;
  TriNodesPerElt[0][2] = 1;
  TriNodesPerElt[1][1] = 2;
  TriNodesPerElt[1][2] = 3;
  TriNodesPerElt[2][1] = 3;
  TriNodesPerElt[2][2] = 6;
  TriNodesPerElt[3][1] = 4;
  TriNodesPerElt[3][2] = 10;

  int **QuadNodesPerElt = ut_alloc_2d_int (4, 3);

  QuadNodesPerElt[0][1] = 1;
  QuadNodesPerElt[0][2] = 1;
  QuadNodesPerElt[1][1] = 2;
  QuadNodesPerElt[1][2] = 3;
  QuadNodesPerElt[2][1] = 4;
  QuadNodesPerElt[2][2] = 8;
  QuadNodesPerElt[3][1] = 8;
  QuadNodesPerElt[3][2] = 20;

  if (dim > 3 || order > 2)
  {
    ut_print_message (2, 0, "Mesh dimension or order out of bounds\n");
    printf ("dim = %d, order = %d\n", dim, order);
    abort ();
  }

  if (type == NULL || strcmp (type, "tri") == 0)
    res = TriNodesPerElt[dim][order];
  else if (strcmp (type, "quad") == 0)
    res = QuadNodesPerElt[dim][order];
  else
  {
    ut_print_message (2, 0, "neut_elt_nodeqty: unknown elt type `%s'.\n", type);
    abort ();
  }

  ut_free_2d_int (TriNodesPerElt, 4);
  ut_free_2d_int (QuadNodesPerElt, 4);

  return res;
}

int
neut_elt_order (char *type, int dim, int nodeqty)
{
  int res = -1;

  if (strcmp (type, "tri") == 0)
  {
    if (dim == 3)
    {
      if (nodeqty == 4)
	res = 1;
      else if (nodeqty == 10)
	res = 2;
    }
    if (dim == 2)
    {
      if (nodeqty == 3)
	res = 1;
      else if (nodeqty == 6)
	res = 2;
    }
    if (dim == 1)
    {
      if (nodeqty == 2)
	res = 1;
      else if (nodeqty == 3)
	res = 2;
    }
    if (dim == 0)
      res = 1;
  }

  return res;
}

int
neut_elt_gtype_prop (int gtype, char* type, int* pdim, int* porder)
{
  int status;

  status = 0;
  if      (gtype ==  1) // 2-node line
  {
    sprintf (type, "tri");
    (*pdim) = 1;
    (*porder) = 1;
  }
  else if (gtype ==  2) // 3-node triangle
  {
    sprintf (type, "tri");
    (*pdim) = 2;
    (*porder) = 1;
  }
  else if (gtype ==  3) // 4-node quadrangle
  {
    sprintf (type, "quad");
    (*pdim) = 2;
    (*porder) = 1;
  }
  else if (gtype ==  4) // 4-node tetrahedron
  {
    sprintf (type, "tri");
    (*pdim) = 3;
    (*porder) = 1;
  }
  else if (gtype ==  5) // 8-node hexahedron
  {
    sprintf (type, "quad");
    (*pdim) = 3;
    (*porder) = 1;
  }
  else if (gtype ==  6) // 6-node prism
  {
    sprintf (type, "pris");
    (*pdim) = 3;
    (*porder) = 1;
  }
  else if (gtype ==  7) // 5-node pyramid
  {
    sprintf (type, "pyr");
    (*pdim) = 3;
    (*porder) = 1;
  }
  else if (gtype ==  8) // 3-node 2nd order line
  {
    sprintf (type, "tri");
    (*pdim) = 1;
    (*porder) = 2;
  }
  else if (gtype ==  9) // 6-node 2nd order triangle
  {
    sprintf (type, "tri");
    (*pdim) = 2;
    (*porder) = 2;
  }
  else if (gtype == 11) // 10-node 2nd order tetrahedron
  {
    sprintf (type, "tri");
    (*pdim) = 3;
    (*porder) = 2;
  }
  else if (gtype == 15) // 1-node point
  {
    sprintf (type, "tri");
    (*pdim) = 0;
    (*porder) = 1;
  }
  else if (gtype == 16) // 8-node 2nd order quadrangle
  {
    sprintf (type, "quad");
    (*pdim) = 2;
    (*porder) = 2;
  }
  else if (gtype == 17) // 8-node 2nd order quadrangle
  {
    sprintf (type, "quad");
    (*pdim) = 3;
    (*porder) = 2;
  }
  else
    status = -1;

  return status;
}

int
neut_elt_dim_gtype_order (int dim, int gtype)
{
  if (dim == 0 && gtype == 15)
    return 1;
  else if (dim == 1 && gtype == 1)
    return 1;
  else if (dim == 1 && gtype == 8)
    return 2;
  else if (dim == 2 && gtype == 2)
    return 1;
  else if (dim == 2 && gtype == 9)
    return 2;
  else if (dim == 3 && gtype == 4)
    return 1;
  else if (dim == 3 && gtype == 11)
    return 2;
  else
    return -1;
}

double
neut_elt_rr_3d (double *p1, double *p2, double *p3, double *p4)
{
  int i, j, k, l;		/* loop indices */
  double facenormal[4][3];	/* the normals of each face of the tet */
  double facearea2[4];		/* areas of the faces of the tet */
  double Z;			/* quantity needed for circumradius */
  double facesum = 0.0;		/* sum of the areas of the faces */
  double vol;
  double **point = ut_alloc_1d_pdouble (4);
  point[0] = p1;
  point[1] = p2;
  point[2] = p3;
  point[3] = p4;

  /* calculate the volume of the tetrahedron */
  vol = ut_space_tet_volume (p1, p2, p3, p4);

  /* for each vertex/face of the tetrahedron */
  for (i = 0; i < 4; i++)
  {
    j = (i + 1) & 3;
    if ((i & 1) == 0)
    {
      k = (i + 3) & 3;
      l = (i + 2) & 3;
    }
    else
    {
      k = (i + 2) & 3;
      l = (i + 3) & 3;
    }

    /* compute the normal for each face */
    facenormal[i][0] =
      (point[k][1] - point[j][1]) * (point[l][2] - point[j][2]) -
      (point[k][2] - point[j][2]) * (point[l][1] - point[j][1]);
    facenormal[i][1] =
      (point[k][2] - point[j][2]) * (point[l][0] - point[j][0]) -
      (point[k][0] - point[j][0]) * (point[l][2] - point[j][2]);
    facenormal[i][2] =
      (point[k][0] - point[j][0]) * (point[l][1] - point[j][1]) -
      (point[k][1] - point[j][1]) * (point[l][0] - point[j][0]);

    /* compute (2 *area)^2 for this face */
    facearea2[i] = facenormal[i][0] * facenormal[i][0] +
      facenormal[i][1] * facenormal[i][1] +
      facenormal[i][2] * facenormal[i][2];
    facesum += sqrt (facearea2[i]) * 0.5;
  }

  /* compute Z */
  Z = getZ (p1, p2, p3, p4);

  /* now we are ready to compute the radius ratio, which is
     (108 * V^2) / Z (A1 + A2 + A3 + A4) */

  ut_free_1d_pdouble (point);

  return (108 * vol * vol) / (Z * facesum);
}

/* compute Z, a quantity associated with circumradius computation
   TODO this code is lifted from Jonathan's tetcircumcenter computation
   in primitives.c */
double
getZ (double *tetorg, double *tetdest, double *tetfapex, double *tettapex)
{
  double xot, yot, zot, xdt, ydt, zdt, xft, yft, zft;
  double otlength, dtlength, ftlength;
  double xcrossdf, ycrossdf, zcrossdf;
  double xcrossfo, ycrossfo, zcrossfo;
  double xcrossod, ycrossod, zcrossod;
  double xct, yct, zct;

  /* Use coordinates relative to the apex of the tetrahedron. */
  xot = tetorg[0] - tettapex[0];
  yot = tetorg[1] - tettapex[1];
  zot = tetorg[2] - tettapex[2];
  xdt = tetdest[0] - tettapex[0];
  ydt = tetdest[1] - tettapex[1];
  zdt = tetdest[2] - tettapex[2];
  xft = tetfapex[0] - tettapex[0];
  yft = tetfapex[1] - tettapex[1];
  zft = tetfapex[2] - tettapex[2];
  /* Squares of lengths of the origin, destination, and face apex edges. */
  otlength = xot * xot + yot * yot + zot * zot;
  dtlength = xdt * xdt + ydt * ydt + zdt * zdt;
  ftlength = xft * xft + yft * yft + zft * zft;
  /* Cross products of the origin, destination, and face apex vectors. */
  xcrossdf = ydt * zft - yft * zdt;
  ycrossdf = zdt * xft - zft * xdt;
  zcrossdf = xdt * yft - xft * ydt;
  xcrossfo = yft * zot - yot * zft;
  ycrossfo = zft * xot - zot * xft;
  zcrossfo = xft * yot - xot * yft;
  xcrossod = yot * zdt - ydt * zot;
  ycrossod = zot * xdt - zdt * xot;
  zcrossod = xot * ydt - xdt * yot;

  /* Calculate offset (from apex) of circumcenter. */
  xct = (otlength * xcrossdf + dtlength * xcrossfo + ftlength * xcrossod);
  yct = (otlength * ycrossdf + dtlength * ycrossfo + ftlength * ycrossod);
  zct = (otlength * zcrossdf + dtlength * zcrossfo + ftlength * zcrossod);

  /* Calculate the length of this vector, which is Z */
  return sqrt (xct * xct + yct * yct + zct * zct);
}

/* returns the radius ratio for a triangle
 * rr = 16 * A^2 / (l1.l2.l3.(l1+l2+l3)) */
double
neut_elt_rr_2d (double *p1, double *p2, double *p3)
{
  double l1, l2, l3, area;

  area = ut_space_trianglearea (p1, p2, p3);
  l1 = ut_space_dist (p1, p2);
  l2 = ut_space_dist (p2, p3);
  l3 = ut_space_dist (p3, p1);

  return 16 * area * area / (l1 * l2 * l3 * (l1 + l2 + l3));
}

// rho factor as defined in Gmsh
double
neut_elt_3d_rho (double* p1, double* p2, double* p3, double* p4)
{
  double rho;
  double* l = ut_alloc_1d (6);

  l[1] = ut_space_dist (p1, p3);
  l[0] = ut_space_dist (p1, p2);
  l[2] = ut_space_dist (p1, p4);
  l[3] = ut_space_dist (p2, p3);
  l[4] = ut_space_dist (p2, p4);
  l[5] = ut_space_dist (p3, p4);

  rho = ut_array_1d_min (l, 6) / ut_array_1d_max (l, 6);

  ut_free_1d (l);

  return rho;
}
