/* KTrack-Qt (2020-2024) https://github.com/dualword/KTrack-Qt License:GNU GPL*/
/***************************************************************************
                          ktrack.cpp  -  description
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

#include <stdio.h>
#include <QTimer>
#include <QLayout>
#include <QCheckBox>
#include <QAction>
#include <QLocale>
#include <QMenuBar>
#include <QMessageBox>
#include <QApplication>

//#include "rigconfig.h"
#include "satelliteselection.h"
#include "transponderdefinitiondialog.h"
#include "groundstationsettings.h"
#include "predictionlist.h"
#include "globals.h"
#include "squintcalculations.h"
#include "ktrack.h"

Ktrack::Ktrack(QWidget *parent, const char *name) : QMainWindow(parent, name="Mainwindow") {
	setWindowTitle("KTrack-Qt");
	QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(tr("Exit"),this, SLOT(close()));
	QMenu* optMenu = menuBar()->addMenu(tr("&Options"));
	//act = new Q3Action(tr("Rig control"),0,this, SLOT(slotRigControl()), actionCollection());
	optMenu->addAction(tr("Groundstation Data"),this, SLOT(slotGroundstation()));
	QMenu* satMenu = menuBar()->addMenu(tr("&Satellite"));
	satMenu->addAction(tr("Select Satellites"),this, SLOT(slotSatellites()));
	satMenu->addAction(tr("Predict satellite passes"),this, SLOT(slotPredict()));
	satMenu->addAction(tr("Transponder definition"),this, SLOT(slotTransponderDefinition()));
	satMenu->addAction(tr("Squint calculations parameters"),this, SLOT(slotSquintCalculations()));
	QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(tr("About"),this, SLOT(slotShowAbout()));

//  KConfig* config;
//  config=kapp->config();
//  config->setGroup("General");
//  int width=config->readNumEntry("Width", 800);
//  int height=config->readNumEntry("Height", 600);
  resize(600, 400);

  // call the start() slot after we are finished
  QTimer::singleShot(0, this, SLOT(start()));
}

Ktrack::~Ktrack()
{
//  if(trxctl) delete trxctl;
  writeConfig();
}
/** This slot is called when we are setup */
void Ktrack::start() {
	PtrSatList* list;
	satellite* sat;
	QStringList strlist;
	QSettings config;
	QWidget* w = new QWidget(this);

	QGridLayout* layout = new QGridLayout(w,2,1);
	trxwidget = new trxWidget(w);
	layout->addWidget(trxwidget,0,0);

	split = new QSplitter(Qt::Vertical,w);
	layout->addWidget(split,1,0);
	//layout->setRowStretch(1,1);

	map = new mapWidget(split);

	satListView=new satelliteListView(split);

	split->addWidget(map);
	split->addWidget(satListView);
	split->setStretchFactor(0,1);
	split->setStretchFactor(1,0.5);
	w->setLayout(layout);
	setCentralWidget(w);

	calc = new calculator();
	calc->init();
	// create a new observer from the configuration file
	config.beginGroup("QTH");
	obsQTH* q = new obsQTH();
	q->setCallsign(config.value("Callsign", "CallSign").toString());
	q->setLongitude(config.value("Longitude", 0.0).toFloat());
	q->setLatitude(config.value("Latitude", 0.0).toFloat());
	q->setHeight(config.value("Height", 0.0).toFloat());
	config.endGroup();
	calc->setObsQTH(q);

  // connect the calculated() signal from the client connection to the satellite listview, and the map update
  QObject::connect(calc, SIGNAL(calculated()), satListView, SLOT(updateListView()));
  QObject::connect(calc, SIGNAL(calculated()), map, SLOT(processSatList()));
  // connect the 'newTrackingSatellite' signal to the slotNewTrackingSatellite slot
  QObject::connect(satListView, SIGNAL(newTrackingSatellite(satellite*)), this, SLOT(slotNewTrackingSatellite(satellite*)));
  // retrieve the satlist, and restore the satellites we displayed last time.
  list = calc->satList();
//  config=kapp->config();
//  config->setGroup("Satellite");
  //strlist=config->readListEntry("poll");
//  for (QStringList::Iterator it = strlist.begin(); it != strlist.end(); ++it ){
//    for(sat=list.first(); sat!=0; sat=list.next())
//      if(sat->name() == *it) sat->setPolled(true);
//  }
  // now we set the satlist of the map widget and the list widget

  map->setSatList(calc->satList());
  satListView->setSatList(calc->satList());

  // get the map a pointer of the observer qth
  map->setObsQth(calc->getObsQTH());
  
  //QObject::connect(calc, SIGNAL(calculated()), this, SLOT(processTrackingSatellite()));
  // readconfig
  //readConfig();
  // init the devices we control
  //slotInitHardware();
}

