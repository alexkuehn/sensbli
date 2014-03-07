/*
 * crecordprocessor.cpp
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
#include "CRecordProcessor.h"
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <QWidget>

CRecordProcessor::CRecordProcessor(QWidget *parent, CImageDB* img)
{
    imgdb = img;
    parentwnd = parent;

    outfile = NULL;
}

CRecordProcessor::~CRecordProcessor()
{
	if( outfile != NULL)
	{
		cvReleaseVideoWriter( &outfile );
		outfile = NULL;
	}
}

void CRecordProcessor::init(QSettings* settings)
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer [80];


    QObject::connect( this, SIGNAL(updateFileText(QString)), parentwnd, SLOT(getFileText(QString)));
    QString txt = tr("none");
    emit updateFileText( txt );

    CvSize imgsize = imgdb->getImageSize();

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    strftime (buffer,80,"sensbli-%Y-%m-%d_%H%M%S.avi",timeinfo);

    if( outfile != NULL )
    {
    	cvReleaseVideoWriter( &outfile );
    }
	outfile =cvCreateVideoWriter( buffer, CV_FOURCC('I','Y','U','V'), 25, cvSize(640,480), 1);

	QString filetext = buffer;
	emit updateFileText( filetext );


}

void CRecordProcessor::process()
{
	if( outfile != NULL)
	{
		cvWriteFrame( outfile, imgdb->get_actual_image());
	}
}

IplImage* CRecordProcessor::getCalibrationOverlay()
{
	return NULL;
}

IplImage* CRecordProcessor::getOverlay()
{
	return imgdb->get_actual_image();
}

QString	CRecordProcessor::getStatusText()
{
	QString status= tr("Recording");

	return status;
}
