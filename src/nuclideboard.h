#ifndef NUCLIDEBOARD_H
#define NUCLIDEBOARD_H
/***************************************************************************
 *   Copyright (C) 2007 by Carsten Niehaus                                 *
 *   cniehaus@kde.org                                                      *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ***************************************************************************/

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsRectItem>

class IsotopeItem;
class Isotope;

#include "ui_isotopedialog.h"

#include <kdialog.h>

class KActionCollection;
class Element;
class Isotope;
class QPainter;
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QGraphicsTextItem;
class QColor;

/**
 * This class is the drawing widget for the whole table
 *
 * @author Pino Toscano
 * @author Carsten Niehaus
 */
class IsotopeTableDialog : public KDialog
{
    Q_OBJECT

    public:
        explicit IsotopeTableDialog( QWidget* parent = 0 );

    private:
        Ui::isotopeWidget ui;
};

/**
 * The class represtens the items which is drawn on the QGraphicsScene. Each such item represents on
 * Isotope.
 * @author Carsten Niehaus
 */
class IsotopeItem : public QGraphicsRectItem
{
	public:
            /**
             * there are several types of decay for an isotope.
             */
            enum IsotopeType { alpha, ec, multiple, bplus, bminus, stable };

            enum { Type = UserType + 1 };

            /**
             * @param isotope The Isotope represented
             */
            IsotopeItem(Isotope * isotope,  qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent = 0);

            /**
             * @return the Isotope the item represents
             */
            Isotope* isotope() const{
                return m_isotope;
            }

            /**
             * @return the Type of the item
             */
            int type() const{
                return Type;
            }


        private:
            IsotopeType m_type;
            Isotope* m_isotope;

            /**
             * @return the IsotopeType of the Isotope
             */
            IsotopeType getType( Isotope * );

        protected:
            void mousePressEvent(QGraphicsSceneMouseEvent *event);
};


class IsotopeScene : public QGraphicsScene
{
    Q_OBJECT

    public:
        IsotopeScene( QObject * parent = 0);

        void displayContextHelp( IsotopeItem * item );

    private:
        void drawIsotopes();

        //the size of each item
        int m_itemSize;

        QGraphicsTextItem *m_infotext;
        QGraphicsRectItem *m_infoitem;

        QGraphicsItemGroup *m_isotopeGroup;


    public slots:
        void slotSetItemSize(int);

    signals:
        void itemSelected(QGraphicsItem *item);
};

	
#endif // NUCLIDEBOARD_H
