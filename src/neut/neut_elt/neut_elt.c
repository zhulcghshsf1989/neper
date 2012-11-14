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
