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
#include "dac.h"

dac::dac()
{
}


dac::~dac()
{
}

/* DACopen(device,mode,&samplerate,&fmt,&stereo)
 *    opens device for mode (r/w) at samplerate samples/sec in 8/16 bit fmt
 *    and in stereo. Sets these params to best match possible.
 * 
 * getfragsize(fd) 
 *    get fragment size of a DSP buffer
 * 
 * getfreeobufs(fd)
 *    return number of free output fragment buffers free at moment
 * 
 * getfullibufs(fd)
 *    return number of full input  fragment buffers full at moment
 *
 * setfragsize(fd,N,bfps,S,afmt,stereo)
 *    set number of buffers to N and size such that we get aprox bfps buffers
 *    played per second. S samples per sec of mono/stereo, 8/16 bit samples.
 * 
 * is16bit(fd)
 *    returns true if file fd is a 16 sound device
 * 
 * isstereo(fd)
 *    returns true if file fd is a stereo sound device  (To Be Done)
 */

/* is16bit(fd)   return true if fd is a dsp device supporting 16 bit
 *               sampling. Return false if not.
 */

int dac::is16bit(int fd)
{
   int fmts;
   
   if (ioctl(fd, SNDCTL_DSP_GETFMTS , &fmts) < 0) {
      return(0);
   }
   return(fmts&AFMT_S16_LE);
}

/* getfragsize(fd)   return fragsize of DSP buufer for fd
 *    or return -1 on error.
 */

int dac::getfragsize(int fd)
{
   int n;
   
   if (ioctl(fd, SNDCTL_DSP_GETBLKSIZE, &n)<0) return(-1);
   return(n);
}

/* getfreeobufs(fd)   return number of empty output fragment buffers 
 *     available or return -1 on error.
 */

int dac::getfreeobufs(int fd)
{
   int n;
   audio_buf_info osp;

   if (ioctl(fd,SNDCTL_DSP_GETOSPACE, &osp)<0) return(-1);
   return(osp.fragments);
}

/* getfullibufs(fd)   return number of full input fragment buffers 
 *     available or return -1 on error.
 */

int dac::getfullibufs(int fd)
{
   int n;
   count_info isp;

   if (ioctl(fd,SNDCTL_DSP_GETIPTR, &isp)<0) return(-1);
   return(isp.bytes);
}

/* printibufs(fd)  print input audio_buf_info
 *      return -1 on error.
 */
int dac::printibufs(int fd)
{
   int n;
   count_info isp;

   if (ioctl(fd,SNDCTL_DSP_GETIPTR, &isp)<0) return(-1);
   printf("Count Info: bytes=%d, blocks=%d, ptr=%0Xx\n",
	  isp.bytes, isp.blocks, isp.ptr);
   return(0);
}

/* setfragsize(fd,N,bfps,S,afmt,stereo)
 *                     calculate buffer size to ensure aproximately
 *                     bfps buffers per second, then set N buffs of this size
 *                     afmt is 8 or 16 bit samples, stereo is true if 2 chans
 *                     return -1 if error - see errno, else return fragsize
 *                     set.
 */

int dac::setfragsize(int fd,int N,int bfps,int S,int afmt,int stereo)
{
   cout << "Values are: " << fd << " fd " << N << " N " << bfps << " bfps " << S << " S " << afmt << " afmt " << stereo << " stereo" << endl;
   int fr,n;
   
   if (N<1) {
      errno=EINVAL; return(-1); 
   }
   
   n=S/bfps;     /* n is samplerate / buffers/sec */
   for (fr=5; (2<<fr) < n ; fr++) { }
                 /* adjust for stereo and 16 bits */
   fr=(N<<16)+fr+((stereo)?1:0)+((afmt==AFMT_S16_LE)?1:0);
   if (ioctl(fd, SNDCTL_DSP_SETFRAGMENT , &fr) < 0) return(-1);
   if (ioctl(fd, SNDCTL_DSP_GETBLKSIZE , &n) < 0) return(-1);
   return(n);
}

/*
 * DACopen(char *fnm, char *mode, int *samples, int *fmt, int *stereo)
 * 
 *   open dspfile for read "r" or write "w", and set samples per sec
 *    as sampling rate - note we get pointer for samples so we can 
 *    return the actual samplerate set. 
 *    If stereo mode is unspecified (-1) then set stereo if possible,
 *    if fmt is unspecified (AFMT_QUERY) then set 16 bit if possible
 *    otherwise we set mono and/or 8 bit.
 *    Actual settings are returned in fmt and stereo.
 *   return file descriptor or -1 on error.
 */

int dac::DACopen(char *fnm,char *mode,unsigned int *samples,unsigned int *fmt,int *stereo)
{
   int fd;
   int m,i;
   
   if (*mode=='r') m=O_RDONLY;
   else if (*mode=='w') m=O_WRONLY;
   else {
      errno=EINVAL; return(-1);
   }
   if ((fd = open (fnm,m,0)) >= 0) {  /* params must be set in this order */
      if ((*fmt==AFMT_QUERY) && is16bit(fd)) *fmt=AFMT_S16_LE;
      if (ioctl(fd, SNDCTL_DSP_SETFMT, fmt)>=0) { 
	 if (*stereo==-1) {
	    *stereo=1;
	    if (ioctl(fd, SNDCTL_DSP_STEREO, stereo)<0) *stereo=0;
	 }
	 if (ioctl(fd, SNDCTL_DSP_STEREO, stereo)>=0) {
	    if (ioctl(fd, SNDCTL_DSP_SPEED, samples)>=0) { 
	       return(fd);
	    }
	 }
      }
   }
   return(-1);
}




