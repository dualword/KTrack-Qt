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
#include <klistbox.h>
#include <qpushbutton.h>

#include "satelliteselection.h"


satelliteSelection::satelliteSelection(QWidget *parent, const char *name, bool modal, WFlags fl ) : satelliteSelectionBase(parent,name,modal,fl)
{
  // connect the buttons
  QObject::connect(buttonAdd, SIGNAL(clicked()), this, SLOT(slotAdd()));
  QObject::connect(buttonRemove, SIGNAL(clicked()), this, SLOT(slotRemove()));
  QObject::connect(buttonOk, SIGNAL(clicked()), this, SLOT(slotOk()));
  QObject::connect(buttonCancel, SIGNAL(clicked()), this, SLOT(slotCancel()));
}
satelliteSelection::~satelliteSelection()
{
}
/** sets the satellite list of the dialog */
void satelliteSelection::setSatList(QList<satellite> s){
  satellite* sat;
  satlist=s;
  // display the satellites in the listboxes
  for(sat=satlist.first(); sat!=0; sat=satlist.next()) {
    if (sat->polled())
      selectedListBox->insertItem(sat->name());
    else
      availableListBox->insertItem(sat->name());
  }
}
/** No descriptions */
void satelliteSelection::slotAdd(){
  // get the selections and tranfer them to the selected listbox
  unsigned int i;
  QString str;
  for(i=0; i<availableListBox->count(); ++i) {
    if(availableListBox->isSelected(i)) {
      str=availableListBox->text(i);
      selectedListBox->insertItem(str);
      availableListBox->removeItem(i);
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
    if(selectedListBox->isSelected(i)) {
      str=selectedListBox->text(i);
      availableListBox->insertItem(str);
      selectedListBox->removeItem(i);
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

  for(sat=satlist.first(); sat!=0; sat=satlist.next())
    sat->setPolled(false);
  for(i=0; i<selectedListBox->count(); ++i) {
    str=selectedListBox->text(i);
    for(sat=satlist.first(); sat!=0; sat=satlist.next())
      if(sat->name() == str)
        sat->setPolled(true);
  }
  emit polledChanged();
  accept();
}
