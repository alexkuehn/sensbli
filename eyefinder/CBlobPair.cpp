/*
 * cblobpair.cpp
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
#include "CBlobPair.h"
#include "../config.h"

#define PI 3.14159268

CBlobPair::CBlobPair( int index1, CBlob* blob1, int index2, CBlob* blob2, ovroi* actovroi )
{
    // initialize the members with parameters
    this->index1 = index1;
    this->blob1 = blob1;
    this->index2 = index2;
    this->blob2 = blob2;
    this->centerroi = actovroi;

    overallvalid = false;
    validity = 0;
}

CBlobPair::~CBlobPair()
{
    // TODO Auto-generated destructor stub
}

void CBlobPair::calculateProperties()
{
    // calculate differences
    diffx1 = blob1->MaxX() - blob1->MinX();
    diffy1 = blob1->MaxY() - blob1->MinY();

    diffx2 = blob2->MaxX() - blob2->MinX();
    diffy2 = blob2->MaxY() - blob2->MinY();

    // calculate the centers
    centerx1 = (diffx1/2.0)+blob1->MinX();
    centery1 = (diffy1/2.0)+blob1->MinY();

    centerx2 = (diffx2/2.0)+blob2->MinX();
    centery2 = (diffy2/2.0)+blob2->MinY();

    // calculate the aspects
    aspect1 = diffx1/diffy1;
    aspect2 = diffx2/diffy2;

    // calculate distances
    xdelta = fabs(centerx1-centerx2);
    ydelta = fabs(centery1-centery2);

    //calculate distance between blobs
    distance = sqrt( (xdelta*xdelta) + (ydelta*ydelta));
    // calculate angle between blobs
    distangle = asin(ydelta/distance)*180/PI;
    // calculate normalized widths
    normwidth1 = diffx1/distance;
    normwidth2 = diffx2/distance;

    basenormwidth1 = 0;
    basenormwidth2 = 0;

    const double biaslower = 1/(CFG_FINDER_BLOB_UP_DISTRATIO - CFG_FINDER_BLOB_MIN_DISTRATIO);
    const double biasupper = -1/(CFG_FINDER_BLOB_MAX_DISTRATIO-CFG_FINDER_BLOB_UP_DISTRATIO);
    const double offslower = -CFG_FINDER_BLOB_MIN_DISTRATIO*biaslower;
    const double offsupper = -CFG_FINDER_BLOB_MAX_DISTRATIO*biasupper;

    if( normwidth1 < CFG_FINDER_BLOB_UP_DISTRATIO)
    {
        if( normwidth1 >= CFG_FINDER_BLOB_MIN_DISTRATIO)
        {
            basenormwidth1 = normwidth1*biaslower+offslower;
        }
    }
    if( normwidth1 >= CFG_FINDER_BLOB_UP_DISTRATIO)
    {
        if( normwidth1 <= CFG_FINDER_BLOB_MAX_DISTRATIO)
        {
            basenormwidth1 = normwidth1*biasupper+offsupper;
        }
    }
    if( normwidth2 < CFG_FINDER_BLOB_UP_DISTRATIO)
    {
        if( normwidth2 >= CFG_FINDER_BLOB_MIN_DISTRATIO)
        {
            basenormwidth2 = normwidth2*biaslower+offslower;
        }
    }
    if( normwidth2 >= CFG_FINDER_BLOB_UP_DISTRATIO)
    {
        if( normwidth2 <= CFG_FINDER_BLOB_MAX_DISTRATIO)
        {
            basenormwidth2 = normwidth2*biasupper+offsupper;
        }
    }

    // build aspectratio between 0 and 1 (1=aspects similiar)
    aspectratio = 0;
    if( aspect1 > aspect2 )
    {
        aspectratio = aspect2/aspect1;
    }
    else
    {
        aspectratio = aspect1/aspect2;
    }


    // build arearatio between 0 and 1 (1= Area are similiar)
    arearatio = 0;
    if( blob1->Area() > blob2->Area() )
    {
        arearatio = blob2->Area()/blob1->Area();
    }
    else
    {
        arearatio = blob1->Area()/blob2->Area();
    }

    // check if pair is overall valid
    if(
            // check if centers are within overall roi
            ( (centerx1 < centerroi->right ) && (centerx1 > centerroi->left)  ) &&
            ( (centery1 < centerroi->lower) && (centery1 > centerroi->upper) ) &&
            ( (centerx2 < centerroi->right) && (centerx2 > centerroi->left)  ) &&
            ( (centery2 < centerroi->lower) && (centery2 > centerroi->upper) ) &&
            ( distangle < CFG_FINDER_BLOB_MAX_ANGLE) &&
            ( distance < CFG_FINDER_MAXDISTANCE) && ( distance > CFG_FINDER_MINDISTANCE ) &&
            ( (aspect1 > CFG_FINDER_BLOB_ASPECT_MIN)  && (aspect1 < CFG_FINDER_BLOB_ASPECT_MAX)) &&
            ( (aspect2 > CFG_FINDER_BLOB_ASPECT_MIN)  && (aspect2 < CFG_FINDER_BLOB_ASPECT_MAX))
      )
    {

        overallvalid = true;
    }
    else
    {
        overallvalid = false;
    }

    // calculate overall validity factor
    if( overallvalid == false )
    {
        // if overall valid is not met -> set validity to lowest
        validity = 0;
    }
    else
    {
        validity = arearatio+aspectratio+basenormwidth1+basenormwidth2;

    }

}

double CBlobPair::getValidity( void )
{
    return validity;
}
int CBlobPair::getIndex1( void )
{
    return index1;
}
int CBlobPair::getIndex2( void )
{
    return index2;
}
