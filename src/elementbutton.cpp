	/***************************************************************************
	 *   Copyright (C) 2003 by Carsten Niehaus                                 *
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
	 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
	 ***************************************************************************/
#include "elementbutton.h"
#include "prefs.h"
#include "infodialog_small_impl.h"
#include "detailinfodlg.h"
#include <qpainter.h>

#include <kdebug.h>

ElementButton::ElementButton( Element *el, QWidget *parent, const char *name)
 : QFrame(parent, name)
{
	e = el;

	setMaximumSize( 40, 40 );
	setFrameStyle(  QFrame::Box );
	setLineWidth(  2 );
	setMidLineWidth(  3 );
	setFrameShadow(  QFrame::Sunken );
}

void ElementButton::mouseReleaseEvent( QMouseEvent *mouse )
{
	int small = Prefs::lMBbeh();
	if ( small == 1 )
	{
		if ( mouse->button() == LeftButton )
		{
			infoDlgSmallImpl *smallDlg = new infoDlgSmallImpl( e, this , "smallDlg" );
			smallDlg->show();
		}else
		{
			DetailedInfoDlg *detailedDlg = new DetailedInfoDlg( e, this , "detailedDlg" );
			detailedDlg->show();
		}	
	}
	if ( small == 0 )
	{
		if ( mouse->button() == LeftButton )
		{
			DetailedInfoDlg *detailedDlg = new DetailedInfoDlg( e, this , "detailedDlg" );
			detailedDlg->show();
		}else
		{
			infoDlgSmallImpl *smallDlg = new infoDlgSmallImpl( e, this , "smallDlg" );
			smallDlg->show();
		}	
	}
}

ElementButton::~ElementButton()
{
}

/*!
    \fn ElementButton::paintEvent( QPaintEvent* )
 */
void ElementButton::paintEvent( QPaintEvent* )
{
    int h, w;
    h = w = 50;
    QPainter p;
    p.begin( this );
	
	switch ( Prefs::pselook() )
	{
		case 0:
    		p.drawText( 20, 30, e->symbol() );
			p.drawText( 2, h/2-10, QString::number( e->strippedWeight( e->weight()) ) );
			break;
		case 1:
			p.drawText( 20, 30, e->symbol() );
			p.drawText( 2, h/2-10, QString::number( e->number() ) );
			break;
	}
	
	p.drawRect( 0, 0, w, h );
    p.end(); 
}

void ElementButton::mousePressEvent( QMouseEvent* /*e*/ )
{
	    emit num( e->number() );
}

#include "elementbutton.moc"

