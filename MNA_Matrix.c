/**
	MNA_Matrix.c
	For the undergraduate EDA course offered at 
	Dept of Micro/Nano-electronics

	(c) Guoyong Shi, 
	All rights reserved, 2006-2015
	Shanghai Jiao Tong University

	If you modify the code, please keep the above copyright info.
*/

#ifdef MATLAB
#include "mex.h"
#endif

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include "Symbol_Table.h"
#include "MNA_Matrix.h"

#define WARNING_DIV_BY_ZERO \
		printf("\nWarning: divide by zero.");

int MatrixSize = 0;	// size of the MNA matrix (i.e., the max dimension)
double **MNAMatrix = NULL;
double **MNAAuxMatrix = NULL;
double *RHS = NULL;

/**
	Assign indexes to all nodes in the node table.
	The ground node (with name "0") must be assigned index zero.
	The rest nodes are assigned indexes from 1, 2, 3 continuously.
*/
void Index_All_Nodes()
{
    Insert_Index_Entry(0); // For the ground node index.

    // Go over all the nodes
    Node_Entry *itr = *NodeTable;
    while(itr != NULL)
    {
        int hash = NameHash(itr->name, NodeTableSize);
        int index = Insert_Index_Entry(hash);

        itr->index = index;

        itr = itr->next;
    }

    MatrixSize = IndexTableSize - 1; // number of nodes = matrix size
}

int Get_Matrix_Size()
{
	return MatrixSize;
}

/**
	The MATLAB matrix data are stored in column-major format.
*/
void Get_MNA_System(double **A, double **b)
{
	int i, j;

	for (j = 0; j <= MatrixSize; j++) {
		for (i = 0; i <= MatrixSize; i++) {
			(*A)[i+j*(MatrixSize+1)] = MNAMatrix[i][j];  // convert to column-major format
		}
		(*b)[j] = RHS[j];
	}
}


/**
	Create memory for MNAMatrix and RHS.
	The adopted storage is in DENSE matrix format.
	The zeroth row/col correspond to the ground.

	Must call Index_All_Nodes() before calling this function.

	(!!) Students can try SPARSE matrix format.
*/
void Init_MNA_System()
{
#if 1
	int i, j;
	
	if (MatrixSize == 0) {
		return;
	}

	MNAMatrix = (double**) malloc( (MatrixSize+1) * sizeof(double*) );
    MNAAuxMatrix = (double**) malloc( (MatrixSize+1) * sizeof(double*) );

	for (i = 0; i <= MatrixSize; i++) {
		MNAMatrix[i] = (double*) malloc( (MatrixSize+1) * sizeof(double) );
        MNAAuxMatrix[i] = (double*) malloc( (MatrixSize+1) * sizeof(double) );
	}

	RHS = (double*) malloc( (MatrixSize+1) * sizeof(double) );

	// Initialize to zero
	for (i = 0; i <= MatrixSize; i++) {
		for (j = 0; j <= MatrixSize; j++) {
			MNAMatrix[i][j] = 0.0;
            MNAAuxMatrix[i][j] = 0.0;
		}
		RHS[i] = 0.0;
	}
#endif
}

/**
	Create the MNA matrix by scanning the device table.
	In principle stamping of each element should be done by the device instance.
	In this example code we do not create device objects. 
	Therefore, stamping is done inside this function for simplicity.
	
	(!!) Students are required to develop device classes to fulfil this task.
*/
void Create_MNA_Matrix()
{
    Device_Entry *itr = *DeviceTable;
    while(itr != NULL)
    {
        switch(itr->name[0])
        {
            case 'R':   //resistor
                MNAMatrix[atoi(itr->nodelist[0]->name)][atoi(itr->nodelist[0]->name)] += 1.0/(itr->value);
                MNAMatrix[atoi(itr->nodelist[1]->name)][atoi(itr->nodelist[1]->name)] += 1.0/(itr->value);
                MNAMatrix[atoi(itr->nodelist[0]->name)][atoi(itr->nodelist[1]->name)] -= 1.0/(itr->value);
                MNAMatrix[atoi(itr->nodelist[1]->name)][atoi(itr->nodelist[0]->name)] -= 1.0/(itr->value);
                break;
            
            case 'G':   //vccs
                MNAMatrix[atoi(itr->nodelist[0]->name)][atoi(itr->nodelist[2]->name)] += (itr->value);
                MNAMatrix[atoi(itr->nodelist[0]->name)][atoi(itr->nodelist[3]->name)] -= (itr->value);
                MNAMatrix[atoi(itr->nodelist[1]->name)][atoi(itr->nodelist[2]->name)] -= (itr->value);
                MNAMatrix[atoi(itr->nodelist[1]->name)][atoi(itr->nodelist[3]->name)] += (itr->value);
                break;
            
            case 'I':   //isrc
                RHS[atoi(itr->nodelist[0]->name)] -= itr->value;
                RHS[atoi(itr->nodelist[1]->name)] += itr->value;
                break;
            
            case 'C':   //cap
                MNAAuxMatrix[atoi(itr->nodelist[0]->name)][atoi(itr->nodelist[0]->name)] += 1.0/(itr->value);
                MNAAuxMatrix[atoi(itr->nodelist[1]->name)][atoi(itr->nodelist[1]->name)] += 1.0/(itr->value);
                MNAAuxMatrix[atoi(itr->nodelist[0]->name)][atoi(itr->nodelist[1]->name)] -= 1.0/(itr->value);
                MNAAuxMatrix[atoi(itr->nodelist[1]->name)][atoi(itr->nodelist[0]->name)] -= 1.0/(itr->value);
                break;

            case 'L':   //Inductor
                MNAAuxMatrix[atoi(itr->nodelist[0]->name)][atoi(itr->nodelist[0]->name)] += (itr->value);
                MNAAuxMatrix[atoi(itr->nodelist[1]->name)][atoi(itr->nodelist[1]->name)] += (itr->value);
                MNAAuxMatrix[atoi(itr->nodelist[0]->name)][atoi(itr->nodelist[1]->name)] -= (itr->value);
                MNAAuxMatrix[atoi(itr->nodelist[1]->name)][atoi(itr->nodelist[0]->name)] -= (itr->value);
                break;

        };

        itr = itr->next;
    }

}

void Print_MNA_System()
{
	int i, j;

	printf("\n\n");
	for (j = 0; j <= MatrixSize; j++) {
		printf("\t%-22d", j);
	}
	printf("\tRHS");
	
	for (i = 0; i <= MatrixSize; i++) {
		printf("\n[%-3d]", i);
		for (j = 0; j <= MatrixSize; j++) {
			printf("\t%f", MNAMatrix[i][j]);
            printf("+%fs", MNAAuxMatrix[i][j]);
		}
		printf("\t%-12f", RHS[i]);
	}
}


