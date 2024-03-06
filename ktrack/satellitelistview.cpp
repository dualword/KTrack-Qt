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
//#include "satellitelistviewitem.h"
#include "satellite.h"

satelliteListView::satelliteListView(QWidget *p) : QTableWidget(p) {

	QStringList list;
	list << tr("Satellite") << tr("Longitude") << tr("Latitude") << tr("El") << tr("Az") << tr("Next AOS/LOS (UTC)") << tr("Footprint");
	list << tr("Height") << tr("Range") << tr("Velocity") << tr("Orbit Number") << tr("MA") << tr("Squint");

  QObject::connect (this, SIGNAL(itemChanged( QTableWidgetItem*)), this, SLOT (newSelection( QTableWidgetItem*)));
  setMinimumHeight(0);
}

satelliteListView::~satelliteListView()
{
}

//TODO
/** Sets a list we use to display, and create the List items */
void satelliteListView::setSatList(PtrSatList* s){
  clear();  // remove all items
  satlist = s;
  auto listView = this;
  QTableWidgetItem * item;
  for(auto sat : *satlist) {
    if(sat->polled()) {
    	insertRow(rowCount());
    	int row = rowCount()-1;
        item = new QTableWidgetItem(sat->calculatedDate());
        listView->setItem(row, 0, item);
        item = new QTableWidgetItem(QString::number(sat->longitude(), 'f', 2)+" ");
        listView->setItem(row, 1, item);
        item = new QTableWidgetItem(QString::number(sat->latitude(), 'f', 2)+" ");
        listView->setItem(row, 2, item);
        item = new QTableWidgetItem(QString::number(sat->elevation(), 'f', 2)+" ");
        listView->setItem(row, 3, item);
        item = new QTableWidgetItem(QString::number(sat->azimuth(), 'f', 2)+" ");
        listView->setItem(row, 4, item);
        item = new QTableWidgetItem(QString::number(sat->footprint(), 'f', 1)+" ");
        listView->setItem(row, 5, item);
        item = new QTableWidgetItem(QString::number(sat->altitude(), 'f', 1)+" ");
        listView->setItem(row, 6, item);
        item = new QTableWidgetItem(QString::number(sat->range(), 'f', 1)+" ");
        listView->setItem(row, 7, item);
        item = new QTableWidgetItem(QString::number(sat->velocity(), 'f', 3)+" ");
        listView->setItem(row, 8, item);
        item = new QTableWidgetItem(QString::number(sat->orbitnum())+" ");
        listView->setItem(row, 9, item);
        item = new QTableWidgetItem(QString::number(sat->ma(), 'f', 1)+" ");
        listView->setItem(row, 10, item);

        if (sat->squinttype()>0) {
            item = new QTableWidgetItem(QString::number(sat->squint(), 'f', 1)+" ");
        } else{
          item = new QTableWidgetItem("--.- ");
        }
        listView->setItem(row, 11, item);
    }
  }
}

//TODO
/** updates the listview */
void satelliteListView::updateListView()
{
//  Q3ListViewItemIterator it;
//  satelliteListViewItem* item;
//  it=Q3ListViewItemIterator(this);
//  for(; it.current(); ++it) {
//    item=(satelliteListViewItem*)it.current();
//    item->update();
//  }
}

void satelliteListView::newSelection(QTableWidgetItem* i)
{
  if (!i) return;
//  satelliteListViewItem* item = (satelliteListViewItem*)i; //TODO
//  emit (newTrackingSatellite(item->getSatellite()));
}
