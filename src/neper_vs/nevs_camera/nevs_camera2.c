/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nevs_camera.h"
  
void
nevs_camera_coo (char* expr, struct GEO Geo, struct NODES Nodes,
                 struct MESH Mesh, struct PRINT* pPrint, int id)
{
  if (id < 1 || id > 3)
  {
    nevs_camera_coo (expr, Geo, Nodes, Mesh, pPrint, 1);
    nevs_camera_coo (expr, Geo, Nodes, Mesh, pPrint, 2);
    nevs_camera_coo (expr, Geo, Nodes, Mesh, pPrint, 3);

    return;
  }

  int var_qty = 4;
  char**  vars = ut_alloc_2d_char (var_qty, 15);
  double* vals = ut_alloc_1d (var_qty);
  int status;
  double* v = ut_alloc_1d (3);
  double* centre = ut_alloc_1d (3);
  v[0] = 3.462;
  v[1] =-5.770;
  v[2] = 4.327;

  if (expr != NULL)
  {
    if (! strcmp (expr, "centre+v"))
    {
      if (Nodes.NodeQty > 0)
	sprintf ((*pPrint).cameracooexpr[id], "meshcentre+v");
      else if (Geo.PolyQty > 0)
	sprintf ((*pPrint).cameracooexpr[id], "tesscentre+v");
    }
    else
      sprintf ((*pPrint).cameracooexpr[id], "%s", expr);
  }
  else if (Nodes.NodeQty > 0)
    sprintf ((*pPrint).cameracooexpr[id], "meshcentre+v");
  else if (Geo.PolyQty > 0)
    sprintf ((*pPrint).cameracooexpr[id], "tesscentre+v");

  sprintf (vars[0], "cameralookat");
  sprintf (vars[1], "v");
  sprintf (vars[2], "tesscentre");
  sprintf (vars[3], "meshcentre");

  vals[0] = (*pPrint).cameralookat[id - 1];
  vals[1] = v[id - 1];

  if (Geo.PolyQty > 0)
  {
    neut_geo_centre (Geo, centre);
    vals[2] = centre [id - 1];
  }

  if (Nodes.NodeQty > 0)
  {
    neut_mesh_centre (Nodes, Mesh, centre);
    vals[3] = centre [id - 1];
  }

  if (strcmp ((*pPrint).cameracooexpr[id], "meshcentre+v") == 0)
    (*pPrint).cameracoo[id - 1] = vals[3] + v[id - 1];
  else
  {
#ifdef HAVE_LIBMATHEVAL
    status = ut_math_eval ((*pPrint).cameracooexpr[id], var_qty, vars, vals,
			     &((*pPrint).cameracoo[id - 1]));
#else
    ut_print_messagewnc (2, 72, "This capability is not available because this version of Neper has not been compiled with libmatheval.");
    abort ();
#endif
    if (status == -1)
      abort ();
  }

  ut_free_2d_char (vars, var_qty);
  ut_free_1d (vals);
  ut_free_1d (v);
  ut_free_1d (centre);

  return;
}

void
nevs_camera_lookat (char* expr, struct GEO Geo, struct NODES Nodes,
                    struct MESH Mesh, struct PRINT* pPrint, int id)
{
  if (id < 1 || id > 3)
  {
    nevs_camera_lookat (expr, Geo, Nodes, Mesh, pPrint, 1);
    nevs_camera_lookat (expr, Geo, Nodes, Mesh, pPrint, 2);
    nevs_camera_lookat (expr, Geo, Nodes, Mesh, pPrint, 3);

    return;
  }

  int i, var_qty = 4;
  char**  vars = ut_alloc_2d_char (var_qty, 15);
  double* vals = ut_alloc_1d (var_qty);
  int status;
  double* centre = ut_alloc_1d (3);

  (*pPrint).cameralookatinit = 1;

  if (expr != NULL)
  {
    if (! strcmp (expr, "centre"))
    {
      if (Nodes.NodeQty > 0)
	sprintf ((*pPrint).cameralookatexpr[id], "meshcentre");
      else if (Geo.PolyQty > 0)
	sprintf ((*pPrint).cameralookatexpr[id], "tesscentre");
    }
    else
      sprintf ((*pPrint).cameralookatexpr[id], "%s", expr);
  }
  else if (Nodes.NodeQty > 0)
    sprintf ((*pPrint).cameralookatexpr[id], "meshcentre");
  else if (Geo.PolyQty > 0)
    sprintf ((*pPrint).cameralookatexpr[id], "tesscentre");

  sprintf (vars[0], "0");
  sprintf (vars[1], "O");
  sprintf (vars[2], "tesscentre");
  sprintf (vars[3], "meshcentre");

  vals[0] = 0;
  vals[1] = 0;

  if (Geo.PolyQty > 0)
  {
    neut_geo_centre (Geo, centre);
    vals[2] = centre [id - 1];
  }

  if (Nodes.NodeQty > 0)
  {
    neut_mesh_centre (Nodes, Mesh, centre);
    vals[3] = centre [id - 1];
  }
  
  int var_set = 0;
  for (i = 0; i < var_qty; i++)
    if (strcmp ((*pPrint).cameralookatexpr[id], vars[i]) == 0)
    {
      (*pPrint).cameralookat[id - 1] = vals[i];
      var_set = 1;
      break;
    }

  if (var_set == 0)
  {
#ifdef HAVE_LIBMATHEVAL
    status = ut_math_eval ((*pPrint).cameralookatexpr[id], var_qty, vars, vals,
			   &((*pPrint).cameralookat[id - 1]));
#else
    ut_print_messagewnc (2, 72, "This capability is not available because this version of Neper has not been compiled with libmatheval.");
    abort ();
#endif

    if (status == -1)
      abort ();
  }

  ut_free_2d_char (vars, var_qty);
  ut_free_1d (vals);
  ut_free_1d (centre);

  return;
}
