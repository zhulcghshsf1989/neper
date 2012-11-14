/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"DeleteEdge.h"

#ifdef HAVE_GSL
#include<gsl/gsl_rng.h>
#include<gsl/gsl_linalg.h>
#endif

void
UpdateFaceState (struct GEO *pGeo, int delver, int newver)
{
  int i, face, NewVerFaceQty, DelVerFaceQty;
  int *NewVerFace = NULL;
  int *DelVerFace = NULL;

  neut_geo_ver_faces (*pGeo, delver, &DelVerFace, &DelVerFaceQty);
  neut_geo_ver_faces (*pGeo, newver, &NewVerFace, &NewVerFaceQty);

  /* every face of the deleted and new vertices will be modified, so
   * their states are set to 1. Maybe some faces will be deleted: in this
   * case, their states will be set to -1 afterwards.
   */
  for (i = 0; i < DelVerFaceQty; i++)
  {
    face = DelVerFace[i];
    (*pGeo).FaceState[face] = 1;
  }

  for (i = 0; i < NewVerFaceQty; i++)
  {
    face = NewVerFace[i];
    (*pGeo).FaceState[face] = 1;
  }

  ut_free_1d_int (NewVerFace);
  ut_free_1d_int (DelVerFace);

  return;
}

/* Delete ver from face; note that this routine DOES NOT modify
 * FaceVerQty (it is also the face edge quantity).
 * FaceVerQty is to be modified either in DeleteVerFromFacep
 * or in DeleteEdgeFromFacep, if UpdateFaceVer is after UpdateFaceEdgep
 * and if vice versa, respectively.
 */
void
UpdateFaceVer (struct GEO *pGeo, int delver, int newver, int verbosity)
{
  int i, j, face, NewVerFaceQty, DelVerFaceQty;
  int *NewVerFace = NULL;
  int *DelVerFace = NULL;

  /* Calculating DelVerFace and NewVerFace */
  neut_geo_ver_faces (*pGeo, delver, &DelVerFace, &DelVerFaceQty);
  neut_geo_ver_faces (*pGeo, newver, &NewVerFace, &NewVerFaceQty);

  for (i = 0; i < DelVerFaceQty; i++)
  {
    face = DelVerFace[i];

    if (verbosity >= 3)
    {
      ut_print_lineheader (-1);
      printf ("Treating delver face %d (%d/%d) ---\n", face, i,
	      DelVerFaceQty);
      ut_print_lineheader (-1);
      printf ("  %d faces: ", (*pGeo).FaceVerQty[face]);
      for (j = 1; j <= (*pGeo).FaceVerQty[face]; j++)
	printf ("%d ", (*pGeo).FaceVerNb[face][j]);
      printf ("\n");
    }

    if (ut_array_1d_int_eltpos (NewVerFace, NewVerFaceQty, face) != -1)
      DeleteVerFromFace (pGeo, face, delver, verbosity);
    else
      ReplaceVerInFace (pGeo, face, delver, newver);
  }

  ut_free_1d_int (NewVerFace);
  ut_free_1d_int (DelVerFace);

  return;
}

/* Delete edge from face; note that this routine MODIFIES
 * FaceVerQty (it is also the face edge quantity).
 * FaceVerQty is to be modified either in DeleteVerFromFacep
 * or in DeleteEdgeFromFacep, if UpdateFaceVer is after UpdateFaceEdgep
 * and vice versa, respectively.
 */
int
UpdateFaceEdge (struct GEO *pGeo, int edge, int verbosity)
{
  int i;
  int face;
  int err = 0;

  for (i = 0; i < (*pGeo).EdgeFaceQty[edge]; i++)
  {
    face = (*pGeo).EdgeFaceNb[edge][i];
    if (verbosity >= 3)
    {
      ut_print_lineheader (-1);
      printf ("Treating face %d (%d/%d) ---\n", face, i + 1,
	      (*pGeo).EdgeFaceQty[edge]);
    }

    if ((*pGeo).FaceState[face] == -1)
    {
      ut_print_lineheader (2);
      printf ("The face to remove (%d) is already useless!\n", face);
      abort ();
    }

    if (verbosity >= 3)
    {
      ut_print_lineheader (0);
      printf ("  - face %d contains %d edges: ", face,
	      (*pGeo).FaceVerQty[face]);
    }

    /* if it contains almost 4 edges, the edge is removed;
     * else, the whole face is removed.                     */
    if ((*pGeo).FaceVerQty[face] >= 4)
      DeleteEdgeFromFace (pGeo, face, edge, verbosity);
    else
      err = DeleteFace (pGeo, face, edge, verbosity);
  }

  return err;
}

