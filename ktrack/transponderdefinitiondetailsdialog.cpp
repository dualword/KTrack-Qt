/***************************************************************************
                          transponderdefinitiondetailsdialog.cpp  -  description
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

#include <klocale.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qcheckbox.h>

#include "transponderdefinitiondetailsdialog.h"


transponderDefinitionDetailsDialog::transponderDefinitionDetailsDialog(transponder* t, QWidget *parent, const char *name,bool modal, const QString &caption, int buttonMask) : KDialogBase(parent,name,modal,caption,buttonMask) {
  // create the main widget
  mainwidget = new transponderDefinitionDetailsWidget(this);
  setMainWidget(mainwidget);
  setCaption(i18n("Transponder Details"));
  enableButtonSeparator(true);
  // fill in values
  trans = t;
  mainwidget->uplinkFreqEdit->setText(QString::number(trans->uplink(),'f',0));
  mainwidget->downlinkFreqEdit->setText(QString::number(trans->downlink(),'f',0));
  mainwidget->uplinkOffsetEdit->setText(QString::number(trans->updiff(),'f',0));
  mainwidget->downlinkOffsetEdit->setText(QString::number(trans->downdiff(),'f',0));
  mainwidget->reverseCheckBox->setChecked(trans->reverse());
  mainwidget->preampCheckBox->setChecked(trans->preamp());

  if(trans->mode() == MODE_SSB) mainwidget->modeEdit->setCurrentItem(0);
  if(trans->mode() == MODE_FM)  mainwidget->modeEdit->setCurrentItem(1);
  if(trans->mode() == MODE_CW)  mainwidget->modeEdit->setCurrentItem(2);
}
transponderDefinitionDetailsDialog::~transponderDefinitionDetailsDialog(){
}

void transponderDefinitionDetailsDialog::slotOk() {
  trans->setUplink(mainwidget->uplinkFreqEdit->text().toDouble());
  trans->setDownlink(mainwidget->downlinkFreqEdit->text().toDouble());
  trans->setUpdiff(mainwidget->uplinkOffsetEdit->text().toDouble());
  trans->setDowndiff(mainwidget->downlinkOffsetEdit->text().toDouble());
  trans->setReverse(mainwidget->reverseCheckBox->isChecked());
  trans->setPreamp(mainwidget->preampCheckBox->isChecked());

  int i = mainwidget->modeEdit->currentItem();
  if (i==0) trans->setMode(MODE_SSB);
  if (i==1) trans->setMode(MODE_FM);
  if (i==2) trans->setMode(MODE_CW);

  KDialogBase::slotOk();
}
