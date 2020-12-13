/***************************************************************************
                          predictionlist.cpp  -  description
                             -------------------
    begin                : Wed Aug 7 2002
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

#include "predictionlist.h"

#include <qcombobox.h>
#include <qdatetimeedit.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qlistview.h>
#include <kapplication.h>
#include <klocale.h>

#include "sgp4sdp4/sgp4sdp4.h"

predictionList::predictionList(QWidget *parent, const char *name, WFlags fl ) : predictionListBase(parent,name,fl) {
  listView->setAllColumnsShowFocus(true);
  listView->addColumn(i18n("Date"));
  listView->addColumn(i18n("Longitude"));
  listView->addColumn(i18n("Latitude"));
  listView->addColumn(i18n("El"));
  listView->addColumn(i18n("Az"));
  listView->addColumn(i18n("Footprint"));
  listView->addColumn(i18n("Height"));
  listView->addColumn(i18n("Range"));
  listView->addColumn(i18n("Velocity"));
  listView->addColumn(i18n("Orbit Number"));
  listView->addColumn(i18n("MA"));
  listView->addColumn(i18n("Squint"));

  listView->setColumnAlignment(1,AlignRight);
  listView->setColumnAlignment(2,AlignRight);
  listView->setColumnAlignment(3,AlignRight);
  listView->setColumnAlignment(4,AlignRight);
  listView->setColumnAlignment(5,AlignRight);
  listView->setColumnAlignment(8,AlignRight);
  listView->setColumnAlignment(7,AlignRight);
  listView->setColumnAlignment(6,AlignRight);
  listView->setColumnAlignment(9,AlignRight);
  listView->setColumnAlignment(9,AlignRight);
  listView->setColumnAlignment(10,AlignRight);
  listView->setColumnAlignment(11,AlignRight);
  listView->setColumnAlignment(12,AlignRight);  

  resultlist.setAutoDelete(true);
  calc = new calculator(this);
}
predictionList::~predictionList(){
}
/** No descriptions */
void predictionList::setSatList(QList<satellite> s)
{
  satlist=s;
  // fill the satellite selection combo box
  satellite* sat;
  for(sat=satlist.first(); sat!=0; sat=satlist.next()) {
    if(sat->polled()) satnameCombo->insertItem(sat->name());
  }
  // default values for the times
  startEdit->setDateTime(QDateTime::currentDateTime());
  stopEdit->setDateTime(QDateTime::currentDateTime().addDays(1));

  // connect the push buttons

  QObject::connect(calculateButton, SIGNAL(clicked()), this, SLOT(slotCalculate()));
  QObject::connect(dismissButton, SIGNAL(clicked()), this, SLOT(slotDismiss()));
}
/** No descriptions */
void predictionList::slotDismiss(){
  close();
}
/** No descriptions */
void predictionList::slotCalculate(){
  double lastel=0.0;
  satellite* s, *sat;
  struct tm TM;
  time_t t = time(0);
  TM=*gmtime(&t);
  bzero(&TM, sizeof(tm));
  // clear the list view
  listView->clear();
  listView->setSorting(-1); // disable sorting
  resultlist.clear();
  double daynum=qDateTime2daynum(startEdit->dateTime());
  double stopdaynum=qDateTime2daynum(stopEdit->dateTime());
  fprintf(stderr, "Start Time: %s\nStop Time: %s\n", calc->daynum2String(daynum).latin1(), calc->daynum2String(stopdaynum).latin1());

    
  // get the sat and make a copy!

  for(s=satlist.first(); s!=0; s=satlist.next()) {
    if (s->name() == satnameCombo->currentText()) {
      break;
    }
  }

  sat = calc->copySatellite(s);
  calc->calc(daynum, sat, false);  
  for(;;) {
    daynum=calc->FindAOS(daynum, sat, true);
    while (sat->elevation()>=0 /*&& daynum < stopdaynum*/) {
      resultlist.append(calc->copySatellite(sat));
      lastel=sat->elevation();
      daynum+=cos((sat->elevation()-1.0)*de2ra)*sqrt(sat->altitude())/25000.0;
      calc->calc(daynum, sat, false);
    }
    if (lastel!=0.0) {
      daynum=calc->FindLOS(daynum, sat, true);
      calc->calc(daynum, sat, false);
      resultlist.append(calc->copySatellite(sat));
    }
    daynum=calc->NextAOS(daynum, sat, true);
    if (daynum>stopdaynum) break;
  }
  displayResults();
}

/** No descriptions */
double predictionList::qDateTime2daynum(QDateTime date){
  struct tm TM;
  extern int daylight;
  tzset();

  TM.tm_sec=date.time().second();
  TM.tm_min=date.time().minute();
  TM.tm_hour=date.time().hour();
  TM.tm_mday=date.date().day();
  TM.tm_mon=date.date().month()-1;
  TM.tm_year=date.date().year()-1900;
  TM.tm_isdst=daylight;
  time_t t = mktime(&TM);

//  gmtime_r(&t, &TM);
  localtime_r(&t, &TM);
  TM.tm_year+=1900;
  TM.tm_mon+=1;
  return Julian_Date(&TM);
}

void predictionList::setQTH(obsQTH* qth) {
  calc->setObsQTH(qth);
}
/** No descriptions */
void predictionList::displayResults(){
  satellite* s;
  QListViewItem* item;
  for(s=resultlist.last(); s!=0; s=resultlist.prev()) {
    item = new QListViewItem(listView);
    item->setText(0, s->calculatedDate());
    item->setText(1, QString::number(s->longitude(), 'f', 2)+" ");
    item->setText(2, QString::number(s->latitude(), 'f', 2)+" ");
    item->setText(3, QString::number(s->elevation(), 'f', 2)+" ");
    item->setText(4, QString::number(s->azimuth(), 'f', 2)+" ");
    item->setText(5, QString::number(s->footprint(), 'f', 1)+" ");
    item->setText(6, QString::number(s->altitude(), 'f', 1)+" ");
    item->setText(7, QString::number(s->range(), 'f', 1)+" ");
    item->setText(8, QString::number(s->velocity(), 'f', 3)+" ");
    item->setText(9, QString::number(s->orbitnum())+" ");
    item->setText(10, QString::number(s->ma(), 'f', 1)+" ");
    if (s->squinttype()>0)
      item->setText(11, QString::number(s->squint(), 'f', 1)+" ");
    else
      item->setText(11, "--.- ");
  }
  resultlist.clear();
}
