/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"DeleteEdge.h"

/* Updating of EdgeState */
void
UpdateEdgeState (struct GEO *pGeo, int edge)
{
  /* edge is set as useless and deleted. */
  (*pGeo).EdgeState[edge] = -1;

  return;
}

/* Searching the vertex to delete and the new one (its coo will change) */
void
SearchDelNNewVer (struct GEO *pGeo, int edge, int *pdelver, int *pnewver)
{
  (*pdelver) = ut_array_1d_int_max ((*pGeo).EdgeVerNb[edge], 2);
  (*pnewver) = ut_array_1d_int_min ((*pGeo).EdgeVerNb[edge], 2);

  return;
}


int
UpdateVerCoo (struct GEO *pGeo, int newver, int verbosity)
{
  return UpdateVerCooMiniFF (pGeo, newver, verbosity);
}

/* UpdateEdgeVerNb updates the edge ver nb
 * the delver vertex is replaced by the new one
 */
void
UpdateEdgeVerNb (struct GEO *pGeo, int delver, int newver, int verbosity)
{
  int i;
  int tmpedge;
  int pos;

  if (verbosity >= 3)
  {
    ut_print_lineheader (-1);
    printf ("    Changing ver %d to ver %d in edges of ver %d\n", newver,
	    delver, delver);
  }

  for (i = 0; i <= (*pGeo).VerEdgeQty[delver] - 1; i++)
  {
    tmpedge = (*pGeo).VerEdgeNb[delver][i];

    if (verbosity >= 3)
    {
      ut_print_lineheader (-1);
      printf ("      ver of edge %d: ", tmpedge);
    }

    pos = oneDIntEltPos ((*pGeo).EdgeVerNb[tmpedge], 0, 1, delver, 0);

    if (verbosity >= 3)
      printf ("%d %d become ", (*pGeo).EdgeVerNb[tmpedge][0],
	      (*pGeo).EdgeVerNb[tmpedge][1]);

    (*pGeo).EdgeVerNb[tmpedge][pos] = newver;

    if (verbosity >= 3)
      printf ("%d %d\n", (*pGeo).EdgeVerNb[tmpedge][0],
	      (*pGeo).EdgeVerNb[tmpedge][1]);
  }

  return;
}

/* UpdateFace updates the properties of the parent faces of the edge */
int
UpdateFaceVerNEdge (struct GEO *pGeo, int edge, int delver, int newver,
		    int verbosity)
{
  int i, err = 0;

  if (verbosity >= 3)
  {
    ut_print_lineheader (-1);
    printf ("  %d parent faces: ", (*pGeo).EdgeFaceQty[edge]);
    for (i = 0; i <= (*pGeo).EdgeFaceQty[edge] - 1; i++)
      printf (" %d", (*pGeo).EdgeFaceNb[edge][i]);
    printf ("\n");

    ut_print_lineheader (-1);
    printf ("Updating face vertices\n");
  }

  UpdateFaceState (pGeo, delver, newver);

  UpdateFaceVer (pGeo, delver, newver, verbosity);

  if (verbosity >= 3)
  {
    printf ("\n");
    ut_print_lineheader (-1);
    printf ("Updating face edges\n");
  }

  err = UpdateFaceEdge (pGeo, edge, verbosity);

  return err;
}

int
UpdateVer (struct GEO *pGeo, int edge, int delver, int newver, int verbosity)
{
  int err = 0;

  /* Updating of VerEdgeQty & VerEdgeNb */
  UpdateVerEdge (pGeo, edge, delver, newver, verbosity);

  /* Updating of VerDom */
  err = UpdateVerBound (pGeo, delver, newver);

  /* Updating of VerState */
  UpdateVerState (pGeo, delver, newver, verbosity);

  return err;
}

