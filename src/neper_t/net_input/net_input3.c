/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_input.h"

/* SetDefaultOptions set the options to their default values */
void
net_input_options_default (struct IN *pIn, struct REG* pReg)
{
  (*pIn).morpho = ut_alloc_1d_char (10);
  sprintf ((*pIn).morpho, "poisson");

  (*pIn).domain = ut_alloc_1d_char (10);
  sprintf ((*pIn).domain, "cube");
  (*pIn).domainparms = ut_alloc_1d (3);
  (*pIn).domainparms[0] = 1;
  (*pIn).domainparms[1] = 1;
  (*pIn).domainparms[2] = 1;
  (*pIn).domainparms2 = NULL;
  
  (*pIn).scale = NULL;

  (*pIn).voxsizetype = 1;
  (*pIn).voxsize = 20;
  (*pIn).voxsize3 = NULL;
  (*pIn).body = NULL;

  (*pIn).n = -1;
  (*pIn).id = -1;
  (*pIn).ttype = ut_alloc_1d_char (9);
  strcpy ((*pIn).ttype, "standard");
  (*pIn).randomize = 0;
  (*pIn).randomize2 = 0;
  (*pIn).randomizedir = ut_alloc_1d_char (10);
  sprintf ((*pIn).randomizedir, "default");

  (*pIn).mode = NULL;
  (*pIn).input = NULL;
  (*pIn).tess = NULL;
  (*pIn).format = ut_alloc_1d_char (100);
  sprintf ((*pIn).format, "tess");
  (*pIn).voxformat = NULL;
  (*pIn).load = NULL;

  (*pIn).printpointpoly = 0;
  (*pIn).point = ut_alloc_1d_char (100);
  (*pIn).polyid = ut_alloc_1d_char (100);
  
  (*pIn).stv = NULL;
  (*pIn).ste = NULL;
  (*pIn).stf = NULL;
  (*pIn).stp = NULL;

  (*pIn).sorttess_qty = 0;

  (*pIn).centroid = 0;
  (*pIn).centroiditermax = 1000;
  (*pIn).centroidconv = 2 * 1e-2;
  (*pIn).centroidfact = 0.5;

  (*pIn).voxsizetype = 1;
  (*pIn).voxsize     = 20;
  (*pIn).voxsize3    = ut_alloc_1d_int (3);
  (*pIn).voxsize3[0] = 20;
  (*pIn).voxsize3[1] = 20;
  (*pIn).voxsize3[2] = 20;

  (*pReg).mloop = 2;
  (*pReg).maxedgedelqty = INT_MAX;
  (*pReg).maxff = 0;
  (*pReg).seltype = 1; // 0: sel, 1: rsel
  (*pReg).sel = -1;
  (*pReg).rsel = 1;
  (*pReg).dboundseltype = 1; // 0: sel, 1: rsel
  (*pReg).dboundsel = -1;
  (*pReg).dboundrsel = 1;
  (*pReg).dbound = NULL;
  
  return;
}

