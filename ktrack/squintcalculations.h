/***************************************************************************
                          squintcalculations.h  -  description
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

#ifndef SQUINTCALCULATIONS_H
#define SQUINTCALCULATIONS_H

#include <qwidget.h>
#include <qptrlist.h>
#include <squintcalculationsbase.h>

#include "satellite.h"

/**
  *@author Luc Langehegermann
  */

class squintCalculations : public squintCalculationsBase  {
   Q_OBJECT
public: 
	squintCalculations(QWidget *parent=0, const char *name=0,WFlags fl=WDestructiveClose);
	~squintCalculations();
  /** No descriptions */
  void setSatList(QList<satellite> s);
private:
  QList<satellite> satlist;
  void enableAlonAlat(bool);
  void enableCenter(bool);
  satellite* currentsat;
private slots: // Private slots
  /** No descriptions */
  void slotNewSat(const QString &);
  /** No descriptions */
  void slotCenter(bool);
  /** No descriptions */
  void slotEnable(bool);
  /** save the changed data */
  void slotSave();
public slots: // Public slots
  /** No descriptions */
  void slotAlonAlatChanged(const QString&);
};

#endif
