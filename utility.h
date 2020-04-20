// $Id: utility.h,v 1.3 2004/07/31 13:39:42 sonntag Exp $
#include <sys/time.h>
#include <math.h>

#define PI (4.0*atan((double)(1.0)))
#define DEG2RAD (PI/180.0)
#define RAD2NM (180.0*60.0/PI)
#define RAD2KM (RAD2NM*6076.1*12.0*2.54/100.0/1000.0)
#define M2FT (100.0/2.54/12.0)
#define NM2KM 1.852
#define AE 6378137.0
#define FLAT (1.0/298.257223563)

#define MAXBANK 6.0

extern double LON0;

double timediff(timeval, timeval);
double gcdist(double lat1,double lon1,double lat2,double lon2);
double gccourse1(double lat1,double lon1,double lat2,double lon2);
double gcproject(double lat1,double lon1,double tc1,double dist,
                 double *lat2,double *lon2);
int geod2utm(double lat,double lon,double *y,double *x,int *zone);
int geod2ps(double lat,double lon,double latcd,double lon0d,double k0,
            double ae,double flat,double *x,double *y);
void projmercator(double, double, double *, double *);
void inversemercator(double, double, double *, double *);
