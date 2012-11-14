/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "GeoStuff.h"

/* Writing of the output data */
/*
void
WriteGeoStuff (struct IN In, struct GEO Geo, struct GEOPARA GeoPara)
{
  WriteGeo (In, Geo, GeoPara);
  
  WriteFoD (Geo, In.fod);

  return;
}
*/

void
WriteStatGeo (char* body, struct GEO Geo)
{
  FILE* file;
  char* name = NULL;
  
  name = ut_string_addextension (body, "stt0");

  file = ut_file_open (name, "w");
  WriteStatGeoVer (file, Geo);
  ut_file_close (file, name, "w");
  ut_free_1d_char (name);

  name = ut_string_addextension (body, "stt1");

  file = ut_file_open (name, "w");
  WriteStatGeoEdge (file, Geo);
  ut_file_close (file, name, "w");
  ut_free_1d_char (name);

  name = ut_string_addextension (body, "stt2");

  file = ut_file_open (name, "w");
  WriteStatGeoFace (file, Geo);
  ut_file_close (file, name, "w");
  ut_free_1d_char (name);

  name = ut_string_addextension (body, "stt3");

  file = ut_file_open (name, "w");
  WriteStatGeoPoly (file, Geo);
  ut_file_close (file, name, "w");
  ut_free_1d_char (name);

  return;
}
