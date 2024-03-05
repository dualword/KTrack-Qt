/* KTrack-Qt (2020-2024) https://github.com/dualword/KTrack-Qt License:GNU GPL*/
/***************************************************************************
                          transponderdefinitiondialog.h  -  description
                             -------------------
    begin                : Sat Mar 22 2003
    copyright            : (C) 2003 by Luc Langehegermann
    email                : lx2gt@users.sourceforge.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef TRANSPONDERDEFINITIONDIALOG_H
#define TRANSPONDERDEFINITIONDIALOG_H

#include "satellite.h"
#include "globals.h"

#include "ui_transponderdefinitionwidget.h"
/**
  *@author Luc Langehegermann
  */

class transponderDefinitionDialog : public QDialog, private Ui_transponderDefinitionWidget {
	Q_OBJECT

public:
	transponderDefinitionDialog(QWidget *parent=0, const char *name=0, bool modal=true, Qt::WFlags fl=Qt::WDestructiveClose);
	~transponderDefinitionDialog();
	void setSatList(PtrSatList*);

private slots:
	void slotNewSat(const QString&);
	void slotDeleteTransponder();
	void slotEditTransponder();
	void slotNewTransponder();

private:
	PtrSatList* satlist;
	satellite* currentSat;

};

#endif
