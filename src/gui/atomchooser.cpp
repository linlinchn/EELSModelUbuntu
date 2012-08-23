/***************************************************************************
                          atomchooser.cpp  -  description
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

#include "src/gui/atomchooser.h"
#include <qinputdialog.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qcombobox.h>
#include <qradiobutton.h>
#include <qlineedit.h>
#include <qvalidator.h>
#include <qlabel.h>
#include <qstring.h>
#include <QWorkspace>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <qpushbutton.h>
#include <iostream>
#include <sstream>
#include "src/gui/mendeleev.h" //defines table of mendeleev
#include "src/gui/energies.h"

Atomchooser::Atomchooser(QWorkspace *parent, const char *name,double Estart,double Estop, double E0, double beta, double alpha, double resolution,bool dofinestructure ) : QDialog(parent) {

 parent->addWindow(this); //add it explicitly to the workspace

this->setWindowTitle("Atom chooser");

//a list of interpolation types

//draw a table of Mendeleev


QGridLayout* lay = new QGridLayout(this);


size_t id=0;
buttonlist.clear();
for (size_t i=0;i<9;i++){
    for (size_t j=0;j<18;j++){

        //first line
        if ((i==0)&(j==1)){
            j=17;
        }
        //2nd
        if ((i==1)&(j==2)){
            j=12;
        }
        //3rd
        if ((i==2)&(j==2)){
            j=12;
        }
         //7th
        if ((i==6)&(j==7)){
            j=4;
            i=7;
        }
         if ((i==8)&(j==0)){
            j=4;
        }
        QPushButton *thisbutton = new QPushButton(mendeleev[id].c_str());
        buttonlist.push_back(thisbutton);
        thisbutton->setCheckable (true);
        thisbutton->setFixedSize(30,30);

        lay->addWidget(thisbutton,i,j);


        thisbutton->setEnabled(false);
        //check if we have a tabulated energy for each
        if (id<size_K){
                if ((energy_K[id]>Estart)&(energy_K[id]<Estop)){
                     thisbutton->setEnabled(true);
                }
        }
        if (id<size_L23){
                if ((energy_L23[id]>Estart)&(energy_L23[id]<Estop)){
                     thisbutton->setEnabled(true);
                }
        }
        id+=1;

        //connect to do something if clicked

    }
}

//convergence angle
valstring="%1"; //required by MacOSX otherwise empty text window
int fieldwidth=-1;
    char format='f';
    int precision=2;
    double_validator=new QDoubleValidator(this);
    alphaedit=new QLineEdit(valstring.arg (alpha/1.0e-3,fieldwidth,format,precision ));
    QLabel* alphalabel=new QLabel("alpha [mrad]");
    alphaedit->setValidator (double_validator);  //make sure only doubles are entered

    lay->addWidget(alphalabel,9,0,1,3);
    lay->addWidget(alphaedit,10,0,1,3);

//collection angle
    betaedit=new QLineEdit(valstring.arg (beta/1.0e-3,fieldwidth,format,precision ));
    QLabel* betalabel=new QLabel("beta [mrad]");
    betaedit->setValidator (double_validator);  //make sure only doubles are entered

    lay->addWidget(betalabel,9,3,1,3);
    lay->addWidget(betaedit,10,3,1,3);

//HT

    HTedit=new QLineEdit(valstring.arg (E0/1.0e3,fieldwidth,format,precision ));
    QLabel* HTlabel=new QLabel("E0 [kV]");
    HTedit->setValidator (double_validator);  //make sure only doubles are entered
    lay->addWidget(HTlabel,9,6,1,3);
    lay->addWidget(HTedit,10,6,1,3);

//energy resolution
 Resedit=new QLineEdit(valstring.arg (resolution,fieldwidth,format,precision ));
    QLabel* Reslabel=new QLabel("resolution [eV]");
    Resedit->setValidator (double_validator);  //make sure only doubles are entered
    lay->addWidget(Reslabel,9,9,1,3);
    lay->addWidget(Resedit,10,9,1,3);


//a radio button to swith fine structure
finestructbutton =new QRadioButton ("Add fine structure",this);
lay->addWidget(finestructbutton,10,12,1,3);
finestructbutton->setChecked(dofinestructure);



//OK and cancel buttons
//OK button

     QPushButton *OK = new QPushButton( "&Make cross sections");
     QPushButton *Cancel = new QPushButton( "&Cancel");
     lay->addWidget(OK,11,0,1,9);
     lay->addWidget(Cancel,11,9,1,9);
     connect( OK, SIGNAL( clicked()), this, SLOT( slot_OK() ) );
     connect( Cancel, SIGNAL( clicked()), this, SLOT( slot_Cancel() ) );

this->show();
 this->setMinimumSize(this->minimumSizeHint()); //needed since QT4 to make sure the widget is not shown minimized
}
Atomchooser::~Atomchooser(){
}


void Atomchooser::slot_OK(){
    //gather results when OK
    emit accept();
}
void Atomchooser::slot_Cancel(){
    //gather results when OK
    emit reject();
}

double Atomchooser::getalpha(){
    //get alpha in rad
    return getnumber(alphaedit->text())*1.0e-3;
}
double Atomchooser::getbeta(){
    //get Beta in rad
    return getnumber(betaedit->text())*1.0e-3;
}
double Atomchooser::getE0(){
    //get HT in eV
    return getnumber(HTedit->text())*1.0e3;
}
double Atomchooser::getresolution(){
    //get HT in eV
    return getnumber(Resedit->text());
}
bool Atomchooser::getdofine(){
    //do fine structure?
    return finestructbutton->isChecked();;
}
double Atomchooser::getnumber(const QString& valstring)const{
  bool ok;
  double result=valstring.toDouble(&ok);
  if (ok) return result;
  else return 0.0;
}

void Atomchooser::getZ(std::vector<size_t>* Zlist){
    //return vector with atom numbers
    Zlist->clear();
    //loop over the buttons and return the ones that are checked
    for (size_t i=0;i<buttonlist.size();i++){
        if (buttonlist[i]->isChecked()){
                Zlist->push_back(i+1); //the index starts from zero while atom numbers start from 1
            }
        }
}

void Atomchooser::getedges(std::vector<double>* Elist,int type){
    //return edge onsets for edges of type (0=K,1=L) of selected atoms, if we don't know the edge onset or if it is outside the range, we have 0
    Elist->clear();
    //loop over the buttons and return the ones that are checked
    for (size_t i=0;i<buttonlist.size();i++){
        if (buttonlist[i]->isChecked()){
            double energy=0.0;
            switch(type){
                case 0:
                    if (i<size_K){
                        energy=energy_K[i];
                    }
                    break;
                case 1:
                    if (i<size_L23){
                        energy=energy_L23[i];
                    }
            }
            Elist->push_back(energy);
        }

    }
}

void Atomchooser::getnamed(std::vector<std::string>* Namelist,int type){
    //return a list with names of the selected atoms
    Namelist->clear();
    //loop over the buttons and return the ones that are checked
    for (size_t i=0;i<buttonlist.size();i++){
        if (buttonlist[i]->isChecked()){
            std::string name;
            std::ostringstream s;
            switch(type){
                case 0:
                    if ((s << mendeleev[i]<<" K")){
                        // conversion worked
                        name=s.str();
                    }
                    break;
                case 1:
                    if ((s << mendeleev[i]<<" L23")){
                        // conversion worked
                        name=s.str();
                    }

            }
            Namelist->push_back(name);
        }
    }
}








