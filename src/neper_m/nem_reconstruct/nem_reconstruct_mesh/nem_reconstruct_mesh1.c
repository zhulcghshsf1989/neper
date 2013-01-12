/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nem_reconstruct_mesh.h"

void
nem_reconstruct_mesh (char* dim, struct NODES* pNodes, struct MESH *pMesh0D,
		      struct MESH *pMesh1D, struct MESH *pMesh2D, struct MESH *pMesh3D,
		      struct TESS* pTess)
{
  struct TESS Tessb;
  struct TESS* pTessb = &Tessb;

  if ((*pMesh2D).EltElset != NULL)
    neut_mesh_init_eltelset (pMesh2D, NULL);

  if ((*pMesh2D).NodeElts != NULL)
    neut_mesh_init_nodeelts (pMesh2D, (*pNodes).NodeQty);

  if ((*pMesh3D).EltElset != NULL)
    neut_mesh_init_eltelset (pMesh3D, NULL);

  if ((*pMesh3D).NodeElts != NULL)
    neut_mesh_init_nodeelts (pMesh3D, (*pNodes).NodeQty);

  if (pTess)
    pTessb = pTess;
  else
    neut_tess_set_zero (pTessb);

  if ((*pTessb).DomType == NULL || strlen ((*pTessb).DomType) == 0)
  {
    (*pTessb).DomType = ut_alloc_1d_char (8);
    sprintf ((*pTessb).DomType, "unknown");
  }

  (*pTessb).N = (*pMesh3D).ElsetQty;
  (*pTessb).Type = ut_alloc_1d_char (10);
  strcpy ((*pTessb).Type, "standard");
  (*pTessb).morpho = ut_alloc_1d_char (10);
  strcpy ((*pTessb).morpho, "unknown");

  int recon_dim = 3;
  if (ut_string_inlist (dim, ',', "0"))
    recon_dim = 0;
  else if (ut_string_inlist (dim, ',', "1"))
    recon_dim = 1;
  else if (ut_string_inlist (dim, ',', "2"))
    recon_dim = 2;

  if ((*pMesh3D).NodeElts == NULL)
    neut_mesh_init_nodeelts (pMesh3D, (*pNodes).NodeQty);

  if (recon_dim <= 2)
    nem_reconstruct_mesh_2d (*pNodes, pMesh2D, pMesh3D, pTessb);

  if (recon_dim <= 1)
    nem_reconstruct_mesh_1d (*pNodes, pMesh1D, pMesh2D, pTessb);

  if (recon_dim <= 0)
    nem_reconstruct_mesh_0d (*pNodes, pMesh0D, pMesh1D, pTess ? pTessb : NULL);

  if (pTess != NULL)
    nem_reconstruct_mesh_finalizetess (pTessb, *pNodes, *pMesh0D, *pMesh1D, *pMesh2D, *pMesh3D);
  else
    neut_tess_free (pTessb);

  if (ut_string_inlist (dim, ',', "2") == 0)
    neut_mesh_free (pMesh2D);
  
  if (ut_string_inlist (dim, ',', "1") == 0)
    neut_mesh_free (pMesh1D);

  if (ut_string_inlist (dim, ',', "0") == 0)
    neut_mesh_free (pMesh0D);

  if (pTess)
    nem_reconstruct_mesh_tess_updating (pTess, *pNodes, *pMesh0D, *pMesh1D,
	*pMesh2D, *pMesh3D);

  /*
  if (pTess != NULL && neut_tess_test (*pTess) != 0)
  {
    ut_print_message (2, 0, "nem_reconstruct_mesh: tess is not valid!\n");
    ut_error_reportbug ();
  }
  */

  return;
}
