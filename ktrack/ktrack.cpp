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
#include <qtimer.h>
#include <qlayout.h>
#include <qcheckbox.h>
#include <kaction.h>
#include <kstdaction.h>
#include <klocale.h>
#include <kmenubar.h>
#include <kconfig.h>
#include <kmessagebox.h>
#include <klineeditdlg.h>
#include <kapplication.h>
#include <dcopclient.h>

#include "rigconfig.h"
#include "satelliteselection.h"
#include "transponderdefinitiondialog.h"
#include "groundstationsettings.h"
#include "predictionlist.h"
#include "globals.h"
#include "squintcalculations.h"
#include "ktrack.h"


Ktrack::Ktrack(QWidget *parent, const char *name) : KMainWindow(parent, name="Mainwindow")
{
  // Register with DCOP
  kapp->dcopClient()->registerAs("ktrack", false);


  trackingSatellite=NULL;
  trxctl=NULL;
  // create the Menu's
  KAction* act;
  QPopupMenu *file = new QPopupMenu();
  menuBar()->insertItem(i18n("&File"),file);
  QPopupMenu *options = new QPopupMenu();
  menuBar()->insertItem(i18n("&Options"),options);
  QPopupMenu *sat = new QPopupMenu();
  menuBar()->insertItem(i18n("&Satellite"),sat);
  act = KStdAction::quit(this, SLOT(close()), actionCollection());
  act->plug(file);

  act = new KAction(i18n("Rig control"),0,this, SLOT(slotRigControl()), actionCollection());
  act->plug(options);
  act = new KAction(i18n("Groundstation Data"), 0, this, SLOT(slotGroundstation()), actionCollection());
  act->plug(options);

  act = new KAction(i18n("&Select Satellites"),0,this, SLOT(slotSatellites()), actionCollection());
  act->plug(sat);
  act = new KAction(i18n("&Predict satellite passes"),0,this, SLOT(slotPredict()), actionCollection());
  act->plug(sat);
  act = new KAction(i18n("Transponder definition"), 0, this, SLOT(slotTransponderDefinition()), actionCollection());
  act->plug(sat);
  act = new KAction(i18n("Squint calculations parameters"), 0, this, SLOT(slotSquintCalculations()), actionCollection());
  act->plug(sat);

  // default kde help menu

  menuBar()->insertItem(i18n("&Help"), (QPopupMenu*)helpMenu());

  // resize outself before we do the start() thing

  KConfig* config;
  config=kapp->config();
  config->setGroup("General");
  int width=config->readNumEntry("Width", 800);
  int height=config->readNumEntry("Height", 600);
  resize(width, height);

  // call the start() slot after we are finished
  QTimer::singleShot(0, this, SLOT(start()));
}

Ktrack::~Ktrack()
{
  if(trxctl) delete trxctl;
  writeConfig();
}
/** This slot is called when we are setup */
void Ktrack::start(){
  QList<satellite> list;
  satellite* sat;
  QStringList strlist;
  KConfig* config;
  QWidget* w = new QWidget(this);
  setCentralWidget(w);
  QGridLayout* layout=new QGridLayout(w,3,1);
  trxwidget = new trxWidget(w);
  layout->addWidget(trxwidget,0,0);

  split = new QSplitter(Qt::Vertical,w);
  layout->addWidget(split,1,0);
  layout->setRowStretch(1,1);

  map = new mapWidget(split);

  satListView=new satelliteListView(split);

  w->show();

  calc = new calculator();
  calc->init();
  // create a new observer from the configuration file
  config=kapp->config();
  config->setGroup("QTH");
  obsQTH* q = new obsQTH();
  q->setCallsign(config->readEntry("Callsign", "LX2GT"));
  q->setLongitude(config->readDoubleNumEntry("Longitude", -6.333));
  q->setLatitude(config->readDoubleNumEntry("Latitude", 49.45));
  q->setHeight(config->readDoubleNumEntry("Height", 300.0));
  calc->setObsQTH(q);

  // connect the calculated() signal from the client connection to the satellite listview, and the map update
  QObject::connect(calc, SIGNAL(calculated()), satListView, SLOT(updateListView()));
  QObject::connect(calc, SIGNAL(calculated()), map, SLOT(processSatList()));
  // connect the 'newTrackingSatellite' signal to the slotNewTrackingSatellite slot
  QObject::connect(satListView, SIGNAL(newTrackingSatellite(satellite*)), this, SLOT(slotNewTrackingSatellite(satellite*)));
  // retrieve the satlist, and restore the satellites we displayed last time.
  list=calc->satList();
  config=kapp->config();
  config->setGroup("Satellite");
  strlist=config->readListEntry("poll");
  for (QStringList::Iterator it = strlist.begin(); it != strlist.end(); ++it ){
    for(sat=list.first(); sat!=0; sat=list.next())
      if(sat->name() == *it) sat->setPolled(true);
  }
  // now we set the satlist of the map widget and the list widget

  map->setSatList(calc->satList());
  satListView->setSatList(calc->satList());

  // get the map a pointer of the observer qth
  map->setObsQth(calc->getObsQTH());
  
  QObject::connect(calc, SIGNAL(calculated()), this, SLOT(processTrackingSatellite()));
  // readconfig
  readConfig();
  // init the devices we control
  slotInitHardware();
}

