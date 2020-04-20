/***************************************************************************
 *   Copyright (C) 2004 by Rob Russell                                     *
 ***************************************************************************/
#ifndef SIGNALGENERATOR_H
#define SIGNALGENERATOR_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <generator.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/soundcard.h>
#include <math.h>
#include <qthread.h>
#include <qmutex.h>
#include "siggen.h"
#include "dac.h"
#include "wavesubs.h"
#include "generator.h"


#define MAXPRM 32
#define chkword(a,b) ((n>=a)&&(strncmp(p,b,n)==0))
#include <time.h>

extern char *sys;
extern int vflg; 


/**
@author Rob Russell
*/
class signalgenerator : public QThread{
public:
	signalgenerator();
	~signalgenerator();
	
	int resetgen();
	int setup(); /* Setup to default values */
	void turnOff(); // out of range, turn off temporarily
	void turnOn();  // within range, turn back on
	void steerPlane(int l_amt, int r_amt);
	void setmaxGainLeft(int gain);
	void setmaxGainRight(int gain);
	void setGranularity(int g);
	void adjustgain(int which, int gainset);	
	int fd;
	int playsamples(int fd);
	inline void exitThread()
		{
		exitme = 1;
		}
	int DAC;
	int exitme;
	int exitVar;
	QMutex *mutex;
	
	int maxLeftGain;
	int maxRightGain;
	bool isOn;
	int granularity;
virtual void run();	
private:
	generator *mygen;
	dac *mydac;
	
	int vflg, dflg;

	int nn;
	int ff,wavef,card;
	int tmpafmt;
	int tmpvar;
	unsigned int i;
	int n;
	
	
	

	int Lsamplerate, LNfb, Lafmt, Lstereo;	/* Local copy of card params to prevent */
	int fragtim;                 		/* Playing Time of one fragment in Microsecs */
	char **WAVEFORMS/*=NULL*/;
	int **wavbufs/*=NULL*/;

	int resolution;                  	/* Freq. resolution for signal generation 
					  	 * 1 for 1Hz res ; 10 for 0.1Hz res; 
					  	 * 100 for 0.01Hz res                   */

	unsigned int samplerate;         	/* Samples/sec        */
	unsigned int stereo;             	/* stereo mono */
	unsigned int afmt;               	/* format for DSP  */
	int Bufspersec;                  	/* number of sounds fragments per second */
	int Nfragbufs;                   	/* number of driver frag buffers */
	int fragsize;           	      	/* size of driver buffer fragments */
	int fragsamplesize;			/* size of fragments in samples */
	int LWn;	                        /* number of specified loadable waveforms */
	char **LWaa;                     	/* array of specifed loadable waveforms */

    	char *wf;				/* waveform type */
	unsigned int freq;			/* signal frequency */
	int ratio;                    		/* used in pulse, sweep etc */
	int Gain;                   		/* Amplification factor 
					    	 Samples are scaled by Gain/1000 */
	char *wf2;		              	/* waveform type */
	unsigned int freq2;          		/* signal frequency */
	int ratio2; 				/* Used in pulse,step etc */
	int Gain2;                  		/* Amplification factor see above */
	int phase;                     		/* phase difference of chan2 from chan1 */
	char *sys;				/* Externs */
	char *configfile;
	int lastgain1;
	int lastgain2;


};

#endif
