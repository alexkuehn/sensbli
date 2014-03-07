/*
 * sensbli.cpp
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
#include "sensbli.h"

#include "setup.h"
#include <iostream>

using namespace std;

sensbli::sensbli(QWidget *parent)
    : QWidget(parent)
{
	// setup mainwindow
	ui.setupUi(this);

	// create camera display widget
//	cvout = new QOpenCVWidget( ui.liveview );

	// create image database
	imgdb = new CImageDB();


	ui.liveview->setScaledContents(true);


	// setup periodic capture loop
	visiontimer = new QTimer(this);
	connect(visiontimer, SIGNAL(timeout()), this, SLOT(updateVision()) );

	// get settings
	settings = new QSettings( "AKu", "sensbli");

	// init camera variables and frameprocessor
	camera = NULL;

	// init frameprocessor object
	frameprocessor = NULL;

	// disable buttons, which are only available with camera
	ui.bnCalib->setEnabled( false );
	ui.bnRecord->setEnabled( false );

	udpsocket = new QUdpSocket(this);
	initCapture();

	QImage dummy(640,480,QImage::Format_RGB32);
	liveimg = dummy;

    for (int x = 0; x < 640; x ++) {
        for (int y =0; y < 480; y++) {
            liveimg.setPixel(x,y,qRgb(0, 0, 0));
        }
    }

    calibration_mode = false;
    recording_mode = false;

    calibview = new CalibView(this);
    setupdlg = new setup(this, settings);
    recorddlg = new Recording(this);

	connect( recorddlg, SIGNAL(stopRecording()), this, SLOT(on_bnRecord_clicked()));


}

void sensbli::getFileText(QString txt)
{
	recorddlg->updateFileText( txt );
}

QImage sensbli::genImage(IplImage *cvimage)
{
	QImage temp(cvimage->width, cvimage->height, QImage::Format_RGB32);
    int cvIndex, cvLineStart;
    // switch between bit depths
    switch (cvimage->depth) {
        case IPL_DEPTH_8U:
            switch (cvimage->nChannels) {
                case 3:
                    cvIndex = 0; cvLineStart = 0;
                    for (int y = 0; y < cvimage->height; y++) {
                        unsigned char red,green,blue;
                        cvIndex = cvLineStart;
                        for (int x = 0; x < cvimage->width; x++) {
                            // DO it
                            red = cvimage->imageData[cvIndex+2];
                            green = cvimage->imageData[cvIndex+1];
                            blue = cvimage->imageData[cvIndex+0];

                            temp.setPixel(x,y,qRgb(red, green, blue));
                            cvIndex += 3;
                        }
                        cvLineStart += cvimage->widthStep;
                    }
                    break;
                case 1:
                    cvIndex = 0; cvLineStart = 0;
                    for (int y = 0; y < cvimage->height; y++) {
                        unsigned char red,green,blue;
                        cvIndex = cvLineStart;
                        for (int x = 0; x < cvimage->width; x++) {
                            // DO it
                            red = cvimage->imageData[cvIndex];
                            green = cvimage->imageData[cvIndex];
                            blue = cvimage->imageData[cvIndex];

                            temp.setPixel(x,y,qRgb(red, green, blue));
                            cvIndex += 1;
                        }
                        cvLineStart += cvimage->widthStep;
                    }
                    break;

                default:
                    printf("This number of channels is not supported\n");
                    break;
            }
            break;
        default:
            printf("This type of IplImage is not implemented in QOpenCVWidget\n");
            break;
    }

    return temp;

}

void sensbli::reinit()
{
	visiontimer->stop();
	calibview->close();
	calibration_mode = false;
	recording_mode = false;

	ui.bnCalib->setText( tr("Calibration"));
	// disable buttons, which are only available with camera
	ui.bnCalib->setEnabled( false );
	ui.bnRecord->setEnabled( false );

	if( camera != NULL )
	{
		cvReleaseCapture( &camera );
	}

	if( frameprocessor != NULL )
	{
		delete frameprocessor;
	}

	initCapture();


}

void sensbli::initCapture()
{
	camera = cvCreateCameraCapture(settings->value("camera/index", -1).toInt());

	if( camera != NULL )
	{
		cvSetCaptureProperty( camera, CV_CAP_PROP_FRAME_WIDTH, 640 );
		cvSetCaptureProperty( camera, CV_CAP_PROP_FRAME_HEIGHT, 480 );


		// reset free running frame counter
		framecounter = 0;

		// capture first frame
		recvimg = cvQueryFrame(camera);

		// check if frame is valid
		if( recvimg != NULL )
		{
			// first valid frame received: init processor
			frameprocessor = new CBlinkProcessor( this, imgdb );
		    framecounter = 1;
		    // init image database with first frame seed
		    imgdb->init( recvimg );
		    // add the first frame to database
		    imgdb->add( recvimg );

		    if( frameprocessor != NULL)
		    {
		        frameprocessor->init(settings);
		    }

			//timer period
			visiontimer->start(15);

			// we can enable GUI options
			ui.bnCalib->setEnabled( true );
			ui.bnRecord->setEnabled( true );
			ui.liveview->setScaledContents(false);
		}
	}

}

sensbli::~sensbli()
{
	if( camera != NULL )
	{
		cvReleaseCapture( &camera );
	}

	delete imgdb;



	if( frameprocessor != NULL )
	{
		delete frameprocessor;
	}

	delete calibview;
	delete setupdlg;
	delete udpsocket;
}

void sensbli::on_bnQuit_clicked()
{

	qApp->quit();
}

void sensbli::on_bnSetup_clicked()
{
	setupdlg->show();
}

void sensbli::broadcastMessage(QByteArray data)
{
	udpsocket->writeDatagram( data.data(), data.size(),
								  QHostAddress(settings->value("system/net/address", "localhost" ).toString()),
								  settings->value("system/net/port", 4200 ).toInt());

}

void sensbli::on_bnRecord_clicked()
{
	visiontimer->stop();
	if( recording_mode == false)
	{

		recording_mode = true;
		ui.bnRecord->setText( tr("Stop Recording"));
		calibration_mode = false;
		calibview->close();
		ui.bnCalib->setText( tr("Calibration"));
		ui.bnCalib->setEnabled(false);

	}
	else
	{
		recorddlg->close();

		recording_mode = false;
		ui.bnRecord->setText( tr("Recording"));
		ui.bnCalib->setEnabled(true);
	}

	if( camera != NULL )
	{
		if( frameprocessor != NULL )
		{
			delete frameprocessor;
		}


		// capture first frame
		recvimg = cvQueryFrame(camera);

		// check if frame is valid
		if( recvimg != NULL )
		{

			if( recording_mode == true )
			{
				// first valid frame received: init processor
				frameprocessor = new CRecordProcessor( this, imgdb );
				recorddlg->show();
			}
			else
			{
				frameprocessor = new CBlinkProcessor( this, imgdb );
			}
		    framecounter = 1;

		    // init image database with first frame seed
		    imgdb->init( recvimg );
		    // add the first frame to database
		    imgdb->add( recvimg );


		    if( frameprocessor != NULL)
		    {
		        frameprocessor->init(settings);
		    }
		    visiontimer->start(15);

		}

	}

}

void sensbli::on_bnCalib_clicked()
{
	visiontimer->stop();
	if( calibration_mode == false)
	{

		calibration_mode = true;
		ui.bnCalib->setText( tr("Exit Calibration"));

	}
	else
	{
		calibview->close();
		calibration_mode = false;
		ui.bnCalib->setText( tr("Calibration"));
	}

	if( camera != NULL )
	{
		if( frameprocessor != NULL )
		{
			delete frameprocessor;
		}


		// capture first frame
		recvimg = cvQueryFrame(camera);

		// check if frame is valid
		if( recvimg != NULL )
		{

			if( calibration_mode == true )
			{
				// first valid frame received: init processor
				frameprocessor = new CCalibProcessor( this, imgdb );
				calibview->show();
			}
			else
			{
				frameprocessor = new CBlinkProcessor( this, imgdb );
			}
		    framecounter = 1;

		    // init image database with first frame seed
		    imgdb->init( recvimg );
		    // add the first frame to database
		    imgdb->add( recvimg );


		    if( frameprocessor != NULL)
		    {
		        frameprocessor->init(settings);
		    }
		    visiontimer->start(15);

		}

	}
}

void sensbli::updateVision()
{

	// periodic frame grabbing, only done if camera is present
	if( camera != NULL )
	{

		// capture frame
		recvimg = cvQueryFrame(camera);


		// check if frame is valid
		if( recvimg != NULL )
		{
			framecounter++;
			// add frame to database
			imgdb->add(recvimg);

		    // check if database is filled enough for processing
		    if( imgdb->is_filled() == true )
		    {
		    	if( frameprocessor != NULL)
		        {

		    		frameprocessor->process();

		    		ui.liveview->setPixmap(QPixmap::fromImage( genImage(frameprocessor->getOverlay() )));

		    		if( calibration_mode == true )
		    		{
		    			calibview->getLabel()->setPixmap(QPixmap::fromImage( genImage(frameprocessor->getCalibrationOverlay())));
		    		}
		        }

		    }

		}
		else
		{
			// error: invalid frame received
		}

	}

	if( frameprocessor == NULL )
	{
		ui.statusinfo->setText( tr("Waiting"));
	}
	else
	{
		ui.statusinfo->setText( frameprocessor->getStatusText() );
	}
}





