/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nevs_data (char**, int*, struct TESS, struct NODES, struct MESH, struct MESH, struct TESSDATA*, struct MESHDATA*);

extern void nevs_data_init (struct MTESS, struct TESS*, int, struct TESSDATA*,
    struct TESSDATA**, struct NODES, struct MESH, struct MESHDATA*);

#include "nevs_tessdata/nevs_tessdata0.h"
#include "nevs_meshdata/nevs_meshdata0.h"
