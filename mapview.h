// $Id: mapview.h,v 1.7 2007/05/13 21:29:31 sonntag Exp $
#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <vector>
#include <string>

#include <qframe.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qstring.h>
#include <qcolor.h>
#include <qbrush.h>
#include <qstring.h>

#include "coords_ll.h"
#include "wp_ll.h"
#include "position.h"
//#include "soxmap.h"

#define NZOOM 17
#define ACLF 15
#define ACLA 20
#define ACLW 20
#define ACLT 8

extern int PROJ;
extern double LON0;

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

class Mapview : public QFrame
{
  Q_OBJECT

  public:
    Mapview(coords_ll *flight,
            Position *cur,QWidget *parent=0,const char *name=0);
    ~Mapview();
    bool gettrackup();
    int getcurrentzoom();
    void setcurrentzoom(int);
    QColor getbgcolor();
    void setbgcolor(QColor);
    QColor getaccolor();
    void setaccolor(QColor);
    QColor getllcolor();
    void setllcolor(QColor);
    QColor getrrcolor();
    void setrrcolor(QColor);
    QColor getpathcolor();
    void setpathcolor(QColor);
    QColor getdicolor();
    void setdicolor(QColor);
    QColor getwpcolor();
    void setwpcolor(QColor);
    int getpathskip();
	void setlgain(int l);
	void setrgain(int r);
		
  private:
    void paintEvent(QPaintEvent *);
    void drawcoords(coords_ll *,int);
    void drawlines(coords_ll *);
    void get_angle_distortion(coords_ll *);
    void drawwp();
    void drawturn();
    int pathskip;
    QPainter *paint;
    QString label;
    QColor bgcolor;  // Background color
    QColor accolor;  // Aircraft symbol color
    QColor llcolor;  // Leader line color
    QColor rrcolor;  // Rangerings color
    QColor pathcolor;  // Color of the breadcrumb (current flight) path
    QColor dicolor;  // Direction indicator color
    QColor waypointcolor;
    coords_ll *flightpath;
    //vector<WPSTRUCT> *wpts;
    //int *nwp;
    //QString *wplabel;
    coords_ll *leader;
    Position *current;
    bool trackup;
    double ddum;
    double tleader_sec;
    double distort;  // this is the constantly-updated angular distortion
                     // caused by the pseudo-UTM map projection
    double latlead;
    double lonlead;
    double ldrlat;
    double ldrlon;
    double dleader_km;
    double lati,loni,hdgi,ti,newlat,newlon;
    double zoom[NZOOM];
    double spixpnm;  // map scale in pixels per nautical mile
    double spixpm;  // map scale in pixels per meter
    int currentzoom;  // Current zoom level array index
    int acxp;  // Aircraft screen x pos in pixels
    int acyp;  // Aircraft screen y pos in pixels
    int acxf;  // Aircraft nose x pos in pixels
    int acyf;  // Aircraft nose y pos in pixels
    int acxa;  // Aircraft tail x pos in pixels
    int acya;  // Aircraft tail y pos in pixels
    int acxr;  // Aircraft right x pos
    int acyr;  // Aircraft right y pos
    int acxl;  // Aircraft left x pos
    int acyl;  // Aircraft left y pos
    int atxr;  // Aircraft right tail x pos
    int atyr;  // Aircraft right tail y pos
    int atxl;  // Aircraft left tail x pos;
    int atyl;  // Aircraft left tail y pos;
    double theta;
    int xneedle;
    QString stemp;
	int lgain;
	int rgain;
  public slots:
    void updatemap();
    void slotzoomin();
    void slotzoomout();
    void slotsettrackup();
    void slotsetnorthup();
    void slotselectbgcolor();
    void slotselectaccolor();
    void slotselectllcolor();
    void slotselectrrcolor();
    void slotselectpathcolor();
    void slotselectdicolor();
    void slotselectwaypointcolor();
    void slotsetpathskip(int);
    void slotPrevWP();
    void slotNextWP();

};

#endif // MAPVIEW_H
