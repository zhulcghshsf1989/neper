/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_geo_fscanf_lcl.h"

void
neut_geo_fscanf (FILE* file, struct GEO* pGeo)
{
  char* version = ut_alloc_1d_char (10);

  if ((*pGeo).PolyQty > 0)
    neut_geo_free (pGeo);

  neut_geo_fscanf_version (file, version);

  if (! strcmp (version, "1.9.2"))
  {
    neut_geo_fscanf_1p9 (file, pGeo);
    (*pGeo).N = (*pGeo).PolyQty;
    (*pGeo).DomType = ut_alloc_1d_char (5);
    strcpy ((*pGeo).DomType, "cube");
    (*pGeo).version = ut_alloc_1d_char (5);
    strcpy ((*pGeo).version, "1.10");
    neut_geo_init_domain (pGeo);

    ut_free_1d_char (version);
    return;
  }

  neut_geo_fscanf_head (pGeo, file);
  neut_geo_fscanf_ver (pGeo, file);
  neut_geo_fscanf_edge (pGeo, file);
  neut_geo_fscanf_face (pGeo, file);
  neut_geo_fscanf_poly (pGeo, file);
  neut_geo_fscanf_domain (pGeo, file);
  neut_geo_fscanf_foot (file);

  sprintf ((*pGeo).version, "1.10"); // new at format 1.10
  neut_geo_init_edgelength (pGeo);

  if (neut_geo_test (*pGeo) != 0)
  {
    ut_print_message (2, 0, "Tessellation is not valid (testing failed).\n");
    abort ();
  }

  ut_free_1d_char (version);

  return;
}

void
neut_geo_name_fscanf (char* name, struct GEO* pGeo)
{
  FILE* file = ut_file_open (name, "r");
  neut_geo_fscanf (file, pGeo);
  ut_file_close (file, name, "r");

  return;
}
