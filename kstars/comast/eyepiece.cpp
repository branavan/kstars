/***************************************************************************
                          eyepiece.cpp  -  description

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

#include "comast/eyepiece.h"

void Comast::Eyepiece::setEyepiece( QString _name, QString _model, QString _vendor, double _fov, QString _fovUnit, double _focalLength ) {
    m_Name = _name;
    m_Model = _model;
    m_Vendor = _vendor;
    m_AppFovUnit = _fovUnit;
    m_AppFOV = _fov;
    m_FocalLength = _focalLength;
}
