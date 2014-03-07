/*
 * cimagedb.cpp
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
#include "CImageDB.h"

#include <iostream>
#include <cv.h>
#include <math.h>

using namespace std;

CImageDB::CImageDB()
{
    for( int i=0; i < CFG_IMGBUFFER_SIZE; i++ )
    {
        imagebuffer_inuse[i] = false;
        imagebuffer[i] = NULL;
        imagebuffer_gray[i] = NULL;
    }
    db_filled = false;
    actptr = 0;
    addedptr = 0;
}

CImageDB::~CImageDB()
{
    reset_imagebuffer();
}

// clean imagebuffer
void CImageDB::reset_imagebuffer()
{
    for( int i=0; i < CFG_IMGBUFFER_SIZE; i++ )
    {
        // we have an allocated object
        if( imagebuffer[i] != NULL )
        {
            cvReleaseImage( &imagebuffer[i]);
            imagebuffer[i] = NULL;
        }

        if( imagebuffer_gray[i] != NULL )
        {
            cvReleaseImage( &imagebuffer_gray[i]);
            imagebuffer[i] = NULL;
        }

        imagebuffer_inuse[i] = false;
    }
    db_filled = false;
    actptr = 0;
    addedptr = 0;
}

// initialize the buffer with an image template
void CImageDB::init( IplImage* imgtemplate )
{
    // first, we reset the buffer
    reset_imagebuffer();

    imagesize = cvGetSize( imgtemplate);

    for( int i=0; i < CFG_IMGBUFFER_SIZE; i++ )
    {
        // allocate image buffers for grayscale image copys
        imagebuffer_gray[i] =
            cvCreateImage(imagesize, IPL_DEPTH_8U, 1);

    }

}

void CImageDB::add( IplImage* img)
{
    if( imagebuffer[actptr] != NULL )
    {
        cvReleaseImage( &imagebuffer[actptr] );
    }

    imagebuffer[actptr] = cvCloneImage( img );

    cvCvtColor( img, imagebuffer_gray[actptr], CV_BGR2GRAY);

    addedptr = actptr;

    actptr++;

    if( actptr >= CFG_IMGBUFFER_SIZE)
    {
        db_filled = true;
        actptr = 0;
    }

}

bool CImageDB::is_filled()
{
   return db_filled;
}

IplImage* CImageDB::get_actual_image()
{
    IplImage* imgptr;

    if( db_filled == false )
    {
        imgptr = NULL;
    }
    else
    {
        imgptr = imagebuffer[addedptr];
    }

    return imgptr;
}

IplImage* CImageDB::get_actual_image_gray()
{
    IplImage* imgptr;

    if( db_filled == false )
    {
        imgptr = NULL;
    }
    else
    {
        imgptr = imagebuffer_gray[addedptr];
    }

    return imgptr;
}

IplImage* CImageDB::get_older_image( unsigned int histindex )
{
    IplImage* imgptr;

    if( (db_filled == false) || (histindex > IMAGEDB_OLDEST))
    {
        imgptr = NULL;
    }
    else
    {
        int idxdiff = addedptr - (CFG_IMGBUFFER_SIZE-1);
        int modidx = 0;

        if( idxdiff < 0 )
        {
            modidx = CFG_IMGBUFFER_SIZE-abs(idxdiff);
        }
        else
        {
            modidx = idxdiff;
        }

        imgptr = imagebuffer[modidx];
    }

    return imgptr;

}

IplImage* CImageDB::get_older_image_gray( unsigned int histindex )
{
    IplImage* imgptr;

    if( (db_filled == false) || (histindex > IMAGEDB_OLDEST))
    {
        imgptr = NULL;
    }
    else
    {
        int idxdiff = addedptr - (CFG_IMGBUFFER_SIZE-1);
        int modidx = 0;

        if( idxdiff < 0 )
        {
            modidx = CFG_IMGBUFFER_SIZE-abs(idxdiff);
        }
        else
        {
            modidx = idxdiff;
        }

        imgptr = imagebuffer_gray[modidx];
    }

    return imgptr;

}

IplImage* CImageDB::get_oldest_image()
{
    return (get_older_image(IMAGEDB_OLDEST));
}

IplImage* CImageDB::get_oldest_image_gray()
{
    return (get_older_image_gray(IMAGEDB_OLDEST));
}

CvSize CImageDB::getImageSize()
{
    return imagesize;
}
