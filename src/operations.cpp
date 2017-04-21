/*-------------operations.cpp-------------------------------------------------//
*
* Purpose: This file holds all necessary functions for vector operations
*
*-----------------------------------------------------------------------------*/

#include <iostream>
#include <fftw3.h>
#include "../include/operations.h"

double abs(fftw_complex value){
    return sqrt(value[0]*value[0] + value[1]*value[1]);
}
double *abs(double *value, int size){

    double *new_array;
    new_array = (double*)malloc(sizeof(double)*size);
    for (int i = 0; i < size; ++i){
        if (value[i] < 0){
            new_array[i] = -value[i];
        }
        else{
            new_array[i] = value[i];
        }
    }

    return new_array;

}
double *abs(fftw_complex *value, int size){

    double *new_array;
    new_array = (double*)malloc(sizeof(double)*size);
    for (int i = 0; i < size; ++i){
        new_array[i] = sqrt(value[i][0]*value[i][0] + value[i][1]*value[i][1]);
    }

    return new_array;
}
std::vector<double> abs(std::vector<double> value){

    std::vector<double> new_array(value.size());
    for (size_t i = 0; i < value.size(); ++i){
        if (value[i] < 0){
            new_array[i] = -value[i];
        }
        else{
            new_array[i] = value[i];
        }
    }

    return new_array;

}
std::vector<double> abs(std::vector<fftw_complex> value){
    std::vector<double> new_array(value.size());
    for (size_t i = 0; i < value.size(); ++i){
        new_array[i] = sqrt(value[i][0]*value[i][0] + value[i][1]*value[i][1]);
    }

    return new_array;

}


double abs2(fftw_complex value){
    return value[0]*value[0] + value[1]*value[1];
}
double *abs2(double *value, int size){

    double *new_array;
    new_array = (double*)malloc(sizeof(double)*size);
    for (int i = 0; i < size; ++i){
        new_array[i] = value[i]*value[i];
    }

    return new_array;

}
double *abs2(fftw_complex *value, int size){

    double *new_array;
    new_array = (double*)malloc(sizeof(double)*size);
    for (int i = 0; i < size; ++i){
        new_array[i] = value[i][0]*value[i][0] + value[i][1]*value[i][1];
    }

    return new_array;
}


std::vector<double> abs2(std::vector<double> value){
    std::vector<double> new_array(value.size());
    for (size_t i = 0; i < value.size(); ++i){
        new_array[i] = value[i]*value[i];
    }

    return new_array;

}
std::vector<double> abs2(std::vector<fftw_complex> value){
    std::vector<double> new_array(value.size());
    for (size_t i = 0; i < value.size(); ++i){
        new_array[i] = value[i][0]*value[i][0] + value[i][1]*value[i][1];
    }

    return new_array;

}