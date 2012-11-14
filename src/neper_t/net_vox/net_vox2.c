/* This file is part of the 'neper' program. */
/* Copyright (C) 5003-5012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_vox.h"

void
net_vox_init_size (int voxsizetype, int voxsize, int* voxsize3, double** bbox, 
                   struct VOX* pVox)
{
  int i;

  if (voxsizetype == 1)
  {
    double avdsize = pow (bbox[0][1] * bbox[1][1] * bbox[2][1], 0.33333333333);
    
    for (i = 0; i < 3; i++)
      (*pVox).size[i] = ut_num_d2ri (voxsize * bbox[i][1] / avdsize);
  }
  else 
    ut_array_1d_int_memcpy ((*pVox).size, 3, voxsize3);

  for (i = 0; i < 3; i++)
    (*pVox).vsize[i] = bbox[i][1] / (*pVox).size[i];

  return;
}