void
UpdateVerState (struct GEO *pGeo, int delver, int newver, int verbosity)
{
  int wgt1, wgt2;

  if (verbosity >= 3)
  {
    ut_print_lineheader (0);
    printf ("  Updating state of ver %d\n", newver);
    ut_print_lineheader (0);
    printf ("    ver %d: %d and ver %d: %d become ", newver,
	    (*pGeo).VerState[newver], delver, (*pGeo).VerState[delver]);
  }
  /* if state of newver is 0, its wgt is 1, else it is the state */
  wgt1 = IntMax (1, (*pGeo).VerState[newver]);
  /* if state of delver is 0, its wgt is 1, else it is the state */
  wgt2 = IntMax (1, (*pGeo).VerState[delver]);

  /* wgt of newver becomes wgt1+wgt2 */
  (*pGeo).VerState[newver] = wgt1 + wgt2;
  /* state of delver is set to -1 --> useless */
  (*pGeo).VerState[delver] = -1;

  if (verbosity >= 3)
    printf ("ver %d: %d\n", newver, (*pGeo).VerState[newver]);

  return;
}

/* Updating of VerEdgeQty & VerEdgeNb */
void
UpdateVerEdge (struct GEO *pGeo, int edge, int delver, int newver,
	       int verbosity)
{
  int i;
  int *array;			/* will be the VerEdgeNb  of newver ver */
  int qty;			/* will be the VerEdgeQty of newver ver */

  array =
    oneDIntCat ((*pGeo).VerEdgeNb[newver], 0, (*pGeo).VerEdgeQty[newver] - 1,
		(*pGeo).VerEdgeNb[delver], 0, (*pGeo).VerEdgeQty[delver] - 1,
		0);

  if (verbosity >= 3)
  {
    ut_print_lineheader (-1);
    printf ("  Giving ver %d the edges of ver %d\n", newver, delver);

    ut_print_lineheader (-1);
    printf ("    ver %d: ", newver);
    for (i = 0; i <= (*pGeo).VerEdgeQty[newver] - 1; i++)
      printf ("%d ", (*pGeo).VerEdgeNb[newver][i]);

    printf ("and ver %d: ", delver);
    for (i = 0; i <= (*pGeo).VerEdgeQty[delver] - 1; i++)
      printf ("%d ", (*pGeo).VerEdgeNb[delver][i]);
    printf ("become\n");
  }

  qty = (*pGeo).VerEdgeQty[delver] + (*pGeo).VerEdgeQty[newver];
  qty -= oneDIntDeleteNCompress (array, 0, qty - 1, edge, 2);

  (*pGeo).VerEdgeQty[newver] = qty;
  (*pGeo).VerEdgeNb[newver] = ut_realloc_1d_int ((*pGeo).VerEdgeNb[newver], qty);
  ut_array_1d_int_memcpy ((*pGeo).VerEdgeNb[newver], qty, array);

  if (verbosity >= 3)
  {
    ut_print_lineheader (-1);
    printf ("    ver %d: ", newver);
    for (i = 0; i <= (*pGeo).VerEdgeQty[newver] - 1; i++)
      printf ("%d ", (*pGeo).VerEdgeNb[newver][i]);
    printf ("\n");
  }

  ut_free_1d_int (array);

  return;
}

