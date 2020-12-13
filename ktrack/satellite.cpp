/***************************************************************************
                          satellite.cpp  -  description
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

#include "satellite.h"

satellite::satellite(QString name) {
  _name=name;
  _polled=false;
  _nextAosLos=0.0;
}
// set
void satellite::setPolled(bool polled) {
  _polled=polled;
}
// and get
QString satellite::name() {
  return _name;
}
bool satellite::polled() {
  return _polled;
}
float satellite::longitude() {
  return _longitude;
}
float satellite::latitude() {
  return _latitude;
}
float satellite::azimuth() {
  return _azimuth;
}
float satellite::elevation() {
  return _elevation;
}
QString satellite::nextAosLos() {
  return _nextAosLosString;
}
QString satellite::calculatedDate() {
  return _calculatedDate;
}
float satellite::footprint() {
  return _footprint;
}
float satellite::range() {
  return _range;
}
float satellite::altitude() {
  return _height;
}
float satellite::velocity() {
  return _velocity;
}
long satellite::orbitnum() {
  return _orbitnum;
}
float satellite::ma() {
  return _ma;
}
double satellite::doppler(double f) {
  double result;
  result = -f*((_range_rate*1000.0)/299792458.0);
  return result;
}
QList<transponder>* satellite::translist() {
  return &_translist;
}

float satellite::squint() {
  return _squint;
}/** No descriptions */

void satellite::setSquintData(double alon, double alat, int squinttype){
  _alon=alon;
  _alat=alat;
  _squinttype=squinttype;
}

double satellite::ALON() {
  return _alon;
}

double satellite::ALAT() {
  return _alat;
}

int satellite::squinttype() {
  return _squinttype;
}

unsigned int satellite::catnum() {
  return _catnr;
}

