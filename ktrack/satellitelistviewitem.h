/***************************************************************************
                          satellitelistviewitem.h  -  description
                             -------------------
    begin                : Tue Jun 18 2002
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

#ifndef SATELLITELISTVIEWITEM_H
#define SATELLITELISTVIEWITEM_H

#include <qlistview.h>
#include "globals.h"
#include "satellite.h"

/**
  *@author Luc Langehegermann
  */

class satelliteListViewItem : public QListViewItem  {
public:
	satelliteListViewItem(QListView* view, satellite* s);
	~satelliteListViewItem();
  /** No descriptions */
  satellite* getSatellite();
  /** No descriptions */
  void update();
private:
  satellite* sat;
private: // Private methods
  /** No descriptions */
  void paintCell ( QPainter * p, const QColorGroup & cg, int column, int width, int align );
};

#endif
