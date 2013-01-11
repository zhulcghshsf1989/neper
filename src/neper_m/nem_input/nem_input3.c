  /* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_input.h"

/* SetDefaultOptions set the options to their default values */
void
nem_input_options_default (struct IN *pIn, struct TESSPARA *pTessPara)
{
  /* General options --------------------------------------- */

  (*pIn).nset = ut_alloc_1d_char (1000);
  sprintf ((*pIn).nset, "faces");
  (*pIn).faset = ut_alloc_1d_char (100);
  strcpy ((*pIn).faset, "none");

  (*pIn).tess = NULL;
  (*pIn).vox = NULL;
  (*pIn).mesh = NULL;
  (*pIn).msh = NULL;
  (*pIn).geof = NULL;
  (*pIn).fepx1 = NULL;
  (*pIn).fepx2 = NULL;
  (*pIn).fepx3 = NULL;
  (*pIn).stn  = NULL;
  (*pIn).ste  = NULL;
  (*pIn).stelset = NULL;
  (*pIn).body = NULL;

  (*pIn).format = ut_alloc_1d_char (5);
  sprintf ((*pIn).format, "msh");

  /* Options for meshing ------------------------------- */
  (*pIn).morder = 1;
  (*pIn).meshdim = -1;
  (*pIn).elttype = ut_alloc_1d_char (10);
  strcpy ((*pIn).elttype, "tet");

  (*pIn).singnodedup = 0;
  (*pIn).dupnodemerge = -1;
  
  (*pTessPara).cltype = 1; // O: cl, 1: rcl, 2: cl3, 3: rcl3 
  (*pTessPara).cl = -1;
  (*pTessPara).rcl = 1;
  (*pTessPara).cl3  = ut_alloc_1d (3);
  (*pTessPara).rcl3 = ut_alloc_1d (3);
  (*pTessPara).dboundcl  = -1;
  (*pTessPara).dboundrcl = -1;
  (*pTessPara).dboundpcl =  2;
  (*pTessPara).dbound = ut_alloc_1d_char (10);

  (*pTessPara).msizetype = 1;
  (*pTessPara).msize = 20;
  (*pTessPara).msize3 = ut_alloc_1d_int (3);
  (*pTessPara).msize3[0] = 20;
  (*pTessPara).msize3[1] = 20;
  (*pTessPara).msize3[2] = 20;

  (*pTessPara).clmin = 0;
  (*pTessPara).pcl = 2;
  (*pIn).mesh2dalgo = ut_alloc_1d_char (100);
  (*pIn).mesh3dalgo = ut_alloc_1d_char (100);
  (*pIn).mesh3dreport = 0;
  (*pIn).meshpoly = NULL;

  (*pIn).mesh3doptiexpr = ut_alloc_1d_char (100);
  sprintf ((*pIn).mesh3doptiexpr, "Odis^0.8*Osize^0.2");
  (*pIn).mesh3doptidisexpr = ut_alloc_1d_char (100);
  sprintf ((*pIn).mesh3doptidisexpr, "dis^(exp((dis^0.1)/(dis^0.1-1)))");

  sprintf ((*pIn).mesh2dalgo, "default");
  sprintf ((*pIn).mesh3dalgo, "default");

  (*pIn).partmethod = ut_alloc_1d_char (10);
  sprintf ((*pIn).partmethod, "default");
  (*pIn).partmode = -1;
  (*pIn).partarchfile = NULL;
  (*pIn).partqty = 0;
  (*pIn).partsets = 1;
  (*pIn).partbalancing = .5;
  (*pIn).partrenumber = 0;

  (*pIn).gmsh = ut_alloc_1d_char (100);
  sprintf ((*pIn).gmsh, "/usr/local/bin/gmsh");

  (*pIn).mesh2dmaxtime = 10000;
  (*pIn).mesh2drmaxtime =  100;
  (*pIn).mesh2diter =  3;
  (*pIn).mesh3dmaxtime = 10000;
  (*pIn).mesh3drmaxtime =  100;
  (*pIn).mesh3diter =  3;
  (*pIn).mesh3dclconv = 0.02;

  /* Options for remeshing --------------------------------- */
  (*pIn).transportqty = 0;
  (*pIn).transportspec = NULL;

  (*pIn).nodecoo = NULL;
  (*pIn).loadmesh = NULL;

  return;
}

