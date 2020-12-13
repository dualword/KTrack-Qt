/***************************************************************************
                          dcopinterface.h  -  description
                             -------------------
    begin                : Tue Aug 20 2002
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

#include <dcopobject.h>

class DCOPInterface : virtual public DCOPObject
{
  K_DCOP
  k_dcop:
  virtual double getUplinkFrequency() = 0;
  virtual double getDownlinkFrequency() = 0;
  virtual QString getMode() = 0;
  virtual QString getSatellite() = 0;
};
