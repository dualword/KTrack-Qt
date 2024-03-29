/* KTrack-Qt (2020) http://github.com/dualword/KTrack-Qt License:GNU GPL*/
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

#include <ui_groundstationsettingsbase.h>

#include "globals.h"

/**configuration dialog for the ground station
  *@author Luc Langehegermann
  */

class groundstationSettings : public QDialog, private Ui::groundstationSettingsBase  {
   Q_OBJECT

public: 
	groundstationSettings(QWidget *p=0,  Qt::WindowFlags f = 0);
	~groundstationSettings();
	/** sets the observer qth */
	void setObsQTH(obsQTH* q);

private slots: // Private slots
	void slotCancel();
	void slotOk();

private:
	obsQTH* qth;

};

#endif
