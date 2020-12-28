/* KTrack-Qt (2020) http://github.com/dualword/KTrack-Qt License:GNU GPL*/
/***************************************************************************
                          mapwidget.cpp  -  description
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

#include "mapwidget.h"

//#include <kstandarddirs.h>
#include <qpainter.h>
#include <qpoint.h>
#include <qbitmap.h>
#include <qtimer.h>

#include "sgp4sdp4/sgp4sdp4.h"

mapWidget::mapWidget(QWidget *parent, const char *name ) : QWidget(parent,name), buffer(100,100) {
  /* load the original map */
  //dayimage.load(locate("appdata", "earth.jpg"));
	dayimage.load("earth.jpg");
  //nightimage.load(locate("appdata", "night.jpg"));
	nightimage.load("night.jpg");
  //setWFlags(Qt::WResizeNoErase|Qt::WRepaintNoErase);
  QTimer* t = new QTimer();
  QObject::connect(t, SIGNAL(timeout()), this, SLOT(updateBackgroundMap()));
  t->start(1000);
}
mapWidget::~mapWidget(){
}

void mapWidget::paintEvent(QPaintEvent*)
{
  bitBlt(this, 0, 0, &buffer);
}

void mapWidget::resizeEvent(QResizeEvent*) {
  QImage i;
  i=dayimage.scaled(width(), height());
  daypixmap.convertFromImage(i);
  i=nightimage.scaled(width(), height());
  nightpixmap.convertFromImage(i);
  updateBackgroundMap();
}

void mapWidget::setObsQth(obsQTH* q) {
  qth=q;
}

void mapWidget::setSatList(PtrSatList* s) {
  satlist = s;
}

void mapWidget::processSatList() {
  buffer = unpaintedmap;
  paintMarker(qth->callsign(), Qt::green, qth->longitude(), qth->latitude());
  for(satellite* sat: *satlist) {
    if (sat->polled()) {
      if (sat == trackingSatellite) {
         paintMarker(sat->name(), Qt::yellow, sat->longitude(), sat->latitude());
         drawArc(sat, Qt::yellow);
      }else if (sat->elevation() > 0.03) {
        paintMarker(sat->name(), Qt::cyan, sat->longitude(), sat->latitude());
        drawArc(sat, Qt::cyan);
      } else {
        paintMarker(sat->name(), Qt::darkGray, sat->longitude(), sat->latitude());
        drawArc(sat, Qt::darkGray);
      }
    }
  }
  repaint(false);
}

int mapWidget::longitude2pixel(double lon) {
  int x;
  lon=360.0-lon;
  if (lon>180.0) {
    lon=lon-180.0;
    x=(int)(width()-(lon*width()/360.0));
  }
  else
    x=(int)((width()/2.0)-(lon*width()/360.0));
  return x;
}

int mapWidget::latitude2pixel(double lat) {
  int x;
  x=(int)((height()/2.0)-(lat*height()/180.0));
  return x;
}

void mapWidget::paintMarker(QString str, QColor color, double lon, double lat) {
  QFont f ("Arial", 12, 99);
  int x = longitude2pixel(lon);
  int y = latitude2pixel(lat);
  QPainter p;
  p.begin(&buffer);
  p.setPen(QPen(color,2));
  p.setFont(f);
  p.drawEllipse(x-3,y-3,6,6);
  p.drawText(x+4, y+6, str);
  p.end();
}

void mapWidget::setTrackingSatellite(satellite* sat) {
  trackingSatellite = sat;
}

double arccos(double x, double y) {
  /* This function implements the arccosine function,
     returning a value between 0 and two pi. */

  double result=0.0, fraction;

  fraction=x/y;

  if ((x>0.0) && (y>0.0))
    result=acos(fraction);

  if ((x<0.0) && (y>0.0))
    result=acos(fraction);

  if ((x<0.0) && (y<0.0))
    result=M_PI+acos(fraction);

  if ((x>0.0) && (y<0.0))
    result=M_PI+acos(fraction);

  return result;
}

