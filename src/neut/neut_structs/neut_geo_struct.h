/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTGEO_H
#define STRUCTGEO_H

struct GEO
{
  // GENERAL INFORMATION -----------------------------------------------
  char* version;

  int N;              // value of option -n.

  int Id;             // identifier of the tessellation

  char* morpho;       // morphology (poisson|cube|dodeca|tocta|custom)

  char* Type;         // type of tessellation (standard)

  double sel;         // small edge length used for regularization
  double maxff;       // fmax used for regularization
  char* dbound;       // domain boundary definition
  double dboundsel;   // domain boundary small edge length

  // VERTEX INFORMATION ------------------------------------------------
  
  int VerQty;         // number of vertices

  // For vertex i (i=1...VerQty):
  double **VerCoo;    // VerCoo[i][j] (j=0...2) are the 3 coordinates.
  
  int *VerEdgeQty;    // VerEdgeQty[i] is the quantity of parent edges.
  int **VerEdgeNb;    // VerEdge[i][j] (j=0...) are the parent edges.

  int **VerDom;     // VerDom[i][0] is the boundary type:
                      // 0: ver = domain ver
		      // 1: ver on a domain edge
		      // 2: ver on a domain face
		      // -1: ver not on domain
		      // VerDom[i][1] is the id of the boundary entity

  int *VerState;      // VerState[i] is the state of vertex i:
		      // 0: unchanged, -1: deleted, >0: modified =
		      // vertex weight (i.e. nb of vertices it
		      // represents).

  // EDGE INFORMATION --------------------------------------------------
  
  int EdgeQty;

  // For edge i (i=1...EdgeQty):
  int **EdgeVerNb;    // VerNb[i][j] (j=0,1) are the numbers of the two 
                      // vertices.
		      
  int *EdgeFaceQty;   // EdgeFaceQty[i] is the quantity of parent faces.
  int **EdgeFaceNb;   // EdgeFaceNb[i][j] (j=0...) are the numbers of the
                      // parent faces.
		      
  double *EdgeLength; // EdgeLength[i] is the length.

  int *EdgeState;     // EdgeState[i]=0 means edge i still exists (=-1 else)

  int *EdgeDel;	      // EdgeDel[i]=0 means edge i can be deleted;
                      // -1 -> to large.
		      //
  int **EdgeDom;    // 

  // FACE INFORMATION --------------------------------------------------
  
  int FaceQty;	
  
  // For face i (i=1...FaceQty):
  int **FacePoly;     // FacePoly[i][j] (j=0,1) are the numbers of the two 
		      // parent polys.

  double **FaceEq;    // FaceEq[i][j] (j=0...3) are the four equation
		      // parameters:
	// FaceEq[i][1]*X1+FaceEq[i][2]*X2+FaceEq[i][3]*X3=FaceEq[i][0]
		      
  int *FaceVerQty;    // FaceVerQty[i] is the quantity of face vertices
  int **FaceVerNb;    // FaceVerNb[i][j] (j=1...FaceVerQty[i]) are the 
                      // numbers of the vertices.
		      
  int **FaceEdgeNb;   // FaceEdgeNb[i][j] (j=1...FaceVerQty[i]) are the
                      // numbers of the edges.
		      
  int **FaceEdgeOri;  // FaceEdgeOri[i][j] (j=1...FaceVerQty[i]) are the 
                      // orientations of the edges.
		      
  int *FaceState;     // FaceState[i] = 0 means face i is unmodified,
                      // >0 means face i is modified, == -1 means face
		      // i is deleted.
		      
  int *FacePt;	      // point for the interpolation of the face. 
                      // if facept>0, facept=nb of the face ver. 
                      // if facept=0, the barycenter is considered.
                      // if facept=-1, undefined (e.g. for remeshing).
  double **FacePtCoo; // coo of the pt used for interpolation.
  double *FaceFF;     // ff of the face.
  int **FaceDom;     // boundary number of the face (none = 0, otherwise > 0).

  // POLYHEDRON INFORMATION --------------------------------------------
  
  int PolyQty;

  // For polyhedron i (i=1...PolyQty):

  int* PolyId;
  
  double **CenterCoo; // CenterCoo[i][j] (j=0...2) are the 3 coordinates
                      // of the center.

  int *PolyFaceQty;   // PolyFaceQty[i] is the quantity of faces.
  int **PolyFaceNb;   // PolyFaceNb[i][j] (j=1...PolyFaceQty[i]) are the 
                      // numbers of the faces.
  int **PolyFaceOri;  // PolyFaceNb[i][j] (j=1...PolyFaceQty[i]) are the 
                      // orientations of the faces: 1 if the face normal
		      // is out-going, -1 otherwise.









  int* PolyTrue;      // true level of the polyhedron (0 if not true, 1
		      // is true, 2 if all neighs are true>=1, 3 if all
		      // neighs are true>=2, etc.

  int* PolyBody;      // same than PolyTrue, but for body.

  // DOMAIN INFORMATION ------------------------------------------------
  char* DomType;
  
  int   DomVerQty;
  int*  DomVerEdgeQty;
  int** DomVerEdgeNb;  // 0 indexed
  int*  DomTessVerNb; 

  int   DomEdgeQty;
  int** DomEdgeVerNb;  // 0 indexed
  int** DomEdgeFaceNb; // 0 indexed
  int*  DomTessEdgeQty;
  int** DomTessEdgeNb; // 1 indexed

  int   DomFaceQty;
  double** DomFaceEq;
  int*  DomFaceVerQty;
  int** DomFaceVerNb;  // 1 indexed
  int** DomFaceEdgeNb; // 1 indexed
  int*  DomTessFaceQty;
  int** DomTessFaceNb; // 1 indexed
};
typedef struct GEO GEO;

#endif /* STRUCTGEO_H */