/** Opens a window with the satelite Selection */
void Ktrack::slotSatellites(){
  satelliteSelection* luc = new satelliteSelection(this);
  luc->setSatList(calc->satList());
  // this will be used, when the user clicks Ok. It makes shure, that the listview is updates properly
  QObject::connect(luc, SIGNAL(polledChanged()), this, SLOT(slotSatellitesOk()));
  luc->show();
}
/** Read the configuration file */
void Ktrack::readConfig(){
  double alon,alat;
  int squinttype;
  QStringList strlist;
  QList<satellite> list;
  satellite* sat;
  transponder* trans;
  int i;
  KConfig* config;
  config=kapp->config();

  config->setGroup("Rig");
  hardwareParameters.rotor450 = config->readBoolEntry("450° Rotor", true);
  hardwareParameters.azimuthCorrection = config->readDoubleNumEntry("Azimuth Correction", 0.0);
  hardwareParameters.elevationCorrection = config->readDoubleNumEntry("Elevation Correction", 0.0);

  config->setGroup("Rotor");
  hardwareParameters.rotorinterface.rotor=config->readNumEntry("Rotor", ROT_MODEL_DUMMY);
  hardwareParameters.rotorinterface.port=config->readEntry("Port", "none");
  hardwareParameters.rotorinterface.speed=config->readNumEntry("Speed", 1200);

  if (hardwareParameters.rotorinterface.rotor==0)
    hardwareParameters.rotorinterface.rotor=ROT_MODEL_DUMMY;
  
  config->setGroup("Downlink Rig");
  hardwareParameters.drig.radio=config->readNumEntry("Radio", RIG_MODEL_DUMMY);
  hardwareParameters.drig.port=config->readEntry("Port", "none");
  hardwareParameters.drig.speed=config->readNumEntry("Speed", 9600);
  hardwareParameters.downlinkreceiver=config->readBoolEntry("downlinkreceiver", false);

  config->setGroup("Uplink Rig");
  hardwareParameters.urig.radio=config->readNumEntry("Radio", RIG_MODEL_DUMMY);
  hardwareParameters.urig.port=config->readEntry("Port", "none");
  hardwareParameters.urig.speed=config->readNumEntry("Speed", 9600);

  // correct values, if we read from an older config file the first time

  if(hardwareParameters.urig.radio==0)
    hardwareParameters.urig.radio=RIG_MODEL_DUMMY;
  if(hardwareParameters.drig.radio==0)
    hardwareParameters.drig.radio=RIG_MODEL_DUMMY;  

  // read the transponders
  list = calc->satList();
  for(sat=list.first(); sat!=0; sat=list.next()) {
    config->setGroup(sat->name() + " transponders");
    i=0;
    for (;;) {
      if (config->hasKey(QString::number(i))) {
      strlist=config->readListEntry(QString::number(i));
        trans = new transponder();
        trans->setLastOffset(0);
        trans->setDownlink(strlist[0].toDouble());
        trans->setUplink(strlist[1].toDouble());
        trans->setDowndiff(strlist[2].toDouble());
        trans->setUpdiff(strlist[3].toDouble());
        trans->setMode(strlist[4].toInt());
        trans->setReverse(strlist[5].toInt());
        trans->setPreamp(strlist[6].toInt());
        sat->translist()->append(trans);
        ++i;
      }
      else {
        // now data for squint calculations
        alon=config->readDoubleNumEntry("ALON", 0.0);
        alat=config->readDoubleNumEntry("ALAT", 0.0);
        squinttype=config->readNumEntry("Squint type", 0);
        sat->setSquintData(alon, alat, squinttype);
        break;
      }
    }
  }
  // splitter sizes
  config->setGroup("General");
  QValueList<int> splittersizes = config->readIntListEntry("splitter");
  split->setSizes(splittersizes);
}

