/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"InputData_t.h"

/* SetDefaultOptions set the options to their default values */
void
SetDefaultOptions_t (struct IN *pIn, struct GERMSET *pGermSet)
{
  (*pIn).libpath = NULL;

  (*pGermSet).morpho = ut_alloc_1d_char (10);
  sprintf ((*pGermSet).morpho, "poisson");

  (*pIn).domain = ut_alloc_1d_char (10);
  sprintf ((*pIn).domain, "cube");
  (*pIn).domainparms = ut_alloc_1d (3);
  (*pIn).domainparms[0] = 1;
  (*pIn).domainparms[1] = 1;
  (*pIn).domainparms[2] = 1;
  (*pIn).domainparms2 = NULL;

  (*pIn).F = ut_alloc_1d (3);	/* transformation after tessellation */

  (*pIn).F[0] = 1;
  (*pIn).F[1] = 1;
  (*pIn).F[2] = 1;

  (*pIn).verbosity = 0;
  (*pIn).body = NULL;

  (*pGermSet).N = -1;
  (*pGermSet).Id = -1;
  (*pGermSet).ttype = ut_alloc_1d_char (9);
  strcpy ((*pGermSet).ttype, "standard");
  (*pGermSet).randomize = 0;
  (*pGermSet).randomize2 = 0;
  (*pGermSet).randomizedir = ut_alloc_1d_char (10);
  sprintf ((*pGermSet).randomizedir, "default");

  (*pIn).input = -1;
  (*pIn).geo = NULL;
  (*pIn).format = ut_alloc_1d_char (100);
  sprintf ((*pIn).format, "tess,oin");
  (*pIn).load = NULL;
  (*pIn).stt0 = NULL;
  (*pIn).stt3 = NULL;

  (*pIn).printpointpoly = 0;
  (*pIn).printneighbour = 0;
  (*pIn).point = ut_alloc_1d_char (100);
  (*pIn).polyid = ut_alloc_1d_char (100);
  
  (*pIn).stattess = 0;
  (*pIn).sorttess_qty = 0;

  (*pIn).centroid = 0;
  (*pIn).centroiditermax = 1000;
  (*pIn).centroidconv = 2 * 1e-2;
  (*pIn).centroidfact = 0.5;
  
  return;
}

