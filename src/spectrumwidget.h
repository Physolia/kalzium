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
#ifndef SPECTRUMWIDGET_H
#define SPECTRUMWIDGET_H

#include <qwidget.h>
#include <kdebug.h>

class Spectrum;
class QMouseEvent;
class QKeyEvent;

/**
 * @author Carsten Niehaus
 * 
 */
class SpectrumWidget : public QWidget
{
	Q_OBJECT

	public:
		SpectrumWidget( QWidget *parent, const char* name );
		
		~SpectrumWidget(){};

		void setSpectrum( Spectrum* spec ){
			m_spectrum = spec;
		}

		/**
		 * This limits the width of the spectrum in terms of
		 * wavelength. For example you can set it to only
		 * show the area between 500 and 550 nm
		 *
		 * @param left the left border
		 * @param right the right border
		 */
		void setBorders( double left, double right ){
			startValue = left;
			endValue = right;
			
			if( endValue-startValue == 0 )
				startValue -= 1;

			update();
		}
		
		/**
		 * find the nearest band. The returnvalue is the number
		 * of pixel the next band is away
		 */
		int findNearestBand( QValueList<double>::iterator it );

		/**
		 * there are several possible types.
		 */
		enum SpectrumType
		{
			EmissionSpectrum = 0,
			AbsorptionSpectrum
		};

		/**
		 * sets the type of the spectrum to @p t
		 * @param t the type of the spectrum
		 */
		void setType( SpectrumType t ){
			m_type = t;
		}

		/**
		 * @return the currently active type
		 * of the spectrum
		 */
		SpectrumType spectrumType() const{
			return m_type;
		}
		
		/**
		 * @returns the color of a line
		 * @param spectrum the value of the spectrum
		 */
		QColor linecolor( double spectrum );
		
		double Gamma;
		int IntensityMax;
		
		/**
		 * @return the adjusted value of the @p color. The
		 * correction depends on @p factor which has been
		 * figured out emperically
		 */
		int Adjust( double color, double factor );
		
		/**
		 * @return the postion in the widget of a band 
		 * with the wavelength @p wavelength
		 * 
		 * @param wavelength the wavelength for which the position is needed
		 */
		inline int xPos( double wavelength ){
			return ( int ) (wavelength * width() / ( endValue - startValue ) );
		}
		
		/**
		 * @param xpos The ratio of the position relative to the width
		 * of the widget.
		 * @return the wavelength on position @p xpos
		 */
		inline double Wavelength( double xpos ){
			return startValue + (  (  endValue-startValue ) *  ( double )xpos );
		}

		/**
		 * This method changes the three values @p r @p g and @p b to the 
		 * correct values
		 * param wavelength the wavelength for which the color is searched
		 * param r red
		 * param g green 
		 * param b blue
		 */
		void wavelengthToRGB( double wavelength, int& r, int& g, int& b );
	
	private:
		QValueList<double> m_spectra;

		SpectrumType m_type;

		Spectrum *m_spectrum;
		
		void paintBands( QPainter* p );
		void drawZoomLine( QPainter* p );
		
		/**
		 * Draw the scale
		 */
		void drawTickmarks( QPainter *p );

		double startValue;
		double endValue;

		int m_realHeight;

		///this QPoint stores the information where
		//the left mouse button has been pressed. This
		//is used for the mouse-zooming
		QPoint m_LMBPointPress;
		
		QPoint m_LMBPointCurrent;

	public slots:
		/**
		 * set the the maximumvalue to @p value
		 */
		void setRightBorder( int value ){
			endValue = value;
			if ( endValue < startValue )
				startValue = endValue-1;
			update();
		}
		
		/**
		 * set the the minimumvalue to @p value
		 */
		void setLeftBorder( int value ){
			startValue = value;
			if ( startValue > endValue )
				endValue = startValue+1;
			update();
		}

		/**
		 * activates the spectrum of the type @p spectrumtype
		 */
		void slotActivateSpectrum( int spectrumtype ){
			m_type = ( SpectrumType )spectrumtype;
			update();
		}

	signals:
		void bordersChanged( int, int );

	private slots:
		void slotZoomIn();
		void slotZoomOut();
	
	protected:
		virtual void paintEvent( QPaintEvent *e );
		virtual void keyPressEvent(QKeyEvent *e);
		virtual void mouseMoveEvent( QMouseEvent *e );
		virtual void mousePressEvent( QMouseEvent *e );
		virtual void mouseReleaseEvent( QMouseEvent *e );
};

#endif // SPECTRUMWIDGET_H
