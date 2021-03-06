  /* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"InputData_fm.h"

/* SetDefaultOptions set the options to their default values */
void
SetDefaultOptions_fm (struct IN *pIn, struct GEOPARA *pGeoPara)
{
  /* General options --------------------------------------- */

  (*pIn).verbosity = 0;
  (*pIn).nset = ut_alloc_1d_char (1000);
  sprintf ((*pIn).nset, "faces");
  (*pIn).faset = ut_alloc_1d_char (100);
  (*pIn).faset[0] = '\0';

  (*pIn).printff = 0;

  (*pIn).ingeo = NULL;
  (*pIn).geo = NULL;
  (*pIn).ply = NULL;
  (*pIn).msh = NULL;
  (*pIn).geof = NULL;
  (*pIn).fev1 = NULL;
  (*pIn).fev2 = NULL;
  (*pIn).fev3 = NULL;
  (*pIn).printfod = 1;
  (*pIn).fod = NULL;
  (*pIn).printstatmesh = ut_alloc_1d_char (5);
  strcpy ((*pIn).printstatmesh, "none");
  (*pIn).printstattess = ut_alloc_1d_char (5);
  strcpy ((*pIn).printstattess, "none");
  (*pIn).stt0 = NULL;
  (*pIn).stt1 = NULL;
  (*pIn).stt2 = NULL;
  (*pIn).stt2a = NULL;
  (*pIn).stn  = NULL;
  (*pIn).stm1 = NULL;
  (*pIn).stm2 = NULL;
  (*pIn).stm3 = NULL;
  (*pIn).stm4 = NULL;
  (*pIn).stm5 = NULL;
  (*pIn).body = NULL;

  (*pIn).format = ut_alloc_1d_char (5);
  sprintf ((*pIn).format, "msh");

  /* Options for the geometry modification ----------------- */
  (*pGeoPara).maxff = 0;
  (*pIn).mloop = 2;
  (*pIn).morder = 1;
  (*pIn).meshdim = -1;
  (*pIn).mesh = 0;
  (*pIn).remesh = 0;
  
  /* Options for the meshing ------------------------------- */
  (*pGeoPara).cltype = 1; // O: cl, 1: rcl, 2: cl3, 3: rcl3 
  (*pGeoPara).cl = -1;
  (*pGeoPara).rcl = 1;
  (*pGeoPara).cl3  = ut_alloc_1d (3);
  (*pGeoPara).rcl3 = ut_alloc_1d (3);
  (*pGeoPara).dboundcl  = -1;
  (*pGeoPara).dboundrcl = -1;
  (*pGeoPara).dboundpcl =  2;
  (*pGeoPara).dbound = ut_alloc_1d_char (10);

  (*pGeoPara).clmin = 0;
  (*pGeoPara).pcl = 2;
  (*pGeoPara).sel = -1;
  (*pGeoPara).rsel = -1;	/* relatively to the default value */
  (*pGeoPara).dboundsel = -1;
  (*pGeoPara).dboundrsel = -1;	/* relatively to the default value */
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
  (*pIn).remap = 0;
  (*pIn).remesh2 = NULL;
  (*pIn).remeshtess = NULL;
  (*pIn).transporttess = NULL;
  (*pIn).remap2 = NULL;
  (*pIn).remapspec = NULL;

  /* Other options ----------------------------------------- */
  /* Options for debugging --------------------------------- */
  (*pIn).maxedgedelqty = INT_MAX;

  (*pIn).nodecoo = NULL;
  (*pIn).loadmesh = NULL;

  return;
}

