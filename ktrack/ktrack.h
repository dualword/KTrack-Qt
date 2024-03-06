/* KTrack-Qt (2020-2024) https://github.com/dualword/KTrack-Qt License:GNU GPL*/
/***************************************************************************
                          ktrack.h  -  description
                             -------------------
    begin                : Tue Apr 23 18:25:06 CEST 2002
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

#ifndef KTRACK_H
#define KTRACK_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

//#include <hamlib/rig.h>
//#include <hamlib/rotator.h>
#include "calculator.h"
#include "satellitelistview.h"
#include "mapwidget.h"
#include "rigctrl.h"
#include "trxwidget.h"

/** Ktrack is the base class of the project */
class Ktrack : public QMainWindow {
Q_OBJECT

public:
	/** construtor */
	Ktrack(QWidget* parent=0, Qt::WindowFlags f = 0);
	/** destructor */
	~Ktrack();

public slots: // Public slots
	/** Opens a window with the satelite Selection */
	void slotSatellites();

private slots: // Private slots
	/** This slot is called when we are setup */
	void start();
	/** Updates the list of satellites in the satelliteListView */
	void slotSatellitesOk();
	/** called, when we want to track another satellite */
	void slotNewTrackingSatellite(satellite* sat);
	/** Called, to display the rigcontrol dialog */
	void slotRigControl();
	/** This slot reinitializes the devices we control with this program */
	void slotInitHardware();
	/** This slot is called, when we have new data from the server */
	void processTrackingSatellite();
	/** calls the transponder definition dialog */
	void slotTransponderDefinition();
	/** calls the groundstation configuration dialog */
	void slotGroundstation();
	/** Calls prediction window */
	void slotPredict();
	/** Called to popup the squint calculations config menu */
	void slotSquintCalculations();
	void slotShowAbout();

private: // Private methods
	/** write the configuration */
	void writeConfig();
	/** Read the configuration file */
	void readConfig();

private:
	calculator* calc;
	mapWidget* map;
	satelliteListView* satListView;
	satellite* trackingSatellite;   // pointer to the satellite we are tracking
	//  s_hardware hardwareParameters;
	rigctrl* trxctl;
	trxWidget* trxwidget;
	QSplitter* split;   // splitter where the map and the listview resides

};

#endif
