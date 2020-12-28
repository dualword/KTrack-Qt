/* KTrack-Qt (2020) http://github.com/dualword/KTrack-Qt License:GNU GPL*/
/***************************************************************************
                          calculator.cpp  -  description
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

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
//#include <kstandarddirs.h>
#include <qfile.h>
#include <qtimer.h>
#include <qtextstream.h>
#include "sgp4sdp4/sgp4sdp4.h"

#include "calculator.h"

calculator::calculator(QObject *parent, const char *name ) : QObject(parent,name) {
  count=0;  // init the counter
}

calculator::~calculator(){
	for(auto sat: satlist) {
		if(sat) delete sat;
	}
}

/** initializes the calculator */
void calculator::init(){
  satellite* sat;
  QString satname, line1, line2;

  // open the keppler file
//  QString filename = locate("appdata", "nasa.tle");
  QString filename =  "nasa.tle";
//  fprintf(stderr, "Using %s as tle input file!\n", filename.latin1());
  // go through the file and create the list entries

  QFile f(filename);
  if ( f.open(IO_ReadOnly) ) {    // file opened successfully
    QTextStream t( &f );        // use a text stream
    while ( !t.atEnd() ) {        // until end of file...
      satname=t.readLine().stripWhiteSpace();
      sat = new satellite(satname);
      sat->line1=t.readLine();
      sat->line2=t.readLine();
      satlist.append(sat);
    }
    f.close();
   }

  // calc every second --> the tracking satellite is calculated more often - but not displayed that often

  QTimer *timer = new QTimer(this);
  QObject::connect (timer, SIGNAL(timeout()), this, SLOT(timeout()));
  timer->start(1000, false);
}
/** return the satellite list */
PtrSatList* calculator::satList(){
  return &satlist;
}
/** return the observers QTH */
obsQTH* calculator::getObsQTH(){
  return qth;
}
/** called when we want to calculate */
void calculator::timeout(){
	for(auto sat : satlist) {
    if (sat->polled()) {
      calc(sat);
    }
  }
  emit calculated();
}
/** set the observers QTH */
void calculator::setObsQTH(obsQTH* q){
  qth=q;
}

void calculator::calc(satellite* sat) {
  struct tm utc;
  struct timeval tmval;
  double daynum;
  UTC_Calendar_Now(&utc);
  gettimeofday(&tmval, NULL);
  daynum=Julian_Date(&utc);
  daynum=daynum+(double)tmval.tv_usec/8.64e+10;
  calc(daynum, sat, true);
}  