void
SetOptions_fm (struct IN *pIn, struct GEOPARA *pGeoPara,
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

  // Geometry regularization options -----------------------------------
  sprintf (ArgList[++ArgQty], "-maxff");
  sprintf (ArgList[++ArgQty], "-sel");
  sprintf (ArgList[++ArgQty], "-rsel");
  sprintf (ArgList[++ArgQty], "-mloop");

  // General meshing options -------------------------------------------
  sprintf (ArgList[++ArgQty], "-cl");
  sprintf (ArgList[++ArgQty], "-rcl");
  sprintf (ArgList[++ArgQty], "-cl3");
  sprintf (ArgList[++ArgQty], "-rcl3");
  sprintf (ArgList[++ArgQty], "-pl");
  sprintf (ArgList[++ArgQty], "-pcl");          // deprecated
  sprintf (ArgList[++ArgQty], "-clmin");
  sprintf (ArgList[++ArgQty], "-morder");
  sprintf (ArgList[++ArgQty], "-order");
  sprintf (ArgList[++ArgQty], "-dim");
  sprintf (ArgList[++ArgQty], "-mesh2dalgo");
  sprintf (ArgList[++ArgQty], "-mesh3dalgo");
  sprintf (ArgList[++ArgQty], "-mesh3doptiexpr");
  sprintf (ArgList[++ArgQty], "-mesh3doptidisexpr");
  
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

  // Geometry and mesh statistics options ------------------------------
  sprintf (ArgList[++ArgQty], "-stattess");
  sprintf (ArgList[++ArgQty], "-statmesh");

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
  sprintf (ArgList[++ArgQty], "-remap");      // deprecated
  sprintf (ArgList[++ArgQty], "-remaptess");  // deprecated
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
  sprintf (ArgList[++ArgQty], "-dboundpcl"); // deprecated
  sprintf (ArgList[++ArgQty], "-dboundpl");
  sprintf (ArgList[++ArgQty], "-dboundsel");
  sprintf (ArgList[++ArgQty], "-dboundrsel");

  // Devel options --------------------------------------- */
  // development option
  sprintf (ArgList[++ArgQty], "-maxedgedelqty");
  
  /* Reading arguments ----------------------------------- */
  for (i = 1; i < argc; i++)
  {
    /* Input file name */
    if (argv[i][0] != '-')
    {
      if ((*pIn).ingeo == NULL)
      {
	i--;

	(*pIn).ingeo = ut_arg_nextaschar (argv, &i, Arg);
	/*
	if (ut_file_testformat ((*pIn).ingeo, "tess") == 0)
	{
	  ut_print_message (2, 0, "File of wrong format (tess)\n");
	  abort ();
	}
	*/
	(*pIn).mesh = 1;
      }
      else
      {
	ut_print_message (2, 0, "Tess file already loaded\n");
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

      /* General options ------------------------------------------------ */
      if (strcmp (Arg, "-o") == 0 && i < argc - 1)
      {
	ut_free_1d_char ((*pIn).body);
	(*pIn).body = ut_arg_nextaschar (argv, &i, Arg);
	ut_string_body ((*pIn).body, (*pIn).body);
      }
      else if (strcmp (Arg, "-v") == 0 && i < argc - 1)
	(*pIn).verbosity = ut_arg_nextasint (argv, &i, Arg, 0, 1);

      else if (strcmp (Arg, "-gmsh") == 0 && i < argc - 1)
      {
	ut_free_1d_char ((*pIn).gmsh);
	(*pIn).gmsh = ut_arg_nextaschar (argv, &i, Arg);
      }

      /* Options for the geometry modification ---------------------------- */
      else if (strcmp (Arg, "-maxff") == 0 && i < argc - 1)
	(*pGeoPara).maxff = ut_arg_nextasreal (argv, &i, Arg, 0., 180.);
      else if (strcmp (Arg, "-mloop") == 0 && i < argc - 1)
	(*pIn).mloop = ut_arg_nextasint (argv, &i, Arg, 0, INT_MAX);

      /* Options for the meshing ----------------------------------------- */

      else if (strcmp (Arg, "-cl") == 0 && i < argc - 1)
      {
	(*pGeoPara).cltype = 0;
	(*pGeoPara).cl = ut_arg_nextasreal (argv, &i, Arg, 0., 1.e30);
      }
      else if (strcmp (Arg, "-rcl") == 0 && i < argc - 1)
      {
	(*pGeoPara).cltype = 1;
	(*pGeoPara).rcl = ut_arg_nextasreal (argv, &i, Arg, 0., 1.e30);
      }
      else if (strcmp (Arg, "-cl3") == 0 && i < argc - 3)
      {
	(*pGeoPara).cltype = 2;
	(*pGeoPara).cl3[0] = ut_arg_nextasreal (argv, &i, Arg, 0., 1.e30);
	(*pGeoPara).cl3[1] = ut_arg_nextasreal (argv, &i, Arg, 0., 1.e30);
	(*pGeoPara).cl3[2] = ut_arg_nextasreal (argv, &i, Arg, 0., 1.e30);
      }
      else if (strcmp (Arg, "-rcl3") == 0 && i < argc - 3)
      {
	(*pGeoPara).cltype = 3;
	(*pGeoPara).rcl3[0] = ut_arg_nextasreal (argv, &i, Arg, 0., 1.e30);
	(*pGeoPara).rcl3[1] = ut_arg_nextasreal (argv, &i, Arg, 0., 1.e30);
	(*pGeoPara).rcl3[2] = ut_arg_nextasreal (argv, &i, Arg, 0., 1.e30);
      }
      else if (strcmp (Arg, "-dbound") == 0 && i < argc - 1)
      {
	ut_free_1d_char ((*pGeoPara).dbound);
	(*pGeoPara).dbound = ut_arg_nextaschar (argv, &i, Arg);
      }
      else if (strcmp (Arg, "-dboundcl") == 0 && i < argc - 1)
	(*pGeoPara).dboundcl = ut_arg_nextasreal (argv, &i, Arg, 0., 1.e30);
      else if (strcmp (Arg, "-dboundrcl") == 0 && i < argc - 1)
	(*pGeoPara).dboundrcl = ut_arg_nextasreal (argv, &i, Arg, 0., 1.e30);
      else if ((strcmp (Arg, "-dboundpcl") == 0 && i < argc - 1)
            || (strcmp (Arg, "-dboundpl") == 0 && i < argc - 1))
	(*pGeoPara).dboundpcl = ut_arg_nextasreal (argv, &i, Arg, 0., 1.e30);
      else if (strcmp (Arg, "-dboundsel") == 0 && i < argc - 1)
	(*pGeoPara).dboundsel = ut_arg_nextasreal (argv, &i, Arg, 0., 1.e30);
      else if (strcmp (Arg, "-dboundrsel") == 0 && i < argc - 1)
	(*pGeoPara).dboundrsel = ut_arg_nextasreal (argv, &i, Arg, 0., 1.e30);
      else if (strcmp (Arg, "-clmin") == 0 && i < argc - 1)
	(*pGeoPara).clmin = ut_arg_nextasreal (argv, &i, Arg, 0., 1.e30);
      else if ((strcmp (Arg, "-pl") == 0 && i < argc - 1)
            || (strcmp (Arg, "-pcl") == 0 && i < argc - 1))
	(*pGeoPara).pcl = ut_arg_nextasreal (argv, &i, Arg, 1., 1.e30);
      else if (strcmp (Arg, "-sel") == 0 && i < argc - 1)
	(*pGeoPara).sel = ut_arg_nextasreal (argv, &i, Arg, 0., 1.e30);
      else if (strcmp (Arg, "-rsel") == 0 && i < argc - 1)
	(*pGeoPara).rsel = ut_arg_nextasreal (argv, &i, Arg, 0., 1.e30);
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
      else if (strcmp (Arg, "-stattess") == 0 && i < argc - 1)
      {
	ut_free_1d_char ((*pIn).printstattess);
	(*pIn).printstattess = ut_arg_nextaschar (argv, &i, Arg);
      }
      else if (strcmp (Arg, "-statmesh") == 0 && i < argc - 1)
      {
	ut_free_1d_char ((*pIn).printstatmesh);
	(*pIn).printstatmesh = ut_arg_nextaschar (argv, &i, Arg);
      }
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
      else if (strcmp (Arg, "-loadtess") == 0 && i < argc - 1)
      {
	ut_free_1d_char ((*pIn).ingeo);
	(*pIn).ingeo = ut_arg_nextaschar (argv, &i, Arg);
	(*pIn).mesh = 1;
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
	ut_free_1d_char ((*pIn).remesh2);
	(*pIn).remesh2 = ut_arg_nextaschar (argv, &i, Arg);
	(*pIn).mesh = 0;
	(*pIn).remesh = 1;
      }
      else if (strcmp (Arg, "-remeshtess") == 0 && i < argc - 1)
      {
	ut_free_1d_char ((*pIn).remeshtess);
	(*pIn).remeshtess = ut_arg_nextaschar (argv, &i, Arg);
      }
      else if ((strcmp (Arg, "-transport") == 0 && i < argc - 2) ||
               (strcmp (Arg, "-remap")     == 0 && i < argc - 2)  )
      {
	ut_free_1d_char ((*pIn).remap2);
	(*pIn).remap2 = ut_arg_nextaschar (argv, &i, Arg);
	(*pIn).remap = ut_arg_nextasint (argv, &i, Arg, 1, INT_MAX);
	if ((*pIn).remap > 0)
	{
	  (*pIn).remapspec = ut_alloc_2d_pchar ((*pIn).remap, 10);
	  for (j = 0; j < (*pIn).remap; j++)
	  {
	    for (k = 0; k < 3; k++)
	    {
	      ut_free_1d_char ((*pIn).remapspec[j][k]);
	      (*pIn).remapspec[j][k] = ut_arg_nextaschar (argv, &i, Arg);
	    }

	    if (strcmp ((*pIn).remapspec[j][1], "id") == 0)
	      sprintf ((*pIn).remapspec[j][1], "int1");
	    else if (strcmp ((*pIn).remapspec[j][1], "e") == 0
	     || strcmp ((*pIn).remapspec[j][1], "ek") == 0)
	      sprintf ((*pIn).remapspec[j][1], "real3");
	  }
	}
      }
      else if ((strcmp (Arg, "-transporttess") == 0 && i < argc - 1)
            || (strcmp (Arg, "-remaptess") == 0 && i < argc - 1))
      {
	ut_free_1d_char ((*pIn).transporttess);
	(*pIn).transporttess = ut_arg_nextaschar (argv, &i, Arg);
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
      // development option
      else if (strcmp (Arg, "-maxedgedelqty") == 0 && i < argc - 1)
	(*pIn).maxedgedelqty = ut_arg_nextasint (argv, &i, Arg, 0, INT_MAX);
     
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