/* UpdateEdgeBound updates the bound of the edges of ver */
void
nefm_edgedel_edgedom_fromverdom (struct GEO *pGeo, int ver)
{
  int i, id, ver2, edge, domedge, domface;
  int* vers = ut_alloc_1d_int (2);

  // Scanning all edges of the new ver
  for (i = 0; i < (*pGeo).VerEdgeQty[i]; i++)
  {
    edge = (*pGeo).VerEdgeNb[ver][i];
    ver2 = ((*pGeo).EdgeVerNb[edge][0] != ver) ? (*pGeo).EdgeVerNb[edge][0]
                                               : (*pGeo).EdgeVerNb[edge][1];

    // at least one ver is not on the domain: edge not on the domain
    if ((*pGeo).VerDom[ver][0] == -1 || (*pGeo).VerDom[ver2][0] == -1)
      (*pGeo).EdgeDom[edge][0] = -1;

    // both vers are boundary vers
    else if ((*pGeo).VerDom[ver][0] == 0 && (*pGeo).VerDom[ver2][0] == 0)
    {
      // test: vers are on the same domain ver: this should not happen
      if ((*pGeo).VerDom[ver][1] == (*pGeo).VerDom[ver2][1])
	ut_error_reportbug ();

      vers[0] = (*pGeo).VerDom[ver][1];
      vers[1] = (*pGeo).VerDom[ver2][1]; 
      if (neut_geo_domvers_commondomedge ((*pGeo), vers, 2, &domedge) == 0)
      {
	(*pGeo).EdgeDom[edge][0] = 1;
	(*pGeo).EdgeDom[edge][1] = domedge;
      }
      else
	ut_error_reportbug ();
    }

    // one ver is on a boundary ver and the other is not
    else if (((*pGeo).VerDom[ver][0] == 0 || (*pGeo).VerDom[ver2][0] == 0)
          && ((*pGeo).VerDom[ver][0] != 0 || (*pGeo).VerDom[ver2][0] != 0))
    {
      // id is not on the boundary ver
      id = ((*pGeo).VerDom[ver][0] != 0) ? ver : ver2;

      // id is on a domain edge or face
      if ((*pGeo).VerDom[id][0] == 1 || (*pGeo).VerDom[id][0] == 2)
	ut_array_1d_int_memcpy ((*pGeo).EdgeDom[edge], 2, (*pGeo).VerDom[id]);
      else
	ut_error_reportbug ();
    }

    // both vers are on boundary edges
    else if (((*pGeo).VerDom[ver][0] == 1 && (*pGeo).VerDom[ver2][0] == 1))
    {
      // if on the same edge
      if ((*pGeo).VerDom[ver][1] == (*pGeo).VerDom[ver2][1])
	ut_array_1d_int_memcpy ((*pGeo).EdgeDom[edge], 2, (*pGeo).VerDom[ver]);
      // if on different edges
      else
      {
	if (neut_geo_domedges_commondomface ((*pGeo), vers, 2, &domface) == 0)
	{
	  (*pGeo).EdgeDom[edge][0] = 2;
	  (*pGeo).EdgeDom[edge][1] = domface;
	}
	else
	  ut_error_reportbug ();

	ut_array_1d_int_memcpy ((*pGeo).EdgeDom[edge], 2, (*pGeo).VerDom[ver]);
      }
    }

    // one ver is on a boundary edge and the other is not
    else if (((*pGeo).VerDom[ver][0] == 1 || (*pGeo).VerDom[ver2][0] == 1)
          && ((*pGeo).VerDom[ver][0] != 1 || (*pGeo).VerDom[ver2][0] != 1))
    {
      // id is not on the boundary edge
      id = ((*pGeo).VerDom[ver][0] != 0) ? ver : ver2;

      // id is on a domain edge or face
      if ((*pGeo).VerDom[id][0] == 2)
	ut_array_1d_int_memcpy ((*pGeo).EdgeDom[edge], 2, (*pGeo).VerDom[id]);
      else
	ut_error_reportbug ();
    }

    // both vers are on boundary faces
    else if (((*pGeo).VerDom[ver][0] == 2 && (*pGeo).VerDom[ver2][0] == 2))
    {
      // if on the same face
      if ((*pGeo).VerDom[ver][1] == (*pGeo).VerDom[ver2][1])
	ut_array_1d_int_memcpy ((*pGeo).EdgeDom[edge], 2, (*pGeo).VerDom[ver]);
      else
	(*pGeo).EdgeDom[edge][0] = -1;
    }
  }

  ut_free_1d_int (vers);

  return;
}

void
nefm_geo_reg_ver_facedom (struct GEO *pGeo, int ver)
{
  int i;
  int faceqty;
  int* face = NULL;
  
  neut_geo_ver_faces (*pGeo, ver, &face, &faceqty);

  for (i = 0; i < faceqty; i++)
    neut_geo_init_facedom_face_v (pGeo, face[i]);

  neut_geo_init_domtessface (pGeo);

  ut_free_1d_int (face);

  return;
}
