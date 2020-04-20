// $Id: showfrom.cpp,v 1.5 2005/07/22 20:38:16 sonntag Exp $
//#include <iostream>
//using namespace std;

#include <qfont.h>

#include "showfrom.h"


Showfrom::Showfrom(Position *cur,QWidget *parent,const char *name )
    : QHBox(parent,name )
{
  QFont labfont("Helvetica",14);
  labfont.setPixelSize(11);
  QFont datfont("Helvetica",14,QFont::Bold);
  datfont.setPixelSize(16);
  
  current = cur;

  QLabel *lab1 = new QLabel("From WP:",this);
  lab1->setFont(labfont);
  fromlab = new QLabel(this);
  fromlab->setFont(datfont);
  fromdlab = new QLabel(this);
  fromdlab->setFont(datfont);

}

Showfrom::~Showfrom()
{
}

void Showfrom::update()
{

  // Waypoint file in use
  if (current->getwpflag())
  {
    fromlab->setText(wplabel[curwp]);
    stemp.sprintf("%.2lf nm",current->getdistfrom());
    fromdlab->setText(stemp);
  }

  // Waypoint file not in use
  else
  {
    fromlab->setText("No waypoints loaded\0");
    fromdlab->setText("\0");
  }


}

