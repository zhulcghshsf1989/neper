/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nem_reconmesh.h"

void
nem_reconmesh (char* dim, struct NODES* pNodes, struct MESH *pMesh0D,
	  struct MESH *pMesh1D, struct MESH *pMesh2D, struct MESH *pMesh3D,
	  struct GEO* pGeo)
{
  struct GEO Geob;
  struct GEO* pGeob = &Geob;

  int recon_dim;
  // Disabling geo reconstruction is pGeo == NULL
  int reconst_geo // = (pGeo == NULL) ? 0 : 1;
    = (! strcmp ((*pMesh3D).EltType, "quad") || pGeo == NULL) ? 0 : 1;

  // if pGeo is NULL, working with pGeob. 
  if (! reconst_geo)
    neut_geo_set_zero (pGeob);
  else
    pGeob = pGeo;

  if (! reconst_geo || (*pGeob).DomType == NULL || strlen ((*pGeob).DomType) == 0)
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

  neut_nodes_init_bbox (pNodes);

  recon_dim = 3;
  if (ut_string_inlist (dim, ',', "0"))
    recon_dim = 0;
  else if (ut_string_inlist (dim, ',', "1"))
    recon_dim = 1;
  else if (ut_string_inlist (dim, ',', "2"))
    recon_dim = 2;

  if ((*pMesh3D).NodeElts == NULL)
    neut_mesh_init_nodeelts (pMesh3D, (*pNodes).NodeQty);

  if (recon_dim <= 2)
    nem_reconmesh_2d (*pNodes, pMesh2D, pMesh3D, pGeob);

  if (recon_dim <= 1)
    nem_reconmesh_1d (*pNodes, pMesh1D, pMesh2D, pGeob);

  if (recon_dim <= 0)
  {
    if (reconst_geo)
      nem_reconmesh_0d (*pNodes, pMesh0D, pMesh1D, pGeob);
    else
      nem_reconmesh_0d (*pNodes, pMesh0D, pMesh1D, NULL);
  }

  ut_array_1d_int_set ((*pGeob).FaceState + 1, (*pGeob).FaceQty, 1);
  ut_array_1d_int_set ((*pGeob).VerState + 1, (*pGeob).VerQty, 1);

  if (ut_string_inlist (dim, ',', "3") && ut_string_inlist (dim, ',', "2")
   && ut_string_inlist (dim, ',', "1") && ut_string_inlist (dim, ',', "0"))
  {
  if (reconst_geo)
  {
    nem_reconmesh_finalizegeo (pGeob, *pNodes, *pMesh0D, *pMesh1D, *pMesh2D,
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
  }

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
    ut_print_message (2, 0, "nem_reconmesh: tess is not valid!\n");
    ut_error_reportbug ();
  }
  */

  return;
}
