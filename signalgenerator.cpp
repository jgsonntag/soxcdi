/***************************************************************************
 *   Copyright (C) 2004 by Robbie Russell                                  *
 *   rob@localhost                                                         *
 *                                                                         *
 ***************************************************************************/

#include "signalgenerator.h"


signalgenerator::signalgenerator()
{
resolution= 100;
WAVEFORMS=NULL;
wavbufs=NULL;
wf="cosine"; 
freq=90;     
ratio=-1;    
Gain=1000;   
		
wf2="cosine";            
freq2=150;
ratio2=-1;
Gain2=1000;
phase=0;   
mygen = new generator;
mydac = new dac;

configfile=DEF_CONF_FILENAME;
samplerate=44100; 
Nfragbufs=16384;
Bufspersec=100;
resolution=100;
afmt = 16;
stereo = -1;
vflg = 1;

maxLeftGain = 1000; // default gain
maxRightGain = 1000; // default gain
exitme = 0;
exitVar = 0;

QMutex *mutex = new QMutex;
}


signalgenerator::~signalgenerator()
{
}

void signalgenerator::steerPlane(int l_amt, int r_amt)
{
//if (l_amt <= 1000)
//{
	double leftVal = (double)l_amt/granularity;
	double rightVal = (double)r_amt/granularity;
//	std::cout << "vals: " << l_amt << " " << leftVal << " " << r_amt << " " << rightVal << std::endl;
	Gain = maxLeftGain*leftVal;
	Gain2 = maxRightGain*rightVal;
//	std::cout << "Gains: " << Gain << " " << Gain2 <<std::endl;
//	std::cout << "Max: " << maxLeftGain << " " << maxRightGain << std::endl;
//}

}

void signalgenerator::setGranularity(int g){
	granularity=g;
}
void signalgenerator::turnOff()
{
	lastgain1=Gain;
	lastgain2=Gain2;
	setmaxGainLeft (0);
	setmaxGainRight (0);
	isOn=false;
}

void signalgenerator::turnOn()
{
	//if (lastgain1!=Gain)
	setmaxGainLeft (lastgain1);
	//Gain=lastgain1;
//	if (lastgain2!=Gain2)
	setmaxGainRight (lastgain2);
	//Gain2=lastgain2;
	isOn=true;
}

void signalgenerator::setmaxGainLeft(int gain)
{
cout << "Setting left gain to " << gain << endl;
if (gain <= 1000 && gain > -1) {maxLeftGain = gain; Gain = gain;}
}

void signalgenerator::setmaxGainRight(int gain)
{
cout << "Setting right gain to " << gain << endl;
if (gain <= 1000 && gain > -1) {maxRightGain = gain; Gain2 = gain;}

}

void signalgenerator::adjustgain(int which, int gainset)
{
/* 	Which : 0 for left, 1 for right.
	Gainsetting: 0-1000 ( >1000 -> clipping)
 */
if (gainset < 2000)
{
	if (which == 0 ) {Gain = gainset;lastgain1=gainset;}
	if (which == 1 ) {Gain2 = gainset;lastgain2=gainset;}
	}
else { cout << "Invalid gain request" << endl;}

}

int signalgenerator::resetgen()
{
int st;
cout << "resetting to new gain values." << endl;
if (st=mygen->genAllWaveforms(&WAVEFORMS,&wavbufs,1,Lsamplerate*resolution,Lafmt)) {
	          cout << "Problem generating Internal waveforms."; return (1);
	       }
}

