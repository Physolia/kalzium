/*
 * Copyright (C) 2003 Carsten Niehaus <cniehaus@kde.org>
 */

#include "kalzium.h"
#include "pse.h"
#include "elementbutton.h"
#include "element.h"
#include "prefs.h"
#include "settings_colorschemes.h"
#include "settings_colors.h"
#include "settings_quiz.h"
#include "settings_misc.h"
#include "questioneditor_impl.h"

#include <qlabel.h>

#include <kmainwindow.h>
#include <kconfigdialog.h>
#include <klocale.h>
#include <kdebug.h>
#include <kaction.h>
#include <kkeydialog.h>
#include <kedittoolbar.h>
#include <kapplication.h>

Kalzium::Kalzium()
    : KMainWindow( 0, "Kalzium" )
{
	pd = new privatedata( this );

	pd->kalziumData = new KalziumDataObject();


	/*
	 * move this into a method later on
	 **/
	/*
	 * the actions for switching PSE
	 **/
	m_pRegularPSEAction = new KAction(i18n("Show &Regular PSE"), 0, this, SLOT(slotSwitchtoPSE()), actionCollection(), "RegularPSE");
	m_pMendeljevPSEAction = new KAction(i18n("Show &Mendeljev PSE"), 0, this, SLOT(slotSwitchtoPSE()), actionCollection(), "MendeljevPSE");
	m_pSimplePSEAction = new KAction(i18n("Show &Simple PSE"), 0, this, SLOT(slotSwitchtoPSE()), actionCollection(), "SimplePSE");

	/*
	 * the actions for the quiz
	 **/
	m_pQuizStart = new KAction(i18n("Start &Quiz"), 0, this, SLOT(slotStartQuiz()), actionCollection(), "quiz_startquiz");
	m_pQuizSetup = new KAction(i18n("Set&up Quiz"), 0, this, SLOT(showSettingsDialog()), actionCollection(), "quiz_setupquiz");
	m_pQuizEditQuestions = new KAction(i18n("&Edit Questions"), 0, this, SLOT(slotEditQuestions()), actionCollection(), "quiz_editquestions");
	 
	m_pBehAcidAction = new KAction(i18n("Show &Acid Behaviour"), 0, this, SLOT(slotShowScheme(void)), actionCollection(), "view_acidic");
	m_pBehBlocksAction = new KAction(i18n("Show Blocks"), 0, this, SLOT(slotShowScheme(void)), actionCollection(), "view_blocks");
	m_pBehBlocksAction = new KAction(i18n("Show Groups"), 0, this, SLOT(slotShowScheme(void)), actionCollection(), "view_groups");
	m_pBehBlocksAction = new KAction(i18n("Show State Of Matter"), 0, this, SLOT(slotShowScheme(void)), actionCollection(), "view_som");
	m_pBehBlocksAction = new KAction(i18n("No Colors"), 0, this, SLOT(slotShowScheme(void)), actionCollection(), "view_normal");
	
	/*
	 * the standardactions
	 **/
	KStdAction::preferences(this, SLOT(showSettingsDialog()), actionCollection());
	KStdAction::quit( kapp, SLOT (closeAllWindows()),actionCollection() );
	KStdAction::keyBindings(this, SLOT(optionsConfigureKeys()), actionCollection());
	KStdAction::configureToolbars(this, SLOT(optionsConfigureToolbars()), actionCollection());

	m_pRegularPSE = new RegularPSE( data(), this, "regularPSE");
	m_pSimplePSE = new SimplifiedPSE( data(), this, "SimplifiedPSE");
	m_pMendeljevPSE = new MendeljevPSE( data(), this, "MendeljevPSE");

	m_pCurrentPSE = m_pRegularPSE;

	// set the shell's ui resource file
	setXMLFile("kalziumui.rc");
	createGUI();

	setCentralWidget( m_pCurrentPSE );
	
	createStandardStatusBarAction(); //post-KDE 3.1

	m_pCurrentPSE->activateColorScheme( Prefs::colorschemebox() );
}

void Kalzium::slotStartQuiz()
{
	kdDebug() << "insdide the start of the quiz" << endl;
}

void Kalzium::slotEditQuestions()
{
	kdDebug() << "inside the questionseditor" << endl;
	questionEditorImpl *q = new questionEditorImpl( this, "questionEditor" );
	q->show();
}

void Kalzium::slotShowScheme(void)
{
	int i = 0;
	QString n = sender()->name();
	if ( n == QString("view_normal"))
		i = 1;
	if ( n == QString("view_groups")) 
		i = 2;
	if ( n == QString("view_blocks")) 
		i = 3;
	if ( n == QString("view_som")) 
		i = 4;
	if ( n == QString("view_acidic"))
		i = 5;
	
	m_pCurrentPSE->activateColorScheme( i );
}

void Kalzium::slotSwitchtoPSE(void)
{
	m_pRegularPSE->hide();
	m_pSimplePSE->hide();
	m_pMendeljevPSE->hide();

	if( sender()->name() == QString("RegularPSE"))
	{
		m_pCurrentPSE = m_pRegularPSE;
	}
	else if( sender()->name() == QString("MendeljevPSE"))
	{
		m_pCurrentPSE = m_pMendeljevPSE;
	}
	else if( sender()->name() == QString("SimplePSE"))
	{
		m_pCurrentPSE = m_pSimplePSE;
	}

	m_pCurrentPSE->show();
	setCentralWidget( m_pCurrentPSE );
	setCaption( m_pCurrentPSE->name() );
}

PSE* Kalzium::currentPSE() const
{
	return m_pCurrentPSE;
}

void Kalzium::showSettingsDialog()
{
	if (KConfigDialog::showDialog("settings"))
		return;

	//KConfigDialog didn't find an instance of this dialog, so lets create it :
	KConfigDialog *dialog = new KConfigDialog(this,"settings", Prefs::self());
	connect( dialog, SIGNAL( okClicked() ), m_pCurrentPSE, SLOT( slotUpdatePSE() ) );
	connect( dialog, SIGNAL( applyClicked() ), m_pCurrentPSE, SLOT( slotUpdatePSE() ) );
	dialog->addPage( new setColorScheme( 0, "colorscheme_page"), i18n("Configure Default Colorscheme"), "colorize");
	dialog->addPage( new setColors( 0, "colors_page"), i18n("Configure Colors"), "colorize");
	dialog->addPage( new setupQuiz( 0, "quizsetuppage" ), i18n( "Configure Quiz" ), "edit" );
	dialog->addPage( new setupMisc( 0, "miscpage" ), i18n( "Configure Misc" ), "misc" );
	dialog->show();
}

void Kalzium::optionsConfigureKeys()
{
	KKeyDialog::configure(actionCollection());
}

void Kalzium::optionsConfigureToolbars( )
{
	saveMainWindowSettings( KGlobal::config(), autoSaveGroup() );
	KEditToolbar dlg(actionCollection());
	connect(&dlg, SIGNAL(newToolbarConfig()), this, SLOT(newToolbarConfig()));
	dlg.exec();
}

void Kalzium::newToolbarConfig()
{
    createGUI();
    applyMainWindowSettings( KGlobal::config(), autoSaveGroup() );
}


KalziumDataObject* Kalzium::data() const { return pd->kalziumData; }

Kalzium::~Kalzium(){}

#include "kalzium.moc"

