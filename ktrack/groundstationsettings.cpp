/* KTrack-Qt (2020) http://github.com/dualword/KTrack-Qt License:GNU GPL*/
/***************************************************************************
                          groundstationsettings.cpp  -  description
                             -------------------
    begin                : Sat Jun 8 2002
    copyright            : (C) 2002 by Luc Langehegermann
    email                : lx2gt@qsl.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <qlineedit.h>
#include <qpushbutton.h>

#include "groundstationsettings.h"

groundstationSettings::groundstationSettings(QWidget *parent, const char *name, bool modal, Qt::WFlags fl )
    : QDialog(parent,name,modal,fl) {
	setupUi(this);

}
groundstationSettings::~groundstationSettings(){
}
/** sets the observer qth */
void groundstationSettings::setObsQTH(obsQTH* q){
  qth=q;
  longitudeEdit->setText(QString::number(qth->longitude()));
  latitudeEdit->setText(QString::number(qth->latitude()));
  heightEdit->setText(QString::number(qth->height()));
  callsignEdit->setText(qth->callsign());

  QObject::connect(buttonOk, SIGNAL(clicked()), this, SLOT(slotOk()));
  QObject::connect(buttonCancel, SIGNAL(clicked()), this, SLOT(slotCancel()));
}
/** No descriptions */
void groundstationSettings::slotOk(){
  qth->setCallsign(callsignEdit->text());
  qth->setLatitude(latitudeEdit->text().toDouble());
  qth->setLongitude(longitudeEdit->text().toDouble());
  qth->setHeight(heightEdit->text().toDouble());
  accept();
}
/** No descriptions */
void groundstationSettings::slotCancel(){
  reject();
}
