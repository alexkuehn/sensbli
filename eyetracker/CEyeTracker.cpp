/*
 * ceyetracker.cpp
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
#include "CEyeTracker.h"
#include "../blinksensor/CBlinkSensorDebugger.h"
#include <highgui.h>
#include "../cvlib/cvlib.h"

CEyeTracker::CEyeTracker(CImageDB* img, CEyeFinder *finder)
{
    imgdb = img;
    eyefinder = finder;

    // default: lost the sight
    statelost = true;

    initialeyepos = NULL;
    initialeyecenter = NULL;

    for( int i=0; i < CFG_NUM_OF_EYES; i++ )
    {
        for(int j=0; j < SNAPSTATE_ACT+1; j++)
        {
            snapshots[i].coloreye[j] = NULL;
            snapshots[i].hsveye[j] = NULL;
            snapshots[i].seye[j] = NULL;
            snapshots[i].sbineye[j] = NULL;

            snapshots[i].colortrack[j] = NULL;
            snapshots[i].graytrack[j] = NULL;


        }

        searchwindow[i].color = NULL;
        searchwindow[i].gray = NULL;
        corrresult[i] = NULL;
    }
}

CEyeTracker::~CEyeTracker()
{
    clear_snapshots();
}

bool CEyeTracker::lost()
{
    return statelost;
}

bool* CEyeTracker::get_state()
{
        return isclosed;
}

void CEyeTracker::determine_best_eye()
{
    #if(CFG_ONLY_BEST)
    besteyeindex_in = 0;

    unsigned int refarea = initialeyepos[0].width * initialeyepos[0].height;
    unsigned int actarea;

    for( int i=1; i < CFG_NUM_OF_EYES; i++ )
    {
        actarea = initialeyepos[i].width * initialeyepos[i].height;
        if( actarea > refarea )
        {
                refarea = actarea;
                besteyeindex_in = i;
        }
    }
    besteyeindex_out = besteyeindex_in+1;
    #else
    besteyeindex_in = 0;
    besteyeindex_out = CFG_NUM_OF_EYES;
    #endif

}

void CEyeTracker::init()
{
    statelost = true;

    initialeyepos = eyefinder->getFoundRects();
    initialeyecenter = eyefinder->getFoundPoints();
    determine_best_eye();

    if( ( take_snapshots()
        && determine_initial_state()
        ) != false
      )
    {
            statelost = false;
    }


    #if(CFG_DBG_TRACKER)
//    cvShowImage( "Shot Color 1 act", snapshots[0].colortrack[1]);
//    cvShowImage( "Shot Color 2 act", snapshots[1].colortrack[1]);
//    cvShowImage( "Shot Color 1 last", snapshots[0].colortrack[0]);
//    cvShowImage( "Shot Color 2 last", snapshots[1].colortrack[0]);
//
//    cvShowImage( "Shot Color 1 open", snapshots[0].coloreye[openindex[0]]);
//    cvShowImage( "Shot Color 2 open", snapshots[1].coloreye[openindex[1]]);
//    cvShowImage( "Shot Color 1 close", snapshots[0].coloreye[closeindex[0]]);
//    cvShowImage( "Shot Color 2 close", snapshots[1].coloreye[closeindex[1]]);
//
//    cvShowImage( "Shot Sbin 1 act", snapshots[0].sbineye[1]);
//    cvShowImage( "Shot Sbin 2 act", snapshots[1].sbineye[1]);
//    cvShowImage( "Shot Sbin 1 last", snapshots[0].sbineye[0]);
//    cvShowImage( "Shot Sbin 2 last", snapshots[0].sbineye[0]);
    #endif

}

void CEyeTracker::process()
{
    if( statelost == true )
        return;



    update_searchwindow();
    correlate_mask();

    #if(CFG_DBG_TRACKER)

//    cvShowImage( "Sbin 1 act", snapshots[0].sbineye[0]);
//    cvShowImage( "Sbin 2 act", snapshots[1].sbineye[0]);
//    cvShowImage( "Eyecomp 1 act", snapshots[0].coloreye[0]);
//    cvShowImage( "Eyecomp 2 act", snapshots[1].coloreye[0]);
//
//    cvShowImage( "SearchWin 1 act", searchwindow[0].color);
//    cvShowImage( "SearchWin 2 act", searchwindow[1].color);
    #endif

    if( determine_correlation() == false )
    {
            statelost = true;
            return;
    }

    determine_whiteness();

    if( calc_state() == false )
    {
            statelost = true;
            return;
    }
}


bool CEyeTracker::calc_state()
{
    bool valid = true;

    for( int i=besteyeindex_in; i < besteyeindex_out; i++ )
    {
        double minpercentopen = maxval[i][openindex[i]] * CFG_MINPERCENT_OPEN;
        double minpercentclose = maxval[i][closeindex[i]] * CFG_MINPERCENT_CLOSE;

         if( (maxval[i][openindex[i]] < CFG_MIN_CORR)
             && (maxval[i][closeindex[i]] < CFG_MIN_CORR) )
         {
                 valid = false;
         }




         if( (whiteness[i] >= minwhiteness_open[i])
             && (maxval[i][openindex[i]] > CFG_MIN_CORR_OPEN )
#if(CFG_FILTER_CLOSEREL)
             && (maxval[i][openindex[i]] >  (maxval[i][closeindex[i]] + minpercentclose) )
#endif
           )
         {
                 opencount[i]++;

                 if( opencount[i] >= CFG_TRACK_FILTERTIME )
                 {
                         opencount[i] = CFG_TRACK_FILTERTIME;
                         closecount[i] = 0;
                         isclosed[i] = false;
                 }

         }

         if( (whiteness[i] < minwhiteness_close[i])
             && (maxval[i][closeindex[i]] > CFG_MIN_CORR_OPEN )
#if(CFG_FILTER_CLOSEREL)
             && (maxval[i][closeindex[i]] >  (maxval[i][openindex[i]] + minpercentopen) )
#endif
           )
         {
                 closecount[i]++;

                 if( closecount[i] >= CFG_TRACK_FILTERTIME )
                 {
                         closecount[i] = CFG_TRACK_FILTERTIME;
                         opencount[i] = 0;
                         isclosed[i] = true;
                 }

         }


    #if(CFG_DBG_RESULT)
        std::cout << "Eye: " << i << " state: " << isclosed[i] << std::endl;
    #endif

    }

    return valid;
#if(CFG_DBG_RESULT)
#endif
}


bool CEyeTracker::correlate_mask()
{
    CvPoint maxopen[CFG_NUM_OF_EYES];
    CvPoint maxclose[CFG_NUM_OF_EYES];

    for( int i=besteyeindex_in; i < besteyeindex_out; i++ )
    {
        cvMatchTemplate( searchwindow[i].gray, snapshots[i].graytrack[openindex[i]], corrresult[i],  CV_TM_CCOEFF_NORMED );
        cvMinMaxLoc( corrresult[i],
                        &minval[i][openindex[i]],  &maxval[i][openindex[i]],
                        &minpoint[i][openindex[i]], &maxpoint[i][openindex[i]], 0 );

        cvMatchTemplate( searchwindow[i].gray, snapshots[i].graytrack[closeindex[i]], corrresult[i],  CV_TM_CCOEFF_NORMED );
        cvMinMaxLoc( corrresult[i],
                        &minval[i][closeindex[i]],  &maxval[i][closeindex[i]],
                        &minpoint[i][closeindex[i]], &maxpoint[i][closeindex[i]], 0 );


        maxopen[i] = maxpoint[i][openindex[i]];
        maxclose[i] = maxpoint[i][closeindex[i]];

        maxopen[i].x += searchwindowrect[i].x + (snaprect[i].width/2);
        maxclose[i].x += searchwindowrect[i].x + (snaprect[i].width/2);
        maxopen[i].y += searchwindowrect[i].y + (snaprect[i].height/2);
        maxclose[i].y += searchwindowrect[i].y + (snaprect[i].height/2);

#if(CFG_DBG_CORR)
        std::cout << "eye: " << i << " maxopen: " << maxval[i][openindex[i]] << "maxclose: " << maxval[i][closeindex[i]] << std::endl;
#endif
    }

    blinkdebug->buildTrackMarks( &maxopen[0], &maxclose[0], besteyeindex_in, besteyeindex_out );

    return false;
}

bool CEyeTracker::determine_whiteness()
{
    CvSize imgsize = imgdb->getImageSize();
    int imgarea = imgsize.height* imgsize.width;

    for( int i=besteyeindex_in; i < besteyeindex_out; i++ )
    {
        int white = cvCountNonZero( snapshots[i].sbineye[0]);

        whiteness[i] = white/(double)imgarea;

#if(CFG_DBG_CORR)
        std::cout << "eye: " << i << " whiteness: " << whiteness[i] << std::endl;
#endif

    }
    return false;
}

bool CEyeTracker::determine_correlation()
{
     int bestfitindex[CFG_NUM_OF_EYES];
     CvSize imgsize = imgdb->getImageSize();
     CvPoint transpoint;

     bool trans_suceeded = true;

     for( int i=besteyeindex_in; i < besteyeindex_out; i++ )
     {
        // which mask fits better

        if( maxval[i][openindex[i]] > maxval[i][closeindex[i]] )
        {
                bestfitindex[i] = openindex[i];
        }
        else
        {
                bestfitindex[i] = closeindex[i];
        }

#if(CFG_DBG_CORR)
        std::cout << "eye: " << i << "bestfitindex: " << bestfitindex[i] << std::endl;
#endif

        transpoint.x =  searchwindowrect[i].x
                       + maxpoint[i][bestfitindex[i]].x
                       + (snaprect[i].width/2)
                       - (searchwindowrect[i].width/2);
        transpoint.y =  searchwindowrect[i].y
                       + maxpoint[i][bestfitindex[i]].y
                       + (snaprect[i].height/2)
                       - (searchwindowrect[i].height/2);

        if( translateRect( &searchwindowrect[i], transpoint, imgsize )  == false )
        {
            trans_suceeded = false;
        }

        transpoint.x =  searchwindowrect[i].x
                       + maxpoint[i][bestfitindex[i]].x
                       + (snaprect[i].width/2)
                       - (eyecomprect[i].width/2);
        transpoint.y =  searchwindowrect[i].y
                       + maxpoint[i][bestfitindex[i]].y
                       + (snaprect[i].height/2)
                       - (eyecomprect[i].height/2);

        if( translateRect( &eyecomprect[i], transpoint, imgsize )  == false )
        {
            trans_suceeded = false;
        }

     }
     return trans_suceeded;


}

bool CEyeTracker::update_searchwindow()
{
    blinkdebug->buildWindowMarks( searchwindowrect, eyecomprect, snaprect, besteyeindex_in, besteyeindex_out );
    for( int i=besteyeindex_in; i < besteyeindex_out; i++ )
    {

        cvSetImageROI( imgdb->get_actual_image(), searchwindowrect[i]);
        cvCopy( imgdb->get_actual_image(), searchwindow[i].color);
        cvResetImageROI( imgdb->get_actual_image() );
        cvSetImageROI( imgdb->get_actual_image_gray(), searchwindowrect[i]);
        cvCopy( imgdb->get_actual_image_gray(), searchwindow[i].gray);
        cvResetImageROI( imgdb->get_actual_image_gray() );
        // eye whiteness definition

        cvSetImageROI( imgdb->get_actual_image(), eyecomprect[i]);
        cvCopy( imgdb->get_actual_image(), snapshots[i].coloreye[0]);


        cvCvtColor( imgdb->get_actual_image(), snapshots[i].hsveye[0], CV_BGR2HSV);
        cvResetImageROI( imgdb->get_actual_image() );
        cvSetImageCOI(snapshots[i].hsveye[0], 2);
        cvCopy( snapshots[i].hsveye[0], snapshots[i].seye[0], 0);

        cvThreshold( snapshots[i].seye[0],snapshots[i].sbineye[0], CFG_SAT_THRESHOLD, CFG_MAX_VAL, CV_THRESH_BINARY_INV);




    }

    return false;

}

bool CEyeTracker::determine_initial_state()
{
    // determine the initial state by measuring the amount of whiteness
    bool found_initial_state = true;

#if(CFG_DBG_LOGFILE)
    std::ofstream* dbgfile = blinkdebug->get_dbgfile_handle();
#endif

    for( int i=besteyeindex_in; i < besteyeindex_out; i++ )
    {
        CvSize imgsize = imgdb->getImageSize();
        int imgarea = imgsize.height* imgsize.width;
        int whitelastsum = cvCountNonZero( snapshots[i].sbineye[SNAPSTATE_LAST]);
        int whiteactsum = cvCountNonZero( snapshots[i].sbineye[SNAPSTATE_ACT]);

        double whitenesslast = whitelastsum/(double)imgarea;
        double whitenessact = whiteactsum/(double)imgarea;
        if( whitenessact > whitenesslast )
        {
            initialwhitesum[i] = whiteactsum;
            initialwhiteness[i] = whitenessact;
            openindex[i] =  SNAPSTATE_ACT;
            closeindex[i] = SNAPSTATE_LAST;

            double whiteratio = whitenesslast/whitenessact;
            if( whiteratio > CFG_MAX_WHITERATIO )
            {
                    found_initial_state = false;
            }
        }
        else if ( whitenessact < whitenesslast )
        {
            initialwhitesum[i] = whitelastsum;
            initialwhiteness[i] = whitenesslast;
            openindex[i] =  SNAPSTATE_LAST;
            closeindex[i] = SNAPSTATE_ACT;
            double whiteratio = whitenessact/whitenesslast;
            if( whiteratio > CFG_MAX_WHITERATIO )
            {
                    found_initial_state = false;
            }

        }
        else
        {
            found_initial_state = false;
        }

        minwhiteness_open[i] = initialwhiteness[i] * CFG_TRACK_OPENMINWHITENESS;
        minwhiteness_close[i] = initialwhiteness[i] * CFG_TRACK_CLOSEMAXWHITENESS;
        opencount[i] = 0;
        closecount[i] = 0;
    }


    return found_initial_state;

}


bool CEyeTracker::take_snapshots()
{
    bool scale_suceeded = true;

    clear_snapshots();


    // initialize the base rects with the initial eye positions
    // and rescale them

    CvSize imgsize = imgdb->getImageSize();
    for( int i= besteyeindex_in; i < besteyeindex_out; i++ )
    {
        eyecomprect[i] = initialeyepos[i];
        snaprect[i] = initialeyepos[i];
        searchwindowrect[i] = initialeyepos[i];


        if(  (rescaleRect(&eyecomprect[i], CFG_SCALEFACT_EYECOMPARE, imgsize)
             && rescaleRect(&snaprect[i], CFG_SCALEFACT_SNAP, imgsize)
             && rescaleRect(&searchwindowrect[i], CFG_SCALEFACT_SEARCHWINDOW, imgsize)
             ) != true )
        {
            scale_suceeded = false;
        }

    }

    // scaling didnt work, throw error
    if( scale_suceeded == false )
    {
        return false;
    }

    IplImage* imagemap[SNAPSTATE_ACT+1];
    IplImage* imagemap_gray[SNAPSTATE_ACT+1];

    imagemap[SNAPSTATE_LAST] = imgdb->get_oldest_image();
    imagemap[SNAPSTATE_ACT] = imgdb->get_actual_image();
    imagemap_gray[SNAPSTATE_LAST] = imgdb->get_oldest_image_gray();
    imagemap_gray[SNAPSTATE_ACT] = imgdb->get_actual_image_gray();

    // do the snapshot
    for( int i= besteyeindex_in; i < besteyeindex_out; i++ )
    {
        for( int j=0; j < SNAPSTATE_ACT+1; j++)
        {
            snapshots[i].coloreye[j] = cvCreateImage( cvSize(eyecomprect[i].width, eyecomprect[i].height), IPL_DEPTH_8U, 3);
            cvSetImageROI( imagemap[j], eyecomprect[i]);
            cvCopy( imagemap[j], snapshots[i].coloreye[j]);


            snapshots[i].hsveye[j] = cvCreateImage( cvSize(eyecomprect[i].width, eyecomprect[i].height), IPL_DEPTH_8U, 3);
            cvCvtColor( imagemap[j], snapshots[i].hsveye[j], CV_BGR2HSV);
            cvResetImageROI( imagemap[j] );

            snapshots[i].seye[j] = cvCreateImage( cvSize(eyecomprect[i].width, eyecomprect[i].height), IPL_DEPTH_8U, 1);
            cvSetImageCOI(snapshots[i].hsveye[j], 2);
            cvCopy( snapshots[i].hsveye[j], snapshots[i].seye[j], 0);

            snapshots[i].sbineye[j] = cvCreateImage( cvSize(eyecomprect[i].width, eyecomprect[i].height), IPL_DEPTH_8U, 1);
            cvThreshold( snapshots[i].seye[j],snapshots[i].sbineye[j], CFG_SAT_THRESHOLD, CFG_MAX_VAL, CV_THRESH_BINARY_INV);

            snapshots[i].colortrack[j] = cvCreateImage( cvSize(snaprect[i].width, snaprect[i].height), IPL_DEPTH_8U, 3);
            cvSetImageROI( imagemap[j], snaprect[i]);
            cvCopy( imagemap[j], snapshots[i].colortrack[j]);
            cvResetImageROI( imagemap[j] );

            snapshots[i].graytrack[j] = cvCreateImage( cvSize(snaprect[i].width, snaprect[i].height), IPL_DEPTH_8U, 1);
            cvSetImageROI( imagemap_gray[j], snaprect[i]);
            cvCopy( imagemap_gray[j], snapshots[i].graytrack[j]);
            cvResetImageROI( imagemap_gray[j] );

        }


        searchwindow[i].color = cvCreateImage( cvSize(searchwindowrect[i].width,searchwindowrect[i].height), IPL_DEPTH_8U, 3 );
        cvSetImageROI( imgdb->get_actual_image(), searchwindowrect[i]);
        cvCopy( imgdb->get_actual_image(), searchwindow[i].color);
        cvResetImageROI( imgdb->get_actual_image() );
        searchwindow[i].gray = cvCreateImage( cvSize(searchwindowrect[i].width,searchwindowrect[i].height), IPL_DEPTH_8U, 1 );
        cvSetImageROI( imgdb->get_actual_image_gray(), searchwindowrect[i]);
        cvCopy( imgdb->get_actual_image_gray(), searchwindow[i].gray);
        cvResetImageROI( imgdb->get_actual_image_gray() );

        corrresult[i] = cvCreateImage( cvSize( searchwindowrect[i].width-snaprect[i].width+1,
                                               searchwindowrect[i].height-snaprect[i].height+1 ), IPL_DEPTH_32F, 1 );
    }

    return true;
}

void CEyeTracker::clear_snapshots()
{
    for( int i=0; i < CFG_NUM_OF_EYES; i++ )
    {
        for(int j=0; j < SNAPSTATE_ACT+1; j++)
        {
            if( snapshots[i].coloreye[j] != NULL )
            {
                cvReleaseImage( &snapshots[i].coloreye[j] );
            }

            if( snapshots[i].hsveye[j] != NULL )
            {
                cvReleaseImage( &snapshots[i].hsveye[j] );
            }

            if( snapshots[i].seye[j] != NULL )
            {
                cvReleaseImage( &snapshots[i].seye[j] );
            }

            if( snapshots[i].sbineye[j] != NULL )
            {
                cvReleaseImage( &snapshots[i].sbineye[j] );
            }



            if( snapshots[i].colortrack[j] != NULL )
            {
                cvReleaseImage( &snapshots[i].colortrack[j] );
            }

            if( snapshots[i].graytrack[j] != NULL )
            {
                cvReleaseImage( &snapshots[i].graytrack[j] );
            }

        }

        if( searchwindow[i].color != NULL )
        {
            cvReleaseImage( &searchwindow[i].color );
        }

        if( searchwindow[i].gray != NULL )
        {
            cvReleaseImage( &searchwindow[i].gray );
        }

        if( corrresult[i] != NULL )
        {
            cvReleaseImage( &corrresult[i] );
        }
    }

    for( int i=0; i < CFG_NUM_OF_EYES; i++ )
    {
        for(int j=0; j < SNAPSTATE_ACT+1; j++)
        {
            snapshots[i].coloreye[j] = NULL;
            snapshots[i].hsveye[j] = NULL;
            snapshots[i].seye[j] = NULL;
            snapshots[i].sbineye[j] = NULL;

            snapshots[i].colortrack[j] = NULL;
            snapshots[i].graytrack[j] = NULL;
        }

        searchwindow[i].color = NULL;
        searchwindow[i].gray = NULL;
        corrresult[i] = NULL;
    }

}





