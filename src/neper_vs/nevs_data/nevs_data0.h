/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nevs_data (char**, int*, struct GEO, struct NODES, struct MESH, struct MESH, struct GEODATA*, struct MESHDATA*);

extern void nevs_data_init (struct GEO, struct GEODATA*, struct NODES,
    struct MESH, struct MESHDATA*);

#include "nevs_geodata/nevs_geodata0.h"
#include "nevs_meshdata/nevs_meshdata0.h"
