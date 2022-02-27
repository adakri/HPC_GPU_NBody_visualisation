# An HPC study of the visualisation of an N-body simulation

GP-GPU project 3A MMIS: Arthir Vivière, Abdelmouttaleb Dakri, Ilyes Moudden

## Short description

This project aims at studying different aspects of real-time viusalisations of N-body simulations. We try and implement a sequential all-pair algorihtm to set up a proper physical simulation and visualization. We then explore different optimisation methids in order to boost the performance of our rendered scene (mainly the Barnes-Hut approximation and GPU parallelization).

This project is entirely coded in C/C++/CUDA and contains to different builds that can be run independantly. The following librairies are only necessary for the use case:

* [CUDA](https://developer.nvidia.com/cuda-downloads) : Needed for the GPU parallel computing part, not needed if one only wants to test the sequential code.

*[OpenGL & Freeglut](http://freeglut.sourceforge.net/) : For the graphics part, needed to render the simulation.

* [Cmake](https://cmake.org/) : For the packaging, build and compilation .

## How to get it up and running ?

Of course running the parallel code needs an NVIDIA GPU, in our case an *NVIDIA GTX 1060TI*. And of course one needs to install the necessary librairies.

### Installing the librairies through the package manager

We leave it to the reader (for now) to download the necessary libraires through the package manager (OpneGL, Freeglut and CUDA's nvcc). Freeglut is already provided in the **OpenGL_libs** folder. Of course gcc/g++ compilers are needed as well as CMake.

### Building the project

For the sequential part:

```shell
cmake -DPARALLEL=OFF -S ../ -B .
```

The executable in this case is named *nbody* and can be executed once compiled.

For the parallel part, the executable is name *nbody_cuda* and can be built like this:

```shell
cmake -DPARALLEL=ON -S ../ -B .
```

## Presentation of the software and the results

We obtain some results through our simulations, a more in depth discussion was led through our  [project report](https://www.overleaf.com/read/gbstyqmzwfkb) We otherwise this demo dor the actual physical simulation (one we used to check the validity od our simulations):

<img src="Results/demo_100_barnes.gif" width="840" height="640" />_barnes


