/***************************************************************************
                          site.cpp  -  description

                             -------------------
    begin                : Wednesday July 8, 2009
    copyright            : (C) 2009 by Prakash Mohan
    email                : prakash.mohan@kdemail.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "comast/site.h"

void Comast::Site::setSite( QString _name, double _lat, QString _latUnit, double _lon, QString _lonUnit ){
    m_Name = _name;
    m_Lat = _lat;
    m_Lon = _lon;
    m_LatUnit = _latUnit;
    m_LonUnit = _lonUnit;
}
