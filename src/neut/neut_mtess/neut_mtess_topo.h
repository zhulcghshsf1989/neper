/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef NEUT_MTESS_TOPO_H
#define NEUT_MTESS_TOPO_H

#include"../neut_structs.h"

/// \brief Get the domain at a given level of the poly of a tess
///
///
///
extern int neut_mtess_tess_level_dom (struct MTESS MTess, struct TESS
    Tess, int level, int* pdom);

#endif /* NEUT_MTESS_TOPO_H */
