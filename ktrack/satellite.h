/***************************************************************************
                          satellite.h  -  description
                             -------------------
    begin                : Fri Aug 16 2002
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

#ifndef SATELLITE_H
#define SATELLITE_H

#include "globals.h"
#include <qstring.h>

/**
  *@author Luc Langehegermann
  */

class satellite {
  friend class calculator;
public:
  satellite(QString name);
  // set
  void setPolled(bool polled);
  // and get
  QString name();
  bool polled();
  unsigned int catnum();
  float longitude();
  float latitude();
  float azimuth();
  float elevation();
  QString nextAosLos();
  QString calculatedDate();
  float footprint();
  float range();
  float altitude();
  float velocity();
  float squint();
  long orbitnum();
  float ma();  // AMSAT mean anomalie
  double doppler(double f);
  QList<transponder> *translist();
  void setSquintData(double alon, double alat, int squinttype);
  double ALON();
  double ALAT();
  int squinttype();
private:
  QString _name;  // name of the satellite
  bool _polled;   // get informations about this satellite?
  QList<transponder> _translist;
  QString line1, line2;
  double _velocity, _azimuth, _elevation, _range, _range_rate, _latitude, _longitude;
  double _height, _ma, _footprint, _alon, _alat, _squint, _nextAosLos;
  int _squinttype, _catnr;
  long int _orbitnum;
  QString _nextAosLosString, _calculatedDate;
};

#endif
