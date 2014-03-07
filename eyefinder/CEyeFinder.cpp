/*
 * ceyefinder.cpp
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

#include "../blinksensor/CBlinkSensorDebugger.h"
#include "CEyeFinder.h"
#include "CBlobPair.h"

CEyeFinder::CEyeFinder(CImageDB* img)
{
    CvSize imgsize;
    imgdb = img;

    imgsize = imgdb->getImageSize();

    foundstate = false;

    framediff     = NULL;
    framediff_bin = NULL;
    framediff_morph = NULL;

    morph_strel = NULL;

    framediff       = cvCreateImage( imgsize, IPL_DEPTH_8U, 1);
    framediff_bin   = cvCreateImage( imgsize, IPL_DEPTH_8U, 1);
    framediff_morph = cvCreateImage( imgsize, IPL_DEPTH_8U, 1);

    morph_strel = cvCreateStructuringElementEx(5,5,2,2,CV_SHAPE_ELLIPSE);

    imgarea = imgsize.width * imgsize.height;
    minblobarea = imgarea * CFG_FINDER_MINBLOBAREA;
    maxblobarea = imgarea * CFG_FINDER_MAXBLOBAREA;

    unsigned int upperlowerwnd = (int)((double)imgsize.height * CFG_FINDER_UPPERLOWER_WINDOW);
    unsigned int leftrightwnd = (int)((double)imgsize.width * CFG_FINDER_LEFTRIGHT_WINDOW);

    actual_ovroi.upper = upperlowerwnd;
    actual_ovroi.lower = imgsize.height - upperlowerwnd;
    actual_ovroi.left = leftrightwnd;
    actual_ovroi.right = imgsize.width - leftrightwnd;
}

CEyeFinder::~CEyeFinder()
{

    if( framediff != NULL )
    {
        cvReleaseImage( &framediff );
    }

    if( framediff_bin != NULL )
    {
        cvReleaseImage( &framediff_bin );
    }

    if( framediff_morph != NULL )
    {
        cvReleaseImage( &framediff_morph );
    }


    if( morph_strel != NULL )
    {
        cvReleaseStructuringElement( &morph_strel );
    }

}

bool CEyeFinder::found()
{
    return foundstate;
}

void CEyeFinder::init()
{
    foundstate = false;
}

void CEyeFinder::process()
{
    bool foundblob = false;



    // difference the gray image frames in time flow
    cvAbsDiff( imgdb->get_actual_image_gray(), imgdb->get_oldest_image_gray(), framediff);

    #if(CFG_DBG_FIND_DIFF)
//    cvShowImage( "Find Diff", framediff);
    #endif

    // threshold for binarisation
    cvThreshold( framediff, framediff_bin, CFG_FINDER_BINTHRESH, CFG_MAX_VAL,CV_THRESH_BINARY);

    // morphological open by combination of Erode->Dilate
    cvErode( framediff_bin, framediff_morph, morph_strel, 1);
    cvDilate( framediff_morph, framediff_morph, morph_strel, 1);

    #if(CFG_DBG_FIND_BIN)
//    cvShowImage( "Find Bin", framediff_morph);
    #endif

    foundblob = blobanalysis();

    // we found a blob
    if( foundblob == true )
    {
        foundstate = true;
    }

}

bool CEyeFinder::blobanalysis()
{
    CBlobResult diffblobs;

    bool foundblob = false;

    blobfiltercause = 0;

    // create blob data
    diffblobs = CBlobResult( framediff_morph, NULL, 100, false );
    blinkdebug->blob_log_unfiltered(&diffblobs);
    // prefilter the blobs by area
    diffblobs.Filter( diffblobs, B_INCLUDE, CBlobGetArea(), B_INSIDE, minblobarea, maxblobarea);

    blinkdebug->blob_log(&diffblobs);
    blinkdebug->buildBlobMarks(&diffblobs);

    // filter criteria: number of blobs
    int numblobs = diffblobs.GetNumBlobs();

    if( (numblobs < CFG_FINDER_MINBLOBCOUNT) || (numblobs > CFG_FINDER_MAXBLOBCOUNT))
    {
        blobfiltercause = 1;
        blinkdebug->blob_filt_log( true );
        return foundblob;
    }
    blinkdebug->blob_filt_log( false );

    // analyze the blob pairs
    foundblob = blobpairanalysis( &diffblobs);

    if( foundblob == true )
    {
        blinkdebug->buildPosBlobMarks( this->foundrects);
    }

    return foundblob;
}

bool CEyeFinder::blobpairanalysis(CBlobResult* diffblobs)
{
    bool foundpair = false;

    // build blob pairs
    std::vector<CBlobPair *> blobpairs;

    // iterate through all blobs
    for( int i=0; i < diffblobs->GetNumBlobs(); i++)
    {
        // and iterate over the following blobs after this element to build a pair link
        for( int j=i+1; j < diffblobs->GetNumBlobs(); j++)
        {
            CBlob* blob1 = diffblobs->GetBlob(i);
            CBlob* blob2 = diffblobs->GetBlob(j);

            CBlobPair* blobpair = new CBlobPair( i, blob1, j, blob2, &actual_ovroi );
            // calculate properties and normalize them
            blobpair->calculateProperties();
            blobpairs.push_back( blobpair);
        }
    }

    // find pair with highest validity
    double highestvalidity = 0;
    bool validpair_found = false;
    unsigned int validpair_index[CFG_NUM_OF_EYES];

    for( unsigned int i=0; i < blobpairs.size(); i++)
    {
        CBlobPair* blobpair = blobpairs[i];

        // filter valid blobs
        if( blobpair->getValidity() > CFG_FINDER_MINVALIDITY )
        {
            if( blobpair->getValidity() > highestvalidity)
            {
                highestvalidity = blobpair->getValidity();
                validpair_found = true;
                validpair_index[0] = blobpair->getIndex1();
                validpair_index[1] = blobpair->getIndex2();
            }
        }

    }

    blinkdebug->blobpair_log(&blobpairs);

    // destroy blobpairs
    for( unsigned int i=0; i < blobpairs.size(); i++)
    {
        CBlobPair* blobpair = blobpairs[i];
        delete blobpair;
    }


    // evaluate found pairs
    if( validpair_found == true )
    {
        foundpair = true;

        for( unsigned int i=0; i < CFG_NUM_OF_EYES; i++ )
        {
            CBlob* validblob = diffblobs->GetBlob(validpair_index[i]);
            foundrects[i].x = validblob->MinX();
            foundrects[i].y = validblob->MinY();

            unsigned int diffx = validblob->MaxX() - validblob->MinX();
            unsigned int diffy = validblob->MaxY() - validblob->MinY();

            foundrects[i].width = diffx;
            foundrects[i].height = diffy;

            foundpoints[i].x = validblob->MinX() + (diffx/2.0);
            foundpoints[i].y = validblob->MinY() + (diffy/2.0);
        }
    }
    return foundpair;
}

CvRect* CEyeFinder::getFoundRects()
{
    if( foundstate == true )
    {

        return &foundrects[0];
    }
    else
    {
        return NULL;
    }
}

CvPoint* CEyeFinder::getFoundPoints()
{
    if( foundstate == true )
    {
        return &foundpoints[0];
    }
    else
    {
        return NULL;
    }

}
