/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"DeleteEdge.h"

/* Delete edge from face; note that this routine DOES modify */
/* FaceVerQty (it is also the face edge quantity).		 */
void
DeleteEdgeFromFace (struct GEO *pGeo, int face, int edge, int verbosity)
{
  int i;
  int pos;

  if (verbosity >= 3)
  {
    printf ("Deleting edge %d from face %d.\n", edge, face);

    ut_print_lineheader (0);
    printf ("    ");
    for (i = 1; i <= (*pGeo).FaceVerQty[face]; i++)
      printf ("%d ", (*pGeo).FaceEdgeNb[face][i]);
  }

  pos =
    oneDIntEltPos ((*pGeo).FaceEdgeNb[face], 1, (*pGeo).FaceVerQty[face],
		   edge, 0);

  oneDIntDeleteNCompress ((*pGeo).FaceEdgeNb[face], 1,
			  (*pGeo).FaceVerQty[face], edge, 1);
  oneDIntCpy ((*pGeo).FaceEdgeOri[face], pos + 1, (*pGeo).FaceVerQty[face],
	      (*pGeo).FaceEdgeOri[face], pos, 0);

  (*pGeo).FaceVerQty[face]--;

  if (verbosity >= 3)
  {
    printf (" become  ");
    for (i = 1; i <= (*pGeo).FaceVerQty[face]; i++)
      printf ("%d ", (*pGeo).FaceEdgeNb[face][i]);
    printf ("\n");
  }

  return;
}

/* Delete ver from face; note that this routine DOES NOT modify
 * FaceVerQty.
 */
void
DeleteVerFromFace (struct GEO *pGeo, int face, int delver, int verbosity)
{
  int qty;			/* for the test */

  if (verbosity >= 3)
  {
    ut_print_lineheader (-1);
    printf ("Deleting ver %d from face %d\n", delver, face);
  }

  qty =
    oneDIntDeleteNCompress ((*pGeo).FaceVerNb[face], 1,
			    (*pGeo).FaceVerQty[face], delver, 1);

  /* test */
  if (qty != 1)
  {
    printf ("qty!=1 =%d\n", qty);
    abort ();
  }

  return;
}

/* Delete ver from face; note that this routine modifies
 * FaceVerQty.
 */
void
ReplaceVerInFace (struct GEO *pGeo, int face, int delver, int newver)
{
  int pos;			/* for the test */

  /* ut_print_lineheader(-1);
   * printf("Replacing ver %d by ver %d in face %d\n",delver,newver,face);
   */

  pos =
    oneDIntEltPos ((*pGeo).FaceVerNb[face], 1, (*pGeo).FaceVerQty[face],
		   delver, 0);

  /* test */
  if (pos == -1)
  {
    printf ("pos==-1 =%d\n", pos);
    abort ();
  }

  (*pGeo).FaceVerNb[face][pos] = newver;

  return;
}

