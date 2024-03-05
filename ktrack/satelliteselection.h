/* KTrack-Qt (2020-2024) https://github.com/dualword/KTrack-Qt License:GNU GPL*/
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

#include "globals.h"
#include "ui_satelliteSelectionBase.h"
#include "satellite.h"

/**
  *@author Luc Langehegermann
  */

class satelliteSelection : public QDialog, private Ui::satelliteSelectionBase {
	Q_OBJECT

public: 
	satelliteSelection(QWidget *parent=0, const char *name=0,bool modal=false, Qt::WFlags fl=Qt::WDestructiveClose );
	~satelliteSelection();
	/** sets the satellite list of the dialog */
	void setSatList(PtrSatList* s);

signals: // Signals
	/** We emit this signal, when the number of polled satellites has changed. Important for satelliteListView */
	void polledChanged();

private slots: // Private slots
	void slotOk();
	void slotCancel();
	void slotRemove();
	void slotAdd();

private:
	PtrSatList* satlist;

};

#endif
