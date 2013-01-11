/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nem_reconmesh.h"

void
nem_reconmesh (char* dim, struct NODES* pNodes, struct MESH *pMesh0D,
	  struct MESH *pMesh1D, struct MESH *pMesh2D, struct MESH *pMesh3D,
	  struct TESS* pTess)
{
  struct TESS Tessb;
  struct TESS* pTessb = &Tessb;

  int recon_dim;
  // Disabling tess reconstruction is pTess == NULL
  int reconst_tess = (pTess == NULL) ? 0 : 1;
    // = (! strcmp ((*pMesh3D).EltType, "quad") || pTess == NULL) ? 0 : 1;

  if (reconst_tess == 1)
    pTessb = pTess;
  else
    neut_tess_set_zero (pTessb);

  if (! reconst_tess || (*pTessb).DomType == NULL || strlen ((*pTessb).DomType) == 0)
  {
    (*pTessb).DomType = ut_alloc_1d_char (10);
    sprintf ((*pTessb).DomType, "unknown");
  }

  (*pTessb).N = (*pMesh3D).ElsetQty;
  (*pTessb).Type = ut_alloc_1d_char (10);
  strcpy ((*pTessb).Type, "standard");
  (*pTessb).Id   = 0;
  (*pTessb).morpho = ut_alloc_1d_char (10);
  strcpy ((*pTessb).morpho, "unknown");

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
    nem_reconmesh_2d (*pNodes, pMesh2D, pMesh3D, pTessb);

  if (recon_dim <= 1)
    nem_reconmesh_1d (*pNodes, pMesh1D, pMesh2D, pTessb);

  if (recon_dim <= 0)
  {
    if (reconst_tess)
      nem_reconmesh_0d (*pNodes, pMesh0D, pMesh1D, pTessb);
    else
      nem_reconmesh_0d (*pNodes, pMesh0D, pMesh1D, NULL);
  }

  ut_array_1d_int_set ((*pTessb).FaceState + 1, (*pTessb).FaceQty, 1);
  ut_array_1d_int_set ((*pTessb).VerState + 1, (*pTessb).VerQty, 1);

  if (ut_string_inlist (dim, ',', "3") && ut_string_inlist (dim, ',', "2")
   && ut_string_inlist (dim, ',', "1") && ut_string_inlist (dim, ',', "0"))
  {
  if (reconst_tess)
  {
    nem_reconmesh_finalizetess (pTessb, *pNodes, *pMesh0D, *pMesh1D, *pMesh2D,
      *pMesh3D);

    // If the domain is not defined, initializing it.
    if ((*pTessb).DomFaceQty == 0)
      neut_tess_init_domain (pTessb);

    if ((*pTess).PolyTrue == NULL)
      neut_tess_init_polytrue (pTessb);

    if ((*pTess).PolyBody == NULL)
      neut_tess_init_polybody (pTessb);
  }
  else
    neut_tess_free (pTessb);
  }

  /*
  neut_debug_tess (stdout, *pTess);
  
  FILE* file = ut_file_open ("dd.tess", "w");
  neut_tess_fprintf (file, *pTess);
  ut_file_close (file, "dd.tess", "w");
  */

  if (ut_string_inlist (dim, ',', "2") == 0)
    neut_mesh_free (pMesh2D);
  
  if (ut_string_inlist (dim, ',', "1") == 0)
    neut_mesh_free (pMesh1D);

  if (ut_string_inlist (dim, ',', "0") == 0)
    neut_mesh_free (pMesh0D);

  /*
  if (pTess != NULL && neut_tess_test (*pTess) != 0)
  {
    ut_print_message (2, 0, "nem_reconmesh: tess is not valid!\n");
    ut_error_reportbug ();
  }
  */

  return;
}
