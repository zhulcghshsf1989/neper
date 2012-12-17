/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef NEUT_MESH_PART_H
#define NEUT_MESH_PART_H

#include"../neut_structs/neut_mesh_struct.h"

/// \brief Get the communications of an elt.
///
/// This function returns the communications of an element. *pcomqty is
/// the number of partitions with which there are communications;
/// for each partition i, coms[i][0] is the number of the partition and
/// coms[i][1] the numbers of communications with it.
///
extern int
neut_mesh_elt_coms (struct MESH Mesh, int elt, int partqty, int *parttab,
		    int *parttab2, int **coms, int *pcomqty);

/// \brief Get the number of communications of an elt.
///
/// 
///
extern int
neut_mesh_elt_comqty (struct MESH Mesh, int elt, int *parttab, int
    *parttab2);

/// \brief Test if an elt has nodes shared between several partitions.
///
/// 
///
extern int neut_mesh_elt_isbound (struct MESH Mesh, int elt, int
    *parttab);

#endif /* NEUT_MESH_PART_H */
