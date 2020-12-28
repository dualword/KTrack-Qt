/* KTrack-Qt (2020) http://github.com/dualword/KTrack-Qt License:GNU GPL*/
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


#include <QWidget>
#include <Q3PtrList>
#include <ui_squintcalculationsbase.h>

#include "satellite.h"

/**
  *@author Luc Langehegermann
  */

class squintCalculations : public QDialog, Ui::squintCalculationsBase {
	Q_OBJECT

public: 
	squintCalculations(QWidget *parent=0, const char *name=0, Qt::WFlags fl=Qt::WDestructiveClose);
	~squintCalculations();
	void setSatList(PtrSatList* s);

public slots: // Public slots
	void slotAlonAlatChanged(const QString&);

private slots: // Private slots
	void slotNewSat(const QString &);
	void slotCenter(bool);
	void slotEnable(bool);
	/** save the changed data */
	void slotSave();

private:
	void enableAlonAlat(bool);
	void enableCenter(bool);

	satellite* currentsat;
	PtrSatList* satlist;

};

#endif
