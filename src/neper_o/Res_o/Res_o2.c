/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "Res_o.h"

void
WriteOri (struct IN In, struct ORI Ori, char *format, FILE * file)
{
  int i, j;

  // plain format
  if (strcmp (format, "plain") == 0)
  {
    if (strcmp (In.descriptor, "e") == 0)
    {
      double* e = ol_e_alloc ();
      for (i = 1; i <= Ori.N; i++)
      {
	ol_q_e  (Ori.q[i], e);
	if (strcmp (In.crysym, "triclinic") != 0)
	  ol_e_ecrysym (e, In.crysym, e);
	ol_e_fprintf (file, e, "%17.12f");
      }
      ol_e_free (e);
    }
    else if (strcmp (In.descriptor, "ek") == 0)
    {
      double* e = ol_e_alloc ();
      for (i = 1; i <= Ori.N; i++)
      {
	ol_q_e  (Ori.q[i], e);
	ol_e_ek (e, e);
	ol_e_fprintf (file, e, "%17.12f");
      }
      ol_e_free (e);
    }
    else if (strcmp (In.descriptor, "er") == 0)
    {
      double* e = ol_e_alloc ();
      for (i = 1; i <= Ori.N; i++)
      {
	ol_q_e  (Ori.q[i], e);
	ol_e_er (e, e);
	ol_e_fprintf (file, e, "%17.12f");
      }
      ol_e_free (e);
    }
    else if (strcmp (In.descriptor, "g") == 0)
    {
      double** g = ol_g_alloc ();
      for (i = 1; i <= Ori.N; i++)
      {
	ol_q_g (Ori.q[i], g);
	if (strcmp (In.crysym, "triclinic") != 0)
	  ol_g_gcrysym (g, In.crysym, g);
	ol_g_fprintf (file, g, "%17.12f");
      }
      ol_g_free (g);
    }
    else if (strcmp (In.descriptor, "rtheta") == 0)
    {
      double* r = ol_r_alloc ();
      double theta;
      for (i = 1; i <= Ori.N; i++)
      {
	ol_q_rtheta (Ori.q[i], r, &theta);
	if (strcmp (In.crysym, "triclinic") != 0)
	  ol_rtheta_rthetacrysym (r, theta, In.crysym, r, &theta);
	ol_rtheta_fprintf (file, r, theta, "%17.12f");
      }
      ol_r_free (r);
    }
    else if (strcmp (In.descriptor, "R") == 0)
    {
      double* R = ol_R_alloc ();
      for (i = 1; i <= Ori.N; i++)
      {
	ol_q_R (Ori.q[i], R);
	if (strcmp (In.crysym, "triclinic") != 0)
	  ol_R_Rcrysym (R, In.crysym, R);
	ol_R_fprintf (file, R, "%17.12f");
      }
      ol_R_free (R);
    }
    else if (strcmp (In.descriptor, "q") == 0)
    {
      for (i = 1; i <= Ori.N; i++)
      {
	if (strcmp (In.crysym, "triclinic") != 0)
	  ol_R_Rcrysym (Ori.q[i], In.crysym, Ori.q[i]);
	ol_q_fprintf (file, Ori.q[i], "%17.12f");
      }
    }
    else 
    {
      ut_print_message (2, 1, "Format %s not implemented\n", In.descriptor);
      ut_error_reportbug ();
    }
  }

  // fepx format
  else if (strcmp (format, "fepx") == 0)
  {
    fprintf (file, "grain-orientations\n%d\n", Ori.N);
    double* e = ol_e_alloc ();
    for (i = 1; i <= Ori.N; i++)
    {
      ol_q_e  (Ori.q[i], e);
      ol_e_ek (e, e);
      for (j = 0; j < 3; j++)
	fprintf (file, "%17.12f ", e[j]);
      fprintf (file, "%d\n", i);
    }
    ol_e_free (e);
    fprintf (file, "EOF\n");
  }

  // geof format
  else if (strcmp (format, "geof") == 0)
  {
    double* e = ol_e_alloc ();
    for (i = 1; i <= Ori.N; i++)
    {
      fprintf (file, "**elset poly%d  *rotation ", i);
      ol_q_e  (Ori.q[i], e);
      ol_e_e  (e, e);
      for (j = 0; j < 3; j++)
        fprintf (file, "%17.12f ", e[j]);
      fprintf (file, "\n");
    }
    ol_e_free (e);
  }

  return;
}
