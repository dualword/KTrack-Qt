/* KTrack-Qt (2020-2024) https://github.com/dualword/KTrack-Qt License:GNU GPL*/
/***************************************************************************
                          satelliteselection.cpp  -  description
                             -------------------
    begin                : Wed May 1 2002
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

#include <stdio.h>

#include "satelliteselection.h"

satelliteSelection::satelliteSelection(QWidget *p, Qt::WindowFlags fl ) : QDialog(p,fl)
{
	setupUi(this);
  QObject::connect(buttonAdd, SIGNAL(clicked()), this, SLOT(slotAdd()));
  QObject::connect(buttonRemove, SIGNAL(clicked()), this, SLOT(slotRemove()));
  QObject::connect(buttonOk, SIGNAL(clicked()), this, SLOT(slotOk()));
  QObject::connect(buttonCancel, SIGNAL(clicked()), this, SLOT(slotCancel()));
}
satelliteSelection::~satelliteSelection()
{
}
/** sets the satellite list of the dialog */
void satelliteSelection::setSatList(PtrSatList* s){
  satlist = s;
  // display the satellites in the listboxes
  for (int i=0; i<s->size(); i++){
    if (s->at(i)->polled())
      selectedListBox->insertItem(i,s->at(i)->name());
    else
      availableListBox->insertItem(i,s->at(i)->name());
  }
}
/** No descriptions */
void satelliteSelection::slotAdd(){
  // get the selections and tranfer them to the selected listbox
  unsigned int i;
  QString str;
  for(i=0; i<availableListBox->count(); ++i) {
    if(availableListBox->item(i)->isSelected()) {
      str=availableListBox->item(i)->text();
      selectedListBox->addItem(str);
      QListWidgetItem * item = availableListBox->takeItem(i);
      delete item;
      i--;
    }
  }
}
/** No descriptions */
void satelliteSelection::slotRemove(){
  // get the selections and tranfer them to the selected listbox
  unsigned int i;
  QString str;
  for(i=0; i<selectedListBox->count(); ++i) {
    if(selectedListBox->item(i)->isSelected()) {
      str=selectedListBox->item(i)->text();
      availableListBox->addItem(str);
      QListWidgetItem * item = selectedListBox->takeItem(i);
      delete item;
      i--;
    }
  }
}
/** No descriptions */
void satelliteSelection::slotCancel(){
  reject();
}
/** No descriptions */
void satelliteSelection::slotOk(){
  satellite* sat;
  unsigned int i;
  QString str;
  // clear the polled flag of the sat list

  for(satellite* sat: *satlist){
	    sat->setPolled(false);
  }

  for(i=0; i<selectedListBox->count(); ++i) {
    str = selectedListBox->item(i)->text();

    for(satellite* sat: *satlist){
        if(sat->name() == str){
            sat->setPolled(true);
        }
    }
  }
  emit polledChanged();
  accept();
}
