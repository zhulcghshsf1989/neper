/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_print.h"

void
neut_print_set_default (struct PRINT* pPrint)
{
  // camera ------------------------------------------------------------
  (*pPrint).cameracooexpr = ut_alloc_2d_char (4, 100);
  sprintf ((*pPrint).cameracooexpr[1], "centre+v");
  sprintf ((*pPrint).cameracooexpr[2], "centre+v");
  sprintf ((*pPrint).cameracooexpr[3], "centre+v");
  (*pPrint).cameracoo     = ut_alloc_1d (3);

  (*pPrint).cameralookatinit = 0;
  (*pPrint).cameralookatexpr = ut_alloc_2d_char (4, 100);
  sprintf ((*pPrint).cameralookatexpr[1], "centre");
  sprintf ((*pPrint).cameralookatexpr[2], "centre");
  sprintf ((*pPrint).cameralookatexpr[3], "centre");
  (*pPrint).cameralookat = ut_alloc_1d (3);
  (*pPrint).camerasky = ut_alloc_1d (3);
  (*pPrint).camerasky[0] = 0;
  (*pPrint).camerasky[1] = 0;
  (*pPrint).camerasky[2] = 1;
  (*pPrint).cameraangle = 25;

  (*pPrint).cameraprojection = ut_alloc_1d_char (100);
  sprintf ((*pPrint).cameraprojection, "perspective");

  // image -------------------------------------------------------------
  (*pPrint).imagewidth  = 1200;
  (*pPrint).imageheight = 900;
  (*pPrint).imagebackground = ut_alloc_1d (3);
  ut_array_1d_set ((*pPrint).imagebackground, 3, 1);
  (*pPrint).imageantialias = 0;

  (*pPrint).showtess  = -1;
  (*pPrint).showmesh  = -1;
  (*pPrint).showslice = NULL;

  (*pPrint).showinit = 0;
  (*pPrint).showverinit = 0;
  (*pPrint).showedgeinit = 0;
  (*pPrint).showver   = ut_alloc_1d_int (1);
  (*pPrint).showedge  = ut_alloc_1d_int (1);
  (*pPrint).showface  = ut_alloc_1d_int (1);
  (*pPrint).showfaceinter = 0;
  (*pPrint).showpoly  = ut_alloc_1d_int (1);
  (*pPrint).showelt   = ut_alloc_1d_int (1);
  (*pPrint).showeltedge = ut_alloc_1d_int (1);
  (*pPrint).shownode  = ut_alloc_1d_int (1);
  (*pPrint).showelt0d = ut_alloc_1d_int (1);
  (*pPrint).showelt1d = ut_alloc_1d_int (1);
  (*pPrint).showelt2d = ut_alloc_1d_int (1);
  (*pPrint).showver   [0] = -1;
  (*pPrint).showedge  [0] = -1;
  (*pPrint).showface  [0] = -1;
  (*pPrint).showpoly  [0] = -1;
  (*pPrint).showelt   [0] = -1;
  (*pPrint).showeltedge[0] = -1;
  (*pPrint).shownode  [0] = -1;
  (*pPrint).showelt0d [0] = -1;
  (*pPrint).showelt1d [0] = -1;
  (*pPrint).showelt2d [0] = -1;
  (*pPrint).showshadow = 1;

  (*pPrint).datareduction = 1;
  
  (*pPrint).format = ut_alloc_1d_char (10);
  sprintf ((*pPrint).format, "png");

  (*pPrint).includepov = NULL;

  (*pPrint).slice = NULL;

  return;
}

void
neut_print_free (struct PRINT* pPrint)
{
  // camera ------------------------------------------------------------
  ut_free_2d_char ((*pPrint).cameracooexpr, 4);
  ut_free_1d ((*pPrint).cameracoo);

  ut_free_2d_char ((*pPrint).cameralookatexpr, 4);
  ut_free_1d ((*pPrint).cameralookat);

  ut_free_1d ((*pPrint).camerasky);

  ut_free_1d_char ((*pPrint).cameraprojection);

  // image -------------------------------------------------------------
  ut_free_1d ((*pPrint).imagebackground);
  ut_free_1d_int ((*pPrint).showver);
  ut_free_1d_int ((*pPrint).showedge);
  ut_free_1d_int ((*pPrint).showface);
  ut_free_1d_int ((*pPrint).showpoly);
  ut_free_1d_int ((*pPrint).showelt);
  ut_free_1d_int ((*pPrint).showeltedge);
  ut_free_1d_int ((*pPrint).shownode);
  ut_free_1d_int ((*pPrint).showelt0d);
  ut_free_1d_int ((*pPrint).showelt1d);
  ut_free_1d_int ((*pPrint).showelt2d);
  
  ut_free_1d_char ((*pPrint).format);
  ut_free_1d_char ((*pPrint).includepov);

  return;
}
