/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTIN_H
#define STRUCTIN_H

struct IN
{
  // prerequisites

  char* gmsh;     	    // absolute path to Gmsh

  // input data

  char* tess;               // input tess file
  char* vox;                // input vox  file
  char *mesh;               // input mesh file
  char* nodecoo;            // node coo file (goes with mesh)

  char* loadmesh;           // load an output mesh file

  // meshing options
  char* elttype;            // type of elements (tri or quad)
  int morder;               // order of the mesh (1 or 2)
  int meshdim;              // dimension of the mesh (0, 1, 2 or 3)

  char* meshpoly;           // polys to mesh (expression)
  char* meshface;           // faces to mesh (expression)

  char* outdim;             // dimensions of the output meshes

  char*  mesh2dalgo;        // 2d meshing algos
  char*  mesh3dalgo;        // 3d meshing algos
  char*  mesh3doptiexpr;    // expression of O (see the Neper paper)
  char*  mesh3doptidisexpr; // expression of Odis (see the Neper paper)
  double mesh2dmaxtime;     // 2D mesher max run time
  double mesh2drmaxtime;    // 2D mesher max relative run time
  int    mesh2diter;        // 2D mesher max number of iterations
  double mesh3dmaxtime;     // 3D mesher max run time
  double mesh3drmaxtime;    // 3D mesher max relative run time
  int    mesh3diter;        // 3D mesher max number of iterations
  double mesh3dclconv;      // 3D mesh cl convergence criterion
  int    mesh3dreport;      // report statistics on the 3D mesh (logical)

  int transportqty;         // number of data to transport
  char*** transportspec;    // data to transport, indexed [0...transportqtytransportqty-1]

  int singnodedup;          // duplicate singular nodes (logical)
  double dupnodemerge;      // merge duplicate nodes (tolerancy)

  char *nset;	            // list of nsets
  char* faset;              // list of fasets

  int partmode;             // partitioning mode: 0 = partitioning, 1 = mapping
  char* partmethod;         // partitioning method (in Scotch jargon)
  int partqty;              // number of partitions
  char* partarchfile;       // partitioning architecture file
  double partbalancing;     // partitioning balancing factor
  int partrenumber;         // renumber nodes / elts according to partitions
  int partsets;             // write partitions in the mesh file

  // output details

  char *format;             // format of the output mesh

  char* body;               // output file name, without extension
  char *msh;                // mesh file at the msh format
  char *abq;                // mesh file at the Abaqus format
  char *geof;               // mesh file at the Zset/Zebulon format
  char *fepx1;              // mesh file at the fepx format, parms file
  char *fepx2;              // mesh file at the fepx format, mesh  file
  char *fepx3;              // mesh file at the fepx format, surf  file
  char *fepx4;              // mesh file at the fepx format, opt   file
  char *fepx5;              // mesh file at the fepx format, bcs   file

  char* npart;              // node partition file
  char* epart;              // element partition file
  char *stn;                // statistics on the nodes
  char *ste;                // statistics on the elts
  char *stelset;            // statistics on the elsets

};
typedef struct IN IN;

#endif /* STRUCTIN_H */
