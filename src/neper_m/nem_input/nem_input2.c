/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_input.h"

void
nem_input_treatargs (int fargc, char **fargv, int argc, char **argv,
		     struct IN *pIn, struct MESHPARA *pMeshPara)
{
  /* Setting default options */
  nem_input_options_default (pIn, pMeshPara);

  /* Reading options. */
  nem_input_options_set (pIn, pMeshPara, fargc, fargv);
  nem_input_options_set (pIn, pMeshPara, argc, argv);

  // If not set up, setting dim default value
  if ((*pIn).meshdim == -1)
  {
    if (! (*pIn).outdim)
      (*pIn).meshdim = 3;
    else
    {
      char** parts = NULL;
      int partqty, i, tmp;
      ut_string_separate ((*pIn).outdim, ',', &parts, &partqty);

      for (i = 0; i < partqty; i++)
      {
	sscanf (parts[i], "%d", &tmp);
	(*pIn).meshdim = ut_num_max ((*pIn).meshdim, tmp);

      }
      ut_free_2d_char (parts, partqty);
    }
  }

  // If not set up, setting outdim default value
  if (! (*pIn).outdim)
  {
    int i;
    (*pIn).outdim = ut_alloc_1d_char (10);
    for (i = 0; i <= (*pIn).meshdim; i++)
      sprintf ((*pIn).outdim + strlen ((*pIn).outdim), "%d,", i);
    (*pIn).outdim[strlen ((*pIn).outdim) - 1] = '\0';
  }

  if (ut_file_exist ((*pIn).gmsh) == 0)
  {
    ut_print_message (2, 0, "You have to specify a valid access path to the gmsh binary\n");
    ut_print_message (2, 0, "through option `-gmsh'.\n");
    abort ();
  }
  else
  {
    char* tmp = ut_alloc_1d_char (1000);
    sprintf (tmp, "%s --version 2>&1 | grep -v \"[a-z]\" > .nepertmp", (*pIn).gmsh);
    if (system (tmp) == -1)
      abort ();

    FILE* file = ut_file_open (".nepertmp", "R");
    if (fscanf (file, "%s", tmp) != 1)
    {
      ut_print_message (2, 0, "You have to specify a valid access path to the gmsh binary\n");
      ut_print_message (2, 0, "through option `-gmsh'.\n");
      abort ();
    }
    ut_file_close (file, ".nepertmp", "R");
    remove (".nepertmp");
    ut_free_1d_char (tmp);
  }

#ifndef HAVE_LIBSCOTCH
  if ((*pIn).partmode >= 0)
  {
    ut_print_messagewnc (2, 72, "Options `-part*' (mesh partitioning) are not available since this version of Neper has been built without libscotch.");
    abort ();
  }
#endif

  /* Recording body */
  if ((*pIn).body == NULL)
  {
    if ((*pIn).tess != NULL && strlen ((*pIn).tess) != 0)
    {
      (*pIn).body = ut_alloc_1d_char (strlen ((*pIn).tess) + 1);
      ut_string_body ((*pIn).tess, (*pIn).body);
    }
    else if ((*pIn).vox != NULL && strlen ((*pIn).vox) != 0)
    {
      (*pIn).body = ut_alloc_1d_char (strlen ((*pIn).vox) + 1);
      ut_string_body ((*pIn).vox, (*pIn).body);
    }
    else if ((*pIn).loadmesh != NULL)
    {
      (*pIn).body = ut_alloc_1d_char (strlen ((*pIn).loadmesh) + 1);
      ut_string_body ((*pIn).loadmesh, (*pIn).body);
    }
    else if ((*pIn).mesh != NULL)
    {
      (*pIn).body = ut_alloc_1d_char (strlen ((*pIn).mesh) + 1);
      ut_string_body ((*pIn).mesh, (*pIn).body);
    }
  }

  /* Writing all file names */
  (*pIn).geof    = ut_string_addextension ((*pIn).body, ".geof");
  (*pIn).msh     = ut_string_addextension ((*pIn).body, ".msh");
  (*pIn).abq     = ut_string_addextension ((*pIn).body, ".inp");
  (*pIn).epart   = ut_string_addextension ((*pIn).body, ".epart");
  (*pIn).npart   = ut_string_addextension ((*pIn).body, ".npart");
  (*pIn).fepx1   = ut_string_addextension ((*pIn).body, ".parms");
  (*pIn).fepx2   = ut_string_addextension ((*pIn).body, ".mesh");
  (*pIn).fepx3   = ut_string_addextension ((*pIn).body, ".surf");
  (*pIn).fepx4   = ut_string_addextension ((*pIn).body, ".opt");
  (*pIn).fepx5   = ut_string_addextension ((*pIn).body, ".bcs");

  return;
}
