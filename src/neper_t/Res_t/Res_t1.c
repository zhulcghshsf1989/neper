/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "Res_t.h"

extern void
Res_t (struct IN In, struct TESS Tess, struct VOX Vox)
{
  FILE* file = NULL;
  
  if (ut_string_inlist (In.format, ',', "tess"))
  {
    if (Tess.PolyQty != 0)
    {
      file = ut_file_open (In.tess, "w");
      neut_tess_fprintf (file, Tess);
      ut_file_close (file, In.tess, "w");
    }
    else
      ut_print_message (1, 2, "Cannot write %s (is void) - skipping.\n", In.tess);
  }

  if (ut_string_inlist (In.format, ',', "geo")) // geo file
  {
    if (Tess.PolyQty != 0)
    {
      file = ut_file_open (In.geo, "w");
      neut_tess_fprintf_gmsh (file, Tess);
      ut_file_close (file, In.geo, "w");
    }
    else
      ut_print_message (1, 2, "Cannot write %s (is void) - skipping.\n", In.geo);
  }

  if (ut_string_inlist (In.format, ',', "ply")) // gmsh ply file
  {
    if (Tess.PolyQty != 0)
    {
      file = ut_file_open (In.ply, "w");
      neut_tess_fprintf_ply (file, Tess);
      ut_file_close (file, In.ply, "w");
    }
    else
      ut_print_message (1, 2, "Cannot write %s (is void) - skipping.\n", In.ply);
  }

  if (ut_string_inlist (In.format, ',', "3dec")) // gmsh ply file
  {
    if (Tess.PolyQty != 0)
    {
      file = ut_file_open (In.dec, "w");
      neut_tess_fprintf_dec (file, Tess);
      ut_file_close (file, In.dec, "w");
    }
    else
      ut_print_message (1, 0, "Tess is void; cannot export; skipping\n");
  }

  if (ut_string_inlist (In.format, ',', "vox")) // vox file
  {
    if (Vox.PolyQty != 0)
    {
      file = ut_file_open (In.vox, "w");
      neut_vox_fprintf (file, In.voxformat, Vox);
      ut_file_close (file, In.vox, "w");
    }
    else
      ut_print_message (1, 2, "Cannot write %s (is void) - skipping.\n", In.vox);
  }

  if (In.printpointpoly)
    PrintPointPoly (In, Tess);

  net_stat (In, Tess);

  if (ut_string_inlist (In.format, ',', "debug"))
  {
    file = ut_file_open (In.debug, "w");
    neut_debug_tess (file, Tess);
    ut_file_close (file, In.debug, "w");
  }

  return;
}
