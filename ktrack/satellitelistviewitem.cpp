/***************************************************************************
                          satellitelistviewitem.cpp  -  description
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

#include "satellitelistviewitem.h"
#include <stdio.h>
#include <qpalette.h>

satelliteListViewItem::satelliteListViewItem(QListView* view, satellite* s) : QListViewItem(view) {
  sat=s;
  setText(0, sat->name());
  update(); // update the view
}
satelliteListViewItem::~satelliteListViewItem(){
}
/** No descriptions */
void satelliteListViewItem::update(){
  setText(1, QString::number(sat->longitude(), 'f', 2)+" ");
  setText(2, QString::number(sat->latitude(), 'f', 2)+" ");
  setText(3, QString::number(sat->elevation(), 'f', 2)+"° ");
  setText(4, QString::number(sat->azimuth(), 'f', 2)+"° ");
  if (sat->nextAosLos() != "")
    setText(5, sat->nextAosLos());
  setText(6, QString::number(sat->footprint(), 'f', 0) +" km ");
  setText(7, QString::number(sat->altitude(), 'f', 0) +" km ");
  setText(8, QString::number(sat->range(), 'f', 0) +" km ");
  setText(9, QString::number(sat->velocity(), 'f', 3) +" km/s ");
  setText(10, QString::number(sat->orbitnum()));
  setText(11, QString::number(sat->ma(), 'f', 2));
  if(sat->squinttype() != 0)
    setText(12, QString::number(sat->squint(), 'f', 2)+"° ");
  else
    setText(12, "--.-° ");
}
/** No descriptions */
satellite* satelliteListViewItem::getSatellite(){
  return sat;
}

/** No descriptions */
void satelliteListViewItem::paintCell ( QPainter * p, const QColorGroup & cg, int column, int width, int align ){
  if (sat->elevation() < 0.0)
    QListViewItem::paintCell(p, cg, column, width, align);
  else {
    QColorGroup mycol=cg;
    mycol.setColor(QColorGroup::Text, QColor(255,0,0));
    mycol.setColor(QColorGroup::HighlightedText, QColor(255,0,0));
    QListViewItem::paintCell(p, mycol, column, width, align);
  }
}
