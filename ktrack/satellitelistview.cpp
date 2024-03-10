/* KTrack-Qt (2020-2024) https://github.com/dualword/KTrack-Qt License:GNU GPL*/
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

#include <stdio.h>
#include <time.h>

#include "satellitelistview.h"
//#include "satellitelistviewitem.h"
#include "satellite.h"

satelliteListView::satelliteListView(QWidget *p) : QTableWidget(p) {
	QStringList list;
	list << tr("Satellite") << tr("Longitude") << tr("Latitude") << tr("El") << tr("Az") << tr("Next AOS/LOS (UTC)") << tr("Footprint");
	list << tr("Height") << tr("Range") << tr("Velocity") << tr("Orbit Number") << tr("MA") << tr("Squint");
	setColumnCount(list.size());
	setHorizontalHeaderLabels(list);
	setSelectionBehavior(QAbstractItemView::SelectRows);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setEditTriggers(QAbstractItemView::NoEditTriggers);
	QObject::connect (this, SIGNAL(itemChanged( QTableWidgetItem*)), this, SLOT (newSelection( QTableWidgetItem*)));
	QObject::connect (this, SIGNAL(itemClicked( QTableWidgetItem*)), this, SLOT (newSelection( QTableWidgetItem*)));
	setMinimumHeight(0);
}

satelliteListView::~satelliteListView()
{
}

/** Sets a list we use to display, and create the List items */
void satelliteListView::setSatList(PtrSatList* s){
	clearContents();
	setRowCount(0);
	satlist = s;
  for(auto sat : *satlist) {
    if(sat->polled()) {
    	this->sat = sat;
    	insertRow(rowCount());
    	int row = rowCount()-1;
        setItem(row, 0, new QTableWidgetItem(sat->name()));
        setItem(row, 1, new QTableWidgetItem(QString::number(sat->longitude(), 'f', 2)+" "));
        setItem(row, 2, new QTableWidgetItem(QString::number(sat->latitude(), 'f', 2)+" "));
        setItem(row, 3, new QTableWidgetItem(QString::number(sat->elevation(), 'f', 2)+" "));
        setItem(row, 4, new QTableWidgetItem(QString::number(sat->azimuth(), 'f', 2)+" "));
        setItem(row, 5, new QTableWidgetItem(sat->nextAosLos()));
        setItem(row, 6, new QTableWidgetItem(QString::number(sat->footprint(), 'f', 1)+" km "));
        setItem(row, 7, new QTableWidgetItem(QString::number(sat->altitude(), 'f', 1)+" km "));
        setItem(row, 8, new QTableWidgetItem(QString::number(sat->range(), 'f', 1)+" km "));
        setItem(row, 9, new QTableWidgetItem(QString::number(sat->velocity(), 'f', 3)+" km/s "));
        setItem(row, 10, new QTableWidgetItem(QString::number(sat->orbitnum())+" "));
        setItem(row, 11, new QTableWidgetItem(QString::number(sat->ma(), 'f', 1)+" "));

        QTableWidgetItem * item;
        if (sat->squinttype()>0) {
            item = new QTableWidgetItem(QString::number(sat->squint(), 'f', 1)+" ");
        } else{
          item = new QTableWidgetItem("--.- ");
        }
        setItem(row, 12, item);
    }
  }
}

/** updates the listview */
void satelliteListView::updateListView()
{
	int i=0;
	for(auto sat : *satlist) {
	    if(sat->polled()) {
		item(i, 0)->setText(sat->name());
		item(i, 1)->setText(QString::number(sat->longitude(), 'f', 2)+" ");
		item(i, 2)->setText(QString::number(sat->latitude(), 'f', 2)+" ");
		item(i, 3)->setText(QString::number(sat->elevation(), 'f', 2)+" ");
		item(i, 4)->setText(QString::number(sat->azimuth(), 'f', 2)+" ");
		if (sat->nextAosLos() != "") item(i, 5)->setText(sat->nextAosLos());
		item(i, 6)->setText(QString::number(sat->footprint(), 'f', 1)+" km ");
		item(i, 7)->setText(QString::number(sat->altitude(), 'f', 1)+" km ");
		item(i, 8)->setText(QString::number(sat->range(), 'f', 1)+" km ");
		item(i, 9)->setText(QString::number(sat->velocity(), 'f', 3)+" km/s ");
		item(i, 10)->setText(QString::number(sat->orbitnum())+" ");
		item(i, 11)->setText(QString::number(sat->ma(), 'f', 1)+" ");
		if (sat->squinttype()>0) {
			item(i, 12)->setText(QString::number(sat->squint(), 'f', 1)+" ");
		} else{
		  item(i, 12)->setText("--.- ");
		}
	    i++;
	  }
   }
}

void satelliteListView::newSelection(QTableWidgetItem* i)
{
  if (!i || !sat || !satlist || !item(currentRow(),0)) return;
  for(auto sat : *satlist) {
    if(sat->name() == item(currentRow(),0)->text()) {
    	this->sat = sat;
		emit (newTrackingSatellite(this->sat));
		break;
    }
  }
}
