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
#include <vector>

// function to find sign
double sign(double val);

// Functions to calculate the absolute value
double abs(fftw_complex value);
double *abs(double *value, int size);
double *abs(fftw_complex *value, int size);
std::vector<double> abs(std::vector<double> value);
std::vector<double> abs(std::vector<fftw_complex> value);

// Functions to calculate absolute value squared
double abs2(fftw_complex value);
double *abs2(double *value, int size);
double *abs2(fftw_complex *value, int size);
std::vector<double> abs2(std::vector<double> value);
std::vector<double> abs2(std::vector<fftw_complex> value);

// Function for normalization
void normalize(double *array, int size);
void normalize(fftw_complex *array, int size);

#endif
