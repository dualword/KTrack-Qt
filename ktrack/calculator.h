/* KTrack-Qt (2020-2024) https://github.com/dualword/KTrack-Qt License:GNU GPL*/
/***************************************************************************
                          calculator.h  -  description
                             -------------------
    begin                : Die Mai 28 2002
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

#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "globals.h"
#include "satellite.h"

#include "sgp4sdp4/sgp4sdp4.h"

/**The class calculates every satellite
  *@author Luc Langehegermann
  */

class calculator : public QObject  {
   friend class predictionList;
   Q_OBJECT

public: 
	calculator(QObject *p=0);
	~calculator();
	/** initializes the calculator */
	void init();
	/** return the satellite list */
	PtrSatList* satList();
	/** return the observers QTH */
	obsQTH* getObsQTH();
	/** set the observers QTH */
	void setObsQTH(obsQTH* q);
	satellite* copySatellite(satellite* sat);

signals: // Signals
	/** Emitted, when we are finished an life calculation */
	void calculated();

private slots: // Private slots
	/** called when we want to calculate */
	void timeout();

private:
	void calc(double daynum, satellite* sat, bool doaoslos);
	void calc(satellite* sat);
	bool aosHappens(tle_t* tle, geodetic_t* obs);
	double FindAOS(double daynum, satellite* sat, bool hasaos);
	double FindLOS(double daynum, satellite* sat, bool haslos);
	double nextAosLos(double daynum, satellite* sat, bool aoslos);
	double FindLOS2(double daynum, satellite* sat, bool haslos);
	double NextAOS(double daynum, satellite* sat, bool aoslos);
	QString daynum2String(double daynum);

	PtrSatList satlist; // list of satellites
	obsQTH* qth;  // observers qth
	satellite* trackingSatellite;
	int count;

};

#endif
