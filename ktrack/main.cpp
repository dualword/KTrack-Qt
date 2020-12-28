/* KTrack-Qt (2020) http://github.com/dualword/KTrack-Qt License:GNU GPL*/
/***************************************************************************
                          main.cpp  -  description
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

#include <QApplication>

#include "ktrack.h"

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	QCoreApplication::setApplicationName("KTrack-Qt");
	QCoreApplication::setOrganizationName("KTrack-Qt");
	QCoreApplication::setOrganizationDomain("KTrack-Qt");
	Ktrack *ktrack = new Ktrack();
	ktrack->show();
	return a.exec();
}
