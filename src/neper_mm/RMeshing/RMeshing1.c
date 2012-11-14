/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "RMeshing.h"

void
RMeshing (struct NODES *pNodes, struct MESH *pMesh,
	  int ***pFoDNodes, struct IN In, struct GERMSET *pGermSet)
{
  int i;
  struct GEO Geo;
  FILE *in = NULL;
  // char *expandnset = NULL;
  // char *expandfaset = NULL;
  int* msize = ut_alloc_1d_int (3);

  neut_geo_set_zero (&Geo);

  /* Loading tessellation ------------------------------------------ */

  double** dsize = ut_alloc_2d (3, 2);
  if (In.ingeo != NULL)
  {
    ut_print_message (0, 2, "Loading tessellation ...\n");

    in = ut_file_open (In.ingeo, "r");
    neut_geo_fscanf (in, &Geo);
    ut_file_close (in, In.ingeo, "r");
    if (strcmp (Geo.DomType, "cube") != 0)
    {
      ut_print_message (2, 0, "Module -MM cannot mesh non-cuboidal tessellations.\n");
      abort ();
    }

    neut_geo_boundingbox (Geo, dsize);
  }
  else
  {
    dsize[0][1] = In.domainparms[0] * In.F[0];
    dsize[1][1] = In.domainparms[1] * In.F[1];
    dsize[2][1] = In.domainparms[2] * In.F[2];
  }

  nemm_in_msize (In, dsize, msize);

  // neut_utils_nset_expand (In.nset, &expandnset);
  // neut_utils_nset_expand (In.faset, &expandfaset);

  /* Creating mapped mesh ------------------------------------------ */

  ut_print_message (0, 2, "Generating mesh ...\n");
  BuildMMesh3D (msize, In.morder, pNodes, pMesh, pFoDNodes);
  neut_nodes_scale (pNodes, dsize[0][1], dsize[1][1], dsize[2][1]);
  neut_mesh_scale  (pMesh, msize, dsize[0][1], dsize[1][1], dsize[2][1]);

  /* Searching elsets ---------------------------------------------- */

  ut_print_message (0, 2, "Searching elsets ... ");
  if (In.ingeo != NULL)
  {
    RMeshingFGeo (Geo, pMesh, pGermSet);
    (*pMesh).ElsetQty = Geo.PolyQty;
  }
  else
  {
    RMeshingFNId (In, pMesh, pGermSet);

    if (! strcmp ((*pGermSet).ttype, "standard") 
     || ! strcmp ((*pGermSet).ttype, "periodic"))
      (*pMesh).ElsetQty = (*pGermSet).N;
    else 
      (*pMesh).ElsetQty = (*pGermSet).N + (*pGermSet).nN;
  }

  /* Calculation of Elsets (they are sorted) */
  int elset;
  (*pMesh).Elsets = ut_alloc_2d_int ((*pMesh).ElsetQty + 1, 1); 
  for (i = 1; i <= (*pMesh).EltQty; i++)
  {
    elset = (*pMesh).EltElset[i];
    if (elset > (*pMesh).ElsetQty)
    {
      fprintf (stderr, "elset = %d > elsetqty = %d\n", elset, (*pMesh).ElsetQty);
      ut_error_reportbug ();
    }
    (*pMesh).Elsets[elset][0]++;
    (*pMesh).Elsets[elset] =
      ut_realloc_1d_int ((*pMesh).Elsets[elset], (*pMesh).Elsets[elset][0] + 1);
    (*pMesh).Elsets[elset][(*pMesh).Elsets[elset][0]] = i;
  }

  // ut_free_1d_char (expandnset);
  // ut_free_1d_char (expandfaset);
  ut_free_1d_int (msize);
  ut_free_2d (dsize, 3);

  return;
}
