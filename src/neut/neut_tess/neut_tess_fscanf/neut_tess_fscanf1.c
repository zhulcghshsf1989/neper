/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_tess_fscanf_lcl.h"

void
neut_tess_fscanf (FILE* file, struct TESS* pTess)
{
  neut_tess_fscanf_verbosity (file, pTess, 0);
}

void
neut_tess_fscanf_verbosity (FILE* file, struct TESS* pTess, int verbosity)
{
  char* version = ut_alloc_1d_char (10);

  if ((*pTess).PolyQty > 0)
    neut_tess_free (pTess);

  neut_tess_fscanf_version (file, version);

  if (! strcmp (version, "1.9.2"))
  {
    neut_tess_fscanf_1p9 (file, pTess);
    (*pTess).N = (*pTess).PolyQty;
    (*pTess).DomType = ut_alloc_1d_char (5);
    strcpy ((*pTess).DomType, "cube");
    neut_tess_init_domain (pTess);
  }

  else if (! strcmp (version, "1.10"))
  {
    neut_tess_fscanf_1p10 (file, pTess);
    neut_tess_init_domain_facelabel (pTess);
  }

  else if (! strcmp (version, "2.0"))
  {
    char* tmp = ut_alloc_1d_char (100);
    neut_tess_fscanf_head (pTess, file);
    neut_tess_fscanf_ver (pTess, file);
    neut_tess_fscanf_edge (pTess, file);
    neut_tess_fscanf_face (pTess, file);
    neut_tess_fscanf_poly (pTess, file);

    ut_file_nextstring (file, tmp);
    if (! strcmp (tmp, "**domain"))
      neut_tess_fscanf_domain (pTess, file);

    neut_tess_fscanf_foot (file);
    neut_tess_init_edgelength (pTess);
  }

  else
  {
    ut_print_message (2, 2, "Unsupported tess file version `%s'.\n", version);
    abort ();
  }

  if (neut_tess_test (*pTess, verbosity) != 0)
  {
    ut_print_message (2, 0, "Tessellation checking failed - tessellation is not valid.\n");
    if (! verbosity)
      ut_print_message (2, 0, "Debug with option -checktess of neper -T.\n");

    abort ();
  }
  else
    if (verbosity) 
      ut_print_message (2, 0, "Tessellation checking succeeded.\n");

  ut_free_1d_char (version);

  return;
}

void
neut_tess_name_fscanf (char* name, struct TESS* pTess)
{
  FILE* file = ut_file_open (name, "r");
  neut_tess_fscanf (file, pTess);
  ut_file_close (file, name, "r");

  return;
}
