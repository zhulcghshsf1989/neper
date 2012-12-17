/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fprintf_gmsh_lcl.h"

void
neut_mesh_fprintf_gmsh (FILE* file, char* dim, struct NODES Nodes,
    struct MESH Mesh0D, struct MESH Mesh1D, struct MESH Mesh2D,
    struct MESH Mesh3D, 
    struct PART Part)
{
  neut_meshheader_fprintf_gmsh (file);

  neut_nodes_fprintf_gmsh (file, Nodes);

  neut_elts_fprintf_gmsh (file, Mesh0D, Mesh1D, Mesh2D, Mesh3D, Part, dim);

/*----------------------------------------------------------------------
 * node sets */

//  if (nset != NULL)
//  {
//    fprintf (file, "\n/* node sets\n");
//    neut_mesh_fprintf_gmsh_nsets (file, nset, NSets, NSetNames);
//    fprintf (file, "\n*/\n");
//  }
  
  return;
}

void
neut_meshheader_fprintf_gmsh (FILE* file)
{
  fprintf (file, "$MeshFormat\n");
  fprintf (file, "2 0 8\n");
  fprintf (file, "$Comments\n");
  fprintf (file, "contiguous\n");
  fprintf (file, "$EndComments\n");
  fprintf (file, "$EndMeshFormat\n");

  return;
}


void
neut_elts_fprintf_gmsh (FILE* file, struct MESH Mesh0D,
                        struct MESH Mesh1D, struct MESH Mesh2D,
			struct MESH Mesh3D, struct PART Part,
			char* dim)
{
  int i, id, elt_type0D, elt_type1D, elt_type2D, elt_type3D;
  int eltnodeqty1D, eltnodeqty2D, eltnodeqty3D;

  eltnodeqty1D = neut_elt_nodeqty (Mesh1D.EltType, Mesh1D.Dimension, Mesh1D.EltOrder);
  eltnodeqty2D = neut_elt_nodeqty (Mesh2D.EltType, Mesh2D.Dimension, Mesh2D.EltOrder);
  eltnodeqty3D = neut_elt_nodeqty (Mesh3D.EltType, Mesh3D.Dimension, Mesh3D.EltOrder);

  if (Mesh0D.EltQty != 0 && neut_gmsh_elt_nb (Mesh0D.EltType, Mesh0D.Dimension,
	Mesh0D.EltOrder, &elt_type0D) != 0)
    ut_error_reportbug ();

  if (Mesh1D.EltQty != 0 && neut_gmsh_elt_nb (Mesh1D.EltType, Mesh1D.Dimension,
	Mesh1D.EltOrder, &elt_type1D) != 0)
    ut_error_reportbug ();

  if (Mesh2D.EltQty != 0 && neut_gmsh_elt_nb (Mesh2D.EltType, Mesh2D.Dimension,
	Mesh2D.EltOrder, &elt_type2D) != 0)
    ut_error_reportbug ();

  if (Mesh3D.EltQty != 0 && neut_gmsh_elt_nb (Mesh3D.EltType, Mesh3D.Dimension,
	Mesh3D.EltOrder, &elt_type3D) != 0)
    ut_error_reportbug ();


  fprintf (file, "$Elements\n");
  int eltqty = 0;
  if (ut_string_inlist (dim, ',', "0"))
    eltqty += Mesh0D.EltQty;
  if (ut_string_inlist (dim, ',', "1"))
    eltqty += Mesh1D.EltQty;
  if (ut_string_inlist (dim, ',', "2"))
    eltqty += Mesh2D.EltQty;
  if (ut_string_inlist (dim, ',', "3"))
    eltqty += Mesh3D.EltQty;

  fprintf (file, "%d\n", eltqty);

  id = 1;

  // 0D mesh
  if (ut_string_inlist (dim, ',', "0"))
  {
    if (Mesh0D.EltElset == NULL)
      neut_mesh_init_eltelset (&Mesh0D, NULL);
    for (i = 1; i <= Mesh0D.EltQty; i++)
    {
      fprintf (file, "%d %d 3 0 %d 0 ", id++, elt_type0D, Mesh0D.EltElset[i]);
      ut_array_1d_int_fprintf (file, Mesh0D.EltNodes[i], 1, "%d");
    }
  }

  // 1D mesh
  if (ut_string_inlist (dim, ',', "1"))
  {
    if (Mesh1D.EltElset == NULL)
      neut_mesh_init_eltelset (&Mesh1D, NULL);

    for (i = 1; i <= Mesh1D.EltQty; i++)
    {
      fprintf (file, "%d %d 3 0 %d 0 ", id++, elt_type1D, Mesh1D.EltElset[i]);
      ut_array_1d_int_fprintf (file, Mesh1D.EltNodes[i], eltnodeqty1D, "%d");
    }
  }

  // 2D mesh
  if (ut_string_inlist (dim, ',', "2"))
  {
    if (Mesh2D.EltElset == NULL)
      neut_mesh_init_eltelset (&Mesh2D, NULL);

    for (i = 1; i <= Mesh2D.EltQty; i++)
    {
      fprintf (file, "%d %d 3 0 %d 0 ", id++, elt_type2D, Mesh2D.EltElset[i]);
      ut_array_1d_int_fprintf (file, Mesh2D.EltNodes[i], eltnodeqty2D, "%d");
    }
  }

  // 3D mesh
  if (ut_string_inlist (dim, ',', "3"))
  {
    if (Part.qty > 0)
      for (i = 1; i <= Mesh3D.EltQty; i++)
      {
	fprintf (file, "%d %d 3 0 %d %d ", id++, elt_type3D, Mesh3D.EltElset[i], Part.elt_parts[i] + 1);
	ut_array_1d_int_fprintf (file, Mesh3D.EltNodes[i], eltnodeqty3D, "%d");
      }
    else
      for (i = 1; i <= Mesh3D.EltQty; i++)
      {
	fprintf (file, "%d %d 3 0 %d 0 ", id++, elt_type3D, Mesh3D.EltElset[i]);
	ut_array_1d_int_fprintf (file, Mesh3D.EltNodes[i], eltnodeqty3D, "%d");
      }
  }

  fprintf (file, "$EndElements\n");
  
  return;
}

void
neut_nodes_fprintf_gmsh (FILE* file, struct NODES Nodes)
{
  int i, j;

  fprintf (file, "$Nodes\n");
  fprintf (file, "%d\n", Nodes.NodeQty);
  for (i = 1; i <= Nodes.NodeQty; i++)
  {
    fprintf (file, "%d", i);
    for (j = 0; j < 3; j++)
      fprintf (file, " %.12f",
	  (fabs (Nodes.NodeCoo[i][j]) < 1e-12) ? 0 : Nodes.NodeCoo[i][j]);
    fprintf (file, "\n");
  }
  fprintf (file, "$EndNodes\n");

  return;
}
