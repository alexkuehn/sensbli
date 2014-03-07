/*
 * cblobpair.h
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
#ifndef CBLOBPAIR_H_
#define CBLOBPAIR_H_

#include "../bloblib/Blob.h"
#include "ovroi.h"

class CBlobPair
{
    friend class CBlinkSensorDebugger;
public:
    CBlobPair(int index1, CBlob* blob1, int index2, CBlob* blob2, ovroi* actovroi);
    virtual ~CBlobPair();

    void calculateProperties();
    double getValidity( void );
    int getIndex1( void );
    int getIndex2( void );

protected:
    unsigned int index1; // index of first pair element
    unsigned int index2; // index of second pair element
    CBlob* blob1;   // pointer to first pair element
    CBlob* blob2;   // pointer to second pair element
    ovroi* centerroi; // pointer to valid center roi coordinates

    bool overallvalid;  // overall validity of this pair
    double validity;    // factor that describes validity


    double diffx1;
    double diffy1;
    double diffx2;
    double diffy2;
    double centerx1;
    double centery1;
    double centerx2;
    double centery2;
    double aspect1;
    double aspect2;
    double xdelta;
    double ydelta;
    double distance;
    double distangle;
    double normwidth1;
    double normwidth2;
    double basenormwidth1;
    double basenormwidth2;
    double aspectratio;
    double arearatio;

};

#endif /* CBLOBPAIR_H_ */
