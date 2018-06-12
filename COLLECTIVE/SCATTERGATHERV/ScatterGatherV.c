#include "BlockRange.h"
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int main(int argc, char ** argv) {
    int i;
    int n = 19;
    int * offsets;
    int * strides;
    int myStride;
    int myOffset;
    double * someNumbers;
    double * splitNumbers;
    double * allNumbers;
    double * startingNumbers;
    int rank,size;

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    // strides is the length of each ranks sub-array
    strides = getStrides(n,size);
    // offsets is the starting index in the master array
    //    of each subarray
    offsets = getOffsets(n,size);

    myStride = strides[rank];
    myOffset = offsets[rank];
    
    // allocate space. Rank Zero gets the master arrays, all ranks
    //   get the sub arrays.
    // staring numbers will be split into the split array on each
    //    rank. Some more numbers will be calculated on each rank
    //    These will be then gathered back at rank 0 into a
    //    master results array with all the numbers.
    someNumbers = (double *)malloc(sizeof(double)*strides[rank]);
    splitNumbers = (double *)malloc(sizeof(double)*strides[rank]);
        // (it wouldn't hurt if every rank allocates the master array
        //   this is just to show that only rank zero
        //   NEEDS it, but don't fear redundancy in MPI codes.)
    if(rank==0) {
        startingNumbers = (double *)malloc(sizeof(double)*n);
        allNumbers = (double *)malloc(sizeof(double)*n);
    }
    srand(rank);


    // rank zero sets up the initial master input array
    if(rank==0) {
        for(i=0;i<n;i++) {
            startingNumbers[i] = (double)rand()/RAND_MAX;
        }
    }
    // scatter into subarrays on each rank
    MPI_Scatterv(startingNumbers, strides, offsets,
                 MPI_DOUBLE, splitNumbers, myStride,
                 MPI_DOUBLE,
                 0, MPI_COMM_WORLD);

    // each rank does just its work, and stores its local
    //   subarray
    for(i=0;i<myStride;i++) {
        someNumbers[i] = (double)rand()/RAND_MAX + splitNumbers[i];
    }

    // gather back into a master array on rank zero.
    MPI_Gatherv(someNumbers,myStride,MPI_DOUBLE,allNumbers,
        strides,offsets,MPI_DOUBLE,0,MPI_COMM_WORLD);

    // rank zero outputs all results.
    if(rank==0) {
        for(i=0;i<n;i++) {
            printf("%g \n",allNumbers[i]);
        }
    }

    free(offsets);
    free(strides);
    free(someNumbers);
    free(splitNumbers);
    if(rank==0) {
        free(startingNumbers);
        free(allNumbers);
    }

    MPI_Finalize();
    
}
