/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "ReconMesh.h"

void
ReconMesh (char* dim, struct NODES Nodes, struct MESH *pMesh0D,
	  struct MESH *pMesh1D, struct MESH *pMesh2D, struct MESH *pMesh3D,
	  struct GEO* pGeo)
{
  struct GEO Geob;
  struct GEO* pGeob = &Geob;

  // if pGeo is NULL, working with pGeob. 
  if (pGeo == NULL)
    neut_geo_set_zero (pGeob);
  else
    pGeob = pGeo;

  if (pGeo == NULL || (*pGeob).DomType == NULL || strlen ((*pGeob).DomType) == 0)
  {
    (*pGeob).DomType = ut_alloc_1d_char (10);
    sprintf ((*pGeob).DomType, "unknown");
  }

  (*pGeob).version = ut_alloc_1d_char (10);
  strcpy ((*pGeob).version, "1.10");
  (*pGeob).N = (*pMesh3D).ElsetQty;
  (*pGeob).Type = ut_alloc_1d_char (10);
  strcpy ((*pGeob).Type, "standard");
  (*pGeob).Id   = 0;
  (*pGeob).morpho = ut_alloc_1d_char (10);
  strcpy ((*pGeob).morpho, "unknown");

  if ((*pMesh3D).NodeElts == NULL)
    neut_mesh_init_nodeelts (pMesh3D, Nodes.NodeQty);

  ReconMesh_2d (Nodes, pMesh2D, pMesh3D, pGeob);

  ReconMesh_1d (Nodes, pMesh1D, pMesh2D, pGeob);

  // This trick is to avoid failure with mapped meshes, in the geo
  // reconstruction in ReconMesh_0D
  if (pGeo != NULL)
    ReconMesh_0d (Nodes, pMesh0D, pMesh1D, pGeob);
  else
    ReconMesh_0d (Nodes, pMesh0D, pMesh1D, NULL);

  ut_array_1d_int_set ((*pGeob).FaceState + 1, (*pGeob).FaceQty, 1);
  ut_array_1d_int_set ((*pGeob).VerState + 1, (*pGeob).VerQty, 1);

  if (pGeo != NULL)
  {
    ReconMesh_finalizegeo (pGeob, Nodes, *pMesh0D, *pMesh1D, *pMesh2D,
      *pMesh3D);

    // If the domain is not defined, initializing it.
    if ((*pGeob).DomFaceQty == 0)
      neut_geo_init_domain (pGeob);

    if ((*pGeo).PolyTrue == NULL)
      neut_geo_init_polytrue (pGeob);

    if ((*pGeo).PolyBody == NULL)
      neut_geo_init_polybody (pGeob);
  }
  else
    neut_geo_free (pGeob);

  /*
  neut_debug_geo (stdout, *pGeo);
  
  FILE* file = ut_file_open ("dd.tess", "w");
  neut_geo_fprintf (file, *pGeo);
  ut_file_close (file, "dd.tess", "w");
  */

  if (ut_string_inlist (dim, ',', "2") == 0)
    neut_mesh_free (pMesh2D);
  
  if (ut_string_inlist (dim, ',', "1") == 0)
    neut_mesh_free (pMesh1D);

  if (ut_string_inlist (dim, ',', "0") == 0)
    neut_mesh_free (pMesh0D);

  /*
  if (pGeo != NULL && neut_geo_test (*pGeo) != 0)
  {
    ut_print_message (2, 0, "ReconMesh: tess is not valid!\n");
    ut_error_reportbug ();
  }
  */

  return;
}
