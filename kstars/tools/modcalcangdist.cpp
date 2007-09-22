/***************************************************************************
                          modcalcapcoord.cpp  -  description
                             -------------------
    begin                : Sun May 30 2004
    copyright            : (C) 2004 by Pablo de Vicente
    email                : vicente@oan.es
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "modcalcangdist.h"
#include "modcalcangdist.moc"

#include <QTextStream>

#include <klocale.h>
#include <kfiledialog.h>
#include <kmessagebox.h>

#include "dms.h"
#include "widgets/dmsbox.h"
#include "skypoint.h"
#include "finddialog.h"
#include "kstars.h"

modCalcAngDist::modCalcAngDist(QWidget *parentSplit)
        : QFrame(parentSplit) {

    setupUi(this);
    FirstRA->setDegType(false);
    SecondRA->setDegType(false);

    connect( FirstRA, SIGNAL(editingFinished()), this, SLOT(slotValidatePositions()) );
    connect( FirstDec, SIGNAL(editingFinished()), this, SLOT(slotValidatePositions()) );
    connect( SecondRA, SIGNAL(editingFinished()), this, SLOT(slotValidatePositions()) );
    connect( SecondDec, SIGNAL(editingFinished()), this, SLOT(slotValidatePositions()) );
    connect( FirstRA, SIGNAL(textEdited(QString)), this, SLOT(slotResetTitle()) );
    connect( FirstDec, SIGNAL(textEdited(QString)), this, SLOT(slotResetTitle()) );
    connect( SecondRA, SIGNAL(textEdited(QString)), this, SLOT(slotResetTitle()) );
    connect( SecondDec, SIGNAL(textEdited(QString)), this, SLOT(slotResetTitle()) );
    connect( FirstObjectButton, SIGNAL(clicked()), this, SLOT(slotObjectButton()) );
    connect( SecondObjectButton, SIGNAL(clicked()), this, SLOT(slotObjectButton()) );
    connect( InputButtonBatch, SIGNAL(clicked()), this, SLOT(slotInputFile()) );
    connect( OutputButtonBatch, SIGNAL(clicked()), this, SLOT(slotOutputFile()) );
    connect( runButtonBatch, SIGNAL(clicked()), this, SLOT(slotRunBatch()) );

    show();
    slotValidatePositions();
}

modCalcAngDist::~modCalcAngDist(){
}

SkyPoint modCalcAngDist::getCoords (dmsBox* rBox, dmsBox* dBox, bool *ok) {
    dms raCoord, decCoord;

    bool ok2=false;
    raCoord = rBox->createDms(false, &ok2);
    if ( ok2 )
        decCoord = dBox->createDms(true, &ok2);

    if ( ok2 ) {
        if ( ok ) *ok = ok2;
        return SkyPoint (raCoord, decCoord);
    } else {
        if ( ok ) *ok = ok2;
        return SkyPoint();
    }
}

void modCalcAngDist::slotValidatePositions(){

    SkyPoint sp0,sp1;
    bool ok;
    sp0 = getCoords(FirstRA, FirstDec, &ok);

    if ( ok )
        sp1 = getCoords(SecondRA, SecondDec, &ok);

    if ( ok )
        AngDist->setText( sp0.angularDistanceTo(&sp1).toDMSString() );
    else
        AngDist->setText( " .... " );
}

void modCalcAngDist::slotObjectButton() {
    FindDialog fd( (KStars*)topLevelWidget()->parent() );
    if ( fd.exec() == QDialog::Accepted ) {
        SkyObject *o = fd.selectedObject();
        if ( sender()->objectName() == QString("FirstObjectButton") ) {
            FirstRA->showInHours( o->ra() );
            FirstDec->showInDegrees( o->dec() );
            FirstPositionBox->setTitle( i18n("First position") + ": " + o->name() );
        } else {
            SecondRA->showInHours( o->ra() );
            SecondDec->showInDegrees( o->dec() );
            SecondPositionBox->setTitle( i18n("Second position") + ": " + o->name() );
        }

        slotValidatePositions();
    }
}

void modCalcAngDist::slotResetTitle() {
    QString name = sender()->objectName();
    if ( name.contains( "First" ) )
        FirstPositionBox->setTitle( i18n("First position") );
    else
        SecondPositionBox->setTitle( i18n("Second position") );
}

void modCalcAngDist::slotInputFile() {
    QString inputFileName;
    inputFileName = KFileDialog::getOpenFileName( );
    InputLineEditBatch->setText( inputFileName );
}

void modCalcAngDist::slotOutputFile() {
    QString outputFileName;
    outputFileName = KFileDialog::getSaveFileName( );
    OutputLineEditBatch->setText( outputFileName );
}

void modCalcAngDist::slotRunBatch() {

    QString inputFileName;

    inputFileName = InputLineEditBatch->text();

    // We open the input file and read its content

    if ( QFile::exists(inputFileName) ) {
        QFile f( inputFileName );
        if ( !f.open( QIODevice::ReadOnly) ) {
            QString message = i18n( "Could not open file %1.", f.fileName() );
            KMessageBox::sorry( 0, message, i18n( "Could Not Open File" ) );
            inputFileName = QString();
            return;
        }

        //		processLines(&f);
        QTextStream istream(&f);
        processLines(istream);
        //		readFile( istream );
        f.close();
    } else  {
        QString message = i18n( "Invalid file: %1", inputFileName );
        KMessageBox::sorry( 0, message, i18n( "Invalid file" ) );
        inputFileName = QString();
        InputLineEditBatch->setText( inputFileName );
        return;
    }
}

//void modCalcAngDist::processLines( const QFile * fIn ) {
void modCalcAngDist::processLines( QTextStream &istream ) {

    // we open the output file

    //	QTextStream istream(&fIn);
    QString outputFileName;
    outputFileName = OutputLineEditBatch->text();
    QFile fOut( outputFileName );
    fOut.open(QIODevice::WriteOnly);
    QTextStream ostream(&fOut);

    QString line;
    QString space = " ";
    int i = 0;
    SkyPoint sp0, sp1;
    dms ra0B, dec0B, ra1B, dec1B, dist;

    while ( ! istream.atEnd() ) {
        line = istream.readLine();
        line.trimmed();

        //Go through the line, looking for parameters

        QStringList fields = line.split( " " );

        i = 0;

        // Read RA and write in ostream if corresponds

        if(ra0CheckBatch->isChecked() ) {
            ra0B = dms::fromString( fields[i],false);
            i++;
        } else
            ra0B = ra0BoxBatch->createDms(false);

        if ( allRadioBatch->isChecked() )
            ostream << ra0B.toHMSString() << space;
        else
            if(ra0CheckBatch->isChecked() )
                ostream << ra0B.toHMSString() << space;

        // Read DEC and write in ostream if corresponds

        if(dec0CheckBatch->isChecked() ) {
            dec0B = dms::fromString( fields[i], true);
            i++;
        } else
            dec0B = dec0BoxBatch->createDms();

        if ( allRadioBatch->isChecked() )
            ostream << dec0B.toDMSString() << space;
        else
            if(dec0CheckBatch->isChecked() )
                ostream << dec0B.toDMSString() << space;

        // Read RA and write in ostream if corresponds

        if(ra1CheckBatch->isChecked() ) {
            ra1B = dms::fromString( fields[i],false);
            i++;
        } else
            ra1B = ra1BoxBatch->createDms(false);

        if ( allRadioBatch->isChecked() )
            ostream << ra1B.toHMSString() << space;
        else
            if(ra1CheckBatch->isChecked() )
                ostream << ra1B.toHMSString() << space;

        // Read DEC and write in ostream if corresponds

        if(dec1CheckBatch->isChecked() ) {
            dec1B = dms::fromString( fields[i], true);
            i++;
        } else
            dec1B = dec1BoxBatch->createDms();

        if ( allRadioBatch->isChecked() )
            ostream << dec1B.toDMSString() << space;
        else
            if(dec1CheckBatch->isChecked() )
                ostream << dec1B.toDMSString() << space;

        sp0 = SkyPoint (ra0B, dec0B);
        sp1 = SkyPoint (ra1B, dec1B);
        dist = sp0.angularDistanceTo(&sp1);

        ostream << dist.toDMSString() << endl;
    }

    fOut.close();
}
