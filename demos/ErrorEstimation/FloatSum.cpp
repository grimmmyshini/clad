//--------------------------------------------------------------------*- C++ -*-
// clad - The C++ Clang-based Automatic Differentiator
//
// A demo demonstrating the usage of clad's error estimation capabilities.
//
// author:  Garima Singh
//----------------------------------------------------------------------------//

// To run the demo please type:
// path/to/clang  -Xclang -add-plugin -Xclang clad -Xclang -load -Xclang \
// path/to/libclad.so  -I../include/ -x c++ -std=c++11 FloatSum.cpp
//
// A typical invocation would be:
// ../../../../obj/Debug+Asserts/bin/clang  -Xclang -add-plugin -Xclang clad \
// -Xclang -load -Xclang ../../../../obj/Debug+Asserts/lib/libclad.dylib     \
// -I../include/ -x c++ -std=c++11 FloatSum.cpp
//
// To plot the results install gnuplot and type:
//
// 1) The following if you want a comparision between calculated reuslts
//
// NOTE: By default clad calculates the error estimates pessimistically,
// which means that all estimates generated by clad reperesent the worst case
// error in all floating point operations. Hence, the in-built model gives an
// upperbound to the error in the function rather than a close estimate.
// To know how you can achieve closer estimates, you may checkout the
// "CustomModel" demo or Clad tutorials.
//
// gnuplot -e "plot 'actualError.dat' using 1:2 with lines; replot \
// 'estimateError.dat' using 1:2 with lines; pause -1"
//
// 2) The following if you want to visualize the difference between
// float and fixed results
//
// gnuplot -e "plot 'kahan.dat' using 1:2 with lines; replot 'vanilla.dat' \
// using 1:2 with lines; pause -1"

// Necessary for clad to work include
#include "clad/Differentiator/Differentiator.h"

#include <iostream> // Necessary for printing to stdout 
#include <fstream>  // Necessary for saving data to plot.
#include <iomanip>  // Necessary for std::setprecision
#include <cmath>    // Necessary for math functions

// Repeated addition of finite precision floating point numbers leads to a loss
// of significance in the results. Hence, here we consider 2 functions, one is
// a vanilla summation algortihm and the other is a compensated summation
// algorithm. We will be using the latter as the ground truth and will be
// comparing the results of our estimation accordingly.

// This is the vanilla summation algorithm in single precision
float vanillaSum(float x, unsigned int n) {
  float sum = 0.0;
  for (unsigned int i = 0; i < n; i++) {
    sum = sum + x;
  }
  return sum;
}

// this is the compensated summation algorithm, also known as 'Kahan Summation
// Algorithm'. This algorithm is used to reduce the loss of precision incurred
// while adding finite floating-point numbers. This algorithm maintains a
// running compensation term to store the error from the resulting calculations.
double kahanSum(float x, unsigned int n) {
  double sum = 0.0;

  // This is the compensation term
  double c = 0.0;

  for (unsigned int i = 0; i < n; i++) {
    double y = x - c;
    double t = sum + y;

    // Algebraically, c is always 0
    // when t is replaced by its
    // value from the above expression.
    c = (t - sum) - y;
    sum = t;
  }
  return sum;
}

int main() {

  auto df = clad::estimate_error(vanillaSum);

  // Select starting parameters
  double x = 0.10003E-5, finalError;
  unsigned int n = 10;
  float ret[2];

  // Build data streams for plotting results later
  std::ofstream kahan("kahan.dat");
  std::ofstream vanilla("vanilla.dat");
  std::ofstream actual("actualError.dat");
  std::ofstream estimate("estimateError.dat");

  for (int i = 0; i < 50; i++, n += 5) {

    // Clear the return array.
    // Otherwise the previous derivatives will carry over.
    ret[0] = ret[1] = 0;

    // Clear the final error
    finalError = 0;

    // First execute the derived function.
    df.execute(x, n, ret, finalError);

    double kahanResult = kahanSum(x, n);
    double vanillaResult = vanillaSum(x, n);

    // Calculate the actual estimate.
    // Here we use the kahanResult as the ground truth.
    double actualError = fabs(kahanResult - vanillaResult);

    std::cout << std::setprecision(19) << "\nIteration " << i
              << " with n = " << n << "\nFloat sum result: " << vanillaResult
              << "\nKahan Sum result: " << kahanResult
              << "\nError Estimated by clad: " << finalError
              << "\nActual Error: " << actualError << std::endl;

    // Save data to their respective streams
    kahan << n << "\t" << kahanResult << std::endl;
    vanilla << n << "\t" << vanillaResult << std::endl;
    // For errors, change to log scale
    actual << n << "\t" << fabs(log10(5 / actualError)) << std::endl;
    estimate << n << "\t" << fabs(log10(5 / finalError)) << std::endl;
  }
}