void calculator::calc(double daynum, satellite* sat, bool doaoslos) {
  bool hasaoslos;
  char tle_lines[3][80];
  geodetic_t obs_geodetic;
  tle_t tle;
  
  sat->_calculatedDate=daynum2String(daynum);

  obs_geodetic.lon=qth->longitude()*de2ra;
  obs_geodetic.lat=qth->latitude()*de2ra;
  obs_geodetic.alt=qth->height()/1000.0;
  obs_geodetic.theta=0;

  strcpy(tle_lines[0], "DUMMY");
  strcpy(tle_lines[1], sat->line1);
  strcpy(tle_lines[2], sat->line2);

  ClearFlag(ALL_FLAGS);
  Get_Next_Tle_Set(tle_lines, &tle);

  hasaoslos=aosHappens(&tle, &obs_geodetic);
    
  sat->_catnr=tle.catnr;

  select_ephemeris(&tle);
  double jul_epoch, jul_utc, tsince, phase, age;
  vector_t vel = {0,0,0,0};
  vector_t pos = {0,0,0,0};
  vector_t obs_set;
  geodetic_t sat_geodetic;

  jul_utc = daynum;
  jul_epoch = Julian_Date_of_Epoch(tle.epoch);
  tsince = (jul_utc - jul_epoch) * xmnpda;

  // call the norad routines according to the deep-space flag
  if (isFlagSet(DEEP_SPACE_EPHEM_FLAG))
    SDP4(tsince, &tle, &pos, &vel, &phase);
  else
    SGP4(tsince, &tle, &pos, &vel, &phase);
  // scale position and velocity to km and km/sec
  Convert_Sat_State(&pos, &vel);

  // get the velocity of the satellite

  Magnitude(&vel);
  sat->_velocity=vel.w;
  Calculate_Obs(jul_utc, &pos, &vel, &obs_geodetic, &obs_set);
  Calculate_LatLonAlt(jul_utc, &pos, &sat_geodetic);

  sat->_azimuth=Degrees(obs_set.x);
  sat->_elevation=Degrees(obs_set.y);
  sat->_range=obs_set.z;
  sat->_range_rate=obs_set.w;
  sat->_latitude=Degrees(sat_geodetic.lat);
  sat->_longitude=Degrees(sat_geodetic.lon);
  sat->_height=sat_geodetic.alt;
  sat->_ma=Degrees(phase);
  sat->_ma=(256.0/360.0)*sat->_ma;
  sat->_footprint=2.0 * xkmper * acos(xkmper / pos.w);
  age=jul_utc - jul_epoch;
  sat->_orbitnum=(long)floor((tle.xno*xmnpda/twopi+age*tle.bstar*ae)*age+tle.xmo/twopi)+tle.revnum-1;

  /////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////// SQUINT ///////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////

  if(sat->_squinttype==2) {
   double ax,ay,az;
   double bx,by,bz;
   double cx,cy,cz;
   double rx,ry,rz;
   double _alon,_alat;
   vector_t obs_pos,obs_vel;
   /* calculate antenna angle with line-of-nodes instead of with major axis, and convert to radians */
     _alon = sat->ALON()*M_PI/180 + tle.omegao1;
     _alat = sat->ALAT()*M_PI/180;
   /* calculate unit vector in orbit plane coordinates: X to asc.node, Z perp.to orbit plane */
     bx = cos(_alat)*cos(_alon);
     by = cos(_alat)*sin(_alon);
     bz = sin(_alat);

   /* transform to coordinate system with X still to asc.node, Z along earth's rotation axis */
     cx = bx;
     cy = by*cos(tle.xincl1) - bz*sin(tle.xincl1);
     cz = by*sin(tle.xincl1) + bz*cos(tle.xincl1);
   /* transform to coordinate system with X to "First Point in Aries", i.e., the standard celestial coordinates ijk */
     ax = cx*cos(tle.xnodeo1) - cy*sin(tle.xnodeo1);
     ay = cx*sin(tle.xnodeo1) + cy*cos(tle.xnodeo1);
     az = cz;

   /* get the range vector */

   Calculate_User_PosVel(jul_utc, &obs_geodetic, &obs_pos, &obs_vel);
   rx = pos.x - obs_pos.x;
   ry = pos.y - obs_pos.y;
   rz = pos.z - obs_pos.z;

   /* cos(squint) is now just minus the inner product of (ax,ay,az) and (rx,ry,rz) normalised */
   sat->_squint=Degrees( acos ( - (ax*rx+ay*ry+az*rz) / sqrt(rx*rx+ry*ry+rz*rz) ));
  }

    if(sat->_squinttype==0) sat->_squint=-1000.0;    // no squint
    if(sat->_squinttype==1) {
      double r = sat->_height + xkmper;
      sat->_squint=Degrees(acos((sat->_range*sat->_range + r*r + - xkmper*xkmper) / (2*sat->_range*r)));
  }

  if(jul_utc>sat->_nextAosLos && doaoslos && hasaoslos) {
    sat->_nextAosLos=nextAosLos(jul_utc, sat, hasaoslos);
    sat->_nextAosLosString=daynum2String(sat->_nextAosLos);
  }
}

bool calculator::aosHappens(tle_t* tle, geodetic_t* obs) {
  bool geo, canreach;
  double lin, sma, apogee;
  // first test if the satellite is geostationary
  if (fabs(tle->xno-1.0027)<0.0002)
    geo=1; else geo=0;

  // does the sat appear on our qth ?

  if (tle->xno==0.0)
    canreach=0;
  else
  {
    lin=tle->xincl;

    if (lin>=90.0)
      lin=180.0-lin;

    sma=331.25*exp(log(1440.0/tle->xno)*(2.0/3.0));
    apogee=sma*(1.0+tle->eo)-xkmper;

    if ((acos(xkmper/(apogee+xkmper))+(lin*de2ra)) > fabs(obs->lat))
      canreach=1;
    else
      canreach=0;
  }
  if (!geo && canreach) return true; else return false;
}

