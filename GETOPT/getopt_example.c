
#include <stdio.h>
#include <ctype.h>    // needed for isprint
#include <math.h>
#include <unistd.h>   // needed for getopt
#include <string.h>   // needed for strncpy
#include <stdlib.h>


int main(int argc, char ** argv) {
    double x=0.5;
    int a=1;
    int f=0;
    char name[256];
    int command_code;

    // get command line arguments using getopt
    while ((command_code = getopt (argc, argv, "x:a:n:f")) != -1)
    switch (command_code) {
      case 'x':  
        sscanf(optarg,"%lf",&x);
        break;
      case 'a': 
        sscanf(optarg,"%d",&a);
        break;
      case 'n':
        strcpy(name,optarg);
        break;
      case 'f':
        f=1;
        break;
      case '?':   // error messages
        if (optopt == 'x' ||
            optopt == 'a' ||
            optopt == 'n')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr,
                   "Unknown option character `\\x%x'.\n",
                   optopt);
        return 1;
      default:
        abort ();
    }
    printf("USAGE: getopt_example -x (float) -a (integer) -f (flag) -n (string) -d \n");

    // echo input
    printf("x (float) = %lf\n",x);
    printf("a (int) = %d\n",a);
    printf("n (string) = %s\n",name);
    printf("f (flag) = %d\n",f);


}
