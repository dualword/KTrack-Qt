/***************************************************************************
                          globals.h  -  description
                             -------------------
    begin                : Fri May 17 2002
    copyright            : (C) 2002 by Luc Langehegermann
    email                : lx2gt@gmx.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// global functions

#ifndef GLOBAL_H
#define GLOBAL_H

#define MODE_LSB 0x00
#define MODE_USB 0x01
#define MODE_CW  0x03
#define MODE_FM  0x05
#define MODE_SSB MODE_USB

#include <qlist.h>
#include <qstring.h>
#include <stdio.h>
#include <time.h>
#include <hamlib/rig.h>
#include <hamlib/rotator.h>

/** This class represents the observers QTH */

class obsQTH {
public:
  obsQTH(QString c="", float lo=0, float la=0, double h=0) {_callsign=c; _longitude=lo; _latitude=la, _height=h;};
  QString callsign() {return _callsign;};
  float longitude() {return _longitude;};
  float latitude() {return _latitude;};
  float height() {return _height;};
  void setCallsign(QString c) {_callsign=c;};
  void setLongitude(float l) {_longitude=l;};
  void setLatitude(float l) {_latitude=l;};
  void setHeight(float h) {_height=h;};
private:
  QString _callsign;
  float _longitude;
  float _latitude;
  float _height;
};

/** This class represents one transponder
   * @author Luc Langehegermann
   */

class transponder {
public:
  transponder() {
    _uplink=0;
    _downlink=0;
    _updiff=0;
    _downdiff=0;
    _mode=0;
    _reverse=0;
    _preamp=0;
  };
  void setUplink(double u) {_uplink=u;};
  void setDownlink(double d) {_downlink=d;};
  void setUpdiff(double ud) {_updiff=ud;};
  void setDowndiff(double dd) {_downdiff=dd;};
  void setMode(int m) {_mode=m;};
  void setReverse(bool r) {_reverse=r;};
  void setPreamp(bool p) {_preamp=p;};
  double uplink() {return _uplink;};
  double downlink() {return _downlink;};
  double updiff() {return _updiff;};
  double downdiff() {return _downdiff;};
  int mode() {return _mode;};
  bool reverse() {return _reverse;};
  bool preamp() {return _preamp;};
  void setLastOffset(int offset){_lastoffset=offset;};
  int getLastOffset(){return _lastoffset;};
private:
  double _uplink, _downlink, _updiff, _downdiff;
  int _mode, _lastoffset;
  bool _reverse;
  bool _preamp;
};


/** this structure will held the information for one transceiver */

typedef struct {
  rig_model_t radio;
  QString port;
  int speed;
} s_rigparam;

typedef struct {
  rot_model_t rotor;
  QString port;
  int speed;
} s_rotparam;


/** this structure is used to store the configuration of our hardware */

typedef struct {
  s_rigparam urig;
  s_rigparam drig;
  s_rotparam rotorinterface;
  bool rotor450;
  bool downlinkreceiver;
  double azimuthCorrection;
  double elevationCorrection;
} s_hardware;

#endif
