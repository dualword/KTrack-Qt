/* KTrack-Qt (2020-2024) https://github.com/dualword/KTrack-Qt License:GNU GPL*/
/***************************************************************************
                          transponderdefinitiondialog.cpp  -  description
                             -------------------
    begin                : Sat Mar 22 2003
    copyright            : (C) 2003 by Luc Langehegermann
    email                : lx2gt@users.sourceforge.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "transponderdefinitiondialog.h"
#include "transponderdefinitiondetailsdialog.h"

transponderDefinitionDialog::transponderDefinitionDialog(QWidget *parent, const char *name,
		bool modal, Qt::WFlags fl) : QDialog(parent,name,modal,fl) {
	setupUi(this);
  setCaption(tr("Transponder Definition"));
  QObject::connect(satelliteComboBox, SIGNAL(activated(const QString&)), this,SLOT(slotNewSat(const QString&)));
  QObject::connect(deleteButton, SIGNAL(clicked()), this, SLOT(slotDeleteTransponder()));
  QObject::connect(newButton, SIGNAL(clicked()), this, SLOT(slotNewTransponder()));
  QObject::connect(editButton, SIGNAL(clicked()), this, SLOT(slotEditTransponder()));
}

transponderDefinitionDialog::~transponderDefinitionDialog(){
}

void transponderDefinitionDialog::setSatList(PtrSatList* s){
  satlist = s;
  // fill the combo box
  satellite* sat;
  for(auto sat : *satlist) {
    if (sat->polled())
      satelliteComboBox->insertItem(sat->name());
  }
  slotNewSat(satelliteComboBox->currentText());
}

/*!
    \fn transponderDefinitionDialog::slotNewSat(const QString&)
 */
void transponderDefinitionDialog::slotNewSat(const QString& str) {
  // delete the listbox
  transponderListBox->clear();
  //find the appropritate satellite
  for(auto sat : *satlist) {
    if (sat->name() == str){
    	currentSat = sat;
    	break;
    }
  }
  transponder* t;
  QString s;
//  for (t=currentSat->translist()->first(); t!=0; t=currentSat->translist()->next()) {
//    s = QString::number(t->uplink()/1000.0,'f',0) + "/" + QString::number(t->downlink()/1000.0,'f',0);
//    transponderListBox->insertItem(s);
//  }

}

/*!
    \fn transponderDefinitionDialog::slotDeleteTransponder()
 */
void transponderDefinitionDialog::slotDeleteTransponder(){
	int position = transponderListBox->currentRow();
	fprintf(stderr, "Position %i\n", position);
	QListWidgetItem * item = transponderListBox->takeItem(position);
    delete item;
	if (position >= 0) {
		//currentSat->translist()->remove(position);
		//transponderListBox->removeItem(position);
	}
}

/*!
    \fn transponderDefinitionDialog::slotEditTransponder()
 */
void transponderDefinitionDialog::slotEditTransponder()
{
	transponder* tr;
	int position = transponderListBox->currentRow();
	if (position >= 0) {
		//tr=currentSat->translist()->at(position);
		transponderDefinitionDetailsDialog* luc = new transponderDefinitionDetailsDialog(tr, this);
		luc->exec();
	}
}

/*!
    \fn transponderDefinitionDialog::slotNewTransponder()
 */
void transponderDefinitionDialog::slotNewTransponder()
{
	transponder* newtransponder=new transponder();
	transponderDefinitionDetailsDialog* luc = new transponderDefinitionDetailsDialog(newtransponder,this);
	luc->exec();
	//currentSat->translist()->append(newtransponder);
	QString s = QString::number(newtransponder->uplink()/1000.0,'f',0) + "/" + QString::number(newtransponder->downlink()/1000.0,'f',0);
	transponderListBox->addItem(s);
}
