/*
 * sensbli.h
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

#ifndef SENSBLI_H
#define SENSBLI_H

#include <QtGui/QWidget>
#include <QSettings>
#include <QTimer>
#include <QUdpSocket>

#include "ui_sensbli.h"

#include <cv.h>
#include <highgui.h>

#include "QOpenCVWidget.h"
#include "calibview.h"
#include "recording.h"
#include "setup.h"
#include "blinksensor/CFrameProcessor.h"
#include "blinksensor/CCalibProcessor.h"
#include "blinksensor/CBlinkProcessor.h"
#include "blinksensor/CRecordProcessor.h"
#include "imagedb/CImageDB.h"

class sensbli : public QWidget
{
    Q_OBJECT

public:
    sensbli(QWidget *parent = 0);
    ~sensbli();

public slots:
	void reinit();

private slots:
	void updateVision();
	void on_bnQuit_clicked();
	void on_bnCalib_clicked();
	void on_bnSetup_clicked();
	void on_bnRecord_clicked();
	void broadcastMessage(QByteArray data);
	void getFileText(QString txt);

private:
    Ui::sensbliClass ui;
    CvCapture *camera;
    QTimer *visiontimer;
    IplImage *recvimg;
    QOpenCVWidget *cvout;
    unsigned long framecounter;
    CImageDB* imgdb;
    CFrameProcessor* frameprocessor;

    QImage liveimg;

    QSettings *settings;
    bool app_is_configured;
    bool calibration_mode;
    bool recording_mode;

    void initCapture();
    QImage genImage(IplImage *cvimage);

    CalibView *calibview;
    setup *setupdlg;
    Recording *recorddlg;

    QUdpSocket *udpsocket;
};


#endif // SENSBLI_H
