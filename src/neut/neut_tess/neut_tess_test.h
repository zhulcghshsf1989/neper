/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef  TESSTEST_H
#define  TESSTEST_H

#include "neut_structs.h"

extern int neut_tess_test (struct TESS, int);
extern int neut_tess_testAroundVer (struct TESS, int, int);
extern int neut_tess_testAroundVer2 (struct TESS, int, int);
extern int neut_tess_test_face_normal (struct TESS, int, int);
extern int neut_tess_test_dom (struct TESS, int);

#endif  /* TESSTEST_H */