void mapWidget::drawArc(satellite* sat, QColor color) {
  double ssplat, ssplong, beta, azimuth, rangelat, rangelong, num, dem;
  int azi;
  int pointsx[360];
  int pointsy[360];
  QPainter p;
    
  beta = (0.5*sat->footprint())/xkmper;
  ssplat=sat->latitude()*de2ra;
  ssplong=sat->longitude()*de2ra;

  for (azi=0; azi<360; azi=azi+1) {
  azimuth=de2ra*(double)azi;
  rangelat=asin(sin(ssplat)*cos(beta)+cos(azimuth)*sin(beta)*cos(ssplat));
  num=cos(beta)-(sin(ssplat)*sin(rangelat));
  dem=cos(ssplat)*cos(rangelat);

  if (azi==0 && (beta > M_PI/2.0-ssplat))
    rangelong=ssplong+M_PI;

  else if (azi==180 && (beta > M_PI/2.0+ssplat))
    rangelong=ssplong+M_PI;

  else if (fabs(num/dem)>1.0)
    rangelong=ssplong;

  else
    {
      if ((180-azi)>=0)
        rangelong=ssplong-arccos(num,dem);
      else
        rangelong=ssplong+arccos(num,dem);
    }

  while (rangelong<0.0)
    rangelong+=M_PI*2.0;

  while (rangelong>(2.0*M_PI))
    rangelong-=M_PI*2.0;

  rangelat=rangelat/de2ra;
  rangelong=rangelong/de2ra;

  pointsx[azi]=longitude2pixel(rangelong);
  pointsy[azi]=latitude2pixel(rangelat);
  }

  p.begin(&buffer);
  p.setPen(QPen(color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  for(azi=0; azi<359; azi=azi+1) {
    if(abs(pointsx[azi] - pointsx[azi+1]) < width()/2 )
      p.drawLine(pointsx[azi], pointsy[azi], pointsx[azi+1], pointsy[azi+1]);
  }
  if(abs(pointsx[0] - pointsx[azi]) < width()/2 )
    p.drawLine(pointsx[0], pointsy[0], pointsx[azi], pointsy[azi]);
  p.end();
}

void mapWidget::updateBackgroundMap(){
  time_t t;
  struct tm *tmp;
  double jt, sunra, sundec, sunrv, sunlong;
  short *wtab;
  QBitmap illuMask(width(), height());
  QBitmap nullbitmap(0,0);
  // calculate the position of the sun
  t = time(NULL);
  tmp = gmtime(&t);
  jt = jtime(tmp);
  sunpos(jt,FALSE, &sunra, &sundec, &sunrv, &sunlong);
  int sec = tmp->tm_hour*60*60 + tmp->tm_min*60 + tmp->tm_sec;
  int gmt_position = width() * sec / 86400; // note: greenwich is in the middle!
  wtab = new short[height()];
  projillum(wtab,width(),height(),sundec);
  illuMask.fill(Qt::black);
  QPainter p;
  p.begin(&illuMask);

  int start, stop;
  int middle = width() - gmt_position;
  for (int y=0; y<height(); y++)
    if (wtab[y]>0)
      {
  start = middle - wtab[y];
  stop = middle + wtab[y];
  if (start < 0)
    {
      p.drawLine(0,y,stop,y);
      p.drawLine(width()+start,y,width(),y);
    }
  else
    if (stop > width())
      {
        p.drawLine(start,y,width(),y);
        p.drawLine(0,y,stop-width(),y);
      }
    else
      p.drawLine(start,y,stop,y);
      }
  p.end();
  delete [] wtab;
  unpaintedmap=nightpixmap;
  p.begin(&unpaintedmap);
  daypixmap.setMask(illuMask);
  p.drawPixmap(0,0,daypixmap);
  p.end();
}

/** routines to calculate the sun */

long mapWidget::jdate(struct tm *t){
	long c, m, y;

	y = t->tm_year + 1900;
	m = t->tm_mon + 1;
	if (m > 2)
	   m = m - 3;
	else {
	   m = m + 9;
	   y--;
	}
	c = y / 100L;		   /* Compute century */
	y -= 100L * c;
	return t->tm_mday + (c * 146097L) / 4 + (y * 1461L) / 4 +
	    (m * 153L + 2) / 5 + 1721119L;
}

/* JTIME --    Convert internal GMT  date  and	time  to  astronomical
	       Julian  time  (i.e.   Julian  date  plus  day fraction,
	       expressed as a double).	*/

double mapWidget::jtime(struct tm* t) {
	return (jdate(t) - 0.5) +
	   (((long) t->tm_sec) +
	     60L * (t->tm_min + 60L * t->tm_hour)) / 86400.0;
}

/*  KEPLER  --	Solve the equation of Kepler.  */

double mapWidget::kepler(double m, double ecc){
	double e, delta;
#define EPSILON 1E-6

	e = m = dtr(m);
	do {
	   delta = e - ecc * sin(e) - m;
	   e -= delta / (1 - ecc * cos(e));
	} while (abs(delta) > EPSILON);
	return e;
}

/*  SUNPOS  --	Calculate position of the Sun.	JD is the Julian  date
		of  the  instant for which the position is desired and
		APPARENT should be nonzero if  the  apparent  position
		(corrected  for  nutation  and aberration) is desired.
                The Sun's co-ordinates are returned  in  RA  and  DEC,
		both  specified  in degrees (divide RA by 15 to obtain
		hours).  The radius vector to the Sun in  astronomical
                units  is returned in RV and the Sun's longitude (true
		or apparent, as desired) is  returned  as  degrees  in
		SLONG.	*/

void mapWidget::sunpos(double jd, int apparent, double *ra, double *dec, double *rv, double *slong) {
	double t, t2, t3, l, m, e, ea, v, theta, omega,
	       eps;

	/* Time, in Julian centuries of 36525 ephemeris days,
	   measured from the epoch 1900 January 0.5 ET. */

	t = (jd - 2415020.0) / 36525.0;
	t2 = t * t;
	t3 = t2 * t;

	/* Geometric mean longitude of the Sun, referred to the
	   mean equinox of the date. */

	l = fixangle(279.69668 + 36000.76892 * t + 0.0003025 * t2);

        /* Sun's mean anomaly. */

	m = fixangle(358.47583 + 35999.04975*t - 0.000150*t2 - 0.0000033*t3);

        /* Eccentricity of the Earth's orbit. */

	e = 0.01675104 - 0.0000418 * t - 0.000000126 * t2;

	/* Eccentric anomaly. */

	ea = kepler(m, e);

	/* True anomaly */

	v = fixangle(2 * rtd(atan(sqrt((1 + e) / (1 - e))  * tan(ea / 2))));

        /* Sun's true longitude. */

	theta = l + v - m;

	/* Obliquity of the ecliptic. */

	eps = 23.452294 - 0.0130125 * t - 0.00000164 * t2 + 0.000000503 * t3;

        /* Corrections for Sun's apparent longitude, if desired. */

	if (apparent) {
	   omega = fixangle(259.18 - 1934.142 * t);
	   theta = theta - 0.00569 - 0.00479 * sin(dtr(omega));
	   eps += 0.00256 * cos(dtr(omega));
	}

        /* Return Sun's longitude and radius vector */

	*slong = theta;
	*rv = (1.0000002 * (1 - e * e)) / (1 + e * cos(dtr(v)));

	/* Determine solar co-ordinates. */

	*ra =
	fixangle(rtd(atan2(cos(dtr(eps)) * sin(dtr(theta)), cos(dtr(theta)))));
	*dec = rtd(asin(sin(dtr(eps)) * sin(dtr(theta))));
}

/*  GMST  --  Calculate Greenwich Mean Siderial Time for a given
	      instant expressed as a Julian date and fraction.	*/

double mapWidget::gmst(double jd){
	double t, theta0;


	/* Time, in Julian centuries of 36525 ephemeris days,
	   measured from the epoch 1900 January 0.5 ET. */

	t = ((floor(jd + 0.5) - 0.5) - 2415020.0) / 36525.0;

	theta0 = 6.6460656 + 2400.051262 * t + 0.00002581 * t * t;

	t = (jd + 0.5) - (floor(jd + 0.5));

	theta0 += (t * 24.0) * 1.002737908;

	theta0 = (theta0 - 24.0 * (floor(theta0 / 24.0)));

	return theta0;
}

/*  PROJILLUM  --  Project illuminated area on the map.  */

void mapWidget::projillum(short *wtab, int xdots, int ydots, double dec){
	int i, ftf = 1, ilon, ilat, lilon, lilat, xt;
	double m, x, y, z, th, lon, lat, s, c;

	/* Clear unoccupied cells in width table */

	for (i = 0; i < ydots; i++)
		wtab[i] = -1;

	/* Build transformation for declination */

	s = sin(-dtr(dec));
	c = cos(-dtr(dec));

	/* Increment over a semicircle of illumination */

	for (th = -(PI / 2); th <= PI / 2 + 0.001;
	    th += PI / TERMINC) {

		/* Transform the point through the declination rotation. */

		x = -s * sin(th);
		y = cos(th);
		z = c * sin(th);

		/* Transform the resulting co-ordinate through the
		   map projection to obtain screen co-ordinates. */

		lon = (y == 0 && x == 0) ? 0.0 : rtd(atan2(y, x));
		lat = rtd(asin(z));

		ilat = (int) (ydots - (lat + 90) * (ydots / 180.0));
		ilon = (int) (lon * (xdots / 360.0));

		if (ftf) {

			/* First time.  Just save start co-ordinate. */

			lilon = ilon;
			lilat = ilat;
			ftf = 0;
		} else {

			/* Trace out the line and set the width table. */

			if (lilat == ilat) {
				wtab[(ydots - 1) - ilat] = ilon == 0 ? 1 : ilon;
			} else {
				m = ((double) (ilon - lilon)) / (ilat - lilat);
				for (i = lilat; i != ilat; i += sgn(ilat - lilat)) {
					xt = (int) (lilon + floor((m * (i - lilat)) + 0.5));
					wtab[(ydots - 1) - i] = xt == 0 ? 1 : xt;
				}
			}
			lilon = ilon;
			lilat = ilat;
		}
	}

	/* Now tweak the widths to generate full illumination for
	   the correct pole. */

	if (dec < 0.0) {
		ilat = ydots - 1;
		lilat = -1;
	} else {
		ilat = 0;
		lilat = 1;
	}

	for (i = ilat; i != ydots / 2; i += lilat) {
		if (wtab[i] != -1) {
			while (1) {
				wtab[i] = xdots / 2;
				if (i == ilat)
					break;
				i -= lilat;
			}
			break;
		}
	}
}