/** Opens a window with the satelite Selection */
void Ktrack::slotSatellites(){
  satelliteSelection* luc = new satelliteSelection(this);
  luc->setSatList(calc->satList());
  // this will be used, when the user clicks Ok. It makes shure, that the listview is updates properly
  QObject::connect(luc, SIGNAL(polledChanged()), this, SLOT(slotSatellitesOk()));
  luc->exec();
  delete luc;
}

/** Read the configuration file */
void Ktrack::readConfig(){
  double alon,alat;
  int squinttype;
  QStringList strlist;
  PtrSatList* list;
  satellite* sat;
  transponder* trans;
  int i;
////  KConfig* config;
////  config=kapp->config();
//
////  config->setGroup("Rig");
//  hardwareParameters.rotor450 = config->readBoolEntry("450� Rotor", true);
//  hardwareParameters.azimuthCorrection = config->readDoubleNumEntry("Azimuth Correction", 0.0);
//  hardwareParameters.elevationCorrection = config->readDoubleNumEntry("Elevation Correction", 0.0);
//
//  config->setGroup("Rotor");
//  hardwareParameters.rotorinterface.rotor=config->readNumEntry("Rotor", ROT_MODEL_DUMMY);
//  hardwareParameters.rotorinterface.port=config->readEntry("Port", "none");
//  hardwareParameters.rotorinterface.speed=config->readNumEntry("Speed", 1200);
//
//  if (hardwareParameters.rotorinterface.rotor==0)
//    hardwareParameters.rotorinterface.rotor=ROT_MODEL_DUMMY;
//
//  config->setGroup("Downlink Rig");
//  hardwareParameters.drig.radio=config->readNumEntry("Radio", RIG_MODEL_DUMMY);
//  hardwareParameters.drig.port=config->readEntry("Port", "none");
//  hardwareParameters.drig.speed=config->readNumEntry("Speed", 9600);
//  hardwareParameters.downlinkreceiver=config->readBoolEntry("downlinkreceiver", false);
//
//  config->setGroup("Uplink Rig");
//  hardwareParameters.urig.radio=config->readNumEntry("Radio", RIG_MODEL_DUMMY);
//  hardwareParameters.urig.port=config->readEntry("Port", "none");
//  hardwareParameters.urig.speed=config->readNumEntry("Speed", 9600);
//
//  // correct values, if we read from an older config file the first time
//
//  if(hardwareParameters.urig.radio==0)
//    hardwareParameters.urig.radio=RIG_MODEL_DUMMY;
//  if(hardwareParameters.drig.radio==0)
//    hardwareParameters.drig.radio=RIG_MODEL_DUMMY;

  // read the transponders
  list = calc->satList();
//  for(sat=list.first(); sat!=0; sat=list.next()) {
//    config->setGroup(sat->name() + " transponders");
//    i=0;
//    for (;;) {
//      if (config->hasKey(QString::number(i))) {
//      strlist=config->readListEntry(QString::number(i));
//        trans = new transponder();
//        trans->setLastOffset(0);
//        trans->setDownlink(strlist[0].toDouble());
//        trans->setUplink(strlist[1].toDouble());
//        trans->setDowndiff(strlist[2].toDouble());
//        trans->setUpdiff(strlist[3].toDouble());
//        trans->setMode(strlist[4].toInt());
//        trans->setReverse(strlist[5].toInt());
//        trans->setPreamp(strlist[6].toInt());
//        sat->translist()->append(trans);
//        ++i;
//      }
//      else {
//        // now data for squint calculations
//        alon=config->readDoubleNumEntry("ALON", 0.0);
//        alat=config->readDoubleNumEntry("ALAT", 0.0);
//        squinttype=config->readNumEntry("Squint type", 0);
//        sat->setSquintData(alon, alat, squinttype);
          alon=0.0;
          alat=0.0;
          squinttype=0;
          sat->setSquintData(alon, alat, squinttype);
//        break;
//      }
//    }
//  }
  // splitter sizes
//  config->setGroup("General");
//  QValueList<int> splittersizes = config->readIntListEntry("splitter");
  //split->setSizes(splittersizes);
}

