/*
 * cblinkprocessor.cpp
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

#include <iostream>
#include <highgui.h>

#include "../config.h"
#include "CBlinkProcessor.h"

using namespace std;

CBlinkProcessor::CBlinkProcessor(QWidget *parent, CImageDB* img)
{
    imgdb = img;
    parentwidget = parent;
    // cvNamedWindow( "Live", CV_WINDOW_AUTOSIZE);

    #if(CFG_DBG_FIND_DIFF)
//    cvNamedWindow( "Find Diff", CV_WINDOW_AUTOSIZE);
    #endif

    #if(CFG_DBG_FIND_BIN)
 //   cvNamedWindow( "Find Bin", CV_WINDOW_AUTOSIZE);
    #endif

    #if(CFG_DBG_TRACKER)
//    cvNamedWindow( "Shot Color 1 act", CV_WINDOW_AUTOSIZE);
//    cvNamedWindow( "Shot Color 2 act", CV_WINDOW_AUTOSIZE);
//    cvNamedWindow( "Shot Color 1 last", CV_WINDOW_AUTOSIZE);
//    cvNamedWindow( "Shot Color 2 last", CV_WINDOW_AUTOSIZE);

//    cvNamedWindow( "Shot Color 1 open", CV_WINDOW_AUTOSIZE);
//    cvNamedWindow( "Shot Color 2 open", CV_WINDOW_AUTOSIZE);
//    cvNamedWindow( "Shot Color 1 close", CV_WINDOW_AUTOSIZE);
//    cvNamedWindow( "Shot Color 2 close", CV_WINDOW_AUTOSIZE);
//
//    cvNamedWindow( "Shot Sbin 1 act", CV_WINDOW_AUTOSIZE);
//    cvNamedWindow( "Shot Sbin 2 act", CV_WINDOW_AUTOSIZE);
//    cvNamedWindow( "Shot Sbin 1 last", CV_WINDOW_AUTOSIZE);
//    cvNamedWindow( "Shot Sbin 2 last", CV_WINDOW_AUTOSIZE);
//
//    cvNamedWindow( "Sbin 1 act", CV_WINDOW_AUTOSIZE);
//    cvNamedWindow( "Sbin 2 act", CV_WINDOW_AUTOSIZE);
//    cvNamedWindow( "Eyecomp 1 act", CV_WINDOW_AUTOSIZE);
//    cvNamedWindow( "Eyecomp 2 act", CV_WINDOW_AUTOSIZE);
//
//    cvNamedWindow( "SearchWin 1 act", CV_WINDOW_AUTOSIZE);
//    cvNamedWindow( "SearchWin 2 act", CV_WINDOW_AUTOSIZE);
    #endif

    eyefinder = NULL;
    eyetracker = NULL;
    eyestatehandler = NULL;
    state = STATE_INIT;
    framecounter = 0;
}

CBlinkProcessor::~CBlinkProcessor()
{
//    cvDestroyWindow( "Live" );

    #if(CFG_DBG_FIND_DIFF)
//    cvDestroyWindow( "Find Diff");
    #endif

    #if(CFG_DBG_FIND_BIN)
//    cvDestroyWindow( "Find Bin");
    #endif

    #if(CFG_DBG_TRACKER)
//    cvDestroyWindow( "Shot Color 1 act");
//    cvDestroyWindow( "Shot Color 2 act");
//    cvDestroyWindow( "Shot Color 1 last");
//    cvDestroyWindow( "Shot Color 2 last");
//
//    cvDestroyWindow( "Shot Color 1 open");
//    cvDestroyWindow( "Shot Color 2 open");
//    cvDestroyWindow( "Shot Color 1 close");
//    cvDestroyWindow( "Shot Color 2 close");
//
//    cvDestroyWindow( "Shot Sbin 1 act");
//    cvDestroyWindow( "Shot Sbin 2 act");
//    cvDestroyWindow( "Shot Sbin 1 last");
//    cvDestroyWindow( "Shot Sbin 2 last");
//
//    cvDestroyWindow( "Sbin 1 act");
//    cvDestroyWindow( "Sbin 2 act");
//    cvDestroyWindow( "Eyecomp 1 act");
//    cvDestroyWindow( "Eyecomp 2 act");
//
//    cvDestroyWindow( "SearchWin 1 act");
//    cvDestroyWindow( "SearchWin 2 act");
    #endif

    if( eyetracker != NULL)
    {
        delete eyetracker;

    }

    if( eyefinder != NULL)
    {
        delete eyefinder;

    }

    if( eyestatehandler != NULL)
    {
        delete eyestatehandler;

    }

    if( blinkdebug != NULL )
    {
        delete blinkdebug;
        blinkdebug = NULL;
    }

}

void CBlinkProcessor::init(QSettings* settings)
{
	framecounter = 0;
    state = STATE_INIT;

    eyefinder = new CEyeFinder( imgdb );
    eyetracker = new CEyeTracker( imgdb, eyefinder );
    eyestatehandler = new CEyeStateHandler();

    QObject::connect( eyestatehandler, SIGNAL(sendMessage(QByteArray)), parentwidget, SLOT(broadcastMessage(QByteArray)));
    eyestatehandler->connectionEstablished();

    if( blinkdebug == NULL )
    {
        blinkdebug = new CBlinkSensorDebugger( imgdb, settings );
    }
}

void CBlinkProcessor::process()
{

	framecounter ++;

#if(CFG_DBG_MAINSTATE)
    std::cout << "DBG: MAINSTATE PRE: " << state << std::endl;
#endif

    blinkdebug->new_log_frame( state, framecounter );
    blinkdebug->resetTrackMarks();
    blinkdebug->resetBlobMarks();


    switch( state )
    {
        // we initialize the whole processing
        case STATE_INIT:

            // init the eyefinder
            eyefinder->init();
            // switch state to findeyes
            state = STATE_FIND;

            break;

        case STATE_FIND:
            // we are in the finder state
            // do the first process loop

            eyefinder->process();


            // check if we found an eye
#if(CFG_DBG_HANGFIND==0)
            if( eyefinder->found() == true )
            {
                // we found the initial eye positions
                // proceed with eye tracking

                // init the eye tracker
                eyetracker->init();
                // and process the first frame
                eyetracker->process();

                eyestatehandler->process( eyetracker->lost(), eyetracker->get_state(),
                        eyetracker->getBesteyeindex_in(), eyetracker->getBesteyeindex_out());

                // switch to tracker state
                state = STATE_TRACK;
            }
#endif
            break;

        case STATE_TRACK:
            // we are in the tracking state

            // check if the tracker is lost
            if( eyetracker->lost() == true )
            {
                // tracker has lost the eyes :-(
                // reinit the whole thing
                state = STATE_INIT;
            }
            else
            {
                // we havent lost the eyes, continue with the tracking
                eyetracker->process();
                eyestatehandler->process( eyetracker->lost(), eyetracker->get_state(),
                        eyetracker->getBesteyeindex_in(), eyetracker->getBesteyeindex_out());
            }
            break;
        default:
            // better we don't come here
            std::cerr << "ERROR: main state machine undefined\n";

    }

#if(CFG_DBG_MAINSTATE)
    std::cout << "DBG: MAINSTATE POST: " << state << std::endl;
#endif

    blinkdebug->post_log_frame( state );
    //blinkdebug->getBlobMarks(imgdb->get_actual_image());

}

IplImage* CBlinkProcessor::getOverlay()
{
	return blinkdebug->getBlobMarks(imgdb->get_actual_image());

}

IplImage* CBlinkProcessor::getCalibrationOverlay()
{
	return NULL;
}

QString	CBlinkProcessor::getStatusText()
{
	QString status;

	switch( state )
	{
		case STATE_INIT:
			status = tr("Waiting");
			break;
		case STATE_FIND:
			status = tr("search Eyes");
			break;
		case STATE_TRACK:
			if( eyestatehandler->getState() == true )
			{
				status = tr("Eye closed");
			}
			else
			{
				status = tr("Eye open");
			}
			break;
		default:
			status = tr("Waiting");
			break;

	}

	return status;
}

