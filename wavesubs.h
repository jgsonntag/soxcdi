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
#ifndef __WAVSUBS_H
#define __WAVSUBS_H

#include <stdio.h>

#ifndef AFMT_S16_LE
# define AFMT_S16_LE 16
#endif
#ifndef AFMT_U8
# define AFMT_U8     8
#endif
#ifndef STEREO
# define STEREO 1
#endif
#ifndef MONO
# define MONO 0
#endif

/* WavValues  structure for holding information about a set of samples
 */

struct WavValues { 
  int samples;     /* number of samples */
  int samplerate;  /* samplerate at which data was sampled */
  int stereo;      /* stereo (1) or mono (0) data */
  int afmt;        /* format of samples unsigned 8 bit (8)
                      or Signed 16 bit little endian (16) */
  int Bps;         /* Bytes per sec */
  int Bypsam;      /* Bytes per sample */
};

/* Functions definitions......
 */

int isriff(FILE *f);

int wavreadfmt(FILE *f,struct WavValues *W);
 
int wavwritefmt(FILE *f,struct WavValues *W);   
 
int wavread(FILE *f,struct WavValues *W,unsigned char *buf,int N);
 
int wavwrite(FILE *f,struct WavValues *W,unsigned char *buf,int N);

int setendian();
int Wconvfmt(unsigned char *p,struct WavValues *W);

/* Macros provided in wavsubs.h for struct WavValues .......
 */

#define GetWavSamplerate(W)   ((W)->samplerate)
#define SetWavSamplerate(W,S) ((W)->samplerate = S)
#define GetWavStereo(W)       ((W)->stereo)
#define SetWavStereo(W,S)     ((W)->stereo = S)
#define GetWavAFmt(W)         ((W)->afmt)
#define SetWavAFmt(W,F)       ((W)->afmt = F)
#define GetWavSamples(W)      ((W)->samples)
#define SetWavSamples(W,N)    ((W)->samples = N)
 
#define IsWavStereo(W)        ((W)->stereo == STEREO)
#define IsWavMono(W)          ((W)->stereo == MONO)
#define IsWav16bit(W)         ((W)->afmt == AFMT_S16_LE)
#define IsWav8bit(W)          ((W)->afmt == AFMT_U8)

#define NewWavValues()   ((struct WavValues *)malloc(sizeof(struct WavValues)))

extern int ISBIGENDIAN;

#endif  /* __WAVSUBS_H */
