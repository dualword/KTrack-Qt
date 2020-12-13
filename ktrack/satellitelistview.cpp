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

#include <klocale.h>
#include <stdio.h>
#include <time.h>

#include "satellitelistview.h"
#include "satellitelistviewitem.h"
#include "satellite.h"

satelliteListView::satelliteListView(QWidget *parent, const char *name ) : QListView(parent,name)
{
  setAllColumnsShowFocus(true);
  addColumn(i18n("Satellite"));
  addColumn(i18n("Longitude"));
  addColumn(i18n("Latitude"));
  addColumn(i18n("El"));
  addColumn(i18n("Az"));
  addColumn(i18n("Next AOS/LOS (UTC)"));
  addColumn(i18n("Footprint"));
  addColumn(i18n("Height"));
  addColumn(i18n("Range"));
  addColumn(i18n("Velocity"));
  addColumn(i18n("Orbit Number"));
  addColumn(i18n("MA"));
  addColumn(i18n("Squint"));
  setColumnAlignment(1,AlignRight);
  setColumnAlignment(2,AlignRight);
  setColumnAlignment(3,AlignRight);
  setColumnAlignment(4,AlignRight);
  setColumnAlignment(6,AlignRight);
  setColumnAlignment(5,AlignRight);
  setColumnAlignment(8,AlignRight);
  setColumnAlignment(7,AlignRight);
  setColumnAlignment(9,AlignRight);
  setColumnAlignment(10, AlignRight);
  setColumnAlignment(11, AlignRight);
  setColumnAlignment(12, AlignRight);

  QObject::connect (this, SIGNAL(selectionChanged(QListViewItem*)), this, SLOT (newSelection(QListViewItem*)));
  setMinimumHeight(0);
}

satelliteListView::~satelliteListView()
{
}
/** Sets a list we use to display, and create the List items */
void satelliteListView::setSatList(QList<satellite> s){
  clear();  // remove all items
  satlist=s;
  satellite* sat;
  for(sat=satlist.first(); sat!=0; sat=satlist.next()) {
    if(sat->polled()) {
      new satelliteListViewItem(this, sat);
    }
  }
}
/** updates the listview */
void satelliteListView::updateListView()
{
  QListViewItemIterator it;
  satelliteListViewItem* item;

  it=QListViewItemIterator(this);
  for(; it.current(); ++it) {
    item=(satelliteListViewItem*)it.current();
    item->update();
  }
}

void satelliteListView::newSelection(QListViewItem* i)
{
  if (!i) return;
  satelliteListViewItem* item = (satelliteListViewItem*)i;
  emit (newTrackingSatellite(item->getSatellite()));
}
