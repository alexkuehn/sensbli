/*
 * ceyestatehandler.h
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
#ifndef CEYESTATEHANDLER_H_
#define CEYESTATEHANDLER_H_

//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <netdb.h>
#include <errno.h>
#include <fstream>
#include <iostream>

#include <QObject>

#include "../config.h"
#define CFG_MSGBUFFER_SIZE 1024

#define SIG_UP "UP"
#define SIG_DOWN "DN"
#define SIG_CLOSE "EC"
#define SIG_OPEN "EO"
#define SIG_LOST "TL"



class CEyeStateHandler : public QObject
{

	Q_OBJECT
public:
    CEyeStateHandler();
    virtual ~CEyeStateHandler();

    void init();
    void connectionEstablished();
    void process( bool loststate, bool* closestates, int beginindex, int endindex );
    bool getState();

signals:
	void sendMessage(QByteArray msg);

protected:
    void send_state();

    char strbuf[CFG_MSGBUFFER_SIZE];
    int s, rc, i;
//    struct hostent *h;
//    struct sockaddr_in cliAddr, remoteServAddr;


    bool lastlost;

    bool laststate;


};

#endif /* CEYETRACKER_H_ */
