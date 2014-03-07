/*
 * config.h
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

#ifndef CONFIG_H_
#define CONFIG_H_

// size of image circular buffer and max history distance
#define CFG_IMGBUFFER_SIZE 3

// alpha for calibration mask
#define CFG_MASK_ALPHA 0.5

// threshold for saturation channel
#define CFG_MAX_VAL 255
#define CFG_SAT_THRESHOLD (CFG_MAX_VAL*0.1)

#define CFG_NUM_OF_EYES 2


// ---- eyefinder config -----
#define CFG_FINDER_BINTHRESH (CFG_MAX_VAL*0.15)

#define CFG_FINDER_MINBLOBAREA 0.0025
#define CFG_FINDER_MAXBLOBAREA 0.008

#define CFG_FINDER_MINBLOBCOUNT 2
#define CFG_FINDER_MAXBLOBCOUNT 10

#define CFG_FINDER_MINVALIDITY 2.9

#define CFG_FINDER_BLOB_UP_DISTRATIO 0.32
#define CFG_FINDER_BLOB_MAX_DISTRATIO 0.5
#define CFG_FINDER_BLOB_MIN_DISTRATIO 0.2

#define CFG_FINDER_BLOB_MAX_ANGLE 18
#define CFG_FINDER_BLOB_ASPECT_MIN 1
#define CFG_FINDER_BLOB_ASPECT_MAX 3.5

#define CFG_FINDER_MINDISTANCE 150
#define CFG_FINDER_MAXDISTANCE 320

#define CFG_FINDER_UPPERLOWER_WINDOW 0.10
#define CFG_FINDER_LEFTRIGHT_WINDOW 0.10
// ---

//---- eyetracker config -----
#define CFG_ONLY_BEST 1
#define CFG_SCALEFACT_EYECOMPARE 1.4
#define CFG_SCALEFACT_SNAP 2.1
#define CFG_SCALEFACT_SEARCHWINDOW 2.8

#define CFG_MAX_WHITERATIO 0.65

#define CFG_TRACK_FILTERTIME 2
#define CFG_MIN_CORR_CLOSE 0.55
#define CFG_MIN_CORR_OPEN 0.55
#define CFG_MINPERCENT_OPEN 0.075
#define CFG_MINPERCENT_CLOSE 0.075
#define CFG_MIN_CORR 0.40
#define CFG_FILTER_CLOSEREL 1
#define CFG_TRACK_OPENMINWHITENESS 0.4
#define CFG_TRACK_CLOSEMAXWHITENESS 0.1
// -----


// comm setting
#define CFG_SERVER_ADDR "192.168.2.10"
#define CFG_SERVER_PORT 4242


// feature definitions

#define CFG_CAPTURE_FROM_FILE 0
#define CFG_INPUT_FILE "debug.avi"

#define CFG_DBG_MAINSTATE 0
#define CFG_DBG_LOGFILE   1
#define CFG_DBG_FIND_DIFF 0
#define CFG_DBG_FIND_BIN  0
#define CFG_DBG_HANGFIND  0
#define CFG_DBG_TRACKER   0
#define CFG_DBG_RESULT    1

#define CFG_DBG_HANDSTEP  0
#define CFG_DBG_CORR      0
#define CFG_DBG_TRACKDRAW 1
#define CFG_DBG_BLOBDRAW  1




#endif /* CONFIG_H_ */
