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
#ifndef DAC_H
#define DAC_H
#include <iostream>
using namespace::std;
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/soundcard.h>


/**
@author Rob Russell
*/
class dac{
public:
    dac();
    int is16bit(int fd);
    int getfragsize(int fd);
    int getfreeobufs(int fd);
    int getfullibufs(int fd);
    int printibufs(int fd);
    int setfragsize(int fd,int N,int bfps,int S,int afmt,int stereo);
    int DACopen(char *fnm,char *mode,unsigned int *samples,unsigned int *fmt,int *stereo);
    

    ~dac();

};

#endif
