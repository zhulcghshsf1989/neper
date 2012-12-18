/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"DeleteEdge.h"

/* Updating of EdgeState */
void
UpdateEdgeState (struct TESS *pTess, int edge)
{
  /* edge is set as useless and deleted. */
  (*pTess).EdgeState[edge] = -1;

  return;
}

/* Searching the vertex to delete and the new one (its coo will change) */
void
SearchDelNNewVer (struct TESS *pTess, int edge, int *pdelver, int *pnewver)
{
  (*pdelver) = ut_array_1d_int_max ((*pTess).EdgeVerNb[edge], 2);
  (*pnewver) = ut_array_1d_int_min ((*pTess).EdgeVerNb[edge], 2);

  return;
}


int
UpdateVerCoo (struct TESS *pTess, int newver, int verbosity)
{
  return UpdateVerCooMiniFF (pTess, newver, verbosity);
}

/* UpdateEdgeVerNb updates the edge ver nb
 * the delver vertex is replaced by the new one
 */
void
UpdateEdgeVerNb (struct TESS *pTess, int delver, int newver, int verbosity)
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

  for (i = 0; i <= (*pTess).VerEdgeQty[delver] - 1; i++)
  {
    tmpedge = (*pTess).VerEdgeNb[delver][i];

    if (verbosity >= 3)
    {
      ut_print_lineheader (-1);
      printf ("      ver of edge %d: ", tmpedge);
    }

    pos = oneDIntEltPos ((*pTess).EdgeVerNb[tmpedge], 0, 1, delver, 0);

    if (verbosity >= 3)
      printf ("%d %d become ", (*pTess).EdgeVerNb[tmpedge][0],
	      (*pTess).EdgeVerNb[tmpedge][1]);

    (*pTess).EdgeVerNb[tmpedge][pos] = newver;

    if (verbosity >= 3)
      printf ("%d %d\n", (*pTess).EdgeVerNb[tmpedge][0],
	      (*pTess).EdgeVerNb[tmpedge][1]);
  }

  return;
}

/* UpdateFace updates the properties of the parent faces of the edge */
int
UpdateFaceVerNEdge (struct TESS *pTess, int edge, int delver, int newver,
		    int verbosity)
{
  int i, err = 0;

  if (verbosity >= 3)
  {
    ut_print_lineheader (-1);
    printf ("  %d parent faces: ", (*pTess).EdgeFaceQty[edge]);
    for (i = 0; i <= (*pTess).EdgeFaceQty[edge] - 1; i++)
      printf (" %d", (*pTess).EdgeFaceNb[edge][i]);
    printf ("\n");

    ut_print_lineheader (-1);
    printf ("Updating face vertices\n");
  }

  UpdateFaceState (pTess, delver, newver);

  UpdateFaceVer (pTess, delver, newver, verbosity);

  if (verbosity >= 3)
  {
    printf ("\n");
    ut_print_lineheader (-1);
    printf ("Updating face edges\n");
  }

  err = UpdateFaceEdge (pTess, edge, verbosity);

  return err;
}

int
UpdateVer (struct TESS *pTess, int edge, int delver, int newver, int verbosity)
{
  int err = 0;

  /* Updating of VerEdgeQty & VerEdgeNb */
  UpdateVerEdge (pTess, edge, delver, newver, verbosity);

  /* Updating of VerDom */
  err = UpdateVerBound (pTess, delver, newver);

  /* Updating of VerState */
  UpdateVerState (pTess, delver, newver, verbosity);

  return err;
}

