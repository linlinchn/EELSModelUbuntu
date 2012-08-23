/***************************************************************************
                          spectrum.h  -  Class to contain EELS and EDS spectra
                             -------------------
    begin                : Sat Oct 12 2002
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
#ifndef SPECTRUM_H
#define SPECTRUM_H
#include <iostream>
#include <vector>
#include <iostream>
#include <qmatrix.h>
#include <string>

/**
	 *	Basic class that holds a single spectrum
	 *  
	 */


class Component;
class Graph; //somewhere there is a class Graph,needed to be able to declare a pointer to graph here
class QWorkspace; //somewhere there is a class called QWorkspace
/**
  *@author Jo Verbeeck
  */
class loadmsa{};

class Spectrum {
//internal data
  unsigned int npoints;
  struct datapt{    double energy;    double counts;    double errorbar;  bool exclude;};
  std::vector<datapt>dataset;
  double pppc;                          //primary particles per count
Graph* graphptr; //pointer to graph of the spectrum
  std::string spectrumname;
  std::string xunits;
  std::string yunits;
  enum datatypetype{XY,Y};
  datatypetype datatype;
  QWidget* parentptr;
  std::string filename;
  double eshift;
//exceptions
  public:class Spectrumerr{   //class for exeptions
    public:class bad_index{int idx,pts;};
    class bad_alloc{};
    class not_same_size{int n1,n2;};
    public:class load_error{ public: const char* msgptr;load_error(const char* e){msgptr=e;}};
    class load_cancelled{};
  };


//member functions
public:
//constructors and destructor
		/**
		 *	Construct a dummy spectrum with zero size		
		 */
Spectrum();
		/**
		*	Construct a spectrum with size n				
		*/
Spectrum(unsigned int n);
		/**
		*	Construct a spectrum with size n,dispersion,Estart				
		*/
Spectrum(unsigned int,double,double);
		/**
		*	Construct a spectrum by loading it as msa file from the specified filename			
		*/
Spectrum(loadmsa l,std::string filename=0);
		/**
		*	Destructor of the spectrum	
		*/
virtual ~Spectrum();

public:
//inspectors
	
			/**
			*	@return the number of points in the spectrum (the size)
			*/
unsigned int getnpoints()const;
void resize(size_t size);
double getpppc()const;
double geteshift()const;
void getdatapoint(int index,double* en,double* cts,double* err)const;
double getcounts(int index)const;
double getenergy(int index)const;
size_t getenergyindex(double energy)const;

double getrawenergy(int index)const;
double geterrorbar(int index)const;
bool isexcluded(int index)const;
double getdispersion()const;
void badindex(int index)const;
const char * getname()const;
const char * getxunits()const;
const char * getyunits()const;
unsigned int getmaxindex()const; //get index of the maximum value point in the spectrum
size_t getfirsthigherthen(double)const; //get index of the first pixel with a value higher then x
void smoothgaussian(double sigma); //smooth data by convolution with a gaussian with stdev sigma
double cumsum()const; //get sum of all counts
std::string getfilename()const{return filename;}
double getmax()const;
double getmin()const;
//modifiers
void clear();
void setdatapoint (int index,double en,double cts,double err);
void setnpoints(int);
void seterror (int index,double err);
void setexclude(int index,bool b);
void setexcluderegion(int startindex,int endindex);
void setcounts (int index,double cts);
 void setenergy(int index,double e);
void setpppc(double p);
virtual void seteshift(double e);
void setname(std::string s);
void setxunits(std::string s);
void setyunits(std::string s);
void setfilename(std::string name){filename=name;}
void resetexcluderegion(int startindex,int endindex);
void resetexcluderegion();
void normalize(double s=1.0);
void initpoissonerror();
virtual void display(QWorkspace* parent=0);
void savedat(std::string filename);
Graph* getgraphptr(){return graphptr;}
const Graph* getgraphptr()const{return graphptr;}
//operators
//carefull pass by reference to avoid coying a spectrum
//not only this takes time
//but a spectrum can point to the same graph and this will
//cause trying to delete it more than once
Spectrum& operator+=(const Spectrum& a1);
Spectrum& operator-=(const Spectrum& a1);
Spectrum& operator+=(const double);
Spectrum& operator-=(const double);
Spectrum& operator/=(const Spectrum& a1);
Spectrum& operator/=(const double);
Spectrum& operator*=(const double);
Spectrum& operator*=(const Spectrum& a1);

//Spectrum operator=(const Component a1);//in case of a component, only copy the y-data
Spectrum& operator=(const Spectrum&); //copy constructor, copy everything but leave graphptr=0
Spectrum operator*(double x);
};
#endif
