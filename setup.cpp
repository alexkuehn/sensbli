/*
 * setup.cpp
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
#include "setup.h"

setup::setup(QWidget *parent, QSettings *parentsettings)
    : QDialog(parent)
{
	this->settings = parentsettings;
	ui.setupUi(this);



	ui.serverport->setValue( settings->value("system/net/port", 4200 ).toInt() );
	ui.serveraddress->setText( settings->value("system/net/address", "localhost" ).toString() );
	ui.camindex->setValue( settings->value("camera/index", -1 ).toInt() );
	ui.dbgenable->setChecked( settings->value("system/debug", 0).toBool() );
	ui.bnApply->setEnabled( false );

	connect( this, SIGNAL(updatedValues()), parent, SLOT(reinit()));

}

setup::~setup()
{

}

void setup::on_bnCancel_clicked()
{
	// reget old values
	ui.serverport->setValue( settings->value("system/net/port", 4200 ).toInt() );
	ui.serveraddress->setText( settings->value("system/net/address", "localhost" ).toString() );
	ui.camindex->setValue( settings->value("camera/index", -1 ).toInt() );
	ui.dbgenable->setChecked( settings->value("system/debug", 0).toBool() );


	ui.bnApply->setEnabled(false);
	this->close();

}


void setup::on_camindex_valueChanged()
{
	ui.bnApply->setEnabled(true);
}

void setup::on_serverport_valueChanged()
{
	ui.bnApply->setEnabled(true);
}

void setup::on_serveraddress_textChanged()
{
	ui.bnApply->setEnabled(true);
}

void setup::on_dbgenable_toggled()
{
	ui.bnApply->setEnabled(true);
}

void setup::on_bnApply_clicked()
{
	settings->setValue("system/net/port", ui.serverport->value() );
	settings->setValue("system/net/address", ui.serveraddress->text() );
	settings->setValue("camera/index", ui.camindex->value() );
	settings->setValue("system/debug", ui.dbgenable->isChecked() );
	emit updatedValues();
	ui.bnApply->setEnabled(false);
	this->close();
}
