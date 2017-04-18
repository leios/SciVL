/*-------------operations.h---------------------------------------------------//
*
* Purpose: This file holds all necessary functions for vector operations
*
*-----------------------------------------------------------------------------*/

#ifndef OPERATIONS_H
#define OPERATIONS_H
#include <iostream>
#include <fftw3.h>
#include <cmath>

// Functions to calculate the absolute value
double abs(fftw_complex value);
double *abs(double *value, int size);
double *abs(fftw_complex *value, int size);

// Functions to calculate absolute value squared
double abs2(fftw_complex value);
double *abs2(double *value, int size);
double *abs2(fftw_complex *value, int size);

#endif
