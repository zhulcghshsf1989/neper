/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"CalcOri.h"

void
CalcOri (struct IN In, struct ORI *pOri)
{
  int i;
  double* e = ol_e_alloc ();
  gsl_rng *r;
  FILE *file;
  double n1, n2, n3;
  int idmax;
  double** q = NULL;
  struct TESS Tess;

  neut_tess_set_zero (&Tess);

  if ((*pOri).N == 0)
  {
    file = ut_file_open (In.tess, "r");
    neo_init_ori (file, pOri);
    ut_file_close (file, In.tess, "r");
  }
  else
  {
    // "-1" is absolutely necessary! and is to avoid correllation bw the
    // germ positions (module -T or -MM) and the germs orientations: we
    // take a different rng seed ('Random' value).
    (*pOri).rngid = ut_alloc_1d_int ((*pOri).N + 1);
    for (i = 1; i <= (*pOri).N; i++)
      (*pOri).rngid[i] = i;
  }

  (*pOri).Random = neut_rand_nnid2rand ((*pOri).N, (*pOri).id) - 1;

  idmax = ut_array_1d_int_max ((*pOri).rngid + 1, (*pOri).N);

  /* allocating q */
  q = ut_alloc_2d (idmax + 1, 4);
  r = gsl_rng_alloc (gsl_rng_ranlxd2);
  gsl_rng_set (r, (*pOri).Random);

  for (i = 1; i <= idmax; i++)
  {
    /* keep the n1 n2 n3 stuff for backward compatibility.  If gsl is
     * called as an argument, n1 and n3 are reversed. */
    n1 = gsl_rng_uniform (r);
    n2 = gsl_rng_uniform (r);
    n3 = gsl_rng_uniform (r);
    ol_nb_e (n1, n2, n3, e);
    ol_e_q (e, q[i]);
    if (strcmp (In.crysym, "triclinic") != 0)
      ol_q_qcrysym (q[i], In.crysym, q[i]);
  }
  
  gsl_rng_free (r);
  ol_e_free (e);

  // Recording orientations
  (*pOri).q = ut_alloc_2d ((*pOri).N + 1, 4);

  for (i = 1; i <= (*pOri).N; i++)
    ut_array_1d_memcpy ((*pOri).q[i], 4, q[(*pOri).rngid[i]]);

  ut_free_2d (q, idmax + 1);
  neut_tess_free (&Tess);

  return;
}
