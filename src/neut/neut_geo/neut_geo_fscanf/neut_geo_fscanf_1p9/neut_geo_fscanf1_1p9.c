/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_geo_fscanf_1p9.h"

void
neut_geo_fscanf_1p9 (FILE* file, struct GEO* pGeo)
{
  if ((*pGeo).PolyQty > 0)
    neut_geo_free (pGeo);

  neut_geo_fscanf_head_1p9 (pGeo, file);
  neut_geo_fscanf_ver_1p9 (pGeo, file);
  neut_geo_fscanf_edge_1p9 (pGeo, file);
  neut_geo_fscanf_face_1p9 (pGeo, file);
  neut_geo_fscanf_poly_1p9 (pGeo, file);
  neut_geo_fscanf_foot_1p9 (file);

  neut_geo_init_edgelength (pGeo);

  return;
}

void
neut_geo_name_fscanf_1p9 (char* name, struct GEO* pGeo)
{
  FILE* file = ut_file_open (name, "r");
  neut_geo_fscanf_1p9 (file, pGeo);
  ut_file_close (file, name, "r");

  return;
}