void
SetOptions_t (struct IN *pIn, struct GERMSET *pGermSet,
	      int argc, char **argv)
{
  int i, status, ArgQty, Res;
  char **ArgList = ut_alloc_2d_char (101, 101);
  char *Arg = ut_alloc_1d_char (101);

  // Argument list =====================================================

  ArgQty = 0;
  // Input data --------------------------------------------------------
  sprintf (ArgList[++ArgQty], "-n");
  sprintf (ArgList[++ArgQty], "-id");
  sprintf (ArgList[++ArgQty], "-morpho");
  sprintf (ArgList[++ArgQty], "-centroid");
  sprintf (ArgList[++ArgQty], "-centrecoo");
  sprintf (ArgList[++ArgQty], "-centercoo");
  sprintf (ArgList[++ArgQty], "-gcoo");
  sprintf (ArgList[++ArgQty], "-regular");
  // General options ---------------------------------------------------
  sprintf (ArgList[++ArgQty], "-o");
  sprintf (ArgList[++ArgQty], "-v");
  sprintf (ArgList[++ArgQty], "-libpath");
  // Tessellation options ----------------------------------------------
  sprintf (ArgList[++ArgQty], "-domain");
  sprintf (ArgList[++ArgQty], "-cylinderfacet");
  sprintf (ArgList[++ArgQty], "-dsize");
  sprintf (ArgList[++ArgQty], "-scale");
  sprintf (ArgList[++ArgQty], "-randomize");
  sprintf (ArgList[++ArgQty], "-randomizedir");
  sprintf (ArgList[++ArgQty], "-centroidfact");
  sprintf (ArgList[++ArgQty], "-centroidconv");
  sprintf (ArgList[++ArgQty], "-centroiditermax");
  // Output options ----------------------------------------------------
  sprintf (ArgList[++ArgQty], "-format");
  // Post-processing ---------------------------------------------------
  sprintf (ArgList[++ArgQty], "-stat");
  sprintf (ArgList[++ArgQty], "-stattess");
  sprintf (ArgList[++ArgQty], "-sort");
  sprintf (ArgList[++ArgQty], "-sorttess");
  sprintf (ArgList[++ArgQty], "-pointpoly");
  sprintf (ArgList[++ArgQty], "-neighbour");
  sprintf (ArgList[++ArgQty], "-neighbor");
  // Restart a job -----------------------------------------------------
  sprintf (ArgList[++ArgQty], "-loadtess");

  // Treating arguments ================================================
  
  for (i = 1; i < argc; i++)
  {
    if (argv[i][0] != '-')
      ut_arg_badarg ();

    /* Searching option name (string completion stuff) */
    Res = ut_string_comp (argv[i], ArgList, ArgQty, Arg);
    if (Res == 1)
    {
      ut_print_lineheader (2);
      printf ("Several possibilities for option `%s'.\n", argv[i]);
      ut_arg_badarg ();
    }
    else if (Res == -1)
    {
      ut_print_lineheader (2);
      printf ("Unknown option `%s'.\n", argv[i]);
      ut_arg_badarg ();
    }

/*---------------------------------------------------------------------- 
* input data */
    status = SetOptions_t_inputdata (pGermSet, argc, argv, &i, Arg, &(*pIn).input);

    if (status == 0)
      continue;

/*---------------------------------------------------------------------- 
* general options */
    else if (strcmp (Arg, "-o") == 0 && i < argc - 1)
    {
      (*pIn).body = ut_arg_nextaschar (argv, &i, Arg);
      ut_string_body ((*pIn).body, (*pIn).body);
    }
    else if (strcmp (Arg, "-v") == 0 && i < argc - 1)
      (*pIn).verbosity = ut_arg_nextasint (argv, &i, Arg, 0, 1);
    else if (strcmp (Arg, "-libpath") == 0 && i < argc - 1)
      (*pIn).libpath = ut_arg_nextaschar (argv, &i, Arg);

/*---------------------------------------------------------------------- 
* tessellation options */
    else if (strcmp (Arg, "-domain") == 0 && i < argc - 1)
    {
      ut_free_1d_char ((*pIn).domain);
      (*pIn).domain = ut_arg_nextaschar (argv, &i, Arg);
      if (! strcmp ((*pIn).domain, "cube"))
      {
	(*pIn).domainparms[0] = ut_arg_nextasreal (argv, &i, Arg, 0, DBL_MAX);
	(*pIn).domainparms[1] = ut_arg_nextasreal (argv, &i, Arg, 0, DBL_MAX);
	(*pIn).domainparms[2] = ut_arg_nextasreal (argv, &i, Arg, 0, DBL_MAX);
      }
      else if (! strcmp ((*pIn).domain, "cylinder"))
      {
	(*pIn).domainparms[0] = ut_arg_nextasreal (argv, &i, Arg, 0, DBL_MAX);
	(*pIn).domainparms[1] = ut_arg_nextasreal (argv, &i, Arg, 0, DBL_MAX);
	(*pIn).domainparms[2] = -1;
      }
      else if (! strcmp ((*pIn).domain, "planes"))
	(*pIn).domainparms2   = ut_arg_nextaschar (argv, &i, Arg);
      else if (! strcmp ((*pIn).domain, "tesspoly"))
      {
	(*pIn).domainparms2   = ut_arg_nextaschar (argv, &i, Arg);
	(*pIn).domainparms[0] = ut_arg_nextasreal (argv, &i, Arg, 0, DBL_MAX);
      }
      else
      {
	ut_arg_badarg ();
	abort ();
      }
    }
    else if (strcmp (Arg, "-cylinderfacet") == 0 && i < argc - 1)
      (*pIn).domainparms[2] = ut_arg_nextasreal (argv, &i, Arg, 0, DBL_MAX);
    else if (strcmp (Arg, "-dsize") == 0 && i < argc - 3)
    {
      (*pIn).domainparms[0] = ut_arg_nextasreal (argv, &i, Arg, 0, DBL_MAX);
      (*pIn).domainparms[1] = ut_arg_nextasreal (argv, &i, Arg, 0, DBL_MAX);
      (*pIn).domainparms[2] = ut_arg_nextasreal (argv, &i, Arg, 0, DBL_MAX);
      ut_print_messagewnc (1, 72, "Option -dsize is depracated and will be removed from future releases.  Use option `-domain' instead.");
    }
    else if (strcmp (Arg, "-scale") == 0 && i < argc - 3)
    {
      (*pIn).F[0] = ut_arg_nextasreal (argv, &i, Arg, 0, DBL_MAX);
      (*pIn).F[1] = ut_arg_nextasreal (argv, &i, Arg, 0, DBL_MAX);
      (*pIn).F[2] = ut_arg_nextasreal (argv, &i, Arg, 0, DBL_MAX);
    }
    else if (strcmp (Arg, "-randomize") == 0 && i < argc - 2)
    {
      (*pGermSet).randomize = ut_arg_nextasreal (argv, &i, Arg, 0, DBL_MAX);
      (*pGermSet).randomize2 = ut_arg_nextasint (argv, &i, Arg, 0, INT_MAX);
    }
    else if (strcmp (Arg, "-randomizedir") == 0 && i < argc - 1)
      (*pGermSet).randomizedir = ut_arg_nextaschar (argv, &i, Arg);
    else if (strcmp (Arg, "-centroid") == 0 && i < argc - 1)
      (*pIn).centroid = ut_arg_nextasint (argv, &i, Arg, 0, 1);
    else if (strcmp (Arg, "-centroidfact") == 0 && i < argc - 1)
      (*pIn).centroidfact = ut_arg_nextasreal (argv, &i, Arg, 0, 1);
    else if (strcmp (Arg, "-centroidconv") == 0 && i < argc - 1)
      (*pIn).centroidconv = ut_arg_nextasreal (argv, &i, Arg, 0, DBL_MAX);
    else if (strcmp (Arg, "-centroiditermax") == 0 && i < argc - 1)
      (*pIn).centroiditermax = ut_arg_nextasint (argv, &i, Arg, 1, INT_MAX);

/*---------------------------------------------------------------------- 
* post-processing options */
    else if ((strcmp (Arg, "-neighbour") == 0 && i < argc - 1)
	  || (strcmp (Arg, "-neighbor")  == 0 && i < argc - 1))
      (*pIn).printneighbour = ut_arg_nextasint (argv, &i, Arg, 0, 1);
    else if (strcmp (Arg, "-pointpoly") == 0 && i < argc - 1)
    {
      (*pIn).printpointpoly = 1;
      (*pIn).point = ut_arg_nextaschar (argv, &i, Arg);
    }
    /*
    else if (strcmp (Arg, "-print") == 0 && i < argc - 1)
    {
      (*pIn).printentity = ut_arg_nextaschar (argv, &i, Arg);
      tmp = ut_arg_nextasint (argv, &i, Arg, 1, INT_MAX);
      (*pIn).printlist = ut_alloc_1d_int (tmp + 1);
      (*pIn).printlist[0] = tmp;
      for (j = 1; j <= (*pIn).printlist[0]; j++)
	(*pIn).printlist[j] = ut_arg_nextasint (argv, &i, Arg, 1, INT_MAX);
    }
    else if (strcmp (Arg, "-printformat") == 0 && i < argc - 1)
      (*pIn).printformat = ut_arg_nextaschar (argv, &i, Arg);
    */
    else if (strcmp (Arg, "-format") == 0 && i < argc - 1)
    {
      ut_free_1d_char ((*pIn).format);
      (*pIn).format = ut_arg_nextaschar (argv, &i, Arg);
    }
    else if (strcmp (Arg, "-stattess") == 0 || strcmp (Arg, "-stat") == 0)
      (*pIn).stattess = ut_arg_nextasint (argv, &i, Arg, 0, 1);
    else if ((strcmp (Arg, "-sorttess") == 0 ||
	      strcmp (Arg, "-sort") == 0   ) && i < argc - 2) 
    {
      (*pIn).sorttess_qty = 2;
      (*pIn).sorttess = ut_alloc_1d_pchar (2);
      (*pIn).sorttess[0] = ut_arg_nextaschar (argv, &i, Arg);
      (*pIn).sorttess[1] = ut_arg_nextaschar (argv, &i, Arg);
    }

/*---------------------------------------------------------------------- 
* load capability */
    else if (strcmp (Arg, "-loadtess") == 0 && i < argc - 1)
    {
      (*pIn).input = 1;
      (*pIn).load = ut_arg_nextaschar (argv, &i, Arg);
    }
    else
    {
      ut_arg_badarg ();
      abort ();
    }
  }

  ut_free_1d_char (Arg);
  ut_free_2d_char (ArgList, 101);

  return;
}
