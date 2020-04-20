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
#ifndef GENERATOR_H
#define GENERATOR_H
using namespace::std;
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include <iostream>
#include "config.h"
#include "misc.h"

#ifndef PI
# ifdef M_PI
#  define PI M_PI
# else
#  define PI 3.141592653589793
# endif
#endif
#define chkword(a,b) ((n>=a)&&(strncasecmp(p,b,n)==0))

#define VERBOSE (vflg>1)


/**
@author rob
*/
class generator{
public:
    generator();

    ~generator();
    int mk8bit(unsigned char *bf,short int *ip,int N);
    void mixplaybuf(unsigned char *a,unsigned char *b,unsigned char *c,int N,int afmt);
    void chanmix(unsigned char *a,unsigned char *b,int Nb,unsigned char *c,int Nc,int N,int afmt);
    void do_antiphase(unsigned char *a,unsigned char *b,int N,int afmt);
    unsigned char* maketone(char *wf,int fr,int N,int G,int afmt);
    int mksweep(char *bf,int bfn,char *FMbuf,char *wavbuf,int Fmin,int Fmax,unsigned int N,unsigned int afmt);
    int mkcos(char *bf,int bfn,unsigned freq,unsigned A,unsigned N,int R,int afmt);
    int mksin(char *bf,int bfn,unsigned freq,unsigned A,unsigned N,int R,int afmt);
    int mksquare(char *bf,int bfn,unsigned freq,unsigned A,unsigned N,int R,int afmt);
    int mkpulse(char *bf,int bfn,unsigned freq,unsigned A,unsigned N,int R,int afmt);
    int mksawtooth(char *bf,int bfn,unsigned freq,unsigned A,unsigned N,int R,int afmt);
    int mktriangle(char *bf,int bfn,unsigned freq,unsigned A,unsigned N,int R,int afmt);
    int mknoise(char *bf,int bfn,unsigned freq,unsigned A,unsigned N,int R,int afmt);
    int mknull(char *bf,int bfn,unsigned freq,unsigned A,unsigned N,int R,int afmt);
    int generate(char *wf,char *buf,unsigned int N,unsigned int fr,unsigned int A,unsigned int S,int R,int afmt);
    char** getWavNames();
    int genAllWaveforms(char **Wfs[],int **bufs[],int F,int samplerate,int afmt);    
    
    int vflg;
    
    
	//static char *WavNAMES[];//= {"sine","cosine","square","triangle","sawtooth","pulse","noise", "off", NULL};
	static int *WavFuncs[];
	//static int WavDefr[];//={ 0,0,0,0,0,10,0,0,0 };

};

#endif