/** write the configuration */
void Ktrack::writeConfig(){
  QStringList strlist;
  satellite* sat;
  QList<transponder> translist;
  transponder* trans;
  int i;
  KConfig* config=kapp->config();
  config->setGroup("Satellite");
  // build a stringlist of all satellites we currently display
  QList<satellite> list = calc->satList();
  for(sat=list.first(); sat!=0; sat=list.next())
    if(sat->polled()) strlist << sat->name();
  config->writeEntry ("poll", strlist);

  config->setGroup("Rig");
  config->writeEntry("450° Rotor", hardwareParameters.rotor450);
  config->writeEntry("Azimuth Correction", hardwareParameters.azimuthCorrection);
  config->writeEntry("Elevation Correction", hardwareParameters.elevationCorrection);

  config->setGroup("Rotor");
  config->writeEntry("Rotor", hardwareParameters.rotorinterface.rotor);
  config->writeEntry("Port", hardwareParameters.rotorinterface.port);
  config->writeEntry("Speed", hardwareParameters.rotorinterface.speed);

  config->setGroup("Downlink Rig");
  config->writeEntry("Radio", hardwareParameters.drig.radio);
  config->writeEntry("Port", hardwareParameters.drig.port);
  config->writeEntry("Speed", hardwareParameters.drig.speed);
  config->writeEntry("downlinkreceiver", hardwareParameters.downlinkreceiver);

  config->setGroup("Uplink Rig");
  config->writeEntry("Radio", hardwareParameters.urig.radio);
  config->writeEntry("Port", hardwareParameters.urig.port);
  config->writeEntry("Speed", hardwareParameters.urig.speed);

  config->setGroup("General");
  config->writeEntry("Width", width());
  config->writeEntry("Height", height());

  // write transponders

  for(sat=list.first(); sat!=0; sat=list.next()) {
    config->deleteGroup(sat->name() + " transponders");
    config->setGroup(sat->name() + " transponders");
    i = 0;
    translist=*sat->translist();
    for (trans=translist.first(); trans!=0; trans=translist.next()) {
      strlist.clear();
      strlist << QString::number(trans->downlink(),'f',0);
      strlist << QString::number(trans->uplink()+trans->getLastOffset(),'f',0);
      strlist << QString::number(trans->downdiff(),'f',0);
      strlist << QString::number(trans->updiff(),'f',0);
      strlist << QString::number(trans->mode());
      strlist << QString::number(trans->reverse());
      strlist << QString::number(trans->preamp());
      config->writeEntry(QString::number(i), strlist);
      ++i;
    }
    // squint settings
    if(sat->squinttype() != 0) {
      config->writeEntry("ALON", sat->ALON());
      config->writeEntry("ALAT", sat->ALAT());
      config->writeEntry("Squint type", sat->squinttype());
    }
  }
  config->setGroup("General");
  config->writeEntry("splitter", split->sizes());
  // write the qth
  obsQTH* q = calc->getObsQTH();
  config->setGroup("QTH");
  config->writeEntry("Callsign", q->callsign());
  config->writeEntry("Longitude", q->longitude());
  config->writeEntry("Latitude", q->latitude());
  config->writeEntry("Height", q->height());

}

void Ktrack::slotSatellitesOk()
{
  satListView->setSatList(calc->satList());
}

void Ktrack::slotNewTrackingSatellite(satellite* sat)
{
  if (sat) {
    map->setTrackingSatellite(sat);
    trackingSatellite=sat;
    if(trxctl)
      trxctl->setSatellite(sat);
    trxwidget->setSatellite(sat);
  }
}
/** Called, to display the rigcontrol dialog */
void Ktrack::slotRigControl()
{
  rigConfig* luc = new rigConfig();
  luc->setRig(&hardwareParameters);
  QObject::connect(luc, SIGNAL(newDevices()), this, SLOT(slotInitHardware()));
  luc->show();
}
/** This slot reinitializes the devices we control with this program */
void Ktrack::slotInitHardware(){
  bool i;
  if (!trxctl) trxctl=new rigctrl();

  // connect the trxwidgets pause button to the icom910 interface
  QObject::connect(trxwidget->buttonPause, SIGNAL(toggled(bool)), trxctl, SLOT(slotSetPause(bool)));

  i = trxctl->open(&hardwareParameters);
  if (i)
    KMessageBox::error(this, i18n ("Unable to Initialize rig control!"));
  else 
    trxwidget->setDevice(trxctl);
}
/** This slot is called, when we have new data from the server */
void Ktrack::processTrackingSatellite(){
  // update the rotor
  if (trackingSatellite) {
    if (trackingSatellite->elevation() >= 0.0) {
      trxctl->setDirection(trackingSatellite->elevation(), trackingSatellite->azimuth());
    }
  }
}

/** calls the transponder definition dialog */
void Ktrack::slotTransponderDefinition(){
  transponderDefinitionDialog* luc = new transponderDefinitionDialog();
  luc->setSatList(calc->satList());
  luc->exec();
  delete luc;
  trxwidget->updateTransponderList();
}
/** calls the groundstation configuration dialog */
void Ktrack::slotGroundstation(){
  groundstationSettings* luc = new groundstationSettings(this);
  luc->setObsQTH(calc->getObsQTH());
  luc->exec();
  // this makes sure, that the calculater gives the kepcalculater the new qth infos
  calc->setObsQTH(calc->getObsQTH());
}
/** Calls prediction window */
void Ktrack::slotPredict(){
  predictionList* luc = new predictionList();
  luc->setQTH(calc->getObsQTH());
  luc->setSatList(calc->satList());
  luc->show();
}
/** Called to popup the squint calculations config menu */
void Ktrack::slotSquintCalculations(){
  squintCalculations* luc = new squintCalculations();
  luc->setSatList(calc->satList());
  luc->show();
}
