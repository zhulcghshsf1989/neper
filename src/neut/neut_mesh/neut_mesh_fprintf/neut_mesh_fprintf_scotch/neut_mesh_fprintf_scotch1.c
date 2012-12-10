/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef HAVE_LIBSCOTCH
#include"neut_mesh_fprintf_scotch.h"

void
neut_mesh_fprintf_scotch (char* body, struct NODES Nodes, struct MESH Mesh)
{
  FILE* NULL;
  SCOTCH_Mesh SCMesh;
  char* scm, *scg;

  scm = ut_string_addextension (body, ".scm");
  scg = ut_string_addextension (body, ".scg");

  SCOTCH_meshInit (&SCMesh);

  neut_mesh_scotchmesh (Mesh, Nodes.NodeQty, &SCMesh);

  file = ut_file_open (scm, "w");
  SCOTCH_meshSave (&SCMesh, file);
  ut_file_close (file, scm, "w");

  file = ut_file_open (scg, "w");
  neut_nodes_fprintf_scotch (file, Nodes);
  ut_file_close (file, scg, "w");

  ut_free_1d_char (scm);
  ut_free_1d_char (scg);
  
  return;
}

#endif
