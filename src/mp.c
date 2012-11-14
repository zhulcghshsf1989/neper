/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"mp.h"
#include"neper_config.h"

void NeperHeader (char*);
void NeperFoot (void);
void ProgInfo (void);

int
main (int argc, char **argv)
{
  int ArgQty, Res;
  int fargc;
  char **ArgList = ut_alloc_2d_char (100, 100);
  char **fargv = NULL;
  char *Arg = NULL;
  char *inifile = NULL;
  int header_print = 1;
  struct timeval beg_time, end_time;
  struct timezone zone = { 0, 0 };
  double t;

  gettimeofday (&beg_time, &zone);

  /* version definitions -------------------------------------------- */

  /* --------------------------------------------------------------- */

  /* This is the possible argument list. */
  ArgQty = 0;
  sprintf (ArgList[++ArgQty], "--help");
  sprintf (ArgList[++ArgQty], "--version");
  sprintf (ArgList[++ArgQty], "--license");
  sprintf (ArgList[++ArgQty], "--rcfile");
  sprintf (ArgList[++ArgQty], "-MM");
  sprintf (ArgList[++ArgQty], "-FM");
  sprintf (ArgList[++ArgQty], "-T");
  sprintf (ArgList[++ArgQty], "-O");
  sprintf (ArgList[++ArgQty], "-VS");

  char* prog_version = ut_alloc_1d_char (100);
  if (NEPER_VERSION_DEVEL != -1)
    sprintf (prog_version, "%d.%d.%drc-%d",
	NEPER_VERSION_MAJOR, NEPER_VERSION_MINOR,
	NEPER_VERSION_PATCH, NEPER_VERSION_DEVEL);
  else
    sprintf (prog_version, "%d.%d.%d",
	NEPER_VERSION_MAJOR, NEPER_VERSION_MINOR, NEPER_VERSION_PATCH);

  if (argc == 1)
  {
    NeperHeader (prog_version);
    ProgInfo ();
    NeperFoot ();
    ut_free_2d_char (ArgList, 100);

    return 1;
  }
  else
  {
    Arg = ut_alloc_1d_char (100);
    fargv = ut_alloc_2d_char (100, 100);

    if (argc == 2)
    {
      Res = ut_string_comp (argv[1], ArgList, ArgQty, Arg);

      /* Checking if it's help or version or license; otherwise it's a module */
      if (strcmp (Arg, "--help") == 0)
      {
	NeperHeader (prog_version);
	ProgInfo ();
	NeperFoot ();
	return 1;
      }
      else if (strcmp (Arg, "--version") == 0)
      {
	printf ("%s\n", prog_version);

	return EXIT_SUCCESS;
      }
      else if (strcmp (Arg, "--license") == 0)
      {
	 ut_print_gplv3 (stdout);
	return EXIT_SUCCESS;
      }
    }
  }

  Res = ut_string_comp (argv[1], ArgList, ArgQty, Arg);

  /* rc stuff */
  if (strcmp (Arg, "--rcfile") == 0)
  {
    NeperHeader (prog_version);
    header_print = 0;
    if (strcmp (argv[2], "none") != 0)
    {
      ut_print_message (0, 0, "Loading initialization file `%s'...\n", argv[2]);
      argv += 2;
      argc -= 2;
      Res = ut_string_comp (argv[1], ArgList, ArgQty, Arg);
      ut_option_read (argv[0], &fargc, fargv, "neper", Arg);
    }
    else
    {
      ut_print_message (0, 0, "Ignoring initialization file.\n");
      fargc = 0;
      argv += 2;
      argc -= 2;
      Res = ut_string_comp (argv[1], ArgList, ArgQty, Arg);
    }
  }
  else if (strcmp (Arg, "--version") != 0)
  {
    inifile = ut_alloc_1d_char (1000);
    sprintf (inifile, "%s/.neperrc", getenv ("HOME"));
    NeperHeader (prog_version);
    header_print = 0;
    if (ut_file_exist (inifile))
      ut_print_message (0, 0, "Loading initialization file `%s'...\n",
			inifile);
    else
      ut_print_message (0, 0, "No initialization file found (`%s').\n",
			inifile);
    ut_option_read (inifile, &fargc, fargv, "neper", Arg);
    ut_free_1d_char (inifile);
  }

  if (Res == 1)
  {
    ut_print_lineheader (2);
    printf ("Several possibilities for option `%s'.\n", argv[1]);
    ut_arg_badarg ();
  }
  else if (Res == -1)
  {
    ut_print_lineheader (2);
    printf ("Unknown option `%s'.\n", argv[1]);
    ut_arg_badarg ();
  }

  if (header_print)
    NeperHeader (prog_version);

  if (strcmp (Arg, "-T") == 0)
    neper_t (fargc, fargv, argc - 1, argv + 1);
  else if (strcmp (Arg, "-O") == 0)
    neper_o (fargc, fargv, argc - 1, argv + 1);
  else if (strcmp (Arg, "-FM") == 0)
    neper_fm (fargc, fargv, argc - 1, argv + 1);
  else if (strcmp (Arg, "-MM") == 0)
    neper_mm (fargc, fargv, argc - 1, argv + 1);
  else if (strcmp (Arg, "-VS") == 0)
    neper_vs (fargc, fargv, argc - 1, argv + 1);

  gettimeofday (&end_time, &zone);
  t = ut_time_subtract (&beg_time, &end_time);

  ut_print_message (0, 1, "Elapsed time: %.3f secs.\n", t);

  /* printing program foot */
  NeperFoot ();

  ut_free_2d_char (fargv, 100);
  ut_free_2d_char (ArgList, 100);
  ut_free_1d_char (Arg);
  ut_free_1d_char (prog_version);

  return 0;
}

void
NeperHeader (char* prog_version)
{
  printf
    ("\n========================    N   e   p   e   r    =======================\n");
  
  ut_print_message (0, 0,
		    "A 3D random polycrystal generator for the finite element method\n");

  ut_print_message (0, 0, "Version %s\n", prog_version);

  if (NEPER_VERSION_DEVEL != -1)
   ut_print_messagewnc (0, 72, "This is a RELEASE CANDIDATE version of Neper. General distribution is discouraged.");

  ut_print_message (0, 0, "(compiled with:");
#ifdef HAVE_GSL
    printf (" gsl");
#endif

#ifdef HAVE_LIBMATHEVAL
    printf (" libmatheval");
#endif

#ifdef HAVE_LIBSCOTCH
    printf (" libscotch");
#endif

  printf (")\n");

  return;
}

void
NeperFoot ()
{
  printf
    ("========================================================================\n\n");

  return;
}

void
ProgInfo ()
{

     ut_print_message (0, 0, "<http://neper.sourceforge.net>\n");

  ut_print_message (0, 0, "Copyright (C) 2003-2012, and GNU GPL'd, by Romain Quey.\n");
     // ut_print_message (0, 0, "This program comes with ABSOLUTELY NO WARRANTY; this is free\n");
     // ut_print_message (0, 0, "software, and you are welcome to redistribute it under certain\n");
     // ut_print_message (0, 0, "conditions; for details, run `neper --license'.\n");


  ut_print_message (0, 72,
		       "Send comments, suggestions or bug reports to\n");
  ut_print_message (0, 72, "<%s>.\n", NEPER_EMAIL);

  return;
}
