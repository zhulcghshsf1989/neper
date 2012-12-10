/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_mesh_gmsh.h"

int
nem_mesh_2d_gmsh (struct GEO Geo, int face, double *face_proj,
		   struct NODES Nodes, struct MESH Mesh1D,
		   double cl, char *gmsh, char *algo, double rnd,
		   double allowed_t,
		   struct NODES *pN, struct MESH *pM,
		   double* pacl, double *pelapsed_t)
{
  FILE *file = NULL;
  int i, nb, status;
  struct timeval beg_time, end_time;
  struct timezone zone = { 0, 0 };
  double *n = ut_alloc_1d (3);
  int **Elsets = NULL;
  int *ElsetLs = NULL;
  int ElsetQty;
  struct MESH M2;
  int match;
  struct MESH Skin;

  neut_nodes_free (pN);
  neut_mesh_free (pM);
  neut_mesh_set_zero (pM);
  neut_nodes_set_zero (pN);
  neut_mesh_set_zero (pM);
  neut_mesh_set_zero (&M2);
  neut_mesh_set_zero (&Skin);

  file = ut_file_open ("tmp.geo", "W");

/*********************************************************************** 
 * general options */

  fprintf (file, "// face %d.\n", face);
  fprintf (file, "// face state = %d\n", Geo.FaceState[face]);
  if (neut_gmsh_meshalgo2d_nb (algo, &nb) != 0)
  {
    printf ("\n");
    ut_print_message (2, 3, "Meshing algorithm `%s' unknown.\n", algo);
    abort ();
  }

  nem_mesh_gmsh_options (file, nb, 4, 1, rnd);

/*********************************************************************** 
 * writing nodes then 1D elements */

  nem_mesh_2d_gmsh_writenodes (Geo, Nodes, Mesh1D, face, face_proj, file);
  nem_mesh_2d_gmsh_write1dmesh (Geo, Mesh1D, face, file);

/*********************************************************************** 
 * writing face */

  nem_mesh_2d_gmsh_writeface (Geo, Mesh1D, face, cl, file);

  ut_file_close (file, "tmp.geo", "W");

/*********************************************************************** 
 * meshing */

  gettimeofday (&beg_time, &zone);
  char *Args = ut_alloc_1d_char (1000);
  sprintf (Args, "%s -2 -v 0 tmp.geo > /dev/null 2> /dev/null", gmsh);
  ut_sys_runwtime (gmsh, Args, allowed_t);
  gettimeofday (&end_time, &zone);
  (*pelapsed_t) = ut_time_subtract (&beg_time, &end_time);
  ut_free_1d_char (Args);

/*********************************************************************** 
 * reading 2D mesh */

  if (ut_file_exist ("tmp.msh") == 0)
    status = 1;
  else
  {
    status = 0;
    file = ut_file_open ("tmp.msh", "R");
    neut_mesh_fscanf_msh (file, pN, NULL, NULL, pM, NULL);
    ut_file_close (file, "tmp.msh", "R");
  
    if ((*pM).EltQty <= 0)
      status = 1;
    else
    {
      neut_mesh_dispfromcl (*pN, *pM, cl, pacl);

      // nodes: skin nodes (numbers refer to *pN)
      neut_mesh_init_nodeelts (pM, (*pN).NodeQty);
      neut_mesh2d_mesh1d (*pN, *pM, &M2, &Elsets, &ElsetLs, &ElsetQty, 0);
      neut_mesh_mergeelsets (&M2);

      neut_mesh_face_skinmesh (Geo, face, Mesh1D, &Skin);
      
      double** coo = ut_alloc_2d (Nodes.NodeQty + 1, 3);
      // node = i;
      for (i = 1; i <= Nodes.NodeQty; i++)
      {
	ut_array_1d_memcpy (coo[i], 3, Nodes.NodeCoo[i]);
	ut_space_projpoint_alongonto (Nodes.NodeCoo[i], face_proj, Geo.FaceEq[face]);
      }

      match = neut_mesh_mesh_match (Nodes, Skin, *pN, M2);

      // node = Mesh1D.EltNodes[i][j];
      for (i = 1; i <= Nodes.NodeQty; i++)
	ut_array_1d_memcpy (Nodes.NodeCoo[i], 3, coo[i]);
      ut_free_2d (coo, Nodes.NodeQty + 1);

      if (match == 1)
	status = 0;
      else
	status = 2;		// means meshes do not match

      ut_free_2d_int (Elsets, ElsetQty);
      ut_free_1d_int (ElsetLs);
    }
  }

  // aligning element normal to face normal (should be the case, but 
  // not always)

  for (i = 1; i <= (*pM).EltQty; i++)
  {
    neut_mesh_eltnormal (*pM, *pN, i, n);
    if (ut_vector_scalprod (face_proj, n) < 0)
      ut_array_1d_int_switch ((*pM).EltNodes[i], 0, 2);
  }

  if (status == 0)
  {
    // commenting these two lines will break multimeshing
    remove ("tmp.geo"); 
    remove ("tmp.msh");
  }

  ut_free_1d (n);
  neut_mesh_free (&Skin);
  neut_mesh_free (&M2);

  return status;
}

