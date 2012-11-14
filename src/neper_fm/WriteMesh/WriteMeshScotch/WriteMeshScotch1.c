/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef HAVE_LIBSCOTCH
#include"WriteMeshScotch.h"

void
WriteMeshScotch (struct IN In, struct NODES Nodes, struct MESH Mesh)
{
  FILE* scm, *scg;
  SCOTCH_Mesh SCMesh;

  SCOTCH_meshInit (&SCMesh);

  neut_mesh_scotchmesh (Mesh, Nodes.NodeQty, &SCMesh);

  scm = ut_file_open (In.scm, "w");
  SCOTCH_meshSave (&SCMesh, scm);
  ut_file_close (scm, In.scm, "w");

  scg = ut_file_open (In.scg, "w");
  neut_nodes_fprintf_scotch (scg, Nodes);
  ut_file_close (scg, In.scg, "w");
  
  return;
}

#endif
