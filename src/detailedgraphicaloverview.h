#ifndef DETAILEDGRAPHICALOVERVIEW_H
#define DETAILEDGRAPHICALOVERVIEW_H
/***************************************************************************
    copyright            : (C) 2004, 2005, 2006 by Carsten Niehaus
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

#include <QWidget>

class Element;

/**
 * @short The widget which displays the most important information
 *
 * In one widget like a lot people know it from school
 *
 * @author Carsten Niehaus
 */
class DetailedGraphicalOverview : public QWidget
{
	Q_OBJECT

	public:
		/**
		 * Construct a new DetailedGraphicalOverview.
		 *
		 * @param parent the parent of this widget
		 */
		DetailedGraphicalOverview( QWidget *parent );

		/**
		 * Set @p el as the element to be drawn
		 */
		void setElement( int el );

		/**
		 * Set the background color to @p bgColor.
		 */
		void setBackgroundColor( const QColor& bgColor );

	private:
		/**
		 * the element whose data will be used
		 */
		Element *m_element;

		/**
		 * draw the small symbol which symbolises the
		 * relevence for humans
		 */
		void drawBiologicalSymbol( QPainter *p );

		//calculation of the corners
		int x1,x2,y1,y2,h_t;

		/**
		 * The background color.
		 */
		QColor m_backgroundColor;

	protected:
		virtual void paintEvent( QPaintEvent* );
};

#endif // DETAILEDGRAPHICALOVERVIEW_H

