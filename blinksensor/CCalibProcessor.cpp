/*
 * ccalibprocessor.cpp
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
#include <highgui.h>
#include <iostream>

#include "../config.h"
#include "CCalibProcessor.h"

CCalibProcessor::CCalibProcessor(QWidget *parent, CImageDB* img)
{
    imgdb = img;

//    cvNamedWindow( "Calib Size", CV_WINDOW_AUTOSIZE);
//    cvNamedWindow( "Calib Whiteness", CV_WINDOW_AUTOSIZE);

    hsvimage = NULL;
    simage = NULL;
    sbinimage = NULL;
    maskimage = NULL;
    masktarget = NULL;

}

CCalibProcessor::~CCalibProcessor()
{
//    cvDestroyWindow( "Calib Size" );
//    cvDestroyWindow( "Calib Whiteness");

    if( hsvimage != NULL )
    {
        cvReleaseImage( &hsvimage );
    }
    if( simage != NULL )
    {
        cvReleaseImage( &simage );
    }

    if( sbinimage != NULL )
    {
        cvReleaseImage( &simage );
    }

    if( maskimage != NULL )
    {
        cvReleaseImage( &simage );
    }

    if( masktarget != NULL )
    {
        cvReleaseImage( &simage );
    }

}

void CCalibProcessor::init(QSettings* settings)
{
    CvSize imgsize = imgdb->getImageSize();
    hsvimage = cvCreateImage( imgsize, IPL_DEPTH_8U, 3);
    simage = cvCreateImage( imgsize, IPL_DEPTH_8U, 1);
    sbinimage = cvCreateImage( imgsize, IPL_DEPTH_8U, 1);
    maskimage = cvCreateImage( imgsize, IPL_DEPTH_8U, 3);
    cvResize( cvLoadImage( "mask.jpg",1), maskimage);
    masktarget = cvCreateImage( imgsize, IPL_DEPTH_8U, 3);
}

void CCalibProcessor::process()
{
    cvCvtColor( imgdb->get_actual_image(), hsvimage, CV_BGR2HSV);
    cvSetImageCOI(hsvimage, 2);
    cvCopy( hsvimage, simage, 0);
    cvThreshold( simage, sbinimage, CFG_SAT_THRESHOLD, CFG_MAX_VAL, CV_THRESH_BINARY_INV);

    if( maskimage != NULL )
    {
    	cvAddWeighted( maskimage, CFG_MASK_ALPHA, imgdb->get_actual_image(), 1-CFG_MASK_ALPHA, 0.0, masktarget );
    }
    else
    {
    	std::cerr << "maskImage not found!" << std::endl;
    }
//    cvShowImage( "Calib Size", masktarget );
//    cvShowImage( "Calib Whiteness", sbinimage );
}

IplImage* CCalibProcessor::getCalibrationOverlay()
{
	return sbinimage;
}

IplImage* CCalibProcessor::getOverlay()
{
	return masktarget;
}

QString	CCalibProcessor::getStatusText()
{
	QString status= tr("Calbrating");

	return status;
}
