// $Id: wp_ll.h,v 1.1 2005/07/22 20:38:16 sonntag Exp $
#ifndef WP_LL_H
#define WP_LL_H

extern int PROJ;
extern double LON0;

//
//  Definition of wp_ll_element class
//

#define PI (4.0*atan((double)(1.0)))
#define AE 6378137.0
#define FLAT (1.0/298.257223563)

#include <iostream>
using namespace std;

#include <qstring.h>
#include <math.h>

class wp_ll_element
{

  friend class wp_ll;

  public:
    wp_ll_element(void);
    ~wp_ll_element(void);

  private:
    QString name;
    double x;
    double y;
    double spd;
    wp_ll_element *next_ptr;

};


//
//  Definition of wp_ll class
//

class wp_ll
{

  public:
    wp_ll(void);
    ~wp_ll(void);
    void empty(); // empty the list and reset pointers to 0
    void add_element(char *, double, double, double, double *, double *);  // add a new element
    void go_to_top(void);  //  Sets current pointer to top of list
    char get_next_element(QString *,double *, double *, double *);  // gets next element
                                                // returns 0 if at end-of-list
                                                // returns 1 otherwise

  private:
    wp_ll_element *first_ptr;
    wp_ll_element *current_ptr;
    wp_ll_element xy;
    bool firstcoords;
    int geod2utm(double lat,double lon,double *y,double *x,int *zone);
    int geod2ps(double lat,double lon,double latcd,double lon0d,double k0,
                double ae,double flat,double *x,double *y);
    double k0,FN,FE,n,e2,ep2,ep4,ep6,ep8,n2,n3,n4,n5; // UTM constants
    double a_prime,b_prime,c_prime,d_prime,e_prime;
    double lon_deg,lon0_deg,lon0;

};

#endif // WP_LL_H
