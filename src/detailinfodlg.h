/***************************************************************************

        detailedinfodlg.h  -  description
                             -------------------
    begin                : Tue Apr 2 20:43:44 2002 UTC
    copyright            : (C) 2003, 2004 by Carsten Niehaus
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

#ifndef _DETAILINFODLG_H_
#define _DETAILINFODLG_H_

#include <kdialogbase.h>
#include "element.h"

class QFrame;
class DetailedGraphicalOverview;



/**
 * @short The dialog which shows all availeble information
 * @author Carsten Niehaus
 */
class DetailedInfoDlg : public KDialogBase
{
    Q_OBJECT
    
	public:
        	DetailedInfoDlg( Element *el , QWidget *parent=0, const char *name=0);
	
	private:
		Element *e;
		QFrame *m_pEnergyTab,
			*m_pOverviewTab,
			*m_pPictureTab,
			*m_pChemicalTab,
			*m_pMiscTab,
			*m_pModelTab;

		DetailedGraphicalOverview *dTab;

};
#endif
