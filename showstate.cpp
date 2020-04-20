// $Id: showstate.cpp,v 1.5 2005/07/22 20:38:16 sonntag Exp $
//#include <iostream>
//using namespace std;

#include <qfont.h>

#include "showstate.h"


Showstate::Showstate(Position *cur,QWidget *parent,const char *name )
    : QVBox(parent,name )
{
  QFont labfont("Helvetica",14);
  labfont.setPixelSize(11);
  QFont datfont("Helvetica",14,QFont::Bold);
  datfont.setPixelSize(16);
  QFont bigfont("Helvetica",14,QFont::Bold);
  bigfont.setPixelSize(24);
  
  current = cur;

  QLabel *lab1 = new QLabel("Desired track is:",this);
  lab1->setFont(labfont);
  xtdlab = new QLabel(this);
  xtdlab->setFont(bigfont);
  signlab = new QLabel(this);
  signlab->setFont(bigfont);
  //QLabel *lab2 = new QLabel("of current position",this);
  //lab2->setFont(labfont);

  QLabel *lab4 = new QLabel("\nWaypoint mode:",this);
  lab4->setFont(labfont);
  autoleglabel = new QLabel(this);
  autoleglabel->setFont(datfont);

  QLabel *lab6 = new QLabel("\nNav technique:",this);
  lab6->setFont(labfont);
  navtechlabel = new QLabel(this);
  navtechlabel->setFont(datfont);

  QLabel *lab3 = new QLabel("\nGroundspeed:",this);
  lab3->setFont(labfont);
  spdlab = new QLabel(this);
  spdlab->setFont(datfont);

  QLabel *lab5 = new QLabel("\nTime to next:",this);
  lab5->setFont(labfont);
  ttlab = new QLabel(this);
  ttlab->setFont(datfont);

}

Showstate::~Showstate()
{
}

void Showstate::update()
{

  // Update the cross-track info
  if (current->getwpflag())
  {
    xtd = current->getxtd();
    if (fabs(xtd)>6076.1)
    {
      stemp.sprintf("%.1lf nm",fabs(xtd/6076.1));
      xtdlab->setText(stemp);
    }
    else
    {
      stemp.sprintf("%.0lf ft",fabs(xtd));
      xtdlab->setText(stemp);
    }
    if (xtd<0.0) signlab->setText("RIGHT");
    else signlab->setText("LEFT");
  }
  else
  {
    xtdlab->setText("n/a\0");
    signlab->setText("\0");
  }

  // Update the autoleg indicator
  if (current->getsteermode()==1)
    stemp = "Auto-leg";
  else if (current->getsteermode()==2)
    stemp = "Turn-in";
  else
    stemp = "Manual";
  autoleglabel->setText(stemp);

  // Update the navigation technique indicator
  if (current->getnavtech()==0)
    stemp = "Great circle";
  else
    stemp = "Rhumb line";
  navtechlabel->setText(stemp);

  // Update the groundspeed
  stemp.sprintf("%.0lf knots",current->getspd());
  spdlab->setText(stemp);

  // Update the time to next
  if (current->getwpflag())
  {
    dtn = current->getdistto();
    spd = current->getspd();
    thr = dtn/spd;
    tmin = (thr-int(thr))*60.0;
    tsec = (tmin-int(tmin))*60.0;
    stemp.sprintf("%02d:%02d:%02d",(int)(thr),(int)(tmin),(int)(tsec));
    ttlab->setText(stemp);
  }
  else
  {
    ttlab->setText("n/a\0");
  }

}

