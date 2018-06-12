#include <stdlib.h>

int median_factorization(int n, int dim, int * values){
    int my_low=1;
    int my_high=n;
    int * my_values;
    int i;
    if (dim<2) {
        // cannot factor a number into only 1 factor
        return 0;
    } else if (dim==2) {
        // build factorization, return median factor pair
        for (i=1;i<my_high||i==1;i++) {
            if (n%i==0) {
                my_low=i;
                my_high=n/i;
            } 
        }
        values[0]=my_low;
        values[1]=my_high;
    } else {
        my_values = (int *) calloc(dim-1,sizeof(int));
        for (i=1;i<my_high||i==1;i++) {
            if (n%i==0) {
                median_factorization(n/i,dim-1,my_values);
                my_low=i;
                my_high=my_values[dim-1];
            }
        }
        values[0]=my_low;
        for (i=1;i<dim;i++) {
            values[i]=my_values[i-1];
        }
        free(my_values);
    }
    return 1;
}
