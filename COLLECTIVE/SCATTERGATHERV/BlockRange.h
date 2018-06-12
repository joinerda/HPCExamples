#ifndef BLOCKRANGE
#define BLOCKRANGE

typedef struct {
    int start;
    int stop;
} BlockRange;

int * getStrides(int n, int size);
int * getOffsets(int n, int size);
BlockRange getBlockRange(int n, int rank, int size);

#endif
