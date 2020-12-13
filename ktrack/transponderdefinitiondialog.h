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

#include <qwidget.h>
#include <kdialogbase.h>

#include "satellite.h"
#include "globals.h"

#include "transponderdefinitionwidget.h"
/**
  *@author Luc Langehegermann
  */

class transponderDefinitionDialog : public KDialogBase  {
  Q_OBJECT
public:
  transponderDefinitionDialog(QWidget *parent=0, const char *name=0, bool modal=true, const QString &caption=QString::null, int buttonMask=Ok);
  ~transponderDefinitionDialog();
  /** No descriptions */
  void setSatList(QList<satellite>);
private:
  transponderDefinitionWidget* mainwidget;
  QList<satellite> satlist;
  satellite* currentSat;
private slots:
  void slotNewSat(const QString&);
  void slotDeleteTransponder();
  void slotEditTransponder();
  void slotNewTransponder();
};

#endif
