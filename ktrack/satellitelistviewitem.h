/* KTrack-Qt (2020) http://github.com/dualword/KTrack-Qt License:GNU GPL*/
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

#include "globals.h"
#include "satellite.h"

#include <Q3ListViewItem>
/**
  *@author Luc Langehegermann
  */

class satelliteListViewItem : public Q3ListViewItem  {

public:
	satelliteListViewItem(Q3ListView* view, satellite* s);
	~satelliteListViewItem();
	satellite* getSatellite();
	void update();

private: // Private methods
	void paintCell ( QPainter * p, const QColorGroup & cg, int column, int width, int align );

private:
	satellite* sat;

};

#endif
