/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#include "ol_com.h"

void
ol_g_g_g (double **g1, double **g2, double **g3)
{
  int i, j, k;
  double **gcpy = ol_g_alloc ();

  ol_g_set_zero (gcpy);
  for (i = 0; i <= 2; i++)
    for (j = 0; j <= 2; j++)
      for (k = 0; k <= 2; k++)
	gcpy[i][j] += g2[i][k] * g1[k][j];

  ol_g_memcpy (gcpy, g3);

  ol_g_free (gcpy);

  return;
}

void
ol_g_g_g_cur (double **g1, double **g2, double **g3)
{
  ol_g_g_g (g1, g2, g3);

  return;
}

void
ol_g_g_g_ref (double **g1, double **g2, double **g3)
{
  ol_g_g_g (g2, g1, g3);

  return;
}

void
ol_q_q_q (double *q1, double *q2, double *q3)
{
  double *qcpy = ol_q_alloc ();

  qcpy[0] = q1[0] * q2[0] - q1[1] * q2[1] - q1[2] * q2[2] - q1[3] * q2[3];
  qcpy[1] = q1[0] * q2[1] + q1[1] * q2[0] + q1[2] * q2[3] - q1[3] * q2[2];
  qcpy[2] = q1[0] * q2[2] - q1[1] * q2[3] + q1[2] * q2[0] + q1[3] * q2[1];
  qcpy[3] = q1[0] * q2[3] + q1[1] * q2[2] - q1[2] * q2[1] + q1[3] * q2[0];

  ol_q_memcpy (qcpy, q3);

  ol_q_free (qcpy);

  return;
}

void
ol_q_q_q_cur (double *q1, double *q2, double *q3)
{
  ol_q_q_q (q1, q2, q3);

  return;
}

void
ol_q_q_q_ref (double *q1, double *q2, double *q3)
{
  ol_q_q_q (q2, q1, q3);

  return;
}

void
ol_R_R_R (double* R1, double* R2, double* R3)
{
  double* q1 = ol_q_alloc ();
  double* q2 = ol_q_alloc ();
  double* q3 = ol_q_alloc ();

  ol_R_q (R1, q1);
  ol_R_q (R2, q2);
  
  ol_q_q_q (q1, q2, q3);
  ol_q_R (q3, R3);

  ol_q_free (q1);
  ol_q_free (q2);
  ol_q_free (q3);

  return;
}

void
ol_R_R_R_cur (double *R1, double *R2, double *R3)
{
  ol_R_R_R (R1, R2, R3);

  return;
}

void
ol_R_R_R_ref (double *R1, double *R2, double *R3)
{
  ol_R_R_R (R2, R1, R3);

  return;
}

void
ol_g_vect_vect (double **g, double *v1, double *v2)
{
  int i, j;
  double vcpy[3];

  for (i = 0; i < 3; i++)
  {
    vcpy[i] = 0;
    for (j = 0; j < 3; j++)
      vcpy[i] += g[i][j] * v1[j];
  }

  for (i = 0; i < 3; i++)
    v2[i] = vcpy[i];

  return;
}
