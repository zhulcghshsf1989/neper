/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nevs_show.h"
#include<unistd.h>

void
nevs_show_init (struct MTESS MTess,
	   struct TESS Tess, struct NODES Nodes, struct MESH Mesh0D,
	   struct MESH Mesh1D, struct MESH Mesh2D, struct MESH  Mesh3D,
	   int SQty, struct PRINT* pPrint)
{
  /*
   * if ((*pPrint).showinit != 0)
    return;
    */
  if ((*pPrint).showtesslevel == -1)
   (*pPrint).showtesslevel = MTess.LevelQty;

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

  if (Tess.PolyQty > 0 && (*pPrint).showtess == -1)
    (*pPrint).showtess = 1;

  if ((*pPrint).showmesh == 1)
  {
    if ((*pPrint).showelt[0] == -1)
    {
      (*pPrint).showelt = ut_realloc_1d_int ((*pPrint).showelt, Mesh3D.EltQty + 1);
      (*pPrint).showelt[0] = Mesh3D.EltQty;
      ut_array_1d_int_set ((*pPrint).showelt + 1, Mesh3D.EltQty, 1);
    }

    if ((*pPrint).showeltedge[0] == -1)
    {
      (*pPrint).showeltedge = ut_realloc_1d_int ((*pPrint).showeltedge, Mesh3D.EltQty + 1);
      (*pPrint).showeltedge[0] = Mesh3D.EltQty;
      ut_array_1d_int_set ((*pPrint).showeltedge + 1, Mesh3D.EltQty, 1);
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
      (*pPrint).showpoly = ut_realloc_1d_int ((*pPrint).showpoly, Tess.PolyQty + 1);
      (*pPrint).showpoly[0] = Tess.PolyQty;
      ut_array_1d_int_set ((*pPrint).showpoly + 1, Tess.PolyQty, 1);
    }

    if ((*pPrint).showface[0] == -1)
    {
      (*pPrint).showface = ut_realloc_1d_int ((*pPrint).showface, Tess.FaceQty + 1);
      (*pPrint).showface[0] = 0;
      ut_array_1d_int_set ((*pPrint).showface + 1, Tess.FaceQty, 0);
    }

    if ((*pPrint).showedge[0] == -1)
    {
      (*pPrint).showedge = ut_realloc_1d_int ((*pPrint).showedge, Tess.EdgeQty + 1);
      (*pPrint).showedge[0] = Tess.EdgeQty;
      ut_array_1d_int_set ((*pPrint).showedge + 1, Tess.EdgeQty, 1);
    }

    if ((*pPrint).showver[0] == -1)
    {
      (*pPrint).showver = ut_realloc_1d_int ((*pPrint).showver, Tess.VerQty + 1);
      (*pPrint).showver[0] = Tess.VerQty;
      ut_array_1d_int_set ((*pPrint).showver + 1, Tess.VerQty, 1);
    }
  }

  /*
  (*pPrint).showver =
    ut_realloc_1d_int ((*pPrint).showver, Tess.VerQty + 1);

  (*pPrint).showedge =
    ut_realloc_1d_int ((*pPrint).showedge, Tess.EdgeQty + 1);

  (*pPrint).showface =
    ut_realloc_1d_int ((*pPrint).showface, Tess.FaceQty + 1);

  (*pPrint).showpoly =
    ut_realloc_1d_int ((*pPrint).showpoly, Tess.PolyQty + 1);

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
  else if (Tess.PolyQty > 0 && (*pPrint).showpoly[0] == -1)
  {
    ut_array_1d_int_set ((*pPrint).showpoly + 1, Tess.PolyQty, 1);
    (*pPrint).showpoly[0] = Tess.PolyQty;
    ut_array_1d_int_set ((*pPrint).showface + 1, Tess.FaceQty, 0);
    (*pPrint).showface[0] = 0;
    ut_array_1d_int_set ((*pPrint).showedge + 1, Tess.EdgeQty, 1);
    (*pPrint).showedge[0] = Tess.EdgeQty;
    ut_array_1d_int_set ((*pPrint).showver + 1, Tess.VerQty, 1);
    (*pPrint).showver[0] =  Tess.VerQty;
  }
  */

  // (*pPrint).showinit = 1;

  return;
}

void
nevs_show_print_mprint (struct MTESS MTess, struct TESS* Tess,
                        struct PRINT Print, struct PRINT** pMPrint)
{
  int i, id;
  int* dom = ut_alloc_1d_int (2);
  struct PRINT* pPrint = NULL; // shortcut

  (*pMPrint) = calloc (MTess.TessQty + 1, sizeof (struct PRINT));

  int level = Print.showtesslevel;
  for (i = 1; i <= MTess.LevelTessQty[level]; i++)
  {
    id = MTess.LevelTess[level][i];
    neut_mtess_tess_level_dom (MTess, Tess[id], 1, dom);

    pPrint = &((*pMPrint)[id]);

    neut_print_set_default (pPrint);

    (*pPrint).showpoly = ut_alloc_1d_int (Tess[id].PolyQty + 1);
    ut_array_1d_int_set ((*pPrint).showpoly + 1, Tess[id].PolyQty, Print.showpoly[dom[1]]);
    (*pPrint).showpoly[0]
      = ut_array_1d_int_sum ((*pPrint).showpoly + 1, Tess[id].PolyQty);

    (*pPrint).showface = ut_alloc_1d_int (Tess[id].FaceQty + 1);
    ut_array_1d_int_set ((*pPrint).showface + 1, Tess[id].FaceQty, 0);

    (*pPrint).showedge = ut_alloc_1d_int (Tess[id].EdgeQty + 1);
    // Showing edges if the poly is shown
    ut_array_1d_int_set ((*pPrint).showedge + 1, Tess[id].EdgeQty,
	Print.showpoly[dom[1]]);

    (*pPrint).showver = ut_alloc_1d_int (Tess[id].VerQty + 1);
    ut_array_1d_int_set ((*pPrint).showver + 1, Tess[id].VerQty, 0);

    (*pPrint).showshadow = Print.showshadow;
  }

  ut_free_1d_int (dom);

  return;
}
