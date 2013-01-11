/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

/// \brief Returns 0 if the resulting tessellation is centroidal, 1
/// otherwise
///
///
///
extern int net_centroid (double centroidfact, double centroidconv, struct TESL Tesl,
                         struct GERMSET* pGermSet, double* prdistmax);
