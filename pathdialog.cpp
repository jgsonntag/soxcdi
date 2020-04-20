// $Id: pathdialog.cpp,v 1.3 2004/08/07 13:38:07 sonntag Exp $
// Pathdialog class implementation
//

#include <qvbox.h>
#include <qpushbutton.h>
#include <qlabel.h>

#include "pathdialog.h"

#include <iostream>
using namespace std;

Pathdialog::Pathdialog(QString logfile,QWidget *parent,const char *name)
	: QDialog(parent,name)
{

  // Set the overall layout
  QVBox *all = new QVBox(this);
  all->setMinimumSize(330,160);
  QString stemp;
  stemp = "The logfile from your previous SoxCDI session was\n";
  stemp.append(logfile);
  stemp.append("\nSelect one of the following:");
  new QLabel(stemp,all);

  // Continue button
  QPushButton *append = new QPushButton("Append to previous logfile",all);
  connect(append,SIGNAL(clicked()),SLOT(slotAppend()));
  QPushButton *owrite = new QPushButton("Start new logfile",all);
  connect(owrite,SIGNAL(clicked()),SLOT(slotOverwrite()));

}


Pathdialog::~Pathdialog()
{
}


void Pathdialog::slotAppend()
{
  accept();
}

void Pathdialog::slotOverwrite()
{
  reject();
}