/* This code would be for the case where a 2D meshing from a 1D mesh was
 * possible with Gmsh, which happens not to be the case
int
nem_mesh_2d_gmsh_b (struct GEO Geo, int face,
		     struct NODES *pNodes, struct MESH Mesh1D,
		     char *gmsh, char *algo, double rnd,
		     double allowed_t,
		     struct NODES *pN, struct MESH *pM,
		     double* pelapsed_t)
{
  FILE *file = NULL;
  FILE *file2 = NULL;
  int i, nb, status;
  struct timeval beg_time, end_time;
  struct timezone zone = { 0, 0 };
  double* p1, *p2, *p3;
  double* n = ut_alloc_1d (3);
  struct MESH Skin;
  struct MESH Garbage;

  neut_nodes_set_zero (pN);
  neut_mesh_set_zero (pM);
  neut_mesh_set_zero (&Skin);
  neut_mesh_set_zero (&Garbage);

// writing boundary = "OD elements = *pNodes", then 1D elements

  neut_mesh_face_skinmesh (Geo, face, Mesh1D, &Skin);

  file2 = ut_file_open ("tmp-surf.msh", "W");
  nem_writemeshGmsh (file2, *pNodes, Skin, Garbage, Garbage);
  ut_file_close (file2, "tmp-surf.msh", "W");

// writing face

  file = ut_file_open ("tmp.geo", "W");
  neut_gmsh_meshalgo2d_nb (algo, &nb);
  nem_mesh_gmsh_options (file, nb, 4, 1, rnd);
  nem_mesh_2d_gmsh_writeface_b (-1, file);
  ut_file_close (file, "tmp.geo", "W");
*/

