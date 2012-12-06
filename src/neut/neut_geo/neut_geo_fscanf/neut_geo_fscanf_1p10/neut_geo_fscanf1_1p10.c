/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_geo_fscanf_lcl_1p10.h"

void
neut_geo_fscanf_1p10 (FILE* file, struct GEO* pGeo)
{
  neut_geo_fscanf_verbosity_1p10 (file, pGeo, 0);
}

void
neut_geo_fscanf_verbosity_1p10 (FILE* file, struct GEO* pGeo, int verbosity)
{
  char* version = ut_alloc_1d_char (10);

  if ((*pGeo).PolyQty > 0)
    neut_geo_free (pGeo);

  neut_geo_fscanf_version_1p10 (file, version);
  neut_geo_fscanf_head_1p10 (pGeo, file);
  neut_geo_fscanf_ver_1p10 (pGeo, file);
  neut_geo_fscanf_edge_1p10 (pGeo, file);
  neut_geo_fscanf_face_1p10 (pGeo, file);
  neut_geo_fscanf_poly_1p10 (pGeo, file);
  neut_geo_fscanf_domain_1p10 (pGeo, file);
  neut_geo_init_domain_facelabel (pGeo);
  neut_geo_fscanf_foot_1p10 (file);

  sprintf ((*pGeo).version, "2.0"); // new at format 1.10
  neut_geo_init_edgelength (pGeo);

  if (neut_geo_test (*pGeo, verbosity) != 0)
  {
    ut_print_message (2, 0, "Tessellation checking failed - tessellation is not valid.\n");
    if (! verbosity)
      ut_print_message (2, 0, "Debug with option -checktess of neper -T.\n");

    abort ();
  }
  else
    if (verbosity) 
      ut_print_message (2, 0, "Tessellation checking succeeded.\n");

  ut_free_1d_char (version);

  return;
}

void
neut_geo_name_fscanf_1p10 (char* name, struct GEO* pGeo)
{
  FILE* file = ut_file_open (name, "r");
  neut_geo_fscanf_1p10 (file, pGeo);
  ut_file_close (file, name, "r");

  return;
}