void
nem_input_options_set (struct IN *pIn, struct TESSPARA *pTessPara,
		       int argc, char **argv)
{
  int i, j, k, ArgQty, Res;
  char **ArgList = ut_alloc_2d_char (101, 101);
  char *Arg = ut_alloc_1d_char (101);
  char *format = ut_alloc_1d_char (100);
  char* tmpstring = NULL;
  char* tmpstring2 = ut_alloc_1d_char (1000);

  /* This is the possible argument list. */
  ArgQty = 0;

  // General options ---------------------------------------------------
  sprintf (ArgList[++ArgQty], "-o");
  sprintf (ArgList[++ArgQty], "-v");
  sprintf (ArgList[++ArgQty], "-gmsh");

  // General meshing options -------------------------------------------
  sprintf (ArgList[++ArgQty], "-elttype");
  sprintf (ArgList[++ArgQty], "-cl");
  sprintf (ArgList[++ArgQty], "-rcl");
  sprintf (ArgList[++ArgQty], "-cl3");
  sprintf (ArgList[++ArgQty], "-rcl3");
  sprintf (ArgList[++ArgQty], "-msize");
  sprintf (ArgList[++ArgQty], "-msize3");
  sprintf (ArgList[++ArgQty], "-pl");
  sprintf (ArgList[++ArgQty], "-clmin");
  sprintf (ArgList[++ArgQty], "-order");
  sprintf (ArgList[++ArgQty], "-dim");
  sprintf (ArgList[++ArgQty], "-mesh2dalgo");
  sprintf (ArgList[++ArgQty], "-mesh3dalgo");
  sprintf (ArgList[++ArgQty], "-mesh3doptiexpr");
  sprintf (ArgList[++ArgQty], "-mesh3doptidisexpr");

  sprintf (ArgList[++ArgQty], "-singnodedup");
  sprintf (ArgList[++ArgQty], "-dupnodemerge");
  
  // development options
  sprintf (ArgList[++ArgQty], "-meshface");
  sprintf (ArgList[++ArgQty], "-meshpoly");
  sprintf (ArgList[++ArgQty], "-mesh3dreport");

  // Mesh partitionning options ----------------------------------------
  sprintf (ArgList[++ArgQty], "-partqty");
  sprintf (ArgList[++ArgQty], "-partarchfile");
  sprintf (ArgList[++ArgQty], "-partbalancing");
  sprintf (ArgList[++ArgQty], "-partmethod");
  sprintf (ArgList[++ArgQty], "-partrenumbering");
  sprintf (ArgList[++ArgQty], "-partsets");

  // Output options ----------------------------------------------------
  sprintf (ArgList[++ArgQty], "-outdim");
  sprintf (ArgList[++ArgQty], "-format");
  sprintf (ArgList[++ArgQty], "-nset");
  sprintf (ArgList[++ArgQty], "-surf");   // deprecated 
  sprintf (ArgList[++ArgQty], "-faset");

  // Tessellation and mesh statistics options ------------------------------
  sprintf (ArgList[++ArgQty], "-statnode");
  sprintf (ArgList[++ArgQty], "-statelt");
  sprintf (ArgList[++ArgQty], "-statelset");

  // Advanced options --------------------------------------------------
  sprintf (ArgList[++ArgQty], "-mesh2dmaxtime");
  sprintf (ArgList[++ArgQty], "-mesh2drmaxtime");
  sprintf (ArgList[++ArgQty], "-mesh2diter");
  sprintf (ArgList[++ArgQty], "-mesh3dmaxtime");
  sprintf (ArgList[++ArgQty], "-mesh3drmaxtime");
  sprintf (ArgList[++ArgQty], "-mesh3diter");
  sprintf (ArgList[++ArgQty], "-mesh3dclconv");

  // Remeshing ---------------------------------------------------------
  sprintf (ArgList[++ArgQty], "-remesh");
  sprintf (ArgList[++ArgQty], "-remeshtess");
  sprintf (ArgList[++ArgQty], "-transport");
  sprintf (ArgList[++ArgQty], "-transporttess");

  // Restart a job -----------------------------------------------------
  sprintf (ArgList[++ArgQty], "-loadtess");  // not documented
  sprintf (ArgList[++ArgQty], "-loadmesh");
  sprintf (ArgList[++ArgQty], "-loadmeshnodecoo");

  // Domain boundary regularization and meshing ------------------------
  sprintf (ArgList[++ArgQty], "-dbound");
  sprintf (ArgList[++ArgQty], "-dboundrcl");
  sprintf (ArgList[++ArgQty], "-dboundcl");
  sprintf (ArgList[++ArgQty], "-dboundpl");
  
  /* Reading arguments ----------------------------------- */
  for (i = 1; i < argc; i++)
  {
    /* Input data --------------------------------------------------- */
    if (argv[i][0] != '-')
    {
      if ((*pIn).tess == NULL && (*pIn).vox == NULL)
      {
	i--;

	char* tmp = ut_arg_nextaschar (argv, &i, Arg);
	char* format = NULL;
	ut_file_format (tmp, &format);

	if (! strcmp (format, "tess"))
	{
	  (*pIn).tess = ut_alloc_1d_char (strlen (tmp) + 1);
	  strcpy ((*pIn).tess, tmp);
	}
	else if (! strcmp (format, "vox"))
	{
	  (*pIn).vox = ut_alloc_1d_char (strlen (tmp) + 1);
	  strcpy ((*pIn).vox, tmp);
	}

	ut_free_1d_char (tmp);
	ut_free_1d_char (format);
      }
      else
      {
	ut_print_message (2, 0, "Input file already loaded\n");
	abort ();
      }
    }
    else
    {
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

      /* Prerequisites ---------------------------------------------- */

      if (strcmp (Arg, "-gmsh") == 0 && i < argc - 1)
      {
	ut_free_1d_char ((*pIn).gmsh);
	(*pIn).gmsh = ut_arg_nextaschar (argv, &i, Arg);
      }

      /* General options ------------------------------------------------ */

      else if (strcmp (Arg, "-o") == 0 && i < argc - 1)
      {
	ut_free_1d_char ((*pIn).body);
	(*pIn).body = ut_arg_nextaschar (argv, &i, Arg);
	ut_string_body ((*pIn).body, (*pIn).body);
      }

      /* Options for the meshing ----------------------------------------- */

      else if (strcmp (Arg, "-elttype") == 0 && i < argc - 1)
	(*pIn).elttype = ut_arg_nextaschar (argv, &i, Arg);
      else if (strcmp (Arg, "-cl") == 0 && i < argc - 1)
      {
	(*pTessPara).cltype = 0;
	(*pTessPara).cl = ut_arg_nextasreal (argv, &i, Arg, 0., 1.e30);
      }
      else if (strcmp (Arg, "-rcl") == 0 && i < argc - 1)
      {
	(*pTessPara).cltype = 1;
	(*pTessPara).rcl = ut_arg_nextasreal (argv, &i, Arg, 0., 1.e30);
      }
      else if (strcmp (Arg, "-cl3") == 0 && i < argc - 3)
      {
	(*pTessPara).cltype = 2;
	(*pTessPara).cl3[0] = ut_arg_nextasreal (argv, &i, Arg, 0., 1.e30);
	(*pTessPara).cl3[1] = ut_arg_nextasreal (argv, &i, Arg, 0., 1.e30);
	(*pTessPara).cl3[2] = ut_arg_nextasreal (argv, &i, Arg, 0., 1.e30);
      }
      else if (strcmp (Arg, "-rcl3") == 0 && i < argc - 3)
      {
	(*pTessPara).cltype = 3;
	(*pTessPara).rcl3[0] = ut_arg_nextasreal (argv, &i, Arg, 0., 1.e30);
	(*pTessPara).rcl3[1] = ut_arg_nextasreal (argv, &i, Arg, 0., 1.e30);
	(*pTessPara).rcl3[2] = ut_arg_nextasreal (argv, &i, Arg, 0., 1.e30);
      }
      else if (strcmp (Arg, "-msize") == 0 && i < argc - 1)
      {
	(*pTessPara).msizetype = 1;
	(*pTessPara).msize = ut_arg_nextasint (argv, &i, Arg, 1, INT_MAX);
      }
      else if (strcmp (Arg, "-msize3") == 0 && i < argc - 1)
      {
	(*pTessPara).msizetype = 3;
	(*pTessPara).msize3[0] = ut_arg_nextasint (argv, &i, Arg, 1, INT_MAX);
	(*pTessPara).msize3[1] = ut_arg_nextasint (argv, &i, Arg, 1, INT_MAX);
	(*pTessPara).msize3[2] = ut_arg_nextasint (argv, &i, Arg, 1, INT_MAX);
      }
      else if (strcmp (Arg, "-dbound") == 0 && i < argc - 1)
      {
	ut_free_1d_char ((*pTessPara).dbound);
	(*pTessPara).dbound = ut_arg_nextaschar (argv, &i, Arg);
      }
      else if (strcmp (Arg, "-dboundcl") == 0 && i < argc - 1)
	(*pTessPara).dboundcl = ut_arg_nextasreal (argv, &i, Arg, 0., 1.e30);
      else if (strcmp (Arg, "-dboundrcl") == 0 && i < argc - 1)
	(*pTessPara).dboundrcl = ut_arg_nextasreal (argv, &i, Arg, 0., 1.e30);
      else if ((strcmp (Arg, "-dboundpl") == 0 && i < argc - 1))
	(*pTessPara).dboundpcl = ut_arg_nextasreal (argv, &i, Arg, 0., 1.e30);
      else if (strcmp (Arg, "-clmin") == 0 && i < argc - 1)
	(*pTessPara).clmin = ut_arg_nextasreal (argv, &i, Arg, 0., 1.e30);
      else if ((strcmp (Arg, "-pl") == 0 && i < argc - 1))
	(*pTessPara).pcl = ut_arg_nextasreal (argv, &i, Arg, 1., 1.e30);
      else if (strcmp (Arg, "-mesh2dalgo") == 0 && i < argc - 1)
      {
	ut_free_1d_char ((*pIn).mesh2dalgo);
	(*pIn).mesh2dalgo = ut_arg_nextaschar (argv, &i, Arg);
      }
      else if (strcmp (Arg, "-mesh3dalgo") == 0 && i < argc - 1)
      {
	ut_free_1d_char ((*pIn).mesh3dalgo);
	(*pIn).mesh3dalgo = ut_arg_nextaschar (argv, &i, Arg);
      }
      else if (strcmp (Arg, "-mesh3doptiexpr") == 0 && i < argc - 1)
      {
	ut_free_1d_char ((*pIn).mesh3doptiexpr);
	(*pIn).mesh3doptiexpr = ut_arg_nextaschar (argv, &i, Arg);
      }
      else if (strcmp (Arg, "-mesh3doptidisexpr") == 0 && i < argc - 1)
      {
	ut_free_1d_char ((*pIn).mesh3doptidisexpr);
	(*pIn).mesh3doptidisexpr = ut_arg_nextaschar (argv, &i, Arg);
      }
      else if (strcmp (Arg, "-singnodedup") == 0 && i < argc - 1)
	(*pIn).singnodedup = ut_arg_nextasint (argv, &i, Arg, 0, 1);
      else if (strcmp (Arg, "-dupnodemerge") == 0 && i < argc - 1)
	(*pIn).dupnodemerge = ut_arg_nextasreal (argv, &i, Arg, 0, DBL_MAX);
      
      // development options 
      else if (strcmp (Arg, "-mesh3dreport") == 0 && i < argc - 1)
	(*pIn).mesh3dreport = ut_arg_nextasint (argv, &i, Arg, 0, INT_MAX);
      else if (strcmp (Arg, "-meshpoly") == 0 && i < argc - 1)
      {
	ut_free_1d_char ((*pIn).meshpoly);
	(*pIn).meshpoly = ut_arg_nextaschar (argv, &i, Arg);
      }
      else if (strcmp (Arg, "-meshface") == 0 && i < argc - 1)
      {
	ut_free_1d_char ((*pIn).meshface);
	(*pIn).meshface = ut_arg_nextaschar (argv, &i, Arg);
      }
      // end of development options
      
      else if ((strcmp (Arg, "-morder") == 0 && i < argc - 1)
            || (strcmp (Arg, "-order") == 0 && i < argc - 1))
	(*pIn).morder = ut_arg_nextasint (argv, &i, Arg, 1, 2);
      else if (strcmp (Arg, "-dim") == 0 && i < argc - 1)
	(*pIn).meshdim = ut_arg_nextasint (argv, &i, Arg, 0, 3);
      else if ((strcmp (Arg, "-elttype") == 0 && i < argc - 1))
      {
	ut_free_1d_char ((*pIn).elttype);
	(*pIn).elttype = ut_arg_nextaschar (argv, &i, Arg);
      }
      else if (strcmp (Arg, "-statnode") == 0 && i < argc - 1)
	(*pIn).stn = ut_arg_nextaschar (argv, &i, Arg);
      else if (strcmp (Arg, "-statelt") == 0 && i < argc - 1)
	(*pIn).ste = ut_arg_nextaschar (argv, &i, Arg);
      else if (strcmp (Arg, "-statelset") == 0 && i < argc - 1)
	(*pIn).stelset = ut_arg_nextaschar (argv, &i, Arg);
      else if (strcmp (Arg, "-mesh3dmaxtime") == 0 && i < argc - 1)
	(*pIn).mesh3dmaxtime = ut_arg_nextasreal (argv, &i, Arg, 0, DBL_MAX);
      else if (strcmp (Arg, "-mesh2drmaxtime") == 0 && i < argc - 1)
	(*pIn).mesh2drmaxtime = ut_arg_nextasreal (argv, &i, Arg, 0, DBL_MAX);
      else if (strcmp (Arg, "-mesh2diter") == 0 && i < argc - 1)
	(*pIn).mesh2diter = ut_arg_nextasint (argv, &i, Arg, 0, INT_MAX);
      else if (strcmp (Arg, "-mesh2dmaxtime") == 0 && i < argc - 1)
	(*pIn).mesh2dmaxtime = ut_arg_nextasreal (argv, &i, Arg, 0, DBL_MAX);
      else if (strcmp (Arg, "-mesh3drmaxtime") == 0 && i < argc - 1)
	(*pIn).mesh3drmaxtime = ut_arg_nextasreal (argv, &i, Arg, 0, DBL_MAX);
      else if (strcmp (Arg, "-mesh3diter") == 0 && i < argc - 1)
	(*pIn).mesh3diter = ut_arg_nextasint (argv, &i, Arg, 0, INT_MAX);
      else if (strcmp (Arg, "-mesh3dclconv") == 0 && i < argc - 1)
	(*pIn).mesh3dclconv = ut_arg_nextasreal (argv, &i, Arg, 0, DBL_MAX);
      else if (strcmp (Arg, "-nset") == 0 && i < argc - 1)
      {
	ut_free_1d_char ((*pIn).nset);
	(*pIn).nset = ut_arg_nextaschar (argv, &i, Arg);
      }
      else if (strcmp (Arg, "-surf") == 0 && i < argc - 1)
      {
	ut_free_1d_char ((*pIn).faset);
	(*pIn).faset = ut_arg_nextaschar (argv, &i, Arg);
	if ((*pIn).faset[0] == '2')
	{
	  ut_free_1d_char ((*pIn).faset);
	  (*pIn).faset = ut_arg_nextaschar (argv, &i, Arg);
	}
      }
      else if (strcmp (Arg, "-faset") == 0 && i < argc - 1)
      {
	ut_free_1d_char ((*pIn).faset);
	(*pIn).faset = ut_arg_nextaschar (argv, &i, Arg);
	if ((*pIn).faset[0] == '2')
	{
	  ut_free_1d_char ((*pIn).faset);
	  (*pIn).faset = ut_arg_nextaschar (argv, &i, Arg);
	}
      }

      else if (strcmp (Arg, "-loadmesh") == 0 && i < argc - 1)
      {
	ut_free_1d_char ((*pIn).loadmesh);
	(*pIn).loadmesh = ut_arg_nextaschar (argv, &i, Arg);
	(*pIn).mesh = 0;
      }

      else if (strcmp (Arg, "-loadmeshnodecoo") == 0 && i < argc - 1)
      {
	ut_free_1d_char ((*pIn).nodecoo);
	(*pIn).nodecoo = ut_arg_nextaschar (argv, &i, Arg);
      }

      else if (strcmp (Arg, "-remesh") == 0 && i < argc - 1)
      {
	ut_free_1d_char ((*pIn).mesh);
	(*pIn).mesh = ut_arg_nextaschar (argv, &i, Arg);
      }

      else if (strcmp (Arg, "-remeshtess") == 0 && i < argc - 1)
      {
	ut_free_1d_char ((*pIn).tess);
	(*pIn).tess = ut_arg_nextaschar (argv, &i, Arg);
      }

      else if ((strcmp (Arg, "-transport") == 0 && i < argc - 2))
      {
	if ((*pIn).mesh != NULL)
	  abort ();

	(*pIn).mesh = ut_arg_nextaschar (argv, &i, Arg);
	(*pIn).transportqty = ut_arg_nextasint (argv, &i, Arg, 1, INT_MAX);
	(*pIn).transportspec = ut_alloc_2d_pchar ((*pIn).transportqty, 10);
	for (j = 0; j < (*pIn).transportqty; j++)
	{
	  for (k = 0; k < 3; k++)
	  {
	    ut_free_1d_char ((*pIn).transportspec[j][k]);
	    (*pIn).transportspec[j][k] = ut_arg_nextaschar (argv, &i, Arg);
	  }

	  if (strcmp ((*pIn).transportspec[j][1], "id") == 0)
	    sprintf ((*pIn).transportspec[j][1], "int1");
	  else if (strcmp ((*pIn).transportspec[j][1], "e") == 0
	   || strcmp ((*pIn).transportspec[j][1], "ek") == 0)
	    sprintf ((*pIn).transportspec[j][1], "real3");
	}
      }

      else if (strcmp (Arg, "-transporttess") == 0 && i < argc - 1)
      {
	ut_free_1d_char ((*pIn).tess);
	(*pIn).tess = ut_arg_nextaschar (argv, &i, Arg);
      }

      else if (strcmp (Arg, "-format") == 0 && i < argc - 1)
      {
	ut_free_1d_char ((*pIn).format);
	(*pIn).format = ut_arg_nextaschar (argv, &i, Arg);
      }
      else if (strcmp (Arg, "-outdim") == 0 && i < argc - 1)
      {
	ut_free_1d_char ((*pIn).outdim);
	(*pIn).outdim = ut_arg_nextaschar (argv, &i, Arg);
      }
      else if (strcmp (Arg, "-partmethod") == 0 && i < argc - 1)
      {
	ut_free_1d_char ((*pIn).partmethod);
	(*pIn).partmethod = ut_arg_nextaschar (argv, &i, Arg);
      }
      else if (strcmp (Arg, "-partarchfile") == 0 && i < argc - 1)
      {
	if ((*pIn).partmode != 0)
	{
	  ut_free_1d_char ((*pIn).partarchfile);
	  (*pIn).partarchfile = ut_arg_nextaschar (argv, &i, Arg);
          if (ut_file_exist ((*pIn).partarchfile) == 0)
          {
            ut_print_message (2, 0, "Architecture file does not exist.\n");
            abort ();
          }
	  (*pIn).partmode = 1;
	}
      }
      else if (strcmp (Arg, "-partqty") == 0 && i < argc - 1)
      {
	if ((*pIn).partmode != 1)
	{
	  (*pIn).partqty = ut_arg_nextasint (argv, &i, Arg, 1, INT_MAX);
	  (*pIn).partmode = 0;
	}
      }
      else if (strcmp (Arg, "-partbalancing") == 0 && i < argc - 1)
	(*pIn).partbalancing = ut_arg_nextasreal (argv, &i, Arg, 0, 1);
      else if (strcmp (Arg, "-partrenumbering") == 0 && i < argc - 1)
	(*pIn).partrenumber = ut_arg_nextasint (argv, &i, Arg, 0, 1);
      else if (strcmp (Arg, "-partsets") == 0 && i < argc - 1)
	(*pIn).partsets = ut_arg_nextasint (argv, &i, Arg, 0, 1);
      else
	ut_arg_error (Arg, "");
    }
  }

  ut_free_2d_char (ArgList, 101);
  ut_free_1d_char (Arg);
  ut_free_1d_char (format);
  ut_free_1d_char (tmpstring);
  ut_free_1d_char (tmpstring2);

  return;
}
