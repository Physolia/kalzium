/***************************************************************************
 *   Copyright (C) 2005 by Carsten Niehaus                                 *
 *   cniehaus@kde.org                                                      *
 *   
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "spectrumwidget.h"
#include "spectrum.h"

#include <kdebug.h>
#include <math.h>

#include <qcursor.h>

SpectrumWidget::SpectrumWidget( QWidget *parent, const char* name = 0 ) : QWidget( parent, name )
{
	kdDebug() << "SpectrumWidget::SpectrumWidget()" << endl;
	
	startValue = 0;
	endValue = 0;

	m_LMBPointCurrent.setX( -1 );
	m_LMBPointPress.setX( -1 );

	m_realHeight = 200;

	Gamma = 0.8;
	IntensityMax = 255;

	setType( EmissionSpectrum );
};

void SpectrumWidget::paintEvent( QPaintEvent * /*e*/ )
{
	kdDebug() << "SpectrumWidget::paintEvent()" << endl;

	if ( !m_spectrum )
		 return;

	QPainter p;
	p.begin( this );
	p.fillRect( 0, 0, width(), m_realHeight, Qt::black ); 

	paintBands( &p );
	
	drawTickmarks( &p );

	if ( m_LMBPointPress.x() != -1 && m_LMBPointCurrent.x() != -1 )
		drawZoomLine( &p );
}

void SpectrumWidget::drawZoomLine( QPainter* p )
{
	p->setPen( Qt::white );
	p->drawLine( m_LMBPointPress.x(), m_LMBPointPress.y(), m_LMBPointCurrent.x(), m_LMBPointPress.y() );
	p->drawLine( m_LMBPointCurrent.x(), m_LMBPointPress.y()+10, m_LMBPointCurrent.x(), m_LMBPointPress.y()-10 );
	p->drawLine( m_LMBPointPress.x(), m_LMBPointPress.y()+10, m_LMBPointPress.x(), m_LMBPointPress.y()-10 );

}

void SpectrumWidget::paintBands( QPainter* p )
{
	kdDebug() << "Spectrum::paintBands()" << endl;

	if ( m_type == AbsorptionSpectrum )
	{
		for ( double va = startValue; va <= endValue ; va += 0.1 )
		{
			int x = xPos( va );
			p->setPen( linecolor( va ) );
			p->drawLine( x,0,x, m_realHeight+10 );
		}

		p->setPen( Qt::black );
	}

 	int i = 0;	

 	for ( QValueList<Spectrum::band>::Iterator it = m_spectrum->bandlist()->begin();
 			it != m_spectrum->bandlist()->end();
 			++it )
 	{
 		if ( ( *it ).wavelength < startValue || ( *it ).wavelength > endValue )
			continue;
 
 		int x = xPos( ( *it ).wavelength );
 	
 		int temp = 0; // every second item will have a little offset
 
 		if ( i%2 )
 			temp = 35;
 		else
 			temp = 0;
 
 		switch ( m_type )
 		{
			case EmissionSpectrum:
				p->setPen( linecolor( ( *it ).wavelength ) );
                 		p->drawLine( x,0,x, m_realHeight );
                
                 		p->setPen( Qt::black );
               			p->drawLine( x,m_realHeight,x, m_realHeight+10+temp );
				break;
 		
			case AbsorptionSpectrum:
				p->setPen( Qt::black );
 	                	p->drawLine( x,0,x, m_realHeight+10+temp );
				break;
 		}
 		
 		QString text = QString::number( ( *it ).wavelength );
 		p->drawText(0, 0, text);
 
 		i++;
 	}
}

QColor SpectrumWidget::linecolor( double spectrum )
{
        int r,g,b;
        wavelengthToRGB( spectrum, r,g,b );

        QColor c( r,g,b );
        return c;
}


