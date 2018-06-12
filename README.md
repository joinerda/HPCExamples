# HPCExamples
Example codes to demonstrate HPC principles. This is a collection of codes I have used in my HPC class to demonstrate different concepts.

The WAVE_COMP module has examples of solving the wave equation in 1-D through a finite difference approach using Euler's method, solved using a variety of different languages. The purpose of the module is to allow students to compare and benchmark language choice and its imapact on time to science using an easy to describe and read, mostly memory coherent application.

The OPTIMIZATION module has code examples designed to show impact of some key principles in loop optimization.

The MANDY directory has starter code to solve for the Mandelbrot set and save as an image using GD. Starter code and one solution for a parallelization of the loop using OpenMP are included. The goal of this module is to give students an example of a calculation to parallelize, with a discussion of scheduling and load balancing.
