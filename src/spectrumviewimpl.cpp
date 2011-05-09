/***************************************************************************
    copyright            : (C) 2005, 2006 by Carsten Niehaus
    email                : cniehaus@kde.org
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "spectrumviewimpl.h"

#include <QTreeWidget>
#include <QTableWidget>

#include <kunitconversion/converter.h>
#include <kdebug.h>
#include "kalziumdataobject.h"
#include <klocalizedstring.h>
#include "prefs.h"

SpectrumViewImpl::SpectrumViewImpl( QWidget *parent )
        : QWidget( parent )
{
    setupUi( this );

    connect( minimumValue, SIGNAL( valueChanged( int ) ),
             m_spectrumWidget, SLOT( setLeftBorder( int ) ) );
    connect( maximumValue, SIGNAL( valueChanged( int ) ),
             m_spectrumWidget, SLOT( setRightBorder( int ) ) );
    connect( m_spectrumWidget, SIGNAL( bordersChanged(int,int) ),
             this, SLOT( updateUI(int,int) ) );
    connect( m_spectrumWidget, SIGNAL(peakSelected(Spectrum::peak*)),
             this, SLOT(updatePeakInformation(Spectrum::peak*)));

    connect( m_spectrumType, SIGNAL( currentIndexChanged( int ) ),
             m_spectrumWidget, SLOT( slotActivateSpectrum( int ) ));
    connect( m_lengthUnit, SIGNAL( currentIndexChanged( int ) ),
             this, SLOT( setUnit() ));

    QStringList headers = QStringList() << i18n("Wavelength") << i18n("Intensity");
    peakListTable->setHeaderLabels(headers);
    peakListTable->setRootIsDecorated(false);

    QList<int> length;
    length << KUnitConversion::Nanometer << KUnitConversion::Angstrom;
    m_lengthUnit->setUnitList(length);

    m_lengthUnit->setIndexWithUnitId( Prefs::spectrumWavelengthUnit() );

    m_spectrumType->setCurrentIndex( Prefs::spectrumType() );

    resize( minimumSizeHint() );
}

void SpectrumViewImpl::fillPeakList()
{
    peakListTable->clear();

    QList<QTreeWidgetItem *> items;

    for (int i = 0; i <  m_spectrumWidget->spectrum()->peaklist().count(); i++ )
    {
        Spectrum::peak * peak = m_spectrumWidget->spectrum()->peaklist().at(i);

        double peakWavelength = peak->wavelengthToUnit( Prefs::spectrumWavelengthUnit() );

        QStringList row = QStringList() << QString::number(peakWavelength)
                          << QString::number(peak->intensity);

        items.append(new QTreeWidgetItem((QTreeWidget*)0, row));
    }
    peakListTable->insertTopLevelItems(0, items);
}

void SpectrumViewImpl::updateUI(int l, int r)
{
    minimumValue->setValue(l);
    maximumValue->setValue(r);
    minimumValue->setSuffix( KalziumDataObject::instance()->unitAsString( Prefs::spectrumWavelengthUnit() ) );
    maximumValue->setSuffix( KalziumDataObject::instance()->unitAsString( Prefs::spectrumWavelengthUnit() ) );
}

void SpectrumViewImpl::updatePeakInformation(Spectrum::peak * peak )
{
    double peakWavelength = peak->wavelengthToUnit( Prefs::spectrumWavelengthUnit() );

    QList<QTreeWidgetItem *> foundItems = peakListTable->findItems( QString::number(peakWavelength) , Qt::MatchExactly );

    if ( foundItems.isEmpty() )
        return;

    foreach( QTreeWidgetItem* item, peakListTable->selectedItems() )
    item->setSelected( false );

    foundItems.first()->setSelected( true );
    peakListTable->scrollToItem( foundItems.first() );
}

void SpectrumViewImpl::setUnit()
{
    Prefs::setSpectrumWavelengthUnit( m_lengthUnit->getCurrentUnitId() );
    Prefs::self()->writeConfig();
    qDebug() << "Unit changed: " << m_lengthUnit->getCurrentUnitId();

//     fillPeakList();
//     minimumValue->setValue( m_spectrumWidget->spectrum()->minPeak( Prefs::spectrumWavelengthUnit() ) );

    m_spectrumWidget->update();

}


#include "spectrumviewimpl.moc"
