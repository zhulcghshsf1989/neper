/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_stat.h"

void
net_stat (struct IN In, struct TESS net_poly_tesl)
{
  FILE* file = NULL;
  char* name = NULL;
  
  if (In.stv != NULL)
  {
    name = ut_string_addextension (In.body, "stver");

    file = ut_file_open (name, "w");
    WriteStatTessVer (file, In.stv, net_poly_tesl);
    ut_file_close (file, name, "w");
    ut_free_1d_char (name);
  }

  if (In.ste != NULL)
  {
    name = ut_string_addextension (In.body, "stedge");

    file = ut_file_open (name, "w");
    WriteStatTessEdge (file, In.ste, net_poly_tesl);
    ut_file_close (file, name, "w");
    ut_free_1d_char (name);
  }

  if (In.stf != NULL)
  {
    name = ut_string_addextension (In.body, "stface");

    file = ut_file_open (name, "w");
    WriteStatTessFace (file, In.stf, net_poly_tesl);
    ut_file_close (file, name, "w");
    ut_free_1d_char (name);
  }

  if (In.stp != NULL)
  {
    name = ut_string_addextension (In.body, "stpoly");

    file = ut_file_open (name, "w");
    WriteStatTessPoly (file, In.stp, net_poly_tesl);
    ut_file_close (file, name, "w");
    ut_free_1d_char (name);
  }

  return;
}