int signalgenerator::setup()
{
/* Setup to default values */
i=afmt;
n=stereo;
if ((DAC=mydac->DACopen(DAC_FILE,"w",&samplerate,&i,&n))<0) {
      QThread::exit();
   }
if (((afmt!=AFMT_QUERY) && (i!=afmt)) ||
       ((stereo!=-1) && (n!=stereo))) {
      QThread::exit();
   }
   afmt=i; stereo=n;
   
   if ((fragsize=mydac->setfragsize(DAC,Nfragbufs,Bufspersec,samplerate,afmt,stereo))<0) {
      cout << "Value of : " << fragsize << " returned." << endl;
      QThread::exit();
   }
   fragsamplesize=(fragsize>>(afmt==AFMT_S16_LE))>>(stereo);
      
   if (freq > (samplerate/2)) {//stereo=-1;
      fprintf(stderr,"%d Hz is more than half the sampling rate!!!\n",freq);
   }

   if (vflg) {
      printf("%s %s bit samples being generated.\n",
	     (stereo)?"Stereo, ":"Mono, ",(afmt==AFMT_S16_LE)?"16":"8");
      printf("Samples amplified by a factor of %d/1000\n",Gain);
      printf("Playing at %d samples/sec\n",samplerate);
      printf("Frequency resolution is %.2fHz\n",1.0/(resolution));
      printf("Initial Frequencies: ch1 %dHz  ch2 %dHz\n",freq,freq2);
      printf("ch2 at a phase difference of %d degrees to ch1\n",phase);
      printf("Buffer fragment size is %d bytes (%d samples). Aprox. %d millisecs.\n",
	     fragsize, fragsamplesize,
	     1000*((fragsize>>(stereo))>>(afmt==AFMT_S16_LE))/samplerate);
      printf("Requested %d buffers/sec and have %d buffs/sec\n",Bufspersec,
	     (samplerate+(fragsamplesize/2))/fragsamplesize);
      printf("%d loadable waveforms specified.\n",LWn);
   }
 
   fd = DAC;
   tmpafmt = afmt;
   Lsamplerate=samplerate;         /* set local working copy of samplerate */
   Lafmt=afmt; Lstereo=stereo;
   LNfb=Nfragbufs;

   fragtim=((fragsamplesize)*1000000)/(samplerate);
   
   if (nn=mygen->genAllWaveforms(&WAVEFORMS,&wavbufs,1,samplerate*resolution,tmpafmt)) {
      return(err_rpt(nn,"Problem generating Internal waveforms."));
   } 
   afmt = tmpafmt;
   nn=0;   /* setup the correct number of Decimal points depending on resolution */
   if (resolution==10) nn=1;
   else if (resolution==100) nn=2;

   freq=(freq)*(resolution); 
   freq2=(freq2)*(resolution);
   wavef=card=ff=0;
   adjustgain(0,200);
   lastgain1=Gain;
   lastgain2=Gain2;
	isOn=true;
}






