/* KTrack-Qt (2020-2024) https://github.com/dualword/KTrack-Qt License:GNU GPL*/
/***************************************************************************
                          hamlibwrapper.cpp  -  description
                             -------------------
    begin                : Thu Sep 5 2002
    copyright            : (C) 2002 by Luc Langehegermann
    email                : luc@lx2gt.lu
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <string.h>
#include <stdlib.h>
//#include <kapplication.h>

#include "hamlibwrapper.h"

hamlibWrapper::hamlibWrapper(QWidget *p) : QObject (p) {
  //uplinkRig=NULL;
  //downlinkRig=NULL;
  //rotor=NULL;
  samefreq=false;
  shouldexit=false;
  setmodes=false;
  resetpreamp=false;
  setuplink=false;
  setdownlink=false;
  //rig_set_debug(RIG_DEBUG_NONE);
}
hamlibWrapper::~hamlibWrapper(){
//  if (running()) {
//    shouldexit=true;
//    wait();
//  }
//  if (downlinkRig) {
//    rig_close(downlinkRig);
//    rig_cleanup(downlinkRig);
//  }
//  if (uplinkRig) {
//    rig_close(uplinkRig);
//    rig_cleanup(uplinkRig);
//  }
}

/** No descriptions */
//int hamlibWrapper::initRotor(rot_model_t r, const char* rotorport, int rotorspeed, bool flag450){
//	int retval=0;
//  azimuth_t maxazimuth;
//  if (flag450)
//    maxazimuth=450.0;
//  else
//    maxazimuth=360.0;
//
//  mutex.lock();
//  if (rotor) {
//    rot_close(rotor);
//    rot_cleanup(rotor);
//  }
//
//  rotor = rot_init(r);
//  if (rotor) {
//    rotor->state.rotport.parm.serial.rate=rotorspeed;
//    rotor->state.max_az=maxazimuth;
//    strcpy(rotor->state.rotport.pathname, rotorport);
//    if(rot_open(rotor)!=RIG_OK)
//			retval--;
//  }
//  else {
//    retval--;
//  }
//	mutex.unlock();
//	return retval;
//}

/** No descriptions */
//int hamlibWrapper::init (rig_model_t drig, const char* dport, int dspeed, rig_model_t urig, const char* uport, int uspeed){
//	int retval=0;
//  mutex.lock();
//  if (downlinkRig) {
//    rig_close(downlinkRig);
//    rig_cleanup(downlinkRig);
//  }
//  if (uplinkRig) {
//    rig_close(uplinkRig);
//    rig_cleanup(uplinkRig);
//  }
//  // first init the first radio - note: we always have the first one!
//  // if we have no rig... simply init dummy rig! So, we do not have to care if we have an
//  // rig or not!
//  if (drig) downlinkRig=rig_init(drig);
//  // set the parameters
//    downlinkRig->state.rigport.parm.serial.rate=dspeed;
//    strcpy(downlinkRig->state.rigport.pathname, dport);
//  // open the rig
//  if (rig_open(downlinkRig) != RIG_OK) {
//    retval--;
//  }
//
//
//
//  // test if we use one or 2 rigs - if we use 1 exit with success
//
//  if(!urig) {
//    uplinkRig=NULL;
//    // only one rig - try to set the satmode
//    rig_set_func (downlinkRig, RIG_VFO_NONE, RIG_FUNC_SATMODE, 1);
//  }
//  else {
//    // open the second rig
//    uplinkRig=rig_init(urig);
//    uplinkRig->state.rigport.parm.serial.rate=uspeed;
//    strcpy(downlinkRig->state.rigport.pathname, uport);
//    if (rig_open(downlinkRig) != RIG_OK) {
//			retval--;
//    }
//  }
//
//  // set the vfo's
//  if (urig) {
//    uplinkvfo=RIG_VFO_A;
//    downlinkvfo=RIG_VFO_A;
//  }
//  else {
//    downlinkvfo=RIG_VFO_A;
//    uplinkvfo=RIG_VFO_B;
//  }
//
//  if (!running() && downlinkRig) start();
//  mutex.unlock();
//  return retval;
//}

/** No descriptions */
double hamlibWrapper::getDownlinkFrequency(){
  return getdlinkfreq;
}
/** No descriptions */
void hamlibWrapper::setFreqs(double dlink, double ulink, bool n){
//  if (n) mutex.lock();
  if (fabs(dlinkfreq-dlink) > 1.0 ) {
    dlinkfreq=round(dlink);
    setdownlink=true;
  }
  if (fabs(ulinkfreq-ulink) > 1.0) {
    ulinkfreq=round(ulink);
    setuplink=true;
  }
//  if (n) mutex.unlock();
}
/** No descriptions */
//void hamlibWrapper::setModes(rmode_t dmode, rmode_t umode){
//  setmodes=true;
//  downmode=dmode;
//  upmode=umode;
//}

