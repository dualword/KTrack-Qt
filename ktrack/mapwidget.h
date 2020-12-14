/* KTrack-Qt (2020) http://github.com/dualword/KTrack-Qt License:GNU GPL*/
/***************************************************************************
                          mapwidget.h  -  description
                             -------------------
    begin                : Mon Jan 5 2004
    copyright            : (C) 2004 by Luc Langehegermann
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

#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <qpixmap.h>
#include <qimage.h>
#include <qlist.h>
#include <qwidget.h>
#include <qcolor.h>
                                             
#include "globals.h"
#include "satellite.h"

#ifndef PI
#define PI 3.14159265358979323846
#endif

#ifndef E
#define E 2.7182818284590452354
#endif

#define abs(x) ((x) < 0 ? (-(x)) : x)			  /* Absolute value */
#define sgn(x) (((x) < 0) ? -1 : ((x) > 0 ? 1 : 0))	  /* Extract sign */
#define dtr(x) ((x) * (PI / 180.0))			  /* Degree->Radian */
#define rtd(x) ((x) / (PI / 180.0))			  /* Radian->Degree */
#define fixangle(a) ((a) - 360.0 * (floor((a) / 360.0)))  /* Fix angle	  */

#define TERMINC  100		   /* Circle segments for terminator */

#define PROJINT  (60 * 10)	   /* Frequency of seasonal recalculation */

/**
  *@author Luc Langehegermann
  */

class mapWidget : public QWidget  {
   Q_OBJECT

public:
  mapWidget(QWidget *parent=0, const char *name=0);
  ~mapWidget();
  void setObsQth(obsQTH* q);
  void setSatList(QList<satellite*>* s);
  void setTrackingSatellite(satellite* sat);

public slots:
  void processSatList();

private:
  QImage dayimage, nightimage;
  QPixmap unpaintedmap, daypixmap, nightpixmap;
  QPixmap buffer;
  obsQTH* qth;
  satellite* trackingSatellite;
  QList<satellite*>* satlist;

  void paintEvent(QPaintEvent*);
  void resizeEvent(QResizeEvent*);
  void paintMarker(QString str, QColor color, double lon, double lat);
  int latitude2pixel(double lat);
  int longitude2pixel(double lat);
  void drawArc(satellite* sat, QColor color);
  double jtime(struct tm *t);
  double kepler(double m, double ecc);
  void sunpos(double jd, int apparent, double *ra, double *dec, double *rv, double *slong);
  void projillum(short *wtab, int xdots, int ydots, double dec);
	long jdate(struct tm *t);
	double gmst(double jd);

private slots: // Private slots
  void updateBackgroundMap();
};

#endif
