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
#include "generator.h"
static char *WavNAMES[]= {"sine","cosine","square","triangle","sawtooth","pulse","noise", "off", NULL};
static int WavDefr[]={ 0,0,0,0,0,10,0,0,0 };
generator::generator()
{

}


generator::~generator()
{
}


 /* mk8bit(bf,ip,N)  convert N 16 bit samples from *ip to 8 bit samples
 *                  in bf
 */

int generator::mk8bit(unsigned char *bf,short int *ip,int N)
{
   unsigned char *p;
   
   for (p=bf; N--; ) {
      *p++=(unsigned char)(((*ip++)>>8)+128);
   }
   return(0);
}

/* mixplaybuf(a,b,c,N,afmt)   mix N samples from buffers b and c into 
 *     sound play buffer a
 *     afmt defines format of samples
 */

void generator::mixplaybuf(unsigned char *a,unsigned char *b,unsigned char *c,int N,int afmt)
{
   int i,word;

   word=(afmt==AFMT_S16_LE);
   for (i=0; i<N; i++) {
      *a++=*b++; if (word) *a++=*b++;
      *a++=*c++; if (word) *a++=*c++;
   }
}

/* chanmix(a,b,Nb,c,Nc,N,afmt)  digitally mix N samples from buffers b and c 
 *            into buffer a.  Nb and Nc are the relative amplitudes of
 *            the two input channels as percentages of fullscale.
 *            afmt defines format of samples
 */

void generator::chanmix(unsigned char *a,unsigned char *b,int Nb,unsigned char *c,int Nc,int N,int afmt)
{
   int i,u,v,w;
   short int *p,*q,*r;

   Nc+=Nb; Nb=Nc>>1;
   if (afmt==AFMT_S16_LE) {   /* 16 bit mixing...... */
      p=(short int *)b; q=(short int *)c; r=(short int *)a;
      for (i=0; i<N; i++) {
	 u=((*p++)+(*q++))*100;
	 *r++=(short int)((u+((u<0)?-Nb:Nb))/Nc);
      }
   } else {                   /* 8 bit mixing....... */
      for (i=0; i<N; i++) {
	 u=((int)(*b++) + (int)(*c++) - 256)*100;
	 *a++=(unsigned char)((u+((u<0)?-Nb:Nb))/Nc + 128);
      }
   }
}

/* do_antiphase(a,b,N,afmt)   create an antiphase version of sample b in
 *                            a. afmt defines 8/16bit, N is number of samples
 */
void generator::do_antiphase(unsigned char *a,unsigned char *b,int N,int afmt)
{
   int i,word;

   if (afmt==AFMT_S16_LE) {
      short int *p=(short int *)a;
      short int *q=(short int *)b;
      for (i=0; i<N; i++) {
	 *p++=-*q++;
      }
   }
   else {
      for (i=0; i<N; i++) {
	 *a++=(unsigned char)(256-(int)(*b++));
      }
   }
}

/* maketone(wf,fr,N,G,afmt)   generate a tone of frequency fr, samplerate N/sec,
 *   waveform wf, and digital Gain G, does a malloc to get a new tone buffer.
 */
unsigned char* generator::maketone(char *wf,int fr,int N,int G,int afmt)
{
   unsigned char *bf;
   int n,bn;
   
   bn=N<<(afmt==AFMT_S16_LE);
   if ((bf=(unsigned char *)malloc(bn))==NULL) return(NULL);
   n=generate((char*)((fr)?wf:"OFF"),(char*)bf,bn,fr,G,N,-1,afmt);
   return((n)?bf:NULL);
}

/* mksweep(bf,bfn,FMbuf,wavbuf,Fmin,Fmax,N,afmt)
 *    create a swept waveform in bf, size bfn, of format afmt. 
 *    Sweeping waveform is in FMbuf - THIS is 16 bit samples!!!!
 *    Swept waveform is in wavbuf
 *    the Freq range is Fmin to Fmax Hz.   Current Samplerate is N/sec
 */