double calculator::FindAOS(double daynum, satellite* sat, bool hasaos) {

  /* This function finds and returns the time of AOS (aostime). */
  double aostime=0.0;

  if (hasaos)
  {
    calc(daynum, sat, false);

    /* Get the satellite in range */

    while (sat->_elevation<-1.0)
    {
      daynum-=0.00035*(sat->_elevation*((sat->_height/8400.0)+0.46)-2.0);
      calc(daynum, sat, false);
    }

    /* Find AOS */

    while (aostime==0.0)
    {
      if (fabs(sat->_elevation)<0.03)
        aostime=daynum;
      else
      {
        daynum-=sat->_elevation*sqrt(sat->_height)/530000.0;
        calc(daynum, sat, false);
      }
    }
  }
//  calc(_daynum, sat, false);
  return aostime;
}

double calculator::FindLOS(double daynum, satellite* sat, bool haslos) {
  double lostime=0.0;
  if (haslos)
  {
    calc(daynum, sat, false);
    do
    {
      daynum+=sat->_elevation*sqrt(sat->_height)/502500.0;
      calc(daynum, sat, false);

      if (fabs(sat->_elevation) < 0.03)
        lostime=daynum;

    } while (lostime==0.0);
  }
//  calc(_daynum, sat, false);
  return lostime;
}

double calculator::FindLOS2(double daynum, satellite* sat, bool haslos)
{
  /* This function steps through the pass to find LOS.
     FindLOS() is called to "fine tune" and return the result. */
  do
  {
    daynum+=cos((sat->_elevation-1.0)*de2ra)*sqrt(sat->_height)/25000.0;
    calc(daynum, sat, haslos);

  } while (sat->_elevation>=0.0);

  return(FindLOS(daynum, sat, haslos));
}

double calculator::NextAOS(double daynum, satellite* sat, bool aoslos)
{
  /* This function finds and returns the time of the next
     AOS for a satellite that is currently in range. */
  if (aoslos)
    daynum=FindLOS2(daynum, sat, aoslos)+0.014;  /* Move to LOS + 20 minutes */
  return (FindAOS(daynum, sat, aoslos));
}

double calculator::nextAosLos(double daynum, satellite* sat, bool aoslos) {
  double result=0.0;
  if (sat->_elevation < 0.03)
    result=FindAOS(daynum, sat, aoslos);
  else
    result=FindLOS2(daynum, sat, aoslos);
  return result;
}

QString calculator::daynum2String(double daynum)
{
  if (daynum==0.0) return "";
  struct tm TM;
  bzero(&TM, sizeof(tm));
  Calendar_Date(daynum, &TM);
  Time_of_Day(daynum, &TM);
  TM.tm_year-=1900;
  TM.tm_mon-=1;
  time_t t = mktime(&TM);
  char* r = ctime(&t);
  r[strlen(r)-1]=0;
  return QString(r);
}

satellite* calculator::copySatellite(satellite* sat) {
  satellite* s = new satellite(sat->name());
  s->line1=sat->line1;
  s->line2=sat->line2;
  s->_alat=sat->_alat;
  s->_alon=sat->_alon;
  s->_calculatedDate=sat->_calculatedDate;
  s->_catnr=sat->_catnr;
  s->_elevation=sat->_elevation;
  s->_azimuth=sat->_azimuth;
  s->_footprint=sat->_footprint;
  s->_height=sat->_height;
  s->_latitude=sat->_latitude;
  s->_longitude=sat->_longitude;
  s->_ma=sat->_ma;
  s->_nextAosLos=sat->_nextAosLos;
  s->_nextAosLosString=sat->_nextAosLosString;
  s->_orbitnum=sat->_orbitnum;
  s->_polled=sat->_polled;
  s->_range=sat->_range;
  s->_range_rate=sat->_range_rate;
  s->_squint=sat->_squint;
  s->_squinttype=sat->_squinttype;
  s->_translist=sat->_translist;
  s->_velocity=sat->_velocity;
  return s;
}
