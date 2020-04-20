// $Id: utility.cpp,v 1.3 2004/07/31 13:39:42 sonntag Exp $
#include "utility.h"

#include <iostream>
using namespace std;


double timediff(timeval t1, timeval t2)
{
  double secs,usecs;

  secs = (double)(t1.tv_sec - t2.tv_sec);
  usecs = (double)(t1.tv_usec - t2.tv_usec);
  return(secs+usecs/1.0e6);
}


double gcdist(double lat1,double lon1,double lat2,double lon2)
{
  double dist;

  lon1 = -lon1;
  lon2 = -lon2;
  dist = acos(sin(lat1)*sin(lat2)+cos(lat1)*cos(lat2)*cos(lon1-lon2));
  return(dist);

}


double gccourse1(double lat1,double lon1,double lat2,
                 double lon2)
{
  double dist,tc1;

  /*  Change longitude sign convention  */
  if (lon1<0.0)
    lon1 += 2.0*PI;
  if (lon2<0.0)
    lon2 += 2.0*PI;
  lon1 = -lon1;
  lon2 = -lon2;

  /*  Check for pole  */
  if (cos(lat1) < 0.00001)
  {
    if (lat1 > 0)
      tc1 = PI;
    else
      tc1 = 0;
  }

  /*  Check for meridian  */
  //else if (lon1 == lon2)
  else if (fabs(lon1-lon2)<(0.00001*DEG2RAD))
  {
    if (lat1>lat2)
    {
      tc1 = PI;
    }
    else
    {
      tc1 = 0.0;
    }
  }

  /*  Point 1 not a pole  */
  else
  {
    dist = acos(sin(lat1)*sin(lat2)+cos(lat1)*cos(lat2)*cos(lon1-lon2));
    tc1 = acos((sin(lat2)-sin(lat1)*cos(dist))/(sin(dist)*cos(lat1)));
    if (sin(lon2-lon1)>=0)
      tc1 = 2*PI-tc1;
  }

  /*  Return the result  */
  return(tc1);

}


double gcproject(double lat1,double lon1,double tc1,double dist,
                 double *lat2,double *lon2)
{
  double templon;

  // Compute point 2
  lon1 = -lon1;
  dist /= RAD2KM;
  *lat2 = asin(sin(lat1)*cos(dist)+cos(lat1)*sin(dist)*cos(tc1));
  templon = atan2(sin(tc1)*sin(dist)*cos(lat1),cos(dist)-sin(lat1)*sin(*lat2));
  /* lonb = (lona-templon+PI)%(2.0*PI) - PI;  */
  *lon2 = -(lon1 - templon);

}


int geod2ps(double latd,double lond,double latcd,double lon0d,double k0,
                       double ae,double flat,double *x,double *y)
{
  int south;
  double lat,lon,latc,lon0,e,t,tc,mc,rho;
  double dtemp1,dtemp2,dtemp3;

  // Convert to west longitude and radians
  if (lond>180.0) lond-=360.0;
  if (lon0d>180.0) lon0d-=360.0;
  lat  = latd*PI/180.0;
  lon  = lond*PI/180.0;
  latc = latcd*PI/180.0;
  lon0 = lon0d*PI/180.0;
  
  // Check for south polar aspect
  south = 0;
  if (latc < 0.0)
  {
    lat  *= -1.0;
    lon  *= -1.0;
    latc *= -1.0;
    lon0 *= -1.0;
    south = 1;
  }
  //printf("%lf %lf %lf %lf %d\n",lat,lon,latc,lon0,south);
  //printf("ae %lf  flat %lf\n",ae,flat);
      
  // Compute the ellipsoid's eccentricity
  e = sqrt(1.0 - (1.0-flat)*(1.0-flat));

  //  Compute t
  dtemp1 = (1.0-sin(lat))/(1.0+sin(lat));
  dtemp2 = pow(((1.0+e*sin(lat))/(1.0-e*sin(lat))),e);
  t = sqrt(dtemp1*dtemp2);

  // Compute rho
  if (latcd==90.0||latcd==-90.0)
  {
    dtemp1 = pow((1.0+e),(1.0+e));
    dtemp2 = pow((1.0-e),(1.0-e));
    dtemp3 = sqrt(dtemp1*dtemp2);
    rho = 2.0*ae*k0*t/dtemp3;
  }
  else
  {
    dtemp1 = (1.0-sin(latc))/(1.0+sin(latc));
    dtemp2 = pow(((1.0+e*sin(latc))/(1.0-e*sin(latc))),e);
    tc = sqrt(dtemp1*dtemp2);
    dtemp1 = sqrt(1.0-e*e*sin(latc)*sin(latc));
    mc = cos(latc)/dtemp1;
    rho = ae*mc*t/tc;
  }
    
  // Compute x and y
  *x = rho*sin(lon-lon0);
  *y = -rho*cos(lon-lon0);
  
  // Check for south polar aspect
  if (south)
  {
    *x *= -1.0;
    *y *= -1.0;
  }

  // Return
  return(0);

}


