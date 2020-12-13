/***************************************************************************
                          predictionlist.h  -  description
                             -------------------
    begin                : Wed Aug 7 2002
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

#ifndef PREDICTIONLIST_H
#define PREDICTIONLIST_H

#include <qwidget.h>
#include <predictionlistbase.h>

#include "globals.h"
#include "satellite.h"
#include "calculator.h"

/**This widget is used to create prediction lists
  *@author Luc Langehegermann
  */

class predictionList : public predictionListBase  {
  Q_OBJECT
public: 
  predictionList(QWidget *parent=0, const char *name=0, WFlags fl=WDestructiveClose);
  ~predictionList();
  void setSatList(QList<satellite> s);
  void setQTH(obsQTH* qth);
private slots: // Private slots
  void slotCalculate();
  void slotDismiss();
private: // Private methods
  double qDateTime2daynum(QDateTime date);
  /** No descriptions */
  void displayResults();
  calculator* calc;
  QPtrList<satellite> satlist;
  QPtrList<satellite> resultlist;
//  QDateTime daynum2QDateTime(double daynum);
};

#endif