/** write the configuration */
void Ktrack::writeConfig(){
  QStringList strlist;
  satellite* sat;
  QList<transponder> translist;
  transponder* trans;
  int i;

  QSettings config;
//  KConfig* config=kapp->config();
//  config->setGroup("Satellite");
  // build a stringlist of all satellites we currently display
  PtrSatList* list = calc->satList();
//  for(sat=list.first(); sat!=0; sat=list.next())
//    if(sat->polled()) strlist << sat->name();
//  config->writeEntry ("poll", strlist);
//
//  config->setGroup("Rig");
//  config->writeEntry("450� Rotor", hardwareParameters.rotor450);
//  config->writeEntry("Azimuth Correction", hardwareParameters.azimuthCorrection);
//  config->writeEntry("Elevation Correction", hardwareParameters.elevationCorrection);
//
//  config->setGroup("Rotor");
//  config->writeEntry("Rotor", hardwareParameters.rotorinterface.rotor);
//  config->writeEntry("Port", hardwareParameters.rotorinterface.port);
//  config->writeEntry("Speed", hardwareParameters.rotorinterface.speed);
//
//  config->setGroup("Downlink Rig");
//  config->writeEntry("Radio", hardwareParameters.drig.radio);
//  config->writeEntry("Port", hardwareParameters.drig.port);
//  config->writeEntry("Speed", hardwareParameters.drig.speed);
//  config->writeEntry("downlinkreceiver", hardwareParameters.downlinkreceiver);
//
//  config->setGroup("Uplink Rig");
//  config->writeEntry("Radio", hardwareParameters.urig.radio);
//  config->writeEntry("Port", hardwareParameters.urig.port);
//  config->writeEntry("Speed", hardwareParameters.urig.speed);
//
//  config->setGroup("General");
//  config->writeEntry("Width", width());
//  config->writeEntry("Height", height());

  // write transponders
//
//  for(sat=list.first(); sat!=0; sat=list.next()) {
//    config->deleteGroup(sat->name() + " transponders");
//    config->setGroup(sat->name() + " transponders");
//    i = 0;
//    translist=*sat->translist();
//    for (trans=translist.first(); trans!=0; trans=translist.next()) {
//      strlist.clear();
//      strlist << QString::number(trans->downlink(),'f',0);
//      strlist << QString::number(trans->uplink()+trans->getLastOffset(),'f',0);
//      strlist << QString::number(trans->downdiff(),'f',0);
//      strlist << QString::number(trans->updiff(),'f',0);
//      strlist << QString::number(trans->mode());
//      strlist << QString::number(trans->reverse());
//      strlist << QString::number(trans->preamp());
//      config->writeEntry(QString::number(i), strlist);
//      ++i;
//    }
//    // squint settings
//    if(sat->squinttype() != 0) {
//      config->writeEntry("ALON", sat->ALON());
//      config->writeEntry("ALAT", sat->ALAT());
//      config->writeEntry("Squint type", sat->squinttype());
//    }
//  }
//  config->setGroup("General");
//  config->writeEntry("splitter", split->sizes());

  // write the qth
  obsQTH* q = calc->getObsQTH();
  config.beginGroup("QTH");
  config.setValue("Callsign", q->callsign());
  config.setValue("Longitude", q->longitude());
  config.setValue("Latitude", q->latitude());
  config.setValue("Height", q->height());
  config.endGroup();

}