int generator::mksweep(char *bf,int bfn,char *FMbuf,char *wavbuf,int Fmin,int Fmax,unsigned int N,unsigned int afmt)
{
   int i,F,dF,x;
   short int *vp,*wb,*fmb;
   // rlr unsigned char *p;
   char *p;

   if (VERBOSE) {
      printf("Generating sweep waveform from %d Hz to %d Hz\n",Fmin,Fmax);
   }
   F=(Fmin+Fmax+1)/2;    /* calc centre freq */
   dF=Fmax-F;            /* scale factor     */
   fmb=(short int *)FMbuf;
   if (afmt==AFMT_S16_LE) {
      wb=(short int *)wavbuf;
      for (x=i=0,vp=(short int *)bf; i<N; i++) {
	 *vp++=wb[x];
	 x=(N+F+x+(((int)(*fmb++)*dF)/32767))%N;
      }
   } else if (afmt==AFMT_U8) {
      for (x=i=0,p=bf; i<N; i++ ) {
	 *p++=wavbuf[x];
	 x=(N+F+x+(((int)(*fmb++)*dF)/32767))%N;
      }
   } else {
      i=0;
   }
   return(i);  /* i=N for ok generation else 0 */
}

/* mkcos(bf,bfn,freq,A,N,R,afmt)
 * cosine wave is 90 degrees ahead of sine wave
 */

int generator::mkcos(char *bf,int bfn,unsigned freq,unsigned A,unsigned N,int R,int afmt)
{
   return(mksin(bf,bfn,freq,A,N,90+R,afmt));
}

/* mksin(bf,bfn,freq,A,N,R,afmt)
 * sine wave of freq Hertz played at N samples/sec, scaled by A/100,
 * starting at R degreees. i.e. for sin R=0, for cos R=90
 */

int generator::mksin(char *bf,int bfn,unsigned freq,unsigned A,unsigned N,int R,int afmt)
{
   unsigned i,n,h,nc;
   float w,wi,sc,pi2;
   // rlr unsigned char c,*p;
   unsigned char c; //rlr
   char *p; //rlr
   short int v,*vp;
   int t;
   
   /* This looks a little complicated, but ........
    * we determine the minimum number of samples to generate
    * so that we have an exact whole number of
    * wavelengths. Then we can simply concatenate multiple copies of
    * these samples to generate the output - the join will be accurate.
    * It's worth noting that we could simply generate N samples.
    * This is guaranteed to generate an exact numbers of wavelengths
    * because we only allow integer definition of the required freq (Hz)
    * However in most cases this is a little more optimum and quicker.
    * 
    * w (radians) is the basic loop count. However sin(w) when w>>2*PI
    * becomes inaccurate, so in the increment loop we check if w>2*PI
    * and if so subtract 2*PI so that the value is kept low for each cycle
    */
   
   n=N/(h=hcf(N,freq));  /* n = N/hcf(N,freq) is number of samples to create */
   nc=freq/h;            /* nc is number of wavelengths generated    */
    
   if (VERBOSE) {
      printf("%dHz at a sampling rate/sec of %d, scaled by %d/100\n",
	     freq,N,A);
      printf("%d samples for %d cycle(s).\n",n,nc);
   }  
   pi2=PI+PI;
   w=((float)(R)*PI)/180.0;
   wi=((float)nc*2*PI)/(float)n;
   if (afmt==AFMT_U8) {
      sc=(float)A*127.0/100.0;
      for (i=0,p=bf; i<n; i++, w+=(wi-((w>pi2)?pi2:0)) ) {
	 t=128+rint(sc*sin(w));
	 *p++=(t>255)?255:((t<0)?0:t);
      }
   } else if (afmt==AFMT_S16_LE) {
      sc=(float)A*32767.0/100.0;
      for (i=0,vp=(short int *)bf; i<n; i++, w+=(wi-((w>pi2)?pi2:0)) ) {
	 t=rint(sc*sin(w));
	 *vp++=(t>32767)?32767:((t<-32767)?-32767:t);
      }
   } else return(0);

   return(n); /* Is return type int, or what? */
}

/* mksquare(bf,bfn,freq,A,N,R,afmt)
 *  square wave - calls mkpulse with 50% mark/space ratio, scaled by A/100
 */

