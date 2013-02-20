/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neper_vs.h"

int
neper_vs (int fargc, char **fargv, int argc, char **argv)
{
  int i;
  struct PRINT Print;
  struct PRINT* MPrint = NULL;
  
  // Tessellation
  struct MTESS    MTess;
  struct TESS*    Tess = NULL;
  
  // Mesh and data
  struct NODES    Nodes;
  struct MESH     Mesh0D;
  struct MESH     Mesh1D;
  struct MESH     Mesh2D;
  struct MESH     Mesh3D;
  struct TESSDATA TessData;
  struct TESSDATA* MTessData = NULL;
  struct MESHDATA MeshData;
  struct SCALE    Scale;

  // Slice: mesh and data
  int SQty = 0;
  struct NODES*    SNodes;
  struct MESH*     SMesh2D;
  struct MESHDATA* SMeshData;

  int expargc;
  char** expargv = NULL;

  ut_arg_expand (argc, argv, &expargc, &expargv);

  // init general stuff ---
  neut_print_set_default (&Print);
  neut_mtess_set_zero    (&MTess);
  neut_nodes_set_zero    (&Nodes);
  neut_mesh_set_zero     (&Mesh0D);
  neut_mesh_set_zero     (&Mesh1D);
  neut_mesh_set_zero     (&Mesh2D);
  neut_mesh_set_zero     (&Mesh3D);
  neut_tessdata_set_default (&TessData);
  neut_meshdata_set_default (&MeshData);
  neut_scale_set_default (&Scale);
  // need to default Data and SData here; neut_data_set_default ();

  // Printing module header --------------------------------------------
  ut_print_moduleheader ("-VS", fargc, fargv, argc, argv);

  // Checking if povray is present -------------------------------------
  neut_povray_check_error ();

  for (i = 1; i < expargc; i++)
  {
    // tessellation / mesh loading -------------------------------------
    if (strncmp (expargv[i], "-load", 5) == 0)
    {
      nevs_load (expargv, &i, &Nodes, &Mesh0D, &Mesh1D, &Mesh2D,
	         &Mesh3D, &MTess, &Tess);

      if (Tess[1].PolyQty > 0)
      {
	TessData.verqty  = Tess[1].VerQty;
	TessData.edgeqty = Tess[1].EdgeQty;
	TessData.faceqty = Tess[1].FaceQty;
	TessData.polyqty = Tess[1].PolyQty;
      }

      if (Mesh3D.EltQty > 0)
      {
	MeshData.elset3dqty = Mesh3D.ElsetQty;
	MeshData.elset2dqty = Mesh2D.ElsetQty;
	MeshData.elset1dqty = Mesh1D.ElsetQty;
	MeshData.elset0dqty = Mesh0D.ElsetQty;
	MeshData.elt3dqty   = Mesh3D.EltQty;
	MeshData.elt2dqty   = Mesh2D.EltQty;
	MeshData.elt1dqty   = Mesh1D.EltQty;
	MeshData.elt0dqty   = Mesh0D.EltQty;
	MeshData.nodeqty    = Nodes.NodeQty;
      }
    }

    // data loading and settings ---------------------------------------
    else if (strncmp (expargv[i], "-data", 5) == 0)
      nevs_data (expargv, &i, Tess[1], Nodes, Mesh1D, Mesh3D, &TessData,
	         &MeshData);

    // show settings ---------------------------------------------------
    else if (strncmp (expargv[i], "-show", 5) == 0)
      nevs_show (expargv, &i, Tess[1], Nodes, Mesh0D, Mesh1D, Mesh2D,
		 Mesh3D, &Print);

    // slicing ---------------------------------------------------------
    else if (strncmp (expargv[i], "-slice", 5) == 0)
      nevs_slice (expargv, &i, &Print);

    // camera settings -------------------------------------------------
    else if (strncmp (expargv[i], "-camera", 7) == 0)
      nevs_camera (expargv, &i, Tess[1], Nodes, Mesh3D, MeshData, &Print);
    
    // image settings --------------------------------------------------
    else if (strncmp (expargv[i], "-image", 6) == 0)
      nevs_image (expargv, &i, &Print);
    
    // scale settings --------------------------------------------------
    // else if (strncmp (expargv[i], "-scale", 6) == 0)
      // nevs_scale (expargv, &i, &Data, DataQty);
    
    // include settings ------------------------------------------------
    else if (strncmp (expargv[i], "-include", 8) == 0)
      nevs_include (expargv, &i, &Print);

    // image printing --------------------------------------------------
    else if (strcmp (expargv[i], "-print") == 0)
    {
      nevs_data_init (MTess, Tess, Print.showtesslevel, &TessData,
	  &MTessData, Nodes, Mesh3D, &MeshData);
      
      if (Print.slice != NULL)
	nevs_slice_mesh (Nodes, Mesh3D, MeshData, Print.slice, 
	                 &SQty, &SNodes, &SMesh2D, &SMeshData);

      int j;
      for (j = 0; j < SQty; j++)
	nevs_meshdata_init (SNodes[j], SMesh2D[j], &(SMeshData[j]));

      nevs_show_init (MTess, Tess[1], Nodes, Mesh0D, Mesh1D, Mesh2D,
		      Mesh3D, SQty, &Print);

      if (Print.showtess)
	nevs_show_print_mprint (MTess, Tess, Print, &MPrint);

      nevs_camera_init (Tess[1], Nodes, Mesh3D, MeshData, &Print);

      nevs_print (expargv, &i, Print, MPrint, MTess, Tess, TessData, MTessData,
	          Nodes, Mesh0D, Mesh1D,
		  Mesh2D, Mesh3D, MeshData, SQty, SNodes, SMesh2D, SMeshData);
    }

    // process image(s) ------------------------------------------------
    else if (strncmp (expargv[i], "-processimage", 13) == 0)
      nevs_processimage (expargv, &i);

    else
    {
      ut_print_message (2, 0, "Unknown option %s (i = %d)\n", expargv[i], i);
      abort ();
    }
  }

  neut_print_free (&Print);
  neut_mtess_free (MTess, Tess);
  neut_nodes_free (&Nodes);
  neut_mesh_free (&Mesh0D);
  neut_mesh_free (&Mesh1D);
  neut_mesh_free (&Mesh2D);
  neut_mesh_free (&Mesh3D);
  neut_scale_free (&Scale);
  neut_tessdata_free (&TessData);
  neut_meshdata_free (&MeshData);

  ut_free_2d_char (expargv, argc);

  // neut_nodes_free (&SNodes);
  // neut_mesh_free  (&SMesh2D);

  return 0;
}
