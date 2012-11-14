/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nevs_show.h"
#include<unistd.h>

void
nevs_show_init (
	   struct GEO  Geo   , struct NODES Nodes, struct MESH Mesh0D,
	   struct MESH Mesh1D, struct MESH Mesh2D, struct MESH  Mesh3D,
	   int SQty, struct PRINT *pPrint)
{
  /*
   * if ((*pPrint).showinit != 0)
    return;
    */

  if (SQty > 0 && (*pPrint).showslice == NULL)
  {
    (*pPrint).showslice = ut_alloc_1d_char (100);
    strcpy ((*pPrint).showslice, "all");

    if ((*pPrint).showmesh == -1)
      (*pPrint).showmesh = 0;
    if ((*pPrint).showtess == -1)
      (*pPrint).showtess = 0;
  }

  if (Mesh3D.EltQty > 0 && (*pPrint).showmesh == -1)
  {
    (*pPrint).showmesh = 1;
    if ((*pPrint).showtess == -1)
      (*pPrint).showtess = 0;
  }

  if (Geo.PolyQty > 0 && (*pPrint).showtess == -1)
    (*pPrint).showtess = 1;

  if ((*pPrint).showmesh == 1)
  {
    if ((*pPrint).showelt[0] == -1)
    {
      (*pPrint).showelt = ut_realloc_1d_int ((*pPrint).showelt, Mesh3D.EltQty + 1);
      (*pPrint).showelt[0] = Mesh3D.EltQty;
      ut_array_1d_int_set ((*pPrint).showelt + 1, Mesh3D.EltQty, 1);
    }
    
    if ((*pPrint).showelt2d[0] == -1)
    {
      (*pPrint).showelt2d = ut_realloc_1d_int ((*pPrint).showelt2d, Mesh2D.EltQty + 1);
      (*pPrint).showelt2d[0] = 0;
      ut_array_1d_int_set ((*pPrint).showelt2d + 1, Mesh2D.EltQty, 0);
    }
    
    if ((*pPrint).showelt1d[0] == -1)
    {
      (*pPrint).showelt1d = ut_realloc_1d_int ((*pPrint).showelt1d, Mesh1D.EltQty + 1);
      (*pPrint).showelt1d[0] = 0;
      ut_array_1d_int_set ((*pPrint).showelt1d + 1, Mesh1D.EltQty, 0);
    }
    
    if ((*pPrint).showelt0d[0] == -1)
    {
      (*pPrint).showelt0d = ut_realloc_1d_int ((*pPrint).showelt0d, Mesh0D.EltQty + 1);
      (*pPrint).showelt0d[0] = 0;
      ut_array_1d_int_set ((*pPrint).showelt0d + 1, Mesh0D.EltQty, 0);
    }
    
    if ((*pPrint).shownode[0] == -1)
    {
      (*pPrint).shownode = ut_realloc_1d_int ((*pPrint).shownode, Nodes.NodeQty + 1);
      (*pPrint).shownode[0] = 0;
      ut_array_1d_int_set ((*pPrint).shownode + 1, Nodes.NodeQty, 0);
    }
  }

  if ((*pPrint).showtess == 1)
  {
    if ((*pPrint).showpoly[0] == -1)
    {
      (*pPrint).showpoly = ut_realloc_1d_int ((*pPrint).showpoly, Geo.PolyQty + 1);
      (*pPrint).showpoly[0] = Geo.PolyQty;
      ut_array_1d_int_set ((*pPrint).showpoly + 1, Geo.PolyQty, 1);
    }
    
    if ((*pPrint).showface[0] == -1)
    {
      (*pPrint).showface = ut_realloc_1d_int ((*pPrint).showface, Geo.FaceQty + 1);
      (*pPrint).showface[0] = 0;
      ut_array_1d_int_set ((*pPrint).showface + 1, Geo.FaceQty, 0);
    }
    
    if ((*pPrint).showedge[0] == -1)
    {
      (*pPrint).showedge = ut_realloc_1d_int ((*pPrint).showedge, Geo.EdgeQty + 1);
      (*pPrint).showedge[0] = Geo.EdgeQty;
      ut_array_1d_int_set ((*pPrint).showedge + 1, Geo.EdgeQty, 1);
    }
    
    if ((*pPrint).showver[0] == -1)
    {
      (*pPrint).showver = ut_realloc_1d_int ((*pPrint).showver, Geo.VerQty + 1);
      (*pPrint).showver[0] = Geo.VerQty;
      ut_array_1d_int_set ((*pPrint).showver + 1, Geo.VerQty, 1);
    }
  }

  /*
  (*pPrint).showver =
    ut_realloc_1d_int ((*pPrint).showver, Geo.VerQty + 1);

  (*pPrint).showedge =
    ut_realloc_1d_int ((*pPrint).showedge, Geo.EdgeQty + 1);

  (*pPrint).showface =
    ut_realloc_1d_int ((*pPrint).showface, Geo.FaceQty + 1);

  (*pPrint).showpoly =
    ut_realloc_1d_int ((*pPrint).showpoly, Geo.PolyQty + 1);

  (*pPrint).shownode =
    ut_realloc_1d_int ((*pPrint).shownode, Nodes.NodeQty + 1);
  ut_array_1d_int_set ((*pPrint).shownode + 1, Nodes.NodeQty, 0);

  (*pPrint).showelt1d =
    ut_alloc_1d_int (Mesh1D.EltQty + 1);
  ut_array_1d_int_set ((*pPrint).showelt1d + 1, Mesh1D.EltQty, 0);
  
  (*pPrint).showelt2d =
    ut_realloc_1d_int ((*pPrint).showelt2d, Mesh2D.EltQty + 1);

  (*pPrint).showelt = ut_realloc_1d_int ((*pPrint).showelt, Mesh3D.EltQty + 1);
  ut_array_1d_int_set ((*pPrint).showelt + 1, Mesh3D.EltQty, 0);
  
  (*pPrint).showelt1dallqty = 0;

  if (Mesh3D.EltQty > 0 && (*pPrint).showelt[0] == -1)
  {
    ut_array_1d_int_set ((*pPrint).showelt + 1, Mesh3D.EltQty, 1);
    (*pPrint).showelt[0] = Mesh3D.EltQty;
  }
  else if (Geo.PolyQty > 0 && (*pPrint).showpoly[0] == -1)
  {
    ut_array_1d_int_set ((*pPrint).showpoly + 1, Geo.PolyQty, 1);
    (*pPrint).showpoly[0] = Geo.PolyQty;
    ut_array_1d_int_set ((*pPrint).showface + 1, Geo.FaceQty, 0);
    (*pPrint).showface[0] = 0;
    ut_array_1d_int_set ((*pPrint).showedge + 1, Geo.EdgeQty, 1);
    (*pPrint).showedge[0] = Geo.EdgeQty;
    ut_array_1d_int_set ((*pPrint).showver + 1, Geo.VerQty, 1);
    (*pPrint).showver[0] =  Geo.VerQty;
  }
  */

  // (*pPrint).showinit = 1;

  return;
}
