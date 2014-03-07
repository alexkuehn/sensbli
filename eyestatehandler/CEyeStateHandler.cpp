/*
 * ceyestatehandler.cpp
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
#include "CEyeStateHandler.h"

#include <iostream>
#include <stdlib.h>
#include <string.h>



CEyeStateHandler::CEyeStateHandler()
{
    // h = gethostbyname( CFG_SERVER_ADDR );

    // if( h == NULL )
    // {
	//        std::cerr << "ERR: network host unknown" << std::endl;
    //    exit( -1 );
    // }


//    remoteServAddr.sin_family = h->h_addrtype;
//	memcpy ( (char *) &remoteServAddr.sin_addr.s_addr,
//	           h->h_addr_list[0], h->h_length);
//	remoteServAddr.sin_port = htons (CFG_SERVER_PORT);
//
//
//    // create socket
//	s = socket (AF_INET, SOCK_DGRAM, 0);
//	if (s < 0)
//	{
//         std::cerr << "ERR: socket port invalid" << std::endl;
//	     exit (-1);
//	}
//
//
//    // bind port
//	cliAddr.sin_family = AF_INET;
//	cliAddr.sin_addr.s_addr = htonl (INADDR_ANY);
//	cliAddr.sin_port = htons (0);
//	rc = bind (s, (struct sockaddr *) &cliAddr, sizeof (cliAddr));
//	if (rc < 0)
//	{
//	     std::cerr << "ERR: cant bind port" << std::endl;
//	     exit (-1);
//	}
//
//#if(0)
//    sendto (s, SIG_UP, strlen (SIG_UP) + 1, 0,
//   			                 (struct sockaddr *) &remoteServAddr,
//    				                 sizeof (remoteServAddr));
//#endif

    lastlost = true;
    laststate = false;

}

CEyeStateHandler::~CEyeStateHandler()
{
	QByteArray msg = SIG_DOWN;

	emit sendMessage(msg);
#if(0)
        sendto (s, SIG_DOWN, strlen (SIG_DOWN) + 1, 0,
   			                 (struct sockaddr *) &remoteServAddr,
    				                 sizeof (remoteServAddr));
#endif
//        close( s );

}

void CEyeStateHandler::connectionEstablished()
{
	QByteArray msg = SIG_UP;
    emit sendMessage(msg);
}

void CEyeStateHandler::init()
{
}

void CEyeStateHandler::send_state()
{
       if( laststate == true )
       {
    		QByteArray msg = SIG_CLOSE;

    	    emit sendMessage(msg);

       }
       else
       {
   		QByteArray msg = SIG_OPEN;

   	    emit sendMessage(msg);
       }

}

void CEyeStateHandler::process( bool loststate, bool* closestates, int beginindex, int endindex )
{
        if( loststate != lastlost )
        {
                lastlost = loststate;

                if( loststate == true )
                {
               		QByteArray msg = SIG_LOST;

               	    emit sendMessage(msg);
                }
                else
                {
                    send_state();
                }
        }


        bool close = true;

        for( int i=beginindex; i < endindex; i++ )
        {
                if( closestates[i] == false )
                {
                        close = false;
                }
        }


        if( close != laststate )
        {
                laststate = close;
                send_state();
        }
}


bool CEyeStateHandler::getState()
{
	return laststate;
}

