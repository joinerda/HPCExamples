# HPCExamples
Example codes to demonstrate HPC principles. This is a collection of codes I have used in my HPC class to demonstrate different concepts.

The WAVE_COMP module has examples of solving the wave equation in 1-D through a finite difference approach using Euler's method, solved using a variety of different languages. The purpose of the module is to allow students to compare and benchmark language choice and its imapact on time to science using an easy to describe and read, mostly memory coherent application.

The OPTIMIZATION module has code examples designed to show impact of some key principles in loop optimization.

The MANDY directory has starter code to solve for the Mandelbrot set and save as an image using GD. Starter code and one solution for a parallelization of the loop using OpenMP are included. The goal of this module is to give students an example of a calculation to parallelize, with a discussion of scheduling and load balancing.

The NBODY directory has starter code as a parallelization challenge with a simple direct force n-body calculation. A README file also covers compiling with options for gprof to look at profiling for the code.

The GETOPT module is an example of using getopt for command line parsing. While not a specific issue for HPC, as setting up inputs for profiling in a scheduler or at the command line is often easier with command line arguments than input files, this is shown here for students to modify as needed to add command line argument parsing to their codes.

The COLLECTIVE module shows code examples of some of the most commonly used collective communication calls in MPI

