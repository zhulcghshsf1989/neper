/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"InputData_fm.h"

void
TreatArg_fm (int fargc, char **fargv, int argc, char **argv,
	     struct IN *pIn, struct GEOPARA *pGeoPara)
{
  /* Setting default options */
  SetDefaultOptions_fm (pIn, pGeoPara);

  /* Reading options. */
  SetOptions_fm (pIn, pGeoPara, fargc, fargv);
  SetOptions_fm (pIn, pGeoPara, argc, argv);

  (*pIn).mesh = 0;
  if ((*pIn).ingeo != NULL && 
     (ut_string_inlist ((*pIn).format, ',', "msh")
   || ut_string_inlist ((*pIn).format, ',', "geof")
   || ut_string_inlist ((*pIn).format, ',', "abq")
   || ut_string_inlist ((*pIn).format, ',', "inp")
   || ut_string_inlist ((*pIn).format, ',', "fev")
   ))
    (*pIn).mesh = 1;
  
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
    system (tmp);
    FILE* file = ut_file_open (".nepertmp", "R");
    if (fscanf (file, "%s%s", tmp, tmp) != 1)
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
    if ((*pIn).ingeo != NULL && strlen ((*pIn).ingeo) != 0)
    {
      (*pIn).body = ut_alloc_1d_char (strlen ((*pIn).ingeo) + 1);
      ut_string_body ((*pIn).ingeo, (*pIn).body);
    }
    else if ((*pIn).loadmesh != NULL)
    {
      (*pIn).body = ut_alloc_1d_char (strlen ((*pIn).loadmesh) + 1);
      ut_string_body ((*pIn).loadmesh, (*pIn).body);
    }
    else if ((*pIn).remesh == 1)
    {
      (*pIn).body = ut_alloc_1d_char (strlen ((*pIn).remesh2) + 1);
      ut_string_body ((*pIn).remesh2, (*pIn).body);
    }
  }


  /* Writing all file names */
  (*pIn).geo     = ut_string_addextension ((*pIn).body, ".tess");
  (*pIn).ply     = ut_string_addextension ((*pIn).body, ".ply");
  (*pIn).gmshgeo = ut_string_addextension ((*pIn).body, ".geo");

  (*pIn).geof    = ut_string_addextension ((*pIn).body, ".geof");
  (*pIn).scm     = ut_string_addextension ((*pIn).body, ".scm");
  (*pIn).scg     = ut_string_addextension ((*pIn).body, ".scg");
  
  (*pIn).stn     = ut_string_addextension ((*pIn).body, ".stn");
  (*pIn).stm1    = ut_string_addextension ((*pIn).body, ".stm1");
  (*pIn).stm2    = ut_string_addextension ((*pIn).body, ".stm2");
  (*pIn).stm3    = ut_string_addextension ((*pIn).body, ".stm3");
  (*pIn).stm4    = ut_string_addextension ((*pIn).body, ".stm4");
  (*pIn).stm5    = ut_string_addextension ((*pIn).body, ".stm5");

  (*pIn).ff      = ut_string_addextension ((*pIn).body, ".ff");
  (*pIn).stt3    = ut_string_addextension ((*pIn).body, ".stt3");
  (*pIn).stt2    = ut_string_addextension ((*pIn).body, ".stt2");
  (*pIn).stt1    = ut_string_addextension ((*pIn).body, ".stt1");
  (*pIn).stt0    = ut_string_addextension ((*pIn).body, ".stt0");
  (*pIn).stt2a   = ut_string_addextension ((*pIn).body, ".stt2a");
  (*pIn).msh     = ut_string_addextension ((*pIn).body, ".msh");
  (*pIn).abq     = ut_string_addextension ((*pIn).body, ".inp");
  (*pIn).oin     = ut_string_addextension ((*pIn).body, ".oin");
  (*pIn).mast    = ut_string_addextension ((*pIn).body, ".mast");
  (*pIn).asy     = ut_string_addextension ((*pIn).body, ".asy");
  (*pIn).epart   = ut_string_addextension ((*pIn).body, ".epart");
  (*pIn).npart   = ut_string_addextension ((*pIn).body, ".npart");
  (*pIn).rem1    = ut_string_addextension ((*pIn).body, ".rem1");
  (*pIn).fev1    = ut_string_addextension ((*pIn).body, ".parms");
  (*pIn).fev2    = ut_string_addextension ((*pIn).body, ".mesh");
  (*pIn).fev3    = ut_string_addextension ((*pIn).body, ".surf");
  (*pIn).fev4    = ut_string_addextension ((*pIn).body, ".opt");
  (*pIn).fev5    = ut_string_addextension ((*pIn).body, ".bcs");
  (*pIn).stenode = ut_string_addextension ((*pIn).body, ".node");
  (*pIn).steele  = ut_string_addextension ((*pIn).body, ".ele");

  return;
}
