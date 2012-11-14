/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "Res_t.h"

extern void
Res_t (struct IN In, struct GERMSET GermSet, struct TESS Tess,
       struct GEO Geo, int* id)
{
  FILE *file;
  /*
  int pos, qty;
  char **parts;
  int *PolyList = NULL;
  int *FaceList = NULL;
  int *EdgeList = NULL;
  int *VerList = NULL;
  */
  char *name = ut_alloc_1d_char (1000);
  char *ext = ut_alloc_1d_char (1000);
  
  if (ut_string_inlist (In.format, ',', "tess"))
  {
    if (Geo.PolyQty != 0)
    {
      file = ut_file_open (In.geo, "w");
      neut_geo_fprintf (file, Geo);
      ut_file_close (file, In.geo, "w");
    }
    else
      ut_print_message (1, 0, "Geo is void; cannot export; skipping\n");
  }

  if (ut_string_inlist (In.format, ',', "geo")) // gmsh geo file
  {
    if (Geo.PolyQty != 0)
    {
      file = ut_file_open (In.gmshgeo, "w");
      neut_geo_fprintf_gmsh (file, Geo);
      ut_file_close (file, In.gmshgeo, "w");
    }
    else
      ut_print_message (1, 0, "Geo is void; cannot export; skipping\n");
  }

  if (ut_string_inlist (In.format, ',', "ply")) // gmsh ply file
  {
    if (Geo.PolyQty != 0)
    {
      file = ut_file_open (In.ply, "w");
      neut_geo_fprintf_ply (file, Geo);
      ut_file_close (file, In.ply, "w");
    }
    else
      ut_print_message (1, 0, "Geo is void; cannot export; skipping\n");
  }

  /* 
  if (strcmp (In.printentity, "none") != 0)
  {
    if (strlen (In.printformat) > 0)
    {
      ut_string_separate (In.printformat, ',', &parts, &qty);
      PrintGeoStuff (In.printentity, In.printlist, Geo, &PolyList,
	  &FaceList, &EdgeList, &VerList);
      pos = 1;
      while (ut_string_part (In.printformat, ',', pos++, ext))
      {
	name = ut_string_addextension (In.body, ext);
	PrintGeoAsImg (Geo, PolyList, FaceList, EdgeList, VerList, ext, name);
      }
    }
  }
  */

  if (In.printpointpoly)
    PrintPointPoly (In, Geo);

  if (In.printneighbour)
    PrintConnectivity (In, Tess);

  if (In.stattess)
    WriteStatGeo (In.body, Geo);

  if (ut_string_inlist (In.format, ',', "oin"))
  {
    file = ut_file_open (In.oin, "w");
    net_oin_fprintf (file, GermSet, id);
    ut_file_close (file, In.oin, "w");
  }

  if (ut_string_inlist (In.format, ',', "debug"))
  {
    file = ut_file_open (In.debug, "w");
    neut_debug_geo (file, Geo);
    ut_file_close (file, In.debug, "w");
  }

  ut_free_1d_char (name);
  ut_free_1d_char (ext);

  return;
}
