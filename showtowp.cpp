// $Id: showtowp.cpp,v 1.5 2005/07/22 20:38:16 sonntag Exp $
//#include <iostream>
//using namespace std;

#include <qfont.h>

#include "showtowp.h"


Showtowp::Showtowp(Position *cur,QWidget *parent,const char *name )
    : QHBox(parent,name )
{
  QFont labfont("Helvetica",14);
  labfont.setPixelSize(11);
  QFont datfont("Helvetica",14,QFont::Bold);
  datfont.setPixelSize(16);
  
  current = cur;

  QLabel *lab1 = new QLabel("To WP:",this);
  lab1->setFont(labfont);
  towplab = new QLabel(this);
  towplab->setFont(datfont);
  towpdlab = new QLabel(this);
  towpdlab->setFont(datfont);

}

Showtowp::~Showtowp()
{
}

void Showtowp::update()
{

  // Waypoint file in use
  if (current->getwpflag())
  {
    towplab->setText(wplabel[curwp+1]);
    stemp.sprintf("%.2lf nm",current->getdistto());
    towpdlab->setText(stemp);
  }
  
  // Waypoint file not in use
  else
  {
    towplab->setText("No waypoints loaded\0");
    towpdlab->setText("\0");
  }

}

