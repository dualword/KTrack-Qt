/* KTrack-Qt (2020) http://github.com/dualword/KTrack-Qt License:GNU GPL*/
/***************************************************************************
                          squintcalculations.cpp  -  description
                             -------------------
    begin                : Thu Aug 29 2002
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

#include "squintcalculations.h"

#include <qcombobox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qpushbutton.h>

squintCalculations::squintCalculations(QWidget *parent, const char *name, Qt::WFlags fl ) :
QDialog(parent,name,fl) {
	setupUi(this);
  QObject::connect(buttonClose, SIGNAL(clicked()), this, SLOT(close()));
  QObject::connect(buttonSave, SIGNAL(clicked()), this, SLOT(slotSave()));
  QObject::connect(satelliteEdit, SIGNAL(activated(const QString&)), this, SLOT(slotNewSat(const QString&)));
  QObject::connect(centerCheckbox, SIGNAL(toggled(bool)), this, SLOT(slotCenter(bool)));
  QObject::connect(enableCheckbox, SIGNAL(toggled(bool)), this, SLOT(slotEnable(bool)));
  QObject::connect(alonEdit, SIGNAL(textChanged(const QString&)), this, SLOT(slotAlonAlatChanged(const QString&)));
  QObject::connect(alatEdit, SIGNAL(textChanged(const QString&)), this, SLOT(slotAlonAlatChanged(const QString&)));
}
squintCalculations::~squintCalculations(){
}
/** No descriptions */
void squintCalculations::setSatList(QList<satellite*>* s){
  satlist = s;
  // fill the combo box
  for(satellite* sat : *satlist) {
    if (sat->polled())
      satelliteEdit->insertItem(sat->name());
  }
  currentsat=satlist->first();
  slotNewSat(satelliteEdit->currentText());
}
/** No descriptions */
void squintCalculations::slotNewSat(const QString & satname){
  // search the satellite class
  for(satellite* sat : *satlist) {
		if (sat->name() == satname) break;
	  // get the values, and toggle the widgets accordingly
	  if(sat->squinttype() == 0) {
		enableCenter(false);
		enableAlonAlat(false);
		enableCheckbox->setChecked(false);
	  }

	  if(sat->squinttype() == 1) {
		enableCenter(true);
		enableAlonAlat(false);
		enableCheckbox->setChecked(true);
		centerCheckbox->setChecked(true);
	  }

	  if(sat->squinttype() == 2) {
		enableCenter(true);
		enableAlonAlat(true);
		enableCheckbox->setChecked(true);
		centerCheckbox->setChecked(false);
		alonEdit->setText(QString::number(sat->ALON(), 'f', 0));
		alatEdit->setText(QString::number(sat->ALAT(), 'f', 0));
	  }
	  buttonSave->setEnabled(false);
	  currentsat=sat;
  }
}

void squintCalculations::enableCenter(bool i){
  centerCheckbox->setEnabled(i);
}

void squintCalculations::enableAlonAlat(bool i){
  alonLabel->setEnabled(i);
  alatLabel->setEnabled(i);
  alonEdit->setEnabled(i);
  alatEdit->setEnabled(i);
}

void squintCalculations::slotCenter(bool i){
  enableAlonAlat(!i);
  buttonSave->setEnabled(true);
}

void squintCalculations::slotEnable(bool i){
  enableCenter(i);
  if(i) centerCheckbox->setChecked(false);
  enableAlonAlat(i);
  buttonSave->setEnabled(true);
}
/** No descriptions */
void squintCalculations::slotAlonAlatChanged(const QString&){
  buttonSave->setEnabled(true);
}
/** save the changed data */
void squintCalculations::slotSave(){
  int squinttype=0;
  double alon;
  double alat;

  // determine squinttype

  if (!enableCheckbox->isChecked()) squinttype=0;
  if (enableCheckbox->isChecked() && centerCheckbox->isChecked()) squinttype=1;
  if (enableCheckbox->isChecked() && !centerCheckbox->isChecked()) squinttype=2;

  alon = alonEdit->text().toFloat();
  alat = alatEdit->text().toFloat();

  currentsat->setSquintData(alon, alat, squinttype);

  buttonSave->setEnabled(false);
}
