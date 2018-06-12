#include "BlockRange.h"
#include <stdlib.h>

int * getStrides(int n, int size) {
    int i;
    int * strides = (int *)malloc(sizeof(int)*n);
    for(i=0;i<size;i++) {
        BlockRange br = getBlockRange(n,i,size);
        strides[i]=br.stop-br.start+1;
    }

    return strides;
}

int * getOffsets(int n, int size) {
    int i;
    int * offsets = (int *)malloc(sizeof(int)*n);
    for(i=0;i<size;i++) {
        BlockRange br = getBlockRange(n,i,size);
        offsets[i]=br.start;
    }

    return offsets;
}

BlockRange getBlockRange(int n, int rank, int size) {
    BlockRange rv;
    int r;
    int nextStart=0;
    int start=0;
    int blockSize=0;
    for(r=0;r<=rank;r++) {
        start=nextStart;
        blockSize = n/size;
        if(size*blockSize+r<n) blockSize += 1;
        nextStart = start+blockSize;
    }
    rv.start = start;
    rv.stop = start+blockSize-1;
    return rv;
}

