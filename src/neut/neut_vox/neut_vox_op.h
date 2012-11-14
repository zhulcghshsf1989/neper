/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef  NEUT_VOX_OP_H
#define  NEUT_VOX_OP_H

/// \brief Set a VOX structure to zero
///
///
///
extern void neut_vox_set_zero (struct VOX * pVox);

/// \brief Free a VOX structure
///
///
///
extern void neut_vox_free (struct VOX* pVox);

/// \brief Copy a VOX structure into another VOX structure
///
///
///
extern void neut_vox_memcpy (struct VOX Vox1, struct VOX* pVox2);

/// \brief Copy the parameters of a VOX structure into another VOX structure
///
///
///
extern void neut_vox_memcpy_parms (struct VOX Vox1, struct VOX* pVox2);

extern int neut_vox_expr_polys (struct VOX Vox, char* expr, int** ppoly, int* ppolyqty);

/// \brief Determine the bounding box of a VOX.
///
///
///
extern void neut_vox_boundingbox (struct VOX Vox, double** bbox);

/// \brief Scale (= stretch) a VOX structure.
///
///
///
extern void neut_vox_scale (struct VOX* pVox, double scale1, double
    scale2, double scale3, char* method);

/// \brief Scale the grid of a VOX structure.
///
///
///
extern void
neut_vox_gridscale (struct VOX* pVox, double scale1, double scale2, double scale3);

/// \brief Return the neighbouring voxels of a given voxel, within a certain polyhedron.
///
///
///
extern void
neut_vox_poly_vox_neighvox (struct VOX, int poly, int* pos, int type, int*** ppos, int* pqty);

/// \brief Return the neighbouring voxels of a given voxel, within certain polyhedra.  If poly == NULL, then all polyhedra (id different from 0) are considered.
///
/// type = 1: 26 neighbours, type = 2: 6 neighbours
///
extern void
neut_vox_polys_vox_neighvox (struct VOX Vox, int* poly, int polyqty, int* pos,
                             int type, int*** ppos, int* pqty);

/// \brief Segment a VOX structure.
///
/// type = 1: 26 neighbours, type = 2: 6 neighbours
///
extern void
neut_vox_segment (struct VOX Vox, int type, struct VOX* pSeg);

/// \brief Clean a VOX structure.
///
/// type = 1: 26 neighbours, type = 2: 6 neighbours
///
extern void
neut_vox_clean (struct VOX Vox, int type, struct VOX* pVox2, int*, int*);

/// \brief Get the voxel quantity of the polys of a VOX structure.
///
///
///
extern void
neut_vox_polys_voxqty (struct VOX, int**);

/// \brief Copy the poly of a VOX structure into another VOX structure
///
///
///
extern void
neut_vox_poly_vox (struct VOX Vox, int poly, struct VOX* PolyVox);

#endif /* NEUT_VOX_OP_H */