int generator::mksquare(char *bf,int bfn,unsigned freq,unsigned A,unsigned N,int R,int afmt)
{
   return(mkpulse(bf,bfn,freq,A,N,50,afmt));
}

/* mkpulse(bf,bfn,freq,A,N,R,afmt)
 * rectangular wave of freq Hertz played at N samples/sec,
 * mark/space ratio R (%)
 * scaled by A/100, 50% is equal mark/space, <50% is mark<space and vice-versa
 */

int generator::mkpulse(char *bf,int bfn,unsigned freq,unsigned A,unsigned N,int R,int afmt)
{
   unsigned int i,n,nc,h;
   // rlr unsigned char c,*p;
   unsigned char c; //rlr
   char *p; //rlr
   short int v,*vp;
   int t,b;
   
   n=N/(h=hcf(N,freq));  /* n = N/hcf(N,freq) is number of samples to create */
   nc=freq/h;            /* nc is number of wavelengths generated    */

   if (VERBOSE) { 
      printf("%dHz at a sampling rate/sec of %d, scaled by %d/100\n",
	     freq,N,A);
      printf("%d samples for %d cycle(s).\n",n,nc);
   }  
   R*=n;    /* scale the Ratio */
            /* we run i from 0 to n, take x=(i*nc)%n which gives result */
            /* between 0 and n and then check if x/n < R/100   or */
            /* if x*100 < R*n     hence the scale for R */
   if (afmt==AFMT_U8) {
      t=A*127/100; t=128+((t>127)?127:t);
      b=A*128/100; b=128-((b>128)?128:b);
      for (i=0,p=bf; i<n; i++) {
	 *p++=((((i*nc)%n)*100)<R)?t:b;
      }
   } else if (afmt==AFMT_S16_LE) {
      t=A*32767/100; t=(t>32767)?32767:t;
      for (i=0,vp=(short int *)bf; i<n; i++) {
	 *vp++=(((((i*nc)%n)*100)<R)?t:-t);
      }
   } else return(0);

   return(n);
}

/* mksawtooth(bf,bfn,freq,A,N,R,afmt)
 * sawtooth wave of freq Hertz, played at N samples, scaled by A/100
 */

int generator::mksawtooth(char *bf,int bfn,unsigned freq,unsigned A,unsigned N,int R,int afmt)
{
   int i,n,nc,h,t,b,d;
   // rlr unsigned char *p;
   char *p;
   short int *vp;
   
   n=N/(h=hcf(N,freq));  /* n = N/hcf(N,freq) is number of samples to create */
   nc=freq/h;            /* nc is number of wavelengths generated    */

   if (VERBOSE) { 
      printf("%dHz at a sampling rate/sec of %d, scaled by %d/100\n",
	     freq,N,A);
      printf("%d samples for %d cycle(s).\n",n,nc);
   }  
   /* for sawtooth we use (i*nc)%n to give a value upto n-1       */
   /* then adjust by subtracting n/2 to give a range of -n/2 to +n/2 */
   /* we then scale to be in the correct range and clip if necessary */
   d=n/2;
   if (afmt==AFMT_U8) {
      t=2*A*127/100;
      for (i=0,p=bf; i<n; i++) {
	 b=((i*nc)%n-d)*t/n;
	 *p++=(unsigned char)(128+((b>127)?127:((b<-128)?-128:b)));
      }
   } else if (afmt==AFMT_S16_LE) {
      t=2*A*32767/100; 
      for (i=0,vp=(short int *)bf; i<n; i++) {
	 b=((long long int)((i*nc)%n-d)*(long long int)t)/n;
	 *vp++=(short int)((b>32767)?32767:((b<-32767)?-32767:b));
      }
   } else return(0);

  return(n);
}

/* mktriangle(bf,bfn,freq,A,N,R,afmt)
 * triangle wave of freq Hertz, played at N samples/sec, scaled by A/100
 */

