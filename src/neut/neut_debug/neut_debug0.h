/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef NEUT_DEBUG0_H
#define NEUT_DEBUG0_H

#include"../neut_structs.h"

extern void neut_debug_nodes (FILE*, struct NODES);
extern void neut_debug_mesh (FILE*, struct MESH);
extern void neut_debug_nset (FILE*, struct NSET);
extern void neut_debug_fod (FILE*, int**);
extern void neut_debug_fodnodes (FILE*, int**);
extern void neut_debug_geo (FILE*, struct GEO);
extern void neut_debug_germset (FILE*, struct GERMSET);
extern void neut_debug_geodata (FILE*, struct GEODATA);
extern void neut_debug_poly (FILE*, struct POLY);
extern void neut_debug_polymod (FILE*, struct POLYMOD);

#endif /* NEUT_DEBUG0_H */
