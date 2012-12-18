/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"../../structIn_t.h"
#include"ut.h"
#include"orilib.h"
#include"neut_t.h"

#include"net_stat0.h"

extern void WriteStatTessVer (FILE*, char*, struct TESS);
extern void WriteStatTessEdge (FILE*, char*, struct TESS);
extern void WriteStatTessFace (FILE*, char*, struct TESS);
extern void WriteStatTessPoly (FILE*, char*, struct TESS);