int generator::mktriangle(char *bf,int bfn,unsigned freq,unsigned A,unsigned N,int R,int afmt)
{
   int i,n,nc,t,d,b,h;
   short int *vp;
   // rlr unsigned char *p;
   char *p;
   
   n=N/(h=hcf(N,freq));   /* n = N/hcf(N,freq) is number of samples to create */
   nc=freq/h;             /* nc is number of wavelengths generated    */
   
   if (VERBOSE) { 
      printf("%dHz at a sampling rate/sec of %d, scaled by %d/100\n",
	     freq,N,A);
      printf("%d samples for %d cycle(s).\n",n,nc);
   }  
   /* for sawtooth we use  b=(i*nc)%n to give a value upto n-1         */
   /* if the value is < n/2 ok for upramp else set to n-b to down ramp */
   /* then adjust by subtracting n/2 to give a range of -n/2 to +n/2   */
   /* we then scale to be in the correct range and clip if necessary   */
   d=n/2;
   if (afmt==AFMT_U8) {
      t=2*A*127/100;
      for (i=0,p=bf; i<n; i++) {
	 b=(i*nc*2)%(2*n);
	 b=(((b<=n)?b:(2*n-b))-d)*t/n;
	 *p++=(unsigned char)(128+((b>127)?127:((b<-128)?-128:b)));
      }
   } else if (afmt==AFMT_S16_LE) {
      t=2*A*32767/100; 
      for (i=0,vp=(short int *)bf; i<n; i++) {
	 b=(i*nc*2)%(2*n);
	 b=((long long int)(((b<=n)?b:(2*n-b))-d)*(long long int)t)/n;
	 *vp++=(short int)((b>32767)?32767:((b<-32767)?-32767:b));
      }
   } else return(0);
   return(n);
}

/* mknoise(bf,bfn,freq,A,N,R,afmt)
 * make noise - freq is ignored. samples scaled by A/100
 * All bf is filled with pseudo random values
 */

int generator::mknoise(char *bf,int bfn,unsigned freq,unsigned A,unsigned N,int R,int afmt)
{
   unsigned int i,n,r;
   // rlr unsigned char c,*p;
   unsigned char c;
   char *p;
   short int *vp;
   short int v;
   int t,b;
   
   n=bfn;                 /* number of samples to generate - fill bf */
   if (afmt==AFMT_S16_LE) n/=2; /* if 16 bit samples = buffsize/2 */
   
   if (VERBOSE) { 
      printf("Noise, %d samples/sec\n%d samples.\n",N,n);
   }  

   srand(time(NULL));
   if (afmt==AFMT_U8) {
      for (i=0,p=bf; i<n; i++) {
	 *p++=(c=(rand()>>8)&255);
      }
   } else if (afmt==AFMT_S16_LE) {
      t=A*32767/100; t=(t>32767)?32767:t;
      for (i=0,vp=(short int *)bf; i<n; i++) {
	 *vp++=(short int)(((rand()>>8)&0xFF)+(rand()&0xFF00));
      }
   } else return(0);
   return(n);
}
  #define chkword(a,b) ((n>=a)&&(strncasecmp(p,b,n)==0))
/* mknull(bf,bfn,freq,A,N,R,afmt)
 * make quiet - fill the buffer with  zero values
 */

int generator::mknull(char *bf,int bfn,unsigned freq,unsigned A,unsigned N,int R,int afmt)
{
   unsigned int i,n,r;
   // rlr unsigned char c,*p;
   unsigned char c;
   char *p;
   short int *vp;
   short int v;
   int t,b;
   
   n=bfn;                 /* number of samples to generate - fill bf */
   if (afmt==AFMT_S16_LE) n/=2; /* if 16 bit samples = buffsize/2 */
   
   if (VERBOSE) { 
      printf("Quiet, %d samples/sec\n%d samples.\n",N,n);
   }  

   if (afmt==AFMT_U8) {
      for (i=0,p=bf; i<n; i++) {
	 *p++=(unsigned char)128;
      }
   } else if (afmt==AFMT_S16_LE) {
      for (i=0,vp=(short int *)bf; i<n; i++) {
	 *vp++=(short int)0;
      }
   } else return(0);
   return(n);
}
  
