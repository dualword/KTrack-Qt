/* KTrack-Qt (2020) http://github.com/dualword/KTrack-Qt License:GNU GPL*/
/***************************************************************************
                          satellitelistview.cpp  -  description
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

//#include <klocale.h>
#include <stdio.h>
#include <time.h>

#include "satellitelistview.h"
#include "satellitelistviewitem.h"
#include "satellite.h"

satelliteListView::satelliteListView(QWidget *parent, const char *name) :
	Q3ListView(parent, name) {
  setAllColumnsShowFocus(true);
  addColumn(tr("Satellite"));
  addColumn(tr("Longitude"));
  addColumn(tr("Latitude"));
  addColumn(tr("El"));
  addColumn(tr("Az"));
  addColumn(tr("Next AOS/LOS (UTC)"));
  addColumn(tr("Footprint"));
  addColumn(tr("Height"));
  addColumn(tr("Range"));
  addColumn(tr("Velocity"));
  addColumn(tr("Orbit Number"));
  addColumn(tr("MA"));
  addColumn(tr("Squint"));
  setColumnAlignment(1,Qt::AlignRight);
  setColumnAlignment(2,Qt::AlignRight);
  setColumnAlignment(3,Qt::AlignRight);
  setColumnAlignment(4,Qt::AlignRight);
  setColumnAlignment(6,Qt::AlignRight);
  setColumnAlignment(5,Qt::AlignRight);
  setColumnAlignment(8,Qt::AlignRight);
  setColumnAlignment(7,Qt::AlignRight);
  setColumnAlignment(9,Qt::AlignRight);
  setColumnAlignment(10, Qt::AlignRight);
  setColumnAlignment(11, Qt::AlignRight);
  setColumnAlignment(12, Qt::AlignRight);

  QObject::connect (this, SIGNAL(selectionChanged( Q3ListViewItem*)), this, SLOT (newSelection( Q3ListViewItem*)));
  setMinimumHeight(0);
}

satelliteListView::~satelliteListView()
{
}
/** Sets a list we use to display, and create the List items */
void satelliteListView::setSatList(QList<satellite*>* s){
  clear();  // remove all items
  satlist = s;
  for(auto sat : *satlist) {
    if(sat->polled()) {
      new satelliteListViewItem(this, sat);
    }
  }
}
/** updates the listview */
void satelliteListView::updateListView()
{
  Q3ListViewItemIterator it;
  satelliteListViewItem* item;
  it=Q3ListViewItemIterator(this);
  for(; it.current(); ++it) {
    item=(satelliteListViewItem*)it.current();
    item->update();
  }
}

void satelliteListView::newSelection(Q3ListViewItem* i)
{
  if (!i) return;
  satelliteListViewItem* item = (satelliteListViewItem*)i;
  emit (newTrackingSatellite(item->getSatellite()));
}