int geod2utm(double lat,double lon,double *y,double *x,int *zone)
{
  double S,nu,k0,FN,FE,ep2,ep4,ep6,ep8,n,n2,n3,n4,n5,e2,lon0;
  double a_prime,b_prime,c_prime,d_prime,e_prime;
  double dlon,dlon2,dlon3,dlon4,dlon5,dlon6,dlon7,dlon8;
  double sinlat,coslat,cos2lat,cos3lat,cos4lat,cos5lat,cos6lat,cos7lat,cos8lat;
  double tanlat,tan2lat,tan4lat,tan6lat,tan8lat;
  double T1,T2,T3,T4,T5,T6,T7,T8,T9;

  //cout << "LON0=" << LON0 << '\n';

  // Convert input lat/lon to radians
  lat *= PI/180.0;
  lon *= PI/180.0;

   /*  Check input lat and lon  */
  if ( lon < 0.0 )
    lon += 2.0*PI;
  if ( lon < 0.0 || lon > 2.0*PI )
  {
    *zone = -99;
    *x = -999.99;
    *y = -999.99;
    return(-1);
  }
  if ( lat < -PI/2.0 || lat > PI/2.0 )
  {
    *zone = -99;
    *x = -999.99;
    *y = -999.99;
    return(-1);
  }

  // Compute and store one-time constants
  //if (firstcoords)
  //{

    /*  Define constants  */
    // For this application we don't use false northing/eastings
    // since they confuse things like the equator crossing
    k0 = 0.9996;
    //if ( lat >= 0.0 )
    //  FN = 0.0;
    //else
    //  FN = 10000000.0;
    //FE = 500000.0;
    FN = 0.0;
    FE = 0.0;

    /*  Compute primed quantities  */
    n = FLAT/(2.0-FLAT);
    e2 = FLAT*(2.0-FLAT);
    ep2 = e2/(1.0-FLAT)/(1.0-FLAT);
    ep4 = ep2*ep2;
    ep6 = ep4*ep2;
    ep8 = ep6*ep2;
    n2 = n*n;
    n3 = n2*n;
    n4 = n3*n;
    n5 = n4*n;
    a_prime = AE*(1.0 - n + 5.0/4.0*(n2-n3) + 81.0/64.0*(n4-n5));
    b_prime = 3.0/2.0*AE*(n - n2 + 7.0/8.0*(n3-n4) + 55.0/64.0*n5);
    c_prime = 15.0/16.0*AE*(n2 - n3 + 3.0/4.0*(n4-n5));
    d_prime = 35.0/48.0*AE*(n3 - n4 + 11.0/16.0*n5);
    e_prime = 315.0/512.0*AE*(n4 - n5);

    /*  Compute zone number and central meridian  */
    //lon_deg = lon*180.0/PI;
    //lon0_deg = 6.0*(int)(1+lon_deg/6.0) - 3.0;
    //lon0 = lon0_deg*PI/180.0;

    //firstcoords = false;
  //}

  // Set the central meridian
  lon0 = LON0*PI/180.0;

  /*  Compute meriodonal arc  */
  nu = AE/sqrt(1.0 - e2*sin(lat)*sin(lat));
  S = a_prime*lat - b_prime*sin(2.0*lat) + c_prime*sin(4.0*lat)
      - d_prime*sin(6.0*lat) + e_prime*sin(8.0*lat);

  // Compute the delta-lons
  //*zone = (int)(1+lon_deg/6.0) + 30;
  //if (*zone > 60 )
  //  *zone -= 60;
  dlon = lon - lon0;
  dlon2 = dlon*dlon;
  dlon3 = dlon2*dlon;
  dlon4 = dlon2*dlon2;
  dlon5 = dlon4*dlon;
  dlon6 = dlon4*dlon2;
  dlon7 = dlon6*dlon;
  dlon8 = dlon6*dlon2;

  /*  Compute the Ts  */
  sinlat = sin(lat);
  coslat = cos(lat);
  cos2lat = coslat*coslat;
  cos3lat = cos2lat*coslat;
  cos4lat = cos2lat*cos2lat;
  cos5lat = cos4lat*coslat;
  cos6lat = cos4lat*cos2lat;
  cos7lat = cos6lat*coslat;
  cos8lat = cos6lat*cos2lat;
  tanlat = tan(lat);
  tan2lat = tanlat*tanlat;
  tan4lat = tan2lat*tan2lat;
  tan6lat = tan4lat*tan2lat;
  tan8lat = tan6lat*tan2lat;
  T1 = S*k0;
  T2 = nu*sinlat*coslat*k0/2.0;
  T3 = nu*sinlat*cos3lat*k0/24*(5.0 - tan2lat + 9.0*ep2*cos2lat
       + 4.0*ep4*cos4lat);
  T4 = nu*sinlat*cos5lat*k0/720.0*(61.0 - 58.0*tan2lat + tan4lat
       + 270.0*ep2*cos2lat - 330.0*tan2lat*ep2*cos2lat + 445.0*ep4*cos4lat
       + 324*ep6*cos6lat - 680.0*tan2lat*ep4*cos4lat + 88.0*ep8*cos8lat
       - 600.0*tan2lat*ep6*cos6lat - 192.0*tan2lat*ep8*cos8lat);
  T5 = nu*sinlat*cos7lat*k0/40320.0*(1385.0 - 3111.0*tan2lat - 543.0*tan4lat
       - tan6lat);
  T6 = nu*coslat*k0;
  T7 = nu*cos3lat*k0/6.0*(1.0 - tan2lat + ep2*cos2lat);
  T8 = nu*cos5lat*k0/120.0*(5.0 - 18.0*tan2lat + tan4lat + 14.0*ep2*cos2lat
       - 58.0*tan2lat*ep2*cos2lat + 13.0*ep4*cos4lat + 4.0*ep6*cos6lat
       - 64.0*tan2lat*ep4*cos4lat - 24.0*tan2lat*ep6*cos6lat);
  T9 = nu*cos7lat*k0/5040.0*(61.0 - 479.0*tan2lat + 179.0*tan4lat
       - tan6lat);

  /*  Compute northing and easting  */
  *y = FN + T1 + dlon2*T2 + dlon4*T3 + dlon6*T4 + dlon8*T5;
  *x  = FE + dlon*T6 + dlon3*T7 + dlon5*T8 + dlon7*T9;
  return(0);

  //*y = lat;
  //*x = lon;
  //*zone = 99;
  //return(0);
}


void inversemercator(double x,double y,double *lat,double *lon)
{
  double dtemp;

  dtemp = exp(-y/AE);
  *lat = PI/2.0-2.0*atan(dtemp);
  *lon = x/AE;

}


void projmercator(double lat,double lon,double *x, double *y)
{
  *x = AE*lon;
  *y = AE*log(tan(PI/4.0+lat/2.0));
}