/* generate()  generate samples of waveform wf, in buf, at frequency fr,
 *             for a playing rate of S samples/sec. R is a further param
 *             if needed for pulse or sweep. buf is size N bytes.
 *             format required is afmt and samples are scaled by A/100
 *             N should be a factor of S, or, more usually, an exact multiple
 *             of S. Usually N=S for 8 bit samples or =2S for 16 bits
 *  return number of samples in buffer, or 0 if there is an error.
 */
int generator::generate(char *wf,char *buf,unsigned int N,unsigned int fr,unsigned int A,unsigned int S,int R,int afmt)
{
   int i,h,nc,n;
   char *p;
   if (wf==NULL || fr>(N/2)) return(0);
	
   n=strlen(p=wf);
   for (i=0; WavNAMES[i]!=NULL; i++) {
      if (chkword(2,WavNAMES[i])) {
	 if (VERBOSE) {
	    fputs(wf,stdout); fputs(" matches generator function : ",stdout);
	    fputs(WavNAMES[i],stdout); fputs("\n",stdout);
	 }
	 if (R==-1) R=WavDefr[i];
	 if (wf == "sine") {n=mksin(buf,N,fr,A,S,R,afmt); if (n==0) return(0);}
	 else if (wf == "cosine") {n=mkcos(buf,N,fr,A,S,R,afmt); if (n==0) return(0);}
	 else if (wf == "square") {n=mksquare(buf,N,fr,A,S,R,afmt); if (n==0) return(0);}
	 else if (wf == "triangle") {n=mktriangle(buf,N,fr,A,S,R,afmt); if (n==0) return(0);}
	 else if (wf == "sawtooth") {n=mksawtooth(buf,N,fr,A,S,R,afmt); if (n==0) return(0);}
	 else if (wf == "pulse") {n=mkpulse(buf,N,fr,A,S,R,afmt); if (n==0) return(0);}
	 else if (wf == "noise") {n=mknoise(buf,N,fr,A,S,R,afmt); if (n==0) return(0);}
	 else if (wf == "off") {n=mknull(buf,N,fr,A,S,R,afmt); if (n==0) return(0);}
	 
   /* put any common post-processing here */
   /*   here we fill the full N byte buf up with h (N/n) basic samples */
	 nc=(afmt==AFMT_S16_LE)?(n<<1):n ;
	 for (p=buf+nc,i=1,h=N/nc; i<h; i++) {
	    memcpy(p,buf,nc); p+=nc;
	 }
	 return(n*h);
      }
   }
   return(0);
}

/* getWavNames()  return pointer to an array of strings containing
 *         the names for the waveforms that we can generate
 */

char** generator::getWavNames()
{
  return(WavNAMES);
}/* = new dac*/

/* genAllWaveforms(Wfs,bufs,F,samplerate,afmt)   generate all possible
 *    waveforms at freq F and setup (*Wfs)[] with waveform names and (*bufs)[]
 *    with the actual samples.
 */

int generator::genAllWaveforms(char **Wfs[],int **bufs[],int F,int samplerate,int afmt)
{
  int i,st,n,N;
   
  if ((*Wfs)==NULL) *Wfs=WavNAMES;
  else {
    for (n=0; WavNAMES[n]!=NULL; n++) { (*Wfs)[n]=WavNAMES[n]; }
    (*Wfs)[n]=NULL;
  }
  for (n=0; WavNAMES[n]!=NULL; n++) { }  /* count the no. of waveforms */
  
  if ((*bufs) == NULL) {
    if ((*bufs=(int **)malloc((n+1)*sizeof(char *)))==NULL) return(ENOMEM);
    for (i=0; i<=n; i++) { (*bufs)[i]=NULL; }
  }
  N=samplerate<<(afmt==16);
  for (i=0; i<n ; i++) {
    if ((*bufs)[i]!=NULL) free((*bufs)[i]);
    if (((*bufs)[i]=(int *)malloc(N))==NULL) return(ENOMEM);
    if ((st=generate((WavNAMES)[i],(char*)(*bufs)[i],N,F,100,samplerate,-1,afmt))==0) 
	return(st);
  }
  return(0);
}



