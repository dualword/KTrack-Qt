/* KTrack-Qt (2020-2024) https://github.com/dualword/KTrack-Qt License:GNU GPL*/
/***************************************************************************
                          rigctrl.h  -  description
                             -------------------
    begin                : Sun Sep 8 2002
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

#ifndef RIGCTRL_H
#define RIGCTRL_H

#include <qobject.h>

#include "globals.h"
#include "satellite.h"
#include "hamlibwrapper.h"

/**
  *@author Luc Langehegermann
  */

class rigctrl : public QObject  {
   Q_OBJECT
public: 
	rigctrl(QObject *p);
	~rigctrl();
  /** No descriptions */
  int open(s_hardware* hardware);
  /** No descriptions */
  void setSatellite(satellite* s);
  /** No descriptions */
  void setTransponder(transponder* t);
  /** No descriptions */
  void setCorrection(int corr);
  /** No descriptions */
  void setDirection(float el, float az);
  hamlibWrapper* Wrapper() {return wrapper;};
private:
  hamlibWrapper* wrapper;
  transponder* currentTransponder;
  satellite* currentSatellite;
  int currentCorrection;
  int counter, correction;
  double currentUplinkFreq, currentDownlinkFreq;
  float azcorrection, elcorrection;
public slots: // Public slots
  /** gets called when a new calculation is done */
  void slotTimeout();
  void slotSetPause(bool);
signals: // Signals
  void newDownlinkFreq(double);
  void newUplinkFreq(double);
  void newHardwareDownlinkFreq(double);
  void newHardwareUplinkFreq(double);
private: // Private methods
  void setFreqs(bool n);
  double oldtrxfreq;
};

#endif
