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
#include "config.h"



extern int vflg,dflg,vikeys;

extern int resolution;                  /* 1 = 1Hz resolution, 10 = 0.1HZ,
					 * 100=0.01 */
extern int DAC;
extern unsigned int samplerate;         /* Samples/sec        */
extern unsigned int stereo;             /* stereo mono */
extern unsigned int afmt;               /* format for DSP  */
extern int Bufspersec;                  /* Number of Buffers per sec */
extern int Nfragbufs;                   /* number of driver buffers */
extern int fragsize;                    /* size of driver buffer fragments */
extern int fragsamplesize;              /* size of fragments in samples */
extern int LWn;                         /* number of specified loadable waveforms */
extern char **LWaa;                     /* array of specifed loadable waveforms */

    /* channel 1  -  or mono..... */
extern char wf[32];                     /* waveform type */
extern unsigned int freq;               /* signal frequency */
extern int ratio;                       /* used in pulse, sweep etc */
extern int Gain;                        /* Amplification factor */

    /* channel 2 when in stereo mode ..... */
extern char wf2[32];                    /* waveform type */
extern unsigned int freq2;              /* signal frequency */
extern int ratio2;                      /* used in pulse, sweep etc */
extern int Gain2;                       /* Amplification factor */
extern int phase;                       /* phase diff with chan1 */

