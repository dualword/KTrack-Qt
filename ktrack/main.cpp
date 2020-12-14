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

//#include <kcmdlineargs.h>
//#include <kaboutdata.h>
#include <QLocale>
#include <QApplication>

#include "ktrack.h"

//static const char *description = I18N_NOOP("Ktrack");
// INSERT A DESCRIPTION FOR YOUR APPLICATION HERE
	
	
//static KCmdLineOptions options[] =
//{
//  { 0, 0, 0 }
//  // INSERT YOUR COMMANDLINE OPTIONS HERE
//};

int main(int argc, char *argv[])
{

//  KAboutData aboutData( "ktrack", I18N_NOOP("KTrack"),
//    VERSION, description, KAboutData::License_GPL,
//    "(c) 2002, 2003 Luc Langehegermann, LX2GT", "The KDE satellite tracking program", 0, "lx2gt@users.sourceforge.net");
//  aboutData.addAuthor("Luc Langehegermann", "Main application programmer", "lx2gt@users.sourceforge.net");
//  aboutData.addAuthor("Neoklis Kyriazis, 5B4AZ", "Porting of Fortran SGP4/SDP4 Routines to C", 0, 0);

    
//  KCmdLineArgs::init( argc, argv, &aboutData );
//  KCmdLineArgs::addCmdLineOptions( options ); // Add our own options.

  QApplication a(argc, argv);
  Ktrack *ktrack = new Ktrack();
//  a.setMainWidget(ktrack);
  ktrack->show();  

  return a.exec();
}
