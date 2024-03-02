/* KTrack-Qt (2020-2024) https://github.com/dualword/KTrack-Qt License:GNU GPL*/
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
#include "sgp4sdp4/sgp4sdp4.h"

predictionList::predictionList(QWidget *parent, const char *name, Qt::WFlags fl ) :	QDialog(parent,name,fl) {
	setupUi(this);
	QStringList list;
	list << tr("Date") << tr("Longitude") << tr("Latitude") << tr("El") << tr("Az") << tr("Footprint");
	list << tr("Height") << tr("Range") << tr("Velocity") << tr("Orbit Number") << tr("MA") << tr("Squint");
	listView->setColumnCount(list.size());
	listView->setHorizontalHeaderLabels(list);
	QObject::connect(calculateButton, SIGNAL(clicked()), this, SLOT(slotCalculate()));
	QObject::connect(dismissButton, SIGNAL(clicked()), this, SLOT(slotDismiss()));
	calc = new calculator(this);
}

predictionList::~predictionList(){
	 while (!resultlist.isEmpty())
	     delete resultlist.takeFirst();
}

void predictionList::setSatList(PtrSatList* s){
	satnameCombo->clear();
  satlist = s;
  // fill the satellite selection combo box
  for(auto sat : *satlist) {
    if(sat->polled()) satnameCombo->insertItem(sat->name());
  }
  // default values for the times
  startEdit->setDateTime(QDateTime::currentDateTime());
  stopEdit->setDateTime(QDateTime::currentDateTime().addDays(1));
}

void predictionList::slotDismiss(){
  close();
}

void predictionList::slotCalculate(){
	if(satnameCombo->count() <= 0)  return;
	double lastel=0.0;
	satellite* s, *sat;
	struct tm TM;
	time_t t = time(0);
	TM=*gmtime(&t);
	bzero(&TM, sizeof(tm));
	// clear the list view
	listView->clear();
	listView->setSortingEnabled(false); // disable sorting
	resultlist.clear();
	double daynum=qDateTime2daynum(startEdit->dateTime());
	double stopdaynum=qDateTime2daynum(stopEdit->dateTime());
	fprintf(stderr, "Start Time: %s\nStop Time: %s\n", calc->daynum2String(daynum).latin1(), calc->daynum2String(stopdaynum).latin1());

	// get the sat and make a copy!
	for(satellite* s : *satlist) {
		if (s->name() == satnameCombo->currentText()) {
			sat = calc->copySatellite(s);
		  break;
		}
	}

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

void predictionList::displayResults(){
	if (resultlist.size()<=0) return;
	listView->clearContents();
	listView->setRowCount(0);
	QTableWidgetItem * item;

  //  for(s=resultlist.last(); s!=0; s=resultlist.prev()) {
  foreach (satellite* s, resultlist) {
	listView->insertRow(listView->rowCount());
	int row = listView->rowCount()-1;
    item = new QTableWidgetItem(s->calculatedDate());
    listView->setItem(row, 0, item);
    item = new QTableWidgetItem(QString::number(s->longitude(), 'f', 2)+" ");
    listView->setItem(row, 1, item);
    item = new QTableWidgetItem(QString::number(s->latitude(), 'f', 2)+" ");
    listView->setItem(row, 2, item);
    item = new QTableWidgetItem(QString::number(s->elevation(), 'f', 2)+" ");
    listView->setItem(row, 3, item);
    item = new QTableWidgetItem(QString::number(s->azimuth(), 'f', 2)+" ");
    listView->setItem(row, 4, item);
    item = new QTableWidgetItem(QString::number(s->footprint(), 'f', 1)+" ");
    listView->setItem(row, 5, item);
    item = new QTableWidgetItem(QString::number(s->altitude(), 'f', 1)+" ");
    listView->setItem(row, 6, item);
    item = new QTableWidgetItem(QString::number(s->range(), 'f', 1)+" ");
    listView->setItem(row, 7, item);
    item = new QTableWidgetItem(QString::number(s->velocity(), 'f', 3)+" ");
    listView->setItem(row, 8, item);
    item = new QTableWidgetItem(QString::number(s->orbitnum())+" ");
    listView->setItem(row, 9, item);
    item = new QTableWidgetItem(QString::number(s->ma(), 'f', 1)+" ");
    listView->setItem(row, 10, item);

    if (s->squinttype()>0) {
        item = new QTableWidgetItem(QString::number(s->squint(), 'f', 1)+" ");
    } else{
      item = new QTableWidgetItem("--.- ");
    }
    listView->setItem(row, 11, item);

  }
  resultlist.clear();
}
