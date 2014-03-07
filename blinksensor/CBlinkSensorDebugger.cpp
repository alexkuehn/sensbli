/*
 * cblinksensordebugger.cpp
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
#include <time.h>
#include <stdio.h>

#include "CBlinkSensorDebugger.h"

//#include "../CAppManager.h"
#include "../config.h"

#include <cv.h>

CBlinkSensorDebugger* blinkdebug = NULL;

CBlinkSensorDebugger::CBlinkSensorDebugger(CImageDB* img, QSettings* settings)
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer [80];

    CvSize imgsize;
    imgdb = img;


    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    strftime (buffer,80,"sensbli-%Y-%m-%d_%H%M%S.log",timeinfo);



    imgsize = imgdb->getImageSize();

    blobmarks = NULL;
    trackmarks = NULL;
    temporig = NULL;
    blobmarks = cvCreateImage( imgsize, IPL_DEPTH_8U, 3);
    trackmarks = cvCreateImage( imgsize, IPL_DEPTH_8U, 3);

    if( settings->value("system/debug", 0).toBool() == true )
    {
    	enable_logfile = true;
    }
    else
    {
    	enable_logfile = false;
    }

    #if(CFG_DBG_LOGFILE)
    if( enable_logfile == true )
    {
    	dbgfile.open( buffer );
    }
    #endif
}

CBlinkSensorDebugger::~CBlinkSensorDebugger()
{
    if( blobmarks != NULL)
    {
        cvReleaseImage( &blobmarks );
    }

    if( trackmarks != NULL)
    {
        cvReleaseImage( &blobmarks );
    }

    if( temporig != NULL)
    {
        cvReleaseImage( &temporig );
    }

    #if(CFG_DBG_LOGFILE)
    if( enable_logfile == true )
    {
        dbgfile.close();
    }
    #endif
}

void CBlinkSensorDebugger::new_log_frame(int state, unsigned long framecnt)
{
	framecounter = framecnt;
    #if(CFG_DBG_LOGFILE)
    if( enable_logfile == true )
    {
        dbgfile << "--f+ " << framecounter << ";" << state << std::endl;
    }
    #endif
}

void CBlinkSensorDebugger::post_log_frame(int state)
{
    #if(CFG_DBG_LOGFILE)
    if( enable_logfile == true )
    {
        dbgfile << "--f- " << framecounter << ";" << state << std::endl;
    }
    #endif
}

void CBlinkSensorDebugger::blob_log(CBlobResult* blobs)
{
#if(CFG_DBG_LOGFILE)
    if( enable_logfile == true )
    {
        dbgfile << "BLOBUFs;" << framecounter << ";" << blobs->GetNumBlobs() << std::endl;
        for( int b=0; b < blobs->GetNumBlobs(); b++ )
         {
               CBlob* blob = blobs->GetBlob(b);

               dbgfile << "BLOBUFe;" << framecounter << ";" << blob->Area() << ";" << blob->MinX() << ";"
                       << blob->MaxX() << ";" << blob->MinY() << ";" << blob->MaxY() << std::endl;

         }

    }
#endif
}

void CBlinkSensorDebugger::blob_log_unfiltered(CBlobResult* blobs)
{
#if(CFG_DBG_LOGFILE)
    if( enable_logfile == true )
    {
        dbgfile << "BLOBRAWs;" << framecounter << ";" << blobs->GetNumBlobs() << std::endl;
        for( int b=0; b < blobs->GetNumBlobs(); b++ )
         {
               CBlob* blob = blobs->GetBlob(b);

               dbgfile << "BLOBRAWe;" << framecounter << ";" << blob->Area() << ";" << blob->MinX() << ";"
                       << blob->MaxX() << ";" << blob->MinY() << ";" << blob->MaxY() << std::endl;

         }
    }

#endif
}
void CBlinkSensorDebugger::blob_filt_log(bool filterednum)
{
#if(CFG_DBG_LOGFILE)
    if( enable_logfile == true )
    {
        dbgfile << "BLOBUFfnum;" << framecounter << ";";
        if( filterednum == true )
        {
            dbgfile << "1";
        }
        else
        {
            dbgfile << "0";
        }
        dbgfile << std::endl;
    }

#endif
}

void CBlinkSensorDebugger::resetBlobMarks()
{
    cvSetZero( blobmarks );
}

void CBlinkSensorDebugger::resetTrackMarks()
{
    cvSetZero( trackmarks );
}

void CBlinkSensorDebugger::buildTrackMarks( CvPoint* popen, CvPoint* pclose, int beginindex, int endindex )
{
#if(CFG_DBG_TRACKDRAW)
        CvPoint p1,p2;
        for( int i=beginindex; i < endindex; i++ )
        {
                p1.x = popen[i].x-2;
                p2.x = popen[i].x+2;
                p1.y = popen[i].y;
                p2.y = popen[i].y;
                cvLine( trackmarks, p1, p2, CV_RGB(0,255,0),1,8,0);
                p1.x = popen[i].x;
                p2.x = popen[i].x;
                p1.y = popen[i].y-2;
                p2.y = popen[i].y+2;
                cvLine( trackmarks, p1, p2, CV_RGB(0,255,0),1,8,0);


                p1.x = pclose[i].x-2;
                p2.x = pclose[i].x+2;
                p1.y = pclose[i].y;
                p2.y = pclose[i].y;
                cvLine( trackmarks, p1, p2, CV_RGB(255,0,0),1,8,0);
                p1.x = pclose[i].x;
                p2.x = pclose[i].x;
                p1.y = pclose[i].y-2;
                p2.y = pclose[i].y+2;
                cvLine( trackmarks, p1, p2, CV_RGB(255,0,0),1,8,0);

        }
#endif
}

void CBlinkSensorDebugger::buildWindowMarks( CvRect* searchwindow,
                                             CvRect* eyewindow,
                                             CvRect* snaprect, int beginindex, int endindex)
{
#if(CFG_DBG_TRACKDRAW)
        CvPoint rv1, rv2;
        for( int i=beginindex; i < endindex; i++ )
        {
            rv1.x = searchwindow[i].x;
            rv1.y = searchwindow[i].y;
            rv2.x = rv1.x + searchwindow[i].width;
            rv2.y = rv1.y + searchwindow[i].height;
            cvRectangle( trackmarks, rv1, rv2, CV_RGB(255,255,0),1,8,0);

            rv1.x = eyewindow[i].x;
            rv1.y = eyewindow[i].y;
            rv2.x = rv1.x + eyewindow[i].width;
            rv2.y = rv1.y + eyewindow[i].height;
            cvRectangle( trackmarks, rv1, rv2, CV_RGB(0,255,255),1,8,0);

            rv1.x = snaprect[i].x;
            rv1.y = snaprect[i].y;
            rv2.x = rv1.x + snaprect[i].width;
            rv2.y = rv1.y + snaprect[i].height;
            cvRectangle( trackmarks, rv1, rv2, CV_RGB(0,0,255),1,8,0);

        }
#endif
}

void CBlinkSensorDebugger::buildBlobMarks(CBlobResult *blobs)
{
#if(CFG_DBG_BLOBDRAW)
    CvPoint rv1, rv2;
    for( int b=0; b < blobs->GetNumBlobs(); b++ )
    {
          CBlob* blob = blobs->GetBlob(b);
          rv1.x = (int)blob->MinX();
          rv1.y = (int)blob->MinY();
          rv2.x = (int)blob->MaxX();
          rv2.y = (int)blob->MaxY();

          cvRectangle( blobmarks, rv1, rv2, CV_RGB(255,0,0),1,8,0);


    }
#endif
}
void CBlinkSensorDebugger::buildPosBlobMarks( CvRect* rects)
{
#if(CFG_DBG_BLOBDRAW)
    CvPoint rv1, rv2;

    for( int i=0; i < CFG_NUM_OF_EYES; i++)
    {
        CvRect* trect = &rects[i];
        rv1.x = trect->x;
        rv1.y = trect->y;
        rv2.x = rv1.x + trect->width;
        rv2.y = rv1.y + trect->height;
        cvRectangle( blobmarks, rv1, rv2, CV_RGB(0,255,0),1,8,0);
    }
#endif
}
IplImage *CBlinkSensorDebugger::getBlobMarks(IplImage* orig)
{
    if( temporig != NULL)
    {
        cvReleaseImage( &temporig );
    }

    temporig = cvCloneImage( orig );
    cvAdd( blobmarks, temporig, temporig);

    cvAdd( trackmarks, temporig, temporig );
    return temporig;

}

void CBlinkSensorDebugger::blobpair_log(std::vector<CBlobPair*> *pairlist)
{
#if(CFG_DBG_LOGFILE)
	if( enable_logfile == true )
	{
		for( unsigned int i=0; i < pairlist->size(); i++)
		{
			  CBlobPair* blobpair = pairlist->at(i);

			   dbgfile << "BLOBP;" << framecounter << ";"
					   << blobpair->index1 << ";"
					   << blobpair->index2 << ";"
					   << blobpair->blob1->Area() << ";"
					   << blobpair->blob1->MinX() << ";"
					   << blobpair->blob1->MaxX() << ";"
					   << blobpair->blob1->MinY() << ";"
					   << blobpair->blob1->MaxY() << ";"
					   << blobpair->blob2->Area() << ";"
					   << blobpair->blob2->MinX() << ";"
					   << blobpair->blob2->MaxX() << ";"
					   << blobpair->blob2->MinY() << ";"
					   << blobpair->blob2->MaxY() << ";"
	//
					   << blobpair->diffx1 << ";"
					   << blobpair->diffy1 << ";"
					   << blobpair->diffx2 << ";"
					   << blobpair->diffy2 << ";"
					   << blobpair->centerx1 << ";"
					   << blobpair->centery1 << ";"
					   << blobpair->centerx2 << ";"
					   << blobpair->centery2 << ";"
	//
					   << blobpair->aspect1 << ";"
					   << blobpair->aspect2 << ";"
					   << blobpair->xdelta << ";"
					   << blobpair->ydelta << ";"
					   << blobpair->distance << ";"
					   << blobpair->distangle << ";"
					   << blobpair->normwidth1 << ";"
					   << blobpair->normwidth2 << ";"
					   << blobpair->basenormwidth1 << ";"
					   << blobpair->basenormwidth2 << ";"
					   << blobpair->aspectratio << ";"
					   << blobpair->arearatio << ";"
					   << blobpair->overallvalid << ";"
					   << blobpair->validity
					   << std::endl;


		}
	}
#endif
}

std::ofstream* CBlinkSensorDebugger::get_dbgfile_handle()
{
#if(CFG_DBG_LOGFILE)
	if( enable_logfile == true )
	{
		return &dbgfile;
	}
	else
	{
		return NULL;
	}
#else
    return NULL;
#endif
}






