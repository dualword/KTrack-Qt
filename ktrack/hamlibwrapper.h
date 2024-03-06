/* KTrack-Qt (2020-2024) https://github.com/dualword/KTrack-Qt License:GNU GPL*/
/***************************************************************************
                          hamlibwrapper.h  -  description
                             -------------------
    begin                : Thu Sep 5 2002
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

#ifndef HAMLIBWRAPPER_H
#define HAMLIBWRAPPER_H

//#include <hamlib/rig.h>
//#include <hamlib/rotator.h>
#include <cmath>
#include <QObject>
#include <QMutex>

/** This is an simple wrapper class around hamlib to control our rigs / rotators

  *@author Luc Langehegermann
  */

class hamlibWrapper : public QObject{ //, QThread
  Q_OBJECT
public: 
	hamlibWrapper(QObject *p = 0);
	~hamlibWrapper();
  /** No descriptions */
  //int init (rig_model_t drig, const char* dport, int dspeed, rig_model_t urig=0, const char* uport=NULL, int uspeed=0);
  /** No descriptions */
  double getDownlinkFrequency();
  /** No descriptions */
  void setFreqs(double dlink, double ulink, bool n);
  /** No descriptions */
  //void setModes(rmode_t dmode, rmode_t umode);
  void run();
  /** No descriptions */
  bool userChangedFreq();
  /** No descriptions */
  void setPause(bool p);
  /** No descriptions */
  void setPreamp (bool e);
  /** No descriptions */
  //int initRotor(rot_model_t r, const char* rotorport, int rotorspeed, bool flag450);
  /** No descriptions */
  //void setDirection(azimuth_t az, elevation_t el);
  /** No descriptions */
private:
  //RIG* uplinkRig;
  //RIG* downlinkRig;
  //ROT* rotor;
  double ulinkfreq, dlinkfreq, getdlinkfreq;
  bool samefreq;
  bool shouldexit;
  bool setmodes;
  bool resetpreamp;
  bool setuplink;
  bool setdownlink;
  //rmode_t downmode, upmode;
  //value_t preampvalue;
  QMutex mutex;
  //freq_t getHardwareDownlinkFrequency();
  //void setHardwareFrequency(bool dl, freq_t frequency);
  //vfo_t uplinkvfo, downlinkvfo;
  //azimuth_t rotaz, rotel;
signals: // Signals
  void newHardwareDownlinkFreq(double);
  void newHardwareUplinkFreq(double);
};

#endif
