/***************************************************************************
 *   Copyright (C) 2004 by rob                                             *
 *   rob@localhost                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "misc.h"
/*
 * delay(us)  wait us microsecs using select. Effectively
 *            causes current process to stop and a reschedule take place
 *            as well as doing the delay.
 */

void delay(int us)
{
        struct timeval tv;

        tv.tv_sec = 0;
        tv.tv_usec = us;
        (void)select( 1, (fd_set *)0, (fd_set *)0, (fd_set *)0, &tv );
}

/***ERR_RPT***/
/* err_rpt(err,msg)
 */

int err_rpt(short int err,char *msg)
 { 
   char *sys;

   if (err>sys_nerr)
     { fprintf(stderr,"%s: Err %d :-  %s\n",sys,err,msg);
     }
   else if (err) fprintf(stderr,"[%s] %s : %s\n",sys,sys_errlist[err],msg);
   return(err);
 }

/***HCF***/
/* hcf(x,y)   find highest common factor of x and y
 */

unsigned hcf(unsigned x,unsigned y)
{
   register unsigned a,b,r;
   
   if (x>y) {
      a=x; b=y; 
   } else {
      a=y; b=x; 
   }
   for ( ; r=a%b ; a=b, b=r) { }
   return(b);
}

/***PARSE***/
/* parse(s,aa,sep) splits s up into parameters and stores ptrs
 *                   to each prm in ptr array aa (upto MAX_ARGS)
 *                   params are space or tab or 'sep' seperated, and may be
 *                   enclosed in single or double quotes e.g. 'this is 1 prm'
 *
 *   returns number of parameters parsed
 */

int parse(char *s,char **aa,char sep)
{ 
  char *p,*q,c;
  int i;

#define EOL ((*s=='\0') || (*s=='\n'))
#define EOP ((*s==' ') || (*s=='\t') || (*s==sep))
#define QUOTE ((*s=='\'') || (*s=='"'))
#define MAX_ARGS 50

   for ( i=1; ; i++) { 
      for ( ; EOP; s++) {}    /* skip leading  separators */
      *aa++=p=s;
      if (EOL) return((--i)?i:1);
      if (i==MAX_ARGS) {
	 return(--i);
      }
      while (!(EOL || EOP)) {
	 if (QUOTE) {
            for (s++; !(EOL || QUOTE); ) *p++=*s++;
            if (QUOTE) s++;
	 } else *p++=*s++;
      }
      if (EOL) {*p='\0'; return(i);}
      *p='\0';
      ++s;
   }
}

#undef EOL
#undef EOP

/***mstosmaples***/
/*
 * mstosamples(ms,sr)  convert ms millisecs into number of samples
 *                     given that the sample rate is sr samples/sec
 */

int mstosamples(int ms,int sr)
{
  int d;
  int m;

  m=INT_MAX/(4*sr);  /* we need to make sure we don't overflow INT size
			including when no. of samples may get upped for stereo
			16 bit into a byte count for getting buffers etc
 		     */
  for ( d=1000; d>0 && ms>m; d/=10 ) { ms/=10; }
  if (d)
    return((sr*ms+d/2)/d);
  else 
    return(0);
}

