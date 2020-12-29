/* KTrack-Qt (2020) http://github.com/dualword/KTrack-Qt License:GNU GPL*/
/***************************************************************************
                 transponderdefinitiondetailsdialog.h  -  description
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

#ifndef TRANSPONDERDEFINITIONDETAILSDIALOG_H
#define TRANSPONDERDEFINITIONDETAILSDIALOG_H

#include "globals.h"
#include "ui_transponderdefinitiondetailswidget.h"

/**
  *@author Luc Langehegermann
  */

class transponderDefinitionDetailsDialog : public QDialog, private Ui_transponderDefinitionDetailsWidget {
	Q_OBJECT

public:
	transponderDefinitionDetailsDialog(transponder* t=0, QWidget *parent=0, const char *name=0, bool modal=true, Qt::WFlags fl=Qt::WDestructiveClose);
	~transponderDefinitionDetailsDialog();

private:
	void slotOk();
	transponder* trans;
};

#endif
