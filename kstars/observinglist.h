/***************************************************************************
                          observinglist.h  -  K Desktop Planetarium
                             -------------------
    begin                : 29 Nov 2004
    copyright            : (C) 2004 by Jeff Woods, Jason Harris
    email                : jcwoods@bellsouth.net, jharris@30doradus.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef OBSERVINGLIST_H
#define OBSERVINGLIST_H

#include <kdialogbase.h>
#include "skyobject.h"

class KStars;
class ObservingListUI;

/**@class ObservingList
  *Tool window for managing a custom list of objects.  The window
  *displays the Name, RA, Dec, mag, and type of each object in the list.
  *
  *By selecting an object in the list, you can perform a number of functions
  *on that object:
  *+ Center it in the display (TBD)
  *+ Examine its Details Window (TBD)
  *+ Point the telescope at it (TBD)
  *+ Attach a custom icon or name label (TBD)
  *+ Attach a trail (solar system only) (TBD)
  *+ Open the AltVsTime tool (TBD)
  *
  *The user can also save/load their observing lists, and can export 
  *list data (TBD: as HTML table?  CSV format?  plain text?)
  *
  *The observing notes associated with the selected object are displayed 
  *below the list. (TBD)
  *
  *TODO: 
  *+ Implement a "shaded" state, in which the UI is compressed to
  *  make it easier to float on the KStars window.  Displays only
  *  object names, and single-letter action buttons, and no user log.
  *+ Implement an InfoBox version (the ultimate shaded state)
  *
  *@short Tool for managing a custom list of objects
  *@author Jeff Woods, Jason Harris
  *@version 1.0
  */

class ObservingList : public KDialogBase
{
  Q_OBJECT

public:
/**@short Cunstructor
  */
  ObservingList( KStars *_ks, QWidget* parent = 0 );
/**@short Destuctor (empty)
  */
  ~ObservingList() {}

/**@return true if the object is in the observing list
  *@p o pointer to the object to test.
  */
  bool contains( const SkyObject *o );

public slots:
/**@short add a new object to list
  *@p o pointer to the object to add to the list
  */
  void slotAddObject( SkyObject *o=NULL );
 
/**@short Remove a skyobject from the observing list.
  *@p o pointer to the object to be removed.
  *If no arg given, remove the currently-selected item in the table. 
  */
  void slotRemoveObject( const SkyObject *o=NULL );     

/**@short Remove a skyobject from the observing list.  
  *Behaves essentially like the above function.
  *@p name the name of the object to be removed
  */
  void slotRemoveObject( const QString &name );     

/**@short center the selected object in the display
  */
  void slotCenterObject();

/**@short slew the telescope to the selected object
  */
  void slotSlewToObject();

/**@short Show the details window for the selected object
  */
  void slotDetails();

/**@short Show the details window for the selected object
  */
  void slotAVT();

/**@short Tasks needed when changing the selected object
  *Save the user log of the previous selected object, 
  *find the new selected object in the obsList, and 
  *show the notes associated with the new selected object
  */
  void slotNewSelection( QListViewItem* );

protected slots:
  void slotClose();

private:
  KStars *ks;
  ObservingListUI *ui;
  QPtrList<SkyObject> obsList;

};

#endif // OBSERVINGLIST_H
