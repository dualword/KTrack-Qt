/* KTrack-Qt (2020) http://github.com/dualword/KTrack-Qt License:GNU GPL*/
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

#include "transponderdefinitiondetailsdialog.h"

transponderDefinitionDetailsDialog::transponderDefinitionDetailsDialog(transponder* t, QWidget *p,
		Qt::WindowFlags fl) : QDialog(p,fl) {
	setupUi(this);
	// create the main widget
	setWindowTitle(tr("Transponder Details"));
	// fill in values
	trans = t;
	uplinkFreqEdit->setText(QString::number(trans->uplink(),'f',0));
	downlinkFreqEdit->setText(QString::number(trans->downlink(),'f',0));
	uplinkOffsetEdit->setText(QString::number(trans->updiff(),'f',0));
	downlinkOffsetEdit->setText(QString::number(trans->downdiff(),'f',0));
	reverseCheckBox->setChecked(trans->reverse());
	preampCheckBox->setChecked(trans->preamp());

	if(trans->mode() == MODE_SSB) modeEdit->setCurrentIndex(0);
	if(trans->mode() == MODE_FM)  modeEdit->setCurrentIndex(1);
	if(trans->mode() == MODE_CW)  modeEdit->setCurrentIndex(2);
}

transponderDefinitionDetailsDialog::~transponderDefinitionDetailsDialog(){
}

void transponderDefinitionDetailsDialog::slotOk(){
	trans->setUplink(uplinkFreqEdit->text().toDouble());
	trans->setDownlink(downlinkFreqEdit->text().toDouble());
	trans->setUpdiff(uplinkOffsetEdit->text().toDouble());
	trans->setDowndiff(downlinkOffsetEdit->text().toDouble());
	trans->setReverse(reverseCheckBox->isChecked());
	trans->setPreamp(preampCheckBox->isChecked());

	int i = modeEdit->currentIndex();
	if (i==0) trans->setMode(MODE_SSB);
	if (i==1) trans->setMode(MODE_FM);
	if (i==2) trans->setMode(MODE_CW);

	//KDialogBase::slotOk();
}
