/*
 * cblinksensordebugger.h
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
#ifndef CBLINKSENSORDEBUGGER_H_
#define CBLINKSENSORDEBUGGER_H_

#include <fstream>
#include <iostream>

#include <cv.h>
#include "../imagedb/CImageDB.h"
#include "../bloblib/BlobResult.h"
#include "../eyefinder/CBlobPair.h"

#include <QSettings>

class CBlinkSensorDebugger
{
public:
    CBlinkSensorDebugger(CImageDB* img,QSettings* settings);
    virtual ~CBlinkSensorDebugger();

    void resetBlobMarks();
    void buildBlobMarks(CBlobResult* blobs);
    IplImage* getBlobMarks(IplImage* orig);
    void buildPosBlobMarks( CvRect* rects);
    void resetTrackMarks();
    void buildWindowMarks( CvRect* searchwindow,
                           CvRect* eyewindow,
                           CvRect* snaprect, int beginindex, int endindex) ;
  
    void buildTrackMarks( CvPoint* popen, CvPoint* pclose, int beginindex, int endindex );

    void new_log_frame(int state, unsigned long framecnt);
    void post_log_frame(int state);
    void blob_log(CBlobResult* blobs);
    void blob_log_unfiltered(CBlobResult* blobs);
    void blob_filt_log(bool filterednum);
    void blobpair_log( std::vector<CBlobPair *>* pairlist);

    std::ofstream* get_dbgfile_handle();

protected:
    CImageDB* imgdb;

    IplImage* blobmarks;
    IplImage* trackmarks;
    IplImage* temporig;

    std::ofstream dbgfile;

    unsigned long framecounter;
    bool enable_logfile;
};

extern CBlinkSensorDebugger* blinkdebug;

#endif /* CBLINKSENSORDEBUGGER_H_ */