void SpectrumWidget::wavelengthToRGB( double wavelength, int& r, int& g, int& b )
{
	double blue = 0.0, green = 0.0, red = 0.0, factor = 0.0;

	int wavelength_ = ( int ) floor( wavelength );
	if ( wavelength_ < 380 || wavelength_ > 780 )
	{
		//make everything white
		r = g = b = 255;
		return;
	}
	else if ( wavelength_ > 380 && wavelength_ < 439 )
	{
		red = -( wavelength-440 ) / ( 440-380 );
		green = 0.0;
		blue = 1.0;
	
	}
	else if ( wavelength_ > 440 && wavelength_ < 489 )
	{
		red = 0.0;
		green = ( wavelength-440 ) / ( 490-440 );
		blue = 1.0;
	}
	else if ( wavelength_ > 490 && wavelength_ < 509 )
	{
		red = 0.0;
		green = 1.0;
		blue = -( wavelength-510 ) / ( 510-490 );
	}
	else if ( wavelength_ > 510 && wavelength_ < 579 )
	{
		red = ( wavelength-510 ) / ( 580-510 );
		green = 1.0;
		blue = 0.0;
	}
	else if ( wavelength_ > 580 && wavelength_ < 644 )
	{
		red = 1.0;
		green = -( wavelength-645 ) / ( 645-580 );
		blue = 0.0;
	}
	else if ( wavelength_ > 645 && wavelength_ < 780 )
	{
		red = 1.0;
		green = 0.0;
		blue = 0.0;
	}

	if ( wavelength_ > 380 && wavelength_ < 419 )
		factor = 0.3 + 0.7*( wavelength - 380 ) / ( 420 - 380 );
	else if ( wavelength_ > 420 && wavelength_ < 700 )
		factor = 1.0;
	else if ( wavelength_ > 701 && wavelength_ < 780 )
		factor = 0.3 + 0.7*( 780 - wavelength ) / ( 780 - 700 );
	else
		factor = 0.0;

	r = Adjust( red, factor );
	g = Adjust( green, factor );
	b = Adjust( blue, factor );
}

int SpectrumWidget::Adjust( double color, double factor )
{
	if ( color == 0.0 )
		return 0;
	else
		return ( int )( round( IntensityMax * pow( color*factor, Gamma ) ) );
}

void SpectrumWidget::drawTickmarks( QPainter* p )
{
	kdDebug() << "SpectrumWidget::drawTickmarks()" << endl;
	const int space = 13;

	int start = (int)startValue % 10;
	int dist = floor(( width()/(endValue-startValue)) * 10 );	//distance between the tickles in px

	int count = ( int )startValue - start + 10;
	start = floor( (width() / ( endValue-startValue )) * (10-start) );

	if ( dist == 0 || width() < start )
	{
		kdDebug()<< "malformed Values" << endl;
		return;
	}

	for ( int i = start; i < width(); i += dist )
	{
		if(count%50 == 0 )
		{
			p->drawLine( i, m_realHeight, i, m_realHeight+10 );	
			p->fillRect( i-space, m_realHeight+12, 2*space, 15, Qt::white );
			p->drawText( i-space, m_realHeight+12, 2*space, 15, Qt::AlignCenter, QString::number( count ) );		
		}
		else
		{
			p->drawLine( i, m_realHeight, i, m_realHeight+5 );
		}
		count += 10;
	}
}

void SpectrumWidget::keyPressEvent( QKeyEvent *e )
{
	 kdDebug() << "SpectrumWidget::keyPressEvent()" << endl;
	switch ( e->key() )
	{
		case Key_Plus:
		case Key_Equal:
			slotZoomIn();
			break;
		case Key_Minus:
		case Key_Underscore:
			slotZoomOut();
			break;
		case Key_Escape:
			close();
			break;
	}
}

void SpectrumWidget::slotZoomOut()
{}

void SpectrumWidget::slotZoomIn()
{
	kdDebug() << "zoomIn" << endl;
}

void SpectrumWidget::mouseMoveEvent( QMouseEvent *e )
{
	m_LMBPointCurrent = e->pos();
	update();
}

void SpectrumWidget::mousePressEvent(  QMouseEvent *e )
{
	if (  e->button() == QMouseEvent::LeftButton )
		m_LMBPointPress = e->pos();
}

void SpectrumWidget::mouseReleaseEvent(  QMouseEvent *e )
{
	double left = Wavelength( ( double )m_LMBPointPress.x()/width() );
	double right = Wavelength( ( double )e->pos().x()/width() );

	if ( (int)left == (int)right )
		return;
	
	kdDebug() << "left:" << QString::number( left ) << endl;
	kdDebug() << "right:" << QString::number( right ) << endl;	
	if ( left > right )
	{
		setBorders( right, left );
		emit bordersChanged( right, left );
	}
	else
	{
		setBorders( left, right );
		emit bordersChanged( left, right );
	}

	m_LMBPointPress.setX( -1 );
	m_LMBPointCurrent.setX( -1 );
}

#include "spectrumwidget.moc"

