// $Id: position.h,v 1.3 2004/07/31 13:39:42 sonntag Exp $
#ifndef POSITION_H
#define POSITION_H

#include <qwidget.h>
#include <qstring.h>
#include <stdlib.h>

//extern double LON0;
extern QString wplabel[200];
extern double wplat[200];
extern double wplon[200];
extern double wpx[200];
extern double wpy[200];
extern int nwp;
extern int curwp;
extern bool turnisactive;
extern double latcenter;
extern double loncenter;

class Position : public QWidget
{
  Q_OBJECT

  public:
    Position(QWidget *parent=0, const char *name=0);
    ~Position();
    void parseinput(QString input,double,double,double,double,double,double);
    QString gettime();
    double getreversehdgnext();
    double getturnradius();
    double getdeltahdg();
    double getleaddist();
    double getlat();
    double getlon();
    double getht();
    double gethdg();
    double getspd();
    double getroc();
    double gethdgrate();
    double getx();
    double gety();
    double getleader();
    double getxtd(); // returns cross-track dist in feet
    double gettot(); // returns "track-on-track", aka desired track angle
    double getomega(); // returns angle between last point of leg and current position
    double getdistfrom(); // returns dist in nm to "from" waypoint
    double getdistto(); // returns dist in nm to "to" waypoint
    bool getwpflag();
    void setleader(double);
    int getsteermode();
    int getnavtech();

  private:
    QString stemp;
    double time,ltime;
    double lat,llat;
    double lon,llon;
    double ht,lht;
    double hdg,lhdg,hdgrate;
    double spd;
    double roc;
    double utmx;
    double utmy;
    double xtd;
    double turnradius;
    double tleader_sec;
    double distfrom;
    double distto;
    int steermode;
    int navtech; // 0=great circle; 1=rhumb line
    bool wpflag; // true=waypoints in use; false=no waypoints

  public slots:
    void slotsetautoleg();
    void slotsetmanual();
    void slotsetturnin();
    void slotsetgreatcircle();
    void slotsetrhumbline();
    void slotsetwpflag(bool);

};

#endif // POSITION_H