int
UpdateVerBound (struct GEO *pGeo, int delver, int newver)
{
  // vertices are not on the domain: nothing to do
  if ((*pGeo).VerDom[delver][0] == -1 && (*pGeo).VerDom[newver][0] == -1)
    return 0;

  // one vertex is on a domain vertex
  else if ((*pGeo).VerDom[delver][0] == 0 || (*pGeo).VerDom[newver][0] == 0)
  {
    // test: both vertices are domain vertices; this should not happen
    if ((*pGeo).VerDom[delver][0] == (*pGeo).VerDom[newver][0]) 
      ut_error_reportbug ();

    // delver is the domain ver: copying;
    // otherwise newver is the domain ver: no need to copy
    if ((*pGeo).VerDom[delver][0] == 0)
      ut_array_1d_int_memcpy ((*pGeo).VerDom[newver], 2, (*pGeo).VerDom[delver]);
  }

  // one vertex in on a domain edge
  else if ((*pGeo).VerDom[delver][0] == 1 || (*pGeo).VerDom[newver][0] == 1)
  {
    // test: both vertices are on a domain edge, but not on the same
    // becoming a domain vertex
    if (((*pGeo).VerDom[delver][0] == (*pGeo).VerDom[newver][0])
     && ((*pGeo).VerDom[delver][1] != (*pGeo).VerDom[newver][1]))
      ut_error_reportbug (); // the edge should not have been marked for deletion in NextEdgeToDel

    // delver is on the domain edge: copying;
    // otherwise newver is on the domain edge: no need to copy
    else if ((*pGeo).VerDom[delver][0] == 1)
      ut_array_1d_int_memcpy ((*pGeo).VerDom[newver], 2, (*pGeo).VerDom[delver]);
  }

  // one vertex in on a domain face
  else if ((*pGeo).VerDom[delver][0] == 2 || (*pGeo).VerDom[newver][0] == 2)
  {
    // if delver: copying, otherwise: no need to copy
    if ((*pGeo).VerDom[delver][0] == 2)
      ut_array_1d_int_memcpy ((*pGeo).VerDom[newver], 2, (*pGeo).VerDom[delver]);
  }

  return 0;
}

void
UpdateEdgeLength (struct GEO *pGeo, int ver)
{
  int i;

  for (i = 0; i < (*pGeo).VerEdgeQty[ver]; i++)
    neut_geo_init_edgelength_edge (pGeo, (*pGeo).VerEdgeNb[ver][i]);

  return;
}