void
net_input_options_set (struct IN *pIn, struct REG* pReg, int argc, char **argv)
{
  int i, ArgQty, Res;
  char **ArgList = ut_alloc_2d_char (101, 101);
  char *Arg = ut_alloc_1d_char (101);

  // Argument list =====================================================

  ArgQty = 0;
  // Input data --------------------------------------------------------
  sprintf (ArgList[++ArgQty], "-n");
  sprintf (ArgList[++ArgQty], "-id");
  sprintf (ArgList[++ArgQty], "-morpho");
  sprintf (ArgList[++ArgQty], "-ttype");
  sprintf (ArgList[++ArgQty], "-centroid");
  // General options ---------------------------------------------------
  sprintf (ArgList[++ArgQty], "-o");
  sprintf (ArgList[++ArgQty], "-v");
  sprintf (ArgList[++ArgQty], "-libpath");
  // net_poly_tesl options ----------------------------------------------
  sprintf (ArgList[++ArgQty], "-domain");
  sprintf (ArgList[++ArgQty], "-cylinderfacet");
  sprintf (ArgList[++ArgQty], "-scale");
  sprintf (ArgList[++ArgQty], "-randomize");
  sprintf (ArgList[++ArgQty], "-randomizedir");
  sprintf (ArgList[++ArgQty], "-centroidfact");
  sprintf (ArgList[++ArgQty], "-centroidconv");
  sprintf (ArgList[++ArgQty], "-centroiditermax");
  // Regularization options --------------------------------------------
  sprintf (ArgList[++ArgQty], "-maxff");
  sprintf (ArgList[++ArgQty], "-sel");
  sprintf (ArgList[++ArgQty], "-rsel");
  sprintf (ArgList[++ArgQty], "-mloop");
  sprintf (ArgList[++ArgQty], "-maxedgedelqty");
  sprintf (ArgList[++ArgQty], "-dbound");
  sprintf (ArgList[++ArgQty], "-dboundsel");
  sprintf (ArgList[++ArgQty], "-dboundrsel");
  // Output options ----------------------------------------------------
  sprintf (ArgList[++ArgQty], "-format");
  sprintf (ArgList[++ArgQty], "-voxformat");
  sprintf (ArgList[++ArgQty], "-voxsize");
  sprintf (ArgList[++ArgQty], "-voxsize3");
  // Post-processing ---------------------------------------------------
  sprintf (ArgList[++ArgQty], "-statver");
  sprintf (ArgList[++ArgQty], "-statedge");
  sprintf (ArgList[++ArgQty], "-statface");
  sprintf (ArgList[++ArgQty], "-statpoly");
  sprintf (ArgList[++ArgQty], "-sort");
  sprintf (ArgList[++ArgQty], "-sorttess");
  sprintf (ArgList[++ArgQty], "-pointpoly");
  // Restart a job -----------------------------------------------------
  sprintf (ArgList[++ArgQty], "-loadtess");
  sprintf (ArgList[++ArgQty], "-loadvox");
  // Debugging ---------------------------------------------------------
  sprintf (ArgList[++ArgQty], "-checktess");

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
    if (strcmp (Arg, "-n") == 0 && i < argc - 1)
    {
      if ((*pIn).input == NULL)
      {
	(*pIn).input = ut_alloc_1d_char (5);
	strcpy ((*pIn).input, "n");
	(*pIn).n = ut_arg_nextasint (argv, &i, Arg, 1, INT_MAX);
	
	if ((strcmp ((*pIn).morpho, "cube"  ) == 0)
	 || (strcmp ((*pIn).morpho, "dodeca") == 0)
	 || (strcmp ((*pIn).morpho, "tocta" ) == 0))
	  strcpy ((*pIn).input, "n_reg");
      }
      else
	ut_arg_badarg ();
    }
    else if (strcmp (Arg, "-id") == 0 && i < argc - 1)
    {
      if ((*pIn).id == 0)
      {
	ut_print_message (2, 0,
			  "Functions `-id' and `-morpho' are mutually exclusive!");
	abort ();
      }
      (*pIn).id = ut_arg_nextasint (argv, &i, Arg, 1, INT_MAX);
    }
    else if (strcmp (Arg, "-morpho") == 0)
    {
      if ((*pIn).morpho != NULL)
	ut_free_1d_char ((*pIn).morpho);
      (*pIn).morpho = ut_arg_nextaschar (argv, &i, Arg);

      if ((*pIn).n > 0)
	if ((! strcmp ((*pIn).morpho, "cube"  ))
	 || (! strcmp ((*pIn).morpho, "dodeca"))
	 || (! strcmp ((*pIn).morpho, "tocta" )))
	{
	  (*pIn).input = ut_alloc_1d_char (6);
	  strcpy ((*pIn).input, "n_reg");
	}
    }
    else if (strcmp (Arg, "-ttype") == 0 && i < argc - 1)
    {
      ut_free_1d_char ((*pIn).ttype);
      (*pIn).ttype = ut_arg_nextaschar (argv, &i, Arg);
    }

/*---------------------------------------------------------------------- 
* general options */
    else if (strcmp (Arg, "-o") == 0 && i < argc - 1)
    {
      (*pIn).body = ut_arg_nextaschar (argv, &i, Arg);
      ut_string_body ((*pIn).body, (*pIn).body);
    }

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
    else if (strcmp (Arg, "-scale") == 0 && i < argc - 3)
    {
      (*pIn).scale = ut_alloc_1d (3);
      (*pIn).scale[0] = ut_arg_nextasreal (argv, &i, Arg, 0, DBL_MAX);
      (*pIn).scale[1] = ut_arg_nextasreal (argv, &i, Arg, 0, DBL_MAX);
      (*pIn).scale[2] = ut_arg_nextasreal (argv, &i, Arg, 0, DBL_MAX);
    }
    else if (strcmp (Arg, "-randomize") == 0 && i < argc - 2)
    {
      (*pIn).randomize = ut_arg_nextasreal (argv, &i, Arg, 0, DBL_MAX);
      (*pIn).randomize2 = ut_arg_nextasint (argv, &i, Arg, 0, INT_MAX);
    }
    else if (strcmp (Arg, "-randomizedir") == 0 && i < argc - 1)
      (*pIn).randomizedir = ut_arg_nextaschar (argv, &i, Arg);
    else if (strcmp (Arg, "-centroid") == 0 && i < argc - 1)
      (*pIn).centroid = ut_arg_nextasint (argv, &i, Arg, 0, 1);
    else if (strcmp (Arg, "-centroidfact") == 0 && i < argc - 1)
      (*pIn).centroidfact = ut_arg_nextasreal (argv, &i, Arg, 0, 1);
    else if (strcmp (Arg, "-centroidconv") == 0 && i < argc - 1)
      (*pIn).centroidconv = ut_arg_nextasreal (argv, &i, Arg, 0, DBL_MAX);
    else if (strcmp (Arg, "-centroiditermax") == 0 && i < argc - 1)
      (*pIn).centroiditermax = ut_arg_nextasint (argv, &i, Arg, 1, INT_MAX);

/*---------------------------------------------------------------------- 
* regularization options */
    else if (strcmp (Arg, "-maxff") == 0 && i < argc - 1)
      (*pReg).maxff = ut_arg_nextasreal (argv, &i, Arg, 0, 180);
    else if (strcmp (Arg, "-sel") == 0 && i < argc - 1)
    {
      (*pReg).seltype = 0;
      (*pReg).sel = ut_arg_nextasreal (argv, &i, Arg, 0, DBL_MAX);
    }
    else if (strcmp (Arg, "-rsel") == 0 && i < argc - 1)
    {
      (*pReg).seltype = 1;
      (*pReg).rsel = ut_arg_nextasreal (argv, &i, Arg, 0, DBL_MAX);
    }
    else if (strcmp (Arg, "-dbound") == 0 && i < argc - 1)
      (*pReg).dbound = ut_arg_nextaschar (argv, &i, Arg);
    else if (strcmp (Arg, "-dboundsel") == 0 && i < argc - 1)
    {
      (*pReg).dboundseltype = 0;
      (*pReg).dboundsel = ut_arg_nextasreal (argv, &i, Arg, 0., 1.e30);
    }
    else if (strcmp (Arg, "-dboundrsel") == 0 && i < argc - 1)
    {
      (*pReg).dboundseltype = 1;
      (*pReg).dboundrsel = ut_arg_nextasreal (argv, &i, Arg, 0., 1.e30);
    }
    else if (strcmp (Arg, "-mloop") == 0 && i < argc - 1)
      (*pReg).mloop = ut_arg_nextasint (argv, &i, Arg, 0, INT_MAX);
    else if (strcmp (Arg, "-maxedgedelqty") == 0 && i < argc - 1)
      (*pReg).maxedgedelqty = ut_arg_nextasint (argv, &i, Arg, 0, INT_MAX);

/*---------------------------------------------------------------------- 
* post-processing options */
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
    else if (strcmp (Arg, "-voxformat") == 0 && i < argc - 1)
    {
      ut_free_1d_char ((*pIn).voxformat);
      (*pIn).voxformat = ut_arg_nextaschar (argv, &i, Arg);
    }
    else if (strcmp (Arg, "-voxsize") == 0 && i < argc - 1)
    {
      (*pIn).voxsizetype = 1;
      (*pIn).voxsize = ut_arg_nextasint (argv, &i, Arg, 0, INT_MAX);
    }
    else if (strcmp (Arg, "-voxsize3") == 0 && i < argc - 3)
    {
      (*pIn).voxsizetype = 2;
      (*pIn).voxsize3 = ut_realloc_1d_int ((*pIn).voxsize3, 3);
      (*pIn).voxsize3[0] = ut_arg_nextasint (argv, &i, Arg, 0, INT_MAX);
      (*pIn).voxsize3[1] = ut_arg_nextasint (argv, &i, Arg, 0, INT_MAX);
      (*pIn).voxsize3[2] = ut_arg_nextasint (argv, &i, Arg, 0, INT_MAX);
    }
    else if (strcmp (Arg, "-statver") == 0 && i < argc - 1)
      (*pIn).stv = ut_arg_nextaschar (argv, &i, Arg);
    else if (strcmp (Arg, "-statedge") == 0 && i < argc - 1)
      (*pIn).ste = ut_arg_nextaschar (argv, &i, Arg);
    else if (strcmp (Arg, "-statface") == 0 && i < argc - 1)
      (*pIn).stf = ut_arg_nextaschar (argv, &i, Arg);
    else if (strcmp (Arg, "-statpoly") == 0 && i < argc - 1)
      (*pIn).stp = ut_arg_nextaschar (argv, &i, Arg);
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
    else if ((! strcmp (Arg, "-loadtess") && i < argc - 1)
          || (! strcmp (Arg, "-checktess") && i < argc - 1))
    {
      (*pIn).input = ut_alloc_1d_char (5);
      strcpy ((*pIn).input, "tess");
      (*pIn).load = ut_arg_nextaschar (argv, &i, Arg);

      if (! strcmp (Arg, "-checktess"))
	(*pIn).checktess = 1;
    }
    else if (strcmp (Arg, "-loadvox") == 0 && i < argc - 1)
    {
      (*pIn).input = ut_alloc_1d_char (4);
      strcpy ((*pIn).input, "vox");
      (*pIn).load = ut_arg_nextaschar (argv, &i, Arg);

      if (! strcmp (Arg, "-checktess"))
	(*pIn).checktess = 1;
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
