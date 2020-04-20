// $Id: showfrom.h,v 1.3 2005/07/22 20:38:16 sonntag Exp $
#ifndef SHOWFROM_H
#define SHOWFROM_H

#include <math.h>
#include <qhbox.h>
#include <qlabel.h>

#include "position.h"

extern QString wplabel[200];
extern double wplat[200];
extern double wplon[200];
extern double wpx[200];
extern double wpy[200];
extern int nwp;
extern int curwp;

class Showfrom : public QHBox
{
  Q_OBJECT

  public:
    Showfrom(Position *cur,QWidget *parent=0,const char *name=0);
    ~Showfrom();

  private:
    QString stemp;
    Position *current;
    QLabel *fromlab;
    QLabel *fromdlab;

  public slots:
    void update();

};

#endif // SHOWFROM_H
