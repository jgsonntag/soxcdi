// $Id: soxgui.h,v 1.8 2007/05/13 21:29:31 sonntag Exp $
// Soxgui class definition
//

#ifndef SOXGUI_H
#define SOXGUI_H

//#include <vector>
//#include <string>

#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qwidget.h>
#include <qstring.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qsettings.h>
#include <qspinbox.h>
#include <qtimer.h>

#include "genericclient.h"
#include "coords_ll.h"
#include "wp_ll.h"
#include "mapview.h"
#include "position.h"
#include "showfrom.h"
#include "showtowp.h"
#include "showstate.h"
//#include "soxmap.h"
#include "signalgenerator.h"

#define FT2M (12.0*2.54/100.0)



class Soxgui : public QWidget
{

  Q_OBJECT

  public:
    Soxgui();  // Constructor
    ~Soxgui();  // Destructor

  private:
    void parseGGA(QString,double *,double *,double *,double *);
    QColor readcolor(QString,QString);
		void writecolor(QString,QColor);
    QSettings usettings;
    QMenuBar *mainmenu;
    QPopupMenu *filemenu;
    QPopupMenu *navmenu;
    GenericClient *client;
    QString stemp,stemp1,stemp2;
    QString gpshost;
    Q_UINT16 gpsport;
    QString gpscmd;
    coords_ll *flight;
    wp_ll *waypoint;
    //vector<WPSTRUCT> wpts;
    //QString wplabel[200];
    //double wplat[200];
    //double wplon[200];
    //double wpx[200];
    //double wpy[200];
    //int nwp;
    //bool wpflag;
    //bool alon;
    Mapview *map;
    QPushButton *zoomin;
    QPushButton *zoomout;
    QPushButton *prevwp;
    QPushButton *nextwp;
    QPushButton *northup;
    QPushButton *trackup;
    Position *current;
    Showfrom *from;
    Showtowp *towp;
    Showstate *state;
    QString logfile;
    QString newlogfile;
    QString logpath;
    QFile *pathfile;
    QTextStream *pathstream;
    QString wpname;
    QFile *wpfile;
    double swathwidth;  // swath width on the ground in meters
    double sec,latm,lonm,t,lat,lon,ht;
    int hr,min,latd,lond,idwp;
    void ingest_latlon(QFile *, coords_ll *);
    void ingest_waypoint(QFile *, wp_ll *);
		
	signalgenerator gen;
	int setupOnce;
    QTimer cdiTimer;	
	int cdigain;
	int odist;
	int cdileftgain;
	int cdirightgain;
	int granularity;
	double sigma;
	double mu;
	int signalmodel;	
	int idDist;
	int idSigma;
	int idMu;
	int idGran;
    int idCDI;
  private slots:
    void about();
    void slotFirstGPS(QString);
    void slotNewGPS(QString);
    void slotGPSClosed();
    void slotGPSError(int);
    void slotWPFile();
    void slotSetSwathWidth();
    void slotSetLeader();
	void slotCdiDist();
	void slotEnableCdi();
    void slotDisableCdi();
    void slotCdiGain();
    void slotCdiLeftGain();
    void slotCdiRightGain();
    void cdiTimerDone();
	void slotCdiGranularity();
	void slotLinearModel();
	void slotGaussianModel();
	void slotCdiSigma();
	void slotCdiMu();

};

#endif // SOXGUI_H
