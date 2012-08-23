/***************************************************************************
                          levenbergmarquardt.h  -  description
                             -------------------
    begin                : Sat Nov 23 2002
    copyright            : (C) 2002 by Jo Verbeeck
    email                : jo@localhost.localdomain
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef LEVENBERGMARQUARDT_H
#define LEVENBERGMARQUARDT_H


//#define FITTER_DEBUG //print some debug info
#include <src/fitters/fitter.h>
#include "src/core/model.h"
#include "src/core/curvematrix.h"
/**Maximum Likelyhood fitter for Poisson Statistics
  *@author Jo Verbeeck
  */
#define N 100
class LevenbergMarquardt : public Fitter  {
    size_t lambdaiter; //number of iterations for lambda
    double lambdac; //alpha starting step
    double nu; //factor with which to reduce lambda during itterations
    bool dolin;
    CurveMatrix Xprime;
    CurveMatrix XprimeT;
    CurveMatrix XTX;
    CurveMatrix XTXcopy;
    CurveMatrix Q;
    CurveMatrix R;
    CurveMatrix Work;
    CurveMatrix Step;
    CurveMatrix dtprime;
    CurveMatrix Ga;
    CurveMatrix GaT;
    CurveMatrix GaTGa;
    CurveMatrix Work2;
    CurveMatrix Y;
    CurveMatrix B;


    std::vector<double> x0;
    std::vector<double> x0nonlin;
    std::vector<double> d0;
    enum method_enum{QR,inversion};

public:
LevenbergMarquardt(Model* m);
~LevenbergMarquardt();
double goodness_of_fit()const;
std::string goodness_of_fit_string()const;
double likelyhoodfunction()const; //the real likelyhood function

void CRLB(); //calculate with Cramer Rao Lower Bound the sigmas of the parameters
double getcovariance(int i,int j); //get values from the covariance matrix, set it up first with preparecovariance
void preparecovariance(); //calculate the covariance matrix from the fischer information matrix
double likelihoodratio();
void updatemonitors(); //update the covariances for each parameter that has a monitor

//overloaded functions
void createmodelinfo();
void modified_partial_derivative(size_t j,const Spectrum* currentspectrum);
void calculate_ModifiedJacobian();
double calcstep(double lambda,method_enum method);
void preparestep(method_enum method);
double iteration();
void prepareforiteration();
void lin_from_nonlin();
void storecurrentparams();
void restorecurrentparams();
void calcscaling();
void calculate_dtprime();
void calculate_Y();
};

#endif
