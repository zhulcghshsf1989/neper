/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nevs_camera_init (struct GEO, struct NODES, struct MESH, struct MESHDATA, struct PRINT*);
extern void nevs_camera (char**, int*, struct GEO, struct NODES, struct MESH, struct MESHDATA,
                         struct PRINT*);
extern void nevs_camera_coo (char*, struct GEO, struct NODES, struct MESH, struct PRINT*, int);
extern void nevs_camera_lookat (char*, struct GEO, struct NODES, struct MESH, struct PRINT*, int);