// See documentation file `edgedel.pdf' for details on the algorithm.
int
UpdateVerCooMiniFF (struct GEO *pGeo, int newver, int verbosity)
{
  int i, j, k;
  gsl_matrix *A = NULL;
  gsl_vector *B = NULL;
  gsl_vector *X = NULL;
  gsl_permutation *p = NULL;
  int s;
  int faceqty;
  int* face = NULL;
  int N; // nb of faces
  int M; // nb of constrained faces

  verbosity = 0;

  neut_geo_ver_faces (*pGeo, newver, &face, &faceqty);

  if (verbosity > 0)
  {
    printf ("newver = %d has bound: %d %d\n", newver, (*pGeo).VerDom[newver][0], (*pGeo).VerDom[newver][1]); 

    printf ("newver = %d has %d faces of nbs: ", newver, faceqty);
    ut_array_1d_int_fprintf (stdout, face, faceqty, "%d");
    printf ("and bounds: \n");
    for (i = 0; i < faceqty; i++)
      ut_array_1d_int_fprintf (stdout, (*pGeo).FaceDom[face[i]], 2, "%d");
  }

  N = faceqty;
  M = 0;
  int* domainface = ut_alloc_1d_int (N);
  // this is an indirect way, but we need face anyway...
  for (i = 0; i < N; i++)
    if ((*pGeo).FaceDom[face[i]][0] == 2)
      domainface[M++] = (*pGeo).FaceDom[face[i]][1];
  
  if (verbosity > 0)
  {
    printf ("domainface = ");
    ut_array_1d_int_fprintf (stdout, domainface, M, "%d");
    ut_array_1d_int_sort_uniq (domainface, M, &M);
    printf ("-> ");
    ut_array_1d_int_fprintf (stdout, domainface, M, "%d");

    printf ("and %d domain faces of nbs: ", M);
    ut_array_1d_int_fprintf (stdout, domainface, M, "%d");
  }

  int qty;
  int* tmp = NULL;
  neut_geo_ver_domfaces (*pGeo, newver, &tmp, &qty);

  if (verbosity > 0)
  {
    printf ("neut_geo_ver_domfaces returns qty = %d and ids = ", qty);
    ut_array_1d_int_fprintf (stdout, tmp, qty, "%d");
  }

  M = qty;

  double** constraint = ut_alloc_2d (M, 4);
  // for (i = 0; i < M; i++)
    // ut_array_1d_memcpy (constraint[i], 4, (*pGeo).DomFaceEq[domainface[i]]);
  for (i = 0; i < M; i++)
    ut_array_1d_memcpy (constraint[i], 4, (*pGeo).DomFaceEq[tmp[i]]);

  if (verbosity > 0)
  {
    printf ("\n\n");
    printf ("newver = %d\n", newver);
    printf ("M = %d: ", M);
    ut_array_1d_int_fprintf (stdout, domainface, M, "%d");
    ut_array_2d_fprintf (stdout, constraint, M, 4, "%9.6f");
    printf ("\n\n");
  }

  A = gsl_matrix_alloc (3 + M, 3 + M);
  B = gsl_vector_alloc (3 + M);
  X = gsl_vector_alloc (3 + M);
  p = gsl_permutation_alloc (3 + M);

  gsl_matrix_set_zero (A);
  gsl_vector_set_zero (B);

  // Filling up the 3 x 3 first values of A
  double** NN = ut_alloc_2d (3, 3);
  double* n = NULL;
  double d;
  for (i = 0; i < N; i++)
  {
    n = (*pGeo).FaceEq[face[i]] + 1;
    for (j = 0; j < 3; j++)
      for (k = 0; k < 3; k++)
	NN[j][k] += n[j] * n[k];
  }

  for (j = 0; j < 3; j++)
    for (k = 0; k < 3; k++)
      gsl_matrix_set (A, j, k, NN[j][k]);

  // Filling up the 3 first values of B
  double* BB = ut_alloc_1d (3);
  for (i = 0; i < N; i++)
  {
    n = (*pGeo).FaceEq[face[i]] + 1;
    d = (*pGeo).FaceEq[face[i]][0];
    for (j = 0; j < 3; j++)
      BB[j] += d * n[j];
  }

  for (j = 0; j < 3; j++)
    gsl_vector_set (B, j, BB[j]);

  // Filling up the 3 x M (x 2) constraint values of A and
  // the M constraint values of B.
  double* nprime = NULL;
  double dprime;
  for (i = 0; i < M; i++)
  {
    nprime = constraint[i] + 1;
    dprime = constraint[i][0];
    for (j = 0; j < 3; j++)
    {
      gsl_matrix_set (A, i + 3, j, nprime[j]);
      gsl_matrix_set (A, j, i + 3, nprime[j]);
      gsl_vector_set (B, i + 3, dprime);
    }
  }

  if (verbosity >= 3)
  {
    printf ("\n\nA = \n");
    for (i = 0; i < 3 + M; i++)
    {
      for (j = 0; j < 3 + M; j++)
	printf ("%f ", gsl_matrix_get (A, i, j));
      printf ("\n");
    }
  }

  if (verbosity >= 3)
  {
  printf ("B = \n");
  for (i = 0; i < 3 + M; i++)
    printf ("%f\n", gsl_vector_get (B, i));
  }

  gsl_linalg_LU_decomp (A, p, &s);

  int status = 0;
  if (fabs (gsl_linalg_LU_det (A, s)) < 1e-15)
    status = -1;

  if (status == 0)
  {
    gsl_linalg_LU_solve (A, p, B, X);

    for (i = 0; i < 3; i++)
      (*pGeo).VerCoo[newver][i] = gsl_vector_get (X, i);
  }

  if (verbosity >= 3)
  {
    printf ("X = \n");
    for (i = 0; i < 3 + M; i++)
      printf ("%f\n", gsl_vector_get (X, i));
  }

  gsl_matrix_free (A);
  gsl_vector_free (B);
  gsl_vector_free (X);
  gsl_permutation_free (p);
  ut_free_1d_int (face);
  ut_free_2d (NN, 3);
  ut_free_1d (BB);
  ut_free_2d (constraint, M);
  ut_free_1d_int (domainface);
  ut_free_1d_int (tmp);

  return status;
}
