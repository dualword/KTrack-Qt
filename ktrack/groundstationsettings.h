/***************************************************************************
                          groundstationsettings.h  -  description
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

#ifndef GROUNDSTATIONSETTINGS_H
#define GROUNDSTATIONSETTINGS_H

#include <qwidget.h>
#include <groundstationsettingsbase.h>

#include "globals.h"

/**configuration dialog for the ground station
  *@author Luc Langehegermann
  */

class groundstationSettings : public groundstationSettingsBase  {
   Q_OBJECT
public: 
	groundstationSettings(QWidget *parent=0, const char *name=0,bool modal=true, WFlags fl=WDestructiveClose );
	~groundstationSettings();
  /** sets the observer qth */
  void setObsQTH(obsQTH* q);
private:
  obsQTH* qth;
private slots: // Private slots
  /** No descriptions */
  void slotCancel();
private slots: // Private slots
  /** No descriptions */
  void slotOk();
};

#endif