int
DeleteFace (struct GEO *pGeo, int face, int edge, int verbosity)
{
  int i, j, k;
  int tmpface;
  int tmpver;
  int nnew;
  int *pnew = &nnew;
  int old;
  int *pold = &old;
  int ori;
  int pos;

  if (verbosity >= 3)
  {
    ut_print_lineheader (-1);
    printf ("Deleting face %d\n", face);
  }

  DeleteFaceFromItsPoly (pGeo, face);

  /* We search out the two remaining edges of the face; they
   * become the same: we keep the one with the lower id.
   */

  if (verbosity >= 3)
  {
    ut_print_lineheader (-1);
    printf ("    Deleting one of its two remaining edges: ");
  }
  /* we determine which are the numbers of the edge we keep (nnew) and the
   * edge we delete (old) -- They are now the same, we keep the lower nb.
   */
  SearchNewNOldEdges (pGeo, face, edge, pnew, pold);
  if (verbosity >= 3)
    printf ("%d (keeping %d)\n", old, nnew);

  if ((*pGeo).EdgeVerNb[nnew][0] == (*pGeo).EdgeVerNb[old][0]
      || (*pGeo).EdgeVerNb[nnew][1] == (*pGeo).EdgeVerNb[old][1])
    ori = 1;
  else
    ori = -1;

  if (verbosity >= 3)
  {
    ut_print_lineheader (-1);
    printf ("      Giving edge %d the faces of edge %d\n", nnew, old);

    ut_print_lineheader (-1);
    printf ("        edge %d: ", nnew);
    for (i = 0; i <= (*pGeo).EdgeFaceQty[nnew] - 1; i++)
      printf ("%d ", (*pGeo).EdgeFaceNb[nnew][i]);
    printf (" and  ");
    printf ("edge %d: ", old);
    for (i = 0; i <= (*pGeo).EdgeFaceQty[old] - 1; i++)
      printf ("%d ", (*pGeo).EdgeFaceNb[old][i]);
    printf ("become:\n");
  }

  /* for the nnew edge: determination of the face properties: qty & nbs
   */
  (*pGeo).EdgeFaceNb[nnew] = ut_realloc_1d_int ((*pGeo).EdgeFaceNb[nnew], (*pGeo).EdgeFaceQty[nnew] + (*pGeo).EdgeFaceQty[old]);
  oneDIntCpy ((*pGeo).EdgeFaceNb[old], 0, (*pGeo).EdgeFaceQty[old] - 1,
	      (*pGeo).EdgeFaceNb[nnew], (*pGeo).EdgeFaceQty[nnew], 0);

  (*pGeo).EdgeFaceQty[nnew] += (*pGeo).EdgeFaceQty[old];

  if (verbosity >= 3)
  {
    printf ("EdgeFaceNb: %d: ", (*pGeo).EdgeFaceQty[nnew]);
    for (i = 0; i <= (*pGeo).EdgeFaceQty[nnew] - 1; i++)
      printf ("%d ", (*pGeo).EdgeFaceNb[nnew][i]);
    printf ("\n");

    printf ("sorting\n");
  }
  /*oneDIntSort((*pGeo).EdgeFaceNb[nnew],0,(*pGeo).EdgeFaceQty[nnew]-1); */
  gsl_sort_int ((*pGeo).EdgeFaceNb[nnew], 1, (*pGeo).EdgeFaceQty[nnew]);
  if (verbosity >= 3)
  {
    printf ("EdgeFaceNb: %d: ", (*pGeo).EdgeFaceQty[nnew]);
    for (i = 0; i <= (*pGeo).EdgeFaceQty[nnew] - 1; i++)
      printf ("%d ", (*pGeo).EdgeFaceNb[nnew][i]);
    printf ("\n");

    printf ("compressing\n");
  }
  (*pGeo).EdgeFaceQty[nnew] =
    1 + oneDIntCompress ((*pGeo).EdgeFaceNb[nnew], 0,
			 (*pGeo).EdgeFaceQty[nnew] - 1);
  if (verbosity >= 3)
  {
    printf ("EdgeFaceNb: %d: ", (*pGeo).EdgeFaceQty[nnew]);
    for (i = 0; i <= (*pGeo).EdgeFaceQty[nnew] - 1; i++)
      printf ("%d ", (*pGeo).EdgeFaceNb[nnew][i]);
    printf ("\n");

    printf ("deleting and compressing\n");
  }

  (*pGeo).EdgeFaceQty[nnew] -=
    oneDIntDeleteNCompress ((*pGeo).EdgeFaceNb[nnew], 0,
			    (*pGeo).EdgeFaceQty[nnew], face, 2);
  if (verbosity >= 3)
  {
    printf ("EdgeFaceNb: %d: ", (*pGeo).EdgeFaceQty[nnew]);
    for (i = 0; i <= (*pGeo).EdgeFaceQty[nnew] - 1; i++)
      printf ("%d ", (*pGeo).EdgeFaceNb[nnew][i]);
    printf ("\n");

    ut_print_lineheader (-1);
    printf ("        edge %d: ", nnew);
    for (i = 0; i <= (*pGeo).EdgeFaceQty[nnew] - 1; i++)
      printf ("%d ", (*pGeo).EdgeFaceNb[nnew][i]);
    printf ("\n");

    ut_print_lineheader (-1);
    printf
      ("      Changing edge %d to edge %d in the parent faces of edge %d\n",
       old, nnew, old);
  }

  /* for the faces of the old edge: edge nb=old <-- nnew
   */
  for (i = 0; i <= (*pGeo).EdgeFaceQty[old] - 1; i++)
  {
    tmpface = (*pGeo).EdgeFaceNb[old][i];
    if (tmpface == face)	/* this is the deleted face */
      continue;

    if (verbosity >= 3)
    {
      ut_print_lineheader (-1);
      printf ("        face %d: ", tmpface);
      for (j = 1; j <= (*pGeo).FaceVerQty[tmpface]; j++)
	printf ("%d ", (*pGeo).FaceEdgeNb[tmpface][j]);
      printf ("\n");
    }

    /*oneDIntFindNReplace((*pGeo).FaceEdgeNb[tmpface],1,(*pGeo).FaceVerQty[tmpface],old,nnew,1); */

    pos =
      oneDIntEltPos ((*pGeo).FaceEdgeNb[tmpface], 1,
		     (*pGeo).FaceVerQty[tmpface], old, 0);
    (*pGeo).FaceEdgeNb[tmpface][pos] = nnew;
    (*pGeo).FaceEdgeOri[tmpface][pos] *= ori;

    if (verbosity >= 3)
    {
      ut_print_lineheader (-1);
      printf ("          become: ");
      for (j = 1; j <= (*pGeo).FaceVerQty[tmpface]; j++)
	printf ("%d ", (*pGeo).FaceEdgeNb[tmpface][j]);
      printf ("\nvertices:\n");
      for (j = 1; j <= (*pGeo).FaceVerQty[tmpface]; j++)
	printf ("%d ", (*pGeo).FaceVerNb[tmpface][j]);
      printf ("\n");
    }

    for (j = 1; j <= (*pGeo).FaceVerQty[tmpface] - 1; j++)
      if ((*pGeo).FaceEdgeNb[tmpface][j] ==
	  (*pGeo).FaceEdgeNb[tmpface][j + 1])
      {
	oneDIntDeleteNCompress ((*pGeo).FaceEdgeNb[tmpface], 1,
				(*pGeo).FaceVerQty[tmpface],
				(*pGeo).FaceEdgeNb[tmpface][j], 2);
	for (k = (*pGeo).FaceVerQty[tmpface]; k >= j + 2; k--)
	  (*pGeo).FaceEdgeOri[tmpface][k - 2] =
	    (*pGeo).FaceEdgeOri[tmpface][k];
	for (k = (*pGeo).FaceVerQty[tmpface]; k >= j + 2; k--)
	  (*pGeo).FaceVerNb[tmpface][k - 2] = (*pGeo).FaceVerNb[tmpface][k];

	(*pGeo).FaceVerQty[tmpface] -= 2;
	if (verbosity >= 2)
	{
	  ut_print_message (1, 3, "critical case detected\n");

	  for (j = 1; j <= (*pGeo).FaceVerQty[tmpface]; j++)
	    printf ("%d ", (*pGeo).FaceEdgeNb[tmpface][j]);
	  printf ("\n");
	}

	break;
      }
    if ((*pGeo).FaceEdgeNb[tmpface][1] ==
	(*pGeo).FaceEdgeNb[tmpface][(*pGeo).FaceVerQty[tmpface]])
    {
      for (k = 1; k <= (*pGeo).FaceVerQty[tmpface] - 1; k++)
	(*pGeo).FaceEdgeNb[tmpface][k] = (*pGeo).FaceEdgeNb[tmpface][k + 1];
      for (k = 1; k <= (*pGeo).FaceVerQty[tmpface] - 1; k++)
	(*pGeo).FaceEdgeOri[tmpface][k] = (*pGeo).FaceEdgeOri[tmpface][k + 1];
      for (k = 1; k <= (*pGeo).FaceVerQty[tmpface] - 1; k++)
	(*pGeo).FaceVerNb[tmpface][k] = (*pGeo).FaceVerNb[tmpface][k + 1];

      (*pGeo).FaceVerQty[tmpface] -= 2;
      if (verbosity >= 2)
	ut_print_message (1, 3, "critical case 2 detected\n");
      return 1;
    }
  }

  if (verbosity >= 3)
  {
    ut_print_lineheader (-1);
    printf ("      Deleting edge %d in its 2 vertices\n", old);
  }
  /* for the vertices of the old edge */
  for (i = 0; i <= 1; i++)
  {
    tmpver = (*pGeo).EdgeVerNb[old][i];

    if (verbosity >= 3)
    {
      ut_print_lineheader (-1);
      printf ("        ver %d: ", tmpver);
      for (j = 0; j <= (*pGeo).VerEdgeQty[tmpver] - 1; j++)
	printf ("%d ", (*pGeo).VerEdgeNb[tmpver][j]);
      printf (" become  ");
    }

    (*pGeo).VerEdgeQty[tmpver] -=
      oneDIntDeleteNCompress ((*pGeo).VerEdgeNb[tmpver], 0,
			      (*pGeo).VerEdgeQty[tmpver] - 1, old, 1);

    if (verbosity >= 3)
    {
      for (j = 0; j <= (*pGeo).VerEdgeQty[tmpver] - 1; j++)
	printf ("%d ", (*pGeo).VerEdgeNb[tmpver][j]);
      printf ("\n");
    }
  }

  if (verbosity >= 3)
  {
    ut_print_lineheader (-1);
    printf ("    Setting edge %d as useless\n", old);
  }
  (*pGeo).EdgeState[old] = -1;

  if ((*pGeo).EdgeDom[old][0] == 1) // means old is tronger than nnew since nnew
    				      // should not have a domain edge
  {
    if (verbosity >= 3)
    {
      ut_print_lineheader (-1);
      printf ("    Changing bound data of edge %d to the one of edge %d\n", nnew, old);
      ut_print_lineheader (-1);
      printf ("    %d %d becomes %d %d\n", (*pGeo).EdgeDom[nnew][0],
		(*pGeo).EdgeDom[nnew][1], (*pGeo).EdgeDom[old][0], (*pGeo).EdgeDom[old][1]);
    }

    ut_array_1d_int_memcpy ((*pGeo).EdgeDom[nnew], 2, (*pGeo).EdgeDom[old]);
  }

  /* ut_print_lineheader(-1); printf("      Setting face %d ver qty to 0\n",face); */
  /* (*pGeo).FaceVerQty[face]=0;      */

  /* the face is set as useless       */
  if (verbosity >= 3)
  {
    ut_print_lineheader (-1);
    printf ("    Setting face %d as useless\n", face);
  }
  (*pGeo).FaceState[face] = -1;

  return 0;
}
