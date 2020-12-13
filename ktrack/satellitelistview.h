/***************************************************************************
                          satellitelistview.h  -  description
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

#ifndef SATELLITELISTVIEW_H
#define SATELLITELISTVIEW_H

#include <qwidget.h>
#include <qlistview.h>
#include <qlist.h>

#include "globals.h"
#include "satellite.h"

/**This widget displays a list of all polled widgets
  *@author Luc Langehegermann
  */

class satelliteListView : public QListView  {
   Q_OBJECT
public: 
	satelliteListView(QWidget *parent=0, const char *name=0);
	~satelliteListView();
  /** Sets a list we use to display */
  void setSatList(QList<satellite> s);
private:
  QList<satellite> satlist;
public slots: // Public slots
  /** updates the listview */
  void updateListView();
signals: // Signals
  /** Emitted, when the user wants to track an satellirte */
  void newTrackingSatellite(satellite* sat);
private slots: // Private slots
  /** No descriptions */
  void newSelection(QListViewItem*);
};

#endif
