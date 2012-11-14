/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#include "ol_mean.h"

void
ol_set_mean (struct OL_SET set, char* crysym, double *qm)
{
  if (set.size > 0)
    ol_set_mean_ref (set, crysym, set.q[0], qm);

  return;
}

void
ol_set_mean_ref (struct OL_SET set, char* crysym, double *qref, double *qm)
{
  unsigned int i, j;
  double qnorm;
  double *qb = ol_q_alloc ();

  ut_array_1d_zero (qm, 4);

  for (i = 0; i < set.size; i++)
  {
    ol_q_q_qdisori_cur (qref, set.q[i], crysym, qb);

    for (j = 0; j < 4; j++)
      qm[j] += qb[j] * set.weight[i];
  }

  qnorm = ut_array_1d_norm (qm, 4);
  ut_array_1d_scale (qm, 4, 1 / qnorm);

  ol_q_q_q (qref, qm, qm);

  ol_q_free (qb);

  return;
}

int
ol_set_mean_iter (struct OL_SET set, char* crysym, double *qm)
{
  double *qb = ol_q_alloc ();
  double theta;
  int it, res = 0;

  it = 0;
  ol_q_memcpy (set.q[0], qb);

  do
  {
    ol_set_mean_ref (set, crysym, qb, qm);
    ol_q_q_disori (qb, qm, crysym, &theta);
    ol_q_memcpy (qm, qb);
    it++;
  }
  while (theta > 1e-1 && it <= 100);

  if (it > 100)
    res = -1;

  ol_q_free (qb);

  return res;
}

int
ol_set_mean_iter_ref (struct OL_SET set, char* crysym, double *qref, double *qm)
{
  double *qb = ol_q_alloc ();
  double theta;
  int it, res = 0;

  it = 0;
  ol_q_memcpy (qref, qb);

  do
  {
    ol_set_mean_ref (set, crysym, qb, qm);
    ol_q_q_disori (qb, qm, crysym, &theta);
    ol_q_memcpy (qm, qb);
    it++;
  }
  while (theta > 1e-1 && it <= 100);

  if (it > 100)
    res = -1;

  ol_q_free (qb);

  return res;
}
