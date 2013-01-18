/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_writemesh.h"

void
nem_writemesh (struct IN In, struct TESS Tess, 
           struct NODES Nodes,
	   struct MESH* pMesh0D, struct MESH* pMesh1D,
	   struct MESH* pMesh2D, struct MESH* pMesh3D,
	   struct NSET NSet0D, struct NSET NSet1D,
	   struct NSET NSet2D, struct PART Part)
{
  double voltot;
  FILE *file;
  char* expandnset = NULL;
  char* expandfaset = NULL;

  neut_nset_expand (NSet0D, NSet1D, NSet2D, In.nset, &expandnset);
  neut_nset_expand (NSet0D, NSet1D, NSet2D, In.faset, &expandfaset);

  if ((ut_string_inlist (In.outdim, ',', "3") && (*pMesh3D).EltElset == NULL)
   || (ut_string_inlist (In.outdim, ',', "2") && (*pMesh2D).EltElset == NULL)
   || (ut_string_inlist (In.outdim, ',', "1") && (*pMesh1D).EltElset == NULL)
   || (ut_string_inlist (In.outdim, ',', "0") && (*pMesh0D).EltElset == NULL))
  {
    ut_print_message (0, 2, "Preparing mesh ...\n");

    if (ut_string_inlist (In.outdim, ',', "3") && (*pMesh3D).EltElset == NULL)
      neut_mesh_init_eltelset (pMesh3D, NULL);
  
    if (ut_string_inlist (In.outdim, ',', "2") && (*pMesh2D).EltElset == NULL)
      neut_mesh_init_eltelset (pMesh2D, NULL);
  
    if (ut_string_inlist (In.outdim, ',', "1") && (*pMesh1D).EltElset == NULL)
      neut_mesh_init_eltelset (pMesh1D, NULL);
  
    if (ut_string_inlist (In.outdim, ',', "0") && (*pMesh0D).EltElset == NULL)
      neut_mesh_init_eltelset (pMesh0D, NULL);
  }
  
  ut_print_message (0, 2, "Mesh properties:\n");

  if (pMesh3D != NULL && (*pMesh3D).EltQty != 0)
  {
    ut_print_message (0, 3, "Node number: %8d\n", Nodes.NodeQty);
    ut_print_message (0, 3, "Elt  number: %8d\n", (*pMesh3D).EltQty);
    neut_mesh_volume (Nodes, *pMesh3D, &voltot);
    ut_print_message (0, 3, "Mesh volume: %8.3f\n", voltot);
  }

  if (strlen (In.format) > 0)
    ut_print_message (0, 2, "Writing mesh ...\n");

  if (ut_string_inlist (In.format, ',', "msh"))
  {
    if (ut_string_inlist (In.outdim, ',', "3") == 1 && (*pMesh3D).EltType == NULL)
      ut_print_message (1, 3, "3D mesh is void.\n");
    if (ut_string_inlist (In.outdim, ',', "2") == 1 && (*pMesh2D).EltType == NULL)
      ut_print_message (1, 3, "2D mesh is void.\n");
    if (ut_string_inlist (In.outdim, ',', "1") == 1 && (*pMesh1D).EltType == NULL)
      ut_print_message (1, 3, "1D mesh is void.\n");
    if (ut_string_inlist (In.outdim, ',', "0") == 1 && (*pMesh0D).EltType == NULL)
      ut_print_message (1, 3, "1D mesh is void.\n");

    file = ut_file_open (In.msh, "w");
    neut_mesh_fprintf_gmsh (file, In.outdim, Nodes, *pMesh0D, *pMesh1D,
	*pMesh2D, *pMesh3D, Part);
    ut_file_close (file, In.msh, "w");
  }

  if (ut_string_inlist (In.format, ',', "abq")
   || ut_string_inlist (In.format, ',', "inp"))
  {
    file = ut_file_open (In.abq, "w");
    neut_mesh_fprintf_inp (file, In.outdim, Nodes, *pMesh0D, *pMesh1D, *pMesh2D,
	*pMesh3D, NSet0D, NSet1D, NSet2D, expandnset);
    ut_file_close (file, In.abq, "w");
  }

  if (ut_string_inlist (In.format, ',', "geof"))
  {
    ut_alloc_1d_int ((*pMesh3D).EltQty + 1);

    file = ut_file_open (In.geof, "w");
    neut_mesh_fprintf_geof (file, Nodes, (*pMesh2D), (*pMesh3D), 
	NSet0D, NSet1D, NSet2D, expandnset, 0, In.partsets, Part);
    ut_file_close (file, In.geof, "w");
  }

  if (ut_string_inlist (In.format, ',', "fepx")
   || ut_string_inlist (In.format, ',', "parms")
   || ut_string_inlist (In.format, ',', "mesh")
   || ut_string_inlist (In.format, ',', "surf")
   || ut_string_inlist (In.format, ',', "opt")
   || ut_string_inlist (In.format, ',', "bcs"))
    neut_mesh_fprintf_fepx_name (In.format, In.body, Tess, Nodes, (*pMesh2D), (*pMesh3D),
			    NSet0D, NSet1D, NSet2D, expandnset, expandfaset);

  /*
  if (ut_string_inlist (In.format, ',', "stellar"))
  {
    qty = 0;
    for (i = 1; i <= 6; i++)
      qty += NSets.nodes[i][0];

    int* BoundNodes = NULL;
    BoundNodes = ut_alloc_1d_int (qty + 1);
    for (i = 1; i <= 6; i++)
      ut_array_1d_int_memcpy (BoundNodes + 1 + BoundNodes[0], NSets.nodes[i][0], NSets.nodes[i] + 1);

    nem_writemeshStellar (In, Nodes, BoundNodes, (*pMesh3D));
    ut_free_1d_int (BoundNodes);
  }
  */

#ifdef HAVE_LIBSCOTCH
  if (In.partmode != -1)
  {
    ut_print_message (0, 2, "Writing partitioning data...\n");
    neut_part_fprintf (In.body, Part);
  }
#endif

  ut_free_1d_char (expandnset);
  ut_free_1d_char (expandfaset);

  return;
}
