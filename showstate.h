// $Id: showstate.h,v 1.3 2005/07/22 20:38:16 sonntag Exp $
#ifndef SHOWSTATE_H
#define SHOWSTATE_H

#include <math.h>
#include <qvbox.h>
#include <qlabel.h>
#include <qstring.h>

#include "position.h"

extern QString wplabel[200];
extern double wplat[200];
extern double wplon[200];
extern double wpx[200];
extern double wpy[200];
extern int nwp;
extern int curwp;

class Showstate : public QVBox
{
  Q_OBJECT

  public:
    Showstate(Position *cur,QWidget *parent=0,const char *name=0);
    ~Showstate();

  private:
    QString stemp;
    Position *current;
    QLabel *xtdlab;
    QLabel *signlab;
    QLabel *spdlab;
    QLabel *autoleglabel;
    QLabel *navtechlabel;
    QLabel *ttlab;
    double xtd;
    double dtn;
    double spd;
    double thr;
    double tmin;
    double tsec;

  public slots:
    void update();

};

#endif // SHOWSTATE_H