void Ktrack::slotSatellitesOk(){
	satListView->setSatList(calc->satList());
}

void Ktrack::slotNewTrackingSatellite(satellite* sat)
{
  if (sat) {
    map->setTrackingSatellite(sat);
    trackingSatellite=sat;
//    if(trxctl)
//      trxctl->setSatellite(sat);
//    trxwidget->setSatellite(sat);
  }
}

/** Called, to display the rigcontrol dialog */
void Ktrack::slotRigControl()
{
//  rigConfig* luc = new rigConfig();
//  luc->setRig(&hardwareParameters);
//  QObject::connect(luc, SIGNAL(newDevices()), this, SLOT(slotInitHardware()));
//  luc->show();
}

/** This slot reinitializes the devices we control with this program */
void Ktrack::slotInitHardware(){
  bool i;
//  if (!trxctl) trxctl=new rigctrl();

  // connect the trxwidgets pause button to the icom910 interface
  //QObject::connect(trxwidget->buttonPause, SIGNAL(toggled(bool)), trxctl, SLOT(slotSetPause(bool)));

  //i = trxctl->open(&hardwareParameters);
//  if (i)
//    KMessageBox::error(this, tr ("Unable to Initialize rig control!"));
//  else
//    trxwidget->setDevice(trxctl);
}

/** This slot is called, when we have new data from the server */
void Ktrack::processTrackingSatellite(){
  // update the rotor
//  if (trackingSatellite) {
//    if (trackingSatellite->elevation() >= 0.0) {
//      trxctl->setDirection(trackingSatellite->elevation(), trackingSatellite->azimuth());
//    }
//  }
}

/** calls the transponder definition dialog */
void Ktrack::slotTransponderDefinition(){
	transponderDefinitionDialog* luc = new transponderDefinitionDialog();
	luc->setSatList(calc->satList());
	luc->exec();
	delete luc;
//  trxwidget->updateTransponderList();
}

/** calls the groundstation configuration dialog */
void Ktrack::slotGroundstation(){
	groundstationSettings* luc = new groundstationSettings(this);
	luc->setObsQTH(calc->getObsQTH());
	luc->exec();
	// this makes sure, that the calculater gives the kepcalculater the new qth infos
	calc->setObsQTH(calc->getObsQTH());
	writeConfig();
	delete luc;
}

/** Calls prediction window */
void Ktrack::slotPredict(){
	predictionList* luc = new predictionList();
	luc->setQTH(calc->getObsQTH());
	luc->setSatList(calc->satList());
	luc->setSatList(calc->satList());
	luc->exec();
	delete luc;
}

/** Called to popup the squint calculations config menu */
void Ktrack::slotSquintCalculations(){
	squintCalculations* luc = new squintCalculations();
	luc->setSatList(calc->satList());
	luc->exec();
	delete luc;
}

void Ktrack::slotShowAbout(){
	QString str;
	str.append("<b>KTrack-Qt</b> - satellite tracking program. Qt port of <i>KTrack</i>. <br>");
	str.append("Source code: <a href='http://github.com/dualword/KTrack-Qt/'>https://github.com/dualword/KTrack-Qt</a><hr/>");
	str.append("<b>KTrack:</b> <br/>");
	str.append("License: GNU General Public License. <br/>");
	str.append("&copy;2002, 2003 Luc Langehegermann (LX2GT), The KDE satellite tracking program, lx2gt@users.sourceforge.net <br/>");
	str.append("Luc Langehegermann, Main application programmer, lx2gt@users.sourceforge.net <br/>");
	str.append("Neoklis Kyriazis (5B4AZ), Porting of Fortran SGP4/SDP4 Routines to C <br/>");
	QMessageBox::about(this, tr("About KTrack-Qt"), str );
}
