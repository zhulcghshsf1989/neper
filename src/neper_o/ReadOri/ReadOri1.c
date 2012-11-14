/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"ReadOri.h"

void
ReadOri (struct IN In, struct ORI *pOri)
{
  int i;
  FILE* file;

  (*pOri).N = ut_file_nbwords (In.load);

  file = ut_file_open (In.load, "r");
  if (! strcmp (In.input, "e"))
  {
    (*pOri).N /= 3;
    (*pOri).q = ut_alloc_2d ((*pOri).N + 1, 4);

    double* e = ol_e_alloc ();
    for (i = 1; i <= (*pOri).N; i++)
    {
      ol_e_fscanf (file, e);
      ol_e_q (e, (*pOri).q[i]);
    }
    ol_e_free (e);
  }
  else if (! strcmp (In.input, "ek"))
  {
    (*pOri).N /= 3;
    (*pOri).q = ut_alloc_2d ((*pOri).N + 1, 4);

    double* e = ol_e_alloc ();
    for (i = 1; i <= (*pOri).N; i++)
    {
      ol_e_fscanf (file, e);
      ol_ek_e (e, e);
      ol_e_q (e, (*pOri).q[i]);
    }
    ol_e_free (e);
  }
  else if (! strcmp (In.input, "er"))
  {
    (*pOri).N /= 3;
    (*pOri).q = ut_alloc_2d ((*pOri).N + 1, 4);

    double* e = ol_e_alloc ();
    for (i = 1; i <= (*pOri).N; i++)
    {
      ol_e_fscanf (file, e);
      ol_er_e (e, e);
      ol_e_q (e, (*pOri).q[i]);
    }
    ol_e_free (e);
  }
  else if (! strcmp (In.input, "g"))
  {
    (*pOri).N /= 9;
    (*pOri).q = ut_alloc_2d ((*pOri).N + 1, 4);

    double** g = ol_g_alloc ();
    for (i = 1; i <= (*pOri).N; i++)
    {
      ol_g_fscanf (file, g);
      ol_g_q (g, (*pOri).q[i]);
    }
    ol_g_free (g);
  }
  else if (! strcmp (In.input, "rtheta"))
  {
    (*pOri).N /= 4;
    (*pOri).q = ut_alloc_2d ((*pOri).N + 1, 4);

    double* r = ol_r_alloc ();
    double theta;
    for (i = 1; i <= (*pOri).N; i++)
    {
      ol_rtheta_fscanf (file, r, &theta);
      ol_rtheta_q (r, theta, (*pOri).q[i]);
    }
    ol_r_free (r);
  }
  else if (! strcmp (In.input, "R"))
  {
    (*pOri).N /= 3;
    (*pOri).q = ut_alloc_2d ((*pOri).N + 1, 4);

    double* R = ol_R_alloc ();
    for (i = 1; i <= (*pOri).N; i++)
    {
      ol_R_fscanf (file, R);
      ol_R_q (R, (*pOri).q[i]);
    }
    ol_R_free (R);
  }
  else if (! strcmp (In.input, "q"))
  {
    (*pOri).N /= 4;
    (*pOri).q = ut_alloc_2d ((*pOri).N + 1, 4);

    for (i = 1; i <= (*pOri).N; i++)
      ol_q_fscanf (file, (*pOri).q[i]);
  }
  else 
  {
    ut_print_message (2, 1, "Input type unavailable.\n");
    ut_error_reportbug ();
  }

  ut_file_close (file, In.load, "r");

  return;
}