void hamlibWrapper::run() {
//  // these are static (urgs) but we can only have on instance of this!
//  static freq_t oldfreq=0, fr=0;
//  for(;;) {
//    if (shouldexit || !downlinkRig) {
//      shouldexit=false;
//      return;
//    }
//
//    if (rotor) {
//      mutex.lock(); rot_set_position(rotor, rotaz, rotel); mutex.unlock();
//    }
//
//    if (setdownlink) {
//      setHardwareFrequency(true, (freq_t)dlinkfreq);
//      oldfreq=(freq_t)dlinkfreq;
//      setdownlink=false;
//    }
//
//    if (setuplink) {
//      setHardwareFrequency(false, (freq_t)ulinkfreq);
//      setuplink=false;
//    }
//
//    fr=getHardwareDownlinkFrequency();
//    if (fr!=oldfreq) {
//      samefreq=false;
//      oldfreq=fr;
//      getdlinkfreq=fr;
//    } else
//      samefreq=true;
//
//    // check if we should set the modes
//    if (setmodes) {
//      setmodes=false;
//      // downlink
//      if (downlinkRig)
//        {mutex.lock(); rig_set_mode(downlinkRig, downlinkvfo, downmode, 0); mutex.unlock();}
//      // uplink
//      if (uplinkRig)
//        {mutex.lock(); rig_set_mode(uplinkRig, uplinkvfo, upmode, 0); mutex.unlock();}
//      else
//        {mutex.lock(); rig_set_mode(downlinkRig, uplinkvfo, upmode, 0); mutex.unlock();}
//    }
//    if (resetpreamp) {
//      resetpreamp=false;
//      mutex.lock(); rig_set_level(downlinkRig, downlinkvfo, RIG_LEVEL_PREAMP, preampvalue); mutex.unlock();
//    }
//
//  }
}
/** No descriptions */
bool hamlibWrapper::userChangedFreq(){
  return samefreq;
}
/** No descriptions */
void hamlibWrapper::setPause(bool p){
  //if (downlinkRig->caps->rig_model==RIG_MODEL_DUMMY)
  //  return;

  shouldexit=p;
  //if (!p && !running())
    //start();
}
/** No descriptions */
void hamlibWrapper::setPreamp (bool e){
//  int i=0;
//  if (e) {
//    while (downlinkRig->caps->preamp[i] != RIG_DBLST_END)
//      ++i;
//    if (i!=0) --i;
//    preampvalue.i = downlinkRig->caps->preamp[i];
//  }
//  else
//    preampvalue.i = 0;
//
//  resetpreamp=true;
}

/** No descriptions */
//void hamlibWrapper::setDirection(azimuth_t az, elevation_t el){
//  rotaz=az; rotel=el;
//}

//freq_t hamlibWrapper::getHardwareDownlinkFrequency() {
//  const char* errorstring=NULL;
//  int errorcode;
//  freq_t frequency=0;
//  mutex.lock();
//  errorcode=rig_get_freq(downlinkRig, downlinkvfo, &frequency);
//  if (errorcode!=RIG_OK) {
//    fprintf(stderr, "WARNING: Error fetching frequency from Radio! Using default Frequency!\n");
//    errorstring=rigerror(errorcode);
//    if (errorstring)
//      fprintf(stderr, "The reported error was: %s\n", errorstring);
//    frequency=433000000;
//  }
//  // sleep on dummy rig - that saves CPU power
//  if (downlinkRig->caps->rig_model == RIG_MODEL_DUMMY)
//    usleep(100000);
//  else
//    emit (newHardwareDownlinkFreq((double)frequency));
//  mutex.unlock();
//  return frequency;
//}

//void hamlibWrapper::setHardwareFrequency(bool dl, freq_t frequency) {
//	frequency=round(frequency);
//	const char* errorstring=NULL;
//  RIG* rig;
//  vfo_t vfo;
//  int errorcode;
//
//  mutex.lock();
//
//  // find out the right hardware and vfo
//  if (dl) {
//    emit (newHardwareDownlinkFreq((double)frequency));
//    rig=downlinkRig;
//    vfo=downlinkvfo;
//  }
//  else {
//    emit (newHardwareUplinkFreq((double)frequency));
//    if (uplinkRig) {
//      rig=uplinkRig;
//      vfo=uplinkvfo;
//    }
//    else {
//      rig=downlinkRig;
//      vfo=uplinkvfo;
//    }
//  }
//  // do not set uplink if downlink is dummy rig
//  if (/*rig == downlinkRig && dl==0 && */rig->caps->rig_model == RIG_MODEL_DUMMY)
//    errorcode=RIG_OK;
//  else
//    errorcode=rig_set_freq(rig, vfo, frequency);
//  if (errorcode!=RIG_OK) {
//    fprintf(stderr, "WARNING: Error setting frequency to the Radio!\n");
//    errorstring=rigerror(errorcode);
//    if (errorstring)
//      fprintf(stderr, "The reported error was: %s\n", errorstring);
//  }
//  mutex.unlock();
//}
