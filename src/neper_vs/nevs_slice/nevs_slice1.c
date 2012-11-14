/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nevs_slice.h"

void
nevs_slice (char** argv, int* pi, struct PRINT* pPrint)
{
  if (strcmp (argv[*pi], "-slicemesh") == 0)
  {
    (*pi)++;
    (*pPrint).slice = ut_alloc_1d_char (strlen (argv[(*pi)]) + 1);
    sscanf (argv[(*pi)], "%s", (*pPrint).slice);
  }
  else
  {
    ut_print_message (2, 0, "Unknown command %s\n", argv[(*pi)]);
    abort ();
  }

  /*
  if ((*pPrint).showslice == NULL)
  {
    (*pPrint).showslice = ut_alloc_1d_char (10);
    strcpy ((*pPrint).showslice, "all");
    if ((*pPrint).showtess == -1)
      (*pPrint).showtess = 0;
    if ((*pPrint).showmesh == -1)
      (*pPrint).showmesh = 0;
  }
  */

  sscanf (argv[(*pi)], "%s", (*pPrint).slice);

  return;
}

void
nevs_slice_mesh (struct NODES Nodes, struct MESH Mesh, struct MESHDATA MeshData,
		 char* slice, int* pSQty,
		 struct NODES** pN  , struct MESH** pM , struct MESHDATA** pMeshData)
{
  char** label = NULL;
  int i;
  double* eq = ut_alloc_1d (4);

  ut_string_separate (slice, ',', &label, pSQty);

  (*pN) = malloc (*pSQty * sizeof (struct NODES));
  (*pM) = malloc (*pSQty * sizeof (struct MESH));
  (*pMeshData) = malloc (*pSQty * sizeof (struct MESHDATA));

  for (i = 0; i < *pSQty; i++)
  {
    neut_nodes_set_zero (&((*pN)[i]));
    neut_mesh_set_zero (&((*pM)[i]));
    neut_meshdata_set_default (&((*pMeshData)[i]));
  }

  for (i = 0; i < *pSQty; i++)
  {
    ut_space_string_plane (label[i], eq);

    int*  elt_newold  = NULL;
    int** node_newold = NULL;
    double* node_fact = NULL;

    neut_mesh3d_slice (Nodes, Mesh, eq, &((*pN)[i]), &((*pM)[i]),
	               &elt_newold, &node_newold, &node_fact);
    neut_meshdata_mesh2slice (Nodes, Mesh, MeshData, (*pN)[i], (*pM)[i], elt_newold, node_newold, node_fact, &((*pMeshData)[i]));
  }

  ut_free_1d (eq);
  ut_free_2d_char (label, *pSQty);

  return;
}
