/**
	MNA_Matrix.h
	For the undergraduate EDA course offered at 
	Dept of Micro/Nano-electronics

	(c) Guoyong Shi, 
	All rights reserved, 2006-2015
	Shanghai Jiao Tong University

	If you modify the code, please keep the above copyright info.
*/

#ifndef MNA_MATRIX_H
#define MNA_MATRIX_H


#ifdef MATLAB
#include "mex.h"
#endif

extern int MatrixSize;	// size of the MNA matrix (i.e., the max dimension)
extern double **MNAMatrix;
extern double *RHS;


int  Get_Matrix_Size();
void Create_MNA_Matrix();
void Get_MNA_System(double **A, double **b);
void Index_All_Nodes();
void Init_MNA_System();
void Print_MNA_System();


#endif