int
nem_mesh_3d_gmsh (struct GEO Geo, int poly, struct NODES Nodes,
		   struct MESH Mesh2D, double cl, double clconv,
                   char *gmsh, char
		   *algo, char *opti, double rnd, double allowed_t,
		   struct NODES *pN, struct MESH *pM,
		   double* pacl, double *pelapsed_t)
{
  FILE *file, *file2;
  char *Args = NULL;
  struct timeval beg_time, end_time;
  struct timezone zone = { 0, 0 };
  int nb, optinb, status;
  struct MESH M2;
  int **Elsets = NULL;
  int ElsetQty;
  int match;
  struct MESH Skin;
  struct MESH Garbage;
  struct PART PGarbage;

  double clmod;
  int redo, iter;

  double speed  = 0.2;

  neut_nodes_free (pN);
  neut_mesh_free (pM);

  neut_nodes_set_zero (pN);
  neut_mesh_set_zero (pM);
  neut_mesh_set_zero (&Skin);
  neut_mesh_set_zero (&M2);
  neut_mesh_set_zero (&Garbage);
  neut_part_set_zero (&PGarbage);

/*********************************************************************** 
 * writing 2D elements (w their nodes) in separate file */

  neut_mesh_poly_skinmesh (Geo, poly, Mesh2D, &Skin);

  file2 = ut_file_open ("tmp-surf.msh", "W");
  neut_mesh_fprintf_gmsh (file2, "2", Nodes, Garbage, Garbage, Skin,
      Garbage, PGarbage);
  ut_file_close (file2, "tmp-surf.msh", "W");

  clmod = cl;
  redo = 0;
  iter = 1;
  double prevacl = 1e32;
  int cross = 0;
  double lmean;

  do
  {
    // writing master file - general options + poly

    file = ut_file_open ("tmp.geo", "W");
    fprintf (file, "// Poly %d\n", poly);
    if (neut_gmsh_meshalgo3d_nb (algo, &nb) != 0)
    {
      printf ("\n");
      ut_print_message (2, 3, "Meshing algorithm `%s' unknown.\n", algo);
      abort ();
    }

    status = neut_gmsh_optialgo_nb (opti, &optinb);
    if (status != 0 && (opti != NULL && strlen (opti) > 0))
    {
      printf ("\n");
      ut_print_message (2, 3, "Mesh optimization algorithm `%s' unknown.\n", opti);
      abort ();
    }

    nem_mesh_gmsh_options (file, 1, nb, optinb, rnd);
    nem_mesh_3d_gmsh_writepoly (clmod, file);
    ut_file_close (file, "tmp.geo", "W");

    // meshing
    gettimeofday (&beg_time, &zone);
    Args = ut_alloc_1d_char (1000);
    sprintf (Args,
	     "%s -3 -v 0 -order 1 tmp.geo -o tmp.msh > /dev/null 2> /dev/null",
	     gmsh);
    ut_sys_runwtime (gmsh, Args, allowed_t);
    gettimeofday (&end_time, &zone);
    (*pelapsed_t) = ut_time_subtract (&beg_time, &end_time);
    ut_free_1d_char (Args);

  /*********************************************************************** 
   * reading 3D mesh */

    if (ut_file_exist ("tmp.msh") == 0)
    {
      status = 1;
      break;
    }
    else
    {
      status = 0;
      file = ut_file_open ("tmp.msh", "R");
      neut_mesh_fscanf_msh (file, pN, NULL, NULL, NULL, pM);
      ut_file_close (file, "tmp.msh", "R");
    }

    neut_mesh_meancl (*pN, *pM, &lmean);

    // printf ("mean = %f vs cl = %f (clmod = %f qty = %d cross = %d)\n", acl, cl, clmod, (*pM).EltQty, cross);

    if ((prevacl > cl && lmean < cl) || (prevacl < cl && lmean > cl))
      cross++;

    redo = 0;
    // if ((iter == 1 && lmean > (1 + clconv) * cl)
     // || (iter >  1 && ut_num_requal (lmean, cl, clconv) == 0))
    if (ut_num_requal (lmean, cl, clconv) == 0)
    {
      if (lmean > cl)
	clmod /= (1 + speed / (cross + 1));
      else
	clmod *= (1 + speed / (cross + 1));
      redo = 1;
    }

    if (redo == 1)
    {
      neut_nodes_free (pN);
      neut_mesh_free (pM);
    }

    // printf ("\niter = %d clmod = %f lmean = %f cl = %f\n", iter, clmod, lmean, cl);
    // printf ("redo = %d\n", redo);
    iter++;
  }
  while (redo == 1 && cross < speed / clconv && clmod >= cl / 5);

  if (ut_file_exist ("tmp.msh") == 0)
    status = 1;
  else
  {
    status = 0;
    file = ut_file_open ("tmp.msh", "R");
    neut_mesh_fscanf_msh (file, pN, NULL, NULL, NULL, pM);
    ut_file_close (file, "tmp.msh", "R");

    if ((*pM).EltQty <= 0)
      status = 1;
    else
    {
      neut_mesh_dispfromcl (*pN, *pM, cl, pacl);

      // nodes: skin nodes (numbers refer to *pN)
      neut_mesh_init_nodeelts (pM, (*pN).NodeQty);

      // useless here but neut_mesh3d_mesh2d requires the bbox for other
      // purposes...
      neut_nodes_init_boundingbox (pN);

      neut_mesh3d_mesh2d (*pN, *pM, &M2, &Elsets, &ElsetQty, 0);
      neut_mesh_mergeelsets (&M2);

      match = neut_mesh_mesh_match (Nodes, Skin, *pN, M2);

      if (match == 1)
	status = 0;
      else
	status = 2;		// means meshes do not match

      ut_free_2d_int (Elsets, ElsetQty);
    }
  }

  if (status == 0)
  {
    remove ("tmp.geo");
    remove ("tmp-surf.msh");
    remove ("tmp.msh");
  }

  neut_mesh_free (&Skin);
  neut_mesh_free (&M2);

  return status;
}
