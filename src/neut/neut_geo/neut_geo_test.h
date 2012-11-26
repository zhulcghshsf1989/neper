/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef  GEOTEST_H
#define  GEOTEST_H

#include "neut_structs.h"

extern int neut_geo_test (struct GEO, int);
extern int neut_geo_testAroundVer (struct GEO, int, int);
extern int neut_geo_testAroundVer2 (struct GEO, int, int);
extern int neut_geo_test_face_normal (struct GEO, int, int);
extern int neut_geo_test_dom (struct GEO, int);

#endif  /* GEOTEST_H */
