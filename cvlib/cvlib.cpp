/*
 * cvlib.cpp
 * This file is part of SENSBLI.
 *
 * Copyright (c) 2009 Alexander Kühn (prj@alexkuehn.de)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the project's author nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include "cvlib.h"

bool rescaleRect(CvRect *rect, double scalefactor, CvSize boundaries)
{
    int width = scalefactor * rect->width;
    int height = scalefactor * rect->height;
    int widthdiff = (width - rect->width)/2;
    int heightdiff = (height - rect->height)/2;

    int x = rect->x - widthdiff;
    int y = rect->y - heightdiff;

    if(     (x < 0)
         || ( (x+width) > boundaries.width)
         || (y < 0)
         || ( (y+height) > boundaries.height)
      )
    {
        return false;
    }
    else
    {
        rect->x = x;
        rect->y = y;
        rect->width = width;
        rect->height = height;
        return true;
    }

}

bool translateRect(CvRect *rect, CvPoint translate, CvSize boundaries)
{
    int x = translate.x;
    int y = translate.y;

    if(     (x < 0)
         || ( (x+rect->width) > boundaries.width)
         || (y < 0)
         || ( (y+rect->height) > boundaries.height)
      )
    {
        return false;
    }
    else
    {
        rect->x = x;
        rect->y = y;
        return true;
    }

}

