#ifndef ELEMENTDATAVIEWER_H
#define ELEMENTDATAVIEWER_H
/***************************************************************************

                           elementdataviewer.h  -  description
                             -------------------
    copyright            : (C) 2004 by Carsten Niehaus
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

#include <kdialogbase.h>

typedef QValueList<double> DoubleList;

class Element;
class KalziumDataObject;
class PlotSetupWidget;
class PlotWidget;

/**
 * @short the values of the y-Axis
 * @author Carsten Niehaus
 */
class AxisData
{
	friend class ElementDataViewer;
	
	public: 
		AxisData();
		
		/**
		 * sets the dataList to @p list
		 */
		void setDataList( DoubleList list ){ 
			dataList = list; 
		};
		
		/**
		 * @return the value of the selected dataset of element @p element
		 */
		double value( int element ){
			return *dataList.at( element-1 );
		}

		/**
		 * This represents the nine possible datasets.
		 * @li MASS: the mass of the element
		 * @li MELTINGPOINT: the meanmass of the element
		 */
		enum PAXISDATA{ 
			MASS=0, 
			MEANWEIGHT,
			DENSITY, 
			EN, 
			MELTINGPOINT, 
			BOILINGPOINT, 
			ATOMICRADIUS,
			COVALENTRADIUS
		};

		/**
		 * @return the currently selected datatype
		 * @see AxisData::PAXISDATA
		 */
		int currentDataType(){
		       return m_currentDataType;
		}

		int numberOfElements() const{
			return dataList.count();
		}

	private:
		/**
		 * the dataList contains the values off all elements
		 * but only of the currently selected datatyp. This
		 * means that it eg contains all boilingpoints
		 */
		DoubleList dataList;

		int m_currentDataType;
};

/**
 * @short This widget shows the plot and the widget
 * where you can setup the plot
 * @author Carsten Niehaus
 */
class ElementDataViewer : public KDialogBase
{
	Q_OBJECT

	public:
		ElementDataViewer( KalziumDataObject *data, QWidget *parent=0 , const char *name =0 );

		/**
		 * the AxixData for the y-Axis
		 */
		AxisData *yData;

	public slots:
		void slotZoomIn();
		void slotZoomOut();

		/**
		 * draws the plot
		 */
		void drawPlot();

		void slotUser1();

	protected:
		virtual void paintEvent(QPaintEvent*);
		virtual void keyPressEvent(QKeyEvent *e);

	private: 
		PlotWidget         *m_pPlotWidget;
		PlotSetupWidget    *m_pPlotSetupWidget;

		KalziumDataObject  *d;

		QStringList         names;

		void initData();
		void setupAxisData();
		
		void setLimits(int, int);

};

#endif // ELEMENTDATAVIEWER_H