/* UpdateEdgeBound updates the bound of the edges of ver */
void
nefm_edgedel_edgedom_fromverdom (struct TESS *pTess, int ver)
{
  int i, id, ver2, edge, domedge, domface;
  int* vers = ut_alloc_1d_int (2);

  // Scanning all edges of the new ver
  for (i = 0; i < (*pTess).VerEdgeQty[i]; i++)
  {
    edge = (*pTess).VerEdgeNb[ver][i];
    ver2 = ((*pTess).EdgeVerNb[edge][0] != ver) ? (*pTess).EdgeVerNb[edge][0]
                                               : (*pTess).EdgeVerNb[edge][1];

    // at least one ver is not on the domain: edge not on the domain
    if ((*pTess).VerDom[ver][0] == -1 || (*pTess).VerDom[ver2][0] == -1)
      (*pTess).EdgeDom[edge][0] = -1;

    // both vers are boundary vers
    else if ((*pTess).VerDom[ver][0] == 0 && (*pTess).VerDom[ver2][0] == 0)
    {
      // test: vers are on the same domain ver: this should not happen
      if ((*pTess).VerDom[ver][1] == (*pTess).VerDom[ver2][1])
	ut_error_reportbug ();

      vers[0] = (*pTess).VerDom[ver][1];
      vers[1] = (*pTess).VerDom[ver2][1]; 
      if (neut_tess_domvers_commondomedge ((*pTess), vers, 2, &domedge) == 0)
      {
	(*pTess).EdgeDom[edge][0] = 1;
	(*pTess).EdgeDom[edge][1] = domedge;
      }
      else
	ut_error_reportbug ();
    }

    // one ver is on a boundary ver and the other is not
    else if (((*pTess).VerDom[ver][0] == 0 || (*pTess).VerDom[ver2][0] == 0)
          && ((*pTess).VerDom[ver][0] != 0 || (*pTess).VerDom[ver2][0] != 0))
    {
      // id is not on the boundary ver
      id = ((*pTess).VerDom[ver][0] != 0) ? ver : ver2;

      // id is on a domain edge or face
      if ((*pTess).VerDom[id][0] == 1 || (*pTess).VerDom[id][0] == 2)
	ut_array_1d_int_memcpy ((*pTess).EdgeDom[edge], 2, (*pTess).VerDom[id]);
      else
	ut_error_reportbug ();
    }

    // both vers are on boundary edges
    else if (((*pTess).VerDom[ver][0] == 1 && (*pTess).VerDom[ver2][0] == 1))
    {
      // if on the same edge
      if ((*pTess).VerDom[ver][1] == (*pTess).VerDom[ver2][1])
	ut_array_1d_int_memcpy ((*pTess).EdgeDom[edge], 2, (*pTess).VerDom[ver]);
      // if on different edges
      else
      {
	if (neut_tess_domedges_commondomface ((*pTess), vers, 2, &domface) == 0)
	{
	  (*pTess).EdgeDom[edge][0] = 2;
	  (*pTess).EdgeDom[edge][1] = domface;
	}
	else
	  ut_error_reportbug ();

	ut_array_1d_int_memcpy ((*pTess).EdgeDom[edge], 2, (*pTess).VerDom[ver]);
      }
    }

    // one ver is on a boundary edge and the other is not
    else if (((*pTess).VerDom[ver][0] == 1 || (*pTess).VerDom[ver2][0] == 1)
          && ((*pTess).VerDom[ver][0] != 1 || (*pTess).VerDom[ver2][0] != 1))
    {
      // id is not on the boundary edge
      id = ((*pTess).VerDom[ver][0] != 0) ? ver : ver2;

      // id is on a domain edge or face
      if ((*pTess).VerDom[id][0] == 2)
	ut_array_1d_int_memcpy ((*pTess).EdgeDom[edge], 2, (*pTess).VerDom[id]);
      else
	ut_error_reportbug ();
    }

    // both vers are on boundary faces
    else if (((*pTess).VerDom[ver][0] == 2 && (*pTess).VerDom[ver2][0] == 2))
    {
      // if on the same face
      if ((*pTess).VerDom[ver][1] == (*pTess).VerDom[ver2][1])
	ut_array_1d_int_memcpy ((*pTess).EdgeDom[edge], 2, (*pTess).VerDom[ver]);
      else
	(*pTess).EdgeDom[edge][0] = -1;
    }
  }

  ut_free_1d_int (vers);

  return;
}

void
nefm_tess_reg_ver_facedom (struct TESS *pTess, int ver)
{
  int i;
  int faceqty;
  int* face = NULL;
  
  neut_tess_ver_faces (*pTess, ver, &face, &faceqty);

  for (i = 0; i < faceqty; i++)
    neut_tess_init_facedom_face_v (pTess, face[i]);

  neut_tess_init_domtessface (pTess);

  ut_free_1d_int (face);

  return;
}
