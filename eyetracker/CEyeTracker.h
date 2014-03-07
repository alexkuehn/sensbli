/*
 * ceyetracker.h
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
#ifndef CEYETRACKER_H_
#define CEYETRACKER_H_

#include <cv.h>

#include "../imagedb/CImageDB.h"
#include "../eyefinder/CEyeFinder.h"


#define SNAPSTATE_LAST 0
#define SNAPSTATE_ACT  1

typedef struct
{
    IplImage* coloreye[2];
    IplImage* hsveye[2];
    IplImage* seye[2];
    IplImage* sbineye[2];

    IplImage* colortrack[2];
    IplImage* graytrack[2];

    IplImage* colorwindow[2];
    IplImage* graywindow[2];

} snapshot_t;

typedef struct
{
    IplImage* color;
    IplImage* gray;
} searchwindow_t;

class CEyeTracker
{
public:
    CEyeTracker(CImageDB* img, CEyeFinder *finder);
    virtual ~CEyeTracker();

    void init();
    void process();
    bool lost();

    bool* get_state();
    int getBesteyeindex_in() const
    {
        return besteyeindex_in;
    }

    int getBesteyeindex_out() const
    {
        return besteyeindex_out;
    }

protected:

    void determine_best_eye();

    bool take_snapshots();
    void clear_snapshots();
    bool determine_initial_state();

    bool update_searchwindow();
    bool correlate_mask();
    bool determine_correlation();
    bool determine_whiteness();
    bool calc_state();

    CImageDB* imgdb;
    CEyeFinder* eyefinder;

    bool statelost;


    CvRect* initialeyepos;
    CvPoint* initialeyecenter;

    CvRect eyecomprect[CFG_NUM_OF_EYES];
    CvRect snaprect[CFG_NUM_OF_EYES];
    CvRect searchwindowrect[CFG_NUM_OF_EYES];

    snapshot_t snapshots[CFG_NUM_OF_EYES];
    searchwindow_t searchwindow[CFG_NUM_OF_EYES];

    double initialwhitesum[CFG_NUM_OF_EYES];
    double initialwhiteness[CFG_NUM_OF_EYES];
    int openindex[CFG_NUM_OF_EYES];
    int closeindex[CFG_NUM_OF_EYES];


    int besteyeindex_in;
    int besteyeindex_out;

    IplImage* corrresult[CFG_NUM_OF_EYES];
    CvPoint minpoint[CFG_NUM_OF_EYES][SNAPSTATE_ACT+1];
    CvPoint maxpoint[CFG_NUM_OF_EYES][SNAPSTATE_ACT+1];

    double minval[CFG_NUM_OF_EYES][SNAPSTATE_ACT+1];
    double maxval[CFG_NUM_OF_EYES][SNAPSTATE_ACT+1];

    double whiteness[CFG_NUM_OF_EYES];


    bool isclosed[CFG_NUM_OF_EYES];

    unsigned int opencount[CFG_NUM_OF_EYES];
    unsigned int closecount[CFG_NUM_OF_EYES];

    double minwhiteness_open[CFG_NUM_OF_EYES];
    double minwhiteness_close[CFG_NUM_OF_EYES];

};

#endif /* CEYETRACKER_H_ */
