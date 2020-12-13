/***************************************************************************
                          satelliteselection.h  -  description
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

#ifndef SATELLITESELECTION_H
#define SATELLITESELECTION_H

#include <qwidget.h>
#include <qlist.h>

#include "globals.h"
#include "satelliteSelectionBase.h"
#include "satellite.h"

/**
  *@author Luc Langehegermann
  */

class satelliteSelection : public satelliteSelectionBase  {
   Q_OBJECT
public: 
	satelliteSelection(QWidget *parent=0, const char *name=0,bool modal=false, WFlags fl=WDestructiveClose );
	~satelliteSelection();
  /** sets the satellite list of the dialog */
  void setSatList(QList<satellite> s);
private:
  QList<satellite> satlist;
private slots: // Private slots
  /** No descriptions */
  void slotOk();
private slots: // Private slots
  /** No descriptions */
  void slotCancel();
private slots: // Private slots
  /** No descriptions */
  void slotRemove();
private slots: // Private slots
  /** No descriptions */
  void slotAdd();
signals: // Signals
  /** We emit this signal, when the number of polled satellites has changed. Important for satelliteListView */
  void polledChanged();
};

#endif