int signalgenerator::playsamples(int fd)
{
   if (exitme == 1) QThread::exit();
   static int fr_size=0;
   static unsigned char *frag=NULL;
   //void *frag=NULL;
   static int of1,of2;

   short int v,*vp,*wv1,*wv2;
   unsigned char *p,c,*w1,*w2;
   int i,n,ph,tof2,t;
   //afmt == AFMT_S16_LE;
   ph=(resolution*samplerate*(phase%360)+180)/360;
                                     /* phase diff of chan2 reduced */
                                     /* to 0-359 degrees then converted */
                                     /* to equiv offset into wave buffs */ 

   if (fd<0) { of2=0; return(of1=0); }     /* reset pointers */ 
   if (fragsize!=fr_size) {
      if (fr_size) free(frag);
      //if (((void *)frag=malloc(fragsize))==NULL) return(-1);
	         if ((frag=(unsigned char*)malloc(fragsize))==NULL) return(-1);
      fr_size=fragsize; of1=0; of2=0;
   }

   for (;;) {
      if ((i=mydac->getfreeobufs(fd))<=0) return(i);
      tof2=(of2+ph)%(samplerate*resolution);  /* add phase to current generator offset */
      if (afmt==AFMT_S16_LE) {
	 wv1=(short int *)(wavbufs[1]);  //rlr 
	 wv2=(short int *)(wavbufs[1]);  //rlr.
	 if (stereo) {
	    for (i=0, vp=(short int *)frag; i<fragsamplesize; i++) {
/*	       t=(5000+(((long long)(wv1[of1]))*(long long)Gain))/(long long)10000; */
	       t=(500+(((int)(wv1[of1]))*Gain))/1000;
	       *vp++=(short int)((t>32767)?32767:((t<-32767)?-32767:t));
/*	       t=(5000+(((long long)(wv2[tof2]))*(long long)Gain2))/(long long)10000; */
	       t=(500+(((int)(wv2[tof2]))*Gain2))/1000;
	       *vp++=(short int)((t>32767)?32767:((t<-32767)?-32767:t));
	       of1=(of1+freq)%(samplerate*resolution);
	       tof2=(tof2+freq2)%(samplerate*resolution);
	    }
	 } else {
	    for (i=0, vp=(short int *)frag; i<fragsamplesize; i++) {
/*	       t=(((long long)(wv1[of1])*(long long)Gain)+
		  ((long long)(wv2[tof2])*(long long)Gain2)+10000)/(long long)20000; */
	       t=(((int)(wv1[of1])*Gain)+
		  ((int)(wv2[tof2])*Gain2)+1000)/2000;
	       *vp++=(short int)((t>32767)?32767:((t<-32767)?-32767:t));
	       of1=(of1+freq)%(samplerate*resolution);
	       tof2=(tof2+freq2)%(samplerate*resolution);
	    }
	 }
      } else {  /* afmt is 8 bit */
	 w1=(unsigned char*)(wavbufs[1]); //rlr
	 w2=(unsigned char*)(wavbufs[1]); //rlr
	 if (stereo) {
	    for (i=0, p=frag; i<fragsamplesize; i++) {
/*	       t=128+(5000+((int)(w1[of1])-128)*Gain)/10000; */
	       t=128+(500+(((int)(w1[of1])-128)*Gain))/1000;
	       *p++=(unsigned char)((t>255)?255:((t<0)?0:t));
/*	       t=128+(5000+((int)(w2[tof2])-128)*Gain2)/10000; */
	       t=128+(500+(((int)(w2[tof2])-128)*Gain2))/1000;
	       *p++=(unsigned char)((t>255)?255:((t<0)?0:t));
	       of1=(of1+freq)%(samplerate*resolution);
	       tof2=(tof2+freq2)%(samplerate*resolution);
	    }
	 } else {
	    for (i=0, p=frag; i<fragsamplesize; i++) {
/*	       t=128+(((int)(w1[of1])*Gain)+
		      ((int)(w2[tof2])*Gain2)-256+10000)/20000; */
	       t=128+((((int)(w1[of1])-128)*Gain)+
		      (((int)(w2[tof2])-128)*Gain2)+1000)/2000;
	       /*  -256 + 10000 = -128-128+5000+5000 */
	       *p++=(unsigned char)((t>255)?255:((t<0)?0:t));
	       of1=(of1+freq)%(samplerate*resolution);
	       tof2=(tof2+freq2)%(samplerate*resolution);
	    }
	 }
      }   /* afmt==AFMT_S16_LE */
      of2=(tof2-ph+(samplerate*resolution))%(samplerate*resolution);  /* remove phase from generator offset */
      if (write(fd,frag,fragsize)<0) return(-1);
   }
   return(0);
}


void signalgenerator::run()
{	

for (;;) {
	
      if (this->playsamples(this->fd)<0 ) {
	 cout << "Problem generating output samples."<< endl;
	 
      }	
   }

  /* for (int i = 0; i < 10000 && !exitme; i++)
   	{
	
	cout << "Number " << i << endl;
	if (mutex.tryLock() == TRUE)
		{
		
		//exitme = exitVar;
		mutex.unlock();
		}
	QThread::sleep(1);
	}
	
	*/

}
